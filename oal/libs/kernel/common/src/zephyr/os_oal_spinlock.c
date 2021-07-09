/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_utils.h>
#include <os_oal_spinlock.h>
#include <spinlock.h>

int32_t OAL_InitSpinLock(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		*apLock = 0U;
	}
	return lRet;
}

int32_t OAL_LockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		/* Loop while the lock is taken */
		while (*apLock == 1U) {
		};
		*apLock = 1U;
	}

	return lRet;
}

int32_t OAL_UnlockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		*apLock = 0U;
	}
	return lRet;
}

int32_t OAL_InitIRQSpinLock(OAL_irqspinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -EINVAL;
	}
	return lRet;
}

int32_t OAL_LockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = 0;
	OAL_UNUSED_ARG(apFlags);
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		apLock->mKey = k_spin_lock(&apLock->mLock);
	}
	return lRet;
}

int32_t OAL_UnlockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = 0;
	OAL_UNUSED_ARG(apFlags);
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		k_spin_unlock(&apLock->mLock, apLock->mKey);
	}
	return lRet;
}
