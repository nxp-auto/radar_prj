/*
 * Copyright 2018-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LAX_LIST_H
#define LAX_LIST_H

/* This doxy template is to be used by files visible to customers.
 * Mind the @} ending comment, too.
 * Remove this explaining comment when specializing this template file.
 */

/** @addtogroup <name_of_doxygen_module>
* @{
*/

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/


/**
 * CONTAINER_OF - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define CONTAINER_OF(ptr, type, member) ({                    \
    ((type *)(((char *) ptr) - (offsetof(type, member)))); })

/**
 * LAX_LIST_ENTRY - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 */
#define LAX_LIST_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)

/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/

/*=================================================================================================
*                                             ENUMS
=================================================================================================*/

/*=================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
=================================================================================================*/
typedef struct listHead {
    struct listHead *next, *prev;
}listHead_t;

/*=================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
=================================================================================================*/


/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/

/**
 * LaxListIsEmpty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int LaxListIsEmpty(const listHead_t *head)
{
    return head->next == head;
}

static inline void LaxListInit(listHead_t *list)
{
    list->next = list;
    list->prev = list;
}

/**
 * LaxListPush - add a new entry
 * @new:    new entry to be added on top of the list
 * @head:   list head to add it after
 *
 * Insert a new entry after the specified head.
 */
static inline void LaxListPush(listHead_t *new, listHead_t *head)
{
    // circular buffer means we can chain as many as needed
    head->next->prev = new;
    new->next        = head->next;
    new->prev        = head;
    head->next       = new;
}

/**
 * LaxListAdd - add a new entry
 * @new:    new entry to be added at the end of the list
 * @head:   list head to add it before
 *
 * Insert a new entry before the specified head.
 */
static inline void LaxListAdd(listHead_t *new, listHead_t *head)
{
    // circular buffer means we can chain as many as needed
    new->next        = head;
    new->prev        = head->prev;
    head->prev->next = new;
    head->prev       = new;
}

static inline void LaxListDel(listHead_t * prev, listHead_t * next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * LaxListMoveTail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void LaxListMoveTail(listHead_t *list, listHead_t *head)
{
    list->next->prev = list->prev;
    list->prev->next = list->next;
    LaxListAdd(list, head);
}

/**
 * LaxListGetEntry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @offset: the offset of the list_head within the struct.
 */
static inline void* LaxListGetEntry(listHead_t *listPtr, size_t offset)
{
    return (listPtr - offset);
}

#ifdef __cplusplus
}
#endif

/** @} */ /*doxygen module*/

#endif /*LAX_LIST_H*/
