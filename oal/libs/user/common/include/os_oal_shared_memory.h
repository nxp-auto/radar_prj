/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_SHARED_MEMORY_H
#define OAL_OS_SHARED_MEMORY_H

#include <oal_utils.h>
#include <sys/mman.h>

__BEGIN_DECLS

int32_t OAL_MapPhysMem(uint64_t aPhysAddr, uint64_t aSize, uint8_t **apVirtAddr,
                       enum OAL_MemoryMapping aVirtType);

static inline int32_t OAL_UnmapVirtAddress(uint8_t *apAddr, uint64_t aSize)
{
	return munmap(apAddr, aSize);
}

__END_DECLS

#endif /* OAL_OS_SHARED_MEMORY_H */
