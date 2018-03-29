/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PRIV_OAL_TIMER_H
#define PRIV_OAL_TIMER_H

#define OAL_MICROSECOND    1000ULL
#define OAL_MILLISECOND    (1000ULL * OAL_MICROSECOND)
#define OAL_SECOND         (1000ULL * OAL_MILLISECOND)

struct OAL_Timer;
typedef struct OAL_Timer OAL_Timer_t;

/**
 * @brief  Timer callback. This function will be called
 * every time when timer fires.
 *
 * @param arg [in]  Timer argument.
 */
typedef void (*OAL_Timer_callback_t)(unsigned long arg);

#endif /* PRIV_OAL_TIMER_H */
