/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "oal_log.h"
#include <oal_spinlock.h>

int32_t OAL_InitSpinLock(OAL_spinlock_t *apLock)
{
	int32_t lRet = -EINVAL;
	if (apLock != NULL) {
		lRet = pthread_spin_init(apLock, PTHREAD_PROCESS_SHARED);
	}

	return lRet;
}

int32_t OAL_LockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = -EINVAL;
	if (apLock != NULL) {
		lRet = pthread_spin_lock(apLock);
	}

	return lRet;
}

int32_t OAL_UnlockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = -EINVAL;
	if (apLock != NULL) {
		lRet = pthread_spin_unlock(apLock);
	}

	return lRet;
}

int32_t OAL_InitIRQSpinLock(OAL_irqspinlock_t *apLock)
{
	int32_t lRet = -EINVAL;
	if (apLock != NULL) {
		(void)memset(apLock, 0, sizeof(*apLock));
		lRet = 0;
	}

	return lRet;
}

int32_t OAL_LockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = -EINVAL;
	OAL_CHECK_NULL_PARAM(apFlags, lock_irq_spin_exit);

	if (apLock != NULL) {
		if (ThreadCtl(_NTO_TCTL_IO, NULL) == -1) {
			OAL_LOG_ERROR(
			    "Failed to call thread obtained I/O privileges: "
			    "%s\n",
			    strerror(errno));
			goto lock_irq_spin_exit;
		}
		InterruptLock(apLock);
		lRet = 0;
	}

lock_irq_spin_exit:
	return lRet;
}

int32_t OAL_UnlockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = -EINVAL;
	OAL_CHECK_NULL_PARAM(apFlags, unlock_irq_spin_exit);

	if (apLock != NULL) {
		InterruptUnlock(apLock);
		lRet = 0;
	}

unlock_irq_spin_exit:
	return lRet;
}
