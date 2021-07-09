/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**
 * This file contains prototypes for the architecture specific
 * standalone functions
 */

#ifndef OAL_ARCH_SPINLOCK
#define OAL_ARCH_SPINLOCK

#include <oal_spinlock.h>
#include <os_utils.h>

__BEGIN_DECLS

/**
 * @brief Obtain the lock
 *
 * @param[in] apLock The lock
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_ARCH_LockSpin(OAL_spinlock_t *apLock);

/**
 * @brief  Release the lock
 *
 * @param[in] apLock The lock
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_ARCH_UnlockSpin(OAL_spinlock_t *apLock);

/**
 * @brief Get the lock in IRQ handler
 *
 * @param[in] apLock  The lock
 * @param[in] apFlags Variable to store the IRQ flags
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_ARCH_LockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags);

/**
 * @brief Release the lock
 *
 * @param apLock  The apLock
 * @param apFlags The apFlags to be restored. Must use the same variable
 * as for #OAL_ARCH_LockIRQSpin call.
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_ARCH_UnlockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags);

__END_DECLS

#endif
