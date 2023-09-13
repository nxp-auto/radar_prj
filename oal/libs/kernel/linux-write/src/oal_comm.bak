/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <linux/atomic.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>

#include <oal_comm.h>
#include <oal_comm_kernel.h>
#include <oal_log.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <os_oal_comm_kernel.h>
#include <linux_device.h>

struct oal_dispatcher {
	OAL_FuncArgs_t *mpArgsBuff;
	struct file *mpFile;
	size_t mFillLevel;
	size_t mMaxSize;
	OAL_RPCService_t mService;
};

struct OAL_RPCService;

struct OAL_RPCEvent {
	uint32_t mID;
	uint8_t mName[OAL_MAX_NAME_LEN + 4U + 1U];  // name + 4 digits + '\0'
	struct file_operations mFops;
	OAL_DevFile_t mDevFile;
	wait_queue_head_t mWaitQueue;
	atomic_t mTriggered;
	struct OAL_RPCService *mpService;
};

struct OAL_RPCService {
	OAL_dispatch_t mDispatch;
	OAL_DevFile_t mDevData;
	struct file_operations mFops;
	uint8_t *mpInBuffer;
	size_t mInBufferSize;
	struct mutex mLock;
	OAL_ServiceData_t mData;
	OAL_DECLARE_STATIC_POOL_UNINITIALIZED(mEventsPool,
	                                      OAL_MAX_EVENTS_PER_SERVICE);
	struct OAL_RPCEvent mEvents[OAL_MAX_EVENTS_PER_SERVICE];
};

static struct OAL_RPCService gsWrDriverServices[OAL_MAX_SERVICES_PER_DRIVER];
static OAL_DECLARE_STATIC_POOL(gsWrDriverServicesPool,
                               OAL_ARRAY_SIZE(gsWrDriverServices));

static int32_t consumeMessage(struct comm_args aInArgs, struct file *apFile,
                              struct OAL_RPCService *apService)
{
	int32_t lRet = 0;
	uint32_t lFret;
	struct oal_dispatcher lDispatcher;

	lDispatcher.mpArgsBuff = aInArgs.mpOutBuff;
	lDispatcher.mFillLevel = (size_t)0;
	lDispatcher.mMaxSize   = aInArgs.mNumOutArgs;
	lDispatcher.mpFile     = apFile;
	lDispatcher.mService   = apService;

	if (apService->mInBufferSize < aInArgs.mInArgBuffLen) {
		if (aInArgs.mInArgBuffLen > ((size_t)KMALLOC_MAX_SIZE)) {
			OAL_LOG_ERROR(
			    "Input payload too long."
			    "Maximum size should be %lu\n",
			    KMALLOC_MAX_SIZE);
			lRet = -EIO;
			goto end_consumeMessage;
		}

		apService->mInBufferSize = aInArgs.mInArgBuffLen;
		apService->mpInBuffer =
		    krealloc(apService->mpInBuffer, apService->mInBufferSize,
		             GFP_KERNEL);

		if (apService->mpInBuffer == NULL) {
			OAL_LOG_ERROR("Failed to allocate memory !\n");
			lRet = -EIO;
			goto end_consumeMessage;
		}
	}

	// Copy serialized input arguments from user-space
	if (copy_from_user(apService->mpInBuffer, (void *)aInArgs.mpInArgBuff,
	                   aInArgs.mInArgBuffLen) != 0U) {
		OAL_LOG_ERROR("copy_from_user failed!\n");
		lRet = -1;
		goto end_consumeMessage;
	}

	// Call the dispatcher
	lFret = apService->mDispatch(&lDispatcher, aInArgs.mFuncId,
	                             (uintptr_t)apService->mpInBuffer,
	                             aInArgs.mInArgBuffLen);

	if (copy_to_user((void *)aInArgs.mpRet, &lFret, sizeof(lFret)) != 0U) {
		OAL_LOG_ERROR("copy_to_user failed!\n");
		lRet = -1;
	}

end_consumeMessage:
	return lRet;
}

int32_t OAL_WriteGetServiceFromFile(struct file *apFile,
                                    OAL_RPCService_t *apService)
{
	int32_t lRet = 0;
	struct cdev *lpCdev;
	struct OAL_DevFile *lpDevData;

	OAL_CHECK_NULL_PARAM(apFile, get_service_exit);
	OAL_CHECK_NULL_PARAM(apService, get_service_exit);

	/* Get cdev from private data */
	lpCdev = apFile->private_data;
	if (lpCdev == NULL) {
		lRet = -EINVAL;
		goto get_service_exit;
	}

	/* Get device data */
	lpDevData = OAL_GET_PARENT_STRUCT(lpCdev, struct OAL_DevFile, mCDev);

	/* Get service */
	*apService =
	    OAL_GET_PARENT_STRUCT(lpDevData, struct OAL_RPCService, mDevData);

get_service_exit:
	return lRet;
}

static ssize_t OAL_WriteCallback(struct file *apFile, const char8_t *acpData,
                                 size_t aDataSize, loff_t *apOffset)
{
	struct comm_args lInArgs;
	struct OAL_RPCService *lpService;
	ssize_t lRet = (ssize_t)aDataSize;
	int32_t lIRet;

	OAL_UNUSED_ARG(apOffset);

	lIRet = OAL_WriteGetServiceFromFile(apFile, &lpService);

	if ((lIRet != 0) || (lpService == NULL)) {
		OAL_LOG_ERROR("Invalid service\n");
		lRet = -EIO;
		goto write_exit;
	}

	if (aDataSize != sizeof(lInArgs)) {
		OAL_LOG_ERROR("Incomplete input arguments\n");
		lRet = (ssize_t)(-EOPNOTSUPP);
		goto write_exit;
	}

	// Guard the access to service, in consequnence all RPC calls
	// are serialized.
	mutex_lock(&lpService->mLock);
	if (copy_from_user(&lInArgs, (void *)(uintptr_t)acpData,
	                   sizeof(lInArgs)) != 0U) {
		OAL_LOG_ERROR("copy_from_user failed!\n");
		lRet = (ssize_t)-EIO;
	} else {
		if (consumeMessage(lInArgs, apFile, lpService) != 0) {
			OAL_LOG_ERROR("Failed to consume message!\n");
			lRet = (ssize_t)-EIO;
		}
	}
	mutex_unlock(&lpService->mLock);

write_exit:
	return lRet;
}

struct file *OAL_WriteGetFile(oal_dispatcher_t *apDisp)
{
	return apDisp->mpFile;
}

static int32_t OAL_WriteOpen(struct inode *apInode, struct file *apFile)
{
	if ((apInode != NULL) && (apFile != NULL)) {
		// Set cdev as private data
		apFile->private_data = apInode->i_cdev;
	}

	return 0;
}

static int32_t defaultClose(struct inode *apInode, struct file *apFile)
{
	OAL_UNUSED_ARG(apInode);
	OAL_UNUSED_ARG(apFile);
	return 0;
}

OAL_RPCService_t OAL_RPCRegister(const char8_t *acpName, OAL_dispatch_t aDisp)
{
	OAL_RPCService_t lService = NULL;
	int32_t lRet;

	if (aDisp == NULL) {
		goto register_exit;
	}

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&gsWrDriverServicesPool,
	                                gsWrDriverServices, &lService);
	if ((lRet != 0) || (lService == NULL)) {
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in driver's services pool. Please adjust its size.");
	} else {
		(void)memset(lService, 0, sizeof(*lService));
		lService->mDispatch = aDisp;

		lService->mFops.owner   = THIS_MODULE;
		lService->mFops.open    = OAL_WriteOpen;
		lService->mFops.release = defaultClose;
		lService->mFops.write   = OAL_WriteCallback;

		mutex_init(&lService->mLock);

		/* Initialize events pool */
		OAL_SET_POOL_SIZE(&lService->mEventsPool,
		                  OAL_ARRAY_SIZE(lService->mEvents));
		OAL_INITIALIZE_POOL(&lService->mEventsPool);

		lRet = OAL_InitDevFile(&lService->mDevData, &lService->mFops,
		                       acpName);
		if (lRet != 0) {
			(void)OAL_RELEASE_ELEM_FROM_POOL(
			    &gsWrDriverServicesPool, gsWrDriverServices,
			    lService);
			lService = NULL;
		}
	}

register_exit:
	return lService;
}

int32_t OAL_SetCustomFileOperations(OAL_RPCService_t aService,
                                    struct file_operations *apFileOps)
{
	int32_t lRet = 0;
	size_t lLen;
	char8_t lName[OAL_MAX_NAME_LEN + 1U];
	void *lpVSrc, *lpVDst;

	if ((aService == NULL) || (apFileOps == NULL)) {
		lRet = -EINVAL;
		goto set_fops_exit;
	}

	mutex_lock(&aService->mLock);

	// Save device name
	lLen = strnlen(aService->mDevData.mName, OAL_MAX_NAME_LEN) + 1U;

	lpVSrc = aService->mDevData.mName;
	lpVDst = lName;
	(void)memcpy(lpVDst, lpVSrc, lLen);

	// Release previously registered device
	lRet = OAL_DestroyDevFile(&aService->mDevData);
	if (lRet != 0) {
		goto set_fops_exit_lock;
	}

	(void)memcpy(&aService->mFops, apFileOps, sizeof(aService->mFops));
	(void)memset(&aService->mDevData, 0, sizeof(aService->mDevData));

	aService->mFops.owner = THIS_MODULE;
	aService->mFops.open  = OAL_WriteOpen;
	aService->mFops.write = OAL_WriteCallback;

	// Register updated operations
	lRet = OAL_InitDevFile(&aService->mDevData, &aService->mFops, lName);

set_fops_exit_lock:
	mutex_unlock(&aService->mLock);
set_fops_exit:
	return lRet;
}

int32_t OAL_RPCSetPrivateData(OAL_RPCService_t aServ, OAL_ServiceData_t aData)
{
	int32_t lRet = 0;
	if (aServ == NULL) {
		lRet = -EINVAL;
	} else {
		aServ->mData = aData;
	}

	return lRet;
}

int32_t OAL_RPCGetPrivateData(OAL_RPCService_t aServ, OAL_ServiceData_t *apData)
{
	int32_t lRet = 0;
	if ((aServ == NULL) || (apData == NULL)) {
		lRet = -EINVAL;
	} else {
		*apData = aServ->mData;
	}

	return lRet;
}

int32_t OAL_RPCCleanup(const OAL_RPCService_t acServ)
{
	struct OAL_RPCService *lpServ = acServ;
	int32_t lRet                  = 0;

	if (lpServ != NULL) {
		lRet = OAL_DestroyDevFile(&acServ->mDevData);
		mutex_destroy(&lpServ->mLock);

		if (lpServ->mpInBuffer != NULL) {
			kfree(lpServ->mpInBuffer);
		}

		(void)OAL_RELEASE_ELEM_FROM_POOL(&gsWrDriverServicesPool,
		                                 gsWrDriverServices, lpServ);
	} else {
		lRet = -1;
	}

	return lRet;
}

int32_t OAL_RPCAppendReply(oal_dispatcher_t *apDisp, uint8_t *apData,
                           size_t aDataSize)
{
	int32_t lRet = 0;
	OAL_FuncArgs_t *lpCurrnetArg;
	OAL_FuncArgs_t lUsArg;

	// Append datasize and apData buffer to be sent to userspace
	if (apDisp->mpArgsBuff == NULL) {
		lRet = -1;
		OAL_LOG_WARNING("NULL output buffer\n");
		goto rpc_append_end;
	}

	if (apDisp->mpArgsBuff->mpData == NULL) {
		lRet = -1;
		OAL_LOG_WARNING("NULL output buffer\n");
		goto rpc_append_end;
	}

	if (apDisp->mFillLevel >= apDisp->mMaxSize) {
		OAL_LOG_WARNING("Output buffer overflow!\n");
		lRet = -1;
		goto rpc_append_end;
	}

	lpCurrnetArg = &(apDisp->mpArgsBuff[apDisp->mFillLevel]);
	if (copy_from_user(&lUsArg, lpCurrnetArg, sizeof(lUsArg)) != 0U) {
		OAL_LOG_ERROR("copy_from_user failed!\n");
		lRet = -1;
		goto rpc_append_end;
	}

	// Check if argument aDataSize from userspace corresponds with the
	// one received
	if (lUsArg.mSize != aDataSize) {
		OAL_LOG_WARNING(
		    "Argument aDataSize doesn't match! (%lu != %lu)\n",
		    lUsArg.mSize, aDataSize);
		lRet = -1;
		goto rpc_append_end;
	}

	if (copy_to_user(lUsArg.mpData, apData, aDataSize) != 0U) {
		OAL_LOG_ERROR("copy_to_user failed!\n");
		lRet = -1;
		goto rpc_append_end;
	}

	apDisp->mFillLevel++;

rpc_append_end:
	return lRet;
}

OAL_RPCService_t OAL_RPCGetService(oal_dispatcher_t *apDisp)
{
	OAL_RPCService_t lServ = NULL;
	if (apDisp != NULL) {
		lServ = apDisp->mService;
	}

	return lServ;
}

static unsigned int OAL_RPCPoll(struct file *apFile, poll_table *apWait)
{
	unsigned int lMask = 0;
	struct cdev *lpCdev;
	struct OAL_DevFile *lpDevData;
	struct OAL_RPCEvent *lpEvent = NULL;

	if ((apFile == NULL) || (apFile->private_data == NULL)) {
		lMask = POLLERR;
		goto poll_exit;
	}

	/* Get cdev from private data */
	lpCdev = apFile->private_data;

	/* Get device data */
	lpDevData = OAL_GET_PARENT_STRUCT(lpCdev, struct OAL_DevFile, mCDev);

	/* Get service event */
	lpEvent =
	    OAL_GET_PARENT_STRUCT(lpDevData, struct OAL_RPCEvent, mDevFile);

	poll_wait(apFile, &lpEvent->mWaitQueue, apWait);

	if (atomic_read(&lpEvent->mTriggered) != 0) {
		/* Read normal data */
		lMask |= (((unsigned int)POLLIN) | ((unsigned int)POLLRDNORM));
		atomic_sub(1, &lpEvent->mTriggered);
	}

poll_exit:
	return lMask;
}

int32_t static init_event_fops(OAL_RPCEvent_t aEvent)
{
	int32_t lRet = 0;

	(void *)memset(&aEvent->mFops, 0, sizeof(aEvent->mFops));

	aEvent->mFops.owner   = THIS_MODULE;
	aEvent->mFops.open    = OAL_WriteOpen;
	aEvent->mFops.release = defaultClose;
	aEvent->mFops.poll    = OAL_RPCPoll;

	lRet =
	    OAL_InitDevFile(&aEvent->mDevFile, &aEvent->mFops, aEvent->mName);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to register: %s\n", aEvent->mName);
	}

	return lRet;
}

int32_t OAL_RPCRegisterEvent(OAL_RPCService_t aServ, uint32_t aEventID,
                             OAL_RPCEvent_t *apEvent)
{
	int32_t lRet = 0;
	struct OAL_RPCEvent *lpEvent;
	uint8_t lDigits = OAL_GetNumDigits(aEventID);
	size_t lLen, lIdx;
	int32_t lIntRet;

	OAL_CHECK_NULL_PARAM(aServ, register_event_exit);
	OAL_CHECK_NULL_PARAM(apEvent, register_event_exit);

	/* Check if already registered */
	for (lIdx = 0; lIdx < OAL_ARRAY_SIZE(aServ->mEvents); lIdx++) {
		lpEvent = &aServ->mEvents[lIdx];
		if ((OAL_IS_BIT_SET(&aServ->mEventsPool.mBitset, lIdx)) &&
		    (aEventID == lpEvent->mID)) {
			OAL_LOG_ERROR("Event %" PRIu32
			              " is already "
			              "registered\n",
			              aEventID);
			lRet = -1;
			goto register_event_exit;
		}
	}

	*apEvent = NULL;

	lRet = OAL_ALLOC_ELEM_FROM_POOL(&aServ->mEventsPool, aServ->mEvents,
	                                &lpEvent);
	if ((lpEvent == NULL) || (lRet != 0)) {
		lRet = -ENOMEM;
		OAL_LOG_ERROR(
		    "There are no more available positions "
		    "in events pool. Please adjust its size.");
		goto register_event_exit;
	}

	lpEvent->mID       = aEventID;
	lpEvent->mpService = aServ;
	atomic_set(&lpEvent->mTriggered, 0);

	/* Set name based on parent device + event ID */
	lLen = strnlen(aServ->mDevData.mName, OAL_MAX_NAME_LEN) + lDigits + 1U;
	lIntRet = snprintf(lpEvent->mName, lLen, "%s%" PRIu32,
	                   aServ->mDevData.mName, aEventID);
	if (((ssize_t)lIntRet) != (((ssize_t)(lLen)-1))) {
		OAL_LOG_ERROR("Unexpected snprintf result\n");
		lRet = -1;
		goto free_event;
	}

	init_waitqueue_head(&lpEvent->mWaitQueue);

	lRet = init_event_fops(lpEvent);
	if (lRet != 0) {
		goto free_event;
	}

	*apEvent = lpEvent;

free_event:
	if (lRet != 0) {
		(void)OAL_RELEASE_ELEM_FROM_POOL(&aServ->mEventsPool,
		                                 aServ->mEvents, lpEvent);
		*apEvent = NULL;
	}

register_event_exit:
	return lRet;
}

int32_t OAL_RPCTriggerEvent(OAL_RPCEvent_t aEvent)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(aEvent, trigger_event_exit);
	atomic_add(1, &aEvent->mTriggered);
	wake_up_interruptible(&aEvent->mWaitQueue);

trigger_event_exit:
	return lRet;
}

int32_t OAL_RPCDeregisterEvent(OAL_RPCEvent_t aEvent)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(aEvent, unregister_event_exit);
	OAL_CHECK_NULL_PARAM(aEvent->mpService, unregister_event_exit);

	(void)OAL_DestroyDevFile(&aEvent->mDevFile);

	(void)OAL_RELEASE_ELEM_FROM_POOL(&aEvent->mpService->mEventsPool,
	                                 aEvent->mpService->mEvents, aEvent);

unregister_event_exit:
	return lRet;
}

int32_t OAL_RPCGetClientPID(oal_dispatcher_t *apDispatch, pid_t *apClientPID)

{
	int32_t lRet = 0;

	if ((apDispatch == NULL) || (apClientPID == NULL)) {
		lRet = -EINVAL;
	} else {
		*apClientPID = task_pgrp_nr(current);
	}

	return lRet;
}
