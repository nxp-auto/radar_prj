/*
* Copyright 2019 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SPT_HW_ERR_H
#define SPT_HW_ERR_H

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "hw_platform.h"
#include "rsdk_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
rsdkStatus_t SptCheckAndResetHwError(volatile struct SPT_tag *const pSptRegs, uint32_t *errInfo);

#ifdef __cplusplus
}
#endif

#endif  // SPT_HW_ERR_H
