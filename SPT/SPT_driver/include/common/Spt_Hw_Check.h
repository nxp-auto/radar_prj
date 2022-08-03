/*
* Copyright 2022 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SPT_HW_CHECK_H
#define SPT_HW_CHECK_H

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Spt_Internals_Types.h"
#include "Spt_Cfg.h"

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

Std_ReturnType  Spt_CheckAndResetHwError(volatile SPT_Type *const pSptRegs, uint32 *errInfo);
Std_ReturnType  Spt_CheckRst(volatile SPT_Type *pSptRegs);
Std_ReturnType  Spt_CheckUnexpectedStop(volatile SPT_Type *const pSptRegs, Spt_DrvStateType drvState);

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* SPT_HW_CHECK_H */
