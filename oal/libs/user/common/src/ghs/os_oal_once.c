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
		goto execute_once_exit;
	}

	/* Execute function if not initialized */
	if (TestAndSet(&apOnce->mState, OAL_NOT_INITIALIZED_STATE,
	               OAL_INITIALIZING_STATE) == Success) {
		aFunc();
		/* Change the state to OAL_INITIALIZED */
		(void)TestAndSet(&apOnce->mState, OAL_INITIALIZING_STATE,
		                 OAL_INITIALIZED_STATE);
	}

	/* Wait the initialization */
	while (TestAndSet(&apOnce->mState, OAL_INITIALIZED_STATE,
	                  OAL_INITIALIZED_STATE) == Failure) {
	};

execute_once_exit:
	return lRet;
}
