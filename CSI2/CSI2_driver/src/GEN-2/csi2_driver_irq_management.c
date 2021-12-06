/*
 * Copyright 2019-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
* @file           csi2_driver_irq_management.c
*
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <stddef.h>
#ifndef linux
#include <stdint.h>
#include <string.h>
#else
#include <linux/uaccess.h>
#include <linux/of_address.h>
#endif

#include "rsdk_status.h"
#include "csi2_driver_irq_management.h"

#ifdef TRACE_ENABLE
#include "csi2_driver_platform_trace.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define RSDK_CSI2_BYTES_PER_SAMPLE 2u  // sample buffer length, only for radar


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

#ifndef linux

static void Csi2IrqHandlerEvents(const rsdkCsi2UnitId_t iUnit);
static void Csi2IrqHandlerPathErr(const rsdkCsi2UnitId_t iUnit);
static void Csi2IrqHandlerRxErr(const rsdkCsi2UnitId_t iUnit);

// Individual irq unit handlers for Rx errors
static void Csi2IrqHandlerRxErr0(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
static void Csi2IrqHandlerRxErr1(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif

);
#ifdef S32R45
static void Csi2IrqHandlerRxErr2(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif

);
static void Csi2IrqHandlerRxErr3(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif

);
#endif

// Individual irq unit handlers for Path errors
static void Csi2IrqHandlerPathErr0(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
static void Csi2IrqHandlerPathErr1(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
#ifdef S32R45
static void Csi2IrqHandlerPathErr2(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
static void Csi2IrqHandlerPathErr3(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
#endif

// Individual irq unit handlers for Events (Rx)
static void Csi2IrqHandlerEvents0(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
static void Csi2IrqHandlerEvents1(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
#ifdef S32R45
static void Csi2IrqHandlerEvents2(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
static void Csi2IrqHandlerEvents3(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
#endif

// Individual irq unit handlers for Tx
static void Csi2IrqHandlerTx0(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
static void Csi2IrqHandlerTx1(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
#ifdef S32R45
static void Csi2IrqHandlerTx2(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
static void Csi2IrqHandlerTx3(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
);
#endif

#endif  // #ifndef linux

// prototypes for each available irq initialization
static rsdkStatus_t Csi2InitPathIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                                    const rsdkCsi2InitParams_t *pParams);
static rsdkStatus_t Csi2InitRxIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                                  const rsdkCsi2InitParams_t *pParams);
static rsdkStatus_t Csi2InitTxIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                                  const rsdkCsi2InitParams_t *pParams);
static rsdkStatus_t Csi2InitEventIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                                     const rsdkCsi2InitParams_t *pParams);

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
// irq handlers matrix
#ifndef linux

static  rsdkIrqHandler_t    sgIrqHandlers[RSDK_CSI2_MAX_UNITS][RSDK_CSI2_MAX_IRQ_ID]
#ifndef __ZEPHYR__
    __attribute__((section(".RSDK_CSI2_INTERNAL_MEMORY"))) 
#endif
        = {
        {// unit 1 handlers
         Csi2IrqHandlerRxErr0, Csi2IrqHandlerPathErr0, Csi2IrqHandlerEvents0, Csi2IrqHandlerTx0},
        {// unit 2 handlers
         Csi2IrqHandlerRxErr1, Csi2IrqHandlerPathErr1, Csi2IrqHandlerEvents1, Csi2IrqHandlerTx1},
#ifdef S32R45
        {// unit 3 handlers
         Csi2IrqHandlerRxErr2, Csi2IrqHandlerPathErr2, Csi2IrqHandlerEvents2, Csi2IrqHandlerTx2},
        {// unit 4 handlers
         Csi2IrqHandlerRxErr3, Csi2IrqHandlerPathErr3, Csi2IrqHandlerEvents3, Csi2IrqHandlerTx3}
#endif
};

#endif

#ifndef linux
static uint32_t gsCsi2IrqUnitRemap[RSDK_CSI2_MAX_UNITS] = {




#if   defined(S32R45)
    (uint32_t)RSDK_CSI2_UNIT_1, (uint32_t)RSDK_CSI2_UNIT_0, (uint32_t)RSDK_CSI2_UNIT_3, (uint32_t)RSDK_CSI2_UNIT_2
#endif  // #ifdef S32R294
};
#endif  // #ifndef linux

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
/*================================================================================================*/
/*
 * @brief       Enable for Rx errors interrupt on the specified VC.
 *
 * @param[in]   vcId    - VC ID, RSDK_CSI2_VC_0 ... RSDK_CSI2_MAX_VC
 * @param[in]   pRegs   - pointer to unit registry
 * @param[in]   val     - value to be set, usually 0 or 0xffffffff
 *
 */
static inline void Csi2SetVCRxIrq(const uint32_t vcId, volatile struct MIPICSI2_REG_STRUCT *pRegs, uint32_t val)
{
    pRegs->RX_VC[vcId].PPERRIE.R = val;
}
/* Csi2SetVCRxIrq *************************/

/*================================================================================================*/
/*
 * @brief       Init for Rx errors interrupt.
 * @details     This procedure is called after the general unit initialization was done.
 *
 * @param[in]   iUnit   - unit ID, RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS
 * @param[in]   pRegs   - pointer to unit registry
 * @param[in]   pParams - pointer to CSI2 unit init parameters
 *
 */
static rsdkStatus_t Csi2InitRxIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                                  const rsdkCsi2InitParams_t *pParams)
{
    uint32_t                vcId;          // VC ID, from RSDK_CSI2_VC_0 to MAX
    rsdkCsi2DriverParams_t *pDriverState;  // pointer to the unit driver state
    rsdkStatus_t            rez = RSDK_SUCCESS;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PHY_IRQ_INIT, (uint32_t)CSI2_SEQ_BEGIN);
    pDriverState = &gCsi2Settings[(uint8_t)iUnit];
#ifndef linux
    if (RsdkGlueIrqHandlerRegister(sgIrqHandlers[iUnit][RSDK_CSI2_RX_ERR_IRQ_ID],
                                   (uint32_t)CSI2_IRQ_RX_BASE_ID_GIC +
                                       ((uint32_t)CSI2_IRQ_MAP_GAP * gsCsi2IrqUnitRemap[iUnit]),
                                   pParams->irqExecCore, pParams->irqPriority) == IRQ_REGISTER_SUCCESS)
    {
#endif
        // save the callback
        pDriverState->pCallback[(uint8_t)RSDK_CSI2_RX_ERR_IRQ_ID] =
            pParams->pCallback[(uint8_t)RSDK_CSI2_RX_ERR_IRQ_ID];
        // real irq enablement
        pRegs->RX_PHYERRIE.R = 0xffffffffu;                                               // enable all error flags
        for (vcId = (uint32_t)RSDK_CSI2_VC_0; vcId < (uint32_t)RSDK_CSI2_MAX_VC; vcId++)  // check all VC
        {
            if (pParams->pVCconfig[vcId] != NULL)  // only for active VC
            {
                Csi2SetVCRxIrq(vcId, pRegs, (uint32_t)0xffffffffu);  // enable the interrupts
            }
            else
            {
                Csi2SetVCRxIrq(vcId, pRegs, (uint32_t)0);  // disable the interrupts
            }
        }
#ifndef linux
    }
    else
    {
        rez = RSDK_CSI2_DRV_ERR_IRQ_HANDLER_REG;
    }
#endif
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PHY_IRQ_INIT, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2InitRxIrq *************************/

/*================================================================================================*/
/*
 * @brief       Enable for Path errors interrupt on VC.
 *
 * @param[in]   vcId    - VC ID, RSDK_CSI2_VC_0 ... RSDK_CSI2_MAX_VC
 * @param[in]   pRegs   - pointer to unit registry
 *
 */
static void Csi2EnableVCPathIrq(const uint32_t vcId, volatile struct MIPICSI2_REG_STRUCT *pRegs)
{
    // check for each flag possible to be set
    // and set the necessary enable bit, to the appropriate registry
    uint32_t regVal;

    regVal = pRegs->CBUF_INTRE.R;
    regVal |= ((uint32_t)CSI2_IRQ_VC_LIN0LENERR_MASK + (uint32_t)CSI2_IRQ_VC_LIN0CNTERR_MASK) << vcId;
    pRegs->CBUF_INTRE.R = regVal;
}
/* Csi2EnableVCPathIrq *************************/

/*================================================================================================*/
/*
 * @brief       Init for Path errors interrupt.
 * @details     This procedure is called after the general unit initialization was done.
 *
 * @param[in]   iUnit   - unit ID, RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS
 * @param[in]   pRegs   - pointer to unit registry
 * @param[in]   pParams - pointer to CSI2 unit init parameters
 *
 */
static rsdkStatus_t Csi2InitPathIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                                    const rsdkCsi2InitParams_t *pParams)
{
    uint32_t                vcId;          // VC ID, from RSDK_CSI2_VC_0 to MAX
    rsdkCsi2DriverParams_t *pDriverState;  // pointer to unit driver state
    rsdkStatus_t            rez = RSDK_SUCCESS;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PKT_IRQ_INIT, (uint32_t)CSI2_SEQ_BEGIN);
    // check for every possible flag request
    pDriverState = &gCsi2Settings[(uint8_t)iUnit];
#ifndef linux
    if (RsdkGlueIrqHandlerRegister(sgIrqHandlers[iUnit][RSDK_CSI2_PATH_ERR_IRQ_ID],
                                   (uint32_t)CSI2_IRQ_PATH_BASE_ID_GIC +
                                       ((uint32_t)CSI2_IRQ_MAP_GAP * gsCsi2IrqUnitRemap[iUnit]),
                                   pParams->irqExecCore, pParams->irqPriority) == IRQ_REGISTER_SUCCESS)
    {
#endif
        pDriverState->pCallback[(uint8_t)RSDK_CSI2_PATH_ERR_IRQ_ID] =
            pParams->pCallback[(uint8_t)RSDK_CSI2_PATH_ERR_IRQ_ID];
        // real irq enablement
        pRegs->CONTROLLER_ERR_IE.R = 0xffffffffu;
        pRegs->RX_CHNL_INTRE.B.BUFFOVFIE = 1u;  // enable the possible errors on channel data
        pRegs->CBUF_INTRE.R = (uint32_t)0;      // first disable all
        for (vcId = (uint32_t)RSDK_CSI2_VC_0; vcId < (uint32_t)RSDK_CSI2_MAX_VC; vcId++)  // check all VC
        {
            if (pParams->pVCconfig[vcId] != NULL)       // only for active VC
            {
                Csi2EnableVCPathIrq(vcId * 2u, pRegs);  // enable the interrupts
            }
            if (pParams->pMetaData[vcId] != NULL)       // for MetaData if required
            {
                Csi2EnableVCPathIrq(vcId + ((uint32_t)RSDK_CSI2_MAX_VC * (uint32_t)4), pRegs);  // enable the interrupts
            }
        }
#ifndef linux
    }
    else
    {
        rez = RSDK_CSI2_DRV_ERR_IRQ_HANDLER_REG;
    }
#endif
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PKT_IRQ_INIT, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2InitPathIrq *************************/

/*================================================================================================*/
/*
 * @brief       Enable for events interrupt on VC.
 * @details     This procedure is called after the general unit initialization was done.
 *
 * @param[in]   iUnit   - unit ID, RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS
 * @param[in]   pRegs   - pointer to unit registry
 * @param[in]   val     - single bit value, 0=disable, 1=enable
 *
 */
static void Csi2SetVCEventIrq(const uint32_t vcId, volatile struct MIPICSI2_REG_STRUCT *pRegs, uint8_t evtMask,
                              uint16_t numeLinesTrigger)
{
    uint8_t valFS;
    uint8_t valFE;

    valFS = ((evtMask & RSDK_CSI2_EVT_FRAME_START) != 0u) ? 1u : 0u;
    valFE = ((evtMask & RSDK_CSI2_EVT_FRAME_END) != 0u) ? 1u : 0u;
    // set all ie bits
    switch (vcId)  // set the necessary enable bit, to the appropriate registry
    {
        default:  // RSDK_CSI2_VC_0
            pRegs->RX_VCINTRE.B.GNSPIE0 = valFE;
            pRegs->RX_VCINTRE.B.FSIE0 = valFS;
            pRegs->RX_VCINTRE.B.FEIE0 = valFE;  // activate Frame End interrupt (for stat. process)
            pRegs->RX[0].CBUF_LPDI.B.NUMLINES =
                (uint8_t)numeLinesTrigger;  // trigger after each chirp (for stat. process)
            break;
        case (uint32_t)RSDK_CSI2_VC_1:
            pRegs->RX_VCINTRE.B.GNSPIE1 = valFE;
            pRegs->RX_VCINTRE.B.FSIE1 = valFS;
            pRegs->RX_VCINTRE.B.FEIE1 = valFE;
            pRegs->RX[1].CBUF_LPDI.B.NUMLINES = (uint8_t)numeLinesTrigger;
            break;
        case (uint32_t)RSDK_CSI2_VC_2:
            pRegs->RX_VCINTRE.B.GNSPIE2 = valFE;
            pRegs->RX_VCINTRE.B.FSIE2 = valFS;
            pRegs->RX_VCINTRE.B.FEIE2 = valFE;
            pRegs->RX[2].CBUF_LPDI.B.NUMLINES = (uint8_t)numeLinesTrigger;
            break;
        case (uint32_t)RSDK_CSI2_VC_3:
            pRegs->RX_VCINTRE.B.GNSPIE3 = valFE;
            pRegs->RX_VCINTRE.B.FSIE3 = valFS;
            pRegs->RX_VCINTRE.B.FEIE3 = valFE;
            pRegs->RX[3].CBUF_LPDI.B.NUMLINES = (uint8_t)numeLinesTrigger;
            break;
    }  // switch
}
/* Csi2SetVCEventIrq *************************/

/*================================================================================================*/
/*
 * @brief       Init for events interrupt.
 * @details     Init for events interrupt. All possibilities will be set.
 *
 * @param[in]   iUnit   - unit ID, RSDK_CSI2_UNIT_0 ... MAX
 * @param[in]   pRegs   - pointer to unit registry
 * @param[in]   pParams - pointer to CSI2 unit initialization parameters
 *
 */
static rsdkStatus_t Csi2InitEventIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                                     const rsdkCsi2InitParams_t *pParams)
{
    uint32_t                vcId;                           // VC ID, from RSDK_CSI2_VC_0 to MAX
    uint8_t                 setLDevt, evtMask;              // set LINEDONE event
    uint16_t                lineDone;
    rsdkCsi2DriverParams_t *pDriverState;                   // pointer to unit driver state
    rsdkStatus_t            rez = RSDK_SUCCESS;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_DATA_IRQ_INIT, (uint32_t)CSI2_SEQ_BEGIN);
    pDriverState = &gCsi2Settings[(uint8_t)iUnit];

#ifndef linux
    if (RsdkGlueIrqHandlerRegister(sgIrqHandlers[iUnit][RSDK_CSI2_EVENTS_IRQ_ID],
                                   (uint32_t)CSI2_IRQ_EVENT_BASE_ID_GIC +
                                       ((uint32_t)CSI2_IRQ_MAP_GAP * gsCsi2IrqUnitRemap[iUnit]),
                                   pParams->irqExecCore, pParams->irqPriority) == IRQ_REGISTER_SUCCESS)
    {
#endif
        // save the callback
        pDriverState->pCallback[(uint8_t)RSDK_CSI2_EVENTS_IRQ_ID] =
            pParams->pCallback[(uint8_t)RSDK_CSI2_EVENTS_IRQ_ID];
        // irq enablement for skew calibration
        pRegs->RX_VCINTRE.R = 0;  // first disable the interrupts
        setLDevt = 0;             // no LINEDONE request
        for (vcId = (uint32_t)RSDK_CSI2_VC_0; vcId < (uint32_t)RSDK_CSI2_MAX_VC; vcId++)  // check all VC
        {
            if(pParams->pVCconfig[vcId] != NULL)
            {
                evtMask = pParams->pVCconfig[vcId]->vcEventsReq | (uint8_t)RSDK_CSI2_EVT_FRAME_END;
                lineDone = pParams->pVCconfig[vcId]->bufNumLinesTrigger;
            }
            else
            {
                evtMask = 0u;
                lineDone = 0u;
            }
            if(pParams->pMetaData[vcId] != NULL)
            {
                evtMask |= pParams->pMetaData[vcId]->vcEventsReq | (uint8_t)RSDK_CSI2_EVT_FRAME_END;
            }
            if (evtMask != 0u)                              // only for active VC
            {
                // enable the interrupts for active VC, FE must be processed anyway
                Csi2SetVCEventIrq(vcId, pRegs, evtMask, lineDone);
            }
            else
            {
                Csi2SetVCEventIrq(vcId, pRegs, 0u, 0u);  // disable the interrupts for inactive VC
            }
            setLDevt |= evtMask & (uint8_t)RSDK_CSI2_EVT_LINE_END;
        }
        // enable the "line done" interrupt, for internal purposes at least
        if ((pDriverState->statisticsFlag == RSDK_CSI2_STAT_EVERY_LINE) || (setLDevt != 0u))
        {
            pRegs->RX_CHNL_INTRE.B.LINEDONEIE = 1u;  // process all chirps statistics
        }
        else
        {
            pRegs->RX_CHNL_INTRE.B.LINEDONEIE = 0u;  // process statistics at FE or not
        }
#ifndef linux
    }
    else
    {
        rez = RSDK_CSI2_DRV_ERR_IRQ_HANDLER_REG;
    }
#endif
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_DATA_IRQ_INIT, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2InitEventIrq *************************/

/*================================================================================================*/
/*
 * @brief       Init for Tx interrupt.
 * @details     TODO : remove it fi Tx feature will not be implemented into the hw.
 *
 * @param[in]   iUnit   - unit ID, RSDK_CSI2_UNIT_0 ... MAX
 * @param[in]   pRegs   - pointer to unit registry
 * @param[in]   pParams - pointer to CSI2 VC init parameters
 *
 */
static rsdkStatus_t Csi2InitTxIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                                  const rsdkCsi2InitParams_t *pParams)
{
    rsdkCsi2DriverParams_t *pDriverState;  // pointer to unit driver state
    rsdkStatus_t            rez = RSDK_SUCCESS;

    pDriverState = &gCsi2Settings[(uint8_t)iUnit];
    // save the callback
    pDriverState->pCallback[(uint8_t)RSDK_CSI2_TX_ERR_IRQ_ID] = pParams->pCallback[(uint8_t)RSDK_CSI2_TX_ERR_IRQ_ID];
    // so, add the irq handler
#ifndef linux
    if (RsdkGlueIrqHandlerRegister(sgIrqHandlers[iUnit][RSDK_CSI2_TX_ERR_IRQ_ID],
                                   (uint32_t)CSI2_IRQ_TX_BASE_ID_GIC +
                                       ((uint32_t)CSI2_IRQ_MAP_GAP * gsCsi2IrqUnitRemap[iUnit]),
                                   pParams->irqExecCore, pParams->irqPriority) == IRQ_REGISTER_ERROR)
    {
        rez = RSDK_CSI2_DRV_ERR_IRQ_HANDLER_REG;
    }
#endif
    // irq enablement - all available request activated
    UNUSED_ARG(pRegs);
    /*          TODO : Tx not enabled in the rev.N of RM, to check later
    */
    return rez;
}
/* Csi2InitTxIrq *************************/

/*================================================================================================*/
/*
 * @brief       Process Rx interrupt at VC level.
 * @details     This procedure only get the specific data.
 *              All the irq specific bits will be reset in the main procedure.
 *
 * @param[in]   vcId        - VC ID, RSDK_CSI2_VC_0 ... MAX
 * @param[in]   regStat     - unit registry (RX_PPERRIS_VC ...) state
 * @param[in]   pErrorS     - pointer to error structure
 * @param[in]   pRegs       - pointer to unit registry
 *
 */
static void Csi2IrqHandlerRxErrVC(const uint32_t vcId, const uint32_t regStat, rsdkCsi2Report_t *pErrorS,
                                  volatile struct MIPICSI2_REG_STRUCT *pRegs)
{
    uint32_t regV;

    pErrorS->errMaskVC[vcId] = 0;  // no errors for the beginning
    // ECC error - 2 bits
    if ((regStat & (uint32_t)CSI2_IRQ_VC_ECC2_MASK) != (uint32_t)0)
    {
        pErrorS->errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_PACK_ECC2;
    }
    // ECC error - one bit
    if ((regStat & (uint32_t)CSI2_IRQ_VC_ECC1_MASK) != (uint32_t)0)
    {
        pErrorS->errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_PACK_ECC1;  // set the mask first
        regV = pRegs->RX_VC[vcId].ERRPOS.B.ERRPOS;
        pErrorS->eccOneBitErrPos[vcId] = (uint8_t)regV;                 // copy the appropriate value
    }
    // CRC error
    if ((regStat & (uint32_t)CSI2_IRQ_VC_CRC_MASK) != (uint32_t)0)
    {
        pErrorS->errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_PACK_CRC;  // set the mask



        regV = pRegs->CRC_REGISTER.R;

        pErrorS->expectedCRC[vcId] = (uint16_t)(regV >> 16u);           // save both, expected CRC
        pErrorS->receivedCRC[vcId] = (uint16_t)(regV);                  //      & received CRC
    }
    // Frame sync error
    if ((regStat & (uint32_t)CSI2_IRQ_VC_FSYN_MASK) != (uint32_t)0)
    {
        pErrorS->errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_PACK_SYNC;
    }
    // Data type error
    if ((regStat & (uint32_t)CSI2_IRQ_VC_IVID_MASK) != (uint32_t)0)
    {
        pErrorS->errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_PACK_ID;
        regV = pRegs->RX_INVIDR.R;
        if (((regV >> 6u) & 0x3u) == vcId)
        {  // get the pachet ID only if the correct VC
            pErrorS->invalidPacketID[vcId] = (uint8_t)(regV & 0x3fu);
        }
    }
    // Frame sync error
    if ((regStat & (uint32_t)CSI2_IRQ_VC_FDAT_MASK) != (uint32_t)0)
    {
        pErrorS->errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_PACK_DATA;
    }
}
/* Csi2IrqHandlerRxErrVC *************************/

/*================================================================================================*/
/*
 * @brief       Generic interrupt procedure for PHY error irq.
 *
 * @param[in]   iUnit   - unit id, RSDK_CSI2_UNIT_0 ... MAX
 *
 */
#ifndef linux
static
#endif
    void
    Csi2IrqHandlerRxErr(const rsdkCsi2UnitId_t iUnit)
{
    uint32_t                             i;
    uint32_t                             mask;
    uint32_t                             vcId;  // index for VC
    uint32_t                             regStat, toCall;
    rsdkCsi2Report_t                     errorS;        // error structure
    volatile struct MIPICSI2_REG_STRUCT *pRegs;         // pointer to CSI2 registers
    rsdkCsi2DriverParams_t *             pDriverState;  // unit driver status

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PHY_ERR_ISR, (uint32_t)CSI2_SEQ_BEGIN);
    pRegs = gpMipiCsi2Regs[(uint8_t)iUnit];  // get the  registry pointer for the unit
    if (pRegs != NULL)
    {
        errorS.unitId = iUnit;
        errorS.errMaskU = 0;
        pDriverState = &gCsi2Settings[(uint8_t)iUnit];
        // FIRST step = check for Unit level DPHY errors
        mask = (uint32_t)1;  // mask for single event
        regStat = pRegs->RX_PHYERRIS.R;
        pRegs->RX_PHYERRIS.R = regStat;  // clear the bits
        for (i = (uint32_t)RSDK_CSI2_LANE_0; i < (uint32_t)RSDK_CSI2_MAX_LANE; i++)
        {
            // check for Error In Synchronization Pattern
            if ((regStat & mask) != (uint32_t)0)
            {
                errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_PHY_SYNC;
            }
            mask <<= 1u;
            // check for Multibit Error In Synchronization Pattern
            if ((regStat & mask) != (uint32_t)0)
            {
                errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_PHY_NO_SYNC;
            }
            mask <<= 1u;
            // check for Control Command Error
            if ((regStat & mask) != (uint32_t)0)
            {
                errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_PHY_ESC;
            }
            mask <<= 1u;
            // check for Synchronization Error in escape mode
            if ((regStat & mask) != (uint32_t)0)
            {
                errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_PHY_SESC;
            }
            mask <<= 1u;
            // check for Control Command Error
            if ((regStat & mask) != (uint32_t)0)
            {
                errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_PHY_CTRL;
            }
            mask <<= 1u;
        }  // for

        // SECOND step = check for VC level Packet & Protocol errors
        toCall = errorS.errMaskU;  // the callback necessity at this point
        for (vcId = (uint32_t)RSDK_CSI2_VC_0; vcId < (uint32_t)RSDK_CSI2_MAX_VC; vcId++)
        {
            errorS.errMaskVC[vcId] = 0u;                            // no errors for VC
            errorS.evtMaskVC[vcId] = 0u;                            // no events for the start
            regStat = pRegs->RX_VC[vcId].PPERRIS.R;                 // read the registry
            pRegs->RX_VC[vcId].PPERRIS.R = regStat;                 // clean the registry
            if ((pDriverState->workingParamVC[vcId].pVCParams != NULL) ||
                    (pDriverState->pMDParams[vcId] != NULL))        // only for active VC
            {
                Csi2IrqHandlerRxErrVC(vcId, regStat, &errorS, pRegs);
                toCall |= errorS.errMaskVC[vcId];           // accumulate the callback necessity
                errorS.errMaskU |= errorS.errMaskVC[vcId];  // accumulate the error masks
            }
        }
        // THIRD step - callback to the application
        if (toCall == (uint32_t)0)
        {  // if no flags till here
            errorS.errMaskU = (uint32_t)RSDK_CSI2_ERR_SPURIOUS_PHY;
        }
        pDriverState->pCallback[(uint8_t)RSDK_CSI2_RX_ERR_IRQ_ID](&errorS);
    } // if (pRegs != NULL)
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PHY_ERR_ISR, (uint32_t)CSI2_SEQ_END);
}
/* Csi2IrqHandlerRxErr *************************/

#ifndef linux

/*================================================================================================*/
/*
 * @brief       Interrupt handler for unit 0/ PHY error irq.
 *
 */
static void Csi2IrqHandlerRxErr0(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerRxErr(RSDK_CSI2_UNIT_0);
}
/* Csi2IrqHandlerRxErr0 *************************/

/*================================================================================================*/
/*
 * @brief       Interrupt handler for unit 1/ PHY error irq.
 *
 */
static void Csi2IrqHandlerRxErr1(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif

)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerRxErr(RSDK_CSI2_UNIT_1);
}
/* Csi2IrqHandlerRxErr1 *************************/

#ifdef S32R45

/*================================================================================================*/
/*
 * @brief       Interrupt handler for unit 2/ PHY error irq.
 *
 */
static void Csi2IrqHandlerRxErr2(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerRxErr(RSDK_CSI2_UNIT_2);
}
/* Csi2IrqHandlerRxErr2 *************************/

/*================================================================================================*/
/*
 * @brief       Interrupt handler for unit 3/ PHY error irq.
 *
 */
static void Csi2IrqHandlerRxErr3(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerRxErr(RSDK_CSI2_UNIT_3);
}
/* Csi2IrqHandlerRxErr3 *************************/

#endif  //#ifdef S32R45
#endif  //#ifndef linux

/*================================================================================================*/
/*
 * @brief       Generic interrupt procedure for errors in protocol & packet level irq.
 *
 * @param[in]   iUnit   - unit id, RSDK_CSI2_UNIT_0 ... MAX
 *
 */
#ifndef linux
static
#endif
    void
    Csi2IrqHandlerPathErr(const rsdkCsi2UnitId_t iUnit)
{
    uint32_t                             vcId;  // VC index
    uint32_t                             regStat, mask, toCall;
    rsdkCsi2Report_t                     errorS;        // reporting error structure
    volatile struct MIPICSI2_REG_STRUCT *pRegs;         // pointer to unit registry
    rsdkCsi2DriverParams_t *             pDriverState;  // unit driver state

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PKT_ERR_ISR, (uint32_t)CSI2_SEQ_BEGIN);
    pRegs = gpMipiCsi2Regs[(uint8_t)iUnit];  // get the  registry pointer for the unit
    if (pRegs != NULL)
    {
        errorS.unitId = iUnit;
        errorS.errMaskU = 0;
        pDriverState = &gCsi2Settings[(uint8_t)iUnit];
        // FIRST step = check for Unit path errors
        regStat = pRegs->RX_CHNL_INTRS.R;
        pRegs->RX_CHNL_INTRS.R = regStat;                               // clear the bits
        // check for Internal Buffer Overflow
        if ((regStat & CSI2_CHNL_INTRS_BUFFOVF) != 0u)
        {
            errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_BUF_OVERFLOW;
        }
        // check for latency in AXI write channel response
        regStat = pRegs->WR_CHNL_INTRS.R;
        pRegs->WR_CHNL_INTRS.R = regStat;
        if ((regStat & CSI2_CHNL_INTRS_BUFFOVF) != 0u)
        {
            errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_AXI_OVERFLOW;
        }
        // check for Error Response on the AXI write channel
        if ((regStat & CSI2_WR_INTRS_ERRRESP) != 0u)
        {
            errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_AXI_RESPONSE;
        }
        // check for overflow of the internal FIFO in the controller
        regStat = pRegs->CONTROLLER_ERR_STATUS_REGISTER.R;
        pRegs->CONTROLLER_ERR_STATUS_REGISTER.R = regStat;              // clear the bits
        if ((regStat & CSI2_CHNL_INTRS_BUFFOVF) != 0u)
        {
            errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_FIFO;
        }
        // check for "PHY has stopped high speed transmission earlier than normal"
        if ((regStat & CSI2_WR_INTRS_ERRRESP) != 0u)
        {
            errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_HS_EXIT;
        }
        toCall = errorS.errMaskU;                                       // the callback necessity
        // SECOND step = check for VC level path errors
        mask = (uint32_t)CSI2_IRQ_VC_LIN0LENERR_MASK;
        regStat = pRegs->CBUF_INTRS.R;                                  // the full error mask
        pRegs->CBUF_INTRS.R = regStat;                                  // clear the bits
        for (vcId = (uint32_t)RSDK_CSI2_VC_0; vcId < (uint32_t)RSDK_CSI2_MAX_VC; vcId++)
        {
            errorS.errMaskVC[vcId] = 0u;                                // no errors for the start
            errorS.evtMaskVC[vcId] = 0u;                                // no events for the start
            if (pDriverState->workingParamVC[vcId].pVCParams != NULL)   // only for active VC
            {
                if ((regStat & mask) != (uint32_t)0)
                {  // line length error
                    errorS.errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_LINE_LEN;
                    errorS.lineLengthErr[vcId].linePoz = (uint16_t)pRegs->RX[vcId].CBUF_ERRLINE.R;
                    errorS.lineLengthErr[vcId].lineLength = (uint16_t)pRegs->RX[vcId].CBUF_ERRLEN.R;
                }
                mask <<= 1u;
                if ((regStat & mask) != (uint32_t)0)
                {  // line count error
                    errorS.errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_LINE_CNT;
                    errorS.lineLengthErr[vcId].linePoz = pRegs->RX[vcId].CBUF_RXLINE.R;
                }
                toCall |= errorS.errMaskVC[vcId];           // accumulate the callback necessity
                errorS.errMaskU |= errorS.errMaskVC[vcId];  // accumulate the error masks
                mask <<= 1u;
            }
            else
            {   // the VC is not initialized
                mask <<= 2u;
            }
        }   // for
        mask = (uint32_t)1 << ((uint32_t)RSDK_CSI2_MAX_VC * (uint32_t)4);
        for (vcId = (uint32_t)RSDK_CSI2_VC_0; vcId < (uint32_t)RSDK_CSI2_MAX_VC; vcId++)
        {
            if (pDriverState->pMDParams[vcId] != NULL)                  // only for active VC
            {
                if ((regStat & mask) != (uint32_t)0)
                {  // line length error
                    errorS.errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_LINE_LEN_MD;
                    errorS.lineLengthErr[vcId].linePoz = (uint16_t)pRegs->RX[vcId].CBUF_ERRLINE.R;
                    errorS.lineLengthErr[vcId].lineLength &= 0xffffu;   // clear the 16 bits MSB
                    errorS.lineLengthErr[vcId].lineLength |= (pRegs->RX[vcId].CBUF_ERRLEN.R << 16u);
                }
                mask <<= 1u;
                if ((regStat & mask) != (uint32_t)0)
                {  // line count error
                    errorS.errMaskVC[vcId] |= (uint32_t)RSDK_CSI2_ERR_LINE_CNT_MD;
                    errorS.lineLengthErr[vcId].linePoz = (uint16_t)pRegs->RX[vcId].CBUF_ERRLINE.R;
                    errorS.lineLengthErr[vcId].linePoz &= 0xffffu;      // clear the 16 bits MSB
                    errorS.lineLengthErr[vcId].linePoz |= (pRegs->RX[vcId].CBUF_RXLINE.R << 16u);
                }
                toCall |= errorS.errMaskVC[vcId];                       // accumulate the callback necessity
                errorS.errMaskU |= errorS.errMaskVC[vcId];              // accumulate the error masks
                mask <<= 1u;
            }
            else
            {   // the VC is not initialized
                mask <<= 2u;
            }
        }   // for
        //THIRD step - callback to the application
        if (toCall == (uint32_t)0)
        {  // if no flags till here
            errorS.errMaskU = (uint32_t)RSDK_CSI2_ERR_SPURIOUS_PKT;
        }
        pDriverState->pCallback[(uint8_t)RSDK_CSI2_PATH_ERR_IRQ_ID](&errorS);
    } // if (pRegs != NULL)
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PKT_ERR_ISR, (uint32_t)CSI2_SEQ_END);
}
/* Csi2IrqHandlerPathErr *************************/

#ifndef linux

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 0/ errors in protocol & packet level irq.
 *
 */
static void Csi2IrqHandlerPathErr0(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerPathErr(RSDK_CSI2_UNIT_0);
}
/* Csi2IrqHandlerPathErr0 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 1/ errors in protocol & packet level irq.
 *
 */
static void Csi2IrqHandlerPathErr1(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerPathErr(RSDK_CSI2_UNIT_1);
}
/* Csi2IrqHandlerPathErr1 *************************/

#ifdef S32R45

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 2/ errors in protocol & packet level irq.
 *
 */
static void Csi2IrqHandlerPathErr2(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerPathErr(RSDK_CSI2_UNIT_2);
}
/* Csi2IrqHandlerPathErr2 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 3/ errors in protocol & packet level irq.
 *
 */
static void Csi2IrqHandlerPathErr3(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerPathErr(RSDK_CSI2_UNIT_3);
}
/* Csi2IrqHandlerPathErr3 *************************/

#endif  //#ifdef S32R45
#endif  //#ifndef linux

/*================================================================================================*/
/*
 * @brief       VC statistics process.
 * @details     At the end of each chirp, the VC statistics are processed.
 *
 * @param[in]   pVCState    - pointer to the unit/VC driver state
 *              crtLine     - the current line/chirp reported by the interface
 *
 */
static void Csi2ProcessChannelStatistics(rsdkCsi2VCDriverState_t *pVCState, uint16_t crtLine)
{
    uint32_t idChannel, numChannel, sum32;
    int64_t  sumBase;
    uint64_t sumU;
    void     *pMapMem;
    rsdkCsi2ChChirpStat_t *pStat;  // pointer to statistics

    // determine the exact position for the received statistics
#ifdef linux
    rsdkCsi2ChChirpStat_t stats[RSDK_CSI2_MAX_CHANNEL];
    pMapMem = pVCState->pVirtData;
#else
    pMapMem = pVCState->pVCParams->pBufData;
#endif

    numChannel = (uint32_t)Csi2PlatformGetChannelNum(
        (rsdkCsi2VCDriverState_t *)(void *)pVCState);  // get the real number of channels
    // the real pointer to the statistics, according the current line
    pStat = (rsdkCsi2ChChirpStat_t *)(void *)((uint8_t *)pMapMem +
                                              ((uint32_t)pVCState->pVCParams->bufLineLen * (uint32_t)crtLine) +
                                              (numChannel * (uint32_t)RSDK_CSI2_BYTES_PER_SAMPLE *
                                               (uint32_t)pVCState->pVCParams->expectedNumSamples));  
#ifdef linux
    // for Linux : copy the data from the memory to local stack
    (void)memcpy(stats, pStat, sizeof(rsdkCsi2ChChirpStat_t) * numChannel);
    pStat = stats;                      // the pointer to statistics is changed
#endif
    // analyze statistics for each channel
    for (idChannel = (uint32_t)RSDK_CSI2_CHANNEL_A; idChannel < numChannel; idChannel++)
    {
        // total channel samples sum, without unnecessary bits




        // little endian for ARM
        sum32 = pStat->channelSumScr;

        sumU = (uint64_t)sum32;
        if ((sumU & 0x80000000u) != 0u)
        {
            sumU += 0xffffffff00000000u;
        }
        sumBase = (int64_t)sumU;
        pVCState->statDC[idChannel].channelSum += (sumBase / (int64_t)CSI2_STAT_CHANNEL_SUM_ADJUST);
        pVCState->statDC[idChannel].channelBitToggle |= pStat->channelBitToggle;  // toggled bits
        if (pVCState->statDC[idChannel].channelMin > pStat->channelMin)
        {
            pVCState->statDC[idChannel].channelMin = pStat->channelMin;  // current min value
        }
        if (pVCState->statDC[idChannel].channelMax < pStat->channelMax)
        {
            pVCState->statDC[idChannel].channelMax = pStat->channelMax;  // current max value
        }

        pStat++;  // next channel statistics
    }
}
/* Csi2ProcessChannelStatistics *************************/

/*================================================================================================*/
/*
 * @brief       VC FrameEnd process.
 * @details     Process a Frame End signal for the VC : do the VC general statistics.
 *              Return the status of the bit toggle at VC level.
 *
 * @param[in]   pRegs
 *              vcId
 * @return      -   0 if no problems
 *                  bits not toggled if necessary and/or 1 on MSbit if NEXTLINE is incorrect
 */
static uint32_t Csi2ProcessChannelFrameEnd(volatile struct MIPICSI2_REG_STRUCT *pRegs, const uint8_t vcId,
                                           rsdkCsi2VCDriverState_t *pVCState, rsdkCsi2Report_t *pErrorS,
                                           rsdkCsi2AutoDCComputeTime_t statFlag)
{
    uint32_t           idChannel, numChannel, dcAdj, rez;
    uint16_t           fToggle, iToggle;
    int64_t            dcTmp;
    int64_t            dcDiv;
    volatile uint32_t *pDCReg;

    pDCReg = (volatile uint32_t *)(volatile void *)((volatile uint8_t *)&pRegs->RX_CBUF0_CHNLOFFSET0_0 +
                                                    (vcId * RSDK_CSI2_VC_CFG_OFFSET));
    numChannel = (uint32_t)Csi2PlatformGetChannelNum(pVCState);  // get the real number of channels
    if (numChannel > (uint32_t)RSDK_CSI2_MAX_CHANNEL)
    {
        numChannel = (uint32_t)RSDK_CSI2_MAX_CHANNEL;
    }
    // divisor for channel statistic sum
    dcDiv = (int64_t)pVCState->pVCParams->expectedNumSamples;
    if (statFlag == RSDK_CSI2_STAT_EVERY_LINE)
    {
        dcDiv *= (int64_t)pVCState->pVCParams->expectedNumLines;
    }
    else
    {
        if (statFlag == RSDK_CSI2_STAT_AT_FE)
        {
            dcDiv *= (int64_t)pVCState->pVCParams->bufNumLines;
        }
    }
    fToggle = 0;  // toggle bits indicator
    dcAdj = 0u;
    for (idChannel = (uint32_t)RSDK_CSI2_CHANNEL_A; idChannel < numChannel; idChannel++)
    {
        // change the DC offsets, according the new values
        if (pVCState->statDC[idChannel].reqChannelDC == (int16_t)RSDK_CSI2_OFFSET_AUTOCOMPUTE)
        {                                                            // auto compute DC offset
            dcTmp = pVCState->statDC[idChannel].channelSum / dcDiv;  // the real DC offset
            // adjust the offset to be able to set the registry correctly
            dcTmp = dcTmp * (int64_t)CSI2_STAT_DC_OFFSET_ADJUST;
            pVCState->statDC[idChannel].channelDC = (int16_t)dcTmp;
            pVCState->statDC[idChannel].channelSum = 0;  // reset the channel statistic sum
        }
        if ((idChannel & 1u) == 0u)
        {  // even number => first channel processed
            dcAdj = (((uint32_t)pVCState->statDC[idChannel].channelDC) & (uint32_t)0xffff);
        }
        else
        {  // odd number => second channel
            dcAdj |= (((uint32_t)pVCState->statDC[idChannel].channelDC) << 16u);
        }
        *pDCReg = dcAdj;  // set the new DC offset value
        if ((idChannel & 1u) == 1u)
        {  // odd number => second channel processed
            pDCReg++;
            dcAdj = 0u;
        }
        // test for bit toggle
        iToggle = (uint16_t)((~pVCState->statDC[idChannel].channelBitToggle) & CSI2_TOGGLE_BITS_MASK);
        fToggle |= iToggle;                                 // reversed toggled bit status
        pErrorS->notToggledBits[idChannel] = iToggle;       // the current bits not toggled (1=not toggled)
        pVCState->statDC[idChannel].channelBitToggle = 0u;  // reset toggle bits for next frame
    }
    // process bit toggle
    rez = 0u;
    if (fToggle != 0u)
    {
        rez = (uint32_t)RSDK_CSI2_EVT_BIT_NOT_TOGGLE;  // report issue to caller
    }







    return rez;
}
/* Csi2ProcessChannelFrameEnd *************************/

/*================================================================================================*/
/*
 * @brief       VC events process.
 * @details     Process the events of the virtual channel
 *              Return the array of VC which reported FrameEnd.
 *
 * @param[in]   pRegs
 *              vcId
 * @return      -   0 if no problems
 *                  bits not toggled if necessary and/or 1 on MSbit if NEXTLINE is incorrect
 */
static uint32_t Csi2ProcessVcEvents(rsdkCsi2DriverParams_t *pDriverState, rsdkCsi2Report_t *errorS,
                                    const rsdkCsi2UnitId_t iUnit, uint32_t *pCallState, uint32_t *pOptFlag)
{
    uint32_t                             vcId;  // virtual channel ID
    uint32_t                             toCall, maskW, maskEv, regStat, optionalFlags;
    uint32_t                             vcIdFe;
    volatile struct MIPICSI2_REG_STRUCT *pRegs;  // registry pointer
    rsdkCsi2VCDriverState_t             *pVCDriverState;

    pVCDriverState = pDriverState->workingParamVC;
    pRegs = gpMipiCsi2Regs[(uint8_t)iUnit];  // get the  registry pointer for the unit
    optionalFlags = 0;                       // no optional flags
    toCall = 0;
    regStat = pRegs->RX_VCINTRS.R;
    pRegs->RX_VCINTRS.R = regStat;        // clear the bits
    vcIdFe = (uint32_t)RSDK_CSI2_MAX_VC;  // FE event unknown
    for (vcId = (uint32_t)RSDK_CSI2_VC_0; vcId < (uint32_t)RSDK_CSI2_MAX_VC; vcId++)
    {
        errorS->evtMaskVC[vcId] = 0u;                           // no errors for the beginning
        maskW = regStat &
                (uint32_t)(RSDK_CSI2_EVT_FRAME_START | RSDK_CSI2_EVT_FRAME_END | RSDK_CSI2_EVT_SHORT_PACKET);
        if (pVCDriverState->pVCParams != NULL)
        {
            // check for events
            if (maskW != (uint32_t)0)
            {  // there is at least one signal set
                optionalFlags += maskW;                         // set the optionalFlags
                if((maskW & RSDK_CSI2_EVT_FRAME_START) != 0u)
                {
                    // reset the current received line in buffer counter
                    pVCDriverState->lastReceivedChirpLine = RSDK_CSI2_CHIRP_NOT_STARTED;
                }
                if((maskW & RSDK_CSI2_EVT_FRAME_END) != 0u)
                {
                    // it is a frame-end event on this radar data channel
                    // reset the current received line in buffer counter
                    pVCDriverState->lastReceivedChirpLine = RSDK_CSI2_CHIRP_NOT_STARTED;
                    vcIdFe <<= CSI2_VC_NUM_TOTALBITS;
                    vcIdFe += vcId;                             // keep the VC id which generated the event
                }
                if((maskW & RSDK_CSI2_EVT_SHORT_PACKET) != 0u)
                {
                    errorS->shortPackets[vcId].dataID = (uint8_t)pRegs->RX_GNSPR_VC[vcId].B.DATAID;
                    errorS->shortPackets[vcId].dataVal = (uint16_t)pRegs->RX_GNSPR_VC[vcId].B.DATA;
                }
                maskEv = maskW & pVCDriverState->pVCParams->vcEventsReq;
                if(maskEv != 0u)
                {   // events to be reported
                    errorS->evtMaskVC[vcId] |= (uint8_t)maskEv;
                    toCall = 1;
                }
            }
        }
        if((pDriverState->pMDParams[vcId] != NULL) &&
                ((maskW & pDriverState->pMDParams[vcId]->vcEventsReq) != 0u))
        {
            // it is a frame-end event on this virtual channel and for meta data flow the FRAME_END event was requested
            Csi2PlatformIncFramesCounter(iUnit, vcId + (uint32_t)RSDK_CSI2_MAX_VC);     // increment the frames
            errorS->evtMaskVC[vcId] |= (uint8_t)RSDK_CSI2_EVT_FRAME_END;
            toCall = 1;
        }
        regStat >>= CSI2_VC_NUM_TOTALBITS;
        pVCDriverState++;                                       // move to next VC driver state
    } // for(vcId = (uint32_t)RSDK_CSI2_VC_0;...
    *pCallState |= toCall;
    *pOptFlag |= optionalFlags;
    return vcIdFe;
}
/* Csi2ProcessVcEvents *************************/

/*================================================================================================*/
/*
 * @brief       Generic interrupt procedure for events irq.
 *
 * @param[in]   iUnit   - unit id, 0...3
 *
 */
#ifndef linux
static
#endif
    void
    Csi2IrqHandlerEvents(const rsdkCsi2UnitId_t iUnit)
{
    uint16_t                             i;
    uint32_t                             j;
    uint32_t                             vcIdFe;      // virtual channel ID which generated the FE event
    uint32_t                             workVcIdFe;  // virtual channel ID updated
    uint32_t                             regStat, toCall, optionalFlags, nextLine;
    rsdkCsi2Report_t                     errorS;          // error structure
    volatile struct MIPICSI2_REG_STRUCT *pRegs;           // registry pointer
    rsdkCsi2DriverParams_t              *pDriverState;    // unit driver state pointer
    rsdkCsi2VCDriverState_t             *pVCDriverState;  // VC driver state pointer

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_DATA_ERR_ISR, (uint32_t)CSI2_SEQ_BEGIN);
    pRegs = gpMipiCsi2Regs[(uint8_t)iUnit];  // get the  registry pointer for the unit
    if (pRegs != NULL)
    {
        pDriverState = &gCsi2Settings[(uint8_t)iUnit];
        errorS.unitId = iUnit;
        (void)memset(&errorS.errMaskU, 0, sizeof(uint32_t) * 6u);   // set the masks to 0

        // FIRST step - process linedone event
        optionalFlags = pRegs->RX_CHNL_INTRS.R;                         // read the LINEDONE bit
        pRegs->RX_CHNL_INTRS.R = optionalFlags;                         // clear the bit
        if ((optionalFlags & CSI2_CHNL_INTRS_LINEDONE) != 0u)
        {
            workVcIdFe = pRegs->RX_DATAIDR.B.VCID;                      // vc with last received data
            pVCDriverState = &pDriverState->workingParamVC[workVcIdFe]; // get the pointer to VC params
            nextLine = pVCDriverState->lastReceivedChirpLine;           // increase the line number
            nextLine++;
            // do processing for application requests (line done trigger)
            if ((pVCDriverState->pVCParams != NULL) &&
                    ((pVCDriverState->pVCParams->vcEventsReq & (uint32_t)RSDK_CSI2_EVT_LINE_END) != 0u) &&
                    ((nextLine % pVCDriverState->pVCParams->bufNumLinesTrigger) == 0u))
            {
                    errorS.evtMaskVC[workVcIdFe] = (uint8_t)RSDK_CSI2_EVT_LINE_END; // mask for application
                    // callback for LINEDONE only
                    // if both LINEDONE and FrameEnd occurred, the call for FE will be done at handler end
                    pDriverState->pCallback[(uint8_t)RSDK_CSI2_EVENTS_IRQ_ID](&errorS);
            }
            pVCDriverState->lastReceivedChirpLine = (uint16_t)nextLine;             // keep the line for the next irq
            nextLine = pVCDriverState->lastReceivedBufLine;                         // keep the buffer pointer
            // do processing for VC/channels statistics
            if ((pDriverState->statisticsFlag == RSDK_CSI2_STAT_EVERY_LINE) && (pVCDriverState->pVCParams != NULL))
            {
                Csi2ProcessChannelStatistics(pVCDriverState, (uint16_t)nextLine);
            }
            nextLine++;
            pVCDriverState->lastReceivedBufLine = (uint16_t)nextLine;
        }

        toCall = 0u;                                                    // no necessary to call the application
        // SECOND step = check for VC level Packet & Protocol errors
        vcIdFe = Csi2ProcessVcEvents(pDriverState, &errorS, iUnit, &toCall, &optionalFlags);

        // THIRD step - process frame end event (final statistics management)
        while (vcIdFe != (uint32_t)RSDK_CSI2_MAX_VC)
        {  // VC identified, process FE
            workVcIdFe = vcIdFe & (uint32_t)CSI2_VC_NUM_BITSMASK;
            vcIdFe >>= CSI2_VC_NUM_TOTALBITS;
            if ((pDriverState->statisticsFlag != RSDK_CSI2_STAT_NO) && (workVcIdFe < (uint32_t)RSDK_CSI2_MAX_VC))
            {
                pVCDriverState = &pDriverState->workingParamVC[workVcIdFe];
                j = 0u;
                if (pDriverState->statisticsFlag == RSDK_CSI2_STAT_AT_FE)
                {
                    j = pVCDriverState->pVCParams->bufNumLines;
                }
                if (pDriverState->statisticsFlag == RSDK_CSI2_STAT_LAST_LINE)
                {
                    j = 1;
                }
                for (i = 0u; i < j; i++)
                {
                    // do processing for VC/channels statistics
                    Csi2ProcessChannelStatistics(pVCDriverState, i);
                }
                // process FrameEnd => statistics, so is possible to have toggle_bit problems
                regStat =
                    Csi2ProcessChannelFrameEnd(pRegs, (uint8_t)workVcIdFe, &pDriverState->workingParamVC[workVcIdFe],
                                               &errorS, pDriverState->statisticsFlag);
                // be sure next time will detect correct the first line if necessary
                pVCDriverState->lastReceivedChirpLine = RSDK_CSI2_CHIRP_NOT_STARTED;
                pVCDriverState->lastReceivedBufLine = RSDK_CSI2_CHIRP_NOT_STARTED;
                if (((pVCDriverState->pVCParams->vcEventsReq & (uint32_t)RSDK_CSI2_EVT_BIT_NOT_TOGGLE) != 0u) &&
                    ((regStat & RSDK_CSI2_EVT_BIT_NOT_TOGGLE) != 0u))
                {
                    errorS.evtMaskVC[workVcIdFe] |= (uint8_t)RSDK_CSI2_EVT_BIT_NOT_TOGGLE;
                    errorS.notToggledBits[workVcIdFe] = (uint16_t)regStat;
                    toCall++;
                }







            }
            Csi2PlatformIncFramesCounter(iUnit, workVcIdFe);
        }   // while
        // FOURTH step - application callback
        if ((toCall == (uint32_t)0) && (optionalFlags == (uint32_t)0))
        {  // no optional or error flags set => spurious
            errorS.errMaskU |= (uint32_t)RSDK_CSI2_ERR_SPURIOUS_EVT;
            toCall++;                                                   // must call the application
        }
        if (toCall != (uint32_t)0)
        {
            pDriverState->pCallback[(uint8_t)RSDK_CSI2_EVENTS_IRQ_ID](&errorS);
        }
    }   // if (pRegs != NULL)
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_DATA_ERR_ISR, (uint32_t)CSI2_SEQ_END);
}
/* Csi2IrqHandlerEvents *************************/

#ifndef linux

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 0/ events irq.
 *
 */
static void Csi2IrqHandlerEvents0(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerEvents(RSDK_CSI2_UNIT_0);
}
/* Csi2IrqHandlerEvents0 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 1/ events irq.
 *
 */
static void Csi2IrqHandlerEvents1(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerEvents(RSDK_CSI2_UNIT_1);
}
/* Csi2IrqHandlerEvents1 *************************/

#ifdef S32R45

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 2/ events irq.
 *
 */
static void Csi2IrqHandlerEvents2(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerEvents(RSDK_CSI2_UNIT_2);
}
/* Csi2IrqHandlerEvents2 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 3/ events irq.
 *
 */
static void Csi2IrqHandlerEvents3(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerEvents(RSDK_CSI2_UNIT_3);
}
/* Csi2IrqHandlerEvents3 *************************/

#endif  //#ifdef S32R45
#endif  //#ifndef linux

/*================================================================================================*/
/*
 * @brief       Generic interrupt procedure for tx level irq.
 *
 * @param[in]   iUnit   - unit id
 *
 */
#ifndef linux
static void Csi2IrqHandlerTx(const rsdkCsi2UnitId_t iUnit)
{
    UNUSED_ARG(iUnit);
    // TBD!!!
}
/* Csi2IrqHandlerTx *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 0/ tx level irq.
 *
 */
static void Csi2IrqHandlerTx0(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerTx(RSDK_CSI2_UNIT_0);
}
/* Csi2IrqHandlerTx0 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 1/ tx level irq.
 *
 */
static void Csi2IrqHandlerTx1(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerTx(RSDK_CSI2_UNIT_1);
}
/* Csi2IrqHandlerTx1 *************************/

#ifdef S32R45

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 2/ tx level irq.
 *
 */
static void Csi2IrqHandlerTx2(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerTx(RSDK_CSI2_UNIT_2);
}
/* Csi2IrqHandlerTx2 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 3/ tx level irq.
 *
 */
static void Csi2IrqHandlerTx3(
#ifdef __ZEPHYR__
        const void * pParams
#else
        void
#endif
)
{
#ifdef __ZEPHYR__
    UNUSED_ARG(pParams);
#endif
    Csi2IrqHandlerTx(RSDK_CSI2_UNIT_3);
}
/* Csi2IrqHandlerTx3 *************************/

#endif  //#ifdef S32R45
#endif  //#ifndef linux

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*================================================================================================*/
/*
 * @brief       Initialize interrupts at Unit level
 *
 * @param[in]   iUnit   - unit ID, RSDK_CSI2_UNIT_0 ... MAX
 * @param[in]   pRegs   - pointer to unit registry
 * @param[in]   pParams - pointer to CSI2 unit initialization parameters
 *
 */
rsdkStatus_t Csi2InitUIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                          const rsdkCsi2InitParams_t *pInitParams)
{
    rsdkStatus_t rez;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_INIT_STEP2, (uint32_t)CSI2_SEQ_BEGIN);
    rez = Csi2InitEventIrq(iUnit, pRegs, pInitParams);
    if (rez == RSDK_SUCCESS)
    {
        rez = Csi2InitRxIrq(iUnit, pRegs, pInitParams);
    }
    if (rez == RSDK_SUCCESS)
    {
        rez = Csi2InitPathIrq(iUnit, pRegs, pInitParams);
    }
    if (rez == RSDK_SUCCESS)
    {
        rez = Csi2InitTxIrq(iUnit, pRegs, pInitParams);
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_INIT_STEP2, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2InitUIrq *************************/

// clang-format on


#ifdef __cplusplus
}
#endif
