/*
 * Copyright 2018,2020 NXP
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
		lRet = pthread_once(&apOnce->mPosixOnce, aFunc);
#ifdef OAL_PROCESS_SAFE
		if (lRet == 0) {
			lRet = pthread_atfork(NULL, NULL, aFunc);
		}
#endif
	}

	return lRet;
}
