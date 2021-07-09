/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TIMER_H
#define OAL_TIMER_H

#include <oal_utils.h>

/**
 * @defgroup OAL_Timer OAL Timer
 *
 * @{
 * @brief Schedule one-shot job
 * @details
 * Simple API for on one-shot execution of a function at some point
 * in the feature.
 */

#define OAL_MICROSECOND 1000ULL
#define OAL_MILLISECOND (1000ULL * OAL_MICROSECOND)
#define OAL_SECOND (1000ULL * OAL_MILLISECOND)

__BEGIN_DECLS

struct OAL_Timer;
typedef struct OAL_Timer OAL_Timer_t;

/**
 * @brief  Timer callback. This function will be called
 * every time when timer fires.
 *
 * @param aArg [in]  Timer argument.
 */
typedef void (*OAL_Timer_callback_t)(uintptr_t aArg);

/**
 * @brief Initialize a timer with callback function (<tt>aCallback</tt>) and
 * the argument to be passed when the timer expires (<tt>aData</tt>).
 *
 * @param[in,out] apTimer  The timer
 * @param[in] aCallback    The callback called when the timer expires
 * @param[in] aData        The data passed to callback
 * @param[in] aFlags       Timer flags
 *
 * @see OAL_destroy_timer
 *
 * @note All resources acquired using this function must be released using
 * #OAL_DestroyTimer.
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_SetupTimer(OAL_Timer_t *apTimer, OAL_Timer_callback_t aCallback,
                       uintptr_t aData, uint32_t aFlags);

/**
 * @brief Change the timeout of a timer
 *
 * @param[in,out] apTimer  The timer
 * @param[in] aNSec        Timeout in nanoseconds
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_SetTimerTimeout(OAL_Timer_t *apTimer, uint64_t aNSec);

/**
 * @brief Activate the timer
 *
 * @param[in] apTimer The timer to be activated
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_AddTimer(OAL_Timer_t *apTimer);

/**
 * @brief Deactivate the timer.
 *
 * @param[in] apTimer The timer to be deactivated
 *
 * @return
 * 	- 0 for an inactive timer
 * 	- 1 for an active timer
 * 	- a negative value for an error
 */
int32_t OAL_DelTimer(OAL_Timer_t *apTimer);

/**
 * @brief Release all resources acquired via #OAL_SetupTimer.
 *
 * @param[in] apTimer The timer to be released
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_DestroyTimer(OAL_Timer_t *apTimer);

__END_DECLS

/* @} */

#include <legacy/oal_timer_1_0.h>
#include <os_oal_timer.h>

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_timer.h>
#endif
#endif /* OAL_TIMER_H */
