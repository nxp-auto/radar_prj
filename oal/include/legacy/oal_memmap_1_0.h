/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_MEMMAP_1_0_H
#define OAL_MEMMAP_1_0_H

#include <oal_utils.h>
#include <os_oal_memmap.h>

__BEGIN_DECLS

static inline uintptr_t OAL_memmap(uint64_t aOffset, size_t aSize,
                                   enum OALMapSource aSrc)
{
	return OAL_MapSystemMemory(aOffset, aSize, aSrc);
}

static inline int32_t OAL_memunmap(uintptr_t aAddr, size_t aSize,
                                   enum OALMapSource aSrc)
{
	return OAL_UnmapSystemMemory(aAddr, aSize, aSrc);
}

static inline uintptr_t OAL_kmemmap(uint64_t aOffset, size_t aSize)
{
	return OAL_MapKernelSpace(aOffset, aSize);
}

static inline int32_t OAL_kmemunmap(uintptr_t aAddr, size_t aSize)
{
	return OAL_UnmapKernelSpace(aAddr, aSize);
}

static inline uintptr_t OAL_umemmap(uint64_t aOffset, size_t aSize)
{
	return OAL_MapUserSpace(aOffset, aSize);
}

static inline int32_t OAL_umemunmap(uintptr_t aAddr, size_t aSize)
{
	return OAL_UnmapUserSpace(aAddr, aSize);
}

static inline uint8_t OAL_read8(uintptr_t aAddr) { return OAL_Read8(aAddr); }
static inline uint16_t OAL_read16(uintptr_t aAddr) { return OAL_Read16(aAddr); }
static inline uint32_t OAL_read32(uintptr_t aAddr) { return OAL_Read32(aAddr); }
static inline uint64_t OAL_read64(uintptr_t aAddr) { return OAL_Read64(aAddr); }
static inline void OAL_write8(uintptr_t aAddr, uint8_t aValue)
{
	OAL_Write8(aAddr, aValue);
}

static inline void OAL_write16(uintptr_t aAddr, uint16_t aValue)
{
	OAL_Write16(aAddr, aValue);
}

static inline void OAL_write32(uintptr_t aAddr, uint32_t aValue)
{
	OAL_Write32(aAddr, aValue);
}

static inline void OAL_write64(uintptr_t aAddr, uint64_t aValue)
{
	OAL_Write64(aAddr, aValue);
}

__END_DECLS

/* @} */

#endif /* OAL_MEMMAP_1_0_H */
