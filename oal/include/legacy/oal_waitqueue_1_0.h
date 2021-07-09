/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_WAIT_QUEUE_1_0_H
#define OAL_WAIT_QUEUE_1_0_H

#include <oal_utils.h>
#include <os_oal_waitqueue.h>

__BEGIN_DECLS

/* Deprecated API*/

static inline void OAL_init_waitqueue(OAL_waitqueue_t *apWq)
{
	(void)OAL_InitWaitQueue(apWq);
}

#define OAL_wait_event(apWq, aCondition) OAL_WaitEvent(apWq, aCondition)

#define OAL_wait_event_timeout(apWq, aCondition, aTimeout)                     \
	OAL_WaitEventTimeout(apWq, aCondition, aTimeout)

#define OAL_wait_event_interruptible(apWq, aCondition)                         \
	OAL_WaitEventInterruptible(apWq, aCondition)

#define OAL_wait_event_interruptible_timeout(apWq, aCondition, aTimeout)       \
	OAL_WaitEventInterruptibleTimeout(apWq, aCondition, aTimeout)

static inline void OAL_wake_up(OAL_waitqueue_t *apWq)
{
	(void)OAL_WakeUp(apWq);
}

static inline void OAL_wake_up_interruptible(OAL_waitqueue_t *apWq)
{
	(void)OAL_WakeUpInterruptible(apWq);
}

__END_DECLS

#endif
