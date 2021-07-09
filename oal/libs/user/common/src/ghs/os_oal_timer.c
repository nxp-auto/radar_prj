/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <oal_timer.h>
#include <ghs_alarm_utils.h>
#include <ghs_task_utils.h>

#define OAL_TIMER_TASK "OAL_TIMER_TASK"

#define STATE_MASK ((Address)0xFU)
#define EXIT_STATE ((Address)0x0U)
#define RUNNING_STATE ((Address)0x1U)
#define WAITING_STATE ((Address)0x2U)
#define RESTART_STATE ((Address)0x3U)

static Value processTimerEvents(Address aAddress)
{
	Value lRet = 0;
	Error lError;
	OAL_Timer_t *lpTimer = (OAL_Timer_t *)aAddress;
	Address lOldState;

	if (lpTimer == NULL) {
		lRet = 1;
		goto process_timer_events_exit;
	}

	/* Change state to running */
	lError = AtomicModify(&lpTimer->mState, &lOldState, STATE_MASK,
	                      RUNNING_STATE);
	if (lError != Success) {
		OAL_LOG_ERROR(
		    "Failed to change state to running."
		    "Timer state : 0x%llx\n",
		    lOldState);
	}

	/* While running */
	while (TestAndSet(&lpTimer->mState, RUNNING_STATE, RUNNING_STATE) ==
	       Success) {
		/* If idle */
		if (TestAndSet(&lpTimer->mState, EXIT_STATE, EXIT_STATE) ==
		    Success) {
			break;
		}

		lError = WaitForSemaphore(lpTimer->mSema);
		if (lError != Success) {
			lRet = EINVAL;
			OAL_LOG_ERROR("Failed to wait on semaphore (%s)\n",
			              ErrorString(lError));
			break;
		}

		/* If idle */
		if (TestAndSet(&lpTimer->mState, EXIT_STATE, EXIT_STATE) ==
		    Success) {
			break;
		}

		/* Change state to waiting for messages */
		lError = AtomicModify(&lpTimer->mState, &lOldState, STATE_MASK,
		                      WAITING_STATE);
		if (lError != Success) {
			OAL_LOG_ERROR(
			    "Failed to change state to waiting."
			    "Timer state : 0x%llx\n",
			    lOldState);
		}

		lError = SynchronousReceive((Connection)lpTimer->mClock, NULL);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed wait on alarm (%s)\n",
			              ErrorString(lError));
			lRet = EINVAL;
		} else {
			/* If restarting  */
			if (TestAndSet(&lpTimer->mState, RESTART_STATE,
			               RUNNING_STATE) == Success) {
				continue;
			}
			if (TestAndSet(&lpTimer->mState, EXIT_STATE,
			               EXIT_STATE) == Success) {
				break;
			}

			lError = AtomicModify(&lpTimer->mState, &lOldState,
			                      STATE_MASK, RUNNING_STATE);
			if (lError != Success) {
				OAL_LOG_ERROR(
				    "Failed to change state to running."
				    "Timer state : 0x%llx\n",
				    lOldState);
			}

			/* A little bit of paranoia */
			if (lpTimer->mCallback != NULL) {
				lpTimer->mCallback(lpTimer->mData);
				lpTimer->mActive = 0;
			} else {
				OAL_LOG_ERROR("Invalid timer callback\n");
			}
		}
	}

process_timer_events_exit:
	return lRet;
}

static int32_t initGHSTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;
	Error lError;

	if (apTimer == NULL) {
		lRet = -1;
		goto init_timer_exit;
	}

	lError = CreateSemaphore((Value)0U, &apTimer->mSema);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to create semaphore : %s\n",
		              ErrorString(lError));
		lRet = -1;
		goto init_timer_exit;
	}

	lError = CreateVirtualClock(
	    HighResClock, ((Value)CLOCK_ALARM) | ((Value)CLOCK_READTIME),
	    &apTimer->mClock);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to create a new clock (%s)\n",
		              ErrorString(lError));
		(void)CloseSemaphore(apTimer->mSema);
		lRet = -1;
		goto init_timer_exit;
	}

	lRet = OAL_GHS_CreateAndRunTask(&apTimer->mTask, OAL_TIMER_TASK,
	                                processTimerEvents, (Address)apTimer);
init_timer_exit:
	return lRet;
}

static int32_t unblockAlarm(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;
	Error lError;
	Time lNow;

	if (apTimer == NULL) {
		lRet = -1;
		goto unblock_exit;
	}

	lError = GetClockTime(apTimer->mClock, &lNow);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get clock time (%s)\n",
		              ErrorString(lError));
	} else {
		/* Unblock "wait" thread */
		lError = SetClockAlarm(apTimer->mClock, false, &lNow, NULLTime);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to set clock alarm (%s)\n",
			              ErrorString(lError));
			lRet = -1;
		}
	}

unblock_exit:
	return lRet;
}

int32_t OAL_SetupTimer(OAL_Timer_t *apTimer, OAL_Timer_callback_t aCallback,
                       uintptr_t aData, uint32_t aFlags)
{
	int32_t lRet = 0;
	Error lError;
	char8_t lName[sizeof(OAL_TIMER_TASK)];
	size_t lLen = sizeof(OAL_TIMER_TASK) - 1U;
	Address lLength;

	OAL_UNUSED_ARG(aFlags);
	if ((apTimer == NULL) || (aCallback == NULL)) {
		lRet = -EINVAL;
		goto setup_timer_exit;
	}

	apTimer->mCallback = aCallback;
	apTimer->mData     = aData;
	apTimer->mState    = RUNNING_STATE;
	apTimer->mActive   = 1;

	lError = GetTaskName(apTimer->mTask, lName, sizeof(lName), &lLength);
	if (lError != Success) {
		lRet = initGHSTimer(apTimer);
	} else {
		if (strncmp(lName, OAL_TIMER_TASK,
		            OAL_Min64u((uint64_t)lLen, lLength)) != 0) {
			lRet = initGHSTimer(apTimer);
		} else {
			/* Unblock If waiting messages */
			if (TestAndSet(&apTimer->mState, WAITING_STATE,
			               RESTART_STATE) == Success) {
				lRet = unblockAlarm(apTimer);
				if (lRet != 0) {
					OAL_LOG_ERROR(
					    "Failed to restart timer\n");
				}
			}
		}
	}

setup_timer_exit:
	return lRet;
}

int32_t OAL_DestroyTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;
	Error lError;
	int32_t lStatus;
	Address lOldState;

	if (apTimer == NULL) {
		lRet = -EINVAL;
		goto destroy_timer_exit;
	}

	/* Unblock If waiting messages */
	if (TestAndSet(&apTimer->mState, WAITING_STATE, EXIT_STATE) ==
	    Success) {
		lRet = unblockAlarm(apTimer);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to restart timer\n");
		}
	} else {
		/* Change state to idle */
		lError = AtomicModify(&apTimer->mState, &lOldState, STATE_MASK,
		                      EXIT_STATE);
		if (lError != Success) {
			OAL_LOG_ERROR(
			    "Failed to change state to IDLE."
			    "Timer state : 0x%llx\n",
			    lOldState);
		}
	}

	lError = ReleaseSemaphore(apTimer->mSema);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to release semaphore (%s)\n",
		              ErrorString(lError));
		lRet = -1;
	}

	lStatus = OAL_GHS_JoinTask(&apTimer->mTask);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to join thread\n");
		lRet = -1;
	}

	lError = CloseSemaphore(apTimer->mSema);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to close semaphore (%s)\n",
		              ErrorString(lError));
		lRet = -1;
	}

	lError = CloseClock(apTimer->mClock);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to close clock (%s)\n",
		              ErrorString(lError));
		lRet = -1;
	}

destroy_timer_exit:
	return lRet;
}

int32_t OAL_SetTimerTimeout(OAL_Timer_t *apTimer, uint64_t aNSec)
{
	int32_t lRet = 0;

	if (apTimer == NULL) {
		lRet = -EINVAL;
		goto set_timer_timeout;
	}

	apTimer->mNSec = aNSec;

set_timer_timeout:
	return lRet;
}

int32_t OAL_AddTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;
	Error lError;

	if (apTimer == NULL) {
		lRet = -EINVAL;
		goto add_timer_exit;
	}

	(void)OAL_DelTimer(apTimer);
	lRet = OAL_GHS_SetOneShotAlarm(apTimer->mClock, apTimer->mNSec);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to set alarm\n");
		goto add_timer_exit;
	}

	lError = ReleaseSemaphore(apTimer->mSema);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to release semaphore (%s)\n",
		              ErrorString(lError));
		lRet = -1;
	}

add_timer_exit:
	return lRet;
}

int32_t OAL_DelTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;

	if (apTimer == NULL) {
		lRet = -EINVAL;
		goto del_timer_exit;
	}

	lRet = OAL_GHS_ClearAlarm(apTimer->mClock);
	/* If no errors */
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to clear alarm\n");
	} else {
		lRet = (int32_t)apTimer->mActive;
	}

del_timer_exit:
	return lRet;
}
