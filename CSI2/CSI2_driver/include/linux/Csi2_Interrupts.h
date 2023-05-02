/*
* Copyright 2019-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RSDK_CSI2_INTERRUPTS_H
#define RSDK_CSI2_INTERRUPTS_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include "oal_comm_kernel.h"
#include "oal_waitqueue.h"
#include "CDD_Csi2.h"
#include "Csi2_Defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
// Linux interrupts handler definitions
extern irqreturn_t RsdkCsi2PhyIrqHandlerLinux(int32_t iIrq, void *pParams);
extern irqreturn_t RsdkCsi2RxIrqHandlerLinux(int32_t iIrq, void *pParams);
extern irqreturn_t RsdkCsi2EventsIrqHandlerLinux(int32_t iIrq, void *pParams);



#ifdef __cplusplus
}
#endif

#endif  //RSDK_CSI2_INTERRUPTS_H
