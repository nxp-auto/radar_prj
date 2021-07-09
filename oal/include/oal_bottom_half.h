/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_BOTTOM_HALF_H
#define OAL_BOTTOM_HALF_H

#include <oal_utils.h>

/**
 * @defgroup OAL_BottomHalf OAL Bottom Half
 *
 * @{
 * @brief Bottom half API
 * @details
 * This API aims to address second / heavy processing phase of an interrupt
 * handler. Usually all heavy operations which might sleep or use floating point
 * are restricted in interrupt handlers and must be executed out of the
 * interrupt context. This situations can be addressed using OAL Bottom Halves,
 * a work scheduled to be executed in the near feature with enabled interrupts.
 */

__BEGIN_DECLS

struct OAL_BottomHalf;

/**
 * @brief Callback prototype for the function responsible for bottom half
 * processing
 *
 * @param[in] aData Generic function argument
 */
typedef void (*OAL_BottomHalfFunction_t)(uintptr_t aData);

/**
 * @brief Initialize the bottom-half structure
 *
 * @param[in] apBtHalf      The bottom-half to be initialized
 * @param[in] aCallback     The function to be called out of interrupt context
 * @param[in] aCallbackArg  Parameter to be passed to <tt>aCallback</tt>
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
int32_t OAL_InitializeBottomHalf(struct OAL_BottomHalf *apBtHalf,
                                 OAL_BottomHalfFunction_t aCallback,
                                 uintptr_t aCallbackArg);
/**
 * @brief Schedule a bottom-half
 * This function is called from interrupt handler using an initialized
 * bottom-half structure.
 *
 * @param[in] apBtHalf The bottom-half to be scheduled
 * @see #OAL_InitializeBottomHalf
 */
void OAL_ScheduleBottomHalf(struct OAL_BottomHalf *apBtHalf);

/**
 * @brief Destroy bottom-half and release all acquired resources
 *
 * @param[in] apBtHalf The bottom-half
 *
 * @return 0 is the operation succeeded, a negative value otherwise
 */
int32_t OAL_DestroyBottomHalf(struct OAL_BottomHalf *apBtHalf);

__END_DECLS

/* @} */

#include <os_oal_bottom_half.h>
#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_bottom_half.h>
#endif
#endif /* OAL_BOTTOM_HALF_H */
