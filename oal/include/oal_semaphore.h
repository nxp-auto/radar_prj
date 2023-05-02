/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_SEMA_H
#define OAL_SEMA_H

#include <oal_utils.h>
#include <os_oal_semaphore.h>

/**
 * @defgroup OAL_Sema OAL Sema
 *
 * @brief Basic semaphore mechanism
 *
 * @detail
 * Semaphore is simply a variable that is non-negative and shared between threads. 
 * This variable is used to solve the critical section problem and to achieve process 
 * synchronization in the multiprocessing environment.
 *
 * @see https://man7.org/linux/man-pages/man7/sem_overview.7.html
 */

__BEGIN_DECLS

struct OAL_Sema;

/**
 * @brief Initialize the semaphore
 *
 * @param[in] apSem A reference to an uninitialized semaphore or a reference to a
 *                   destroyed semaphore. 
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
int32_t OAL_InitializeSemaphore(struct OAL_Sema *apSem);

/**
 * @brief Unlock the semaphore
 *
 * @param[in] apSem A reference to an initialized semaphore
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 *
 * @note Calling #OAL_GiveSemaphore will increament the semaphore pointed
 * to by apSem
 */
int32_t OAL_GiveSemaphore(struct OAL_Sema *apSem);

/**
 * @brief Lock the semaphore
 *
 * @param[in] apSem A reference to an initialized semaphore
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 *
 * @note Calling #OAL_TakeSemaphore will decreament the semaphore pointed
 * to by apSem
 */
int32_t OAL_TakeSemaphore(struct OAL_Sema *apSem);

/**
 * @brief Get the value of semphore
 *
 * @param[in] apSem A reference to an initialized semaphore
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
int32_t OAL_GetValueSemaphore(struct OAL_Sema *apSem);

/**
 * @brief Destroy the semaphore
 *
 * @param[in] apSem A reference to an initialized semaphore
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
int32_t OAL_DestroySemaphore(struct OAL_Sema *apSem);

__END_DECLS

#endif