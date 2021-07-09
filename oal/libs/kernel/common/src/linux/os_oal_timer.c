/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_timer.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/timer.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
static void timerCallback(struct timer_list *apTimer)
{
	struct OAL_Timer *lpOalTimer =
	    from_timer(lpOalTimer, apTimer, mTimerList);
	lpOalTimer->mCallback(lpOalTimer->mData);
}
#endif

int32_t OAL_SetupTimer(OAL_Timer_t *apTimer, OAL_Timer_callback_t aCallback,
                       uintptr_t aData, uint32_t aFlags)
{
	int32_t lRet = 0;

	OAL_UNUSED_ARG(aFlags);
	if (apTimer != NULL) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
		timer_setup(&apTimer->mTimerList, timerCallback, 0);
		apTimer->mData     = aData;
		apTimer->mCallback = aCallback;
#else
		setup_timer(&apTimer->mTimerList, aCallback, aData);
#endif
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_SetTimerTimeout(OAL_Timer_t *apTimer, uint64_t aNSec)
{
	int32_t lRet = 0;

	if (apTimer != NULL) {
		(void)del_timer(&apTimer->mTimerList);
		apTimer->mTimerList.expires = jiffies + nsecs_to_jiffies(aNSec);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_AddTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;

	if (apTimer != NULL) {
		add_timer(&apTimer->mTimerList);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_DelTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;
	if (apTimer != NULL) {
		lRet = del_timer_sync(&apTimer->mTimerList);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_DestroyTimer(OAL_Timer_t *apTimer)
{
	int32_t lRet = 0;
	if (apTimer == NULL){
		lRet = -EINVAL;
	}

	return lRet;
}
