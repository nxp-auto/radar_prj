/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_WAIT_QUEUE_H
#define OAL_WAIT_QUEUE_H

/* Each platform must have its own implementation of the OAL_waitqueue_t.
 * OAL_waitqueue_t describes a wait queue and  must be defined in
 * os_oal_wait_queue.h for that specific OS.
 */

#include "common_stringify_macros.h"
#include XSTR(OS/os_oal_waitqueue.h)

__BEGIN_DECLS

/**
 * @brief init_waitqueue Wait queue initialization function
 *
 * @param q[in]: The queue to be initialized
 */
void OAL_init_waitqueue(OAL_waitqueue_t *q);

/**
 * Sleeps until the condition becomes true
 *
 * @param OAL_waitqueue_t wq[in]: The wait queue.
 * @param (expression) condition[in]: The expression to be evaluated
 *
 */
#define OAL_wait_event(wq, condition) \
	os_oal_wait_event(wq, condition)

/**
 * Sleeps until the condition becomes true or the timeout elapses.
 *
 * @param OAL_waitqueue_t wq[in]: The wait queue
 * @param (expression) condition[in]: The expression to be evaluated
 * @param uint32_t timeout[in]: The timeout in jiffies
 *
 * @return 0, if the condition evaluated to false after the timeout elapsed,
 *         1, if the condition evaluated to true after the timeout elapsed,
 *         the remaining jiffies,  (at least 1) if the condition evaluated
 *          to true before the timeout elapsed.
 */
#define OAL_wait_event_timeout(wq, condition, timeout) \
	os_oal_wait_event_timeout(wq, condition, timeout)


#define OAL_wait_event_interruptible(wq, condition) \
	os_oal_wait_event_interruptible(wq, condition)

/**
 * Wake up a sleeping process.
 *
 * @param wq[in]: The wait queue
 */
void OAL_wake_up(OAL_waitqueue_t *wq);

/**
 * Wake up a process performing an interruptible sleep
 *
 * @param wq[in]: The wait queue
 */
void OAL_wake_up_interruptible(OAL_waitqueue_t *wq);

__END_DECLS

#endif
