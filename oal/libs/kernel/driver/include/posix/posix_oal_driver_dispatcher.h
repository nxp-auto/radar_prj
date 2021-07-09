/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_POSIX_DRIVER_DISPATCH_H
#define OAL_POSIX_DRIVER_DISPATCH_H

#include <oal_comm_kernel.h>

__BEGIN_DECLS

uint32_t OAL_PosixOalDriverDispatcher(oal_dispatcher_t *apDisp, uint32_t aFuncId,
                                  uintptr_t aInData, int32_t aDataLen);

__END_DECLS

#endif /* OAL_POSIX_DRIVER_DISPATCH_H */
