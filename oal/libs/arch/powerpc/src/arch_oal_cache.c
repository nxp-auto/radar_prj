/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <arch_oal_cache.h>
#include <ppc_core_regs.h>

uint32_t OAL_ARCH_GetCacheLine(void)
{
	uint32_t lCacheConfigReg;
	uint32_t lCacheSize;

	OAL_ASM_READ_SPECIAL_REG(OAL_PPC_L1CFG0, lCacheConfigReg);
	switch ((lCacheConfigReg >> OAL_L1CFG0_CBSIZE_BIT) & 0x3U) {
		case 0x0:
			lCacheSize = 32U;
			break;
		case 0x1:
			lCacheSize = 64U;
			break;
		case 0x2:
			lCacheSize = 128U;
			break;
		default:
			OAL_LOG_ERROR("Unknow cache size");
			lCacheSize = 0u;
			break;
	}
	return lCacheSize;
}
