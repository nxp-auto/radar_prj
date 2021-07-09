/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_comm.h>
#include <oal_log.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <os_oal_comm.h>
#include <os_oal_comm_kernel.h>
#include <posix_oal_static_memory_pool.h>

#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>

#define MAX_ALLOCA_SPACE 1024U

static struct OS_OAL_DriverHandle
    gsWrDriverHandles[OAL_MAX_DRIVER_HANDLES_PER_PROCESS];
static OAL_DECLARE_STATIC_POOL(gsWrDriverHandlesPool,
                               OAL_ARRAY_SIZE(gsWrDriverHandles));

OAL_DriverHandle_t OAL_OpenDriver(const char8_t *acpChannelName)
{
	int32_t lRet = 0;
	size_t lIntRet;
	OAL_DriverHandle_t lHandle = NULL;
	size_t lPathLen;

	if (acpChannelName == NULL) {
		goto open_driver_end;
	}

	lPathLen = strnlen(acpChannelName, OAL_MAX_DRIVER_PATH_LEN) +
	           sizeof(OAL_LINUX_DEV_PREFIX);

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&gsWrDriverHandlesPool,
	                                gsWrDriverHandles, &lHandle);
	if ((lHandle == NULL) || (lRet != 0)) {
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in driver handles pool. Please adjust its size.\n");
		goto open_driver_end;
	}

	if (strnlen(acpChannelName, OAL_MAX_DRIVER_PATH_LEN) ==
	    OAL_MAX_DRIVER_PATH_LEN) {
		OAL_LOG_WARNING(
		    "Driver's path will be truncated because it's "
		    "too long : (%s)\n",
		    acpChannelName);
	}

	lIntRet = (size_t)snprintf(lHandle->mPath, sizeof(lHandle->mPath),
	                           OAL_LINUX_DEV_PREFIX "%s", acpChannelName);
	if (lIntRet != (lPathLen - 1U)) {
		lRet = -1;
		OAL_LOG_ERROR("Failed to assembly driver's path -%s-\n",
		              lHandle->mPath);
		goto free_meta_data;
	}

	lHandle->mFd = open(lHandle->mPath, O_RDWR);

	if (lHandle->mFd == -1) {
		lRet = -1;
		OAL_LOG_ERROR("Device %s couldn't be opened!\n",
		              lHandle->mPath);
	}

	/* Initialize events pool */
	OAL_SET_POOL_SIZE(&lHandle->mEventsPool,
	                  OAL_ARRAY_SIZE(lHandle->mEvents));
	OAL_INITIALIZE_POOL(&lHandle->mEventsPool);

free_meta_data:
	if (lRet != 0) {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsWrDriverHandlesPool,
		                                 gsWrDriverHandles, lHandle);
		lHandle = NULL;
	}

open_driver_end:
	return lHandle;
}

int32_t OAL_CloseDriver(OAL_DriverHandle_t *apHandle)
{
	int32_t lRet = -1;

	if (apHandle != NULL) {
		if (*apHandle != NULL) {
			lRet = close((*apHandle)->mFd);
			(void)OAL_RELEASE_ELEM_FROM_POOL(&gsWrDriverHandlesPool,
			                                 gsWrDriverHandles,
			                                 *apHandle);
			*apHandle = NULL;
		}
	}

	return lRet;
}

static void serialize_arguments(uint8_t *apDest, OAL_FuncArgs_t *apInArgs,
                                size_t aNumInArgs)
{
	size_t lIdx;
	uint8_t *lpDest = apDest;
	void *lpVDst, *lpVSrc;

	for (lIdx = 0U; lIdx < aNumInArgs; lIdx++) {
		lpVDst = (void *)lpDest;
		lpVSrc = (void *)apInArgs[lIdx].mpData;
		(void)memcpy(lpVDst, lpVSrc, apInArgs[lIdx].mSize);
		lpDest += apInArgs[lIdx].mSize;
	}
}

uint32_t OAL_DriverCall(OAL_DriverHandle_t aHandle, uint32_t aFuncID,
                        OAL_FuncArgs_t *apInArgs, size_t aNumInArgs,
                        OAL_FuncArgs_t *apOutArgs, size_t aNumOutArgs)
{
	struct comm_args lCommArguments;
	size_t lInSize = 0U;
	ssize_t lWriteRet;
	size_t lIdx;

	int32_t lStatus;
	uint8_t *lpAllocPtr = NULL;

	void *lpDest  = NULL;
	uint32_t lRet = 0U;

	if ((aHandle == NULL) || (aHandle->mFd == -1)) {
		OAL_LOG_ERROR("Invalid handler!\n");
		lRet = (uint32_t)EINVAL;
		goto driver_call_exit;
	}

	for (lIdx = 0U; lIdx < aNumInArgs; lIdx++) {
		if (apInArgs[lIdx].mpData == NULL) {
			OAL_LOG_ERROR("Invalid argument\n");
			lRet = (uint32_t)EINVAL;
			break;
		}
	}

	if (lRet == (uint32_t)EINVAL) {
		goto driver_call_exit;
	}

	lInSize = OAL_GetArgumentsBufferSize(apInArgs, aNumInArgs);

	/* buffer size + return size */
	lStatus = OAL_AllocFromMemoryPool(lInSize, &lpAllocPtr);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to allocate memory for communication\n");
		lRet = (uint32_t)ENOMEM;
		goto driver_call_end;
	}

	lpDest = (void *)(uintptr_t)lpAllocPtr;
	if ((lpDest == NULL) && (lInSize != 0U)) {
		lRet = (uint32_t)ENOMEM;
		OAL_LOG_ERROR("Could not allocate memory!\n");
		goto driver_call_end;
	}

	if ((apInArgs != NULL) && (aNumInArgs > 0U)) {
		serialize_arguments(lpDest, apInArgs, aNumInArgs);
	}

	lCommArguments.mFuncId       = aFuncID;
	lCommArguments.mpInArgBuff   = lpDest;
	lCommArguments.mInArgBuffLen = lInSize;
	lCommArguments.mpOutBuff     = apOutArgs;
	lCommArguments.mNumOutArgs   = aNumOutArgs;
	lCommArguments.mpRet         = &lRet;

	lWriteRet =
	    write(aHandle->mFd, &lCommArguments, sizeof(lCommArguments));
	if (lWriteRet != (ssize_t)sizeof(lCommArguments)) {
		lRet = (uint32_t)EIO;
		if (lWriteRet < 0) {
			OAL_LOG_ERROR("Failed to write argumtens' address\n");
		} else {
			OAL_LOG_ERROR("Passed only %zd from %zu bytes\n",
			              lWriteRet, sizeof(lCommArguments));
		}
	}

driver_call_end:
	if (lpAllocPtr != NULL) {
		lStatus = OAL_ReleasePoolMemory(lInSize, lpAllocPtr);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed to release memory\n");
			lRet = (uint32_t)ENOMEM;
		}
	}
driver_call_exit:
	return lRet;
}

int32_t OAL_SubscribeToEvent(OAL_DriverHandle_t aHandle, uint32_t aEventId,
                             OAL_DriverEvent_t *apEvent)
{
	int32_t lRet = 0;
	size_t lPathLen;
	size_t lBytes;
	uint8_t lDigits = OAL_GetNumDigits(aEventId);
	OAL_DriverEvent_t lEvent;

	OAL_CHECK_NULL_PARAM(aHandle, event_subscription_exit);
	OAL_CHECK_NULL_PARAM(apEvent, event_subscription_exit);

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&aHandle->mEventsPool, aHandle->mEvents,
	                                &lEvent);
	if ((lEvent == NULL) || (lRet != 0)) {
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in events pool. Please adjust its size.");
		lRet = -ENOMEM;
		goto event_subscription_exit;
	}

	lPathLen =
	    strnlen(aHandle->mPath, OAL_LINUX_DRIVER_PATH_SIZE) + lDigits + 1U;
	if (lPathLen >= OAL_LINUX_EVENT_PATH_SIZE) {
		OAL_LOG_WARNING("Event's path will be truncated (%s%d)\n",
		                lEvent->mPath, aEventId);
		lPathLen = OAL_LINUX_EVENT_PATH_SIZE;
	}

	lBytes = (size_t)snprintf(lEvent->mPath, lPathLen, "%s%" PRIu32,
	                          aHandle->mPath, aEventId);
	if (lBytes != (lPathLen - 1U)) {
		lRet = -1;
		OAL_LOG_ERROR("Failed to assembly event path\n");
		goto free_event;
	}

	lEvent->mID      = aEventId;
	lEvent->mFd      = open(lEvent->mPath, O_RDWR);
	lEvent->mpHandle = aHandle;
	if (lEvent->mFd == -1) {
		lRet = -1;
		OAL_LOG_ERROR("Device %s couldn't be opened!\n", lEvent->mPath);
		goto free_event;
	}

	*apEvent = lEvent;

free_event:
	if (lRet != 0) {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&aHandle->mEventsPool,
		                                 aHandle->mEvents, lEvent);
	}

event_subscription_exit:
	return lRet;
}

int32_t OAL_WaitForEventsWithTimeout(OAL_DriverEvent_t *apEvents,
                                     size_t aNumEvents,
                                     OAL_Timespec_t *apTimeout)
{
	int32_t lRet = 0;
	size_t lIdx;
	struct pollfd lPollFds[aNumEvents];
	struct timespec lTimeout, *lpTimeout = NULL;

	OAL_CHECK_NULL_PARAM(apEvents, wait_event_exit);

	if (aNumEvents == 0U) {
		lRet = -EINVAL;
		goto wait_event_exit;
	}

	if (apTimeout != NULL) {
		lTimeout.tv_sec  = apTimeout->mSec;
		lTimeout.tv_nsec = apTimeout->mNsec;
		lpTimeout        = &lTimeout;
	}

	for (lIdx = 0U; lIdx < aNumEvents; lIdx++) {
		if (apEvents[lIdx] == NULL) {
			lRet = -EINVAL;
			OAL_LOG_ERROR("Event %zu is NULL\n", lIdx);
			goto wait_event_exit;
		}

		apEvents[lIdx]->mStatus = 0U;
		lPollFds[lIdx].fd       = apEvents[lIdx]->mFd;
		lPollFds[lIdx].events   = POLLIN;
		lPollFds[lIdx].revents  = 0;
	}

	lRet = ppoll(lPollFds, OAL_ARRAY_SIZE(lPollFds), lpTimeout, NULL);
	if (lRet < 0) {
		OAL_LOG_ERROR("ppol failed : %s\n", strerror(errno));
		goto wait_event_exit;
	} else if (lRet == 0) {
		lRet = OAL_RPC_TIMEOUT;
		goto wait_event_exit;
	} else {
		lRet = 0;
	}

	for (lIdx = 0U; lIdx < aNumEvents; lIdx++) {
		if ((((uint32_t)lPollFds[lIdx].revents) & ((uint32_t)POLLIN)) !=
		    0U) {
			lPollFds[lIdx].revents  = 0;
			apEvents[lIdx]->mStatus = 1U;
		}

		if ((((uint32_t)lPollFds[lIdx].revents) &
		     ((uint32_t)POLLHUP)) != 0U) {
			lRet = -1;
			OAL_LOG_ERROR(
			    "POLL Hangup occurred on "
			    "event %zu\n",
			    lIdx);
		}

		if ((((uint32_t)lPollFds[lIdx].revents) &
		     ((uint32_t)POLLERR)) != 0U) {
			lRet = -1;
			OAL_LOG_ERROR("Received error from event %zu\n", lIdx);
		}
	}

wait_event_exit:
	return lRet;
}

int32_t OAL_WaitForEvents(OAL_DriverEvent_t *apEvents, size_t aNumEvents)
{
	return OAL_WaitForEventsWithTimeout(apEvents, aNumEvents, NULL);
}

int32_t OAL_HasEventFired(OAL_DriverEvent_t aEvent, uint8_t *apEventStatus)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(aEvent, has_fired_exit);
	OAL_CHECK_NULL_PARAM(apEventStatus, has_fired_exit);

	*apEventStatus = aEvent->mStatus;
has_fired_exit:
	return lRet;
}

int32_t OAL_GetEventId(OAL_DriverEvent_t aEvent, uint32_t *apEventId)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(aEvent, get_id_exit);
	OAL_CHECK_NULL_PARAM(apEventId, get_id_exit);

	*apEventId = aEvent->mID;
get_id_exit:
	return lRet;
}

int32_t OAL_UnsubscribeFromEvent(OAL_DriverEvent_t aEvent)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(aEvent, unsubscribe_exit);
	OAL_CHECK_NULL_PARAM(aEvent->mpHandle, unsubscribe_exit);

	lRet = close(aEvent->mFd);

	(void)OAL_RELEASE_ELEM_FROM_POOL(&aEvent->mpHandle->mEventsPool,
	                                 aEvent->mpHandle->mEvents, aEvent);

unsubscribe_exit:
	return lRet;
}
