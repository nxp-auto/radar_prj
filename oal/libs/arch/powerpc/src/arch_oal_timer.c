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
#include <ppc_core_regs.h>

#define IRQ_TIMER (10)

void OAL_SA_ARCH_MaskTimerInterrupts(void)
{
	uint32_t lCtrlReg, lIrqStatus;

	OAL_ASM_READ_SPECIAL_REG(OAL_PPC_TCR, lCtrlReg);
	lCtrlReg &= ~(1UL << OAL_TCR_DIE_BIT);
	OAL_ASM_WRITE_SPECIAL_REG(OAL_PPC_TCR, lCtrlReg);

	/* Halt decrementer */
	OAL_ASM_WRITE_SPECIAL_REG(OAL_PPC_DEC, 0U);

	/* Clear decrementer interrupt status */
	OAL_ASM_READ_SPECIAL_REG(OAL_PPC_TSR, lIrqStatus);
	lIrqStatus &= ~(1UL << OAL_TSR_DIS_BIT);
	OAL_ASM_WRITE_SPECIAL_REG(OAL_PPC_TSR, lIrqStatus);
}

uint8_t OAL_SA_ARCH_IsTimerEnabled(void)
{
	uint8_t lRet = 0U;
	uint32_t lCtrlReg, lMsrReg;

	OAL_ASM_READ_MSR_REG(lMsrReg);
	OAL_ASM_READ_SPECIAL_REG(OAL_PPC_TCR, lCtrlReg);

	if (((lCtrlReg & (1UL << OAL_TSR_DIS_BIT)) != 0U) &&
	    ((lMsrReg & (1UL << OAL_MSR_EE_BIT)) != 0U)) {
		lRet = 1U;
	}

	return lRet;
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

	*apAbsTimeTicks = OAL_GetTicks() + aDeltaTicks;
	OAL_ASM_WRITE_SPECIAL_REG(OAL_PPC_DEC, (uint32_t)aDeltaTicks);

set_timeout_exit:
	return;
}

void OAL_SA_ARCH_UnmaskTimerInterrupts(void)
{
	uint32_t lCtrlReg;
	OAL_ASM_READ_SPECIAL_REG(OAL_PPC_TCR, lCtrlReg);
	lCtrlReg |= (1UL << OAL_TCR_DIE_BIT);
	OAL_ASM_WRITE_SPECIAL_REG(OAL_PPC_TCR, lCtrlReg);
}

void OAL_SA_ARCH_EnableAndUnmaskTimer(void)
{
	uint32_t lMsrReg;

	OAL_ASM_READ_MSR_REG(lMsrReg);

	/* Enable External Interrupt */
	if ((lMsrReg & (1UL << OAL_MSR_EE_BIT)) == 0U) {
		lMsrReg |= (1UL << OAL_MSR_EE_BIT);
		OAL_ASM_WRITE_MSR_REG(lMsrReg);
	}

	OAL_SA_ARCH_UnmaskTimerInterrupts();
}

int32_t OAL_SA_ARCH_RegisterTimerIrq(OAL_irq_handler_t aHandler)
{
	return registerIrq(IRQ_TIMER, aHandler);
}

void OAL_SA_ARCH_ClearTimerState(void)
{
	OAL_SA_ARCH_MaskTimerInterrupts();

	deregisterIrq(IRQ_TIMER);
}

uint64_t OAL_SA_ARCH_GetTimerCounter(void) { return OAL_GetTicks(); }
void OAL_SA_ARCH_SetTimerTimeout(uint64_t aTimeout)
{
	uint64_t lDeltaTicks = aTimeout - OAL_GetTicks();
	OAL_ASM_WRITE_SPECIAL_REG(OAL_PPC_DEC, (uint32_t)lDeltaTicks);
}
