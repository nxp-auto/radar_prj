/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_timer.h>
#include <arch_oal_timer.h>
#include <arch_oal_timespec.h>

int32_t OAL_SetupTimer(OAL_Timer_t *apTimer, OAL_Timer_callback_t aCallback,
                       uintptr_t aData, uint32_t aFlags)
{
	int32_t lRet = 0;

	OAL_UNUSED_ARG(aFlags);
	if (apTimer != NULL) {
		apTimer->mExpires  = 0;
		apTimer->mCallback = aCallback;
		apTimer->mData     = aData;
		apTimer->mActive   = 0;
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_AddTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;

	if (apTimer != NULL) {
		lRet = OAL_SA_ARCH_AddTimer(apTimer);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_DelTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;
	if (apTimer != NULL) {
		lRet = OAL_SA_ARCH_DelTimer(apTimer);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_SetTimerTimeout(OAL_Timer_t *apTimer, uint64_t aNSec)
{
	int32_t lRet = 0;

	if (apTimer != NULL) {
		OAL_ARCH_SetTimerTimeout(apTimer, aNSec);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_DestroyTimer(OAL_Timer_t *apTimer)
{
	OAL_UNUSED_ARG(apTimer);
	return 0;
}
