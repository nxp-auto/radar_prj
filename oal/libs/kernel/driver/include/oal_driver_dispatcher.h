/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_DRIVER_DISPATCH_H
#define OAL_DRIVER_DISPATCH_H

#include "oal_comm_kernel.h"

uint32_t OAL_DriverDispatcher(oal_dispatcher_t *apDisp, uint32_t aFuncId,
                             uintptr_t aInData, int32_t aDataLen);
uint32_t OAL_OS_DriverDispatcher(oal_dispatcher_t *apDisp, uint32_t aFuncId,
                               uintptr_t aInData, int32_t aDataLen);

#endif /* OAL_DRIVER_DISPATCH_H */
