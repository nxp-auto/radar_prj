/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_utils.h>
#include <os_oal_spinlock.h>
#include <arch_oal_spinlock.h>

#define ASM_MASK_INTERRUPTS(VAR)                                               \
	__asm__ volatile(                                                      \
	    "	mrs %0, daif\n"                                                  \
	    "	msr daifset, #3"                                                 \
	    : "=r"(VAR)                                                        \
	    :                                                                  \
	    : "memory")

#define ASM_UNMASK_INTERRUPTS(VAR)                                             \
	__asm__ volatile("	msr daif, %0" : : "r"(VAR) : "memory")

int32_t OAL_ARCH_LockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -1;
	} else {
		/* Loop while the lock is taken */
		while (apLock->mValue == 1U) {
		};
		apLock->mValue = 1U;
	}

	return lRet;
}

int32_t OAL_ARCH_UnlockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;
	if (apLock == NULL) {
		lRet = -1;
	} else {
		apLock->mValue = 0U;
	}

	return lRet;
}

int32_t OAL_ARCH_LockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	OAL_UNUSED_ARG(apLock);

	/*
	 * Assuming an use case without SMP, there is a single thread
	 * that is running and the interrupts are the
	 * only ones that can execute apart from our thread, we can
	 * aquire a critical region by masking the interrupts
	 */
	ASM_MASK_INTERRUPTS(*apFlags);

	return 0;
}

int32_t OAL_ARCH_UnlockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	OAL_UNUSED_ARG(apLock);

	ASM_UNMASK_INTERRUPTS(*apFlags);

	return 0;
}
