/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <oal_thread.h>

static int32_t oal_kthread_callback(void *apData)
{
	int32_t lRet = 0;
	struct OAL_Thread *lpThread;

	if (apData == NULL) {
		OAL_LOG_ERROR("Corrupted thread data\n");
		lRet = -EINVAL;
		goto oal_thread_callback_exit;
	}

	lpThread = (struct OAL_Thread *)apData;
	/* Paranoid check */
	if (lpThread->mpCallback == NULL) {
		OAL_LOG_ERROR("Corrupted thread data");
		lRet = -EINVAL;
		goto oal_thread_callback_exit;
	}

	lpThread->mpFuncRet = lpThread->mpCallback(lpThread->mpFuncArg);

oal_thread_callback_exit:
	return lRet;
}

int32_t OAL_CreateThread(struct OAL_Thread *apThread,
                         const char8_t *acpTaskName,
                         OAL_TaskCallback_t aTaskFunc, void *apTaskArg)
{
	int32_t lRet               = 0;
	const char8_t *lcpTaskName = "oal_thread";

	if ((apThread == NULL) || (aTaskFunc == NULL)) {
		lRet = -EINVAL;
		goto create_thread_exit;
	}

	apThread->mpOSThread = NULL;
	apThread->mpCallback = aTaskFunc;
	apThread->mpFuncArg  = apTaskArg;
	apThread->mpFuncRet  = NULL;

	if (acpTaskName != NULL) {
		lcpTaskName = acpTaskName;
	}

	apThread->mpOSThread =
	    kthread_create(oal_kthread_callback, apThread, lcpTaskName);
	if (IS_ERR(apThread->mpOSThread)) {
		apThread->mpOSThread = NULL;
		OAL_LOG_ERROR("Failed to create a new thread\n");
		lRet = -EINVAL;
		goto create_thread_exit;
	}

	(void)wake_up_process(apThread->mpOSThread);

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

	if (apThread->mpOSThread != NULL) {
		do {
			/* Wait first thread schedule */
			set_current_state(TASK_INTERRUPTIBLE);
			(void)schedule_timeout(1);
			lRet = kthread_stop(apThread->mpOSThread);
		} while (lRet == -EINTR);
		if (apThreadReturn != NULL) {
			*apThreadReturn = apThread->mpFuncRet;
		}
	}
	apThread->mpOSThread = NULL;

oal_join_task_exit:
	return lRet;
}
