/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_atomic.h>
#include <oal_log.h>

int32_t OAL_SetAtomic(struct OAL_Atomic *apAtomic, uint32_t aNewValue,
                      uint32_t *apOldValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, set_atomic_exit);

	if (apOldValue != NULL) {
		*apOldValue = atomic_read(&apAtomic->mOSAtomic);
	}

	atomic_set(&apAtomic->mOSAtomic, aNewValue);
set_atomic_exit:
	return lRet;
}

int32_t OAL_GetAtomic(struct OAL_Atomic *apAtomic, uint32_t *apValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, get_atomic_exit);
	OAL_CHECK_NULL_PARAM(apValue, get_atomic_exit);

	*apValue = atomic_read(&apAtomic->mOSAtomic);
get_atomic_exit:
	return lRet;
}

int32_t OAL_IncrementAtomic(struct OAL_Atomic *apAtomic, uint32_t *apOldValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, inc_atomic_exit);

	if (apOldValue != NULL) {
		*apOldValue = atomic_read(&apAtomic->mOSAtomic);
	}

	atomic_inc(&apAtomic->mOSAtomic);
inc_atomic_exit:
	return lRet;
}

int32_t OAL_DecrementAtomic(struct OAL_Atomic *apAtomic, uint32_t *apOldValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, dec_atomic_exit);

	if (apOldValue != NULL) {
		*apOldValue = atomic_read(&apAtomic->mOSAtomic);
	}

	atomic_dec(&apAtomic->mOSAtomic);

dec_atomic_exit:
	return lRet;
}

int32_t OAL_AddAtomic(struct OAL_Atomic *apAtomic, uint32_t aAddedValue,
                      uint32_t *apOldValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, add_atomic_exit);

	if (apOldValue != NULL) {
		*apOldValue = atomic_read(&apAtomic->mOSAtomic);
	}

	atomic_add((int32_t)aAddedValue, &apAtomic->mOSAtomic);

add_atomic_exit:
	return lRet;
}

int32_t OAL_SubtractAtomic(struct OAL_Atomic *apAtomic, uint32_t aSubValue,
                           uint32_t *apOldValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, sub_atomic_exit);

	if (apOldValue != NULL) {
		*apOldValue = atomic_read(&apAtomic->mOSAtomic);
	}

	atomic_sub((int32_t)aSubValue, &apAtomic->mOSAtomic);

sub_atomic_exit:
	return lRet;
}
