/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_ARCH_TIMESPEC_H
#define OAL_ARCH_TIMESPEC_H
#include "oal_timespec.h"
#include "oal_utils.h"

__BEGIN_DECLS

/**
 * @brief Suspends the execution of the calling thread for <tt>nsecs</tt>
 * nanoseconds
 *
 * @param[in] begin The OAL_GetTicks() value at the sleep start
 * @param[in] nsecs Sleep duration
 *
 * @return 0 for success or a negative value for an error
 */
int32_t OAL_SA_nsleep(uint64_t aBegin, uint64_t aNsecs);

/**
 * @brief Get current time value
 *
 * @return Time value
 */
uint64_t OAL_SA_GetTime(void);

/**
 * @brief Get current time value
 *
 * @return Time value in usecs
 */
uint64_t OAL_GettimeUsec(void);

/**
 * @brief Get number of ticks
 *
 * @return 0 success or a negative value for an error
 */
uint64_t OAL_GetTicks(void);

/**
 * @brief Get ticks from microseconds
 *
 * @param[in] aUsec Value in microseconds
 *
 * @return Value in ticks
 */
uint64_t OAL_Usec2timerTicks(uint64_t aUsec);

/**
 * @brief Get ticks from nanoseconds
 *
 * @param[in] aNsec Value in nanoseconds
 *
 * @return Value in timer ticks
 */
uint64_t OAL_Nsec2timerTicks(uint64_t aNsecs);

/**
 * @brief Get clock frequency
 *
 * @return Clock's frequency in HZ
 */
uint64_t OAL_GetTbclk(void);

__END_DECLS
#endif
