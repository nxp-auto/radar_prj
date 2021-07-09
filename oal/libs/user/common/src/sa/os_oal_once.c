/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_once.h>

int32_t OAL_ExecuteOnce(struct OAL_OnceControl *apOnce,
                        OAL_OnceFunction_t aFunc)
{
	int32_t lRet = 0;

	if ((apOnce == NULL) || (aFunc == NULL)) {
		lRet = EINVAL;
	} else {
		if (apOnce->mInitialized == 0U) {
			apOnce->mInitialized = 1U;
			aFunc();
		}
	}

	return lRet;
}
