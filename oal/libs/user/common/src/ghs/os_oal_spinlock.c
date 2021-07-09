/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_spinlock.h>

int32_t OAL_InitSpinLock(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -1;
	} else {
		apLock->mSpinValue = 0U;
	}
	return lRet;
}

int32_t OAL_LockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -1;
	} else {
		while (TestAndSet(&apLock->mSpinValue, 0, 1) != Success) {
		};
	}
	return lRet;
}

int32_t OAL_UnlockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -1;
	} else {
		while (TestAndSet(&apLock->mSpinValue, 1, 0) != Success) {
		};
	}
	return lRet;
}

int32_t OAL_InitIRQSpinLock(OAL_irqspinlock_t *apLock)
{
	return OAL_InitSpinLock(apLock);
}

int32_t OAL_LockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = 0;
	OAL_UNUSED_ARG(apFlags);
	if (apLock == NULL) {
		lRet = -1;
	} else {
		while (INTERRUPT_BooleanTestAndSet(&apLock->mSpinValue) != 0U) {
		};
	}
	return lRet;
}

int32_t OAL_UnlockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = 0;
	OAL_UNUSED_ARG(apFlags);
	if (apLock == NULL) {
		lRet = -1;
	} else {
		while (INTERRUPT_AtomicSwap(&apLock->mSpinValue, 0) == 1U) {
		};
	}
	return lRet;
}
