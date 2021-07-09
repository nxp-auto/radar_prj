/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_memmap.h>
#include <stddef.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-fpermissive"

#include <hw/inout.h>

#pragma GCC diagnostic pop

uint8_t OAL_Read8(uintptr_t aAddr) { return in8(aAddr); }
uint16_t OAL_Read16(uintptr_t aAddr) { return in16(aAddr); }
uint32_t OAL_Read32(uintptr_t aAddr) { return in32(aAddr); }
uint64_t OAL_Read64(uintptr_t aAddr)
{
	uint64_t lVal;
	uintptr_t lValAddr = (uintptr_t)&lVal;
	uintptr_t lAddr    = (uintptr_t)aAddr;

	uint32_t *lpVal32 = (uint32_t *)lValAddr;

	*lpVal32       = in32(lAddr);
	*(lpVal32 + 1) = in32(lAddr + sizeof(*lpVal32));

	return lVal;
}

void OAL_Write8(uintptr_t aAddr, uint8_t aValue) { out8(aAddr, aValue); }
void OAL_Write16(uintptr_t aAddr, uint16_t aValue) { out16(aAddr, aValue); }
void OAL_Write32(uintptr_t aAddr, uint32_t aValue) { out32(aAddr, aValue); }
void OAL_Write64(uintptr_t aAddr, uint64_t aValue)
{
	uint32_t *lpVal32 = (uint32_t *)(uintptr_t)&aValue;
	uintptr_t lAddr   = (uintptr_t)aAddr;

	out32(lAddr, *lpVal32);
	out32(lAddr + sizeof(*lpVal32), *(lpVal32 + 1));
}
