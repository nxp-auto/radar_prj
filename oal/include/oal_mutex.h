/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_MUTEX_H
#define OAL_MUTEX_H

#include <oal_utils.h>
#include <os_oal_mutex.h>

/**
 * @defgroup OAL_Mutex OAL Mutex
 *
 * @{
 * @brief Basic mutual exclusion mechanism
 * @details
 * This mechanism is used to synchronize multiple running execution entities on
 * the same shared data. There are two main operations, #OAL_LockMutex that
 * takes the lock and #OAL_UnlockMutex that releases the mutual access. The
 * execution between #OAL_LockMutex is guaranteed to be serialized.
 *
 * @see https:/en.wikipedia.org/wiki/Lock_(computer_science)
 */

__BEGIN_DECLS

struct OAL_Mutex;

/**
 * @brief Initialize the mutex
 *
 * @param[in] apLock A reference to an uninitialized mutex or a reference to a
 *                   destroyed mutex.
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
int32_t OAL_InitializeMutex(struct OAL_Mutex *apLock);

/**
 * @brief Take the lock
 *
 * @param[in] apLock A reference to an initialized mutex.
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 *
 * @note Calling #OAL_LockMutex using an uninitialized will lead to an
 * unpredictable behavior.
 */
int32_t OAL_LockMutex(struct OAL_Mutex *apLock);

/**
 * @brief Release the lock
 *
 * @param[in] apLock A reference to an initialized mutex.
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 *
 * @note Calling #OAL_UnlockMutex using an uninitialized will lead to an
 * unpredictable behavior.
 */

int32_t OAL_UnlockMutex(struct OAL_Mutex *apLock);

/**
 * @brief Destroy the mutex.
 *
 * @param[in] apLock A reference to an initialized mutex.
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
int32_t OAL_DestroyMutex(struct OAL_Mutex *apLock);

__END_DECLS

/* @} */

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_mutex.h>
#endif
#endif
