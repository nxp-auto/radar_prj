/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
* @file           lax_atu.c
* @brief          ATU driver implementation
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#include "lax_driver.h"
#include "lax_atu.h"

/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 ==================================================================================================*/

/*==================================================================================================
 *                                       LOCAL MACROS
 ==================================================================================================*/
#define SZ_TO_MASK(sz)                  ((1 << sz) - 1)
#define ATU_WINDW_BASE_ADD(win_num)     pAtuCtrl->laxAtuWin[win_num].windwBaseAdd
#define ATU_WINDW_SZ(win_num)           pAtuCtrl->laxAtuWin[win_num].windwSz
#define ATU_WINDW_MAPPED_ADD(win_num)   pAtuCtrl->laxAtuWin[win_num].windwMappedAdd

#define LAX_ATU_OWBAR(x)       (pAtuCtrl->pLaxAtuRegs + (0x110U >> 2) + 0x4*(x))
#define LAX_ATU_OWAR(x)        (pAtuCtrl->pLaxAtuRegs + (0x114U >> 2) + 0x4*(x))
#define LAX_ATU_OTEAR(x)       (pAtuCtrl->pLaxAtuRegs + (0x118U >> 2) + 0x4*(x))
#define LAX_ATU_OTAR(x)        (pAtuCtrl->pLaxAtuRegs + (0x11CU >> 2) + 0x4*(x))


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
/*********************** ATU services  ****************************/

uint32_t LaxMapAxiAddrAtuWin(atuControl_t *pAtuCtrl, dma_addr_t dmaAxiAddr)
{
    int atuWinCnt;

    /* Find the windows with which this address belongs to */
    for (atuWinCnt = 0; atuWinCnt < pAtuCtrl->numAtuWin; atuWinCnt++) 
    {
        if (((u32)dmaAxiAddr & (~SZ_TO_MASK(ATU_WINDW_SZ(atuWinCnt)))) == (u32)ATU_WINDW_MAPPED_ADD(atuWinCnt)) 
        {
            /* Window Found - Map the address to window and return */
            return ((u32)(u64)ATU_WINDW_MAPPED_ADD(atuWinCnt) + (dmaAxiAddr & SZ_TO_MASK(ATU_WINDW_SZ(atuWinCnt))));
        }
    }

    return -1U;
}


void LaxAtuEnable(atuControl_t *pAtuCtrl)
{
    int atuWinCnt;

    /* Enable all ATU windows configured from DTS */
    for (atuWinCnt = 0; atuWinCnt < pAtuCtrl->numAtuWin; atuWinCnt++) 
    {
        /* OWBAR provides windows that ATU maps inbound addresses to
        [31 ................ 16 15 ........... 5 4 ........ 0 ]
          Window Base Address       Reserved      Window Size*/
        LaxRegWrite(LAX_ATU_OWBAR(atuWinCnt),
                (((u32)(u64) ATU_WINDW_MAPPED_ADD(atuWinCnt) & (~SZ_TO_MASK(ATU_WINDW_SZ(atuWinCnt)))) |
                 ((ATU_WINDW_SZ(atuWinCnt) - LAX_ATU_MIN_WIN_SZ) & 0x1F)));

        /* OTEAR provides bits [63:32] for the outbound ATU translation */
        LaxRegWrite(LAX_ATU_OTEAR(atuWinCnt), ((ATU_WINDW_BASE_ADD(atuWinCnt) & (-1LU << 32)) >> 32));

        /* OTAR provides bits "31:31-n" for the outbound ATU translation
        * where n is "16 - window size" */
        LaxRegWrite(LAX_ATU_OTAR(atuWinCnt),
                ((u32)(u64)ATU_WINDW_BASE_ADD(atuWinCnt) & (~SZ_TO_MASK(ATU_WINDW_SZ(atuWinCnt)))));

        /* OWAR enables the corresponding ATU translation windwos */
        LaxRegWrite(LAX_ATU_OWAR(atuWinCnt), 0x80000000);
    }
}

void LaxAtuDisable(atuControl_t *pAtuCtrl)
{
    int atuWinCnt;

    /* Disable all ATU windows configured from DTS */
    for (atuWinCnt = 0; atuWinCnt < pAtuCtrl->numAtuWin; atuWinCnt++) 
    {
        LaxRegWrite(LAX_ATU_OWAR(atuWinCnt), 0x0);
    }
}


