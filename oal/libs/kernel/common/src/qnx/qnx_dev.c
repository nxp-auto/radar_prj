/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <qnx_dev.h>
#include <qnx_dev_resource.h>


#define MIN_AVAILABLE_THREADS 2
#define MAX_AVAILABLE_THREADS 4
#define THREAD_INCREMENT 1
#define MAX_THREADS 50

#define MSG_MAX_SIZE 4096
#define NPARTS_MAX 5

typedef THREAD_POOL_PARAM_T *(*OAL_attr_block_func_t)(THREAD_POOL_PARAM_T *apPoolParam);
typedef void (*OAL_attr_unblock_func_t)(THREAD_POOL_PARAM_T *apPoolParam);
typedef int (*OAL_attr_handler_func_t)(THREAD_POOL_PARAM_T *apPoolParam);
typedef THREAD_POOL_PARAM_T *(*OAL_attr_context_alloc_t)(THREAD_POOL_HANDLE_T *apPoolHdl);
typedef void (*OAL_attr_context_free_t)(THREAD_POOL_PARAM_T *apPoolParam);

// 0666U
#define UGO_RW 438U

int32_t OAL_QnxInitDevice(qnx_device_t *apDev, const char8_t *acpBaseName,
                        uint32_t aNumber)
{
	size_t lNameLen;
	uint8_t lDigits;
	mode_t lMode;
	int32_t lRet = 0;

	if ((apDev == NULL) || (acpBaseName == NULL)) {
		lRet = -EINVAL;
		goto init_device_exit;
	}

	(void)memset(apDev, 0, sizeof(*apDev));

	lNameLen = strnlen(acpBaseName, OAL_MAX_NAME_LEN) + OAL_QNX_DEVICE_PREFIX_LEN;
	if (aNumber != OAL_NO_INDEX) {
		lDigits = OAL_GetNumDigits(aNumber);
		lNameLen += lDigits;
		(void)snprintf(apDev->mName, lNameLen,
		               OAL_QNX_DEVICE_PREFIX "%s%u", acpBaseName, aNumber);
	} else {
		(void)snprintf(apDev->mName, lNameLen, OAL_QNX_DEVICE_PREFIX "%s",
		               acpBaseName);
	}

	apDev->mpDpp = OAL_DevDispatchCreate();
	if (apDev->mpDpp == NULL) {
		OAL_LOG_ERROR("Unable to dispatch_create(): %s\n",
		              strerror(errno));
		lRet = -ENOMEM;
		goto init_device_exit;
	}

	/* initialize device file */

	/* bind default functions into the outcall tables */
	lMode = ((mode_t)S_IFREG) | ((mode_t)UGO_RW);
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &apDev->mConnectFunc,
	                 _RESMGR_IO_NFUNCS, &apDev->mIoFunc);
	iofunc_attr_init(&apDev->mAttr, lMode, NULL, NULL);

	/* initialize the various data structures */
	(void)memset(&apDev->mResmgrAttr, 0, sizeof(apDev->mResmgrAttr));
	/* configure message attributes */
	apDev->mResmgrAttr.nparts_max = NPARTS_MAX;
	/* 4KB should be enough for our data structures */
	apDev->mResmgrAttr.msg_max_size = MSG_MAX_SIZE;

	(void)memset(&apDev->mTpAttr, 0, sizeof(apDev->mTpAttr));
	apDev->mTpAttr.handle = apDev->mpDpp;
	apDev->mTpAttr.context_alloc =
	    (OAL_attr_context_alloc_t)dispatch_context_alloc;
	apDev->mTpAttr.block_func   = (OAL_attr_block_func_t)dispatch_block;
	apDev->mTpAttr.unblock_func = (OAL_attr_unblock_func_t)dispatch_unblock;
	apDev->mTpAttr.handler_func = (OAL_attr_handler_func_t)dispatch_handler;
	apDev->mTpAttr.context_free =
	    (OAL_attr_context_free_t)dispatch_context_free;
	apDev->mTpAttr.lo_water  = MIN_AVAILABLE_THREADS;
	apDev->mTpAttr.hi_water  = MAX_AVAILABLE_THREADS;
	apDev->mTpAttr.increment = THREAD_INCREMENT;
	apDev->mTpAttr.maximum   = MAX_THREADS;

	apDev->mpThreadPool = NULL;
init_device_exit:
	return lRet;
}

static void *handle_requests_thread(void *apArg)
{
	qnx_device_t *lpDev = apArg;

	/* Detach this thread. It will be used as worker by the pool */
	(void)pthread_detach(pthread_self());

	if (lpDev == NULL) {
		goto handle_requests_exit;
	}

	/* Creating thread pool */
	lpDev->mpThreadPool =
	    OAL_DevThreadPoolCreate(&lpDev->mTpAttr, POOL_FLAG_USE_SELF);
	if (lpDev->mpThreadPool == NULL) {
		OAL_LOG_ERROR("Unable to initialize thread pool.\n");
		goto handle_requests_exit;
	}

	/* This call will never return */
	(void)thread_pool_start(lpDev->mpThreadPool);

handle_requests_exit:
	return NULL;
}

int32_t OAL_QnxHandleRequests(qnx_device_t *apDev)
{
	int32_t lRet;

	OAL_CHECK_NULL_PARAM(apDev, handle_requests_exit);

	apDev->mDppId = OAL_DevResmgrAttach(
	    apDev->mpDpp, &apDev->mResmgrAttr, apDev->mName, _FTYPE_ANY, 0,
	    &apDev->mConnectFunc, &apDev->mIoFunc, &apDev->mAttr);
	if (apDev->mDppId == -1) {
		lRet = -1;
		OAL_LOG_ERROR("Unable to resmgr_attach\n");
		goto handle_requests_exit;
	}

	/* allocate context for resource manager */
	apDev->mpCtp = OAL_DevResmgrContextAlloc(apDev->mpDpp);
	if (apDev->mpCtp == NULL) {
		lRet = -1;
		OAL_LOG_ERROR("resmgr_context_alloc\n");
		goto handle_requests_exit;
	}

	lRet =
	    pthread_create(NULL, NULL, handle_requests_thread, apDev);
	if (lRet != 0) {
		OAL_LOG_ERROR("pthread_create failed with %d code\n", lRet);
	}

handle_requests_exit:
	return lRet;
}

int32_t OAL_QnxWaitDevRelease(void)
{
	int32_t lSignal;
	int32_t lRet;
	siginfo_t lSigInfo;
	sigset_t lSigSet;

	// release fdt so it can be used by another driver
	(void)OAL_ReleaseFdt();
	lRet = sigfillset(&lSigSet);
	if (lRet == -1) {
		OAL_LOG_ERROR("sigfillset failed with errno %d\n", errno);
		lRet = -EINVAL;
		goto wait_dev_release_exit;
	}

	lRet = sigprocmask(SIG_SETMASK, &lSigSet, NULL);
	if (lRet == -1) {
		OAL_LOG_ERROR("sigprocmask failed with errno %d\n", errno);
		lRet = -EINVAL;
		goto wait_dev_release_exit;
	}

	while (true) {
		lSignal = sigwaitinfo(&lSigSet, &lSigInfo);
		if (lSignal == -1) {
			OAL_LOG_ERROR("sigwaitinfo failed with errno %d\n",
			              errno);
			lRet = -EINVAL;
			break;
		}

		if (lSignal == SIGINT) {
			lRet = EXIT_SUCCESS;
			break;
		}
	}

wait_dev_release_exit:
	return lRet;
}
