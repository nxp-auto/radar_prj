/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
* @file           cbuffer.c
* @brief          Implementation of circular buffer functions
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "cbuffer.h"

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

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/************  Command / Reply Memory Pool Management **************/
void CbufferReset(circularBuffer_t *cbuf)
{
    cbuf->writeIdx = 0;
    cbuf->readIdx = 0;
    cbuf->cbSizePrev = 0;
    cbuf->cbIdxPrev = 0;
}

void CbufferInit(circularBuffer_t *cbuf, uint32_t size,
    uint32_t *pAddr, uint32_t *pVaddr)
{
    cbuf->size = size;
    cbuf->paddr = pAddr;
    CbufferReset(cbuf);
}

/* Make sure size is AXI aligned */
int CbufferAdd(circularBuffer_t *cbuf, uint32_t size, uint32_t nextPtrSize)
{
    uint32_t space;
    uint32_t index;
    uint32_t newIdx;

    if (cbuf->writeIdx >= cbuf->readIdx) {
        index = cbuf->writeIdx;
        space = cbuf->size - index;
        if (space < size) {
            index = 0;
            space = cbuf->readIdx;
        }
    } else {
        index = cbuf->writeIdx;
        space = cbuf->readIdx - index;
    }

    if (space < size) {
        /* as a last resort resort reset the indexes if empty */
        if (cbuf->writeIdx == cbuf->readIdx && cbuf->size >= size) {
            cbuf->writeIdx = 0;
            cbuf->readIdx = 0;
            index = 0;
        } else
            return -ENOBUFS;
    }

    newIdx = index + size;
    /* commented out for lax
    if (newIdx == cbuf->size)
        newIdx = 0; */

    if (newIdx == cbuf->readIdx)
        return -ENOBUFS;

    cbuf->writeIdx = ((newIdx == 0) ? 0 : newIdx - nextPtrSize);
    return index;
}

void CbufferFree(circularBuffer_t *cbuf,
        uint32_t index, uint32_t size)
{
    cbuf->readIdx = index + size - 8; // align_words = pVspaDev->hardware.axi_data_width >> 5 = 8
    if (cbuf->readIdx >= cbuf->size)
        cbuf->readIdx = 0;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

