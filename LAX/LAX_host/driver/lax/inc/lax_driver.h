/*
 * Copyright 2018-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LAX_DRIVER_H
#define LAX_DRIVER_H

/** @addtogroup <lax driver>
* @{
*/

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/
#include <oal_utils.h>
#include <oal_spinlock.h>
#include <oal_memmap.h>
#include <oal_timer.h>
#include <oal_irq_utils.h>

#include "rsdk_S32R45.h"
#include "rsdk_lax_common.h"
#include "rsdk_status.h"
#include "lax_uapi.h"

#ifdef __cplusplus
extern "C" {
#endif


/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/
#define IF_LAX_DRV_DEBUG(x) if ((pLaxCtrl->debug & (x)) != (uint32_t)0) \

#define LAX_DRV_ERR(...) if ((pLaxCtrl->debug & (uint32_t)(DEBUG_MESSAGES)) != (uint32_t)0)   \
                         {                                              \
                             pr_err("LAX agnostic"__VA_ARGS__);         \
                         }                                              \


/* Debug bit assignments */
#define DMA_FLAG_XFRERR                 (1U<<1U)
#define DMA_FLAG_CFGERR                 (1U<<2U)



#define DEBUG_MESSAGES                  (0x1UL<<0U)
#define DEBUG_DMA                       (0x1UL<<1U)
#define DEBUG_DMA_IRQ                   (0x1UL<<2U)
#define DEBUG_FLAGS0_IRQ                (0x1UL<<3U)
#define DEBUG_FLAGS1_IRQ                (0x1UL<<4U)
#define DEBUG_SEQID                     (0x1UL<<5U)
#define DEBUG_INIT                      (0x1UL<<6U)

#define GP_OUT_REG_PARITY_ENABLE        (0x00000001U)
#define GP_OUT_REG_PARITY_DISABLE       (0xFFFFFFF0U)
#define GP_OUT_REG_PARITY_FAIL_CLEAR    (0x80000000U)
#define GP_IN_PARRITY_ERROR_MASK        (0x0000001EU)


#define STATUS_REG_IRQ_ILLEGALOP        (0x00000080U)
#define STATUS_REG_IRQ_DMA_ERR          (0x00000020U)
#define STATUS_REG_IRQ_DMA_COMP         (0x00000010U)
#define STATUS_REG_IRQ_FLAGS1           (0x00000008U)
#define STATUS_REG_IRQ_FLAGS0           (0x00000004U)
#define STATUS_REG_IRQ_NON_GEN          (STATUS_REG_IRQ_FLAGS1 | STATUS_REG_IRQ_FLAGS0 | STATUS_REG_IRQ_DMA_COMP |   \
                                            STATUS_REG_IRQ_DMA_ERR | STATUS_REG_IRQ_ILLEGALOP)

#define DMA_QUEUE_ENTRIES               (16U)
#define MAX_SEQIDS                      (RSDK_LAX_MAX_CMDS_NUM)
#define MBOX_QUEUE_ENTRIES              (16U)

#ifdef LAX_OS_sa

#define IRQ_HANDLED						1u				// return for normal handled interrupt request
#endif


/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/

/*=================================================================================================
*                                             ENUMS
=================================================================================================*/

/*=================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
=================================================================================================*/
#ifdef LAX_OS_sa
    typedef int32_t    irqreturn_t;     // sa equivalent for irq return value (Linux defined)
#endif


/**
* @brief          DMA queue structure.
* @details        DMA queue structure.
*/
typedef struct {
    struct laxDmaReq entry[DMA_QUEUE_ENTRIES];
    uint8_t chan;
    uint8_t idxQueue;   /* Index for next item to be enqueued */
    uint8_t idxDma;     /* Index for next item to be sent on DMA */
    uint8_t idxChk;     /* Index for current item to complete */
} dmaQueue_t;


/**
* @brief          LAX control structure.
* @details        LAX control structure.
*/
typedef struct {
    uint32_t    debug;
    int32_t     id;

    /* DMA queue */
    dmaQueue_t  dmaQueue;

    OAL_spinlock_t  dmaEnqueueLock;
    OAL_irqspinlock_t   dmaTxQueueLock; /* called from irq handler */

    /* DMA channel usage */
    uint8_t     cmdDmaChan;

    /* IP registers */
    uint32_t    *pMemAddr;          /* physical address */
    uintptr_t   pRegs;             /* virtual address */


    struct laxVersions versions;
    struct laxHardware hardware;

    /* Debug registers */
    uint32_t    dbgSize;            /* size */
    uint32_t    *pDbgAddr;          /* physical address */
    uint32_t    *pDbgRegs;          /* virtual address */

    /* IP registers */
    uint32_t    memSize;            /* size */

    /* flags */
    uint32_t    flags;
}lldLaxControl_t;

/*=================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
=================================================================================================*/
extern lldLaxControl_t *gOalCommLaxCtrl[RSDK_LAX_CORES_NUM];

/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/
OAL_IRQ_HANDLER(LaxIrqHandler);

rsdkStatus_t LaxLowLevelDriverInit(lldLaxControl_t *pLaxCtrl);
rsdkStatus_t LaxOalCommInit(void);
rsdkStatus_t LaxOalCommExit(void);
rsdkStatus_t LaxDeInit(lldLaxControl_t *pLaxCtrl);

#ifdef LAX_OS_sa
/**
* @brief        Initiate event deregistation in low-level driver
* @param[in]    Deregister events up to lastEvt
* @return       The execution result : RSDK_SUCCESS or error: RSDK_LAX_ERR_OAL_EVENT_DEREGISTER
*/
rsdkStatus_t LaxDeregisterEvents(rsdkLaxEventType_t lastEvt);
#endif


#ifdef __cplusplus
}
#endif

/** @} */ /*doxygen module*/

#endif /* LAX_DRIVER_H */

