/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#include "oal_timespec.h"

int OAL_Sleep(const OAL_Timespec_t *tm)
{
	int ret = 0;
	int64_t usec;

	if (tm == NULL) {
		ret = -EINVAL;
	} else {
		if (tm->sec != 0) {
			msleep(tm->sec * MSEC_PER_SEC);
		}

		usec = tm->nsec / NSEC_IN_USEC;
		if (tm->nsec <= MAX_UDELAY_MS * NSEC_IN_MSEC) {
			udelay(usec);
		} else {
			usleep_range(usec, usec);
		}
	}
	return ret;
}

int OAL_GetTime(OAL_Timespec_t *tm)
{
	int ret = 0;
	struct timespec ts;

	if (tm == NULL) {
		ret = -EINVAL;
	} else {
		getnstimeofday(&ts);
		tm->sec = ts.tv_sec;
		tm->nsec = ts.tv_nsec;
	}

	return ret;
}
