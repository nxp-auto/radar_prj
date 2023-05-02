/*
 * Copyright 2018-2023 NXP
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
#include <oal_uptime.h>
#include <oal_irq_utils.h>
#include <oal_timespec.h>
#include <oal_comm_kernel.h>

#include "lax_driver.h"
#include "rsdk_lax_common.h"
#include "rsdk_status.h"

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
static OAL_RPCService_t     gsOalCommServ = NULL;

//array of events triggered by the low-level driver
static OAL_RPCEvent_t gsRsdkLaxEvents[RSDK_LAX_MAX_EVENTS] = {0}; 

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
lldLaxControl_t *gOalCommLaxCtrl[RSDK_LAX_CORES_NUM];

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
static rsdkStatus_t DmaEnqueue(lldLaxControl_t *pLaxCtrl, struct laxDmaReq *pDmaReq);
static rsdkStatus_t DmaResetQueue(lldLaxControl_t *pLaxCtrl);
static rsdkStatus_t DmaTransmit(lldLaxControl_t *pLaxCtrl);
static void LaxClearAllParityFailBits(lldLaxControl_t *pLaxCtrl);

#ifndef LAX_OS_sa
static
#endif
    rsdkStatus_t LaxDmaRequest(lldLaxControl_t *pLaxCtrl, struct laxDmaReq * pDmaReq);

/**
* @brief        Trigger the specified event in low-level driver
* @param[in]    Trigger the event
* @return       The execution result : RSDK_SUCCESS or  error: RSDK_LAX_ERR_OAL_EVENT_TRIGGER
*/
#ifndef LAX_OS_sa
static 
#endif
rsdkStatus_t LaxTriggerEvent(rsdkLaxEventType_t evtId);

/**
* @brief        Initiate event registation in low-level driver
* @param[in]    Register events up to lastEvt
* @return       The execution result : RSDK_SUCCESS or error RSDK_LAX_ERR_OAL_EVENT_REGISTER
*/
#ifndef LAX_OS_sa
static 
#endif
rsdkStatus_t LaxRegisterEvents(rsdkLaxEventType_t lastEvt);

/**
* @brief        Initiate event deregistation in low-level driver
* @param[in]    Deregister events up to lastEvt
* @return       The execution result : RSDK_SUCCESS or error RSDK_LAX_ERR_OAL_EVENT_DEREGISTER
*/
#ifndef LAX_OS_sa
static rsdkStatus_t LaxDeregisterEvents(rsdkLaxEventType_t lastEvt);
#endif


/**
* @brief            Clear parity error bits in GP_IN registers by setting pairty fail clear bit in GP_OUT registers
* @param[in]        pLaxCtrl    Pointer to lldLaxControl_t structure
* @return           void
*/
static void LaxClearAllParityFailBits(lldLaxControl_t *pLaxCtrl)
{
    uint32_t statParity, i;

    for(i = LAX_INPUT_PARITY_FIRST_REG; i <= LAX_INPUT_PARITY_LAST_REG; i++)
    {
        statParity = LAX_INPUT_REG_PTR->GP_OUT[i].R;
    statParity |= GP_OUT_REG_PARITY_FAIL_CLEAR;
        LAX_INPUT_REG_PTR->GP_OUT[i].R = statParity;
    statParity &= (~GP_OUT_REG_PARITY_FAIL_CLEAR);
        LAX_INPUT_REG_PTR->GP_OUT[i].R = statParity;
    }
}


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
/*
 * The OAL communication server dispatcher
 */
static uint32_t OalCommDispatcher(oal_dispatcher_t *d, uint32_t func, uintptr_t in, int32_t len)
{
    rsdkStatus_t ret;
    uint32_t laxId;
    (void)d;

    ret = RSDK_SUCCESS;

    // used only for Linux; for sa all necessary procedures will be called directly
#ifdef LAX_OS_sa
    OAL_UNUSED_ARG(d);
    OAL_UNUSED_ARG(func);
    OAL_UNUSED_ARG(in);
    OAL_UNUSED_ARG(len);
    OAL_UNUSED_ARG(laxId);
#else
    switch (func) 
    {  
        case (uint32_t)RSDK_LAX_UAPI_DMA_LAX0:
        case (uint32_t)RSDK_LAX_UAPI_DMA_LAX1:
        {
            laxId = func - (uint32_t)RSDK_LAX_UAPI_DMA_LAX0;
            LAX_LOG_DEBUG("lax%d: RSDK_LAX_UAPI_DMA\n", laxId);
            if ((uint32_t)len != sizeof(struct laxDmaReq))
            {
                LAX_LOG_ERROR("lax%d: RSDK_LAX_UAPI_DMA incorrect len \n", laxId);
                ret = RSDK_LAX_ERR_OAL_COMM_DISPATCH;
                break;
            }
            ret = LaxDmaRequest(gOalCommLaxCtrl[laxId], (struct laxDmaReq *)in);
            break;
        }
        case (uint32_t)RSDK_LAX_UAPI_TRIGGER_EVENT:
        {
            LAX_LOG_DEBUG("RSDK_LAX_UAPI_TRIGGER_EVENT\n");
            if ((uint32_t)len != sizeof(rsdkLaxEventType_t))
            {
                LAX_LOG_ERROR("RSDK_LAX_UAPI_TRIGGER_EVENT incorrect len \n");
                ret = RSDK_LAX_ERR_OAL_COMM_DISPATCH;
                break;
            }
            ret = LaxTriggerEvent(*((rsdkLaxEventType_t*)in));
            break;
        }
        case (uint32_t)RSDK_LAX_UAPI_REGISTER_EVENTS: 
        {
            LAX_LOG_DEBUG("RSDK_LAX_UAPI_REGISTER_EVENTS\n");
            if ((uint32_t)len != sizeof(rsdkLaxEventType_t))
            {
                LAX_LOG_ERROR("RSDK_LAX_UAPI_REGISTER_EVENTS incorrect len \n");
                ret = RSDK_LAX_ERR_OAL_COMM_DISPATCH;
                break;
            }
            ret = LaxRegisterEvents(*((rsdkLaxEventType_t*)in));
            break;
        }
        case (uint32_t)RSDK_LAX_UAPI_DEREGISTER_EVENTS: 
        {
            LAX_LOG_DEBUG("RSDK_LAX_UAPI_DEREGISTER_EVENTS\n");
            if ((uint32_t)len != sizeof(rsdkLaxEventType_t))
            {
                LAX_LOG_ERROR("RSDK_LAX_UAPI_DEREGISTER_EVENTS incorrect len \n");
                ret = RSDK_LAX_ERR_OAL_COMM_DISPATCH;
                break;
            }
            ret = LaxDeregisterEvents(*((rsdkLaxEventType_t*)in));
            if (ret != RSDK_SUCCESS)
            {
                ret = RSDK_LAX_ERR_OAL_COMM_DISPATCH;
            }
            break;
        }
        default:
        {
            ret = RSDK_LAX_ERR_OAL_COMM_DISPATCH;      // unknown request
            break;
        }
    }
#endif  // #ifdef LAX_OS_sa

    return (uint32_t)ret;
}



/************* DMA Functions **************************************************/

static uint8_t DmaNextIndex(uint8_t curr)
{
    return ((curr == ((uint8_t)DMA_QUEUE_ENTRIES - (uint8_t)1)) ? (uint8_t)0 : (uint8_t)(curr + (uint8_t)1));
}

static rsdkStatus_t DmaResetQueue(lldLaxControl_t *pLaxCtrl)
{
    uint64_t        irqflags;
    rsdkStatus_t    ret = RSDK_SUCCESS;

    if(0 != OAL_spin_lock_irqsave(&pLaxCtrl->dmaTxQueueLock, &irqflags))
    {
        ret = RSDK_LAX_ERR_RET_OAL;
    }
    else
    {
        pLaxCtrl->dmaQueue.idxDma = 0;
        if(0 != OAL_spin_unlock_irqrestore(&pLaxCtrl->dmaTxQueueLock, &irqflags))
        {
            ret = RSDK_LAX_ERR_RET_OAL;
        }
    }

    if(0 != OAL_spin_lock(&pLaxCtrl->dmaEnqueueLock))
    {
        ret = RSDK_LAX_ERR_RET_OAL;
    }
    else
    {
        pLaxCtrl->dmaQueue.idxQueue = 0;
        pLaxCtrl->dmaQueue.idxChk = 0;
        if(0 != OAL_spin_unlock(&pLaxCtrl->dmaEnqueueLock))
        {
            ret = RSDK_LAX_ERR_RET_OAL;
        }
    }

    return ret;
}


/* sometimes called from IRQ handler */
static rsdkStatus_t DmaTransmit(lldLaxControl_t *pLaxCtrl)
{
    rsdkStatus_t        ret;
    dmaQueue_t          *pDmaQueue;
    struct laxDmaReq    *pDr;
    uint64_t            irqflags;

    pDmaQueue = &(pLaxCtrl->dmaQueue);
    ret       = RSDK_SUCCESS;

    irqflags  = 0;
    if(0 != OAL_spin_lock_irqsave(&pLaxCtrl->dmaTxQueueLock, &irqflags))
    {
        ret = RSDK_LAX_ERR_RET_OAL;
    }
    else
    {
        pDr = &(pDmaQueue->entry[pDmaQueue->idxDma]);

        if (pDmaQueue->idxDma == pDmaQueue->idxQueue) /* Queue is empty */
        {
            ret = RSDK_LAX_ERR_DMA_ENOMSG;
        }
        else if (pDmaQueue->idxDma != pDmaQueue->idxChk) /* DMA is in process */
        {
            ret = RSDK_LAX_ERR_DMA_EBUSY;
        }
        else
        {
            /* Update the queue */
            pDmaQueue->idxDma = DmaNextIndex(pDmaQueue->idxDma);

            /* Program the DMA transfer */
                LAX_DMA_CTRL_REG_PTR->DMA_DMEM_PRAM_ADDR.R = pDr->dmemAddr;
                LAX_DMA_CTRL_REG_PTR->DMA_AXI_ADDRESS.R = (uint32_t)pDr->axiAddr;
                LAX_DMA_CTRL_REG_PTR->DMA_AXI_BYTE_CNT.R = pDr->byteCnt;
                LAX_DMA_CTRL_REG_PTR->DMA_XFR_CTRL.R = pDr->xfrCtrl;
        }
        if(0 != OAL_spin_unlock_irqrestore(&pLaxCtrl->dmaTxQueueLock, &irqflags))
        {
           ret = RSDK_LAX_ERR_RET_OAL;
        }
    }

    return ret;
}

static rsdkStatus_t DmaEnqueue(lldLaxControl_t *pLaxCtrl, struct laxDmaReq *pDmaReq)
{
    rsdkStatus_t     ret;
    uint8_t     nextSlot;
    dmaQueue_t  *pDmaQueue;
    struct laxDmaReq    *pDr;

    pDmaQueue = &(pLaxCtrl->dmaQueue);

    if(0 != OAL_spin_lock(&pLaxCtrl->dmaEnqueueLock))
    {
        ret = RSDK_LAX_ERR_RET_OAL;
    }
    else
    {
        nextSlot = DmaNextIndex(pDmaQueue->idxQueue);
        if (nextSlot == pDmaQueue->idxChk) /* Queue is full */
        {
            ret = RSDK_LAX_ERR_DMA_QUEUE_FULL;
        }
        else
        {
            ret = RSDK_SUCCESS;
            pDr = &(pDmaQueue->entry[pDmaQueue->idxQueue]);
            IF_LAX_DRV_DEBUG(DEBUG_DMA)
            {
                LAX_LOG_INFO("DMA Enqueue(%d), bytecnt=%d\n", pDmaQueue->idxQueue, pDmaReq->byteCnt);
            }
            pDr->control    = pDmaReq->control;
            pDr->dmemAddr   = pDmaReq->dmemAddr;
            pDr->axiAddr    = pDmaReq->axiAddr;
            pDr->byteCnt    = pDmaReq->byteCnt;

            pDr->xfrCtrl    = pDmaReq->xfrCtrl | (uint32_t)0x4000; /* IRQ_EN */
            pDmaQueue->chan = (uint8_t)(pDr->xfrCtrl & (uint32_t)0x1F);

            IF_LAX_DRV_DEBUG(DEBUG_DMA)
            {
                LAX_LOG_INFO("lax%d dma: %08X %08X %016llX %08X %08X\n",
                    pLaxCtrl->id, pDr->control, pDr->dmemAddr,
                    (long long)pDr->axiAddr, pDr->byteCnt, pDr->xfrCtrl);
            }
            /* barrier */
            pDmaQueue->idxQueue = nextSlot;
        }

        if(0 != OAL_spin_unlock(&pLaxCtrl->dmaEnqueueLock))
        {
            ret = RSDK_LAX_ERR_RET_OAL;
        }
    }

    //TODO: review for using the return value
    (void)DmaTransmit(pLaxCtrl);

    return ret;
}


/************************ IRQ handlers ****************************************/

// Command has been consumed
static void LaxFlags0IrqHandler(lldLaxControl_t *pLaxCtrl)
{

    uint32_t    flags0;
    uint32_t    cmdMask;

    flags0 = LAX_VCPU_HOST_REG_PTR->VCPU_HOST_FLAGS[0].R;
    LAX_VCPU_HOST_REG_PTR->VCPU_HOST_FLAGS[0].R = flags0;

    IF_LAX_DRV_DEBUG(DEBUG_FLAGS0_IRQ)
    {
        LAX_LOG_INFO("lax%d: flags0 = %08x, status = %08x\n", pLaxCtrl->id, flags0, LAX_VCPU_REG_PTR->STATUS.R);
    }

    // Handle Flags0 interrupts
    while (flags0 != 0U)
    {
        uint32_t     seqId; // as flags0 is non-zero, ffs always returns values >=1 
        seqId = ((uint32_t)ffs((int32_t)flags0)) - 1U;
        cmdMask = RSDK_LAX_CMD_DOUBLE_IDX_BITS << seqId;

        if(((flags0 & cmdMask) == cmdMask) && (seqId < RSDK_LAX_MAX_CMDS_NUM))
        {
            // trigger associated event, for every completed command
            if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[(RSDK_LAX_MAX_CMDS_NUM * (uint32_t)pLaxCtrl->id) + seqId]) != 0)
            {
                LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for seqId %d \n", pLaxCtrl->id, seqId);
            }
        }
        else
        {
            if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_REGIF_ERR]) != 0)
            {
                LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for potential random fault %d \n", pLaxCtrl->id, seqId);
            }
        }
        flags0 &= ~(cmdMask);
    }
}


static void LaxFlags1IrqHandler(lldLaxControl_t *pLaxCtrl)
{
    uint32_t flags1, cmdMask, eventId, eventIdOffset;
    uint32_t const  validFlags1 = (RSDK_LAX_HOST_FLAGS1_CMD_ERR_MASK | RSDK_LAX_HOST_FLAGS1_TABLE_ERR_MASK |
                                RSDK_LAX_HOST_FLAGS1_SPT_CMD_MASK | RSDK_LAX_HOST_FLAGS1_CTE_CMD_MASK |
                                RSDK_LAX_HOST_FLAGS1_LAX_CMD_MASK | RSDK_LAX_HOST_FLAGS1_PARITY_ERR_MASK);
    uint32_t const middleBit = 16U;

    flags1 = LAX_VCPU_HOST_REG_PTR->VCPU_HOST_FLAGS[1].R;
    LAX_VCPU_HOST_REG_PTR->VCPU_HOST_FLAGS[1].R = flags1;

    IF_LAX_DRV_DEBUG(DEBUG_FLAGS1_IRQ)
    {
        LAX_LOG_INFO("lax%d: flags1 = %08x, status = %08x\n", pLaxCtrl->id, flags1, LAX_VCPU_GO_REG_PTR->EXT_GO_STAT.R);
    }

    if ((flags1 & (~validFlags1)) != (uint32_t)0)
    {
        flags1 &= validFlags1;    
        if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_UNEXPECTED_INT]) != 0)
        {
            LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for unexpected interrupt \n", pLaxCtrl->id);
            // TODO - report error event
        }        
    }

    if ((flags1 & RSDK_LAX_HOST_FLAGS1_CMD_ERR_MASK) != (uint32_t)0)
    {
        flags1 &= (~(RSDK_LAX_HOST_FLAGS1_CMD_ERR_MASK));
        if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_CMD_CONTENT_ERR]) != 0)
        {
            LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for LAX command content error \n", pLaxCtrl->id);
            // TODO - report error event
        }
    }

    if ((flags1 & RSDK_LAX_HOST_FLAGS1_TABLE_ERR_MASK) != (uint32_t)0)
    {
        flags1 &= (~(RSDK_LAX_HOST_FLAGS1_TABLE_ERR_MASK));
        if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_TABLE_ERR]) != 0)
        {
            LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for graph table error \n", pLaxCtrl->id);
            // TODO - report error event
        }
    }

    if ((flags1 & RSDK_LAX_HOST_FLAGS1_PARITY_ERR_MASK) != (uint32_t)0)
    {
        flags1 &= (~(RSDK_LAX_HOST_FLAGS1_PARITY_ERR_MASK));
        if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_PARITY_ERR]) != 0)
        {
            LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for LAX parity error \n", pLaxCtrl->id);
            // TODO - report error event ???
        }
        LaxClearAllParityFailBits(pLaxCtrl);
    }
        
    // Handle Flags1 interrupts for OtherLAX/CTE/SPT-triggered commands completions
    while (flags1 != 0U)
    {   
        uint32_t     bitId; // as flags1 is non-zero, ffs always returns values >=1 
        bitId = ((uint32_t)ffs((int32_t)flags1)) - 1U;
        cmdMask = RSDK_LAX_CMD_DOUBLE_IDX_BITS << bitId;
        if(((cmdMask & flags1) == cmdMask) && (bitId < middleBit))
        {
            if (bitId < RSDK_LAX_CTE_INT_OFFSET)
            {  //this is a SPT-triggered command completion
                eventIdOffset = (pLaxCtrl->id == (int32_t)RSDK_LAX_CORE_0_ID) ?
                    (uint32_t)RSDK_LAX_EVENT_LAX0_SPT_CMD_0_DONE : (uint32_t)RSDK_LAX_EVENT_LAX1_SPT_CMD_0_DONE;
                eventId = eventIdOffset + bitId;
            }
            else if (bitId < RSDK_LAX_LAX_INT_OFFSET)
            {  //this is a CTE-triggered command completion
                eventIdOffset = (pLaxCtrl->id == (int32_t)RSDK_LAX_CORE_0_ID) ?
                    (uint32_t)RSDK_LAX_EVENT_LAX0_CTE_CMD_0_DONE : (uint32_t)RSDK_LAX_EVENT_LAX1_CTE_CMD_0_DONE;
                eventId = eventIdOffset + (bitId - RSDK_LAX_CTE_INT_OFFSET);                
            }
            else
            { //this is a LAX-triggered command completion
                eventIdOffset = (pLaxCtrl->id == (int32_t)RSDK_LAX_CORE_0_ID) ?
                    (uint32_t)RSDK_LAX_EVENT_LAX0_LAX_CMD_0_DONE : (uint32_t)RSDK_LAX_EVENT_LAX1_LAX_CMD_0_DONE;
                eventId = eventIdOffset + (bitId - RSDK_LAX_LAX_INT_OFFSET);                    
            }

            // notify OtherLAX/CTE/SPT-triggered command completion
            if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[eventId]) != 0)
            {
                LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for CTE/SPT cmd completion \n", pLaxCtrl->id);
                // TODO - report error event
            }
        }
        else
        {
            if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_REGIF_ERR]) != 0)
            {
                LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for potential random fault %d \n", pLaxCtrl->id, seqId);
            }
        }
        flags1 &= (~cmdMask);
    }
}


static void LaxIllegalopIrqHandler(lldLaxControl_t *pLaxCtrl)
{
    uint32_t    status;

    status = LAX_VCPU_REG_PTR->STATUS.R;
    status |= STATUS_REG_IRQ_ILLEGALOP;
    /* W1C write 1 to clear vcpu_iit bit */
    LAX_VCPU_REG_PTR->STATUS.R = status;
    
    if(pLaxCtrl->id == (int32_t)RSDK_LAX_CORE_0_ID)
    {
        if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_LAX0_ILLEGALOP]) != 0)
        {
            LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for RSDK_LAX_EVENT_LAX0_ILLEGALOP \n", pLaxCtrl->id);
        }
    }
    else
    {
        if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_LAX1_ILLEGALOP]) != 0)
        {
            LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for RSDK_LAX_EVENT_LAX1_ILLEGALOP \n", pLaxCtrl->id);
        }
    }
}

/**
* @brief       Handle DMA interrupts from LAX and notify the user space about DMA transfer completions or errors
* @details          There are 3 types of interrupts, each having their corresponding actions:
*                   - DMA transfer complete interrupts notify that a DMA transfer has completed successfully
*                               - RSDK_LAX_EVENT_DMA_DONE user-space event is generated for elf download
*                               - no user-space event is generated for LAX command launches
*                               - RSDK_LAX_EVENT_UNEXP_DMA_COMP user-space event is generated 
*                                 in case DMA transfer completion is observed on other DMA channels
*                   - DMA configuration errors notifying that the DMA configuration was incorrect for any channel 
*                     (either used for eld download, LAX command launch or data buffer transfer in a LAX graph); 
*                     in this case, the user-space event generated is RSDK_LAX_EVENT_DMA_FLAG_CFGERR
*                   - DMA transfer errors notifying that the DMA transfer failed for any channel 
*                     (either used for eld download, LAX command launch or data buffer transfer in a LAX graph); 
*                     RSDK_LAX_EVENT_DMA_FLAG_XFRERR is the user-space event generated in this case
*
*                   Note: isQueuedDmaReq flag is used to identify interrupts resulting from 
*                   DMA transfers originating from the driver’sDMA Request Queue 
*                   and thus DMATransmit should be called to process the next request in the queue
*
* @param[in]        pLaxCtrl    Pointer to lldLaxControl_t structure
*
* @return           void
*/
static void LaxDmaIrqHandler(lldLaxControl_t *pLaxCtrl)
{
    dmaQueue_t  *pDmaQueue;
    struct laxDmaReq *pDr;
    uint32_t    status, statR, mask, statParity;
    uint32_t isQueuedDmaReq = 0U; 

    pDmaQueue = &(pLaxCtrl->dmaQueue);

    status = LAX_VCPU_REG_PTR->STATUS.R;
    IF_LAX_DRV_DEBUG(DEBUG_DMA_IRQ)
    {
        LAX_LOG_INFO("lax%d: dma_irq %08x, COMP %08x, XFRERR %08x, CFGERR %08x\n",
                pLaxCtrl->id,
                LAX_DMA_CTRL_REG_PTR->DMA_IRQ_STAT.R,
                LAX_DMA_CTRL_REG_PTR->DMA_COMP_STAT.R,
                LAX_DMA_CTRL_REG_PTR->DMA_XFRERR_STAT.R,
                LAX_DMA_CTRL_REG_PTR->DMA_CFGERR_STAT.R);
    }
    pDr = &(pDmaQueue->entry[pDmaQueue->idxChk]);
    mask = ((uint32_t)0x1U);
    mask <<= ((pDr->type == (uint8_t)LAX_DMA_REQ_ELD) ? RSDK_LAX_DMA_ELD_CHANNEL : RSDK_LAX_DMA_CMD_CHANNEL);

    // Check for completed DMAs
    if ((status & (uint32_t)STATUS_REG_IRQ_DMA_COMP)  != (uint32_t)0)
    {
        statR = LAX_DMA_CTRL_REG_PTR->DMA_IRQ_STAT.R;
        if ((statR & mask)  != (uint32_t)0)
        {
            isQueuedDmaReq = (statR & mask);
            LAX_DMA_CTRL_REG_PTR->DMA_IRQ_STAT.R = mask;
            LAX_DMA_CTRL_REG_PTR->DMA_COMP_STAT.R = mask;
            statR &= ~mask;
            if (pDr->type == (uint8_t)LAX_DMA_REQ_ELD)
            {
                if(OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_DMA_DONE]) != 0)
                {
                     LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for RSDK_LAX_EVENT_DMA_DONE \n", pLaxCtrl->id);
                }
            }
            //Check parity error bits
            statParity = 0U;
            if ((pDr->type == (uint8_t)LAX_DMA_REQ_ELD) || (pDr->type == (uint8_t)LAX_DMA_REQ_CMD))
            {
                statParity = LAX_INPUT_REG_PTR->GP_IN[1].R;
            }
            if((statParity & GP_IN_PARRITY_ERROR_MASK) != (uint32_t)0)
            {
                if(OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_PARITY_ERR]) != 0)
                {
                     LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for RSDK_LAX_EVENT_PARITY_ERR \n", pLaxCtrl->id);
                }
                LaxClearAllParityFailBits(pLaxCtrl);
            }
        }
        // Watch for completed DMAs from LAX with incorrect IRQ_EN
        if (statR != (uint32_t)0)
        {
            LAX_LOG_ERROR("%d: DMA IRQ STAT %08x\n", pLaxCtrl->id, statR);
            if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_UNEXP_DMA_COMP]) != 0)
            {
                LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for RSDK_LAX_EVENT_UNEXP_DMA_COMP \n", pLaxCtrl->id);
            }
            LAX_DMA_CTRL_REG_PTR->DMA_IRQ_STAT.R = statR;
        }
    }
    // Check for DMA errors from any channel
    if ((status & STATUS_REG_IRQ_DMA_ERR)  != (uint32_t)0)
    {
        // DMA transfer errors
        statR = LAX_DMA_CTRL_REG_PTR->DMA_XFRERR_STAT.R;
        if (statR != (uint32_t)0) /* Transfer error from DMA channel */
        {
            isQueuedDmaReq = (statR & mask);
            if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_DMA_FLAG_XFRERR]) != 0)
            {
                LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for RSDK_LAX_EVENT_DMA_FLAG_XFRERR \n", pLaxCtrl->id);
            }
            LAX_DMA_CTRL_REG_PTR->DMA_XFRERR_STAT.R = statR;
        }
        // DMA configuration errors
        statR = LAX_DMA_CTRL_REG_PTR->DMA_CFGERR_STAT.R;
        if (statR != (uint32_t)0) // Config error from DMA channel
        {
            isQueuedDmaReq = (statR & mask);
            if (OAL_RPCTriggerEvent(gsRsdkLaxEvents[RSDK_LAX_EVENT_DMA_FLAG_CFGERR]) != 0)
            {
                LAX_LOG_ERROR("%d: OAL_RPCTriggerEvent failed for RSDK_LAX_EVENT_DMA_FLAG_CFGERR \n", pLaxCtrl->id);
            }

            LAX_DMA_CTRL_REG_PTR->DMA_CFGERR_STAT.R = statR;
        }
    }

    if(isQueuedDmaReq != 0U)
    {
        pDmaQueue->idxChk = pDmaQueue->idxDma;
        DmaTransmit(pLaxCtrl);
    }
}


static void LaxGenIrqHandler(lldLaxControl_t *pLaxCtrl)
{
    uint32_t    status;

    status = LAX_VCPU_REG_PTR->STATUS.R;

    LAX_VCPU_REG_PTR->STATUS.R = status;
}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/



#ifndef LAX_OS_sa
static
#endif
rsdkStatus_t LaxDmaRequest(lldLaxControl_t *pLaxCtrl, struct laxDmaReq * pDmaReq)
{
    rsdkStatus_t     ret;
    uint32_t    alignBytes32, chan;
    uint64_t    alignBytes64;
    
    ret = RSDK_SUCCESS;
    alignBytes32 = (uint32_t)PS_AXI_BUS_WIDTH_BITS >> (uint8_t)3;
    alignBytes64 = (uint64_t)PS_AXI_BUS_WIDTH_BITS >> (uint8_t)3;

    if (((pDmaReq->dmemAddr & (alignBytes32 - (uint32_t)1)) != (uint32_t)0) ||
                    ((pDmaReq->axiAddr & (alignBytes64 - (uint32_t)1)) != (uint32_t)0))
    {
        ret = RSDK_LAX_ERR_EINVAL;
    } 
    else 
    {
        chan = (pDmaReq->type == (uint8_t)LAX_DMA_REQ_ELD) ? RSDK_LAX_DMA_ELD_CHANNEL : RSDK_LAX_DMA_CMD_CHANNEL;
        pDmaReq->xfrCtrl = (pDmaReq->xfrCtrl & ~((uint32_t)0x1F)) | chan;

        ret = DmaEnqueue(pLaxCtrl, pDmaReq);
    }
    return ret;
}


rsdkStatus_t LaxLowLevelDriverInit(lldLaxControl_t *pLaxCtrl)
{
    uint32_t    param0, param1, param2;
    struct laxHardware *pHw;
    rsdkStatus_t ret = RSDK_SUCCESS;
    
    /* Map LAX registers */
    pLaxCtrl->pRegs = OAL_memmap((uintptr_t)pLaxCtrl->pMemAddr, pLaxCtrl->memSize, KERNEL_MAP);
    pLaxCtrl->pDbgRegs = (uint32_t*)OAL_memmap((uintptr_t)pLaxCtrl->pDbgAddr, pLaxCtrl->dbgSize, KERNEL_MAP);

    pHw = &pLaxCtrl->hardware;
    param0 = LAX_VERS_CFG_REG_PTR->PARAM0.R;
    pHw->param0           = param0;
    param1 = LAX_VERS_CFG_REG_PTR->PARAM1.R;
    pHw->param1           = param1;
    pHw->dmaChannels      = (param1 >> 16U) & (uint32_t)0xFF;
    pHw->gpOutRegs        = (param1 >> 8U) & (uint32_t)0xFF;
    pHw->gpInRegs         = param1 & (uint32_t)0xFF;
    param2 = LAX_VERS_CFG_REG_PTR->PARAM2.R;
    pHw->param2           = param2;
    pHw->dmemBytes        = (uint32_t)((param2 >> 8U) & (uint32_t)0x3FF) * (uint32_t)400;
    pHw->ippuBytes        = (param2 >> 31U) * (uint32_t)4096;
    pHw->arithmeticUnits  = param2 & (uint32_t)0xFF;

    pLaxCtrl->versions.laxHwVersion = LAX_VERS_CFG_REG_PTR->HWVERSION.R;
    pLaxCtrl->versions.ippuHwVersion = LAX_IPPU_REG_PTR->IPPUHWVER.R;
    
    /* Disbale all irqs of LAX (before IRQ registration) */
    LAX_VCPU_REG_PTR->IRQEN.R = 0u;

    if ((0 != OAL_irqspin_lock_init(&pLaxCtrl->dmaTxQueueLock)) ||
        (0 != OAL_spin_lock_init(&pLaxCtrl->dmaEnqueueLock)))
    {
        ret = RSDK_LAX_ERR_RET_OAL;
    }
    else
    {
        ret= DmaResetQueue(pLaxCtrl);
    }

#ifdef PRINTF_ALLOWED
    IF_LAX_DRV_DEBUG(DEBUG_INIT)
    {
        LAX_LOG_INFO("lax%d: hwver 0x%08x, %d AUs, dmem %d bytes\n",
                pLaxCtrl->id, LAX_VERS_CFG_REG_PTR->HWVERSION.R,
                pHw->arithmeticUnits, pHw->dmemBytes);
    }
#endif

    return ret;
}

rsdkStatus_t LaxDeInit(lldLaxControl_t *pLaxCtrl)
{
    rsdkStatus_t    ret = RSDK_SUCCESS;
    if(0 != OAL_memunmap((uintptr_t)pLaxCtrl->pRegs, pLaxCtrl->memSize, KERNEL_MAP))
    {
        ret = RSDK_LAX_ERR_RET_OAL;
    }

    if(0 != OAL_memunmap((uintptr_t)pLaxCtrl->pDbgRegs, pLaxCtrl->dbgSize, KERNEL_MAP))
    {
        ret = RSDK_LAX_ERR_RET_OAL;
    }
    return ret;
}

OAL_IRQ_HANDLER(LaxIrqHandler)
{
    uint32_t    status;
    lldLaxControl_t *pLaxCtrl;
    OAL_UNUSED_ARG(irq_no);

    pLaxCtrl = (lldLaxControl_t*)irq_data;
    status = LAX_VCPU_REG_PTR->STATUS.R;

    if ((status & STATUS_REG_IRQ_NON_GEN) != (uint32_t)0)
    {
        if ((status & STATUS_REG_IRQ_FLAGS0) != (uint32_t)0)
        {
            LaxFlags0IrqHandler(pLaxCtrl);
        }
        if ((status & STATUS_REG_IRQ_FLAGS1) != (uint32_t)0)
        {
            LaxFlags1IrqHandler(pLaxCtrl);
        }
        if (((status & STATUS_REG_IRQ_DMA_COMP) != (uint32_t)0) || ((status & STATUS_REG_IRQ_DMA_ERR) != (uint32_t)0))
        {
            LaxDmaIrqHandler(pLaxCtrl);
        }
        if ((status & STATUS_REG_IRQ_ILLEGALOP) != (uint32_t)0)
        {
            LaxIllegalopIrqHandler(pLaxCtrl);
        }
    } 
    else
    {
        LaxGenIrqHandler(pLaxCtrl);
    }

    return OAL_IRQ_HANDLED;
}

/************* LAX events manipulation ****************************************/
/**
* @brief        Trigger the specified event in low-level driver
* @param[in]    Trigger the event
* @return       The execution result : RSDK_SUCCESS or error: RSDK_LAX_ERR_OAL_EVENT_TRIGGER
*/
#ifndef LAX_OS_sa
static
#endif
rsdkStatus_t LaxTriggerEvent(rsdkLaxEventType_t evtId)
{
    rsdkStatus_t ret = RSDK_SUCCESS;

    if (0 != OAL_RPCTriggerEvent(gsRsdkLaxEvents[(uint32_t)evtId]))
    {
        ret = RSDK_LAX_ERR_OAL_EVENT_TRIGGER;
    }
    return ret;
}
/*LaxTriggerEvent()===========================================================*/


/**
* @brief        Initiate event registation in low-level driver
* @param[in]    Register events up to lastEvt
* @return       The execution result : RSDK_SUCCESS or error RSDK_LAX_ERR_OAL_EVENT_REGISTER
 */
#ifndef LAX_OS_sa
static
#endif
rsdkStatus_t LaxRegisterEvents(rsdkLaxEventType_t lastEvt)
{
    uint32_t i;
    rsdkStatus_t ret = RSDK_SUCCESS;

    //register the driver events 
    for (i = 0; i < (uint32_t)lastEvt; i++)
    {
        if (OAL_RPCRegisterEvent(gsOalCommServ, i,&gsRsdkLaxEvents[i]) != 0)
        {
            ret = RSDK_LAX_ERR_OAL_EVENT_REGISTER;
            break;
        }
    }
    return ret;
}
/*LaxRegisterEvents()=========================================================*/



/**
* @brief        Initiate event deregistation in low-level driver
* @param[in]    Deregister events up to lastEvt
* @return       The execution result : RSDK_SUCCESS or error: RSDK_LAX_ERR_OAL_EVENT_DEREGISTER
*/
#ifndef LAX_OS_sa
static
#endif
rsdkStatus_t LaxDeregisterEvents(rsdkLaxEventType_t lastEvt)
{
    uint32_t i;
    rsdkStatus_t ret = RSDK_SUCCESS;

    //deregister the driver events 
    for (i = 0; i < (uint32_t)lastEvt; i++)
    {
        if (OAL_RPCDeregisterEvent(gsRsdkLaxEvents[i]) != 0)
        {
            ret = RSDK_LAX_ERR_OAL_EVENT_DEREGISTER;
        }
    }
    return ret;
}
/*LaxDeregisterEvents()=======================================================*/



rsdkStatus_t LaxOalCommInit(void)
{
    rsdkStatus_t ret;
    static int32_t sInitDone = 0;

    ret = RSDK_SUCCESS;
    if (sInitDone == 0)
    {
        gsOalCommServ = OAL_RPCRegister(RSDK_LAX_OAL_COMM_SERV, OalCommDispatcher);
        sInitDone = 1;
        if (gsOalCommServ == NULL)
        {
            ret = RSDK_LAX_ERR_OAL_COMM_INIT;
        }
    }
    return ret;
}

rsdkStatus_t LaxOalCommExit(void)
{
    rsdkStatus_t ret = RSDK_SUCCESS;

    if(0 != OAL_RPCCleanup(gsOalCommServ))
    {
        ret = RSDK_LAX_ERR_OAL_COMM_EXIT;
    }
    return ret;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

