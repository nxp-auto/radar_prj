/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OS_OAL_WAITQUEUE_H
#define OS_OAL_WAITQUEUE_H

#include <linux/wait.h>
#include <linux/sched.h>
#include "oal_utils.h"

typedef wait_queue_head_t OAL_waitqueue_t;

static inline void OAL_init_waitqueue(OAL_waitqueue_t *q)
{
	init_waitqueue_head(q);
}

#define os_oal_wait_event(wq, condition)    \
	wait_event(wq, condition)


#define os_oal_wait_event_interruptible(wq, condition)    \
	 wait_event_interruptible(wq, condition)

#define os_oal_wait_event_timeout(wq, condition, timeout) \
	wait_event_timeout(wq, condition, timeout)

#define os_oal_wait_event_interruptible_timeout(wq, condition, timeout) \
	wait_event_interruptible_timeout(wq, condition, timeout)

static inline void OAL_wake_up(OAL_waitqueue_t *wq)
{
	wake_up(wq);
}

static inline void OAL_wake_up_interruptible(OAL_waitqueue_t *wq)
{
	wake_up_interruptible(wq);
}

#endif /* OS_OAL_WAITQUEUE_H */
