/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_ONCE_H
#define OAL_OS_ONCE_H

#ifndef __KERNEL__
#include <posix/posix_oal_once.h>
#else
#error "OAL once isn't implemented for Kernel Modules, use module "
"initialization mechanism instead."
#endif

#endif
