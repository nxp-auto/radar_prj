/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_ARCH_TIMER_H
#define OAL_ARCH_TIMER_H
#include "oal_timer.h"

/**
 * @brief The timer is acknowledged by the system and is
 * started
 *
 * @param[in] apT The timer
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_SA_ARCH_AddTimer(OAL_Timer_t *apT);

/**
 * @brief The timer is de-activated
 *
 * @param[in] apT The timer
 *
 * @return 0 for an inactive timer, 1 for an active timer
 * or a negative value for an error
 */
int32_t OAL_SA_ARCH_DelTimer(OAL_Timer_t *apT);

/**
 * @brief Set timer timeout
 *
 * @param[in] apT   The timer
 * @param[in] aNsec The number of nanoseconds from now until timeout
 *
 */
void OAL_ARCH_SetTimerTimeout(OAL_Timer_t *apT, uint64_t aNsec);

void OAL_SA_ARCH_MaskTimerInterrupts(void);
void OAL_SA_ARCH_UnmaskTimerInterrupts(void);
void OAL_SA_ARCH_ClearTimerState(void);
void OAL_SA_ARCH_SetTimerTimeout(uint64_t aTimeout);
uint8_t OAL_SA_ARCH_IsTimerEnabled(void);
uint64_t OAL_SA_ARCH_GetTimerCounter(void);
int32_t OAL_SA_ARCH_RegisterTimerIrq(OAL_irq_handler_t aHandler);
void OAL_SA_ARCH_UpdateHWTimerTimeout(uint64_t aDeltaTicks,
                                  uint64_t *apAbsTimeTicks);
void OAL_SA_ARCH_EnableAndUnmaskTimer(void);

#endif
