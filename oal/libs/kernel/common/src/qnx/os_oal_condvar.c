/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <os_oal_condvar.h>
#include <posix/posix_oal_condvar.h>

int32_t OAL_InitCondVar(struct OAL_CondVar *apCondVar)
{
	int32_t lRet = 0;
	int32_t lRet1 = 0;
	pthread_condattr_t lAttr;
	if (apCondVar == NULL) {
		lRet = -1;
		goto init_cond_var_exit;
	}
	lRet = pthread_condattr_init(&lAttr);
	if (lRet != 0) {
		OAL_LOG_ERROR(
			"Failed to  Initialize the attribute "
			"object for creating condition variables (%s)\n",
			strerror(lRet));
		goto init_cond_var_exit;
	}
	lRet = pthread_condattr_setclock(&lAttr, CLOCK_MONOTONIC);
	if (lRet != 0) {
		OAL_LOG_ERROR(
			"Failed to set the clock attribute "
			"of a condition-variable attribute object  (%s)\n",
			strerror(lRet));
		goto destroy_attr;
	}
	lRet = pthread_cond_init(&apCondVar->mCond, &lAttr);
	if (lRet != 0) {
		OAL_LOG_ERROR(
			"Failed to Initialize a condition "
			"variable (%s)\n",
			strerror(lRet));
	}

destroy_attr:
	lRet1 = pthread_condattr_destroy(&lAttr);
	if (lRet1 != 0) {
		lRet = lRet1;
		OAL_LOG_ERROR(
		    "Failed to Destroy a condition-variable "
		    "attribute object  (%s)\n",
		    strerror(lRet));
	}
init_cond_var_exit:
	return lRet;
}

int32_t OAL_DestroyCondVar(struct OAL_CondVar *apCondVar)
{
	int32_t lRet = 0;
	if (apCondVar == NULL) {
		lRet = -1;
	} else {
		lRet = pthread_cond_destroy(&apCondVar->mCond);
	}

	return lRet;
}

int32_t OAL_SignalCondVar(struct OAL_CondVar *apCondVar)
{
	int32_t lRet = 0;
	if (apCondVar == NULL) {
		lRet = -1;
	} else {
		lRet = pthread_cond_signal(&apCondVar->mCond);
		if (lRet != 0) {
			OAL_LOG_ERROR(
			    "Failed to signal conditional "
			    "variable (%s)\n",
			    strerror(lRet));
		}
	}

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
	int32_t lRet = 0;
	if ((apCondVar == NULL) || (apMutex == NULL)) {
		lRet = -1;
		goto signal_cond_var_exit;
	}

	lRet = pthread_cond_wait(&apCondVar->mCond, &apMutex->mOSMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to wait ! (%s)\n", strerror(lRet));
	}

signal_cond_var_exit:
	return lRet;
}

int32_t OAL_TimedWaitCondVar(struct OAL_CondVar *apCondVar,
                             struct OAL_Mutex *apMutex, uint64_t aUsec)
{
	int32_t lRet   = EOK;
	uint64_t lNsec = aUsec * (uint64_t)OAL_NSEC_IN_USEC;
	struct timespec lTs;
	uint64_t lNowNs;

	lRet = ClockTime(CLOCK_MONOTONIC, NULL, &lNowNs);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get time (%s)\n", strerror(errno));
		lRet = EINVAL;
	} else {
		nsec2timespec(&lTs, lNsec + lNowNs);
		lRet = pthread_cond_timedwait(&apCondVar->mCond,
		                              &apMutex->mOSMutex, &lTs);
	}

	return lRet;
}

int32_t OAL_BroadcastCondVar(struct OAL_CondVar *apCondVar)
{
	int32_t lRet = -1;

	if (apCondVar == NULL) {
		lRet = -1;
	} else {
		lRet = pthread_cond_broadcast(&apCondVar->mCond);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to send broadcast (%s)\n",
			              strerror(lRet));
		}
	}

	return lRet;
}
