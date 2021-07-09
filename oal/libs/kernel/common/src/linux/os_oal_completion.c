/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_completion.h>

int32_t OAL_InitCompletion(OAL_Completion_t *apComp)
{
	int32_t lRet = 0;

	if (apComp != NULL) {
		init_completion(&apComp->mComp);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_WaitForCompletion(OAL_Completion_t *apComp)
{
	int32_t lRet = 0;

	if (apComp != NULL) {
		wait_for_completion(&apComp->mComp);
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
		lRet = wait_for_completion_timeout(&apComp->mComp, aTimeout);
	} else {
		lRet = 0;
	}

	return lRet;
}

int32_t OAL_Complete(OAL_Completion_t *apComp)
{
	int32_t lRet = 0;

	if (apComp != NULL) {
		complete(&apComp->mComp);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_CompleteAll(OAL_Completion_t *apComp)
{
	int32_t lRet = 0;

	if (apComp != NULL) {
		complete_all(&apComp->mComp);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

int32_t OAL_DestroyCompletion(OAL_Completion_t *apComp)
{
	int32_t lRet = 0;

	if (apComp == NULL) {
		lRet = -EINVAL;
	}

	return lRet;
}
