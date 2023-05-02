/*
 * Copyright 2018, 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <oal_bottom_half.h>
#include <linux/version.h>

int32_t OAL_InitializeBottomHalf(struct OAL_BottomHalf *apBtHalf,
                                 OAL_BottomHalfFunction_t aCallback,
                                 uintptr_t aCallbackArg)
{
	int32_t lRet = 0;
	if (apBtHalf == NULL) {
		lRet = -1;
	} else {
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 9, 0)
		DECLARE_TASKLET(lTasklet, aCallback, aCallbackArg);

		apBtHalf->mTasklet = lTasklet;
#else
		tasklet_init(&apBtHalf->mTasklet, aCallback, aCallbackArg);
#endif

	}

	return lRet;
}

void OAL_ScheduleBottomHalf(struct OAL_BottomHalf *apBtHalf)
{
	if (apBtHalf == NULL) {
		OAL_LOG_ERROR("Invalid argument\n");
	} else {
		tasklet_schedule(&apBtHalf->mTasklet);
	}

	return;
}

int32_t OAL_DestroyBottomHalf(struct OAL_BottomHalf *apBtHalf)
{
	int32_t lRet = 0;

	if (apBtHalf == NULL) {
		lRet = -1;
	}

	/* Nothing to do */
	return lRet;
}
