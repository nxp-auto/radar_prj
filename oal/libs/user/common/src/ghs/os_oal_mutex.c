/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_mutex.h>

int32_t OAL_InitializeMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;
	Error lError;

	if (apLock != NULL) {
		lError = CreateLocalMutex(&apLock->mOSMutex);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to create the mutex: %s\n",
			              ErrorString(lError));
		} else {
			lRet = 0;
		}
	}

	return lRet;
}

int32_t OAL_LockMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;
	Error lError;

	if (apLock != NULL) {
		lError = WaitForLocalMutex(apLock->mOSMutex);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to lock the mutex: %s\n",
			              ErrorString(lError));
		} else {
			lRet = 0;
		}
	}

	return lRet;
}

int32_t OAL_UnlockMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;
	Error lError;

	if (apLock != NULL) {
		lError = ReleaseLocalMutex(apLock->mOSMutex);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to unlock the mutex: %s\n",
			              ErrorString(lError));
		} else {
			lRet = 0;
		}
	}

	return lRet;
}

int32_t OAL_DestroyMutex(struct OAL_Mutex *apLock)
{
	int32_t lRet = -1;
	Error lError;

	if (apLock != NULL) {
		lError = CloseLocalMutex(apLock->mOSMutex);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to destroy the mutex: %s\n",
			              ErrorString(lError));
		} else {
			lRet = 0;
		}
	}

	return lRet;
}
