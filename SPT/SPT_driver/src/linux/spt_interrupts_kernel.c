/*
* Copyright 2019-2020 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <linux/interrupt.h>
#include "spt_driver_module.h"
#include "Spt_Oal.h"
#include "Spt_Hw_Check.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

static void SptEcsIsr(evtSharedData_t *evtData)
{
    volatile SPT_Type *const    pSptRegs = sptDevice.pSptRegs;
    rsdkStatus_t                isrStatus;

    isrStatus = Spt_CheckAndResetHwError(pSptRegs, &(evtData->errInfo));

    //first things first: look for errors
    if (isrStatus != RSDK_SUCCESS)
    {
        PR_ALERT("spt_driver module: SptEcsIsr error: %x.\n", isrStatus);
    }
    // if the SPT_CS_STATUS0[PS_STOP] bit is set, then it means the SPT has finished running the command sequence
    else if ((pSptRegs->CS_STATUS0 & SPT_CS_STATUS0_PS_STOP_MASK) != 0u)
    {
        //clear the interrupt source
        pSptRegs->CS_STATUS0 = SPT_CS_STATUS0_PS_STOP_MASK;

        isrStatus = RSDK_SUCCESS;
        PR_ALERT("spt_driver module: SptEcsIsr: SPT stopped.\n");
    }
    else
    {
        //something must have gone wrong - reached this place not knowing root cause of this ISR
        isrStatus = RSDK_SPT_RET_ERR_OTHER;
        PR_ALERT("spt_driver module: SptEcsIsr: unexpected interrupt!.\n");
    }

    evtData->isrStatus = isrStatus;
}

static void SptEvtIsr(evtSharedData_t *evtData)
{
    volatile SPT_Type *const    pSptRegs = sptDevice.pSptRegs;
    uint32_t                    evtInfo = (uint32_t)pSptRegs->CS_EVTREG1;

    //write 1 to clear the interrupt source:
    pSptRegs->CS_EVTREG1 = SPT_CS_EVTREG1_EVTREG1_MASK;  //clear all event bits

    evtData->isrStatus = RSDK_SUCCESS;
    evtData->errInfo = evtInfo;
}

static void SptDspIsr(evtSharedData_t *evtData)
{
    volatile SPT_Type *const    pSptRegs = sptDevice.pSptRegs;
    rsdkStatus_t                dspErrCode = (uint32_t)pSptRegs->DSP_ERR_INFO_REG;

    //write 1 to clear the interrupt source:
    pSptRegs->DSP_ERR_INFO_REG = SPT_DSP_ERR_INFO_REG_DSP_ERR_INFO_MASK;

    evtData->isrStatus = dspErrCode;
    evtData->errInfo = (uint32_t)pSptRegs->DSP_DEBUG1_REG;
}

/**
 * @brief   SPT irq handler
 */
irqreturn_t SptDevIrqHandler(int irq, void *pDev)
{
    evtSharedData_t     evtData;
    irqreturn_t         irqStatus = IRQ_HANDLED;
    int32_t             oalStatus;
    uint32_t            queueIdxWr;

    UNUSED_ARG(pDev);
    PR_ALERT("spt_driver module: SptDevIrqHandler() is called for irq ID: %d \n", irq);

    if ((uint32_t)irq == sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_ECS])
    {
        evtData.evtType = SPT_OAL_RPC_EVT_IRQ_ECS;
        SptEcsIsr(&evtData);
    }
    else if ((uint32_t)irq == sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_EVT])
    {
        evtData.evtType = SPT_OAL_RPC_EVT_IRQ_EVT1;
        SptEvtIsr(&evtData);
    }
    else if ((uint32_t)irq == sptDevice.dtsInfo.irqId[SPT_DTS_IRQ_IDX_DSP])
    {
        evtData.evtType = SPT_OAL_RPC_EVT_IRQ_DSP;
        SptDspIsr(&evtData);
    }
    else
    {
        PR_ERR("SptDevIrqHandler: unsupported interrupt ID: %d !\n", irq);
        irqStatus = IRQ_NONE;
    }

    if (irqStatus != IRQ_NONE)
    {
        /* Send notification to user space:*/
        PR_ALERT("SptDevIrqHandler: Waking up user process\n");

        /* Get and update the queue write index.
         * By using 'atomic_inc_return' we can be sure that each IRQ handler that executes concurrently
         * will get a different value for 'queueIdxWr'. By calling 'atomic_cmpxchg' the value of the index
         * will be wrapped if needed. This will be performed successfully only by the handler that has the
         * most up to date value of 'queueIdxWr'. The 'atomic_cmpxchg' call ensures a correct wrap-around
         * of the index, regardless of the queue size (if the queue size is not a power of 2, the default
         * wrap-around of the atomic_t variable - int - will corrupt the queue).
         */
        queueIdxWr = (uint32_t)atomic_inc_return(&(sptDevice.queueIdxWr));
        (void)atomic_cmpxchg(&(sptDevice.queueIdxWr), (int32_t)queueIdxWr, (int32_t)(queueIdxWr % SPT_DATA_Q_SIZE));

        /* Compute local index ('atomic_inc_return' returns the incremented value) */
        queueIdxWr = (queueIdxWr - 1u) % SPT_DATA_Q_SIZE;

        /* Write current evtData to the head of the queue */
        sptDevice.queueEvtData[queueIdxWr] = evtData;

        /* Increment the number of interrupts that require evtData transfer */
        if ((uint32_t)atomic_inc_return(&(sptDevice.irqsNotServed)) > (SPT_DATA_Q_SIZE - SPT_DATA_Q_CNT_LAST))
        {
            PR_ALERT("SptDevIrqHandler: Data queue almost full!\n");
        }

        /* Signal the user space thread */
        oalStatus = OAL_WakeUpInterruptible(&(sptDevice.irqWaitQ));
        if (oalStatus != 0)
        {
            PR_ERR("SptDevIrqHandler: Cannot wake up user process!\n");
            irqStatus = IRQ_NONE;
        }
    }

    return irqStatus;
}

#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
