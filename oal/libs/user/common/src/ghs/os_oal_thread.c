/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <oal_thread.h>
#include <ghs_task_utils.h>

static Value OAL_GHS_ThreadCallback(Address aAddress)
{
	Value lRet = 0;
	struct OAL_Thread *lpThread;

	if (aAddress == 0U) {
		OAL_LOG_ERROR("Corrupted thread data\n");
		lRet = EINVAL;
		goto thread_callback_exit;
	}

	lpThread = (struct OAL_Thread *)aAddress;
	if (lpThread->mpCallback == NULL) {
		OAL_LOG_ERROR("Corrupted thread data");
		lRet = EINVAL;
		goto thread_callback_exit;
	}

	lpThread->mpFuncRet = lpThread->mpCallback(lpThread->mpFuncArg);

thread_callback_exit:
	return lRet;
}

int32_t OAL_CreateThread(struct OAL_Thread *apThread,
                         const char8_t *acpTaskName,
                         OAL_TaskCallback_t aTaskFunc, void *apTaskArg)
{
	int32_t lRet                  = 0;
	const char8_t *lcpDefaultName = "oal_thread";

	if ((apThread == NULL) || (aTaskFunc == NULL)) {
		lRet = -EINVAL;
		goto create_thread_exit;
	}

	if (acpTaskName != NULL) {
		lcpDefaultName = acpTaskName;
	}

	apThread->mpCallback = aTaskFunc;
	apThread->mpFuncArg  = apTaskArg;
	apThread->mpFuncRet  = NULL;

	lRet =
	    OAL_GHS_CreateAndRunTask(&apThread->mOSThread, lcpDefaultName,
	                             OAL_GHS_ThreadCallback, (Address)apThread);
create_thread_exit:
	return lRet;
}

int32_t OAL_JoinTask(struct OAL_Thread *apThread, void **apThreadReturn)
{
	int32_t lRet = 0;

	if (apThread == NULL) {
		lRet = -EINVAL;
		goto oal_join_task_exit;
	}

	if (apThread->mOSThread != NULL) {
		lRet = OAL_GHS_JoinTask(&apThread->mOSThread);
		if (apThreadReturn != NULL) {
			*apThreadReturn = apThread->mpFuncRet;
		}
	}
	apThread->mOSThread = NULL;

oal_join_task_exit:
	return lRet;
}
