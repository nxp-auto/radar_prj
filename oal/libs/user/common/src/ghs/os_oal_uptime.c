/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_timespec.h>
#include <oal_uptime.h>

uint64_t OAL_GetUptime(void)
{
	uint64_t lTicks = 0;
	Time lTime;
	Error lError;

	lError = GetClockTime(HighResClock, &lTime);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get time (%s)\n", ErrorString(lError));
		goto uptime_exit;
	}

	lTicks = OAL_SecToTicks((uint64_t)lTime.Seconds);
	lTicks += OAL_NsecToTicks(OAL_GHS_FractionToNsec(lTime.Fraction));
uptime_exit:
	return lTicks;
}

uint64_t OAL_GHS_FractionToNsec(uint32_t aGhsFraction)
{
	uint64_t lNsec;
	/*
	 * Equivalent of aGhsFraction * OAL_NSEC_IN_SEC / 0xFFFFFFFFUL
	 * @see time_to_timespec
	 */
	lNsec = aGhsFraction * ((uint64_t)OAL_NSEC_IN_SEC) + (1ULL << 31U);
	lNsec >>= 32U;
	return lNsec;
}

uint32_t OAL_GHS_NsecToFraction(uint64_t aNsec)
{
	uint32_t lFraction;
	uint64_t lFrac64;
	/*
	 * Equivalent of aNsec * 0xFFFFFFFFUL / OAL_NSEC_IN_SEC
	 * @see timespec_to_time
	 */
	lFrac64   = (aNsec << 32U) / (uint64_t)OAL_NSEC_IN_SEC;
	lFraction = (uint32_t)lFrac64;
	return lFraction;
}
