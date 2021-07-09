/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_bitset.h>
#include <oal_log.h>
#include <oal_mutex.h>
#include <oal_once.h>
#include <oal_timespec.h>
#include <oal_uptime.h>
#include <ghs_alarm_utils.h>

#define OAL_MAX_THREADS_PER_VAS 100U

struct GHSClock {
	Clock mClock;
	Time mAlarm;
};

/* Pool of clocks, one per thread */
static OAL_DECLARE_BITSET(gClocksBitset, OAL_MAX_THREADS_PER_VAS);
/* Mutual access on pool */
static struct OAL_Mutex gsClocksMutex;

static void timespecConstructor(void)
{
	int32_t lRet;
	lRet = OAL_INIT_BITSET(&gClocksBitset[0], OAL_MAX_THREADS_PER_VAS);
	if (lRet == 0) {
		lRet = OAL_InitializeMutex(&gsClocksMutex);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to initialize clocks pool\n");
		}
	} else {
		OAL_LOG_ERROR("Failed to initialize the bitset\n");
	}
}

static int32_t getFreeGhsClock(struct GHSClock **apClock)
{
	int32_t lRet = 0;
	uint64_t lFirstUnused;
	static struct GHSClock lsClocks[OAL_MAX_THREADS_PER_VAS];

	if (apClock == NULL) {
		OAL_LOG_ERROR("Invalid argument\n");
		lRet = -1;
		goto get_free_clock_exit;
	}

	lRet = OAL_LockMutex(&gsClocksMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to acquire mutex\n");
		goto get_free_clock_exit;
	}

	lRet = OAL_BITSET_GET_UNUSED_BIT(gClocksBitset, &lFirstUnused);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get an unused bit\n");
		goto unlock_mutex;
	}

	if (lFirstUnused == OAL_BITSET_CHUNK_MASK) {
		lRet = -1;
		OAL_LOG_ERROR("Failed to get an unused clock\n");
	} else {
		*apClock = &lsClocks[lFirstUnused];
	}

unlock_mutex:
	(void)OAL_UnlockMutex(&gsClocksMutex);

get_free_clock_exit:
	return lRet;
}

static void clockCleanup(void *apData)
{
	struct GHSClock *lpClock = (struct GHSClock *)apData;
	if (apData == NULL) {
		goto clock_cleanup_exit;
	}

	(void)SetClockAlarm(lpClock->mClock, false, NULLTime, NULLTime);

clock_cleanup_exit:
	return;
}

static Error initTaskKey(struct GHSClock **apClock)
{
	Error lError;
	int32_t lRet;
	struct GHSClock *lpClock;

	/* Initialization for pool and mutex */
	static struct OAL_OnceControl lsClocksInitialized = OAL_ONCE_INIT;

	/* TLS key for each thread */
	static TaskKey *lpsSleepClockKey;

	if (apClock == NULL) {
		OAL_LOG_ERROR("Invalid argument\n");
		lError = ArgumentError;
		goto init_task_key_exit;
	}

	if (lpsSleepClockKey == NULL) {
		lError = CreateTaskKey(&lpsSleepClockKey, clockCleanup);
		if ((lError != Success) &&
		    (lError != OperationAlreadyPerformed)) {
			OAL_LOG_ERROR("Failed to create a new task key (%s)\n",
			              ErrorString(lError));
			goto init_task_key_exit;
		}
	}

	(void)OAL_ExecuteOnce(&lsClocksInitialized, timespecConstructor);

	lError = GetTaskKey(lpsSleepClockKey, (void **)apClock);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get the clock (%s)\n", lError);
		goto init_task_key_exit;
	}

	if (*apClock != NULL) {
		/* Cached value from TLS */
		lError = Success;
		goto init_task_key_exit;
	}

	lRet = getFreeGhsClock(apClock);
	if (lRet != 0) {
		lError = ArgumentError;
		goto init_task_key_exit;
	}

	lpClock                  = *apClock;
	lpClock->mAlarm.Seconds  = 0;
	lpClock->mAlarm.Fraction = 0U;

	lError = CreateVirtualClock(
	    HighResClock, ((Value)CLOCK_ALARM) | ((Value)CLOCK_READTIME),
	    &lpClock->mClock);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to create a new clock (%s)\n",
		              ErrorString(lError));
		goto init_task_key_exit;
	}

	lError = SetTaskKey(lpsSleepClockKey, lpClock);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to set task key (%s)\n",
		              ErrorString(lError));
		goto init_task_key_exit;
	}

init_task_key_exit:
	return lError;
}

int32_t OAL_GetTime(OAL_Timespec_t *apTm)
{
	Time lTime;
	int32_t lRet = 0;
	Error lError;

	if (apTm == NULL) {
		lRet = -EINVAL;
		goto get_time_exit;
	}

	lError = GetClockTime(HighResClock, &lTime);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get time (%s)\n", ErrorString(lError));
		lRet = -EINVAL;
		goto get_time_exit;
	}

	apTm->mSec  = lTime.Seconds;
	apTm->mNsec = (int64_t)OAL_GHS_FractionToNsec(lTime.Fraction);

get_time_exit:
	return lRet;
}

int32_t OAL_Sleep(const OAL_Timespec_t *acpTm)
{
	int32_t lRet = 0;
	Time lGhsTime;
	Error lError;
	struct GHSClock *lpClock = NULL;

	if (acpTm == NULL) {
		lRet = -EINVAL;
		goto sleep_exit;
	}

	lError = initTaskKey(&lpClock);
	if (lError != Success) {
		lRet = -EINVAL;
		goto sleep_exit;
	}

	if (lpClock == NULL) {
		OAL_LOG_ERROR("TLS corruption detected\n");
		lRet = -EINVAL;
		goto sleep_exit;
	}

	lGhsTime.Seconds  = acpTm->mSec;
	lGhsTime.Fraction = OAL_GHS_NsecToFraction((uint64_t)acpTm->mNsec);

	if (!((lpClock->mAlarm.Seconds == lGhsTime.Seconds) &&
	      (lpClock->mAlarm.Fraction == lGhsTime.Fraction))) {
		/* If alarm was used before */
		if ((lpClock->mAlarm.Fraction != 0U) ||
		    (lpClock->mAlarm.Seconds != 0)) {
			(void)OAL_GHS_ClearAlarm(lpClock->mClock);
		}
		lError =
		    SetClockAlarm(lpClock->mClock, true, NULLTime, &lGhsTime);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to set alarm (%s)\n",
			              ErrorString(lError));
			lRet = -EINVAL;
			goto sleep_exit;
		}
		lpClock->mAlarm = lGhsTime;
	}

	lError = SynchronousReceive((Connection)lpClock->mClock, NULL);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed wait on alarm (%s)\n",
		              ErrorString(lError));
		lRet = -EINVAL;
		goto sleep_exit;
	}

sleep_exit:
	return lRet;
}
