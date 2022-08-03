/*
* Copyright 2022 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <string.h>

#include "CDD_Spt.h"
#include "Spt_Cfg.h"
#include "Spt_Internals.h"
#include "Spt_Irq_Config.h"
#include "Spt_Hw_Check.h"
#include "Spt_Hw_Ctrl.h"

#if defined(RSDK_AUTOSAR) || (!RSDK_OSENV_SA)
#include "Spt_Seq_Ctrl.h"
#endif

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

Std_ReturnType Spt_Run(Spt_DriverContextType const *const sptContext)
{
    Std_ReturnType                  retStatus = (Std_ReturnType)E_OK;
    volatile SPT_Type *const        pSptRegs = Spt_GetMemMap();
#if (!RSDK_OSENV_SA)
    Std_ReturnType seqStatus;
#endif

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_FUNC_START, (uint16)RSDK_TRACE_JOB_SPT_DRV_RUN, (uint32)gSptMemPer.state);

#if (!RSDK_OSENV_SA)
    retStatus = Spt_ApiSequenceTryEnter(&gSptMemPer.apiSeqCtrl);

#endif /* BM or ASR */

#if (SPT_DEV_ERROR_DETECT == STD_ON)
    if (retStatus == (Std_ReturnType)E_OK)
    {
        retStatus = Spt_ParamCheckRun(sptContext, gSptMemPer.state);
    }
#endif

    if (retStatus == (Std_ReturnType)E_OK)
    {
        retStatus = Spt_CheckRst(pSptRegs);
    }

    if (retStatus == (Std_ReturnType)E_OK)
    {
        /* Copy information to persistent memory to make it visible from the ISR: */
        gSptMemPer.ecsIsrCb = sptContext->ecsIsrCb;
        gSptMemPer.kernelRetPar = sptContext->kernelRetPar;
        gSptMemPer.evtIsrCb = sptContext->evtIsrCb;

        /* Clear all core event flags: */

        /* Prevent erroneous flags that could remain from a previous error */
        Spt_ClearEcsInterruptFlags(pSptRegs);

        if (gSptMemPer.prevOpMode != sptContext->opMode)
        {
            gSptMemPer.prevOpMode = sptContext->opMode;
            Spt_ConfigEcsInterrupts(pSptRegs, sptContext->opMode);
        }

        /* Initialize program start address register */
        SPT_HW_WRITE_BITS(pSptRegs->CS_PG_ST_ADDR, SPT_CS_PG_ST_ADDR_PG_ST_ADDR_MASK, SPT_CS_PG_ST_ADDR_PG_ST_ADDR(sptContext->kernelCodeAddr));

        /* Parse parameter list and pass them to SPT according to the calling convention: */
        retStatus = Spt_SetInputParams(sptContext->kernelParList);
    }

    if (retStatus == (Std_ReturnType)E_OK)
    {
        Spt_SetDrvState(SPT_STATE_HW_BUSY);

        RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_SPT_KERNEL_START, 0u);

        retStatus = Spt_StartExec(pSptRegs);
    }

    if (retStatus == (Std_ReturnType)E_OK)
    {
        if (sptContext->opMode == SPT_OP_MODE_NONBLOCK)
        {
            /* Do not wait for SPT completion, exit immediately to make the CPU available
             * Now the driver state can only be changed asynchronously by SptEcsIsr, or by
             * the user calling Spt_Stop or Spt_Setup */
        }
#if(SPT_RUN_POLL == STD_ON)
        else  /* Assume OP_MODE_BLOCK */
        {
            retStatus = Spt_WaitForSptDone(pSptRegs, sptContext->kernelRetPar);

        }
#endif
    }

#if (!RSDK_OSENV_SA)
    seqStatus = Spt_ApiSequenceExit(&gSptMemPer.apiSeqCtrl);  /* Must be called unconditionally, otherwise any subsequent API
                                                                 call will fail with RSDK_SPT_RET_WARN_DRV_BUSY. */
    if (retStatus == (Std_ReturnType)E_OK)
    {
        retStatus = seqStatus;  /* Report ApiSequenceExit failure only if it does not overwrite other error flags. */
    }
#endif /* RSDK_OSENV_SA */

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_FUNC_END, (uint16)RSDK_TRACE_JOB_SPT_DRV_RUN, (uint32)gSptMemPer.state);

    return retStatus;
}

/*================================================================================================*/
Std_ReturnType Spt_Setup(Spt_DriverInitType const *const pSptInitInfo)
{
    volatile SPT_Type *         pSptRegs = NULL;
    Std_ReturnType              retStatus = (Std_ReturnType)E_OK;
    Spt_DrvStateType            drvState = gSptMemPer.state;
#if (!RSDK_OSENV_SA)
    Std_ReturnType seqStatus;
#endif

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_FUNC_START, (uint16)RSDK_TRACE_JOB_SPT_DRV_INIT, (uint32)gSptMemPer.state);

#if (!RSDK_OSENV_SA)
    if (OAL_ExecuteOnce(&gSptMemPer.apiSeqCtrl.mutexOnceInitCtrl, Spt_ApiSequenceMutexInit) != 0)
    {
        retStatus = SPT_REPORT_ERROR(RSDK_SPT_RET_ERR_API_INIT_LOCK_FAIL, SPT_API_CALL, SPT_E_OTHER);
    }
    else
    {
        retStatus = Spt_ApiSequenceTryEnter(&gSptMemPer.apiSeqCtrl);
    }
#endif

#if (SPT_DEV_ERROR_DETECT == STD_ON)
    if (retStatus == (Std_ReturnType)E_OK)
    {
        retStatus = Spt_ParamCheckInit(pSptInitInfo);
    }
#endif

    if (retStatus == (Std_ReturnType)E_OK)
    {
        /* If this is not the first call to Spt_Setup and the user did not do a clean shutdown of the Driver
         * (using Spt_Stop), then we need to clean up previously allocated Driver resources */
        if ((drvState == SPT_STATE_INITIALIZED) || (drvState == SPT_STATE_HW_BUSY) || (drvState == SPT_STATE_FAULT))
        {
            retStatus = Spt_UnmapMem();
#if (!RSDK_OSENV_SA)
            if (retStatus == (Std_ReturnType)E_OK)
            {
                retStatus = SptIrqCaptureThreadStop(&(gSptMemPer.irqCapThreadData));
            }
#endif
        }
    }

    if (retStatus == (Std_ReturnType)E_OK)
    {
        /* Initialize driver's persistent memory. This must be done after the 'cleanup' section above, but before calling Spt_GetMemMap() */
        Spt_InitPersistentMem(&gSptMemPer, pSptInitInfo);

        /* Map SPT peripheral memory to the driver: */
        pSptRegs = Spt_GetMemMap();

#if (!RSDK_OSENV_SA)
        /* Start a separate thread to intercept SPT interrupts which are treated in the OS kernel */
        retStatus = SptIrqCaptureThreadStart(&(gSptMemPer.irqCapThreadData));
    }

    if (retStatus == (Std_ReturnType)E_OK)
    {
#endif
        retStatus = Spt_ConfigHw(pSptInitInfo, pSptRegs);
    }

    if (retStatus == (Std_ReturnType)E_OK)
    {
        Spt_SetDrvState(SPT_STATE_INITIALIZED);
    }

#if (!RSDK_OSENV_SA)
    seqStatus = Spt_ApiSequenceExit(&gSptMemPer.apiSeqCtrl);  /* Must be called unconditionally */

    if (retStatus == (Std_ReturnType)E_OK)
    {
        retStatus = seqStatus;  /* Report ApiSequenceExit failure only if it does not overwrite other error flags. */
    }
#endif

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_FUNC_END, (uint16)RSDK_TRACE_JOB_SPT_DRV_INIT, (uint32)gSptMemPer.state);

    return retStatus;
}

/*================================================================================================*/
Std_ReturnType Spt_Command(Spt_DriverCommandType const *const pSptCommand, Spt_DriverCmdResType *const pSptCmdResult)
{
    Std_ReturnType                  retStatus = (Std_ReturnType)E_OK;
    Spt_DrvStateType                drvState = gSptMemPer.state;
    volatile SPT_Type *const        pSptRegs = Spt_GetMemMap();
#if (!RSDK_OSENV_SA)
    Std_ReturnType seqStatus;
#endif

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_FUNC_START, (uint16)RSDK_TRACE_JOB_SPT_DRV_CMD, (uint32)gSptMemPer.state);

#if (!RSDK_OSENV_SA)
    retStatus = Spt_ApiSequenceTryEnter(&gSptMemPer.apiSeqCtrl);
#endif

#if (SPT_DEV_ERROR_DETECT == STD_ON)
    if ((retStatus == (Std_ReturnType)E_OK) && ((pSptCommand == NULL) || (pSptCmdResult == NULL)))
    {
        retStatus = SPT_REPORT_ERROR(RSDK_SPT_RET_ERR_INVALID_PARAM, SPT_API_CALL, SPT_E_INVALID_PARAM);
    }
#endif /* #if (SPT_DEV_ERROR_DETECT == STD_ON) */

    if ((retStatus == (Std_ReturnType)E_OK) && ((drvState == SPT_STATE_DISABLED) || (drvState == SPT_STATE_FAULT)))
    {
        retStatus = SPT_REPORT_ERROR(RSDK_SPT_RET_ERR_INVALID_STATE, SPT_API_CALL, SPT_E_INVALID_STATE);
    }

    if (retStatus == (Std_ReturnType)E_OK)
    {
        retStatus = Spt_CheckRst(pSptRegs);
    }

    if (retStatus == (Std_ReturnType)E_OK)
    {
        switch ((uint32)pSptCommand->cmdId)
        {
#if(SPT_DSP_ENABLE == STD_ON)
            case (uint32)SPT_CMD_GEN_DSP_CMD_CRC:
                {
#if (SPT_DEV_ERROR_DETECT == STD_ON)
                    if((pSptCommand->cmdParam == (uintptr)NULL))
                    {
                        retStatus = SPT_REPORT_ERROR(RSDK_SPT_RET_ERR_INVALID_PARAM, SPT_API_CALL, SPT_E_INVALID_PARAM);
                    }
                    else
#endif /* #if (SPT_DEV_ERROR_DETECT == STD_ON) */
                    {
                        Spt_DspCmdType* pDspCmd = (Spt_DspCmdType*)pSptCommand->cmdParam;
                        uint8 dspCmdVec[sizeof(pDspCmd->id)+sizeof(pDspCmd->arg)];
                        (void)memcpy(&dspCmdVec[0], &(pDspCmd->id), sizeof(pDspCmd->id));
                        (void)memcpy(&dspCmdVec[sizeof(pDspCmd->id)], (uint8*)(&pDspCmd->arg) , sizeof(pDspCmd->arg));
                        /* Compute 8-bit CRC on the DSP commmand "id" and "arg" fields. */
                        pDspCmd->crc = Spt_GenCrc8(dspCmdVec, (uint8)sizeof(dspCmdVec));
                    }
                }
                break;
#endif /* #if(SPT_DSP_ENABLE == STD_ON) */
            case (uint32)SPT_CMD_MEM_ERR_INJECT_EN:
                /* Enable injection of all possible parity errors for OPRAM and TRAM */
                SPT_HW_WRITE_BITS(pSptRegs->MEM_ERR_INJECT_CTRL, SPT_MEM_ERR_INJECT_CTRL_OR_PAR_ERR_INJ_MASK, SPT_MEM_ERR_INJECT_CTRL_OR_PAR_ERR_INJ(0xFu));
                SPT_HW_WRITE_BITS(pSptRegs->MEM_ERR_INJECT_CTRL, SPT_MEM_ERR_INJECT_CTRL_TR_PAR_ERR_INJ_MASK, SPT_MEM_ERR_INJECT_CTRL_TR_PAR_ERR_INJ(0xFu));
                break;
            case (uint32)SPT_CMD_MEM_ERR_INJECT_DIS:
                SPT_HW_WRITE_BITS(pSptRegs->MEM_ERR_INJECT_CTRL, SPT_MEM_ERR_INJECT_CTRL_OR_PAR_ERR_INJ_MASK, SPT_MEM_ERR_INJECT_CTRL_OR_PAR_ERR_INJ(0x0u));
                SPT_HW_WRITE_BITS(pSptRegs->MEM_ERR_INJECT_CTRL, SPT_MEM_ERR_INJECT_CTRL_TR_PAR_ERR_INJ_MASK, SPT_MEM_ERR_INJECT_CTRL_TR_PAR_ERR_INJ(0x0u));
                break;
            case (uint32)SPT_CMD_TRIGGER_SW_EVENT:
                /* Set all bits to cover all possible events configured in the SPT "wait" instruction */
                SPT_HW_WRITE_BITS(pSptRegs->CS_SW_EVTREG, SPT_CS_SW_EVTREG_SW_EVTREG_MASK, SPT_CS_SW_EVTREG_SW_EVTREG(pSptCommand->cmdParam));
                break;
#if defined(S32R45) && (!RSDK_OSENV_SA)
            case (uint32)SPT_CMD_BBE32_REBOOT:
				retStatus = SptBbe32Reboot(&(gSptMemPer.irqCapThreadData));
				break;
#endif
            default:
                retStatus = SPT_REPORT_ERROR(RSDK_SPT_RET_ERR_INVALID_PARAM, SPT_API_CALL, SPT_E_INVALID_PARAM);
                break;
        }
    }

#if (!RSDK_OSENV_SA)
    seqStatus = Spt_ApiSequenceExit(&gSptMemPer.apiSeqCtrl);  /* Must be called unconditionally */

    if (retStatus == (Std_ReturnType)E_OK)
    {
        retStatus = seqStatus;  /* Report ApiSequenceExit failure only if it does not overwrite other error flags. */
    }
#endif

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_FUNC_END, (uint16)RSDK_TRACE_JOB_SPT_DRV_CMD, (uint32)gSptMemPer.state);

    return retStatus;
}
/*================================================================================================*/
Std_ReturnType Spt_Stop(void)
{
    Std_ReturnType      retStatus = (Std_ReturnType)E_OK;
    Spt_DrvStateType    drvState = gSptMemPer.state;
#if (!RSDK_OSENV_SA)
    Std_ReturnType      seqStatus;
#endif

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_FUNC_START, (uint16)RSDK_TRACE_JOB_SPT_DRV_STOP, (uint32)gSptMemPer.state);

#if (!RSDK_OSENV_SA)
    retStatus = Spt_ApiSequenceTryEnter(&gSptMemPer.apiSeqCtrl);
#endif

    if (retStatus == (Std_ReturnType)E_OK)
    {
        if ((drvState <= SPT_STATE_DISABLED) || (drvState > SPT_STATE_FAULT))
        {
            /* In this case consider Spt_Setup() has not been called prior to this point as expected, so there is nothing to stop. */
            retStatus = SPT_REPORT_ERROR(RSDK_SPT_RET_ERR_INVALID_STATE, SPT_API_CALL, SPT_E_INVALID_STATE);
        }
        else
        {
            retStatus = Spt_StopHw();
        }
    }

    if (retStatus == (Std_ReturnType)E_OK)
    {
        retStatus = Spt_UnmapMem();
#if (!RSDK_OSENV_SA)
        if (retStatus == (Std_ReturnType)E_OK)
        {
            retStatus = SptIrqCaptureThreadStop(&(gSptMemPer.irqCapThreadData));
        }
#endif
        if (retStatus == (Std_ReturnType)E_OK)
        {
            Spt_SetDrvState(SPT_STATE_DISABLED);
        }
    }

#if (!RSDK_OSENV_SA)
    seqStatus = Spt_ApiSequenceExit(&gSptMemPer.apiSeqCtrl);  /* Must be called unconditionally */

    if (retStatus == (Std_ReturnType)E_OK)
    {
        retStatus = seqStatus;  /* Report ApiSequenceExit failure only if it does not overwrite other error flags. */
    }
    /* No need to destroy the mutex here, rely on the OS to clean it up when the user application process is terminated. */
#endif /* RSDK_OSENV_SA */

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_FUNC_END, (uint16)RSDK_TRACE_JOB_SPT_DRV_STOP, (uint32)gSptMemPer.state);

    return retStatus;
}

/*================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
