/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_memmap.h>
#include <linux/io.h>

uintptr_t OAL_MapSystemMemory(uint64_t aOffset, size_t aSize,
                              enum OALMapSource aSrc)
{
	OAL_UNUSED_ARG(aSrc);
	return (uintptr_t)ioremap((phys_addr_t)aOffset, aSize);
}

int32_t OAL_UnmapSystemMemory(uintptr_t aAddr, size_t aSize,
                              enum OALMapSource aSrc)
{
	int32_t lRet = 0;
	OAL_UNUSED_ARG(aSrc);
	OAL_UNUSED_ARG(aSize);
	iounmap((void __iomem *)aAddr);
	return lRet;
}

uint8_t OAL_Read8(uintptr_t aAddr) { return readb((void __iomem *)aAddr); }
uint16_t OAL_Read16(uintptr_t aAddr) { return readw((void __iomem *)aAddr); }
uint32_t OAL_Read32(uintptr_t aAddr) { return readl((void __iomem *)aAddr); }
uint64_t OAL_Read64(uintptr_t aAddr) { return readq((void __iomem *)aAddr); }
void OAL_Write8(uintptr_t aAddr, uint8_t aValue)
{
	writeb(aValue, (void __iomem *)aAddr);
}

void OAL_Write16(uintptr_t aAddr, uint16_t aValue)
{
	writew(aValue, (void __iomem *)aAddr);
}

void OAL_Write32(uintptr_t aAddr, uint32_t aValue)
{
	writel(aValue, (void __iomem *)aAddr);
}

void OAL_Write64(uintptr_t aAddr, uint64_t aValue)
{
	writeq(aValue, (void __iomem *)aAddr);
}
