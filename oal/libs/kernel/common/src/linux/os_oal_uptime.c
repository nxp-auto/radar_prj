/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_uptime.h>
#include <linux/module.h>

uint64_t OAL_GetUptime(void) { return jiffies; }
