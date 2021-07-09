/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <linux/ktime.h>
#include <linux/timekeeping.h>

#include "oal_timespec.h"

int32_t OAL_Sleep(const OAL_Timespec_t *acpTm)
{
	int32_t lRet = 0;
	int64_t lUsec;

	if (acpTm == NULL) {
		lRet = -EINVAL;
		goto sleep_exit;
	}

	if (acpTm->mSec != 0) {
		msleep(acpTm->mSec * MSEC_PER_SEC);
	}

	lUsec = acpTm->mNsec / OAL_NSEC_IN_USEC;
	if (acpTm->mNsec <= MAX_UDELAY_MS * OAL_NSEC_IN_MSEC) {
		udelay(lUsec);
	} else {
		usleep_range(lUsec, lUsec);
	}

sleep_exit:
	return lRet;
}

int32_t OAL_GetTime(OAL_Timespec_t *apTm)
{
	int32_t lRet = 0;
	struct timespec lTs;

	if (apTm == NULL) {
		lRet = -EINVAL;
	} else {
		getnstimeofday(&lTs);
		apTm->mSec  = lTs.tv_sec;
		apTm->mNsec = lTs.tv_nsec;
	}

	return lRet;
}
