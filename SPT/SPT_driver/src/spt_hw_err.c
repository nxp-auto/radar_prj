/*
* Copyright 2019 - 2021 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "spt_hw_err.h"
#include "spt_hw_defs.h"
#include "spt_oal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

//Description: look for hw errors in SPT regs, set errInfo, then reset them.
rsdkStatus_t SptCheckAndResetHwError(volatile struct SPT_tag *const pSptRegs, uint32_t *errInfo)
{
    rsdkStatus_t status = RSDK_SUCCESS;

    *errInfo = 0;

    // if any bit set in the corresponding status register then output an error code and clear the register error flags
    if (HW_READ(pSptRegs->MEM_ERR_STATUS.R) != 0u)
    {
        *errInfo = HW_READ(pSptRegs->MEM_ERR_STATUS.R);
        HW_WRITE(pSptRegs->MEM_ERR_STATUS.R, MEM_ERR_STATUS_W1C_MASK);
        status = RSDK_SPT_RET_ERR_MEM;
    }
    else if (HW_READ(pSptRegs->HW_ACC_ERR_STATUS.R) != 0u)
    {
        *errInfo = HW_READ(pSptRegs->HW_ACC_ERR_STATUS.R);
        HW_WRITE(pSptRegs->HW_ACC_ERR_STATUS.R, HW_ACC_ERR_STATUS_W1C_MASK);
        status = RSDK_SPT_RET_ERR_HW_ACC;
    }
    else if (HW_READ(pSptRegs->CS_STATUS1.R) != 0u)
    {
        *errInfo = HW_READ(pSptRegs->CS_STATUS1.R);
        HW_WRITE(pSptRegs->CS_STATUS1.R, CS_STATUS1_W1C_MASK);
        status = RSDK_SPT_RET_ERR_ILLOP;
    }
    else
    {
        //"default" case, nothing to do
        status = RSDK_SUCCESS;
    }

    //split in blocks of 3 checks, to avoid having code nesting level > 4
    if (status == RSDK_SUCCESS)
    {
        if (HW_READ(pSptRegs->HIST_OVF_STATUS0.R) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->HIST_OVF_STATUS0.R);
            HW_WRITE(pSptRegs->HIST_OVF_STATUS0.R, HIST_OVF_W1C_MASK);
            status = RSDK_SPT_RET_ERR_HIST_OVF0;
        }
        else if (HW_READ(pSptRegs->HIST_OVF_STATUS1.R) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->HIST_OVF_STATUS1.R);
            HW_WRITE(pSptRegs->HIST_OVF_STATUS1.R, HIST_OVF_W1C_MASK);
            status = RSDK_SPT_RET_ERR_HIST_OVF1;
        }
#if defined(S32R294)
        else if (HW_READ(pSptRegs->DMA_ERR_STATUS.R) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->DMA_ERR_STATUS.R);
            HW_WRITE(pSptRegs->DMA_ERR_STATUS.R, DMA_ERR_STATUS_W1C_MASK);
            status = RSDK_SPT_RET_ERR_DMA;
        }
#elif defined(S32R45)
        else if (HW_READ(pSptRegs->HW2_ACC_ERR_STATUS.R) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->HW2_ACC_ERR_STATUS.R);
            HW_WRITE(pSptRegs->HW2_ACC_ERR_STATUS.R, HW2_ACC_ERR_STATUS_W1C_MASK);
            status = RSDK_SPT_RET_ERR_HW2_ACC;
        }
#endif
        else
        {
            //"default" case, nothing to do
            status = RSDK_SUCCESS;
        }
    }

#if defined(S32R45) || defined(S32R41) || defined(SAF85XX)
    //split in blocks of 3 checks, to avoid having code nesting level > 4
    if (status == RSDK_SUCCESS)
    {
        if ((HW_READ(pSptRegs->GBL_STATUS.R) & GBL_STATUS_ERR_W1C_MASK) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->GBL_STATUS.R) & GBL_STATUS_ERR_W1C_MASK;
            HW_WRITE(pSptRegs->GBL_STATUS.R, GBL_STATUS_ERR_W1C_MASK);
            status = RSDK_SPT_RET_ERR_DMA;
        }
        else if ((HW_READ(pSptRegs->WR_ACCESS_ERR_REG.R) & WR_ACCESS_ERR_REG_W1C_MASK) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->WR_ACCESS_ERR_REG.R) & WR_ACCESS_ERR_REG_W1C_MASK;
            HW_WRITE(pSptRegs->WR_ACCESS_ERR_REG.R, WR_ACCESS_ERR_REG_W1C_MASK);
            status = RSDK_SPT_RET_ERR_WR;
        }
        else
        {
            //"default" case, nothing to do
            status = RSDK_SUCCESS;
        }
    }
#endif

#if defined(S32R45) || defined(S32R41)
    //split in blocks of 3 checks, to avoid having code nesting level > 4
    if (status == RSDK_SUCCESS)
    {
        if (HW_READ(pSptRegs->SCS0_STATUS1.R) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->SCS0_STATUS1.R);
            HW_WRITE(pSptRegs->SCS0_STATUS1.R, SCS_STATUS1_W1C_MASK);
            status = RSDK_SPT_RET_ERR_ILLOP_SCS0;
        }
        else if (HW_READ(pSptRegs->SCS1_STATUS1.R) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->SCS1_STATUS1.R);
            HW_WRITE(pSptRegs->SCS1_STATUS1.R, SCS_STATUS1_W1C_MASK);
            status = RSDK_SPT_RET_ERR_ILLOP_SCS1;
        }
        else
        {
            //"default" case, nothing to do
            status = RSDK_SUCCESS;
        }
    }
#endif

#if defined(S32R45)
    //split in blocks of 3 checks, to avoid having code nesting level > 4
    if (status == RSDK_SUCCESS)
    {
        if (HW_READ(pSptRegs->SCS2_STATUS1.R) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->SCS2_STATUS1.R);
            HW_WRITE(pSptRegs->SCS2_STATUS1.R, SCS_STATUS1_W1C_MASK);
            status = RSDK_SPT_RET_ERR_ILLOP_SCS2;
        }
        else if (HW_READ(pSptRegs->SCS3_STATUS1.R) != 0u)
        {
            *errInfo = HW_READ(pSptRegs->SCS3_STATUS1.R);
            HW_WRITE(pSptRegs->SCS3_STATUS1.R, SCS_STATUS1_W1C_MASK);
            status = RSDK_SPT_RET_ERR_ILLOP_SCS3;
        }
        else
        {
            //"default" case, nothing to do
        }
    }
#endif

    return status;
}

#ifdef __cplusplus
}
#endif
