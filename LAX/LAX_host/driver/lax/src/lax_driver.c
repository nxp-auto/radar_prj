/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
* @file           lax_driver.c
* @brief          LAX driver OS agnostic implementation
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <oal_utils.h>
#include <oal_memmap.h>
#include <oal_spinlock.h>
#include <oal_waitqueue.h>
#include <oal_uptime.h>
#include <oal_irq_utils.h>
#include <oal_timespec.h>
#include <oal_comm_kernel.h>

//#include  "vspa_uapi.h"
#include "lax_driver.h"
#include "cbuffer.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

#define VSPA_HALT_TIMEOUT       (100000)
#define VSPA_STARTUP_TIMEOUT    (100000)

#define CONTROL_REG_MASK        (~0x000100FF)
#define CONTROL_PDN_EN          (1<<31)
#define CONTROL_HOST_MSG_GO     (1<<20 | 1<<21 | 1<<22 | 1<<23)
#define CONTROL_VCPU_RESET      (1<<16)
#define CONTROL_DEBUG_MSG_GO    (1<<5)
#define CONTROL_IPPU_GO         (1<<1)
#define CONTROL_HOST_GO         (1<<0)

#define MSB_HANDSHAKE           0x70001000
#define LSB_HANDSHAKE           0xFEC00000

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
static OAL_RPCService_t     gsOalCommServ = NULL;
static volatile uint16_t    gsCmdCounter;            // 16 bits command counter for user space
static uint32_t             gsCmdList[MAX_SEQIDS];   // array for user space IDs

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
vspaControl_t *gOalCommVspaCtrl;
volatile uint8_t     gIsRecovery;            // flag for recovery state


/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

static int SeqIdGetNext(vspaControl_t *pVspaCtrl);
void SeqIdRelease(vspaControl_t *pVspaCtrl, int seqId);
static int32_t DmaEnqueue(vspaControl_t *pVspaCtrl, struct vspa_dma_req *pDmaReq);
static void DmaResetQueue(vspaControl_t *pVspaCtrl);
static int32_t DmaTransmit(vspaControl_t *pVspaCtrl);
static void VspaEnableDmaIrqs(vspaControl_t *pVspaCtrl);
static void VspaEnableMailboxIrqs(vspaControl_t *pVspaCtrl);
static int32_t VSPAPowerUp(vspaControl_t *pVspaCtrl);
static int32_t StartUp(vspaControl_t *pVspaCtrl);
static void MboxReset(vspaControl_t *pVspaCtrl);
static void CmdReset(vspaControl_t *pVspaCtrl);
static int CrcDiffIndex(vspaControl_t *pVspaCtrl, int seqId);
static int32_t BuffersAlloc(vspaControl_t *pVspaCtrl);
static void BuffersDealloc(vspaControl_t *pVspaCtrl);
static void CopyBytes(char* pDest, char* pSrc, uint32_t len);


extern void Sig2UsrSend(vspaControl_t *pVspaCtrl, int32_t aValue);
extern uint32_t VspaGetAXIAddr(vspaControl_t *pVspaCtrl, dma_addr_t dmaAxiAddr);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

static uint32_t OalCommDispatcher(oal_dispatcher_t *d, uint32_t func, char *in, int len)
{
    int32_t ret = 0;

    switch (func) {
        case RSDK_LAX_UAPI_GET_HW_CFG: 
        {
            pr_debug("vspa%d: RSDK_LAX_UAPI_GET_HW_CFG\n", gOalCommVspaCtrl->id);
            if (len != 0)
            {
                ret = -EFAULT;
                break;
            }
            ret = OAL_RPCAppendReply(d, (char*)&gOalCommVspaCtrl->hardware, sizeof(struct vspa_hardware));
            if (ret != 0)
            {
                ret = -EFAULT;
            }
            break;
        }
        case RSDK_LAX_UAPI_STARTUP: 
        {
            pr_debug("vspa%d: RSDK_LAX_UAPI_STARTUP\n", gOalCommVspaCtrl->id);
            if (len != sizeof(struct vspa_startup))
            {
                pr_err("vspa%d: RSDK_LAX_UAPI_STARTUP incorrect len\n", gOalCommVspaCtrl->id);
                ret = -EFAULT;
                break;
            }
            ret = VspaStartUp(gOalCommVspaCtrl, (struct vspa_startup*)in);
            break;
        }
        case RSDK_LAX_UAPI_CMD_WRITE: 
        {
            uint32_t cmdSize;
            pr_debug("vspa%d: RSDK_LAX_UAPI_CMD_WRITE\n", gOalCommVspaCtrl->id);
            cmdSize = *((uint32_t*)in);

            ret = VspaCmdSend(gOalCommVspaCtrl, in + sizeof(uint32_t), cmdSize);
            break;
        }
        case RSDK_LAX_UAPI_REQ_POWER:
        {
            pr_debug("vspa%d: RSDK_LAX_UAPI_REQ_POWER\n", gOalCommVspaCtrl->id);
            if (len != sizeof(uint32_t))
            {
                pr_err("vspa%d: RSDK_LAX_UAPI_REQ_POWER incorrect len \n", gOalCommVspaCtrl->id);
                ret = -EFAULT;
                break;
            }
            ret = VspaPowerRequest(gOalCommVspaCtrl, *((uint32_t*)in));
            break;
        }
        case RSDK_LAX_UAPI_DMA:
        {
            pr_debug("vspa%d: RSDK_LAX_UAPI_DMA\n", gOalCommVspaCtrl->id);
            if (len != sizeof(struct vspa_dma_req))
            {
                pr_err("vspa%d: RSDK_LAX_UAPI_DMA incorrect len \n", gOalCommVspaCtrl->id);
                ret = -EFAULT;
                break;
            }
            ret = VspaDmaRequest(gOalCommVspaCtrl, (struct vspa_dma_req *)in);
            break;
        }
        case RSDK_LAX_UAPI_SET_RECOVERY:
        {
            if (len != 0)
            {
                pr_err("vspa%d: RSDK_LAX_UAPI_SET_RECOVERY incorrect len \n", gOalCommVspaCtrl->id);
                ret = -EFAULT;
                break;
            }
            VspaSetRecovery(gOalCommVspaCtrl);
            break;
        }
        default:
        {
            break;
        }
    }

    return ret;
}

static void SeqIdReset(vspaControl_t *pVspaCtrl)
{
    int seqId;

    pVspaCtrl->activeSeqIds = 0;
    pVspaCtrl->lastSeqId = 0;
    for (seqId = 0; seqId < MAX_SEQIDS; seqId++)
        pVspaCtrl->seqId[seqId].cmdId = -1;
}


static int SeqIdGetNext(vspaControl_t *pVspaCtrl)
{
    int next;
    uint32_t ids = (pVspaCtrl->activeSeqIds) << MAX_SEQIDS |
                pVspaCtrl->activeSeqIds;

    ids = ids >> (pVspaCtrl->lastSeqId + 1);
    next = ffs(~ids) - 1;
    if (next >= MAX_SEQIDS)
    {
        return -ENOSR;
    }
    next += (pVspaCtrl->lastSeqId + 1);
    next &= MAX_SEQIDS - 1;

    IF_LAX_DRV_DEBUG(DEBUG_SEQID)
        pr_info("seqId: active %04X, last %d, ids %08X, next %d\n",
        pVspaCtrl->activeSeqIds, pVspaCtrl->lastSeqId, ids, next);

    pVspaCtrl->activeSeqIds |= 1 << next;
    pVspaCtrl->lastSeqId = next;
    pVspaCtrl->seqId[next].cmdId = -1;
    gsCmdList[next] = (uint32_t)(gsCmdCounter++);   // the ID for user space
    return next;                                    // return the index for LAX
}

void SeqIdRelease(vspaControl_t *pVspaCtrl, int seqId)
{
    IF_LAX_DRV_DEBUG(DEBUG_SEQID)
        pr_info("SeqIdRelease(%d)\n", seqId);

    if (seqId < 0 || seqId >= MAX_SEQIDS)
    {
        return;
    }

    if (pVspaCtrl->seqId[seqId].cmdId >= 0) {
        pVspaCtrl->seqId[seqId].cmdId = -1;

        if (pVspaCtrl->seqId[seqId].cmdBufferIdx >= 0) {
            CbufferFree(&pVspaCtrl->cmd_buffer,
                pVspaCtrl->seqId[seqId].cmdBufferIdx,
                pVspaCtrl->seqId[seqId].cmdBufferSize);
            pVspaCtrl->seqId[seqId].cmdBufferIdx = -1;
        }

        if (pVspaCtrl->seqId[seqId].cmdBdIndex >= 0) {
            pVspaCtrl->seqId[seqId].cmdBdIndex = -1;
        }
    }
    pVspaCtrl->activeSeqIds &= ~(1 << seqId);
}

/************************ Mailbox Queues ***************************/

static void MboxReset(vspaControl_t *pVspaCtrl)
{
    pVspaCtrl->mb64Queue.idxEnqueue = 0;
    pVspaCtrl->mb64Queue.idxDequeue = 0;
    pVspaCtrl->mb64Queue.idxComplete = 0;
}

static void CmdReset(vspaControl_t *pVspaCtrl)
{
    pVspaCtrl->firstCmd = 1;
    CbufferReset(&pVspaCtrl->cmd_buffer);
    SeqIdReset(pVspaCtrl);
}


/************* DMA Functions **************************************************/

static int32_t DmaNextIndex(int curr)
{
    return (curr == (DMA_QUEUE_ENTRIES - 1)) ? 0 : curr+1;
}

static void DmaResetQueue(vspaControl_t *pVspaCtrl)
{
    unsigned long irqflags;

    OAL_spin_lock_irqsave(&pVspaCtrl->dmaTxQueueLock, &irqflags);
    pVspaCtrl->dmaQueue.idxDma = 0;
    OAL_spin_unlock_irqrestore(&pVspaCtrl->dmaTxQueueLock, &irqflags);

    OAL_spin_lock(&pVspaCtrl->dmaEnqueueLock);
    pVspaCtrl->dmaQueue.idxQueue = 0;
    pVspaCtrl->dmaQueue.idxChk = 0;
    pVspaCtrl->dmaQueue.chan = 0;
    pVspaCtrl->cmdDmaChan   = 0;
    OAL_spin_unlock(&pVspaCtrl->dmaEnqueueLock);
}


/* sometimes called from IRQ handler */
static int32_t DmaTransmit(vspaControl_t *pVspaCtrl)
{
    int ret = 0;
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    dmaQueue_t *pDmaQueue = &(pVspaCtrl->dmaQueue);
    struct vspa_dma_req *pDr;
    unsigned long irqflags = 0;
    uint32_t mappedAxiAddr;

    OAL_spin_lock_irqsave(&pVspaCtrl->dmaTxQueueLock, &irqflags);
    pDr = &(pDmaQueue->entry[pDmaQueue->idxDma]);

    if (pDmaQueue->idxDma == pDmaQueue->idxQueue) /* Queue is empty */
        ret = -ENOMSG;
    else if (pDmaQueue->idxDma != pDmaQueue->idxChk) /* DMA is in process */
        ret = -EBUSY;
    else {
        /* TODO Find a proper platform independent way of calling Address Translation */
        /* Map the DMA AXI address to ATU window */
        mappedAxiAddr = (LAX_PLATFORM_ATU != 0) ? VspaGetAXIAddr(pVspaCtrl, pDr->axi_addr) : pDr->axi_addr;

        if (-1U == mappedAxiAddr) {
            ret = -EINVAL;
            goto hndl_retval;
        }

        /* Program the DMA transfer */
        VspaRegWrite(pRegs + DMA_DMEM_ADDR_REG_OFFSET, pDr->dmem_addr);
        VspaRegWrite(pRegs + DMA_AXI_ADDR_REG_OFFSET,  mappedAxiAddr);
        VspaRegWrite(pRegs + DMA_BYTE_CNT_REG_OFFSET,  pDr->byte_cnt);
        VspaRegWrite(pRegs + DMA_XFR_CTRL_REG_OFFSET,  pDr->xfr_ctrl);

        /* Update the queue */
        pDmaQueue->idxDma = DmaNextIndex(pDmaQueue->idxDma);
    }
hndl_retval:
    OAL_spin_unlock_irqrestore(&pVspaCtrl->dmaTxQueueLock, &irqflags);
    return ret;
}

static int32_t DmaEnqueue(vspaControl_t *pVspaCtrl, struct vspa_dma_req *pDmaReq)
{
    int ret = 0;
    dmaQueue_t *pDmaQueue = &(pVspaCtrl->dmaQueue);
    int nextSlot;

    OAL_spin_lock(&pVspaCtrl->dmaEnqueueLock);

    nextSlot = DmaNextIndex(pDmaQueue->idxQueue);

    if (nextSlot == pDmaQueue->idxChk) /* Queue is full */
        ret = -ENOMEM;
    else {
        struct vspa_dma_req *pDr;

        pDr = &(pDmaQueue->entry[pDmaQueue->idxQueue]);
        pr_debug("DMA Enqueue(%d)\n", pDmaQueue->idxQueue);

        pDr->control   = pDmaReq->control;
        pDr->dmem_addr = pDmaReq->dmem_addr;
        pDr->axi_addr  = pDmaReq->axi_addr;
        pDr->byte_cnt  = pDmaReq->byte_cnt;
        pDr->xfr_ctrl  = pDmaReq->xfr_ctrl | 0x4000; /* IRQ_EN */
        pDmaQueue->chan      = pDr->xfr_ctrl & 0x1F;

        IF_LAX_DRV_DEBUG(DEBUG_DMA)
            pr_info("vspa%d dma: %08X %08X %016llX %08X %08X\n",
                pVspaCtrl->id, pDr->control, pDr->dmem_addr,
                pDr->axi_addr, pDr->byte_cnt, pDr->xfr_ctrl);
        /* barrier */
        pDmaQueue->idxQueue = nextSlot;
    }
    OAL_spin_unlock(&pVspaCtrl->dmaEnqueueLock);
    DmaTransmit(pVspaCtrl);
    return ret;
}

/************************ VSPA helper functions **********************/

static int32_t VSPAPowerUp(vspaControl_t *pVspaCtrl)
{
    int ret = 0;
    uint32_t  *pRegs = pVspaCtrl->pRegs;

    /* Disable all interrupts */
    VspaRegWrite(pRegs + IRQEN_REG_OFFSET, 0x0);
    DmaResetQueue(pVspaCtrl);
    MboxReset(pVspaCtrl);
    CmdReset(pVspaCtrl);

    return ret;
}


static int32_t StartUp(vspaControl_t *pVspaCtrl)
{
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    uint32_t val, msb, lsb;
    uint32_t dmaChannels;
    uint32_t vspaSwVersion, ippuSwVersion;
    int ctr;

    IF_LAX_DRV_DEBUG(DEBUG_STARTUP) {
        pr_info("vspa%d: StartUp()\n", pVspaCtrl->id);
        pr_info("Command buffer: addr = %08X\n",
            pVspaCtrl->cmdBufAddr);
    }
    /* Ask the VSPA to go */
    val = VspaRegRead(pRegs + CONTROL_REG_OFFSET);
    val = (val & CONTROL_REG_MASK) | CONTROL_HOST_GO;
    VspaRegWrite(pRegs + CONTROL_REG_OFFSET, val);

    /* Wait for the 64 bit mailbox bit to be set */
    for (ctr = VSPA_STARTUP_TIMEOUT; ctr; ctr--) {
        if (VspaRegRead(pRegs + HOST_MBOX_STATUS_REG_OFFSET) &
                            MBOX_STATUS_IN_64_BIT)
            break;
        OAL_usleep(1);
    }
    if (!ctr) {
        LAX_DRV_ERR("%d: timeout waiting for Boot Complete msg\n", pVspaCtrl->id);
        goto startup_fail;
    }
    msb = VspaRegRead(pRegs + HOST_IN_64_MSB_REG_OFFSET);
    lsb = VspaRegRead(pRegs + HOST_IN_64_LSB_REG_OFFSET);
    IF_LAX_DRV_DEBUG(DEBUG_STARTUP)
        pr_info("Boot Ok Msg: msb = %08X, lsb = %08X\n", msb, lsb);
    /* Check Boot Complete message */
    if (msb != 0xF1000000) {
        LAX_DRV_ERR("%d: Boot Complete msg did not match\n", pVspaCtrl->id);
        goto startup_fail;
    }
    dmaChannels = lsb;

    vspaSwVersion = VspaRegRead(pRegs + SWVERSION_REG_OFFSET);
    ippuSwVersion = VspaRegRead(pRegs + IPPU_SWVERSION_REG_OFFSET);

    /* Set SPM buffer */
    msb = MSB_HANDSHAKE;
    lsb = LSB_HANDSHAKE;
    VspaRegWrite(pRegs + HOST_OUT_64_MSB_REG_OFFSET, msb);
    VspaRegWrite(pRegs + HOST_OUT_64_LSB_REG_OFFSET, lsb);
    /* Wait for the 64 bit mailbox bit to be set */
    for (ctr = VSPA_STARTUP_TIMEOUT; ctr; ctr--) {
        if (VspaRegRead(pRegs + HOST_MBOX_STATUS_REG_OFFSET) &
                            MBOX_STATUS_IN_64_BIT)
            break;
        OAL_usleep(1);
    }
    if (!ctr) {
        LAX_DRV_ERR("%d: timeout waiting for SPM Ack msg\n", pVspaCtrl->id);
        goto startup_fail;
    }
    msb = VspaRegRead(pRegs + HOST_IN_64_MSB_REG_OFFSET);
    lsb = VspaRegRead(pRegs + HOST_IN_64_LSB_REG_OFFSET);
    IF_LAX_DRV_DEBUG(DEBUG_STARTUP)
        pr_info("SPM Ack Msg: msb = %08X, lsb = %08X\n", msb, lsb);
    if (msb != 0xF0700000) {
        LAX_DRV_ERR("%d: SPM Ack error %08X\n", pVspaCtrl->id, msb);
        goto startup_fail;
    }

    if (dmaChannels) {
        pVspaCtrl->cmdDmaChan   = (dmaChannels)&0xFF;
    } else { /* legacy images */
        pVspaCtrl->cmdDmaChan   = pVspaCtrl->legacyCmdDmaChan;
    }

    IF_LAX_DRV_DEBUG(DEBUG_STARTUP) {
        pr_info("SW Version: vspa = %08X, ippu = %08X\n",
                vspaSwVersion, ippuSwVersion);
    }

    pVspaCtrl->versions.vspa_sw_version = vspaSwVersion;
    pVspaCtrl->versions.ippu_sw_version = ippuSwVersion;
    VspaEnableMailboxIrqs(pVspaCtrl);
    return 0;

startup_fail:
    pVspaCtrl->versions.vspa_sw_version = ~0;
    pVspaCtrl->versions.ippu_sw_version = ~0;
    VspaRegWrite(pVspaCtrl->pRegs + IRQEN_REG_OFFSET, 0);
    return -EIO;
}

/************************ IRQ handlers ****************************************/

static void VspaEnableDmaIrqs(vspaControl_t *pVspaCtrl)
{
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    uint32_t irqEn  = VspaRegRead(pRegs + IRQEN_REG_OFFSET);

    VspaRegWrite(pRegs + DMA_IRQ_STAT_REG_OFFSET, 0xFFFFFFFFUL);
    VspaRegWrite(pRegs + DMA_COMP_STAT_REG_OFFSET, 0xFFFFFFFFUL);
    VspaRegWrite(pRegs + DMA_XFRERR_STAT_REG_OFFSET, 0xFFFFFFFFUL);
    VspaRegWrite(pRegs + DMA_CFGERR_STAT_REG_OFFSET, 0xFFFFFFFFUL);

    /* Enable DMA complete and DMA error IRQs */
    irqEn |= 0x30;
    VspaRegWrite(pRegs + IRQEN_REG_OFFSET, irqEn);
}

static void VspaEnableMailboxIrqs(vspaControl_t *pVspaCtrl)
{
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    uint32_t irqEn  = VspaRegRead(pRegs + IRQEN_REG_OFFSET);

    VspaRegWrite(pRegs + HOST_FLAGS0_REG_OFFSET, 0xFFFFFFFFUL);
    VspaRegWrite(pRegs + HOST_FLAGS1_REG_OFFSET, 0xFFFFFFFFUL);

    VspaRegRead(pRegs + HOST_IN_64_MSB_REG_OFFSET);
    VspaRegRead(pRegs + HOST_IN_64_LSB_REG_OFFSET);
    VspaRegWrite(pRegs + STATUS_REG_OFFSET, STATUS_REG_IRQ_VCPU_SENT_MSG);

    /*Enable Mailbox anf Flag IRQs */
    irqEn |= 0xF00C;
    VspaRegWrite(pRegs + IRQEN_REG_OFFSET, irqEn);
}

/* Command has been consumed or Mailbox transfer completed */
static void VspaFlags0IrqHandler(vspaControl_t *pVspaCtrl)
{
    int seqId;
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    uint32_t status = VspaRegRead(pRegs + STATUS_REG_OFFSET);
    uint32_t flags0 = VspaRegRead(pRegs + HOST_FLAGS0_REG_OFFSET);

    VspaRegWrite(pRegs + HOST_FLAGS0_REG_OFFSET, flags0);

    /* OAL_spin_lock(&pVspaCtrl->irq_lock); */
    if (pVspaCtrl->irqBits) {
        pr_err("VSPA%d flg0 irqs = %d\n",
            pVspaCtrl->id, pVspaCtrl->irqBits);
    }
    pVspaCtrl->irqBits |= 2;

    IF_LAX_DRV_DEBUG(DEBUG_FLAGS0_IRQ)
        pr_info("vspa%d: flags0 = %08x, status = %08x\n",
            pVspaCtrl->id, flags0, status);
#ifdef MB_CHECK_IN_IRQ
    /* Check if any queued mailbox transactions completed */
    mb_transmit_check(pVspaCtrl);
#endif


    /* Handle Flags0 interrupts */
    while (flags0) {
        seqId = ffs(flags0) - 1;
        //pr_info("flags0 = %08X, seqId = %d, pVspaCtrl->seqId[seqId].cmdId=%d\n", flags0, seqId, pVspaCtrl->seqId[seqId].cmdId);
        flags0 &= ~(1 << seqId);
        if (pVspaCtrl->seqId[seqId].cmdId < 0) 
        {
            LAX_DRV_ERR("%d: unmatched seqId %d - flag ignored\n",
                pVspaCtrl->id, seqId);
            /* TODO - report error event */
            continue;
        }

        // new user space signalling, for every completed command
        Sig2UsrSend(pVspaCtrl, ((((int)RSDK_LAX_SIG_COMMAND_DONE) << RSDK_LAX_SIGNAL_TYPE_SHIFT) | 
                (pVspaCtrl->seqId[seqId].cmdId)));
        // release the command id
        SeqIdRelease(pVspaCtrl, pVspaCtrl->seqId[seqId].flagId);
        
        /* Check CRC values for redundantly executed commands */
        if (pVspaCtrl->seqId[seqId].cmdRdntReq != 0)
        {
            DmaWait(pVspaCtrl, RSDK_LAX_DMA_CRC_CHANNEL);
            if (CrcDiffIndex(pVspaCtrl, seqId) >= 0)
            {
                Sig2UsrSend(pVspaCtrl, (((int)RSDK_LAX_SIG_CRC_FAIL) << RSDK_LAX_SIGNAL_TYPE_SHIFT));
            }
        }
                                        
        /* Release SEQID if no reply is expected */
        if (pVspaCtrl->seqId[seqId].cmdBdIndex >= 0) {
            pVspaCtrl->seqId[seqId].cmdBdIndex = -1;
        }
    }

    /* OAL_spin_unlock(&pVspaCtrl->irq_lock); */
    pVspaCtrl->irqBits &= ~2;
}


static void VspaFlags1IrqHandler(vspaControl_t *pVspaCtrl)
{
    int bitId;
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    uint32_t status = VspaRegRead(pRegs + STATUS_REG_OFFSET);
    uint32_t flags1 = VspaRegRead(pRegs + HOST_FLAGS1_REG_OFFSET);

    VspaRegWrite(pRegs + HOST_FLAGS1_REG_OFFSET, flags1);

    IF_LAX_DRV_DEBUG(DEBUG_FLAGS1_IRQ)
        pr_info("vspa%d: flags1 = %08x, status = %08x\n",
            pVspaCtrl->id, flags1, status);


    /* Handle Flags1 interrupts for CTE/SPT-triggered commands completions */
    while (flags1) {
        int crcIdxOff, cmdIdx;
        rsdkLaxSigType_t sigType;

        bitId = ffs(flags1) - 1;
        flags1 &= ~(1 << bitId);
        sigType = (bitId >= RSDK_LAX_CTE_INT_OFFSET) ? RSDK_LAX_SIG_CTE_COMMAND_DONE :  RSDK_LAX_SIG_SPT_COMMAND_DONE;
        cmdIdx = (sigType == RSDK_LAX_SIG_CTE_COMMAND_DONE) ? (bitId - RSDK_LAX_CTE_INT_OFFSET) : bitId;
        crcIdxOff = (sigType == RSDK_LAX_SIG_CTE_COMMAND_DONE) ? (bitId - RSDK_LAX_CTE_INT_OFFSET) : (bitId + RSDK_LAX_CTE_CMD_MAX_NUM);
        /* Check CRC values for redundantly executed CTE/SPT commands */
        if (((sigType == RSDK_LAX_SIG_CTE_COMMAND_DONE) && (pVspaCtrl->cteRdnt[cmdIdx] != 0))
             || (pVspaCtrl->sptRdnt[cmdIdx] != 0))
        {
            DmaWait(pVspaCtrl, RSDK_LAX_DMA_CRC_CHANNEL);
            if (CrcDiffIndex(pVspaCtrl, RSDK_LAX_MAX_CMDS_NUM + crcIdxOff) >= 0)
            {
                Sig2UsrSend(pVspaCtrl, (((int)RSDK_LAX_SIG_CRC_FAIL) << RSDK_LAX_SIGNAL_TYPE_SHIFT));
            }
        }

        /* send CTE/SPT-triggered command consumed signal */
        Sig2UsrSend(pVspaCtrl, ((((int)sigType) << RSDK_LAX_SIGNAL_TYPE_SHIFT) | (cmdIdx)));
    }
}


static void VspaDmaIrqHandler(vspaControl_t *pVspaCtrl)
{
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    dmaQueue_t *pDmaQueue = &(pVspaCtrl->dmaQueue);
    struct vspa_dma_req *pDr;
    int flags = 0;
    uint32_t status;
    uint32_t stat;
    uint32_t mask;
    int err;

    if (pVspaCtrl->irqBits) {
        pr_err("VSPA%d dma irqs = %d\n",
            pVspaCtrl->id, pVspaCtrl->irqBits);
    }
    pVspaCtrl->irqBits |= 4;

    status = VspaRegRead(pRegs + STATUS_REG_OFFSET);
    IF_LAX_DRV_DEBUG(DEBUG_DMA_IRQ)
    pr_info("vspa%d: dma_irq %08x, COMP %08x, XFRERR %08x, CFGERR %08x\n",
            pVspaCtrl->id,
            VspaRegRead(pRegs + DMA_IRQ_STAT_REG_OFFSET),
            VspaRegRead(pRegs + DMA_COMP_STAT_REG_OFFSET),
            VspaRegRead(pRegs + DMA_XFRERR_STAT_REG_OFFSET),
            VspaRegRead(pRegs + DMA_CFGERR_STAT_REG_OFFSET));

    mask = 1 << pVspaCtrl->dmaQueue.chan;
    /* legacy VSPA images consider all other DMA channels to be SPM */

    /* Check for completed DMAs */
    if (status & STATUS_REG_IRQ_DMA_COMP) {
        stat = VspaRegRead(pRegs + DMA_IRQ_STAT_REG_OFFSET);
        if (stat & mask) {
            flags |= DMA_FLAG_COMPLETE;
            VspaRegWrite(pRegs + DMA_IRQ_STAT_REG_OFFSET, mask);
            VspaRegWrite(pRegs + DMA_COMP_STAT_REG_OFFSET, mask);
            stat &= ~mask;
        }
        /* Watch for completed DMAs from VSPA with incorrect IRQ_EN */
        if (stat) {
            LAX_DRV_ERR("%d: DMA IRQ STAT %08x\n", pVspaCtrl->id, stat);
            VspaRegWrite(pRegs + DMA_IRQ_STAT_REG_OFFSET, stat);
        }
    }
    /* Check for DMA errors from any channel */
    if (status & STATUS_REG_IRQ_DMA_ERR) {
        /* DMA transfer errors */
        stat = VspaRegRead(pRegs + DMA_XFRERR_STAT_REG_OFFSET);
        if (stat) {
            if (stat & mask) /* Transfer error from DMA channel */
                //todo: for lax safety, let upper layers know about it
                flags |= DMA_FLAG_XFRERR;
            if (stat & ~(mask)) { /* Other channels */
                LAX_DRV_ERR("%d: DMA XFRERR %08x\n", pVspaCtrl->id, stat);
            }
            VspaRegWrite(pRegs+DMA_XFRERR_STAT_REG_OFFSET, stat);
        }
        /* DMA configuration errors */
        stat = VspaRegRead(pRegs + DMA_CFGERR_STAT_REG_OFFSET);
        if (stat) {
            if (stat & mask) /* Config error from DMA channel */
                flags |= DMA_FLAG_CFGERR;
            if (stat & ~(mask)) { /* Other channels */
                LAX_DRV_ERR("%d: DMA CFGERR %08x\n", pVspaCtrl->id, stat);
            }
            VspaRegWrite(pRegs+DMA_CFGERR_STAT_REG_OFFSET, stat);
        }
    }

    if (flags) 
    {
        pDr = &(pDmaQueue->entry[pDmaQueue->idxChk]);
        if (pDr->type == VSPA_EVENT_DMA) 
        {
            err = 0;
            if (flags & DMA_FLAG_CFGERR)
                err = VSPA_ERR_DMA_CFGERR;
            else if (flags & DMA_FLAG_XFRERR)
                err = VSPA_ERR_DMA_XFRERR;
            if (pDr->flags & VSPA_FLAG_REPORT_DMA_COMPLETE || err) 
            {
                // send signal to user space for event processing
                Sig2UsrSend(pVspaCtrl, (((int)RSDK_LAX_SIG_DMA_DONE) << RSDK_LAX_SIGNAL_TYPE_SHIFT) | pDr->id);
            }
        } 
        else if (pDr->type == VSPA_EVENT_CMD) 
        {
            if (pDr->id >= MAX_SEQIDS)
            {
                ; /* skip multiple DMAs */
                // TODO : rise safety related process exception
            }
            else
            {
                if (pVspaCtrl->seqId[pDr->id].cmdBufferIdx >= 0) 
                {
                    CbufferFree(&pVspaCtrl->cmd_buffer,
                            pVspaCtrl->seqId[pDr->id].cmdBufferIdx,
                            pVspaCtrl->seqId[pDr->id].cmdBufferSize);
                    pVspaCtrl->seqId[pDr->id].cmdBufferIdx = -1;
                }
                else
                {
                }
            }
        } else
            LAX_DRV_ERR("%d: unknown DMA type %d\n", pVspaCtrl->id, pDr->type);
        pDmaQueue->idxChk = pDmaQueue->idxDma;
        DmaTransmit(pVspaCtrl);
    }

    /* OAL_spin_unlock(&pVspaCtrl->irq_lock); */
    pVspaCtrl->irqBits &= ~4;
}


static void VspaGenIrqHandler(vspaControl_t *pVspaCtrl)
{
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    uint32_t status = VspaRegRead(pRegs + STATUS_REG_OFFSET);

    /* OAL_spin_lock(&pVspaCtrl->irq_lock); */
    if (pVspaCtrl->irqBits) {
        pr_err("VSPA%d gen irqs = %d\n",
            pVspaCtrl->id, pVspaCtrl->irqBits);
    }
    pVspaCtrl->irqBits |= 8;

    VspaRegWrite(pRegs + STATUS_REG_OFFSET, status);
    /*pr_info("vspa%d: IRQEN %08x, STATUS %08x => %08X\n",
        pVspaCtrl->id, irqen, status,
        VspaRegRead(pRegs + STATUS_REG_OFFSET)); */

    /* OAL_spin_unlock(&pVspaCtrl->irq_lock); */
    pVspaCtrl->irqBits &= ~8;
}

static void CopyBytes(char* pDest, char* pSrc, uint32_t len)
{
    uint32_t i;
    for(i = 0; i < len; i++)
    {
        *pDest = *pSrc;
        pDest++;
        pSrc++;
    }
}


static int CrcDiffIndex(vspaControl_t *pVspaCtrl, int replyIdx)
{
    int i, retIdx = -1;

    uint32_t  *crc1 = pVspaCtrl->replyVaddr[replyIdx];
    uint32_t  *crc2 = pVspaCtrl->replyVaddr[replyIdx] + (RSDK_LAX_CRC_BIT_LEN >> 5);
    for (i = 0; i < (RSDK_LAX_CRC_BIT_LEN >> 5); i++, crc1++, crc2++)
    {
        if (OAL_read32(crc1) != OAL_read32(crc2))
        {
            retIdx = i;
            break;
        }
    }
    return retIdx;
}


static int32_t BuffersAlloc(vspaControl_t *pVspaCtrl)
{
    uint32_t i;
    uint32_t rc = 0;
    uint64_t offset = pVspaCtrl->shbufstart;
    const uint32_t align_bytes = PS_AXI_BUS_WIDTH_BITS >> 3;
    //size in bytes for reply buffers
    uint32_t rbytes = 2 * (RSDK_LAX_CRC_BIT_LEN >> 3);
    //size in bytes for command buffers
    uint32_t bytes = RSDK_LAX_SIZEOF_LIBVSPACMD + sizeof(uint32_t) + RSDK_LAX_MAX_FUNC_ARGS * sizeof(uint64_t);

    //initialize virt addresses to NULL
    pVspaCtrl->wrpVirtAddr = NULL; 
    for(i = 0; i < RSDK_LAX_MAX_CMDS_NUM; i++)
    {
        pVspaCtrl->cmdVirtAddr[i] = NULL;
    }
    for(i = 0; i < RSDK_LAX_MAX_CMDS_NUM + RSDK_LAX_CTE_CMD_MAX_NUM + RSDK_LAX_SPT_CMD_MAX_NUM; i++)
    {
        pVspaCtrl->replyVaddr[i] = NULL;
    }

    //allocate the command buffers
    bytes = bytes + align_bytes;
    bytes = align_bytes * ((bytes + align_bytes - 1) / align_bytes);

    for(i = 0; i < RSDK_LAX_MAX_CMDS_NUM; i++)
    {
        pVspaCtrl->cmdPhyAddr[i] = offset;
        pVspaCtrl->cmdVirtAddr[i] = OAL_memmap(pVspaCtrl->cmdPhyAddr[i], bytes, KERNEL_MAP);
        if (pVspaCtrl->cmdVirtAddr[i] == NULL)
        {
            rc = -ENOMEM;
            break;
        }
        offset += bytes;
    }

    if (rc == 0)
    {
        //allocate a buffer for wrap-around case
        pVspaCtrl->wrpPhyAddr = offset;
        pVspaCtrl->wrpVirtAddr = OAL_memmap(pVspaCtrl->wrpPhyAddr, bytes,  KERNEL_MAP );
        if (pVspaCtrl->wrpVirtAddr == NULL)
        {
            rc = -ENOMEM;
        }
        offset += bytes;
    }

    if (rc == 0)
    {
        //allocate the reply buffers
        rbytes = rbytes + align_bytes;
        rbytes = align_bytes * ((rbytes + align_bytes - 1) / align_bytes);

        for(i = 0; i < RSDK_LAX_MAX_CMDS_NUM + RSDK_LAX_CTE_CMD_MAX_NUM + RSDK_LAX_SPT_CMD_MAX_NUM; i++)
        {
            pVspaCtrl->replyPaddr[i] = offset;
            pVspaCtrl->replyVaddr[i] = OAL_memmap(pVspaCtrl->replyPaddr[i], rbytes, KERNEL_MAP);
            if (pVspaCtrl->replyVaddr[i] == NULL)
            {
                rc = -ENOMEM;
                break;
            }
            offset += rbytes;
        }
    }
    if (offset >= pVspaCtrl->shbufstart + pVspaCtrl->shbuflen)
    {
        rc = -ENOMEM;
    }
    if (rc != 0)
    {
        BuffersDealloc(pVspaCtrl);
    }
    return rc;
}


static void BuffersDealloc(vspaControl_t *pVspaCtrl)
{
    uint32_t i;
    const uint32_t align_bytes = PS_AXI_BUS_WIDTH_BITS >> 3;
    //size in bytes for reply buffers
    uint32_t rbytes = 2 * (RSDK_LAX_CRC_BIT_LEN >> 3);
    //size in bytes for command buffers
    uint32_t bytes = RSDK_LAX_SIZEOF_LIBVSPACMD + sizeof(uint32_t) + RSDK_LAX_MAX_FUNC_ARGS * sizeof(uint64_t);

    //deallocate the command buffers
    bytes = bytes + align_bytes;
    bytes = align_bytes * ((bytes + align_bytes - 1) / align_bytes);

    for(i = 0; i < RSDK_LAX_MAX_CMDS_NUM; i++)
    {
        if (pVspaCtrl->cmdVirtAddr[i] != NULL)
        {
            OAL_memunmap(pVspaCtrl->cmdVirtAddr[i], bytes, KERNEL_MAP);
        }
    }
    //deallocate a buffer for wrap-around case
    if (pVspaCtrl->wrpVirtAddr != NULL)
    {
        OAL_memunmap(pVspaCtrl->wrpVirtAddr, bytes,  KERNEL_MAP );
    }

    //deallocate the reply buffers
    rbytes = rbytes + align_bytes;
    rbytes = align_bytes * ((rbytes + align_bytes - 1) / align_bytes);

    for(i = 0; i < RSDK_LAX_MAX_CMDS_NUM + RSDK_LAX_CTE_CMD_MAX_NUM + RSDK_LAX_SPT_CMD_MAX_NUM; i++)
    {
        if (pVspaCtrl->replyVaddr[i] != NULL)
        {
            OAL_memunmap(pVspaCtrl->replyVaddr[i], rbytes, KERNEL_MAP);
        }
    }

    return;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/


int32_t VSPAPowerDown(vspaControl_t *pVspaCtrl)
{
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    uint32_t  *pDbgRegs = pVspaCtrl->pDbgRegs;
    int ret = 0, ret1 = 0;
    uint32_t val;
    int ctr;

    gIsRecovery = 0;             // reset the recovery flag too
    /* Disable all interrupts */
    VspaRegWrite(pRegs + IRQEN_REG_OFFSET, 0x0);

    pVspaCtrl->versions.vspa_sw_version = ~0;
    pVspaCtrl->versions.ippu_sw_version = ~0;

    /* clear debug_msg_go, host_msg_go, ru_go and host_go bits */
    val = VspaRegRead(pRegs + CONTROL_REG_OFFSET);
    val = (val & CONTROL_REG_MASK) & ~CONTROL_HOST_GO;
    VspaRegWrite(pRegs + CONTROL_REG_OFFSET, val);

    /* Enable the invasive (halting) debug mode */
    VspaRegWrite(pDbgRegs + DBG_GDBEN_REG_OFFSET, 0x1);

    /* Enable dbg_dbgen */
    val = VspaRegRead(pDbgRegs + DBG_DVR_REG_OFFSET);
    val = val | (1 << 13);
    VspaRegWrite(pDbgRegs + DBG_DVR_REG_OFFSET, val);

    /* change access perpective to VCPU */
    val = VspaRegRead(pDbgRegs + DBG_RAVAP_REG_OFFSET);
    val = val | (1 << 31);
    VspaRegWrite(pDbgRegs + DBG_RAVAP_REG_OFFSET, val);

    val = VspaRegRead(pRegs + CONTROL_REG_OFFSET);
    val = (val & CONTROL_REG_MASK) &
        ~(CONTROL_IPPU_GO | CONTROL_HOST_MSG_GO
                | CONTROL_DEBUG_MSG_GO);

    /* change access perpective back to Host */
    val = VspaRegRead(pDbgRegs + DBG_RAVAP_REG_OFFSET);
    val = val & ~(1 << 31);
    VspaRegWrite(pDbgRegs + DBG_RAVAP_REG_OFFSET, val);

    /* Stop all VSPA activities using “force_halt” */
    VspaRegWrite(pDbgRegs + DBG_RCR_REG_OFFSET, 0x4);

    /* Wait for the “halted” bit to be set */
    for (ctr = VSPA_HALT_TIMEOUT; ctr; ctr--) {
        OAL_usleep(1);
        ret1 = VspaRegRead(pDbgRegs + DBG_RCSTATUS_REG_OFFSET)
            & (1 << 13);
        if (ret1)
            break;
    }
    if (!(ret1)) {
        LAX_DRV_ERR("%d: PowerDown() timeout waiting for halt\n",
                pVspaCtrl->id);
        ret = -ETIME;
        return ret;
    }

    /* Issue the reset */
    val = VspaRegRead(pRegs + CONTROL_REG_OFFSET);
    val = (val & CONTROL_REG_MASK) | CONTROL_VCPU_RESET;
    VspaRegWrite(pRegs + CONTROL_REG_OFFSET, val);

    /* Start VSPA activities using “resume” */
    VspaRegWrite(pDbgRegs + DBG_RCR_REG_OFFSET, 0x2);

    return ret;
} 


int32_t VspaStartUp(vspaControl_t *pVspaCtrl, struct vspa_startup * pStartupDesc)
{
    int32_t rc = 0;
    uint32_t alignBytes = pVspaCtrl->hardware.axi_data_width >> 3;


    if ((pStartupDesc->cmd_buf_size & (alignBytes - 1)) || (pStartupDesc->cmd_buf_addr & (alignBytes - 1)))
        return -EINVAL;
    else
        if (pStartupDesc->spm_buf_size && (
                    (pStartupDesc->spm_buf_size & (alignBytes - 1)) ||
                    (pStartupDesc->spm_buf_addr & (alignBytes - 1))))
        {
            return -EINVAL;
        }

    OAL_spin_lock(&pVspaCtrl->controlLock);

    gIsRecovery = 1;                     // set the recovery flag for watchdog lock down
    
    pVspaCtrl->legacyCmdDmaChan = pStartupDesc->cmd_dma_chan;
    pVspaCtrl->cmdBufAddr = pStartupDesc->cmd_buf_addr;
    pVspaCtrl->flags = pStartupDesc->flags;

    rc = BuffersAlloc(pVspaCtrl);
    if (rc == 0)
    {
        rc = StartUp(pVspaCtrl);
        gIsRecovery = 0;                     // clear the recovery flag
    }
    OAL_spin_unlock(&pVspaCtrl->controlLock);

    return rc;
}


int32_t VspaDmaRequest(vspaControl_t *pVspaCtrl, struct vspa_dma_req * pDmaReq)
{
    int32_t rc = 0;
    uint32_t alignBytes = pVspaCtrl->hardware.axi_data_width >> 3;
    /* Enable DMA error and DMA complete interrupts */
    VspaEnableDmaIrqs(pVspaCtrl);

    if ((pDmaReq->dmem_addr & (alignBytes - 1)) || (pDmaReq->axi_addr & (alignBytes - 1))) {
        rc = -EINVAL;
    } else {
        pDmaReq->type = VSPA_EVENT_DMA;
        pDmaReq->xfr_ctrl = (pDmaReq->xfr_ctrl & ~0x1F) | pVspaCtrl->cmdDmaChan;

        rc = DmaEnqueue(pVspaCtrl, pDmaReq);
    }
    return rc;
}

int32_t VspaPowerRequest(vspaControl_t *pVspaCtrl, uint32_t arg)
{
    int rc = 0;

    OAL_spin_lock(&pVspaCtrl->controlLock);
    switch (arg) {
        case VSPA_POWER_DOWN:
            rc = VSPAPowerDown(pVspaCtrl);
            break;
        case VSPA_POWER_CYCLE:
            rc = VSPAPowerDown(pVspaCtrl); /*FALLTHROUGH*/
        case VSPA_POWER_UP:
            if (rc == 0)
                rc = VSPAPowerUp(pVspaCtrl);
            break;
        default:
            rc = -EINVAL;
            break;
    }
    OAL_spin_unlock(&pVspaCtrl->controlLock);
    return rc;
}


int32_t VspaCmdSend(vspaControl_t *pVspaCtrl, char* pCmd, uint32_t len)
{
    int err, i, seqId;
    struct vspa_dma_req dmaReq;
    int cbSize, cbIdx, bufNum;
    uint32_t flags = 0;
    const int funcInfoOff = RSDK_LAX_SIZEOF_LIBVSPACMD; // this is (sizeof(struct libvspa_cmd)
    const int bufaddrsOff = RSDK_LAX_SIZEOF_LIBVSPACMD + sizeof (uint32_t); // this is (sizeof(struct libvspa_cmd) + sizeof(func_addr))
    uint32_t * dmaBuf;
    uint32_t* pFuncInfoPtr ;
    dma_addr_t* pBufAddrsPtr;
    uint32_t mappedAxiAddr;
    rsdkLaxCmdType_t cmdType;

    IF_LAX_DRV_DEBUG(DEBUG_IOCTL)
        pr_info("vspa%d: VspaCmdSend() len = %d\n", pVspaCtrl->id, len);

    if (len > (((RSDK_LAX_MAX_FUNC_ARGS << 1) + 32) << 2)) {
        LAX_DRV_ERR("%d: command exceeds %d bytes \n", pVspaCtrl->id, (((RSDK_LAX_MAX_FUNC_ARGS << 1) + 32) << 2));
        return -EMSGSIZE;
    }

    if ((len & 3) != 0 || len == 0) {
        LAX_DRV_ERR("%d: command must be multiple of 4 bytes and non zero\n",
                             pVspaCtrl->id);
        return -EINVAL;
    }

    OAL_spin_lock(&pVspaCtrl->controlLock);
    seqId = SeqIdGetNext(pVspaCtrl);
    IF_LAX_DRV_DEBUG(DEBUG_IOCTL)
        pr_info("vspa%d: VspaCmdSend() seqId = %d\n", pVspaCtrl->id, seqId);

    if (seqId < 0) {
        LAX_DRV_ERR("%d: no sequence id available\n", pVspaCtrl->id);
        err = -ENOSR;
        goto seqid_fail;
    }
    //copy command content to command buffer
    CopyBytes((char*)pVspaCtrl->cmdVirtAddr[seqId], pCmd, len);

    dmaBuf = pVspaCtrl->cmdVirtAddr[seqId];
    pFuncInfoPtr = &dmaBuf[funcInfoOff / 4];
    pBufAddrsPtr = (dma_addr_t*)&dmaBuf[bufaddrsOff / 4];

    cbSize = len >> 2;
    cbIdx = CbufferAdd(&pVspaCtrl->cmd_buffer, cbSize, pVspaCtrl->hardware.axi_data_width >> 5);
    if (cbIdx < 0) {
        LAX_DRV_ERR("%d: no cmd buffer space available\n", pVspaCtrl->id);
        err = -ENOBUFS;
        goto seqid_release;
    }
    IF_LAX_DRV_DEBUG(DEBUG_IOCTL)
        pr_info("vspa%d: VspaCmdSend() cbidx = %d\n", pVspaCtrl->id, cbIdx);

    //get the command type
    cmdType = (rsdkLaxCmdType_t)(dmaBuf[pVspaCtrl->hardware.axi_data_width >> 5] & 0x0000FF00) >> 8;

    //set the cmdid as seqId, at 32bit word offset AXI_BUS_WORD32_WIDTH
    dmaBuf[pVspaCtrl->hardware.axi_data_width >> 5] =
        (dmaBuf[pVspaCtrl->hardware.axi_data_width >> 5] & 0x00FFFFFF) | (seqId << 24);

    //map the data buffer addresses to ATU window and update the command content
    bufNum = dmaBuf[pVspaCtrl->hardware.axi_data_width >> 5] & 0x000000FF;
    for(i = 0; i < bufNum; i++)
    {
        if (cmdType == RSDK_LAX_CMD_CFG_REPLY_BUF)
        {
            *pBufAddrsPtr = pVspaCtrl->replyPaddr[i];
        }

        /* TODO Find a proper platform independent way of calling Address Translation */
        mappedAxiAddr = (LAX_PLATFORM_ATU != 0) ? VspaGetAXIAddr(pVspaCtrl, *pBufAddrsPtr) : *pBufAddrsPtr ;
        IF_LAX_DRV_DEBUG(DEBUG_IOCTL) {
            pr_info("vspa: VspaCmdSend() dma_addr_t for buf%d is %016llX \n",i, *pBufAddrsPtr);
            pr_info("vspa: VspaCmdSend() mappedAxiAddr for buf%d is %X \n",i, mappedAxiAddr);
        }
        if (-1U == mappedAxiAddr) {
            err = -EINVAL;
            goto seqid_release;
        }
        *pBufAddrsPtr = mappedAxiAddr;
        pBufAddrsPtr++;
    }

    //set the pointer to this command, as 16bit word addresss
    dmaBuf[(len >> 2) - 1] = (pVspaCtrl->cmdBufAddr + (pVspaCtrl->cmd_buffer.size << 2)
            - (cbIdx << 2) - (cbSize << 2) +
            (pVspaCtrl->hardware.axi_data_width >> 3)) / 2;

    pVspaCtrl->seqId[seqId].flags = VSPA_FLAG_REPORT_CMD_CONSUMED;
    pVspaCtrl->seqId[seqId].cmdId = gsCmdList[seqId];            // get the allocated ID, for user space
    pVspaCtrl->seqId[seqId].flagId = seqId;                     // this is the ID for LAX
    pVspaCtrl->seqId[seqId].cmdBufferIdx = cbIdx;
    pVspaCtrl->seqId[seqId].cmdBufferSize = cbSize;
    pVspaCtrl->seqId[seqId].cmdRdntReq = 0;

    if (cmdType == RSDK_LAX_CMD_EXEC)
    {
        pVspaCtrl->seqId[seqId].cmdRdntReq = *pFuncInfoPtr & RSDK_LAX_CMD_REDUNDANCY_MASK;
    }
    else if (cmdType == RSDK_LAX_CMD_CFG_CTE_GRAPH)
    {
         pVspaCtrl->cteRdnt[*pFuncInfoPtr & ~RSDK_LAX_CMD_REDUNDANCY_MASK] = *pFuncInfoPtr & RSDK_LAX_CMD_REDUNDANCY_MASK;
    }
    else if (cmdType == RSDK_LAX_CMD_CFG_SPT_GRAPH)
    {
         pVspaCtrl->sptRdnt[*pFuncInfoPtr & ~RSDK_LAX_CMD_REDUNDANCY_MASK] = *pFuncInfoPtr & RSDK_LAX_CMD_REDUNDANCY_MASK;
    }

    dmaReq.dmem_addr = pVspaCtrl->cmdBufAddr + (pVspaCtrl->cmd_buffer.size << 2)
            - (cbIdx << 2) - (cbSize << 2);
    dmaReq.axi_addr = (dma_addr_t)pVspaCtrl->cmdPhyAddr[seqId];

    if (pVspaCtrl->cmd_buffer.cbIdxPrev > cbIdx)
        dmaReq.xfr_ctrl = 0x0000;    //the cmd buffer wrap-around case
    else
        dmaReq.xfr_ctrl = 0x2000; /* VCPU_GO */
    dmaReq.xfr_ctrl |= pVspaCtrl->cmdDmaChan;
    dmaReq.byte_cnt  = cbSize << 2;

    dmaReq.type = VSPA_EVENT_CMD;
    dmaReq.id =  seqId ;
    dmaReq.flags = flags;

    err = DmaEnqueue(pVspaCtrl, &dmaReq);
    if (err){
        LAX_DRV_ERR("%d: write() DmaEnqueue() failed\n", pVspaCtrl->id);
        goto seqid_release;
    }

    //handle the cmd buffer wrap-around case
    if (pVspaCtrl->cmd_buffer.cbIdxPrev > cbIdx)
    {
        OAL_write32(&pVspaCtrl->wrpVirtAddr[(pVspaCtrl->hardware.axi_data_width >> 5) - 1], dmaBuf[(len >> 2) - 1]);
        dmaReq.dmem_addr = pVspaCtrl->cmdBufAddr + (pVspaCtrl->cmd_buffer.size << 2)
                - (pVspaCtrl->cmd_buffer.cbIdxPrev << 2) - (pVspaCtrl->cmd_buffer.cbSizePrev << 2);
        dmaReq.axi_addr = (dma_addr_t)pVspaCtrl->wrpPhyAddr;

        dmaReq.xfr_ctrl = 0x2000; /* VCPU_GO */
        dmaReq.xfr_ctrl |= pVspaCtrl->cmdDmaChan;
        dmaReq.byte_cnt  = pVspaCtrl->hardware.axi_data_width >> 3;

        dmaReq.type = VSPA_EVENT_CMD;
        dmaReq.id =  seqId ;
        dmaReq.flags = flags;

        err = DmaEnqueue(pVspaCtrl, &dmaReq);
        if (err){
            LAX_DRV_ERR("%d: VspaCmdSend() DmaEnqueue() failed (cmd buffer wrap-around)\n", pVspaCtrl->id);
            goto seqid_release;
        }
    }

    pVspaCtrl->cmd_buffer.cbIdxPrev = cbIdx;
    pVspaCtrl->cmd_buffer.cbSizePrev = cbSize;

    OAL_spin_unlock(&pVspaCtrl->controlLock);

    return err ? err : gsCmdList[seqId];     // return the ID for user space

seqid_release:
    SeqIdRelease(pVspaCtrl, seqId);
seqid_fail:
    OAL_spin_unlock(&pVspaCtrl->controlLock);
    return err;
}


//TODO move this function where relevant
int FullState(vspaControl_t *pVspaCtrl)
{
    return 0;
}


void VspaDisableAllIRQs(vspaControl_t *pVspaCtrl)
{
    /* Disbale all irqs of VSPA */
    VspaRegWrite(pVspaCtrl->pRegs + IRQEN_REG_OFFSET, 0);
}


void VspaInit(vspaControl_t *pVspaCtrl)
{
    size_t dmaSize;
    uint32_t param0, param1, param2;
    dma_addr_t dmaPaddr = 0x00;
    uint32_t val;
    struct vspa_hardware *pHw;


    dmaSize = pVspaCtrl->cmdBufferBytes;
    CbufferInit(&pVspaCtrl->cmd_buffer,
            pVspaCtrl->cmdBufferBytes >> 2,
            &((uint32_t *)dmaPaddr)
            [0],
            0);
    /* Map VSPA registers */
    pVspaCtrl->pRegs = OAL_memmap((dma_addr_t)pVspaCtrl->pMemAddr,
            pVspaCtrl->memSize, KERNEL_MAP);
    pVspaCtrl->pDbgRegs = OAL_memmap((dma_addr_t)pVspaCtrl->pDbgAddr,
            pVspaCtrl->dbgSize, KERNEL_MAP);

    pHw = &pVspaCtrl->hardware;
    param0 = VspaRegRead(pVspaCtrl->pRegs + PARAM0_REG_OFFSET);
    pHw->param0           = param0;
    param1 = VspaRegRead(pVspaCtrl->pRegs + PARAM1_REG_OFFSET);
    pHw->param1           = param1;
    pHw->axi_data_width   = 32 << ((param1 >> 28) & 7);
    pHw->dma_channels     = (param1 >> 16) & 0xFF;
    pHw->gp_out_regs      = (param1 >> 8) & 0xFF;
    pHw->gp_in_regs       = param1 & 0xFF;
    param2 = VspaRegRead(pVspaCtrl->pRegs + PARAM2_REG_OFFSET);
    pHw->param2           = param2;
    pHw->dmem_bytes       = ((param2 >> 8) & 0x3FF) * 400;
    pHw->ippu_bytes       = (param2 >> 31) * 4096;
    pHw->arithmetic_units = param2 & 0xFF;

    pVspaCtrl->versions.vspa_hw_version =
            VspaRegRead(pVspaCtrl->pRegs+HWVERSION_REG_OFFSET);
    pVspaCtrl->versions.ippu_hw_version =
            VspaRegRead(pVspaCtrl->pRegs+IPPU_HWVERSION_REG_OFFSET);
    pVspaCtrl->versions.vspa_sw_version = ~0;
    pVspaCtrl->versions.ippu_sw_version = ~0;

    OAL_spin_lock_init(&pVspaCtrl->dmaTxQueueLock);
    OAL_spin_lock_init(&pVspaCtrl->dmaEnqueueLock);
    OAL_spin_lock_init(&pVspaCtrl->mb64Lock);
    OAL_spin_lock_init(&pVspaCtrl->mbChkLock);
    OAL_spin_lock_init(&pVspaCtrl->controlLock);


    DmaResetQueue(pVspaCtrl);
    MboxReset(pVspaCtrl);
    CmdReset(pVspaCtrl);

    /* Enable core power gating */
    val = VspaRegRead(pVspaCtrl->pRegs + CONTROL_REG_OFFSET);
    val = (val & CONTROL_REG_MASK) | CONTROL_PDN_EN;
    VspaRegWrite(pVspaCtrl->pRegs + CONTROL_REG_OFFSET, val);

    pr_info("vspa%d: hwver 0x%08x, %d AUs, dmem %d bytes\n",
            pVspaCtrl->id, pVspaCtrl->pRegs[HWVERSION_REG_OFFSET],
            pHw->arithmetic_units, pHw->dmem_bytes);

}

void VspaDeInit(vspaControl_t *pVspaCtrl)
{
    OAL_memunmap(pVspaCtrl->pRegs, pVspaCtrl->memSize, KERNEL_MAP);
    OAL_memunmap(pVspaCtrl->pDbgRegs, pVspaCtrl->dbgSize, KERNEL_MAP);
    BuffersDealloc(pVspaCtrl);
}

irqreturn_t VspaIrqHandler(int irq, vspaControl_t *pVspaCtrl)
{
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    uint32_t status = VspaRegRead(pRegs + STATUS_REG_OFFSET);

    if (status & STATUS_REG_IRQ_NON_GEN) {
        if ((status & STATUS_REG_IRQ_FLAGS0) ||
            status & STATUS_REG_IRQ_VCPU_MSG)
            VspaFlags0IrqHandler(pVspaCtrl);
        if (status & STATUS_REG_IRQ_FLAGS1)
            VspaFlags1IrqHandler(pVspaCtrl);
        if ((status & STATUS_REG_IRQ_DMA_COMP) ||
            (status & STATUS_REG_IRQ_DMA_ERR))
            VspaDmaIrqHandler(pVspaCtrl);
    } else
        VspaGenIrqHandler(pVspaCtrl);
    return IRQ_HANDLED;
}

void VspaShutdownTimer(vspaControl_t *pVspaCtrl)
{
    /* shutdown timer cleanly */
}

void DmaWait(vspaControl_t *pVspaCtrl, uint32_t chan)
{
    uint32_t  *pRegs = pVspaCtrl->pRegs;
    do
    {/* wait */
        uint32_t statChan  = VspaRegRead(pRegs + DMA_STAT_ABORT_REG_OFFSET);
        if ((statChan & (1 << chan)) == 0)
        {
            break;
        }
    }while(1);
}

void VspaSetRecovery(vspaControl_t *pVspaCtrl)
{
        gIsRecovery = 1;
}


int VspaOalCommInit()
{
    int rc = 0;

    gsOalCommServ = OAL_RPCRegister(RSDK_LAX_OAL_COMM_SERV, OalCommDispatcher);
    if (gsOalCommServ == NULL) {
        rc = -1;
    }
    return rc;
}

int VspaOalCommExit(void)
{
    return OAL_RPCCleanup(gsOalCommServ);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

