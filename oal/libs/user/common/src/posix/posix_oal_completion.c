/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_completion.h>
#include <oal_uptime.h>

int32_t OAL_InitCompletion(OAL_Completion_t *apComp)
{
	int32_t lRet = 0, lStatus;
	if (apComp == NULL) {
		lRet = -EINVAL;
		goto init_comp_exit;
	}

	apComp->mDone = 0U;
	lRet          = OAL_InitCondVar(&apComp->mCondVar);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "Failed to initialize conditional"
		    " variable\n");
	}

	lRet = OAL_InitializeMutex(&apComp->mMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize mutex\n");
		lStatus = OAL_DestroyCondVar(&apComp->mCondVar);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed to destroy cond variable\n");
		}
	}

init_comp_exit:
	return lRet;
}

int32_t OAL_DestroyCompletion(OAL_Completion_t *apComp)
{
	int32_t lRet = -EINVAL;
	int32_t lStatus;

	if (apComp != NULL) {
		lRet = OAL_DestroyCondVar(&apComp->mCondVar);
		if (lRet != 0) {
			OAL_LOG_ERROR(
			    "Failed to destroy conditional "
			    "variable\n");
		}

		lStatus = OAL_DestroyMutex(&apComp->mMutex);
		if (lStatus != 0) {
			lRet = lStatus;
			OAL_LOG_ERROR("Failed to destroy mutex\n");
		}
	}

	return lRet;
}

int32_t OAL_WaitForCompletion(OAL_Completion_t *apComp)
{
	int32_t lRet, lStatus;
	if (apComp == NULL) {
		lRet = -EINVAL;
		goto wait_completion_end;
	}

	lRet = OAL_LockMutex(&apComp->mMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to acquire lock\n");
		goto wait_completion_end;
	}

	if (apComp->mDone == 0U) {
		lRet = OAL_WaitCondVar(&apComp->mCondVar, &apComp->mMutex);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to wait !\n");
		}
	}

	if (apComp->mDone != 0U) {
		apComp->mDone--;
	}

	lStatus = OAL_UnlockMutex(&apComp->mMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release lock\n");
		lRet = lStatus;
	}

wait_completion_end:
	return lRet;
}

int32_t OAL_Complete(OAL_Completion_t *apComp)
{
	int32_t lStatus, lRet;

	OAL_CHECK_NULL_PARAM(apComp, complete_exit);

	lRet = OAL_LockMutex(&apComp->mMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to acquire lock\n");
		goto complete_exit;
	}

	if (apComp->mDone != UINT_MAX) {
		apComp->mDone++;
	}

	lRet = OAL_SignalCondVar(&apComp->mCondVar);
	if (lRet != 0) {
		OAL_LOG_ERROR("cond_signal error\n");
	}

	lStatus = OAL_UnlockMutex(&apComp->mMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release lock\n");
		if (lRet == 0) {
			lRet = lStatus;
		}
	}

complete_exit:
	return lRet;
}

int32_t OAL_CompleteAll(OAL_Completion_t *apComp)
{
	int32_t lStatus, lRet;

	OAL_CHECK_NULL_PARAM(apComp, complete_all_exit);

	lRet = OAL_LockMutex(&apComp->mMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to acquire lock\n");
		goto complete_all_exit;
	}

	apComp->mDone = UINT_MAX;
	lRet          = OAL_BroadcastCondVar(&apComp->mCondVar);
	if (lRet != 0) {
		OAL_LOG_ERROR("cond broadcast error");
	}

	lStatus = OAL_UnlockMutex(&apComp->mMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release lock\n");
		if (lRet == 0) {
			lRet = lStatus;
		}
	}

complete_all_exit:
	return lRet;
}

uint64_t OAL_WaitForCompletionTimeout(OAL_Completion_t *apComp,
                                      uint64_t aTimeout)
{
	int32_t lRet;
	uint64_t lUsecs    = OAL_TicksToUsec(aTimeout);
	uint64_t lTimeLeft = 0U;
	OAL_Timespec_t lStartTs;
	int64_t lElapsedUs = 0;
	uint32_t lDone;

	if (apComp == NULL) {
		goto wait_completion_timeout_end;
	}

	lRet = OAL_LockMutex(&apComp->mMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to acquire lock\n");
		goto wait_completion_timeout_end;
	}

	if (apComp->mDone == 0U) {
		lRet = OAL_GetTime(&lStartTs);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to get time\n");
			lTimeLeft = 0U;
			goto wait_timeout_unlock;
		}

		do {
			lRet = OAL_TimedWaitCondVar(&apComp->mCondVar,
			                            &apComp->mMutex, lUsecs);
			(void)OAL_GetElapsedUs(&lStartTs, &lElapsedUs);
			if (lRet == ETIMEDOUT) {
				lTimeLeft = 0U;
			} else {
				if (lElapsedUs < ((int64_t)lUsecs)) {
					lTimeLeft = OAL_UsecToTicks(
					    lUsecs - (uint64_t)lElapsedUs);
				} else {
					lTimeLeft = 0U;
				}
			}
			lDone = apComp->mDone;
		} while ((lTimeLeft != 0U) && (lDone == 0U));

	} else {
		lTimeLeft = aTimeout;
	}

	lDone = apComp->mDone;
	if (lDone != 0U) {
		apComp->mDone--;
	}

wait_timeout_unlock:
	lRet = OAL_UnlockMutex(&apComp->mMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to release lock\n");
	}

wait_completion_timeout_end:
	return lTimeLeft;
}
