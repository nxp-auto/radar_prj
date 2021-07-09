/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_SERVER_H
#define OAL_OS_SERVER_H

#include <oal_driver_functions.h>
#include <oal_utils.h>

Error OAL_GHS_AllocateMemory(CMD_ALLOC_TYPE *apAux, MemoryRegion *apMemRegion);
Error OAL_GHS_FreeMemory(MemoryRegion aMemRegion);

#endif
