/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <arch_oal_cache.h>
#include <arm64_core_regs.h>

uint32_t OAL_ARCH_GetCacheLine(void)
{
	uint32_t lCacheInfo;
	uint32_t lDCacheLineSize;
	uint32_t lDMinLine;

	OAL_ASM_READ_COPROC_REG(OAL_CACHE_TYPE_REG, lCacheInfo);

	/**
	 * DminLine - Log2 of the number of words in the smallest cache line of
	 * all data and unified caches that the processor controls
	 */
	lDMinLine       = ((lCacheInfo & OAL_D_MIN_LINE_MASK) >> OAL_D_MIN_LINE_POS);
	lDCacheLineSize = (uint32_t)(((uint32_t)4U) << lDMinLine);

	return lDCacheLineSize;
}
