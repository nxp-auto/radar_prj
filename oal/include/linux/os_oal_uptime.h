/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OS_OAL_UPTIME_H
#define OS_OAL_UPTIME_H

#include "oal_utils.h"

#ifdef __KERNEL__
#include <linux/module.h>
static inline unsigned long OAL_Uptime(void)
{
	return jiffies;
}
#endif

#endif /* OS_OAL_UPTIME_H */
