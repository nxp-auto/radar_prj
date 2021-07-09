/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_timespec.h>
#include <oal_uptime.h>
#include <arch_oal_timespec.h>
#include <arm64_core_regs.h>

uint64_t OAL_GetTbclk(void)
{
	uint64_t lFreq;
	OAL_ASM_READ_COPROC_REG(OAL_TIMER_FREQ_REG, lFreq);
	return lFreq;
}

uint64_t OAL_GetTicks(void)
{
	uint64_t lCnt = 0;

#ifdef CONFIG_SYS_FSL_ERRATUM_A008585
	uint64_t lTmp;
#endif

	OAL_ASM_READ_COPROC_REG(OAL_TIMER_COUNTER_REG, lCnt);

#ifdef CONFIG_SYS_FSL_ERRATUM_A008585
	OAL_ASM_READ_COPROC_REG(OAL_TIMER_COUNTER_REG, lTmp);
	while (lTmp != lCnt) {
		OAL_ASM_READ_COPROC_REG(OAL_TIMER_COUNTER_REG, lCnt);
		OAL_ASM_READ_COPROC_REG(OAL_TIMER_COUNTER_REG, lTmp);
	}
#endif

	return lCnt;
}
