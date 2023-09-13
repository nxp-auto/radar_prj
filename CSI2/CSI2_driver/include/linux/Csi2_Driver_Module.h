/*
* Copyright 2019-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RSDK_CSI2_DRIVER_MODULE_H
#define RSDK_CSI2_DRIVER_MODULE_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#ifdef __KERNEL__
    #include <linux/cdev.h>
    #include <linux/interrupt.h>
#endif
#include "Csi2_Linux_Def.h"
#include "rsdk_csi2_driver_api.h"
#include "os_oal_waitqueue.h"
#include "oal_comm_kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef struct
{
    int32_t devId;

    uint32_t *pMemMapBaseAddr;                  // base physical address of the CSI2 registry
    uint32_t  memSize;

    uint32_t irqId[4];
} rsdkCsi2DtsInfo_t;

// structure for unit driver management in kernel space
typedef struct
{
    struct device       *pDev;
    struct cdev         cdevRef;
    dev_t               devNum;
    uint32_t            gUserPid;

    rsdkCsi2DtsInfo_t   dtsInfo;
    volatile uint32_t   *pMemMapVirtAddr;         // base virtual address of the CSI2 registry
    volatile uint32_t   *pMemMapVirtDfs;          // base virtual address of the DFS registry

    OAL_RPCService_t    gspRpcServW, gspRpcServE;  // RPC servers for work & errors
    // waitqueues, pipe & pipe indexes for reporting errors on CSI2
    OAL_waitqueue_t     irqWaitQ;
    volatile uint8_t    irqReadP, irqWriteP;
    rsdkCsi2KernelErr_t irqErrorPipe[RSDK_CSI2_ERROR_QUEUE_LEN];

} rsdkCsi2Device_t;


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
extern rsdkCsi2Device_t *gpRsdkCsi2Device[RSDK_MIPICSI2_INTERFACES];

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
int32_t RsdkCsi2RpcSrvInit(uint32_t unitId);
int32_t RsdkCsi2RpcSrvExit(uint32_t unitId);

#ifdef __cplusplus
}
#endif

#endif  //RSDK_CSI2_DRIVER_MODULE_H
