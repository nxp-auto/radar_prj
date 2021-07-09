/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_UPTIME_H
#define OAL_UPTIME_H

#ifndef OAL_HZ
#define OAL_HZ 100U
#endif

#ifndef OAL_BILLION
#define OAL_BILLION 1000000000ULL
#endif

#ifndef OAL_MILLION
#define OAL_MILLION 1000000ULL
#endif

#include <oal_utils.h>
#include <os_oal_uptime.h>

/**
 * @defgroup OAL_Uptime OAL System uptime and ticks conversion utilities
 *
 * @{
 * @brief System ticks conversion utilities
 */

__BEGIN_DECLS

/**
 * @brief OAL_Uptime Return uptime ticks
 *
 * @return The elapsed ticks during the uptime (<tt>jiffies</tt>).
 */
uint64_t OAL_GetUptime(void);

/**
 * @brief ticks2usec Convert ticks to microseconds
 *
 * @param[in] aTicks Number of ticks
 *
 * @return Number of microseconds corresponding to the given number of ticks
 */
static inline uint64_t OAL_TicksToUsec(uint64_t aTicks)
{
	return (uint32_t)(aTicks * (OAL_MILLION / OAL_HZ));
}

/**
 * @brief OAL_sec2ticks Convert seconds to aTicks
 *
 * @param[in] aSec Number of seconds
 *
 * @return Number of aTicks corresponding to the given time interval
 */
static inline uint64_t OAL_SecToTicks(uint64_t aSec) { return aSec * OAL_HZ; }
/**
 * @brief OAL_nsec2ticks Convert nanoseconds to ticks
 *
 * @param[in] aNsec Number of nanoseconds
 *
 * @return Number of ticks corresponding to the given time interval
 */
static inline uint64_t OAL_NsecToTicks(uint64_t aNsec)
{
	uint64_t lTicks = aNsec;
	lTicks          = (lTicks * OAL_HZ) / OAL_BILLION;
	return (uint64_t)lTicks;
}

/**
 * @brief Convert microseconds to lTicks
 *
 * @param[in] aUsec Number of microseconds
 *
 * @return Number of lTicks corresponding to the given time interval
 */
static inline uint64_t OAL_UsecToTicks(uint64_t aUsec)
{
	uint64_t lTicks = aUsec;
	lTicks          = (lTicks * OAL_HZ) / OAL_MILLION;
	return (uint64_t)lTicks;
}

__END_DECLS

/* @} */

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_uptime.h>
#endif
#endif /* OAL_UPTIME_H */
