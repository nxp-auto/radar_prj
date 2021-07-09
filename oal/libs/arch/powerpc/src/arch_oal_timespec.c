/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_timespec.h>
#include <oal_uptime.h>
#include <arch_oal_timespec.h>
#include <ppc_core_regs.h>

uint64_t OAL_GetTicks(void)
{
	uint64_t lRet;

	uint32_t lTbu1, lTbu2;
	uint32_t lTbl = 0U;
	do {
		OAL_ASM_READ_SPECIAL_REG(OAL_PPC_TBU_RO, lTbu1);
		OAL_ASM_READ_SPECIAL_REG(OAL_PPC_TBL_RO, lTbl);
		OAL_ASM_READ_SPECIAL_REG(OAL_PPC_TBU_RO, lTbu2);
	} while (lTbu1 != lTbu2);

	lRet = ((((uint64_t)lTbu1) << 32U) | lTbl);

	return lRet;
}

/* 400 MHz */
#define CLK_FREQ (400000000U)

/* Overwrite this symbol if it's not accurate */
__attribute__((weak)) uint64_t OAL_GetTbclk(void) { return CLK_FREQ; }
