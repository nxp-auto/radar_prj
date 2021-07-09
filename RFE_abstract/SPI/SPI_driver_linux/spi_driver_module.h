/*
* Copyright 2019-2020 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SPI_DRIVER_MODULE_H
#define SPI_DRIVER_MODULE_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/atomic.h>
#include "rsdk_S32R45.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define PR_ERR(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#ifdef PRINTK_ENABLE
#define PR_ALERT(fmt, ...) pr_alert(fmt, ##__VA_ARGS__)
#else
#define PR_ALERT(fmt, ...) 
#endif

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef struct
{
    uint32_t devId;

    uint32_t *pMemMapBaseAddr;
    uint64_t  memSize;

} spiDtsInfo_t;

typedef struct
{
    struct device *dev;
    struct cdev    cdevice;
    dev_t          deviceNum;
    int32_t        gUserPid;

    spiDtsInfo_t       dtsInfo;
    volatile struct SPI_tag *pSpiRegs;

} spiDevice_t;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
extern spiDevice_t spiDevice;

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif  //SPI_DRIVER_MODULE_H
