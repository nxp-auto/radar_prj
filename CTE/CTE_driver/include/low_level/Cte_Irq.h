/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CTE_IRQ_H
#define CTE_IRQ_H

/**
*   @file
*   @implements Cte_Irq.h_Artifact
*
*   @addtogroup CTE_ASR
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*
* @page misra_violations MISRA-C:2012 violations
*
* @section Cte_Irq_h_REF_1
* Violates MISRA 2012 Advisory Rule 20.1, #Include directives should only be preceded by preprocessor directives or comments.
* <MA>_MemMap.h is included after each section define in order to set the current memory section as defined by AUTOSAR.
*/

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Cte_Cfg.h"
#include "Cte_Types.h"

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
  #if defined(linux)
    extern void Cte_IrqHandler(void);
  #endif


/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief   Procedure to define the necessary clock periods.
 * @details The hardware can use up to 4 clock dividers, so up to 4 periods available.
 *          If more clocks required, intermediate periods will be used, to reduce the deviation below 40%.
 *          If it is not possible to have up to 4 clocks and all the required periods to enter 40% deviation,
 *          an error will be returned.
 *
 * @param[in]   pointers to the initialization params
 * @return      E_OK/RSDK_SUCCESS = success; other = error
 *
 */
Std_ReturnType Cte_IrqInit(const Cte_SetupParamsType *pCteInitParams);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CTE_IRQ_H */
