/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SPT_HW_DEFS_H
#define SPT_HW_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#define SPT_CS_STATUS0_STOP_BIT         (0x8u)
#define SPT_CS_STATUS1_W1C_MASK         (0x00000FFFu)
#define SPT_CS_STATUS0_W1C_MASK         (0x8001FFFFu)
#define SPT_HIST_OVF_W1C_MASK           (0xFFFFFFFFu)

/*--------------------------------------------------*/
/*--------------------------------------------------*/

#define SPT_MEM_ERR_STATUS_W1C_MASK     (0xF0FFCF06u)
#define SPT_HW_ACC_ERR_STATUS_W1C_MASK  (0xFFF9FFEFu)
#define SPT_HW2_ACC_ERR_STATUS_W1C_MASK (0xFF88E000u)
#define SPT_GBL_STATUS_W1C_MASK         (0x00031F1Fu)
#define SPT_GBL_STATUS_ERR_W1C_MASK     (0x00031F00u)  /* masks only the error flags in GBL_STATUS */

#define SPT_SCS_STATUS0_W1C_MASK        (0x0001FFFFu)
#define SPT_SCS_STATUS1_W1C_MASK        (0x00000FFFu)

#define SPT_WR_ACCESS_ERR_REG_W1C_MASK  (0x07071F1Fu)
#define SPT_CS_EVTREG1_MASK             (0xFFFFFFFFu)  /* 32 event lines from SPT to CPU */
#define SPT_DSP_ERR_INFO_REG_MASK       (0xFFFFFFFFu)

/*--------------------------------------------------*/
/*--------------------------------------------------*/

/* interrupt IDs in the GIC500: */
#define SPT_INTC_IRQ_OFFSET_SPT_DSP_ERR (220)
#define SPT_INTC_IRQ_OFFSET_SPT_EVT1    (221)
#define SPT_INTC_IRQ_OFFSET_SPT_ECS     (222)
#define SPT_INTC_IRQ_OFFSET_SPT_DMA     (223)

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* SPT_HW_DEFS_H */
