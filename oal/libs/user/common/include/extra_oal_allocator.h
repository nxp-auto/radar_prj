/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_EXTRA_ALLOCATOR_H
#define OAL_EXTRA_ALLOCATOR_H

#include <oal_utils.h>

__BEGIN_DECLS

uint32_t OAL_GetNumberOfReserverdChunks(void);
uint64_t OAL_GetResMemBase(uint32_t aMemId);
uint64_t OAL_GetResMemSize(uint32_t aMemId);
uint32_t OAL_IsResMemAutobalanced(uint32_t aMemId);
uintptr_t OAL_GetSizeOfAllocatedMemory(void);

__END_DECLS

#endif /* OAL_EXTRA_ALLOCATOR_H */
