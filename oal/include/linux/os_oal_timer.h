/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_OS_TIMER_H
#define OAL_OS_TIMER_H

#include <oal_utils.h>

#ifdef __KERNEL__
#include <linux/version.h>

/* Linux kernel timer */
struct OAL_Timer {
	struct timer_list mTimerList;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
	uintptr_t mData;
	OAL_Timer_callback_t mCallback;
#endif
};
#endif

#endif /* OAL_OS_TIMER_H */
