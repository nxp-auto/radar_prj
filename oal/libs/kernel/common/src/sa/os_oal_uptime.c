/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_timespec.h>
#include <oal_uptime.h>
#include <arch_oal_timespec.h>

uint64_t OAL_GetUptime(void) { return OAL_SA_GetTime(); }
