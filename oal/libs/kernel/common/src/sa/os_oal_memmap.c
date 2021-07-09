/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_memmap.h>
#include <stdint.h>

uint8_t OAL_Read8(uintptr_t aAddr) { return *((uint8_t*)aAddr); }
uint16_t OAL_Read16(uintptr_t aAddr) { return *((uint16_t*)aAddr); }
uint32_t OAL_Read32(uintptr_t aAddr) { return *((uint32_t*)aAddr); }
uint64_t OAL_Read64(uintptr_t aAddr) { return *((uint64_t*)aAddr); }
void OAL_Write8(uintptr_t aAddr, uint8_t aValue)
{
	*((uint8_t*)aAddr) = aValue;
}

void OAL_Write16(uintptr_t aAddr, uint16_t aValue)
{
	*((uint16_t*)aAddr) = aValue;
}

void OAL_Write32(uintptr_t aAddr, uint32_t aValue)
{
	*((uint32_t*)aAddr) = aValue;
}

void OAL_Write64(uintptr_t aAddr, uint64_t aValue)
{
	*((uint64_t*)aAddr) = aValue;
}

uintptr_t OAL_MapSystemMemory(uint64_t aOffset, size_t aSize,
                              enum OALMapSource aSrc)
{
	OAL_UNUSED_ARG(aOffset);
	OAL_UNUSED_ARG(aSrc);
	uint64_t lRet = 0;

	if (aSize == 0U) {
		OAL_LOG_ERROR("Invalid arguments\n");
	} else {
		lRet = aOffset;
	}
	return (uintptr_t)lRet;
}

int32_t OAL_UnmapSystemMemory(uintptr_t aAddr, size_t aSize,
                              enum OALMapSource aSrc)
{
	int32_t lRet = 0;
	OAL_UNUSED_ARG(aAddr);
	OAL_UNUSED_ARG(aSize);
	OAL_UNUSED_ARG(aSrc);
	return lRet;
}
