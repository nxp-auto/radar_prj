/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_spinlock.h>
#include <linux/spinlock.h>

int32_t OAL_InitSpinLock(OAL_spinlock_t *apLock)
{
	int32_t lRet;
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		spin_lock_init(apLock);
		lRet = 0;
	}
	return lRet;
}

int32_t OAL_LockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet;
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		spin_lock(apLock);
		lRet = 0;
	}
	return lRet;
}

int32_t OAL_UnlockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet;
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		lRet = 0;
		spin_unlock(apLock);
	}
	return lRet;
}

int32_t OAL_InitIRQSpinLock(OAL_irqspinlock_t *apLock)
{
	int32_t lRet;
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		lRet = 0;
		spin_lock_init(apLock);
	}
	return lRet;
}

int32_t OAL_LockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	unsigned long lFlags;
	int32_t lRet;

	if ((apLock == NULL) || (apFlags == NULL)) {
		lRet = -EINVAL;
	} else {
		lFlags = *apFlags;
		spin_lock_irqsave(apLock, lFlags);
		*apFlags = lFlags;
		lRet     = 0;
	}
	return lRet;
}

int32_t OAL_UnlockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet;
	unsigned long lFlags;
	if ((apLock == NULL) || (apFlags == NULL)) {
		lRet = -EINVAL;
	} else {
		lFlags = *apFlags;
		spin_unlock_irqrestore(apLock, lFlags);
		*apFlags = lFlags;
		lRet     = 0;
	}
	return lRet;
}
