/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "oal_timespec.h"
#include <time.h>

int32_t OAL_Sleep(const OAL_Timespec_t *acpTm)
{
	int32_t lRet = 1;
	struct timespec lTm;

	if (acpTm == NULL) {
		lRet = -EINVAL;
	} else {
		lTm.tv_sec  = acpTm->mSec;
		lTm.tv_nsec = acpTm->mNsec;

		while (((lTm.tv_sec > 0) || (lTm.tv_nsec > 0)) && (lRet != 0)) {
			lRet = nanosleep(&lTm, &lTm);
		}
	}
	return lRet;
}
