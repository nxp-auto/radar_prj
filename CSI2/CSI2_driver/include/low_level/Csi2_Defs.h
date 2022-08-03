/*
* Copyright 2022 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef CSI2_DEFS_H
#define CSI2_DEFS_H

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#include "rsdk_status.h"
#if defined(TRACE_ENABLE)
    #include "csi2_driver_platform_trace.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                          CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      DEFINES AND MACROS
 ==================================================================================================*/
/* IO definitions (access restrictions to peripheral registers) */
/**
*   IO Type Qualifiers are used
*   \li to specify the access to peripheral variables.
*   \li for automatic generation of peripheral register debug information.
*/
#ifndef __IO
#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions                 */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions                */
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */
#endif

#define E_OK        RSDK_SUCCESS
#define E_NOT_OK    RSDK_ERROR
#define NULL_PTR    0U

// interrupt definitions
#define CSI2_IRQ_RX_BASE_ID_GIC         204         // irq number for CSI2 Rx error interrupt, unit 0
#define CSI2_IRQ_PATH_BASE_ID_GIC       205         // irq number for CSI2 Path error interrupt, unit 0
#define CSI2_IRQ_EVENT_BASE_ID_GIC      206         // irq number for CSI2 events interrupt, unit 0
#define CSI2_IRQ_MAP_GAP                4           // gap between the similar irq numbers

// very specific CSI2 definition for S32R294


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
 *                                    FUNCTION PROTOTYPES
 ==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /*CSI2_DEFS_H*/
