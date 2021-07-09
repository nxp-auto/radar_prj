/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_PAGE_MANAGER_H
#define OAL_OS_PAGE_MANAGER_H
#include <oal_utils.h>
#include <asp_export.h>

#define OAL_PAGE_SIZE (((uint32_t)1U) << ((uint32_t)ASP_LOG2PAGESIZE))

__BEGIN_DECLS

struct OSOALMemoryChunk {
	MemoryRegion mPages[OAL_MAX_REGION_SIZE / ASP_PAGESIZE];
	Address mFirstKernelAddress;
};

__END_DECLS

#endif /* OAL_OS_PAGE_MANAGER_H */
