/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TIMER_H
#define OAL_TIMER_H

#include "priv_oal_timer.h"
#include "common_stringify_macros.h"
#include XSTR(OS/os_oal_timer.h)


__BEGIN_DECLS
/**
 * @brief OAL_setup_timer Initialize a timer. All resources acquired using this
 * function must be released using OAL_destroy_timer.
 *
 * @param t [in]        The timer
 * @param callback [in] The callback called when the timer expires
 * @param data [in]     The data passed to callback
 * @param flags [in]    Timer flags
 *
 * @see OAL_destroy_timer
 *
 * @return 0 for success or a negative value for an error
 */
int OAL_setup_timer(OAL_Timer_t *t,
			OAL_Timer_callback_t callback,
			unsigned long data,
			uint32_t flags);

/**
 * @brief OAL_set_timer_timeout Change the timeout of the timer
 *
 * @param t    [in] Timer
 * @param nsec [in] Timeout in nanoseconds
 *
 * @return 0 for success or a negative value for an error
 */
int OAL_set_timer_timeout(OAL_Timer_t *t, unsigned long nsec);

/**
 * @brief OAL_add_timer Activate the timer
 *
 * @param timer[in] The timer to be activated
 *
 * @return 0 for success or a negative value for an error
 */
int OAL_add_timer(OAL_Timer_t *t);

/**
 * @brief OAL_del_timer Deactivate the timer.
 *
 * @param t[in] The timer
 *
 * @return 0 for an inactive timer, 1 for an active timer
 * or a negative value for an error
 */
int OAL_del_timer(OAL_Timer_t *t);

/**
 * @brief OAL_destroy_timer Release all resources
 * acquired via OAL_setup_timer.
 *
 * @param t [in] The timer to be released
 */
void OAL_destroy_timer(OAL_Timer_t *t);

__END_DECLS
#endif /* OAL_TIMER_H */
