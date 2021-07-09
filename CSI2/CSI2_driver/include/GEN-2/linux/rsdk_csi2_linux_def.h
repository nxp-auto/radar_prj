/*
* Copyright 2019-2020 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RSDK_CSI2_LINUX_DEF_H
#define RSDK_CSI2_LINUX_DEF_H

#include "rsdk_csi2_driver_api.h"

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define RSDK_MIPICSI2_INTERFACES    4u
#define RSDK_CSI2_RPC_WORK          "RsdkCsi2RpcW"  // RPC channel name for normal working
#define RSDK_CSI2_RPC_ERROR         "RsdkCsi2RpcE"  // RPC channel name for error messages
#define RSDK_CSI2_ERROR_QUEUE_LEN   4u              // the length of the error queue for each unit
#define RSDK_CSI2_ERROR_TO_EXIT     0xfffffff0u     // fake error mask to signal closing user-space to kernel connection


/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
// calls identifiers for CSI2 work Linux RPC
typedef enum
{
    RSDK_CSI2_LX_CALL_INIT = 0,       // unit initialization
    RSDK_CSI2_LX_RX_STOP,             // unit Rx stop
    RSDK_CSI2_LX_RX_START,            // unit Rx start
    RSDK_CSI2_LX_POWER_OFF,           // unit power off
    RSDK_CSI2_LX_POWER_ON,            // unit power on
    RSDK_CSI2_LX_GET_IFACE,           // get unit/interface status
    RSDK_CSI2_LX_GET_LANE,            // get unit & lane status
    RSDK_CSI2_LX_GET_CHIRP_LEN,       // get normal chirp length
    RSDK_CSI2_LX_GET_FRAMES,          // get number of received frames
    RSDK_CSI2_LX_GET_ERRORS,          // get errors
} rsdkCsi2RpcCalls_t;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
// structure for recording CSI2 errors in kernel
typedef struct
{
    uint32_t            errType;
    rsdkCsi2Report_t    errReport;
} rsdkCsi2KernelErr_t;

// 

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif  //RSDK_CSI2_LINUX_DEF_H
