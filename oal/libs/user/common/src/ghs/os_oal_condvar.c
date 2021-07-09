/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <oal_timespec.h>
#include <ghs_alarm_utils.h>
#include <posix/posix_oal_condvar.h>

int32_t OAL_InitCondVar(struct OAL_CondVar *apCondVar)
{
	int32_t lRet = 0;
	Error lError;

	if (apCondVar == NULL) {
		goto init_condvar_exit;
	}

	lError = CreateSemaphore((Value)0U, &apCondVar->mSema);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to create semaphore : %s\n",
		              ErrorString(lError));
		lRet = -1;
		goto init_condvar_exit;
	}

	lError = CreateVirtualClock(
	    HighResClock, ((Value)CLOCK_ALARM) | ((Value)CLOCK_READTIME),
	    &apCondVar->mClock);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to create a new clock (%s)\n",
		              ErrorString(lError));
		(void)CloseSemaphore(apCondVar->mSema);
		lRet = -1;
	}

	apCondVar->mIsAlarmSet   = 0U;
	apCondVar->mClearedAlarm = 0U;

init_condvar_exit:
	return lRet;
}

int32_t OAL_DestroyCondVar(struct OAL_CondVar *apCondVar)
{
	int32_t lRet = 0;
	Error lError;

	if (apCondVar == NULL) {
		lRet = -1;
		goto exit_destroy_cond_var;
	}

	lError = CloseSemaphore(apCondVar->mSema);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to close semaphore (%s)\n",
		              ErrorString(lError));
		lRet = -1;
	}

	lError = CloseClock(apCondVar->mClock);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to close clock (%s)\n",
		              ErrorString(lError));
		lRet = -1;
	}

exit_destroy_cond_var:
	return lRet;
}

int32_t OAL_SignalCondVar(struct OAL_CondVar *apCondVar)
{
	SignedValue lSemVal;
	int32_t lRet = 0;
	Error lError;

	if (apCondVar == NULL) {
		lRet = -1;
		goto signal_cond_exit;
	}

	lError = GetSemaphoreValue(apCondVar->mSema, &lSemVal);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get semaphore's value (%s)\n",
		              ErrorString(lError));
		lRet = -1;
		goto signal_cond_exit;
	}

	if (lSemVal > 0) {
		OAL_LOG_WARNING("Called %s without wait\n", __func__);
	}

	if (apCondVar->mIsAlarmSet != 0U) {
		Time lNow;
		lError = GetClockTime(apCondVar->mClock, &lNow);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to get clock time (%s)\n",
			              ErrorString(lError));
		} else {
			/* Unblock "wait" thread */
			apCondVar->mClearedAlarm = 1U;
			lError = SetClockAlarm(apCondVar->mClock, false, &lNow,
			                       NULLTime);
			if (lError != Success) {
				OAL_LOG_ERROR("Failed to clear clock alarm\n");
			}
		}
	}

	lError = ReleaseSemaphore(apCondVar->mSema);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to release semaphore (%s)\n",
		              ErrorString(lError));
		lRet = -1;
	} else {
		lRet = 0;
	}

signal_cond_exit:
	return lRet;
}

int32_t OAL_BroadcastCondVar(struct OAL_CondVar *apCondVar)
{
	int32_t lRet = -1;
	Error lError;
	SignedValue lSemVal, lIdx;

	if (apCondVar == NULL) {
		OAL_LOG_ERROR("Invalid arguments\n");
		lRet = -1;
		goto broadcast_exit;
	}

	lError = GetSemaphoreValue(apCondVar->mSema, &lSemVal);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get semaphore's value (%s)\n",
		              ErrorString(lError));
		lRet = -1;
		goto broadcast_exit;
	}

	lRet = 0;
	for (lIdx = lSemVal; lIdx < 0; lIdx++) {
		lError = ReleaseSemaphore(apCondVar->mSema);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to release semaphore (%s)\n",
			              ErrorString(lError));
			lRet = -1;
			break;
		}
	}

broadcast_exit:
	return lRet;
}

int32_t OAL_SignalCondVarWithMutex(struct OAL_CondVar *apCondVar,
                                   struct OAL_Mutex *apMutex)
{
	int32_t lRet = 0;
	int32_t lStatus;

	if ((apCondVar == NULL) || (apMutex == NULL)) {
		lRet = -1;
		goto signal_cond_var_exit;
	}

	lStatus = OAL_LockMutex(apMutex);
	if (lStatus != 0) {
		lRet = -1;
		OAL_LOG_ERROR("Failed to take mutex\n");
		goto signal_cond_var_exit;
	}

	lStatus = OAL_SignalCondVar(apCondVar);
	if (lStatus != 0) {
		lRet = -1;
		OAL_LOG_ERROR("Failed signal conditional variable\n");
	}

	lStatus = OAL_UnlockMutex(apMutex);
	if (lStatus != 0) {
		lRet = -1;
		OAL_LOG_ERROR("Failed to release mutex\n");
	}

signal_cond_var_exit:
	return lRet;
}

int32_t OAL_WaitCondVar(struct OAL_CondVar *apCondVar,
                        struct OAL_Mutex *apMutex)
{
	int32_t lRet = 0, lStatus;
	Error lError;

	if ((apCondVar == NULL) || (apMutex == NULL)) {
		lRet = EINVAL;
		goto exit_cond_wait;
	}

	lStatus = OAL_UnlockMutex(apMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to release mutex\n");
		goto exit_cond_wait;
	}

	lError = WaitForSemaphore(apCondVar->mSema);
	if (lError != Success) {
		lRet = -1;
		OAL_LOG_ERROR("Failed to wait on semaphore (%s)\n",
		              ErrorString(lError));
	}

	lStatus = OAL_LockMutex(apMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to take mutex\n");
	}

exit_cond_wait:
	return lRet;
}

static inline int32_t setCondVarAlarm(struct OAL_CondVar *apCondVar,
                                      uint64_t aUsec)
{
	return OAL_GHS_SetOneShotAlarm(apCondVar->mClock,
	                               aUsec * (uint64_t)OAL_NSEC_IN_USEC);
}

int32_t OAL_TimedWaitCondVar(struct OAL_CondVar *apCondVar,
                             struct OAL_Mutex *apMutex, uint64_t aUsec)
{
	int32_t lRet = 0;
	int32_t lStatus;
	Error lError;

	if ((apCondVar == NULL) || (apMutex == NULL)) {
		lRet = EINVAL;
		goto wait_event_timeout_exit;
	}

	lStatus = setCondVarAlarm(apCondVar, aUsec);
	if (lStatus != 0) {
		lRet = EINVAL;
		goto wait_event_timeout_exit;
	}

	apCondVar->mIsAlarmSet = 1U;

	lStatus = OAL_UnlockMutex(apMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to take mutex\n");
		goto wait_event_timeout_exit;
	}

	lError = SynchronousReceive((Connection)apCondVar->mClock, NULL);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed wait on alarm (%s)\n",
		              ErrorString(lError));
		lRet = EINVAL;
	} else {
		lRet = ETIMEDOUT;
	}

	lStatus = OAL_LockMutex(apMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to take mutex\n");
		goto os_oal_wait;
	}

	if (apCondVar->mClearedAlarm != 0U) {
		lRet                     = 0;
		apCondVar->mClearedAlarm = 0U;
	}

	/* Disarm alarm */
	(void)OAL_GHS_ClearAlarm(apCondVar->mClock);
	apCondVar->mIsAlarmSet = 0U;

os_oal_wait:
	if (lRet != ETIMEDOUT) {
		lStatus = OAL_WaitCondVar(apCondVar, apMutex);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed to wait on wait queue\n");
		}
	}

wait_event_timeout_exit:
	return lRet;
}
