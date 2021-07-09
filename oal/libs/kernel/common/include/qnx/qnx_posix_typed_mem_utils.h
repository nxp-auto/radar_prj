/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_QNX_POSIX_TYPED_MEM_UTILS_H
#define OAL_QNX_POSIX_TYPED_MEM_UTILS_H

#include <oal_utils.h>
#include <oal_kernel_memory_allocator.h>
#include <fcntl.h>
#include <sys/asinfo.h>
#include <sys/stat.h>

#define OAL_MAX_POSIX_TYPED_MEM_LEN (400U)

__BEGIN_DECLS

int32_t OAL_OS_ResolveMemRegName(struct OAL_MemoryAllocatorRegion *apMemReg);

int32_t OAL_QNX_GetPathFromAsInfo(struct asinfo_entry const *acpAs,
                                  char8_t *apPath, size_t aPathLen);

__END_DECLS

#endif
