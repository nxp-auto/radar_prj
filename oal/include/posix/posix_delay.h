/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_POSIX_DELAY_H
#define OAL_POSIX_DELAY_H

#include <unistd.h>

#define msleep(A) usleep(((useconds_t)A) * 1000U)

#endif /* POSIX_DELAY */
