/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_MODULE_PROBE_H
#define OAL_OS_MODULE_PROBE_H

#include <oal_utils.h>
#include <oal_devtree_utils.h>
#include <oal_kernel_memory_allocator.h>
#include <qnx_posix_typed_mem_utils.h>

__BEGIN_DECLS

int32_t OAL_OS_ProbeDriver(const struct fdt_node *acpNode,
                           const void **acpData);

__END_DECLS

#endif
