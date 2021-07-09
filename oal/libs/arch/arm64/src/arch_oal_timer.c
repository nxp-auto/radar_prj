/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <sa/oal_standalone_stubs.h>
#include <oal_irq_utils.h>
#include <oal_timer.h>
#include <oal_utils.h>
#include <arch_oal_timer.h>
#include <arch_oal_timespec.h>
#include <arm64_core_regs.h>

#define IRQ_TIMER (30)

void OAL_SA_ARCH_SetTimerTimeout(uint64_t aTimeout)
{
	OAL_ASM_WRITE_COPROC_REG(OAL_TIMER_COMP_VALUE_REG, aTimeout);
}

uint64_t OAL_SA_ARCH_GetTimerCounter(void)
{
	uint64_t lCounter;
	OAL_ASM_READ_COPROC_REG(OAL_TIMER_COUNTER_REG, lCounter);
	return lCounter;
}

void OAL_SA_ARCH_MaskTimerInterrupts(void)
{
	uint32_t lCtrlReg;
	OAL_ASM_READ_COPROC_REG(OAL_TIMER_CONTROL_REG, lCtrlReg);
	lCtrlReg |= (1U << OAL_TIMER_INTR_MASK_BIT);
	OAL_ASM_WRITE_COPROC_REG(OAL_TIMER_CONTROL_REG, lCtrlReg);
}

void OAL_SA_ARCH_EnableAndUnmaskTimer(void)
{
	uint32_t lCtrlReg;
	/* Enable timer and unmask interrupts */
	OAL_ASM_READ_COPROC_REG(OAL_TIMER_CONTROL_REG, lCtrlReg);
	lCtrlReg |= (1U << OAL_TIMER_ENABLE_BIT);
	lCtrlReg &= ~(1U << OAL_TIMER_INTR_MASK_BIT);
	OAL_ASM_WRITE_COPROC_REG(OAL_TIMER_CONTROL_REG, lCtrlReg);
}

void OAL_SA_ARCH_UpdateHWTimerTimeout(uint64_t aDeltaTicks,
                                  uint64_t *apAbsTimeTicks)
{
	if (apAbsTimeTicks == NULL) {
		OAL_LOG_ERROR("Invalid argument\n");
		goto set_timeout_exit;
	}

	if (aDeltaTicks == 0U) {
		goto set_timeout_exit;
	}

	/* Write countdown timer */
	OAL_ASM_WRITE_COPROC_REG(OAL_TIMER_COUNT_DOWN_REG, aDeltaTicks);

	/**
	 * Update expiration time. The hardware computes it based on
	 * cntp_tval_el0.
	 */
	OAL_ASM_READ_COPROC_REG(OAL_TIMER_COMP_VALUE_REG, *apAbsTimeTicks);

set_timeout_exit:
	return;
}

uint8_t OAL_SA_ARCH_IsTimerEnabled(void)
{
	uint32_t lCtrlReg;
	uint8_t lRet = 0;
	OAL_ASM_READ_COPROC_REG(OAL_TIMER_CONTROL_REG, lCtrlReg);

	if ((lCtrlReg & (1U << OAL_TIMER_ENABLE_BIT)) != 0U) {
		lRet = 1U;
	}

	return lRet;
}

void OAL_SA_ARCH_UnmaskTimerInterrupts(void)
{
	uint32_t lCtrlReg;
	OAL_ASM_READ_COPROC_REG(OAL_TIMER_CONTROL_REG, lCtrlReg);
	lCtrlReg &= ~(1U << OAL_TIMER_INTR_MASK_BIT);
	OAL_ASM_WRITE_COPROC_REG(OAL_TIMER_CONTROL_REG, lCtrlReg);
}

int32_t OAL_SA_ARCH_RegisterTimerIrq(OAL_irq_handler_t aHandler)
{
	return registerIrq(IRQ_TIMER, aHandler);
}

void OAL_SA_ARCH_ClearTimerState(void)
{
	/* Disable HW timer if there are no SW timers */
	uint32_t lDisableTimer = 0;

	OAL_SA_ARCH_MaskTimerInterrupts();
	deregisterIrq(IRQ_TIMER);

	OAL_ASM_READ_COPROC_REG(OAL_TIMER_CONTROL_REG, lDisableTimer);
	lDisableTimer &= ~(1U << OAL_TIMER_ENABLE_BIT);
	OAL_ASM_WRITE_COPROC_REG(OAL_TIMER_CONTROL_REG, lDisableTimer);
}
