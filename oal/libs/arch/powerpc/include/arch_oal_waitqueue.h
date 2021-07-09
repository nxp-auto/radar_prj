/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_ARCH_WAITQUEUE_H
#define OAL_ARCH_WAITQUEUE_H
#include <oal_utils.h>
#include <ppc_core_regs.h>

static inline void OAL_ARCH_PPCSleep(void)
{
	uint32_t lMsrVal;
	uint32_t lHid0Val;

	OAL_ASM_READ_SPECIAL_REG(OAL_PPC_HID0, lHid0Val);
	OAL_ASM_READ_MSR_REG(lMsrVal);

	OAL_ASM_SYNC_MEMORY();

	OAL_ASM_WRITE_SPECIAL_REG(OAL_PPC_HID0, lHid0Val |
                                            (1UL << OAL_HID0_SLEEP_BIT) |
	                                    (1UL << OAL_HID0_DOZE_BIT));

	OAL_ASM_WRITE_MSR_REG(lMsrVal | (1UL << OAL_MSR_EE_BIT) |
                          (1UL << OAL_MSR_CE_BIT) |
	                  (1UL << OAL_MSR_WE_BIT));

	OAL_ASM_SYNC_INSTRUCTIONS();
}

#define OAL_SA_ARCH_WAIT_EVENT()                                               \
	do {                                                                   \
		OAL_ARCH_PPCSleep();                                           \
	} while (0 == 1)

/**
 * Relies on interrupts which will wake-up the core periodically.
 * E.g. DECR / OAL_PPC_DECAR interrupt or any other external interrupts.
 */
#define OAL_SA_ARCH_GENERATE_EVENT()                                           \
	do {                                                                   \
	} while (0 == 1)

#endif
