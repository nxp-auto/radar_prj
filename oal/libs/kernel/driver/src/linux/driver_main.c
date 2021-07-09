/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_utils.h>
#include <driver_init.h>
#include <linux/module.h>

static int32_t init_oal_driver(void) { return OAL_InitializeDriver(); }

static void release_oal_driver(void) { OAL_ReleaseDriver(); }

module_init(init_oal_driver);
module_exit(release_oal_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(
    "OAL kernel module for contiguous "
    "memory allocations and its management.");
MODULE_ALIAS("OAL");
