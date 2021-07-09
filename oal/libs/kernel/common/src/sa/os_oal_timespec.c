/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "oal_timespec.h"
#include "oal_uptime.h"
#include "arch_oal_timespec.h"

int32_t OAL_Sleep(const OAL_Timespec_t *acpTm)
{
	int32_t lRet = 0;
	uint64_t lBegin, lEnd;
	lBegin = OAL_GetTicks();

	if (acpTm == NULL) {
		lRet = -EINVAL;
	} else {
		lEnd = (uint64_t)acpTm->mSec;
		lEnd *= (uint64_t)OAL_NSEC_IN_SEC;
		lEnd += (uint64_t)acpTm->mNsec;
		(void)OAL_SA_nsleep(lBegin, lEnd);
	}

	return lRet;
}

int32_t OAL_GetTime(OAL_Timespec_t *apTm)
{
	int32_t lRet = 0;
	uint64_t lTime;

	if (apTm == NULL) {
		lRet = -EINVAL;
	} else {
		lTime       = OAL_GettimeUsec();
		apTm->mSec  = ((int64_t)lTime) / OAL_USEC_IN_SEC;
		apTm->mNsec = (((int64_t)lTime) % OAL_USEC_IN_SEC) * (OAL_NSEC_IN_USEC);
	}

	return lRet;
}
