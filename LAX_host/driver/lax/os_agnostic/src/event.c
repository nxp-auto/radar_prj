/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
 /**
* @file           event.c
* @brief          Implementation of events handling
*/
#include <oal_utils.h>
#include <oal_waitqueue.h>
#include <oal_uptime.h>

#include <uapi/linux/vspa_uapi.h>
#include "event.h"
#include "lax_driver.h"

/************************** Event Queue *******************************/
static inline bool EventsPresentCmdExt(vspaEventList_t *pVspaList, uint32_t mask, int cmdId )
{
    uint32_t lMask = pVspaList->eventListMask;
    uint32_t lCmds = pVspaList->eventListCmds;
    eventQueue_t *pEventQueue = &(pVspaList->eventQueue);

    if (pEventQueue->idxEnqueue != pEventQueue->idxDequeue) /* Queue is not empty */{
        lMask |= pVspaList->eventQueueMask;
        lCmds |= pVspaList->eventQueueCmds;
    }
    if (mask != VSPA_MSG_CMD)
        return (lMask & mask);
    return ((1 << cmdId) & lCmds);
}

static inline bool WaitCond(vspaEventList_t *pVspaList, uint32_t mask, int cmdId ){
    uint32_t lMask = pVspaList->eventListMask;
    uint32_t lCmds = pVspaList->eventListCmds;
    //pr_info("WaitCond(): pVspaList->eventListCmds = %d, (mask == VSPA_MSG_CMD)=%d, cmdId=%d\n", lCmds, mask == VSPA_MSG_CMD, cmdId);
    if ((lMask & mask) == 0)
        return true;
    if (mask == VSPA_MSG_CMD && ((1 << cmdId) & lCmds) == 0)
        return true;
    return false;
}

static inline uint32_t EventsPresent(vspaEventList_t *pVspaList)
{
    uint32_t mask = pVspaList->eventListMask;
    eventQueue_t *pEventQueue = &(pVspaList->eventQueue);

    if (pEventQueue->idxEnqueue != pEventQueue->idxDequeue) /* Queue is not empty */
        mask |= pVspaList->eventQueueMask;
    return mask;
}

static inline int EventNextIndex(int curr)
{
    return (curr == (EVENT_QUEUE_ENTRIES - 1)) ? 0 : curr + 1;
}

void EventResetQueue(vspaEventList_t *pVspaList)
{
    int i;
    unsigned long irqFlags;

    OAL_spin_lock_irqsave(&pVspaList->eventQueueLock, &irqFlags);
    pVspaList->eventQueueMask = 0;
    pVspaList->eventQueueCmds = 0;
    pVspaList->eventQueue.idxEnqueue = 0;
    pVspaList->eventQueue.idxDequeue = 0;
    pVspaList->eventQueue.idxQueued = 0;
    OAL_spin_unlock_irqrestore(&pVspaList->eventQueueLock, &irqFlags);

    OAL_spin_lock(&pVspaList->eventListLock);
    pVspaList->eventListMask = 0;
    pVspaList->eventListCmds = 0;
    LaxListInit(&pVspaList->eventsFreeList);
    LaxListInit(&pVspaList->eventsQueuedList);
    for (i = 0; i < EVENT_LIST_ENTRIES; i++)
        LaxListPush(&pVspaList->events[i].list,
             &pVspaList->eventsFreeList);
    OAL_spin_unlock(&pVspaList->eventListLock);
}

/* This routine is usually called from IRQ handlers */
void EventEnqueue(vspaEventList_t *pVspaList, uint8_t type,
    uint8_t id, uint8_t err, uint32_t data0, uint32_t data1)
{
    eventQueue_t *pEventQueue = &(pVspaList->eventQueue);
    eventEntry_t *pEventEntry;
    int nextSlot;
    unsigned long irqFlags;

    OAL_spin_lock_irqsave(&pVspaList->eventQueueLock, &irqFlags);

    if (pEventQueue->idxEnqueue == pEventQueue->idxDequeue) /* Queue is empty */{
        pVspaList->eventQueueMask = 0;
        pVspaList->eventQueueCmds = 0;
    }

    nextSlot = EventNextIndex(pEventQueue->idxEnqueue);

    if (nextSlot == pEventQueue->idxDequeue) {    /* Queue is full */
        pEventEntry = &(pEventQueue->entry[pEventQueue->idxQueued]);
        if (type > 7)
            type = 0;
        pEventEntry->lost |= 1 << type;
    } else {
        pEventEntry = &(pEventQueue->entry[pEventQueue->idxEnqueue]);
        pEventEntry->type  = type;
        pEventEntry->id    = id;
        pEventEntry->err   = err;
        pEventEntry->data0 = data0;
        pEventEntry->data1 = data1;
        pEventEntry->lost = 0;
        pEventQueue->idxQueued = pEventQueue->idxEnqueue;
        pEventQueue->idxEnqueue = nextSlot;

        pVspaList->eventQueueMask |= 0x10 << type;
        if ((0x10 << type) == VSPA_MSG_CMD){
            pVspaList->eventQueueCmds |= ((1 << id) & VSPA_CMD_ALL_IDS);
        }
        //pr_info("EventEnqueue(): type = %d, id = %d, pVspaList->eventQueueCmds=%d\n", type, id, pVspaList->eventQueueCmds);
        OAL_wake_up_interruptible(&pVspaList->eventWaitQ);
    }
    OAL_spin_unlock_irqrestore(&pVspaList->eventQueueLock, &irqFlags);
}

void EventListUpdate(vspaEventList_t *pVspaList)
{
    eventList_t     *pEvList;
    eventList_t     *pLastEvList;
    eventQueue_t    *pEventQueue = &(pVspaList->eventQueue);
    eventEntry_t    *pEventEntry;
    uint32_t mask = 1;

    if (pEventQueue->idxEnqueue == pEventQueue->idxDequeue)
        return;

    OAL_spin_lock(&pVspaList->eventListLock);

    if (LaxListIsEmpty(&pVspaList->eventsQueuedList))
        pLastEvList = NULL;
    else
        pLastEvList = (eventList_t *) LaxListGetEntry((&pVspaList->eventsQueuedList)->prev,
                       offsetof(eventList_t, list));
        mask = 0;

    while (pEventQueue->idxEnqueue != pEventQueue->idxDequeue) {
        /* TODO - check SPM for command error messages */
        pEventEntry = &(pEventQueue->entry[pEventQueue->idxDequeue]);
        /* coalese error events of the same type */
        if (pLastEvList &&
            pLastEvList->type == ((0x10<<VSPA_EVENT_ERROR)|VSPA_EVENT_ERROR) &&
            pEventEntry->type == VSPA_EVENT_ERROR &&
            pEventEntry->id == pLastEvList->id &&
            pEventEntry->id == VSPA_ERR_WATCHDOG) {
            pLastEvList->data0 = pEventEntry->data0;
            pLastEvList->data1++;
            IF_EVENT_DEBUG(DEBUG_EVENT)
                pr_info("vspa%d: co %04X %02X %02X %08X %08X\n",
                    EVENT_GET_DEV_ID(), pLastEvList->type, pLastEvList->id,
                    pLastEvList->err, pLastEvList->data0, pLastEvList->data1);
        } else {
            if (LaxListIsEmpty(&pVspaList->eventsFreeList)) {
                mask = 0;
                EVENT_ERR("%d: Event queue overflowed\n",
                                 EVENT_GET_DEV_ID());
                /* TODO - add lost events */
                pEvList = (eventList_t *) LaxListGetEntry((&pVspaList->eventsQueuedList)->next,
                            offsetof(eventList_t, list));
                LaxListMoveTail(&pEvList->list,
                         &pVspaList->eventsFreeList);
            }
            pEvList = (eventList_t *) LaxListGetEntry((&pVspaList->eventsFreeList)->next,
                           offsetof(eventList_t, list));
            pEvList->err   = pEventEntry->err;
            pEvList->id    = pEventEntry->id;
            pEvList->type  = (0x10 << pEventEntry->type) | pEventEntry->type;
            pEvList->data0 = pEventEntry->data0;
            pEvList->data1 = pEventEntry->data1;
            IF_EVENT_DEBUG(DEBUG_EVENT)
                pr_info("vspa%d: up %04X %02X %02X %08X %08X\n",
                    EVENT_GET_DEV_ID(), pEvList->type, pEvList->id,
                    pEvList->err, pEvList->data0, pEvList->data1);
            LaxListMoveTail(&pEvList->list,
                    &pVspaList->eventsQueuedList);
            pLastEvList = pEvList;
            pVspaList->eventListMask |= 0x10 << pEventEntry->type;
            if ((0x10 << pEventEntry->type) == VSPA_MSG_CMD){
                pVspaList->eventListCmds |= ((1 << pEventEntry->id) & VSPA_CMD_ALL_IDS);
            }
        }
        pEventQueue->idxDequeue = EventNextIndex(pEventQueue->idxDequeue);
    }

    /* update event list mask if needed */
    if (mask == 0) {

        pVspaList->eventListCmds = 0;
        pEvList = (eventList_t *) LaxListGetEntry((&pVspaList->eventsQueuedList)->next, offsetof(eventList_t, list));
        while(&pEvList->list != (&pVspaList->eventsQueuedList))
        {
            mask |= pEvList->type;
            if (pEvList->type & VSPA_MSG_CMD)
                pVspaList->eventListCmds |= ((1 << pEvList->id) & VSPA_CMD_ALL_IDS);
            pEvList = (eventList_t *) LaxListGetEntry((pEvList)->list.next, offsetof(eventList_t, list));
        }
        //pr_info("EventListUpdate(): pVspaList->eventListCmds =%d\n", pVspaList->eventListCmds);
        pVspaList->eventListMask = mask & VSPA_MSG_ALL_EVENTS;
    }

    OAL_spin_unlock(&pVspaList->eventListLock);
    /* TODO - report lost events */
}

static const int eventSize[16] = {
    0, // VSPA_EVENT_NONE
    0, // VSPA_EVENT_DMA
    0, // VSPA_EVENT_CMD
    8, // VSPA_EVENT_ERROR
    0,
    0,
    0,
    0,
    0,
    0
};

extern void SeqIdRelease(vspaControl_t *pVspaCtrl, int seqid);

int ReadEvent(vspaEventList_t *pVspaList, struct vspa_event_read *evtRd, struct vspa_event *pEvent)
{
    eventList_t *pEvList, *pEvList1;
    uint32_t *pPayload = &pEvent->data[0];
    int err = 0;
    unsigned int mask;
    int type, cmdId = -1;
    uint32_t srcLen;
    size_t length;
    listHead_t *pPrev;
    int timeout;
    unsigned long startTime, newTime;

    if (EVENT_GET_DEV_STATE() == VSPA_STATE_UNKNOWN)
        return -ENODATA;

    startTime = OAL_Uptime();

    EventListUpdate(pVspaList);

    /* If no filter is specified then match all message types */
    timeout = evtRd->timeout;
    if (timeout > 0)
    {
        timeout = OAL_nsec2ticks(timeout * MILLION);
    }
    /* If no filter is specified then match all message types */
    mask = evtRd->event_mask & VSPA_MSG_ALL;
    /*for command events, look for a specific command ID */
    if (mask == VSPA_MSG_CMD){
        cmdId = pEvent->id;

    }

    if ((mask & VSPA_MSG_ALL_EVENTS) == 0)
        mask |= VSPA_MSG_ALL_EVENTS;

    OAL_spin_lock(&pVspaList->eventListLock);
    while ( WaitCond(pVspaList, mask, cmdId) &&
        (EVENT_GET_DEV_STATE() > VSPA_STATE_POWER_DOWN) &&
        (EVENT_GET_DEV_STATE() != VSPA_STATE_RECOVERY)) {
        /* nothing to read */
        OAL_spin_unlock(&pVspaList->eventListLock);
        if (EVENT_GET_DEV_STATE() <= VSPA_STATE_POWER_DOWN ||
            EVENT_GET_DEV_STATE() == VSPA_STATE_RECOVERY )
            return -ENODATA;
        if (timeout == 0) /* non-blocking */
            return -EAGAIN;
        pr_debug("vspa%d sleep: mask %04X, elm %04X, ep %04X, st %d\n",
                EVENT_GET_DEV_ID(), mask, pVspaList->eventListMask,
                EventsPresent(pVspaList), EVENT_GET_DEV_STATE());
        if (timeout < 0) {
            err = OAL_wait_event_interruptible(pVspaList->eventWaitQ,
                ((EventsPresentCmdExt(pVspaList, mask, cmdId)) ||
                 (EVENT_GET_DEV_STATE() <= VSPA_STATE_POWER_DOWN) ||
                 (EVENT_GET_DEV_STATE() == VSPA_STATE_RECOVERY)));
            if (err < 0)
                return err;
        } else {
            err = OAL_wait_event_timeout(
                pVspaList->eventWaitQ,
                ((EventsPresentCmdExt(pVspaList, mask, cmdId)) ||
                 (EVENT_GET_DEV_STATE() <= VSPA_STATE_POWER_DOWN)   ||
                 (EVENT_GET_DEV_STATE() == VSPA_STATE_RECOVERY)),
                timeout);
            if (err < 0)
                return err;
            newTime = OAL_Uptime();
            timeout -= newTime - startTime;
            startTime = newTime;
            if (timeout < 0)
                timeout = 0;
        }

        pr_debug("vspa%d wakup: mask %04X, elm %04X, ep %04X, st %d\n",
                EVENT_GET_DEV_ID(), mask, pVspaList->eventListMask,
                EventsPresent(pVspaList), EVENT_GET_DEV_STATE());

        EventListUpdate(pVspaList);
        /* otherwise loop, but first reacquire the lock */
        OAL_spin_lock(&pVspaList->eventListLock);
    }
    /* Find first event that matches the mask */
    type = 0;
    pEvList = (eventList_t *) LaxListGetEntry((&pVspaList->eventsQueuedList)->next, offsetof(eventList_t, list));
    while (&pEvList->list != (&pVspaList->eventsQueuedList))
    {
        pr_debug("vspa%d: read %04X check %04X\n", EVENT_GET_DEV_ID(), mask,
                pEvList->type);
        if ((mask == VSPA_MSG_CMD) && (pEvList->type & mask) && (pEvList->id == cmdId)) {
            type = pEvList->type & 0xF;
            SeqIdRelease(CONTAINER_OF(pVspaList, vspaControl_t, vspaList), cmdId);
            break;
        }

        if ((mask != VSPA_MSG_CMD) && (pEvList->type & mask)) {
            type = pEvList->type & 0xF;
            break;
        }
        pEvList = (eventList_t *) LaxListGetEntry(pEvList->list.next, offsetof(eventList_t, list));
    }
    if (!type) {
        OAL_spin_unlock(&pVspaList->eventListLock);
        return (EVENT_GET_DEV_STATE() <= VSPA_STATE_POWER_DOWN) ||
                EVENT_GET_DEV_STATE() == VSPA_STATE_RECOVERY ? -ENODATA :
                                   -EAGAIN;
    }
    srcLen = 0;

    length = eventSize[type];
    pEvent->control  = pEvList->control;
    pEvent->type     = type;
    pEvent->pkt_size = length ? length : srcLen;
    pPayload[0]    = pEvList->data0;
    pPayload[1]    = pEvList->data1;

    srcLen = evtRd->buf_len - sizeof(*pEvent);
    if (srcLen > pEvent->pkt_size)
        srcLen = pEvent->pkt_size;
    if (evtRd->buf_len <  sizeof(*pEvent))
        srcLen = 0;
    pEvent->buf_size = srcLen;

    IF_EVENT_DEBUG(DEBUG_EVENT) {
        pr_info("vspa%d: evt_read %2d %2d %2d %2d %08X %08X\n",
            EVENT_GET_DEV_ID(), type, pEvent->err, pEvent->pkt_size,
             pEvent->buf_size, pEvList->data0, pEvList->data1);
    }

    /* free the message unless PEEKing */
    if (!(mask & VSPA_MSG_PEEK)) {
        pPrev = pEvList->list.prev;
        LaxListMoveTail(&pEvList->list, &pVspaList->eventsFreeList);

        /* If not at the end of the list try coalescing messages */
        if (pPrev->next != &pVspaList->eventsQueuedList &&
                pPrev != &pVspaList->eventsQueuedList) {
            /* try to coalese Watchdog errors */
            pEvList = (eventList_t *) LaxListGetEntry(pPrev, offsetof(eventList_t, list));
            pEvList1 = (eventList_t *) LaxListGetEntry(pPrev->next, offsetof(eventList_t, list));
            if (pEvList->type ==
                ((0x10<<VSPA_EVENT_ERROR)|VSPA_EVENT_ERROR) &&
                pEvList1->type ==
                ((0x10<<VSPA_EVENT_ERROR)|VSPA_EVENT_ERROR) &&
                pEvList->id == pEvList1->id &&
                pEvList->id == VSPA_ERR_WATCHDOG) {
                pEvList->data0 = pEvList1->data0;
                pEvList->data1++;
                LaxListMoveTail(&pEvList1->list,
                    &pVspaList->eventsFreeList);
            }
        }
        /* update event list mask */
        mask = 0;
        pVspaList->eventListCmds = 0;
        pEvList = (eventList_t *) LaxListGetEntry((&pVspaList->eventsQueuedList)->next, offsetof(eventList_t, list));
        while(&pEvList->list != (&pVspaList->eventsQueuedList))
        {
            mask |= pEvList->type;
            if (pEvList->type & VSPA_MSG_CMD)
                pVspaList->eventListCmds |= ((1 << pEvList->id) & VSPA_CMD_ALL_IDS);
            pEvList = (eventList_t *) LaxListGetEntry(pEvList->list.next, offsetof(eventList_t, list));
        }
        pVspaList->eventListMask = mask & VSPA_MSG_ALL_EVENTS;
    }

    /* only copy up to the length of the message */
    length += sizeof(*pEvent);
    if (length > evtRd->buf_len)
        length = evtRd->buf_len;

    OAL_spin_unlock(&pVspaList->eventListLock);
    pr_debug("vspa%d: err %d, length %zu bytes\n", EVENT_GET_DEV_ID(), err,
            length);
    return  length;
}
