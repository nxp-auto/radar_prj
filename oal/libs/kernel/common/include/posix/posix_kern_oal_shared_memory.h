/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_POSIX_SHARED_MEMORY_H
#define OAL_POSIX_SHARED_MEMORY_H
#include <oal_utils.h>

__BEGIN_DECLS

int32_t OAL_DRV_CreateToken(uint64_t aPhysAddr, uint64_t aSize,
                            uintptr_t *apToken);

int32_t OAL_DRV_GetToken(uintptr_t aToken, uint64_t *apPhysAddr,
                         uint64_t *apSize);

int32_t OAL_DRV_ReleaseToken(uintptr_t aToken);

__END_DECLS

#endif /* OAL_POSIX_SHARED_MEMORY_H */
