/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RSDK_LAX_DRIVER_H
#define RSDK_LAX_DRIVER_H

/** @addtogroup <lax driver>
* @{
*/

/*=================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
=================================================================================================*/
#include <oal_utils.h>
#include <oal_spinlock.h>
#include <oal_memmap.h>
#include <oal_timer.h>
#include <oal_irq_utils.h>
#include <oal_completion.h>
/* TODO find a proper way to include types */

#include "event.h"
#include "cbuffer.h"

#ifdef __cplusplus
/*do this after the #includes*/
extern "C" {
#endif


/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/
/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/

#define IF_LAX_DRV_DEBUG(x) if (pVspaCtrl->debug & (x))

#define LAX_DRV_ERR(...) { if (pVspaCtrl->debug & DEBUG_MESSAGES) \
                pr_err("LAX agnostic"__VA_ARGS__); }


/* Debug bit assignments */
#define DEBUG_MESSAGES      (1<<0)
#define DEBUG_STARTUP       (1<<1)
#define DEBUG_CMD           (1<<2)
#define DEBUG_REPLY         (1<<3)
#define DEBUG_SPM           (1<<4)
#define DEBUG_DMA           (1<<5)
#define DEBUG_EVENT         (1<<6)
#define DEBUG_WATCHDOG      (1<<7)
#define DEBUG_MBOX64_OUT    (1<<8)
#define DEBUG_MBOX32_OUT    (1<<9)
#define DEBUG_MBOX64_IN     (1<<10)
#define DEBUG_MBOX32_IN     (1<<11)
#define DEBUG_DMA_IRQ       (1<<12)
#define DEBUG_FLAGS0_IRQ    (1<<13)
#define DEBUG_FLAGS1_IRQ    (1<<14)
#define DEBUG_IOCTL         (1<<15)
#define DEBUG_SEQID         (1<<16)
#define DEBUG_CMD_BD        (1<<17)
#define DEBUG_REPLY_BD      (1<<18)
#define DEBUG_TEST_SPM      (1<<24)


#define DMA_FLAG_COMPLETE       (1<<0)
#define DMA_FLAG_XFRERR         (1<<1)
#define DMA_FLAG_CFGERR         (1<<2)

#define MBOX_STATUS_IN_1_64_BIT         (0x00000008)
#define MBOX_STATUS_IN_64_BIT           (0x00000004)
#define MBOX_STATUS_OUT_1_64_BIT        (0x00000002)
#define MBOX_STATUS_OUT_64_BIT          (0x00000001)

/* IP register offset for the registers used by the driver */
#define HWVERSION_REG_OFFSET        (0x0000>>2)
#define SWVERSION_REG_OFFSET        (0x0004>>2)
#define CONTROL_REG_OFFSET          (0x0008>>2)
#define IRQEN_REG_OFFSET            (0x000C>>2)
#define STATUS_REG_OFFSET           (0x0010>>2)
#define HOST_FLAGS0_REG_OFFSET      (0x0014>>2)
#define HOST_FLAGS1_REG_OFFSET      (0x0018>>2)
#define VCPU_FLAGS0_REG_OFFSET      (0x001C>>2)
#define VCPU_FLAGS1_REG_OFFSET      (0x0020>>2)
#define EXT_GO_ENABLE_REG_OFFSET    (0x0028>>2)
#define EXT_GO_STATUS_REG_OFFSET    (0x002C>>2)
#define PARAM0_REG_OFFSET           (0x0040>>2)
#define PARAM1_REG_OFFSET           (0x0044>>2)
#define PARAM2_REG_OFFSET           (0x0048>>2)
#define DMA_DMEM_ADDR_REG_OFFSET    (0x00B0>>2)
#define DMA_AXI_ADDR_REG_OFFSET     (0x00B4>>2)
#define DMA_BYTE_CNT_REG_OFFSET     (0x00B8>>2)
#define DMA_XFR_CTRL_REG_OFFSET     (0x00BC>>2)
#define DMA_STAT_ABORT_REG_OFFSET   (0x00C0>>2)
#define DMA_IRQ_STAT_REG_OFFSET     (0x00C4>>2)
#define DMA_COMP_STAT_REG_OFFSET    (0x00C8>>2)
#define DMA_XFRERR_STAT_REG_OFFSET  (0x00CC>>2)
#define DMA_CFGERR_STAT_REG_OFFSET  (0x00D0>>2)
#define DMA_XRUN_STAT_REG_OFFSET    (0x00D4>>2)
#define DMA_GO_STAT_REG_OFFSET      (0x00D8>>2)
#define DMA_FIFO_STAT_REG_OFFSET    (0x00DC>>2)

#define GP_OUT1_REG_OFFSET          (0x0580>>2)
#define GP_OUT2_REG_OFFSET          (0x0584>>2)
#define GP_OUT3_REG_OFFSET          (0x0588>>2)

#define HOST_OUT_64_MSB_REG_OFFSET      (0x0680>>2)
#define HOST_OUT_64_LSB_REG_OFFSET      (0x0684>>2)
#define HOST_OUT_1_64_MSB_REG_OFFSET    (0x0688>>2)
#define HOST_OUT_1_64_LSB_REG_OFFSET    (0x068C>>2)
#define HOST_IN_64_MSB_REG_OFFSET       (0x0690>>2)
#define HOST_IN_64_LSB_REG_OFFSET       (0x0694>>2)
#define HOST_IN_1_64_MSB_REG_OFFSET     (0x0698>>2)
#define HOST_IN_1_64_LSB_REG_OFFSET     (0x069C>>2)
#define HOST_MBOX_STATUS_REG_OFFSET     (0x06A0>>2)

#define IPPU_CONTROL_REG_OFFSET         (0x0700>>2)
#define IPPU_STATUS_REG_OFFSET          (0x0704>>2)
#define IPPU_ARG_BASEADDR_REG_OFFSET    (0x070C>>2)
#define IPPU_HWVERSION_REG_OFFSET       (0x0710>>2)
#define IPPU_SWVERSION_REG_OFFSET       (0x0714>>2)

#define DBG_GDBEN_REG_OFFSET            (0x800>>2)
#define DBG_RCR_REG_OFFSET              (0x804>>2)
#define DBG_RCSTATUS_REG_OFFSET         (0x808>>2)
#define DBG_RAVAP_REG_OFFSET            (0x870>>2)
#define DBG_DVR_REG_OFFSET              (0x87C>>2)


#define STATUS_REG_PDN_ACTIVE           (0x80000000)
#define STATUS_REG_PDN_DONE             (0x40000000)
#define STATUS_REG_IRQ_VCPU_READ_MSG    (0x0000C000)
#define STATUS_REG_IRQ_VCPU_SENT_MSG    (0x00003000)
#define STATUS_REG_IRQ_VCPU_MSG         (STATUS_REG_IRQ_VCPU_READ_MSG | \
                    STATUS_REG_IRQ_VCPU_SENT_MSG)
#define STATUS_REG_BUSY                 (0x00000100)
#define STATUS_REG_IRQ_DMA_ERR          (0x00000020)
#define STATUS_REG_IRQ_DMA_COMP         (0x00000010)
#define STATUS_REG_IRQ_FLAGS1           (0x00000008)
#define STATUS_REG_IRQ_FLAGS0           (0x00000004)
#define STATUS_REG_IRQ_IPPU_DONE        (0x00000002)
#define STATUS_REG_IRQ_DONE             (0x00000001)
#define STATUS_REG_IRQ_NON_GEN          (STATUS_REG_IRQ_FLAGS1 |         \
            STATUS_REG_IRQ_FLAGS0 | STATUS_REG_IRQ_DMA_COMP |   \
            STATUS_REG_IRQ_DMA_ERR | STATUS_REG_IRQ_VCPU_SENT_MSG)

#define DMA_QUEUE_ENTRIES       (16)
#define MAX_SEQIDS              (RSDK_LAX_MAX_CMDS_NUM)
#define MBOX_QUEUE_ENTRIES      (16)


/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/
/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/



/*=================================================================================================
*                                             ENUMS
=================================================================================================*/
/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
* @pre            Preconditions as text description. Optional tag.
* @post           Postconditions as text description. Optional tag.
*
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/

/*=================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
=================================================================================================*/
/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/

typedef long unsigned int (copyFunc_t)(void *to, const void *from, long unsigned int n);

typedef struct 
{
    uint32_t    flags;
    int         cmdId;
    int         cmdBufferIdx;
    int         cmdBufferSize;
    int         cmdBdIndex;
    int         cmdRdntReq;  /* redundancy execution request if non-zero */
    uint32_t    payload1;

} seqId_t;


typedef struct 
{
    volatile int    idxEnqueue;
    volatile int    idxDequeue;
    volatile int    idxComplete;
} mboxQueue_t;

typedef struct {
    struct vspa_dma_req entry[DMA_QUEUE_ENTRIES];
    int     chan;
    int     idxQueue;   /* Index for next item to enqueue */
    int     idxDma; /* Index for next item to DMA */
    int     idxChk; /* Index for next item to check */
} dmaQueue_t;

typedef struct {

    /* Sequence IDs */
    uint32_t    activeSeqIds;
    uint32_t    lastSeqId;
    seqId_t     seqId[MAX_SEQIDS];

    uint64_t shbufstart;
    uint32_t shbuflen;

    /* reply buffers for host, CTE and SPT commands */
    uint32_t *replyVaddr[RSDK_LAX_MAX_CMDS_NUM + RSDK_LAX_CTE_CMD_MAX_NUM + RSDK_LAX_SPT_CMD_MAX_NUM]; 
    uint64_t replyPaddr[RSDK_LAX_MAX_CMDS_NUM + RSDK_LAX_CTE_CMD_MAX_NUM + RSDK_LAX_SPT_CMD_MAX_NUM];

    /* command buffers */
    uint32_t *cmdVirtAddr[RSDK_LAX_MAX_CMDS_NUM];
    uint64_t cmdPhyAddr[RSDK_LAX_MAX_CMDS_NUM];

    /* shared buffer for wrap-around case */
    uint32_t *wrpVirtAddr;
    uint64_t wrpPhyAddr;

    int cteRdnt[RSDK_LAX_CTE_CMD_MAX_NUM]; //redundancy request for CTE-triggered commands
    int sptRdnt[RSDK_LAX_SPT_CMD_MAX_NUM]; //redundancy request for SPT-triggered commands

    /* TODO sync these variables with the kernel structure somehow */
    /* Current state of the device */
    enum vspa_state state;
    uint32_t    debug;
    int id;

    /* Memory pools */
    circularBuffer_t cmd_buffer;
    uint32_t    firstCmd;

    mboxQueue_t mb64Queue;

    /* DMA queue */
    dmaQueue_t  dmaQueue;
    OAL_spinlock_t  dmaEnqueueLock;
    OAL_spinlock_t  dmaTxQueueLock; /* called from irq handler */

    /* DMA channel usage */
    uint8_t     cmdDmaChan;

    /* IP registers */
    uint32_t *pMemAddr;    /* physical address */
    uint32_t *pRegs; /* virtual address */


    /* IRQ handling */
    /* TODO spinlock_t irq_lock; */
    uint32_t    irqBits; /* TODO - test code */
    OAL_spinlock_t  controlLock;


    /* Watchdog */
    /*TODO replace this with static allocation */
    OAL_Timer_t watchdogTimer;
    uint32_t    watchdogIntervalMsecs;
    uint32_t    watchdogValue;
    OAL_Completion_t watchdogComplete;




    struct vspa_versions versions;
    struct vspa_hardware hardware;

    uint32_t    cmdBufAddr;

    struct vspa_mb64 mb64[MBOX_QUEUE_ENTRIES];
    OAL_spinlock_t  mb64Lock; /* called from irq handler */
    OAL_spinlock_t  mbChkLock; /* called from irq handler */
    OAL_Completion_t mbChkComplete;


    uint8_t     legacyCmdDmaChan;

    /* Debug registers */
    uint32_t dbgSize; /* size */
    uint32_t *pDbgAddr;    /* physical address */
    uint32_t *pDbgRegs;    /* virtual address */

    /* Buffer sizes */
    uint32_t    cmdBufferBytes;

    /* IP registers */
    uint32_t memSize; /* size */

    vspaEventList_t    vspaList;

    /* flags */
    unsigned int flags;
} vspaControl_t;

/*=================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
=================================================================================================*/
extern vspaControl_t *gOalCommVspaCtrl;

/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/

/**
* @brief          Interrupt Service Routine for VSPA.
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
*
* @param[in]      irq         Interrupt number
* @param[in]      pDev        .
*                             Must be omitted if the function does not have parameters.
*
* @return         Status of the interrupt handling
*
* @note           Function needs to be modified to have a more generic signature rather than the
*                 linux type.
*
*/

irqreturn_t VspaIrqHandler(int irq, vspaControl_t *pVspaCtrl);

void VspaDisableAllIRQs(vspaControl_t *pVspaCtrl);


void DmaWait(vspaControl_t *pVspaCtrl, uint32_t chan);
int32_t VSPAPowerDown(vspaControl_t *pVspaDev);
int32_t VspaCmdSend(vspaControl_t *pVspaDev, char* pcmd, uint32_t len);
int32_t VspaPowerRequest(vspaControl_t *pVspaDev, uint32_t arg);
int32_t VspaDmaRequest(vspaControl_t *pVspaDev, struct vspa_dma_req * pDmaReq);
int32_t VspaStartUp(vspaControl_t *pVspaDev, struct vspa_startup * pStartupDesc);
void VspaInit(vspaControl_t *pVspaDev);
void VspaDeInit(vspaControl_t *pVspaDev);
void VspaShutdownTimer(vspaControl_t *pVspaDev);
int FullState(vspaControl_t *pVspaDev);
void VspaSetRecovery(vspaControl_t *pVspaCtrl);
int VspaOalCommInit(void);
int VspaOalCommExit(void);


/*
 * Accessor Functions
 */

static inline void VspaRegWrite(void *addr, uint32_t val)
{
    return OAL_write32(addr, val);
}

static inline unsigned int VspaRegRead(void *addr)
{
    return OAL_read32(addr);
}

#ifdef __cplusplus
}
#endif

/** @} */ /*doxygen module*/

#endif /*RSDK_LAX_DRIVER_H*/

