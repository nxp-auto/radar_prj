/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "oal_timespec.h"
#include <time.h>

#define MS 1000000

int32_t OAL_Sleep(const OAL_Timespec_t *acpTm)
{
	struct timespec lLtm;
	int32_t lRet = -EINVAL;

	if (acpTm != NULL) {
		lLtm.tv_sec  = acpTm->mSec;
		lLtm.tv_nsec = acpTm->mNsec;

		if (lLtm.tv_sec == 0) {
			lRet = nanospin(&lLtm);
		}

		if ((lRet == E2BIG) || (lLtm.tv_sec > 0)) {
			lRet = nanosleep(&lLtm, &lLtm);
		}

		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to sleep (%s)\n",
			              strerror(errno));
		}
	}

	return lRet;
}
