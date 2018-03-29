/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_UPTIME_H
#define OAL_UPTIME_H

#ifndef HZ
#define HZ 100U
#endif

#ifndef BILLION
#define BILLION 1000000000ULL
#endif

#ifndef MILLION
#define MILLION 1000000ULL
#endif

#include "oal_utils.h"
#include XSTR(OS/os_oal_uptime.h)

__BEGIN_DECLS

/**
 * @brief OAL_Uptime Return uptime ticks
 *
 * @return The elapsed ticks during the uptime (jiffies).
 */
unsigned long OAL_Uptime(void);

/**
 * @brief ticks2sec Convert ticks to seconds
 *
 * @param ticks[in] Number of ticks
 *
 * @return Number of seconds corresponding to the given number of ticks
 */
static inline double OAL_ticks2sec(unsigned long ticks)
{
	double ret = (double)ticks;
	ret /= (double)HZ;
	return ret;
}

/**
 * @brief ticks2sec Convert ticks to micro seconds
 *
 * @param ticks[in] Number of ticks
 *
 * @return Number of micro seconds corresponding to the given number of ticks
 */
static inline uint32_t OAL_ticks2usec(unsigned long ticks)
{
	return (uint32_t)(ticks * (MILLION / HZ));
}


/**
 * @brief OAL_sec2ticks Convert seconds to ticks
 *
 * @param sec[in] Number of seconds
 *
 * @return Number of ticks corresponding to the given time interval
 */
static inline unsigned long OAL_sec2ticks(uint64_t sec)
{
	return sec * HZ;
}

/**
 * @brief OAL_nsec2ticks Convert nanoseconds to ticks
 *
 * @param nsec[in] Number of nanoseconds
 *
 * @return Number of ticks corresponding to the given time interval
 */
static inline unsigned long OAL_nsec2ticks(uint64_t nsec)
{
	double ticks = nsec;
	ticks = ticks * HZ / BILLION;
	return (unsigned long)ticks;
}

__END_DECLS
#endif /* OAL_UPTIME_H */
