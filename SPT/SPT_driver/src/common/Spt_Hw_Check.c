/*
* Copyright 2022 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Spt_Hw_Check.h"
#include "Spt_Hw_Defs.h"
#include "Spt_Internals.h"
#include "Spt_Cfg.h"

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

/* Description: look for hw errors in SPT regs, set errInfo, then reset them. */
Std_ReturnType Spt_CheckAndResetHwError(volatile SPT_Type *const pSptRegs, uint32 *errInfo)
{
    Std_ReturnType status = (Std_ReturnType)E_OK;

    *errInfo = 0;

    /* If any bit set in the corresponding status register then output an error code and clear the register error flags */
    if (SPT_HW_READ_REG(pSptRegs->MEM_ERR_STATUS) != 0u)
    {
        *errInfo = SPT_HW_READ_REG(pSptRegs->MEM_ERR_STATUS);
        SPT_HW_WRITE_REG(pSptRegs->MEM_ERR_STATUS, SPT_MEM_ERR_STATUS_W1C_MASK);
        status = RSDK_SPT_RET_ERR_MEM;
    }
    else if (SPT_HW_READ_REG(pSptRegs->HW_ACC_ERR_STATUS) != 0u)
    {
        *errInfo = SPT_HW_READ_REG(pSptRegs->HW_ACC_ERR_STATUS);
        SPT_HW_WRITE_REG(pSptRegs->HW_ACC_ERR_STATUS, SPT_HW_ACC_ERR_STATUS_W1C_MASK);
        status = RSDK_SPT_RET_ERR_HW_ACC;
    }
    else if (SPT_HW_READ_REG(pSptRegs->CS_STATUS1) != 0u)
    {
        *errInfo = SPT_HW_READ_REG(pSptRegs->CS_STATUS1);
        SPT_HW_WRITE_REG(pSptRegs->CS_STATUS1, SPT_CS_STATUS1_W1C_MASK);
        status = RSDK_SPT_RET_ERR_ILLOP;
    }
    else
    {
        /* "default" case, nothing to do */
        status = (Std_ReturnType)E_OK;
    }

    /* Split in blocks of 3 checks, to avoid having code nesting level > 4 */
    if (status == (Std_ReturnType)E_OK)
    {
        if (SPT_HW_READ_REG(pSptRegs->HIST_OVF_STATUS0) != 0u)
        {
            *errInfo = SPT_HW_READ_REG(pSptRegs->HIST_OVF_STATUS0);
            SPT_HW_WRITE_REG(pSptRegs->HIST_OVF_STATUS0, SPT_HIST_OVF_W1C_MASK);
            status = RSDK_SPT_RET_ERR_HIST_OVF0;
        }
        else if (SPT_HW_READ_REG(pSptRegs->HIST_OVF_STATUS1) != 0u)
        {
            *errInfo = SPT_HW_READ_REG(pSptRegs->HIST_OVF_STATUS1);
            SPT_HW_WRITE_REG(pSptRegs->HIST_OVF_STATUS1, SPT_HIST_OVF_W1C_MASK);
            status = RSDK_SPT_RET_ERR_HIST_OVF1;
        }
        else if (SPT_HW_READ_REG(pSptRegs->HW2_ACC_ERR_STATUS) != 0u)
        {
            *errInfo = SPT_HW_READ_REG(pSptRegs->HW2_ACC_ERR_STATUS);
            SPT_HW_WRITE_REG(pSptRegs->HW2_ACC_ERR_STATUS, SPT_HW2_ACC_ERR_STATUS_W1C_MASK);
            status = RSDK_SPT_RET_ERR_HW2_ACC;
        }
        else
        {
            /* "default" case, nothing to do */
            status = (Std_ReturnType)E_OK;
        }
    }

    /* Split in blocks of 3 checks, to avoid having code nesting level > 4 */
    if (status == (Std_ReturnType)E_OK)
    {
        if ((SPT_HW_READ_REG(pSptRegs->GBL_STATUS) & SPT_GBL_STATUS_ERR_W1C_MASK) != 0u)
        {
            *errInfo = SPT_HW_READ_REG(pSptRegs->GBL_STATUS) & SPT_GBL_STATUS_ERR_W1C_MASK;
            SPT_HW_WRITE_REG(pSptRegs->GBL_STATUS, SPT_GBL_STATUS_ERR_W1C_MASK);
            status = RSDK_SPT_RET_ERR_DMA;
        }
        else if ((SPT_HW_READ_REG(pSptRegs->WR_ACCESS_ERR_REG) & SPT_WR_ACCESS_ERR_REG_W1C_MASK) != 0u)
        {
            *errInfo = SPT_HW_READ_REG(pSptRegs->WR_ACCESS_ERR_REG) & SPT_WR_ACCESS_ERR_REG_W1C_MASK;
            SPT_HW_WRITE_REG(pSptRegs->WR_ACCESS_ERR_REG, SPT_WR_ACCESS_ERR_REG_W1C_MASK);
            status = RSDK_SPT_RET_ERR_WR;
        }
        else
        {
            /* "default" case, nothing to do */
            status = (Std_ReturnType)E_OK;
        }
    }

    /* Split in blocks of 3 checks, to avoid having code nesting level > 4 */
    if (status == (Std_ReturnType)E_OK)
    {
        if (SPT_HW_READ_REG(pSptRegs->SCS0_STATUS1) != 0u)
        {
            *errInfo = SPT_HW_READ_REG(pSptRegs->SCS0_STATUS1);
            SPT_HW_WRITE_REG(pSptRegs->SCS0_STATUS1, SPT_SCS_STATUS1_W1C_MASK);
            status = RSDK_SPT_RET_ERR_ILLOP_SCS0;
        }
        else if (SPT_HW_READ_REG(pSptRegs->SCS1_STATUS1) != 0u)
        {
            *errInfo = SPT_HW_READ_REG(pSptRegs->SCS1_STATUS1);
            SPT_HW_WRITE_REG(pSptRegs->SCS1_STATUS1, SPT_SCS_STATUS1_W1C_MASK);
            status = RSDK_SPT_RET_ERR_ILLOP_SCS1;
        }
        else
        {
            /* "default" case, nothing to do */
            status = (Std_ReturnType)E_OK;
        }
    }

    //split in blocks of 3 checks, to avoid having code nesting level > 4
    if (status == (Std_ReturnType)E_OK)
    {
        if (SPT_HW_READ_REG(pSptRegs->SCS2_STATUS1) != 0u)
        {
            *errInfo = SPT_HW_READ_REG(pSptRegs->SCS2_STATUS1);
            SPT_HW_WRITE_REG(pSptRegs->SCS2_STATUS1, SPT_SCS_STATUS1_W1C_MASK);
            status = RSDK_SPT_RET_ERR_ILLOP_SCS2;
        }
        else if (SPT_HW_READ_REG(pSptRegs->SCS3_STATUS1) != 0u)
        {
            *errInfo = SPT_HW_READ_REG(pSptRegs->SCS3_STATUS1);
            SPT_HW_WRITE_REG(pSptRegs->SCS3_STATUS1, SPT_SCS_STATUS1_W1C_MASK);
            status = RSDK_SPT_RET_ERR_ILLOP_SCS3;
        }
        else
        {
            //"default" case, nothing to do
        }
    }

    return status;
}

Std_ReturnType Spt_CheckRst(volatile SPT_Type *pSptRegs)
{
    Std_ReturnType      retStatus = (Std_ReturnType)E_NOT_OK;
    volatile uint32     timeout = 10u;

    /* Wait for a little while (32 cycles of the SPT domain), to make sure any previous PROC_STATE value has changed,
     * to avoid a possible race condition (e.g. triggered by setting the SPT_EN or PG_ST_CTRL bits right before calling
     * this function) */
    while (timeout > 0u)
    {
        if (SPT_HW_READ_BITS(pSptRegs->CS_STATUS3, SPT_CS_STATUS3_PROC_STATE_MASK, SPT_CS_STATUS3_PROC_STATE_SHIFT) != SPT_SEQUENCER_STATE_RST)
        {
            retStatus = (Std_ReturnType)E_OK;
            break;
        }

        timeout--;
    }

    if (retStatus != (Std_ReturnType)E_OK)
    {
        retStatus = SPT_REPORT_ERROR(RSDK_SPT_RET_ERR_HW_RST, SPT_HW_CHECK, SPT_E_HW_RST);
    }

    return retStatus;
}

Std_ReturnType Spt_CheckUnexpectedStop(volatile SPT_Type *const pSptRegs, Spt_DrvStateType drvState)
{
    Std_ReturnType  retStatus = (Std_ReturnType)E_OK;
    uint8           sptCurrentInstrOpcode = 0;

    /* Get the current SPT instruction opcode (bits 127:122 ) */
    sptCurrentInstrOpcode = (uint8)(SPT_HW_READ_REG(pSptRegs->CS_CURR_INST3) >> 26u);

    /* Check for unexpected SPT_IRQ_ECS STOP interrupt:
     * the STOP interrupt can only be issued when the 'current instruction' in the command sequencer
     * is a 'STOP' and the Driver state is either 'BUSY' (running a kernel) or 'FAULT' (in case of a non-command SPT error) */
    if (!((drvState == SPT_STATE_HW_BUSY) || (drvState == SPT_STATE_FAULT)) &&
        (sptCurrentInstrOpcode == SPT_STOP_OPCODE))
    {
        retStatus = (Std_ReturnType)E_NOT_OK;
    }

    return retStatus;
}

#ifdef __cplusplus
}
#endif

/** @} */
