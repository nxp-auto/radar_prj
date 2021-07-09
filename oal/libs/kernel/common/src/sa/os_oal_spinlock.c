/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_spinlock.h>
#include <arch_oal_spinlock.h>

int32_t OAL_InitSpinLock(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -1;
	} else {
		apLock->mValue = 0U;
	}
	return lRet;
}

int32_t OAL_LockSpin(OAL_spinlock_t *apLock)
{
	return OAL_ARCH_LockSpin(apLock);
}

int32_t OAL_UnlockSpin(OAL_spinlock_t *apLock)
{
	return OAL_ARCH_UnlockSpin(apLock);
}

int32_t OAL_InitIRQSpinLock(OAL_irqspinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -EINVAL;
	} else {
		*apLock = 0U;
	}
	return lRet;
}

int32_t OAL_LockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = 0;

	if ((apFlags == NULL) || (apLock == NULL)) {
		lRet = -EINVAL;
		goto spinlock_irqsave_end;
	}

	lRet = OAL_ARCH_LockIRQSpin(apLock, apFlags);

spinlock_irqsave_end:
	return lRet;
}

int32_t OAL_UnlockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = 0;

	if ((apFlags == NULL) || (apLock == NULL)) {
		lRet = -EINVAL;
		goto spinlock_irqrestore_end;
	}

	lRet = OAL_ARCH_UnlockIRQSpin(apLock, apFlags);

spinlock_irqrestore_end:
	return lRet;
}
