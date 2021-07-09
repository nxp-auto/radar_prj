/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_mutex.h>

int32_t OAL_InitializeMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;

	if (apLock != NULL)
		lRet = OAL_spin_lock_init(&apLock->mOSLock);

	return lRet;
}

int32_t OAL_LockMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;

	if (apLock != NULL)
		lRet = OAL_spin_lock(&apLock->mOSLock);

	return lRet;
}

int32_t OAL_UnlockMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;

	if (apLock != NULL)
		lRet = OAL_spin_unlock(&apLock->mOSLock);

	return lRet;
}

int32_t OAL_DestroyMutex(struct OAL_Mutex *apLock)
{
	OAL_UNUSED_ARG(apLock);
	return 0;
}
