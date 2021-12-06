/*
* Copyright 2019-2021 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SPT_HW_DEFS_H
#define SPT_HW_DEFS_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define CS_STATUS0_STOP_BIT (0x8u)
#define CS_STATUS1_W1C_MASK (0x00000FFFu)
#define CS_STATUS0_W1C_MASK (0x8001FFFFu)
#define HIST_OVF_W1C_MASK (0xFFFFFFFFu)

//--------------------------------------------------


















#if   defined(S32R45)
//--------------------------------------------------

#define MEM_ERR_STATUS_W1C_MASK (0xF0FFCF06u)
#define HW_ACC_ERR_STATUS_W1C_MASK (0xFFF9FFEFu)
#define HW2_ACC_ERR_STATUS_W1C_MASK (0xFF88E000u)
#define GBL_STATUS_W1C_MASK (0x00031F1Fu)
#define GBL_STATUS_ERR_W1C_MASK (0x00031F00u)  //masks only the error flags in GBL_STATUS

#define SCS_STATUS0_W1C_MASK (0x0001FFFFu)
#define SCS_STATUS1_W1C_MASK (0x00000FFFu)

#define WR_ACCESS_ERR_REG_W1C_MASK (0x07071F1Fu)
#define CS_EVTREG1_MASK (0xFFFFFFFFu)  //32 event lines from SPT to CPU
#define DSP_ERR_INFO_REG_MASK (0xFFFFFFFFu)

//interrupt IDs in the GIC500:
#define INTC_IRQ_OFFSET_SPT_DSP_ERR (220)
#define INTC_IRQ_OFFSET_SPT_EVT1 (221)
#define INTC_IRQ_OFFSET_SPT_ECS (222)
#define INTC_IRQ_OFFSET_SPT_DMA (223)

//--------------------------------------------------








































#endif
//--------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif  // SPT_HW_DEFS_H
