/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_SPINLOCK_H
#define OAL_SPINLOCK_H

#include <oal_utils.h>
#include <os_oal_spinlock.h>

/**
 * @defgroup OAL_Spinlock OAL Spinlock
 *
 * @{
 * @brief Mutual exclusion mechanism implemented with spinning
 * @details
 * This mechanism is used to synchronize multiple running execution entities on
 * the same shared data. It's very similar to @ref OAL_Mutex API, the main
 * difference being that @ref OAL_Spinlock will not sleep, it will spin in a
 * loop instead.
 *
 * The spinlocks are usually used when sleeping during an operation is
 * prohibited e.g. interrupt handlers.
 *
 * @see https:/en.wikipedia.org/wiki/Spinlock
 */

__BEGIN_DECLS

/**
 * @brief Initialize the spinlock
 *
 * @param[in] apLock The spinlock to be initialized.
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_InitSpinLock(OAL_spinlock_t *apLock);

/**
 * @brief Obtain the lock
 *
 * @param[in] apLock The lock
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_LockSpin(OAL_spinlock_t *apLock);

/**
 * @brief  Release the lock
 *
 * @param[in] apLock The lock
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_UnlockSpin(OAL_spinlock_t *apLock);

/**
 * @brief Initialize the spinlock for IRQ handler
 *
 * @param[in] apLock The lock to be initialized
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_InitIRQSpinLock(OAL_irqspinlock_t *apLock);

/**
 * @brief Get the lock in IRQ handler
 *
 * @param[in] apLock  The lock
 * @param[in] apFlags A variable to store the IRQ flags
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_LockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags);

/**
 * @brief Release the lock
 *
 * @param[in] apLock  The lock
 * @param[in] apFlags The flags to be restored. Must use the same variable
 * as for #OAL_LockIRQSpin call.
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_UnlockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags);

/* @} */

__END_DECLS

#include <legacy/oal_spinlock_1_0.h>

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_spinlock.h>
#endif
#endif /* OAL_SPINLOCK_H*/
