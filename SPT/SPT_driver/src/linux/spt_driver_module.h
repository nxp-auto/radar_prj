/*
* Copyright 2019-2020 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SPT_DRIVER_MODULE_H
#define SPT_DRIVER_MODULE_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/atomic.h>
#include <linux/reset.h>
#include "S32R45_SPT.h"
#include "oal_comm_kernel.h"
#include "oal_waitqueue.h"
#include "Spt_Oal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#ifdef PRINTK_ENABLE
#define PR_ERR(fmt, ...) pr_err(fmt, ##__VA_ARGS__)
#define PR_ALERT(fmt, ...) pr_alert(fmt, ##__VA_ARGS__)
#else
#define PR_ERR(fmt, ...) 
#define PR_ALERT(fmt, ...) 
#endif

#define SPT_REG_TESTVAL (0xDEADBE00u)

#define SPT_DATA_Q_SIZE         (64U)
#define SPT_DATA_Q_CNT_LAST      (4U)

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
typedef enum
{
    SPT_DTS_IRQ_IDX_DSP = 0U,
    SPT_DTS_IRQ_IDX_EVT,
    SPT_DTS_IRQ_IDX_ECS,
    SPT_DTS_IRQ_IDX_DMA,
    SPT_DTS_IRQ_IDX_SIZE
} sptIrqTableIdx_t;

typedef struct
{
    uint32_t devId;
    struct reset_control *bbe32Reset;
    uint32_t *pMemMapBaseAddr; /* physical address of the SPT register block */
    uint64_t  memSize;

    uint32_t irqId[SPT_DTS_IRQ_IDX_SIZE];
} sptDtsInfo_t;

typedef struct
{
    struct device *dev;
    struct cdev    cdevice;
    dev_t          deviceNum;
    int32_t       gUserPid;

    sptDtsInfo_t        dtsInfo;
    volatile SPT_Type * pSptRegs; /* Virtual address of the SPT register block */

    OAL_RPCService_t  gsOalCommServ[2];
    OAL_waitqueue_t   irqWaitQ;
    atomic_t          irqsNotServed; /* Counter used for wait queue condition */

    evtSharedData_t   queueEvtData[SPT_DATA_Q_SIZE]; /* Queue that assures evtData transfer from multiple interrupts */
    uint8_t           queueIdxRd; /* Queue tail */
    atomic_t          queueIdxWr; /* Queue head */

} sptDevice_t;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
extern sptDevice_t sptDevice;

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
extern irqreturn_t SptDevIrqHandler(int irq, void *pDev);
int32_t            SptOalCommInit(void);
int32_t            SptOalCommExit(void);

#ifdef __cplusplus
}
#endif

#endif  //SPT_DRIVER_MODULE_H
