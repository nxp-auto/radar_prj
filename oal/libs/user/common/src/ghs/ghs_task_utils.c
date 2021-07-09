/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <ghs_task_utils.h>

#define OAL_GHS_THREAD_ACTIVITY_ID 199

int32_t OAL_GHS_CreateAndRunTask(Task *apTask, const char8_t *acpTaskName,
                                 TASKENTRYPOINT aEntryPoint, Address aArgument)
{
	Error lError;
	int32_t lRet = 0;
	Value lPriority;
	Address lStackBottom, lStackTop;
	Address lStackSize;

	if ((apTask == NULL) || (acpTaskName == NULL) ||
	    (aEntryPoint == NULL)) {
		lRet = -1;
		goto init_task_exit;
	}

	lError = GetTaskStackLimits(CurrentTask(), &lStackBottom, &lStackTop);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get stack limits (%s)\n",
		              ErrorString(lError));
		lRet = -1;
		goto init_task_exit;
	}

	if (lStackTop > lStackBottom) {
		lStackSize = lStackTop - lStackBottom;
	} else {
		lStackSize = lStackBottom - lStackTop;
	}

	lError = GetTaskPriority(CurrentTask(), &lPriority);
	if (lError != Success) {
		OAL_LOG_ERROR(
		    "Failed to get priority of the current task (%s)\n",
		    ErrorString(lError));
		lRet = -1;
		goto init_task_exit;
	}

	lError = CommonCreateTaskWithArgument(
	    lPriority, aEntryPoint, aArgument, lStackSize,
	    (char8_t *)(uintptr_t)acpTaskName, apTask);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to create a new task (%s)\n",
		              ErrorString(lError));
		lRet = -1;
		goto init_task_exit;
	}

	lError = RunTask(*apTask);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed start the thread (%s)\n",
		              ErrorString(lError));
		lRet = -1;
		goto init_task_exit;
	}

init_task_exit:
	return lRet;
}

int32_t OAL_GHS_JoinTask(Task *apTask)
{
	Value lTaskStatus = 0;
	int32_t lRet      = 0;
	Error lError;

	if (apTask == NULL) {
		lRet = EINVAL;
		goto join_task_exit;
	}

	while (lRet == 0) {
		lError = SynchronousReceive((Connection)*apTask, NULL);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to wait task's exit\n (%s)\n",
			              ErrorString(lError));
			lRet = -1;
			break;
		}

		lError = GetTaskStatus(*apTask, &lTaskStatus, NULL, NULL);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to get task status\n");
			lRet = -1;
			break;
		}

		if (lTaskStatus == (Value)StatExited) {
			lError = CommonCloseTask(*apTask);
			if (lError != Success) {
				OAL_LOG_ERROR("Failed to close task (%s)\n",
				              ErrorString(lError));
				lRet = -1;
			}

			break;
		} else {
			OAL_LOG_ERROR("Received signal %lld\n", lTaskStatus);
			lRet = -1;
		}
	}

join_task_exit:
	return lRet;
}
