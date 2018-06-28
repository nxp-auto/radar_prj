/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OS_OAL_TIMER_H
#define OS_OAL_TIMER_H

#include "priv_oal_timer.h"
#include "oal_utils.h"

/* Linux Kernel functionality */
#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/slab.h>

/* Linux kernel timer */
struct OAL_Timer {
	struct timer_list lt;
};

static inline int OAL_setup_timer(OAL_Timer_t *t,
				   OAL_Timer_callback_t callback,
				   unsigned long data,
				   uint32_t flags)
{
	int ret = 0;

	UNUSED_ARG(flags);
	if (t != NULL) {
		setup_timer(&t->lt, callback, data);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

static inline int OAL_set_timer_timeout(OAL_Timer_t *t, unsigned long nsec)
{
	int ret = 0;

	if (t != NULL) {
		del_timer(&t->lt);
		t->lt.expires = jiffies + nsecs_to_jiffies(nsec);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

static inline int OAL_add_timer(OAL_Timer_t *t)
{
	int ret = 0;

	if (t != NULL) {
		add_timer(&t->lt);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

static inline int OAL_del_timer(OAL_Timer_t *t)
{
	int ret = 0;
	if (t != NULL) {
		ret = del_timer_sync(&t->lt);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

static inline void OAL_destroy_timer(OAL_Timer_t *t)
{
	UNUSED_ARG(t);
}

#endif /* __KERNEL__ */
#endif /* OS_OAL_TIMER_H */
