/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OS_OAL_SPINLOCK_H
#define OS_OAL_SPINLOCK_H

#include "oal_utils.h"
#include <linux/spinlock.h>

typedef spinlock_t OAL_spinlock_t;
typedef spinlock_t OAL_irqspinlock_t;

static inline int OAL_spin_lock_init(OAL_spinlock_t *lock)
{
	if (lock == NULL) {
		return -EINVAL;
	}

	spin_lock_init(lock);
	return 0;
}

static inline int OAL_spin_lock(OAL_spinlock_t *lock)
{
	if (lock == NULL) {
		return -EINVAL;
	}

	spin_lock(lock);
	return 0;
}

static inline int OAL_spin_unlock(OAL_spinlock_t *lock)
{
	if (lock == NULL) {
		return -EINVAL;
	}

	spin_unlock(lock);
	return 0;
}

static inline int OAL_irqspin_lock_init(OAL_irqspinlock_t *lock)
{
	if (lock == NULL) {
		return -EINVAL;
	}

	spin_lock_init(lock);
	return 0;
}

static inline int OAL_spin_lock_irqsave(OAL_irqspinlock_t *lock,
					unsigned long *flags)
{
	if ((lock == NULL) || (flags == NULL)) {
		return -EINVAL;
	}

	spin_lock_irqsave(lock, *flags);
	return 0;
}

static inline int OAL_spin_unlock_irqrestore(OAL_irqspinlock_t *lock,
						unsigned long *flags)
{
	if ((lock == NULL) || (flags == NULL)) {
		return -EINVAL;
	}

	spin_unlock_irqrestore(lock, *flags);
	return 0;
}

#endif
