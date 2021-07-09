/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_MODULE_PROBE_H
#define OAL_OS_MODULE_PROBE_H

#include <oal_devtree_utils.h>
#include <oal_kernel_memory_allocator.h>
#include <oal_utils.h>

__BEGIN_DECLS

int32_t OAL_OS_ProbeDriver(const struct fdt_node *acpNode, const void **acpData);
int32_t OAL_OS_InitKernelAllocator(void);

static inline int32_t OAL_OS_ResolveMemRegName(
    struct OAL_MemoryAllocatorRegion *apMemReg)
{
	OAL_UNUSED_ARG(apMemReg);
	return 0;
}

__END_DECLS

#endif
