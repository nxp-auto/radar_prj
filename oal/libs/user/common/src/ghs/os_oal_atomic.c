/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_atomic.h>
#include <oal_log.h>
#include <bsp.h>

int32_t OAL_SetAtomic(struct OAL_Atomic *apAtomic, uint32_t aNewValue,
                      uint32_t *apOldValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, set_atomic_exit);

	if (apOldValue != NULL) {
		*apOldValue = (uint32_t)INTERRUPT_AtomicSwap(
		    &apAtomic->mOSAtomic, aNewValue);
	} else {
		(void)INTERRUPT_AtomicSwap(&apAtomic->mOSAtomic, aNewValue);
	}

set_atomic_exit:
	return lRet;
}

int32_t OAL_GetAtomic(struct OAL_Atomic *apAtomic, uint32_t *apValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, get_atomic_exit);
	OAL_CHECK_NULL_PARAM(apValue, get_atomic_exit);

	*apValue = (uint32_t)INTERRUPT_AtomicModify(&apAtomic->mOSAtomic, 0, 0);

get_atomic_exit:
	return lRet;
}

int32_t OAL_AddAtomic(struct OAL_Atomic *apAtomic, uint32_t aAddedValue,
                      uint32_t *apOldValue)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apAtomic, add_atomic_exit);

	if (apOldValue != NULL) {
		*apOldValue = (uint32_t)INTERRUPT_AtomicModify(
		    &apAtomic->mOSAtomic, 0, aAddedValue);
	} else {
		(void)INTERRUPT_AtomicModify(&apAtomic->mOSAtomic, 0,
		                             aAddedValue);
	}

add_atomic_exit:
	return lRet;
}

int32_t OAL_SubtractAtomic(struct OAL_Atomic *apAtomic, uint32_t aSubValue,
                           uint32_t *apOldValue)
{
	int32_t lRet      = 0;
	Address lSubValue = 0U;

	if (aSubValue > 0U) {
		/* Equivalent of -1 representation */
		lSubValue = ~((Address)0U);
		lSubValue -= (aSubValue - ((Address)1U));
	}

	OAL_CHECK_NULL_PARAM(apAtomic, sub_atomic_exit);

	/* Subtraction by addition */
	if (apOldValue != NULL) {
		*apOldValue = (uint32_t)INTERRUPT_AtomicModify(
		    &apAtomic->mOSAtomic, 0, lSubValue);
	} else {
		(void)INTERRUPT_AtomicModify(&apAtomic->mOSAtomic, 0,
		                             lSubValue);
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
