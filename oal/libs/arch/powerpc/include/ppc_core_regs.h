/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_PPC_CORE_REGS_H
#define OAL_PPC_CORE_REGS_H

/* Timer regs */

#if defined(OAL_SA_E200Z4) || defined(OAL_SA_E200Z7)
/* E200Z4  / E200Z7 registers */
#define OAL_SPR_DEC (22)
#define OAL_SPR_DECAR (54)
#define OAL_SPR_TBL_READ (268)
#define OAL_SPR_TBL_WRITE (284)
#define OAL_SPR_TBU_READ (269)
#define OAL_SPR_TBU_WRITE (285)
#define OAL_SPR_TCR (340)
#define OAL_SPR_TSR (336)
#define OAL_SPR_L1CFG0 (515)
#define OAL_SPR_HID0 (1008)
#else
/* MPC85xx registers - QEMU */
#define OAL_SPR_DEC (22)
#define OAL_SPR_DECAR (54)
#define OAL_SPR_TBL_READ (284)
#define OAL_SPR_TBL_WRITE (284)
#define OAL_SPR_TBU_READ (285)
#define OAL_SPR_TBU_WRITE (285)
#define OAL_SPR_TCR (340)
#define OAL_SPR_TSR (336)
#define OAL_SPR_L1CFG0 (515)
#define OAL_SPR_HID0 (1008)
#endif

#define OAL_PPC_DEC OAL_SPR_DEC
#define OAL_PPC_DECAR OAL_SPR_DECAR
#define OAL_PPC_TBL_RO OAL_SPR_TBL_READ
#define OAL_PPC_TBL_WO OAL_SPR_TBL_WRITE
#define OAL_PPC_TBU_RO OAL_SPR_TBU_READ
#define OAL_PPC_TBU_WO OAL_SPR_TBU_WRITE
#define OAL_PPC_TCR OAL_SPR_TCR
#define OAL_PPC_TSR OAL_SPR_TSR
#define OAL_PPC_L1CFG0 OAL_SPR_L1CFG0
#define OAL_PPC_HID0 OAL_SPR_HID0

/* OAL_PPC_TCR fields */
/* Decrementer interrupt */
#define OAL_TCR_DIE_BIT (26U)

/* OAL_PPC_TSR fields */
/* Decrementer interrupt status */
#define OAL_TSR_DIS_BIT (27U)

/* MSR fields */
#define OAL_MSR_EE_BIT (15U)
#define OAL_MSR_CE_BIT (17U)
#define OAL_MSR_WE_BIT (18U)

/* L1 Cache Configuration Register 0 fields */
#define OAL_L1CFG0_CBSIZE_BIT (23U)

/* Hardware Implementation Dependent Register 0 */
#define OAL_HID0_SLEEP_BIT (21U)
#define OAL_HID0_NAP_BIT (22U)
#define OAL_HID0_DOZE_BIT (23U)

#define OAL_ASM_READ_SPECIAL_REG(REG, VALUE)                                       \
	do {                                                                   \
		__asm__ volatile("mfspr %0, " OAL_XSTR(REG)                    \
		                 : "=r"(VALUE)::"memory");                     \
	} while (0 == 1)

#define OAL_ASM_WRITE_SPECIAL_REG(REG, VALUE)                                      \
	do {                                                                   \
		__asm__ volatile("mtspr " OAL_XSTR(REG) ", %0"                 \
		                 :                                             \
		                 : "r"(VALUE)                                  \
		                 : "memory");                                  \
	} while (0 == 1)

/* Read / Write MSR - Machine State Register */
#define OAL_ASM_READ_MSR_REG(VALUE)                                                \
	do {                                                                   \
		__asm__ volatile("mfmsr %0" : "=r"(VALUE)::"memory");          \
	} while (0 == 1)

#define OAL_ASM_WRITE_MSR_REG(VALUE)                                               \
	do {                                                                   \
		__asm__ volatile("mtmsr %0" : : "r"(VALUE) : "memory");        \
	} while (0 == 1)

#define OAL_ASM_SYNC_INSTRUCTIONS()                                                \
	do {                                                                   \
		__asm__ volatile("isync" ::: "memory");                        \
	} while (0 == 1)

#define OAL_ASM_SYNC_MEMORY()                                                      \
	do {                                                                   \
		__asm__ volatile("msync" ::: "memory");                        \
	} while (0 == 1)

#endif
