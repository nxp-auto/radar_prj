/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_uptime.h>
#include <time.h>

uint64_t OAL_GetUptime(void)
{
	struct timespec lTime;
	int32_t lRet;
	uint64_t lTicks = 0;

	lRet = clock_gettime(CLOCK_MONOTONIC, &lTime);
	if (lRet == -1) {
		OAL_LOG_ERROR("Failed to get uptime (%s)\n", strerror(lRet));
	} else {
		lTicks = OAL_SecToTicks((uint64_t)lTime.tv_sec);
		lTicks += OAL_NsecToTicks((uint64_t)lTime.tv_nsec);
	}

	return lTicks;
}
