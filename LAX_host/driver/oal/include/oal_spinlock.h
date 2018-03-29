/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_SPINLOCK_H
#define OAL_SPINLOCK_H

#include "common_stringify_macros.h"
#include XSTR(OS/os_oal_spinlock.h)

/* Each platform must have its own implementation for :
 *  o OAL_spinlock_t
 *  o OAL_irqspinlock_t
 *
 * OAL_spinlock_t    - Guards a critical section
 * OAL_irqspinlock_t - Guards a critical section in an IRQ handler
 */


__BEGIN_DECLS

/**
 * @brief OAL_spin_lock_init Initialize the spinlock
 *
 * @param lock The spinlock to be initialized.
 *
 * @return 0 for success or a negative value for an error
 */
int OAL_spin_lock_init(OAL_spinlock_t *lock);

/**
 * @brief OAL_spin_lock Obtain the lock
 *
 * @param lock The lock
 *
 * @return 0 for success or a negative value for an error
 */
int OAL_spin_lock(OAL_spinlock_t *lock);

/**
 * @brief OAL_spin_unlock Release the lock
 *
 * @param lock The lock
 *
 * @return 0 for success or a negative value for an error
 */
int OAL_spin_unlock(OAL_spinlock_t *lock);

/**
 * @brief OAL_irqspin_lock_init Initialize the spinlock for IRQ handler
 *
 * @param lock The lock to be initialized
 *
 * @return 0 for success or a negative value for an error
 */
int OAL_irqspin_lock_init(OAL_irqspinlock_t *lock);

/**
 * @brief OAL_spin_lock_irqsave Get the lock in IRQ handler
 *
 * @param lock  The lock
 * @param flags A variable to store the IRQ flags
 *
 * @return 0 for success or a negative value for an error
 */
int OAL_spin_lock_irqsave(OAL_irqspinlock_t *lock, unsigned long *flags);

/**
 * @brief OAL_spin_unlock_irqrestore Release the lock
 *
 * @param lock  The lock
 * @param flags The flags to be restored. Must use the same variable
 * as for OAL_spin_lock_irqsave call.
 *
 * @return 
 */
int OAL_spin_unlock_irqrestore(OAL_irqspinlock_t *lock, unsigned long *flags);

__END_DECLS

#endif /* OAL_SPINLOCK_H*/
