/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_bottom_half.h>
#include <oal_log.h>

int32_t OAL_InitializeBottomHalf(struct OAL_BottomHalf *apBtHalf,
                                 OAL_BottomHalfFunction_t aCallback,
                                 uintptr_t aCallbackArg)
{
	int32_t lRet = 0;
	if (apBtHalf == NULL) {
		lRet = -1;
	} else {
		apBtHalf->mFunc = aCallback;
		apBtHalf->mData = aCallbackArg;
	}

	return lRet;
}

void OAL_ScheduleBottomHalf(struct OAL_BottomHalf *apBtHalf)
{
	if (apBtHalf == NULL) {
		OAL_LOG_ERROR("Invalid argument\n");
	} else {
		apBtHalf->mFunc(apBtHalf->mData);
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
