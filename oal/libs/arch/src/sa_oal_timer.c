/*
 * Copyright 2018-2020, 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_utils.h>
#include <oal_irq_utils.h>
#include <oal_timer.h>
#include <arch_oal_timer.h>
#include <arch_oal_timespec.h>

#define NEVER_EXPIRE_VAL (0xffffffffffffffffULL)

#ifndef OAL_MAX_SW_VIRT_TIMERS
#define OAL_MAX_SW_VIRT_TIMERS (32)
#endif

#define MAX_POSITIVE_COUNTER_VALUE (0x7FFFFFFFU)

struct TimerEntry {
	OAL_Timer_t *mpTimer;
	uint8_t mRegistered;
	uint64_t mExpCompValue;
};

static uint32_t gsRegisteredTimers;
static int32_t gsIrqRegistered;
static struct TimerEntry gsTimerEntryVector[OAL_MAX_SW_VIRT_TIMERS];
static struct TimerEntry *gpsCurrentTimer;

/**
 * Below code uses one single physical timer in EL0 for multiple
 * software virtual timers (!= physical virtual timers) and updates
 * compared value of the timer every time when and interrupt is triggered.
 *
 * The compared value of the timer is always determined as the lowest
 * expiration value of the software timers.
 */
static void scheduleNexTimer(void)
{
	size_t lI;
	uint64_t lMinTimeout   = NEVER_EXPIRE_VAL;
	int32_t lMinTimerIndex = -1;

	/* Identify next software timer */
	for (lI = 0; lI < OAL_ARRAY_SIZE(gsTimerEntryVector); lI++) {
		if ((gsTimerEntryVector[lI].mRegistered != 0U) &&
		    (gsTimerEntryVector[lI].mpTimer->mActive != 0U) &&
		    (gsTimerEntryVector[lI].mExpCompValue < lMinTimeout)) {
			lMinTimeout    = gsTimerEntryVector[lI].mExpCompValue;
			lMinTimerIndex = (int32_t)lI;
		}
	}

	if (lMinTimerIndex >= 0) {
		/* Write next timeout value */
		OAL_SA_ARCH_SetTimerTimeout(lMinTimeout);

		gpsCurrentTimer = &gsTimerEntryVector[lMinTimerIndex];
	} else {
		gpsCurrentTimer = NULL;
	}
}

static OAL_IRQ_HANDLER(timerIrqHandler)
{
	OAL_UNUSED_ARG(irq_no);
	OAL_UNUSED_ARG(irq_data);

	/* Disable timer interrupts */
	OAL_SA_ARCH_MaskTimerInterrupts();

	if (gpsCurrentTimer != NULL) {
		OAL_Timer_t *lpTimer = gpsCurrentTimer->mpTimer;
		(lpTimer->mCallback)(lpTimer->mData);
		lpTimer->mActive = 0;
		scheduleNexTimer();
	}

	/* Enable interrupts only if it makes sense */
	if (gpsCurrentTimer != NULL) {
		OAL_SA_ARCH_UnmaskTimerInterrupts();
	}

	return 0;
}

static inline int32_t initHWTimer(void)
{
	int32_t lRet;
	OAL_SA_ARCH_ClearTimerState();

	lRet = OAL_SA_ARCH_RegisterTimerIrq(timerIrqHandler);
	if (lRet == 0) {
		gsIrqRegistered = 1;
	}

	return lRet;
}

void OAL_ARCH_SetTimerTimeout(OAL_Timer_t *apT, uint64_t aNsec)
{
	apT->mExpires = (uint64_t)OAL_Nsec2timerTicks(aNsec);
}

static void addUpdateIrqTimeout(struct TimerEntry *apT)
{
	OAL_Timer_t *lpTimer = apT->mpTimer;
	uint64_t lTime;

	/* Keep timeout as an unsigned value */
	if (lpTimer->mExpires > MAX_POSITIVE_COUNTER_VALUE) {
		lTime = MAX_POSITIVE_COUNTER_VALUE;
	} else {
		lTime = lpTimer->mExpires;
	}

	if (OAL_SA_ARCH_IsTimerEnabled() != 0U) {
		OAL_SA_ARCH_MaskTimerInterrupts();
	}

	gsRegisteredTimers++;

	OAL_SA_ARCH_UpdateHWTimerTimeout(lTime, &apT->mExpCompValue);

	/* Select next timer */
	scheduleNexTimer();

	OAL_SA_ARCH_EnableAndUnmaskTimer();
}

int32_t OAL_SA_ARCH_AddTimer(OAL_Timer_t *apT)
{
	size_t lI;
	int32_t lRet = -1;

	if (gsIrqRegistered == 0) {
		lRet = initHWTimer();
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to initialize HW timer\n");
			goto add_timer_exit;
		}
	}

	if (apT->mExpires == 0U) {
		goto add_timer_exit;
	}

	for (lI = 0; lI < OAL_ARRAY_SIZE(gsTimerEntryVector); lI++) {
		if (gsTimerEntryVector[lI].mRegistered == 0U) {
			gsTimerEntryVector[lI].mpTimer          = apT;
			gsTimerEntryVector[lI].mpTimer->mActive = 1;
			gsTimerEntryVector[lI].mRegistered      = 1U;
			gsTimerEntryVector[lI].mExpCompValue = NEVER_EXPIRE_VAL;
			addUpdateIrqTimeout(&gsTimerEntryVector[lI]);
			lRet = 0;
			break;
		}
	}

add_timer_exit:
	return lRet;
}

int32_t OAL_SA_ARCH_DelTimer(OAL_Timer_t *apT)
{
	size_t lI;
	int32_t lRet   = 0;
	int32_t lCheck = 0;

	for (lI = 0; lI < OAL_ARRAY_SIZE(gsTimerEntryVector); lI++) {
		if ((gsTimerEntryVector[lI].mRegistered != 0U) &&
		    (gsTimerEntryVector[lI].mpTimer == apT)) {
			/*
			 * Mark software timer as inactive and
			 * re-run scheduler
			 */
			OAL_SA_ARCH_MaskTimerInterrupts();
			lRet = (int32_t)gsTimerEntryVector[lI].mpTimer->mActive;
			gsTimerEntryVector[lI].mRegistered = 0U;
			gsRegisteredTimers--;
			if (lRet != 0) {
				scheduleNexTimer();
			}
			OAL_SA_ARCH_UnmaskTimerInterrupts();

			lCheck = 1;
		}
	}

	if (lCheck == 0) {
		lRet = -1;
	}

	if (gsRegisteredTimers == 0U) {
		OAL_SA_ARCH_ClearTimerState();
		gsIrqRegistered = 0;
	}

	return lRet;
}
