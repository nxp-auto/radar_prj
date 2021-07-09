/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_ARM64_CORE_REGS_H
#define OAL_ARM64_CORE_REGS_H

/* Timer regs */
#define OAL_TIMER_CONTROL_REG "cntp_ctl_el0"
#define OAL_TIMER_COMP_VALUE_REG "cntp_cval_el0"
#define OAL_TIMER_COUNTER_REG "cntpct_el0"
#define OAL_TIMER_COUNT_DOWN_REG "cntp_tval_el0"
#define OAL_TIMER_FREQ_REG "cntfrq_el0"

#define OAL_TIMER_ENABLE_BIT (0U)
#define OAL_TIMER_INTR_MASK_BIT (1U)

/* Cache reg */
#define OAL_CACHE_TYPE_REG "ctr_el0"

#define OAL_D_MIN_LINE_POS (16U)
#define OAL_D_MIN_LINE_MASK (((uint32_t)0xFU) << OAL_D_MIN_LINE_POS)

#define OAL_ASM_WRITE_COPROC_REG(REG, VALUE)                                   \
	do {                                                                   \
		__asm__ volatile("msr " REG ", %0" : : "r"(VALUE));            \
	} while (0 == 1)

#define OAL_ASM_READ_COPROC_REG(REG, VALUE)                                    \
	do {                                                                   \
		__asm__ volatile("mrs %0, " REG : "=r"(VALUE));                \
	} while (0 == 1)

#endif
