/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "oal_timespec.h"
#include <errno.h>
#include <time.h>

int32_t OAL_GetTime(OAL_Timespec_t *apTm)
{
	struct timespec lTm;
	int32_t lRet = 0;

	if (apTm == NULL) {
		lRet = -EINVAL;
	} else {
		lRet = clock_gettime(CLOCK_REALTIME, &lTm);
		if (lRet == 0) {
			apTm->mSec  = lTm.tv_sec;
			apTm->mNsec = lTm.tv_nsec;
		}
	}

	return lRet;
}
