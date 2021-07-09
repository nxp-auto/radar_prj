/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_ATOMIC_H
#define OAL_ATOMIC_H

#include <oal_utils.h>
#include <os_oal_atomic.h>

/**
 * @defgroup OAL_Atomic OAL Atomic Operations
 *
 * @{
 * @brief OAL Atomic Operations
 * @details Atomic operations that are guaranteed to be uninterruptible. It
 * represents the fastest inter thread synchronization mechanism but with
 * a limited set of operations.
 *
 *
 * Available operations:
 *  * set / get a value
 *  * adding / subtracting a value
 *  * incrementing / decrementing
 */

__BEGIN_DECLS

struct OAL_Atomic;

/**
 * @brief Set a value of an \ref OAL_Atomic
 *
 * @param[in] apAtomic   Atomic structure
 * @param[in] aNewValue  New value
 * @param[out] apOldValue Old value of \p apAtomic.
 *                        Ignored if set to <tt>NULL</tt>.
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_SetAtomic(struct OAL_Atomic *apAtomic, uint32_t aNewValue,
                      uint32_t *apOldValue);

/**
 * @brief Get value stored by \p apAtomic
 *
 * @param[in] apAtomic   Atomic structure
 * @param[out] apValue   Value stored by \p apAtomic
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_GetAtomic(struct OAL_Atomic *apAtomic, uint32_t *apValue);

/**
 * @brief Increment value stored by an atomic structure
 *
 * @param[in] apAtomic   Atomic structure
 * @param[out] apOldValue Old value of \p apAtomic.
 *                        Ignored if set to <tt>NULL</tt>.
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_IncrementAtomic(struct OAL_Atomic *apAtomic, uint32_t *apOldValue);

/**
 * @brief Decrement value stored by an atomic structure
 *
 * @param[in] apAtomic    Atomic structure
 * @param[out] apOldValue Old value of \p apAtomic.
 *                        Ignored if set to <tt>NULL</tt>.
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_DecrementAtomic(struct OAL_Atomic *apAtomic, uint32_t *apOldValue);

/**
 * @brief Adds a value to an atomic structure
 *
 * @param[in] apAtomic     Atomic structure
 * @param[in] aAddedValue  The value to be added
 * @param[out] apOldValue Old value of \p apAtomic.
 *                        Ignored if set to <tt>NULL</tt>.
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 *
 */
int32_t OAL_AddAtomic(struct OAL_Atomic *apAtomic, uint32_t aAddedValue,
                      uint32_t *apOldValue);

/**
 * @brief Subtract a value from an atomic structure
 *
 * @param[in] apAtomic     Atomic structure
 * @param[in] aSubValue    The value to be subtracted
 * @param[out] apOldValue Old value of \p apAtomic.
 *                        Ignored if set to <tt>NULL</tt>.
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_SubtractAtomic(struct OAL_Atomic *apAtomic, uint32_t aSubValue,
                           uint32_t *apOldValue);

__END_DECLS

/* @} */

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_atomic.h>
#endif
#endif
