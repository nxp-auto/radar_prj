/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_WAITQUEUE_H
#define OAL_OS_WAITQUEUE_H

#include <oal_utils.h>

__BEGIN_DECLS

#ifdef __KERNEL__
#define OAL_OS_WAIT_EVENT(wq, condition) wait_event(wq, condition)

#define OAL_OS_WAIT_EVENT_INTERRUPTIBLE(wq, condition)                         \
	wait_event_interruptible(wq, condition)

#define OAL_OS_WAIT_EVENT_TIMEOUT(wq, condition, timeout)                      \
	wait_event_timeout(wq, condition, timeout)

#define OAL_OS_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(wq, condition, timeout)        \
	wait_event_interruptible_timeout(wq, condition, timeout)

typedef wait_queue_head_t OAL_waitqueue_t;
#endif

__END_DECLS
#endif /* OAL_OS_WAITQUEUE_H */
