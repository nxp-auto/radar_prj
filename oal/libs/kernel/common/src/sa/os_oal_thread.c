/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_thread.h>

int32_t OAL_CreateThread(struct OAL_Thread *apThread,
                         const char8_t *acpTaskName,
                         OAL_TaskCallback_t aTaskFunc, void *apTaskArg)
{
	int32_t lRet = 0;
	OAL_UNUSED_ARG(acpTaskName);

	if ((apThread == NULL) || (aTaskFunc == NULL)) {
		lRet = -EINVAL;
		goto create_thread_exit;
	}

	apThread->mpFuncReturn = aTaskFunc(apTaskArg);

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

	if (apThreadReturn != NULL) {
		*apThreadReturn = apThread->mpFuncReturn;
	}

oal_join_task_exit:
	return lRet;
}
