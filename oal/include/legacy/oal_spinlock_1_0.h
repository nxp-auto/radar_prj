/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_SPINLOCK_1_0_H
#define OAL_SPINLOCK_1_0_H

#include <os_oal_spinlock.h>

__BEGIN_DECLS

static inline int32_t OAL_spin_lock_init(OAL_spinlock_t *apLock)
{
	return OAL_InitSpinLock(apLock);
}

static inline int32_t OAL_spin_lock(OAL_spinlock_t *apLock)
{
	return OAL_LockSpin(apLock);
}

static inline int32_t OAL_spin_unlock(OAL_spinlock_t *apLock)
{
	return OAL_UnlockSpin(apLock);
}

static inline int32_t OAL_irqspin_lock_init(OAL_irqspinlock_t *apLock)
{
	return OAL_InitIRQSpinLock(apLock);
}

static inline int32_t OAL_spin_lock_irqsave(OAL_irqspinlock_t *apLock,
                                            uint64_t *apFlags)
{
	return OAL_LockIRQSpin(apLock, apFlags);
}

static inline int32_t OAL_spin_unlock_irqrestore(OAL_irqspinlock_t *apLock,
                                                 uint64_t *apFlags)
{
	return OAL_UnlockIRQSpin(apLock, apFlags);
}

__END_DECLS

#endif /* OAL_SPINLOCK_1_0_H */
