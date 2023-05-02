/*
 * Copyright 2019-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <linux/types.h>
#include <linux/stddef.h>

#include "Csi2_Driver_Module.h"
#include "Csi2_Interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/
static inline rsdkCsi2UnitId_t GetUnitId(int32_t iUnit)
{
    return (*(rsdkCsi2UnitId_t *)(void *)&iUnit);
}

/******************************************************************************/
/**
 * @brief   CSI2 PHY error interrupt handler
 */
irqreturn_t RsdkCsi2PhyIrqHandlerLinux(int32_t iIrq, void *pParams)
{
    (void)iIrq;
    Csi2_IrqHandlerPathErr((Csi2_UnitIdType)GetUnitId(((rsdkCsi2Device_t *)pParams)->dtsInfo.devId));
    return IRQ_HANDLED;
}

/******************************************************************************/
/**
 * @brief   CSI2 reception error interrupt handler
 */
irqreturn_t RsdkCsi2RxIrqHandlerLinux(int32_t iIrq, void *pParams)
{
    (void)iIrq;
    Csi2_IrqHandlerRxErr((Csi2_UnitIdType)GetUnitId(((rsdkCsi2Device_t *)pParams)->dtsInfo.devId));
    return IRQ_HANDLED;
}

/******************************************************************************/
/**
 * @brief   CSI2 events interrupt handler
 */
irqreturn_t RsdkCsi2EventsIrqHandlerLinux(int32_t iIrq, void *pParams)
{
    (void)iIrq;
    Csi2_IrqHandlerEvents((Csi2_UnitIdType)GetUnitId(((rsdkCsi2Device_t *)pParams)->dtsInfo.devId));
    return IRQ_HANDLED;
}

#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
