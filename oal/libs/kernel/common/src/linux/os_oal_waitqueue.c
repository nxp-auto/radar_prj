/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <oal_waitqueue.h>
#include <linux/sched.h>
#include <linux/wait.h>

int32_t OAL_InitWaitQueue(OAL_waitqueue_t *apWq)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apWq, init_waitqueue_exit);

	init_waitqueue_head(apWq);
init_waitqueue_exit:
	return lRet;
}

int32_t OAL_WakeUp(OAL_waitqueue_t *apWq)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apWq, wake_exit);

	wake_up(apWq);
wake_exit:
	return lRet;
}

int32_t OAL_WakeUpInterruptible(OAL_waitqueue_t *apWq)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apWq, wake_irq_exit);

	wake_up_interruptible(apWq);
wake_irq_exit:
	return lRet;
}

int32_t OAL_DestroyWaitQueue(OAL_waitqueue_t *apWq)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apWq, destroy_wq_exit);

destroy_wq_exit:
	return lRet;
}
