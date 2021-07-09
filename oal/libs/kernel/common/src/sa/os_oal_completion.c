/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_completion.h>
#include <arch_oal_timespec.h>
#include <arch_oal_waitqueue.h>

int32_t OAL_InitCompletion(OAL_Completion_t *apComp)
{
	int32_t lRet = 0;

	if (apComp != NULL) {
		apComp->mDone = 0U;
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_WaitForCompletion(OAL_Completion_t *apComp)
{
	int32_t lRet = 0;

	if (apComp != NULL) {
		while (apComp->mDone == 0U) {
			OAL_SA_ARCH_WAIT_EVENT();
		}
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

uint64_t OAL_WaitForCompletionTimeout(OAL_Completion_t *apComp,
                                      uint64_t aTimeout)
{
	uint64_t lRet = 0;

	if (apComp != NULL) {
		uint64_t lEndTstamp = OAL_GetTicks() + aTimeout;
		while ((OAL_GetTicks() < lEndTstamp)) {
			if (apComp->mDone == 1) {
				lRet = 1;
				break;
			}
		}
	}
	
	return lRet;
}

int32_t OAL_Complete(OAL_Completion_t *apComp)
{
	int32_t lRet = 0;

	if (apComp != NULL) {
		apComp->mDone = 1U;
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_CompleteAll(OAL_Completion_t *apComp)
{
	return OAL_Complete(apComp);
}

int32_t OAL_DestroyCompletion(OAL_Completion_t *apComp)
{
	int32_t lRet = 0;

	if (apComp == NULL) {
		lRet = -EINVAL;
	}

	return lRet;
}
