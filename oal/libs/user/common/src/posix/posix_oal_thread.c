/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <oal_thread.h>

int32_t OAL_CreateThread(struct OAL_Thread *apThread,
                         const char8_t *acpTaskName,
                         OAL_TaskCallback_t aTaskFunc, void *apTaskArg)
{
	int32_t lRet = 0;

	if ((apThread == NULL) || (aTaskFunc == NULL)) {
		lRet = -EINVAL;
		goto create_thread_exit;
	}

	apThread->mState = OAL_NOT_JOINED_YET;
	lRet = pthread_create(&apThread->mOSThread, NULL, aTaskFunc, apTaskArg);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to create a thread: %s\n",
		              strerror(lRet));
		goto create_thread_exit;
	}

	if (acpTaskName != NULL) {
		/* Best effort */
		(void)pthread_setname_np(apThread->mOSThread, acpTaskName);
	}

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

	if (apThread->mState == OAL_NOT_JOINED_YET) {
		lRet = pthread_join(apThread->mOSThread, apThreadReturn);
	}
	apThread->mState = OAL_JOINED_THREAD;

oal_join_task_exit:
	return lRet;
}
