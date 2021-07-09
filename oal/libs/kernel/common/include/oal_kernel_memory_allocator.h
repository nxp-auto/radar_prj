/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_KERNEL_MEMORY_ALLOCATOR_H
#define OAL_KERNEL_MEMORY_ALLOCATOR_H

#include <oal_mem_constants.h>
#include <oal_utils.h>

#define OAL_ALLOCATOR_GRANULARITY OAL_PAGE_SIZE
#define OAL_MAX_NAME_LENGTH 50

struct OAL_MemoryAllocatorRegion {
	char8_t mName[OAL_MAX_NAME_LENGTH];
	uintptr_t mPhysAddr;
	uintptr_t mSize;
	uint32_t mStartID;
	uint32_t mAlign;
	uint8_t mInit;
	uint8_t mAutobalance;
};

#endif /* OAL_MEMORY_ALLOCATOR_H */
