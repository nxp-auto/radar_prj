/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_DRIVER_DISPATCH_H
#define OAL_OS_DRIVER_DISPATCH_H

#include "oal_allocator.h"
#include "oal_comm_kernel.h"

uint32_t OAL_OS_DriverDispatcher(oal_dispatcher_t *apDisp, uint32_t aFuncId,
                               uintptr_t aInData, int32_t aDataLen);
uint32_t getNChunks(void);
OAL_ReservedRegion_t **getDriverData(void);

#endif /* OAL_OS_DRIVER_DISPATCH_H */
