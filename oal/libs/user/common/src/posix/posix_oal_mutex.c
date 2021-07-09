/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_mutex.h>

int32_t OAL_InitializeMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;
	int32_t lError;

	if (apLock != NULL) {
		lError = pthread_mutex_init(&apLock->mOSMutex, NULL);
		if (lError != 0) {
			OAL_LOG_ERROR("Failed to create the mutex: %s\n",
					strerror(lError));
		} else {
			lRet = 0;
		}
	}

	return lRet;
}

int32_t OAL_LockMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;
	int32_t lError;

	if (apLock != NULL) {
		lError = pthread_mutex_lock(&apLock->mOSMutex);
		if (lError != 0) {
			OAL_LOG_ERROR("Failed to lock the mutex: %s\n",
			              strerror(lError));
			lRet = lError;
		} else {
			lRet = 0;
		}
	}

	return lRet;
}

int32_t OAL_UnlockMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;
	int32_t lError;

	if (apLock != NULL) {
		lError = pthread_mutex_unlock(&apLock->mOSMutex);
		if (lError != 0) {
			OAL_LOG_ERROR("Failed to unlock the mutex: %s\n",
			              strerror(errno));
		} else {
			lRet = 0;
		}
	}

	return lRet;
}

int32_t OAL_DestroyMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;
	int32_t lError;

	if (apLock != NULL) {
		lError = pthread_mutex_destroy(&apLock->mOSMutex);
		if (lError != 0) {
			OAL_LOG_ERROR("Failed to destroy the mutex: %s\n",
			              strerror(errno));
		} else {
			lRet = 0;
		}
	}

	return lRet;
}
