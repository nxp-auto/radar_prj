/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_POSIX_MEMORY_ALLOCATOR_H
#define OAL_POSIX_MEMORY_ALLOCATOR_H

#include <oal_comm.h>
#include <oal_utils.h>
#include <oal_virtmem_manager.h>

__BEGIN_DECLS

extern OAL_DriverHandle_t gOalH;

int32_t OAL_InitializeMemoryFd(void);
int32_t OAL_DeinitializeMemoryFd(void);

int32_t OAL_MapPhysArea(struct PhysicalPageMapping *apPhys,
                        uint8_t **apVirtAddr, enum OAL_MemoryMapping aVirtType);

int32_t OAL_UnmapMemArea(struct PhysicalPageMapping *apPhys,
                         uint8_t *apVirtAddr);

__END_DECLS

#endif
