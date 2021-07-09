/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_waitqueue.h>
#include <oal_log.h>

int32_t OAL_InitWaitQueue(OAL_waitqueue_t *apWq)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apWq, init_waitqueue_exit);

	*apWq = 0;
init_waitqueue_exit:
	return lRet;
}

int32_t OAL_DestroyWaitQueue(OAL_waitqueue_t *apWq)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apWq, destroy_wq_exit);

	*apWq = 0;

destroy_wq_exit:
	return lRet;
}

int32_t OAL_WakeUp(OAL_waitqueue_t *apWq)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apWq, wake_exit);

	/* Assumption: Single process, single core application */
	OAL_SA_ARCH_GENERATE_EVENT();

wake_exit:
	return lRet;
}

int32_t OAL_WakeUpInterruptible(OAL_waitqueue_t *apWq)
{
	return OAL_WakeUp(apWq);
}
