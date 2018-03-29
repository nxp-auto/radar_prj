/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RSDK_EVENT_H
#define RSDK_EVENT_H

/* This doxy template is to be used by files visible to customers.
 * Mind the @} ending comment, too.
 * Remove this explaining comment when specializing this template file.
 */

/** @addtogroup <name_of_doxygen_module>
* @{
*/

/*=================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
=================================================================================================*/

#include <uapi/linux/vspa_uapi.h>
#include <oal_spinlock.h>
#include <oal_waitqueue.h>
#include "lax_list.h"

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


#define IF_EVENT_DEBUG(x)    if (pVspaList->debug & (x))
#define EVENT_ERR(...)    { if (pVspaList->debug & DEBUG_MESSAGES) \
                pr_err("VSPA event" __VA_ARGS__); }

#define EVENT_GET_DEV_ID()     (CONTAINER_OF(pVspaList, vspaControl_t, vspaList)->id)
#define EVENT_GET_DEV_STATE() (CONTAINER_OF(pVspaList, vspaControl_t, vspaList)->state)

#define EVENT_LIST_ENTRIES      (256)
#define EVENT_QUEUE_ENTRIES     (256)


#define VSPA_CMD_ALL_IDS        (0x0000FFFF)

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


typedef struct {
    listHead_t list;
    union {
        uint32_t  control;
        struct {
            uint8_t     id;
            uint8_t     err;
            uint16_t    type;
        };
    };
    uint32_t    data0;
    uint32_t    data1;
} eventList_t;

typedef struct {
    union {
        uint32_t      control;
        struct {
            uint8_t     id;
            uint8_t     err;
            uint8_t     rsvd;
            uint8_t     type;
        };
    };
    uint32_t    data0;
    uint32_t    data1;
    uint32_t    lost;
} eventEntry_t;

typedef struct {
    eventEntry_t    entry[EVENT_QUEUE_ENTRIES];
    volatile int    idxEnqueue; /* Index for next item to enqueue */
    volatile int    idxQueued;  /* Index for last item enqueued */
    volatile int    idxDequeue; /* Index for next item to dequeue */
} eventQueue_t;

typedef struct
{
    uint32_t debug;
    /* Event queue */
    eventQueue_t    eventQueue;
    eventList_t     events[EVENT_LIST_ENTRIES];
    listHead_t eventsFreeList;
    listHead_t eventsQueuedList;
    OAL_spinlock_t  eventListLock;
    OAL_spinlock_t  eventQueueLock; /* called from irq handler */


    /* Wait queue for event notifications*/
    OAL_waitqueue_t eventWaitQ;
    uint32_t    eventListMask;
    uint32_t    eventQueueMask;
    uint32_t    pollMask;
    uint32_t    eventListCmds;
    uint32_t    eventQueueCmds;
}
vspaEventList_t;


/*=================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
=================================================================================================*/


/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/

/**
* @brief          A brief text description (one line).
* @details        A detailed text description of the code object being described, it can span more
*                 lines and contain formatting tags (both Doxygen and HTML). Optional tag.
*
* @param[in]      number      Describes a parameter that is input to the function or macro.
*                             Must be omitted if the function does not have parameters.
* @param[out]     object_ptr  Describes a parameter that is output of the function or macro.
*                             Must be omitted if the function does not have parameters.
* @param[in,out]  config_ptr  Describes a parameter that is both input and output of the function or
*                             macro.
*                             Must be omitted if the function does not have parameters.
*
* @return         Description of the returned value.
*                 One can use @ref to point to a regularly used type defined in an @addtogroup.
* @retval RETURNED_VALUE_1    Describes specific return value RETURNED_VALUE_1 and its meaning. There
*                             can be more than one @retval tags for each function.
*                             It must be omitted if the function does not return specific values.
* @retval RETURNED_VALUE_2    Describes specific return value RETURNED_VALUE_2 and its meaning. There
*                             can be more than one @retval tags for each function.
*                             It must be omitted if the function does not return specific values.
*
* @pre            Preconditions as text description. Optional tag.
* @post           Postconditions as text description. Optional tag.
*
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/

void EventResetQueue(vspaEventList_t *pVspaDev);
void EventEnqueue(vspaEventList_t *pVspaDev, uint8_t type, uint8_t id, uint8_t err, uint32_t data0, uint32_t data1);
void EventListUpdate(vspaEventList_t *pVspaDev);
int ReadEvent(vspaEventList_t *pVspaDev, struct vspa_event_read *evt_rd, struct vspa_event *pEvent);

#ifdef __cplusplus
}
#endif

/** @} */ /*doxygen module*/

#endif /*RSDK_EVENT_H*/

