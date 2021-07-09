/*
* Copyright 2020-2021 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RSDK_CTE_DRIVER_MODULE_H
#define RSDK_CTE_DRIVER_MODULE_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include "oal_comm_kernel.h"
#include "oal_waitqueue.h"
#include "rsdk_cte_linux_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define CTE_REG_DBG_OFFSET_U32	(0x278u >> 2u)		// the offset of the debug registry of CTE, as uint32_t array

// definition for debug messages at Linux module level
#ifndef DEBUG_VERSION
    #define DebugMessage(fmt, ...)
#else
    #define DebugMessage(fmt, ...) printk(fmt, ##__VA_ARGS__)
#endif


/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

typedef struct
{
    int32_t devId;

    uint32_t *pMemMapBaseAddr; /* base physical address of the CTE registry */
    uint32_t  memSize;

    uint32_t irqId;
} rsdkCteDtsInfo_t;

typedef struct
{
    struct device *dev;
    struct cdev    cdevRef;
    dev_t          devNum;
    uint32_t       gUserPid;

    rsdkCteDtsInfo_t  dtsInfo;
    volatile uint32_t *pMemMapVirtAddr; 	// base virtual address of the CTE registry
	volatile uint32_t *pSrc_1;				// pointer to the SRC_1 registry, used only for Linux

    OAL_RPCService_t gspRpcServ;
    OAL_waitqueue_t  irqWaitQ;
    uint8_t          registeredEvents;

} rsdkCteDevice_t;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
extern rsdkCteDevice_t *gpRsdkCteDevice;

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
int32_t RsdkCteRpcSrvInit(void);
int32_t RsdkCteRpcSrvExit(void);
int32_t CtePlatformUnregisterEvents(void);

#ifdef __cplusplus
}
#endif

#endif  //RSDK_CTE_DRIVER_MODULE_H
