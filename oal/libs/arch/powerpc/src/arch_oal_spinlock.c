/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <os_oal_spinlock.h>
#include <arch_oal_spinlock.h>
#include <ppc_core_regs.h>

#define UNLOCKED_STATE (0U)
#define LOCKED_STATE (1U)

#define OAL_ASM_PPC_TEST_AND_SET(ADDR, OLD_VAL, TEST_VAL, SET_VAL)             \
	do {                                                                   \
		__asm__ volatile(                                              \
		    /* Load and reserve value */                               \
		    "spinloop: lwarx  %[oldval], 0, %[testaddr]\n"             \
		    /* Equal ?*/                                               \
		    "          cmpw   %[oldval], %[testval]\n"                 \
		    "          bne    spindone\n"                              \
		    "          stwcx. %[setval], 0, %[testaddr]\n"             \
		    "          bne-   spinloop\n"                              \
		    "spindone:\n"                                              \
		    : "+m"(*(ADDR)), [oldval] "=&r"(OLD_VAL) /* Output */      \
		    : [testaddr] "r"(ADDR), [testval] "r"(TEST_VAL),           \
		    [setval] "r"(SET_VAL) /* Input */                          \
		    : "cc" /* Keep conditional flags */                        \
		    );                                                         \
	} while (0 == 1)


static uint8_t testAndSet(volatile uint32_t *apAddr, uint32_t aTestValue,
                          uint32_t aSetValue)
{
	uint32_t lOldVal;
	uint32_t lAddrVal;

	OAL_ASM_PPC_TEST_AND_SET(apAddr, lOldVal, aTestValue, aSetValue);
	lAddrVal = *apAddr;

	return (((lOldVal == aTestValue) && (lAddrVal == aSetValue)) ? (1U)
	                                                            : (0U));
}

static void lockSpinAddress(volatile uint32_t *apAddr)
{
	/* Busy-waiting while in LOCKED state */
	while (testAndSet(apAddr, UNLOCKED_STATE, LOCKED_STATE) == 0U) {
	}

	OAL_ASM_SYNC_INSTRUCTIONS();
}

static void unlockSpinAddress(volatile uint32_t *apAddr)
{
	OAL_ASM_SYNC_MEMORY();
	*apAddr = UNLOCKED_STATE;
}

int32_t OAL_ARCH_LockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apLock, lock_spin_exit);

	lockSpinAddress(&apLock->mValue);

lock_spin_exit:
	return lRet;
}

int32_t OAL_ARCH_UnlockSpin(OAL_spinlock_t *apLock)
{
	int32_t lRet = 0;

	OAL_CHECK_NULL_PARAM(apLock, unlock_spin_exit);

	unlockSpinAddress(&apLock->mValue);

unlock_spin_exit:
	return lRet;
}

int32_t OAL_ARCH_LockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = 0;

	OAL_UNUSED_ARG(apFlags);
	OAL_CHECK_NULL_PARAM(apLock, lock_spin_exit);

	lockSpinAddress(apLock);

lock_spin_exit:
	return lRet;
}

int32_t OAL_ARCH_UnlockIRQSpin(OAL_irqspinlock_t *apLock, uint64_t *apFlags)
{
	int32_t lRet = 0;

	OAL_UNUSED_ARG(apFlags);
	OAL_CHECK_NULL_PARAM(apLock, unlock_spin_exit);

	unlockSpinAddress(apLock);

unlock_spin_exit:
	return lRet;
}
