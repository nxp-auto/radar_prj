/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_uptime.h>
#include <arch_oal_timespec.h>

static uint64_t tick_to_time(uint64_t aTick)
{
	uint64_t lTick = aTick;
	uint64_t lDiv  = OAL_GetTbclk();

	lTick *= OAL_HZ;
	lTick /= lDiv;
	return lTick;
}

uint64_t OAL_Usec2timerTicks(uint64_t aUsec)
{
	uint64_t lTicks;
	if (aUsec < 1000U) {
		lTicks = ((aUsec * (OAL_GetTbclk() / 1000U)) + 500U) / 1000U;
	} else {
		lTicks = ((aUsec / 10U) * (OAL_GetTbclk() / 100000U));
	}

	return lTicks;
}

uint64_t OAL_Nsec2timerTicks(uint64_t aNsecs)
{
	uint64_t lTicks;

	if (aNsecs < OAL_MILLION) {
		lTicks = (aNsecs * (OAL_GetTbclk() / OAL_MILLION)) / 1000U;
	} else {
		lTicks = OAL_Usec2timerTicks(aNsecs / 1000U);
	}

	return lTicks;
}

#define GETTICKS_OVERHEAD 8U
int32_t OAL_SA_nsleep(uint64_t aBegin, uint64_t aNsecs)
{
	uint64_t lEnd;

	if (aNsecs < 1000000U) {
		lEnd = aBegin + (aNsecs * (OAL_GetTbclk() / OAL_MILLION) / 1000U) -
		       GETTICKS_OVERHEAD;
	} else {
		lEnd = aBegin + OAL_Usec2timerTicks(aNsecs / 1000U);
	}

	while (OAL_GetTicks() < lEnd) {
	};

	return 0;
}

uint64_t OAL_SA_GetTime(void) { return tick_to_time(OAL_GetTicks()); }
uint64_t OAL_GettimeUsec(void)
{
	uint64_t lTicks = OAL_GetTicks();
	uint64_t lDiv   = OAL_GetTbclk();

	return lTicks / (lDiv / OAL_MILLION);
}
