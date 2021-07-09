/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <atomic.h>
#include <stddef.h>

#include <sync.h>

#include <sched.h>
#include <time.h>

#include <oal_timer.h>

#define TIMERS_PULSE_CODE _PULSE_CODE_MINAVAIL
#define TIMERS_EXIT_CODE (_PULSE_CODE_MINAVAIL + 1)

#define INVALID_TIMER_ID (-1)
#define INVALID_THREAD_STATE ((uint32_t)(-1))

#define TIMER_STATE_MASK ((uint32_t)0U)
#define TIMER_RUNNING_STATE ((uint32_t)1U)
#define TIMER_INVOKED_STATE ((uint32_t)2U)

static int32_t setRTPriority(pthread_t aThread)
{
	int32_t lRet;
	struct sched_param lParams;

	(void)memset(&lParams, 0, sizeof(lParams));
	lParams.sched_priority = sched_get_priority_max(SCHED_FIFO);

	/* Attempt to set real-time priority */
	lRet = pthread_setschedparam(aThread, SCHED_FIFO, &lParams);
	if (lRet != 0) {
		OAL_LOG_ERROR("pthread_setschedparam fail :%s\n",
		              strerror(lRet));
	}
	return lRet;
}

static void *timer_pulses(void *apArg)
{
	OAL_Timer_t *lpTimer = (OAL_Timer_t *)apArg;
	struct _pulse lPulse;
	int32_t lRcvid;
	int32_t lRet;

	(void)atomic_clr_value(&lpTimer->mState, INVALID_THREAD_STATE);
	(void)atomic_set_value(&lpTimer->mState, TIMER_RUNNING_STATE);

	lRet = pthread_barrier_wait(&lpTimer->mBarrier);
	if ((lRet != PTHREAD_BARRIER_SERIAL_THREAD) && (lRet != 0)) {
		OAL_LOG_ERROR("pthread_barrier_wait failed %d\n", lRet);
		goto timer_pulses_exit;
	} else {
		lRet = 0;
	}


	do {
		lRcvid = MsgReceive(lpTimer->mChannelId, &lPulse,
		                    sizeof(lPulse), NULL);

		// Pulse
		if (lRcvid == 0) {
			if ((lPulse.code == TIMERS_PULSE_CODE) &&
			    (lpTimer->mCallback != NULL)) {
				// Mark the timer as invoked
				(void)atomic_set_value(&lpTimer->mState,
				                       TIMER_INVOKED_STATE);
				lpTimer->mCallback(lpTimer->mData);
			} else if (lPulse.code == TIMERS_EXIT_CODE) {
				// Canceling an active timer
				break;
			} else {
				OAL_LOG_ERROR("Uknown pulse id: %u\n",
				              lPulse.code);
				lRet = EINVAL;
				break;
			}
		}
	} while (atomic_set_value(&lpTimer->mState, TIMER_STATE_MASK) != 0U);

	(void)atomic_set_value(&lpTimer->mState, INVALID_THREAD_STATE);
timer_pulses_exit:
	pthread_exit((void *)(uintptr_t)lRet);
}

int32_t OAL_SetupTimer(OAL_Timer_t *apTimer, OAL_Timer_callback_t aCallback,
                       uintptr_t aData, uint32_t aFlags)
{
	int32_t lRet = 0;

	OAL_UNUSED_ARG(aFlags);

	if (apTimer == NULL) {
		lRet = -EINVAL;
		OAL_LOG_ERROR("Invalid argument\n");
		goto init_timer_exit;
	}

	apTimer->mCallback = aCallback;
	apTimer->mData     = aData;
	apTimer->mTimerId  = INVALID_TIMER_ID;

	(void)atomic_set_value(&apTimer->mState, INVALID_THREAD_STATE);

	lRet = pthread_barrier_init(&apTimer->mBarrier, NULL, 2);
	if (lRet != 0) {
		OAL_LOG_ERROR("barrier_init fail\n");
		goto init_timer_exit;
	}

	apTimer->mChannelId = ChannelCreate(0);
	if (apTimer->mChannelId == -1) {
		lRet = -EIO;
		OAL_LOG_ERROR("ChannelCreate fail\n");
		goto barrier_destroy;
	}

	apTimer->mEvent.sigev_notify   = SIGEV_PULSE;
	apTimer->mEvent.sigev_priority = (short)SS_REPL_MAX;
	apTimer->mEvent.sigev_code     = TIMERS_PULSE_CODE;
	apTimer->mEvent.sigev_coid     = ConnectAttach(
            ND_LOCAL_NODE, 0, apTimer->mChannelId, _NTO_SIDE_CHANNEL, 0);
	if (apTimer->mEvent.sigev_coid == -1) {
		lRet = -EIO;
		OAL_LOG_ERROR("ConnectAttach fail\n");
		goto destroy_channel;
	}

	lRet =
	    timer_create(CLOCK_MONOTONIC, &apTimer->mEvent, &apTimer->mTimerId);
	if (lRet != 0) {
		OAL_LOG_ERROR("timer_create fail\n");
		goto detach_conn;
	}

	lRet = pthread_create(&apTimer->mThread, NULL, timer_pulses, apTimer);
	if (lRet != 0) {
		OAL_LOG_ERROR("pthread_create fail\n");
		goto delete_timer;
	} else {
		// Wait second part of the initialization done by
		// timer_pulses
		lRet = pthread_barrier_wait(&apTimer->mBarrier);
		if (lRet == PTHREAD_BARRIER_SERIAL_THREAD) {
			lRet = 0;
		}
		if (lRet != 0) {
			OAL_LOG_ERROR("pthread_barrier_wait failed\n");
			goto delete_timer;
		}

		lRet = setRTPriority(apTimer->mThread);
	}

	goto init_timer_exit;

delete_timer:
	(void)timer_delete(apTimer->mTimerId);
detach_conn:
	(void)ConnectDetach(apTimer->mEvent.sigev_coid);
destroy_channel:
	(void)ChannelDestroy(apTimer->mChannelId);
barrier_destroy:
	(void)pthread_barrier_destroy(&apTimer->mBarrier);
init_timer_exit:
	return lRet;
}

int32_t OAL_SetTimerTimeout(OAL_Timer_t *apTimer, uint64_t aNSec)
{
	int32_t lRet = 0;

	if (apTimer != NULL) {
		// Disarm the timer
		(void)OAL_DelTimer(apTimer);
		apTimer->mExpires = aNSec;
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_AddTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;
	uint64_t lSec;
	uint64_t lNsec;
	struct itimerspec lTimeSpec;

	if (apTimer == NULL) {
		OAL_LOG_ERROR("Invalid argument\n");
		lRet = -EINVAL;
		goto add_timer_exit;
	}

	lSec  = apTimer->mExpires / OAL_SECOND;
	lNsec = apTimer->mExpires % OAL_SECOND;

	(void)memset(&lTimeSpec, 0, sizeof(lTimeSpec));

	// One / First shot timer
	lTimeSpec.it_value.tv_sec  = (int64_t)lSec;
	lTimeSpec.it_value.tv_nsec = (int64_t)lNsec;

	lRet = timer_settime(apTimer->mTimerId, 0, &lTimeSpec, NULL);
	if (lRet != 0) {
		OAL_LOG_ERROR("timer_settime failed: %s\n", strerror(errno));
	}

add_timer_exit:
	return lRet;
}

int32_t OAL_DelTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;
	uint32_t lState;
	struct itimerspec lTimeSpec;

	if (apTimer == NULL) {
		lRet = -EINVAL;
		goto del_timer_exit;
	}

	// Disarm the timer
	(void)memset(&lTimeSpec, 0, sizeof(lTimeSpec));
	lRet = timer_settime(apTimer->mTimerId, 0, &lTimeSpec, NULL);
	if (lRet != 0) {
		OAL_LOG_ERROR("timer_settime failed: %s\n", strerror(errno));
		goto del_timer_exit;
	}

	lState = atomic_set_value(&apTimer->mState, TIMER_STATE_MASK);
	(void)atomic_clr_value(&apTimer->mState, TIMER_INVOKED_STATE);

	if ((lState & TIMER_INVOKED_STATE) == 0U) {
		lRet = 1;
	}

del_timer_exit:
	return lRet;
}

int32_t OAL_DestroyTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0, lFuncRet;
	int32_t lStatus;
	int64_t lMRet;

	if (apTimer == NULL) {
		lRet = -EINVAL;
		goto destroy_timer_exit;
	}

	if (atomic_set_value(&apTimer->mState, TIMER_STATE_MASK) !=
	    INVALID_THREAD_STATE) {
		(void)atomic_clr_value(&apTimer->mState, INVALID_THREAD_STATE);

		/* Unblock timer thread from MsgReceive */
		lMRet = MsgSendPulse(apTimer->mEvent.sigev_coid, -1,
		                     TIMERS_EXIT_CODE, TIMERS_EXIT_CODE);
		if (lMRet != 0) {
			OAL_LOG_ERROR(
			    "Failed to send EXIT "
			    "message: %s\n",
			    strerror(errno));
			lRet = -EINVAL;
			/*
			 * Will not risk to join if the thread
			 * isn't responsive
			 */
		} else {
			void *lpVoidPtr;
			lRet =
			    pthread_join(apTimer->mThread, &lpVoidPtr);
			if (lRet != 0) {
				OAL_LOG_ERROR("pthread_join failed\n");
			}

			lStatus = (int32_t)(uintptr_t)lpVoidPtr;
			if (lStatus != 0) {
				OAL_LOG_ERROR(
				    "timer thread exit status = %" PRIi32 "\n",
				    lStatus);
				lRet = lStatus;
			}
		}
	}

	lFuncRet = pthread_barrier_destroy(&apTimer->mBarrier);
	if (lFuncRet != 0) {
		if (lRet == 0) {
			lRet = lFuncRet;
		}
		OAL_LOG_ERROR("Failed to destroy the barrier\n");
	}

	lFuncRet = ConnectDetach(apTimer->mEvent.sigev_coid);
	if (lFuncRet != 0) {
		if (lRet == 0) {
			lRet = lFuncRet;
		}
		OAL_LOG_ERROR("ConnectDetach failed: %s\n", strerror(errno));
	}

	lFuncRet = ChannelDestroy(apTimer->mChannelId);
	if (lFuncRet != 0) {
		if (lRet == 0) {
			lRet = lFuncRet;
		}
		OAL_LOG_ERROR("ChannelDestroy failed: %s\n", strerror(errno));
	}

	if (apTimer->mTimerId != INVALID_TIMER_ID) {
		lFuncRet = timer_delete(apTimer->mTimerId);
		if (lFuncRet != 0) {
			if (lRet == 0) {
				lRet = lFuncRet;
			}
			OAL_LOG_ERROR("Failed to destroy timer: %s\n",
			              strerror(errno));
		}
	}

destroy_timer_exit:
	return lRet;
}
