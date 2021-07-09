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
		*apOldValue = (uint32_t)__atomic_exchange_n(
		    &apAtomic->mOSAtomic, aNewValue, __ATOMIC_SEQ_CST);
	} else {
		(void)__atomic_store_n(&apAtomic->mOSAtomic, aNewValue,
		                       __ATOMIC_SEQ_CST);
	}

set_atomic_exit:
	return lRet;
}

int32_t OAL_GetAtomic(struct OAL_Atomic *apAtomic, uint32_t *apValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, get_atomic_exit);
	OAL_CHECK_NULL_PARAM(apValue, get_atomic_exit);

	(void)__atomic_load(&apAtomic->mOSAtomic, apValue, __ATOMIC_SEQ_CST);

get_atomic_exit:
	return lRet;
}

int32_t OAL_AddAtomic(struct OAL_Atomic *apAtomic, uint32_t aAddedValue,
                      uint32_t *apOldValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, add_atomic_exit);

	if (apOldValue != NULL) {
		*apOldValue =
		    __sync_fetch_and_add(&apAtomic->mOSAtomic, aAddedValue);
	} else {
		(void)__sync_fetch_and_add(&apAtomic->mOSAtomic, aAddedValue);
	}

add_atomic_exit:
	return lRet;
}

int32_t OAL_SubtractAtomic(struct OAL_Atomic *apAtomic, uint32_t aSubValue,
                           uint32_t *apOldValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, sub_atomic_exit);

	if (apOldValue != NULL) {
		*apOldValue =
		    __sync_fetch_and_sub(&apAtomic->mOSAtomic, aSubValue);
	} else {
		(void)__sync_fetch_and_sub(&apAtomic->mOSAtomic, aSubValue);
	}

sub_atomic_exit:
	return lRet;
}

int32_t OAL_IncrementAtomic(struct OAL_Atomic *apAtomic, uint32_t *apOldValue)
{
	return OAL_AddAtomic(apAtomic, 1U, apOldValue);
}

int32_t OAL_DecrementAtomic(struct OAL_Atomic *apAtomic, uint32_t *apOldValue)
{
	return OAL_SubtractAtomic(apAtomic, 1U, apOldValue);
}
