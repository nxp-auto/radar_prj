/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CSI2_IRQ_H
#define CSI2_IRQ_H

/**
*   @file
*   @implements Csi2_Irq.h_Artifact
*
*   @addtogroup CSI2_ASR
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*
* @page misra_violations MISRA-C:2012 violations
*
* @section Csi2_Irq_h_REF_1
* Violates MISRA 2012 Advisory Rule 20.1, #Include directives should only be preceded by preprocessor directives or comments.
* <MA>_MemMap.h is included after each section define in order to set the current memory section as defined by AUTOSAR.
*/

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Csi2_Cfg.h"
#include "Csi2_Types.h"

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

/*
 * @brief       Initialize interrupts at Unit level
 *
 * @param[in]   iUnit   - unit ID, CSI2_UNIT_0 ... MAX
 * @param[in]   pRegs   - pointer to unit registry
 * @param[in]   pParams - pointer to CSI2 unit initialization parameters
 *
 */
rsdkStatus_t
Csi2_SetupUIrq(const Csi2_UnitIdType iUnit, volatile GENERIC_CSI2_Type *pRegs,
        const Csi2_SetupParamsType *pInitParams);

/*
 * @brief       Interrupt handlers prototypes
 *
 * @param[in]   iUnit   - unit ID, CSI2_UNIT_0 ... MAX
 *
 */
void Csi2_IrqHandlerEvents(const Csi2_UnitIdType iUnit);
void Csi2_IrqHandlerPathErr(const Csi2_UnitIdType iUnit);
void Csi2_IrqHandlerRxErr(const Csi2_UnitIdType iUnit);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CSI2_IRQ_H */
