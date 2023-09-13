/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**
*   @file
*   @implements Csi2_Irq_Handlers.c_Artifact
*
*   @addtogroup CSI2
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "CDD_Csi2.h"
#include "rsdk_version.h"
#if defined(linux)
    #include <linux/string.h>
#else
    #include "string.h"
#endif


/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
/**
 * @brief       Structure for channel statistics, added to the chirp data by the hardware.
 * @details     It is added at the end of each chirp.
 *
 */
typedef struct __attribute__((__packed__)) {
    uint32  channelSumScr;          /* chirp channel sum, scrambled :
                                     *  in memory (LSB+1)(LSB)(MSB)(MSB-1)
                                     *  must be translated to (signed)(MSB,MSB-1,LSB+1,LSB)                         */
    sint16  channelMin;             /* chirp channel min                                                            */
    sint16  channelMax;             /* chirp channel max                                                            */
    uint16  channelBitToggle;       /* chirp channel bit toggle                                                     */
}Csi2_ChChirpStatType;


/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/
/* masks for VC PnP errors                                                                                          */
#define CSI2_IRQ_VC_ECC1_MASK           0x01u       /* ECC one bit error                                            */
#define CSI2_IRQ_VC_ECC2_MASK           0x02u       /* ECC two bits error                                           */
#define CSI2_IRQ_VC_FSYN_MASK           0x04u       /* frame sync error                                             */
#define CSI2_IRQ_VC_FDAT_MASK           0x08u       /* frame data error (==CRC)                                     */
#define CSI2_IRQ_VC_CRC_MASK            0x10u       /* CRC error                                                    */
#define CSI2_IRQ_VC_IVID_MASK           0x20u       /* invalid frame ID                                             */

#define CSI2_IRQ_VC_LIN0LENERR_MASK     1u          /* line length errors for VC 0 only                             */

#define CSI2_CHIRP_NOT_STARTED          0u          /* value for first chirp line received                          */
#define CSI2_VC_NUM_TOTALBITS           3u          /* bits length for VC number                                    */
#define CSI2_VC_NUM_BITSMASK            0x7u        /* mask for VC number (only the lowest part)                    */

#define CSI2_STAT_DC_OFFSET_ADJUST      0x4         /* multiplier for the channel statistic value                   */
#define CSI2_TOGGLE_BITS_MASK           0xfff0u     /* mask for toggle bits                                         */

#define CSI2_BYTES_PER_SAMPLE           2u          /* sample buffer length, only for radar                         */
#define CSI2_STAT_CHANNEL_SUM_ADJUST    0x1         /* divider for the channel samples sum                          */
#define CSI2_VC_CFG_OFFSET              0x10u       /* offset between two similar VC config registry                */
#define CSI2_OFFSET_AUTOCOMPUTE         0x7fff      /* definition for auto computing offset for incoming data       */


/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/
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
static void Csi2_IrqHandlerRxErrVC(const uint32 vcId, const uint32 regStat, Csi2_ErrorReportType *pErrorS,
                                  const volatile GENERIC_CSI2_Type *pRegs)
{
    uint32      regV;

    pErrorS->errMaskVC[vcId] = (uint32)CSI2_ERR_NO;                         /* no errors for the beginning                  */
    /* ECC error - 2 bits                                                                                           */
    if ((regStat & (uint32)CSI2_IRQ_VC_ECC2_MASK) != 0u)
    {
        pErrorS->errMaskVC[vcId] |= (uint32)CSI2_ERR_PACK_ECC2;
    }
    /* ECC error - one bit                                                                                          */
    if ((regStat & (uint32)CSI2_IRQ_VC_ECC1_MASK) != 0u)
    {
        pErrorS->errMaskVC[vcId] += (uint32)CSI2_ERR_PACK_ECC1;     /* set the mask first                           */
        pErrorS->eccOneBitErrPos[vcId] = (uint8)pRegs->RX_VC[vcId].ERRPOS;  /* copy the appropriate value           */
    }
    /* CRC error                                                                                                    */
    if ((regStat & (uint32)CSI2_IRQ_VC_CRC_MASK) != 0u)
    {
        pErrorS->errMaskVC[vcId] += (uint32)CSI2_ERR_PACK_CRC;      /* set the mask                                 */
        regV = pRegs->CRC_REGISTER
                 ;
        pErrorS->expectedCRC[vcId] = (uint16)(regV >> 16u);         /* save both, expected CRC                      */
        pErrorS->receivedCRC[vcId] = (uint16)(regV);                /*   & received CRC                             */
    }
    /* Frame sync error                                                                                             */
    if ((regStat & (uint32)CSI2_IRQ_VC_FSYN_MASK) != 0u)
    {
        pErrorS->errMaskVC[vcId] += (uint32)CSI2_ERR_PACK_SYNC;
    }
    /* Data type error                                                                                              */
    if ((regStat & (uint32)CSI2_IRQ_VC_IVID_MASK) != 0u)
    {
        pErrorS->errMaskVC[vcId] += (uint32)CSI2_ERR_PACK_ID;
        regV = pRegs->RX_INVIDR;
        if (((regV >> 6u) & 0x3u) == vcId)
        {       /* get the pachet ID only if the correct VC     */
            pErrorS->invalidPacketID[vcId] = (uint8)(regV & 0x3fu);
        }
    }
    /* Frame sync error                                                                                             */
    if ((regStat & (uint32)CSI2_IRQ_VC_FDAT_MASK) != 0u)
    {
        pErrorS->errMaskVC[vcId] += (uint32)CSI2_ERR_PACK_DATA;
    }
}
/* Csi2_IrqHandlerRxErrVC *************************/


/*================================================================================================*/
/*
 * @brief       VC events process.
 * @details     Process the events of the virtual channel
 *              Return the array of VC which reported FrameEnd.
 *
 * @param[in]   pRegs
 *              vcId
 * @return      -   a 32 bits value, containing all vcId with FrameEnd reported
 *
 */
static uint32 Csi2_ProcessVcEvents(Csi2_DriverParamsType *pDriverState, Csi2_ErrorReportType *errorS,
                                    const Csi2_UnitIdType iUnit, uint32 *pCallState, uint32 *pOptFlag)
{
    uint32                              vcId;                           /* virtual channel ID                       */
    uint32                              toCall, maskW, maskEv, optionalFlags;
    static uint32                       regStat;
    uint32                              vcIdFe;
    volatile GENERIC_CSI2_Type          *pRegs;                         /* registry pointer                         */
    const Csi2_VCDriverStateType        *pVCDriverState;

    pVCDriverState = pDriverState->workingParamVC;
    pRegs = gMipiCsi2RegsPtr[(uint8)iUnit];                               /* get the  registry pointer for the unit   */
    optionalFlags = 0;                                                  /* no optional flags                        */
    toCall = 0;
    regStat = pRegs->RX_VCINTRS;
    pRegs->RX_VCINTRS = regStat;                                        /* clear the bits                           */
    vcIdFe = (uint32)CSI2_MAX_VC;                                       /* FE event unknown                         */
    for (vcId = (uint32)CSI2_VC_0; vcId < (uint32)CSI2_MAX_VC; vcId++)
    {
        errorS->evtMaskVC[vcId] = 0u;                                   /* no errors for the beginning              */
        if (pVCDriverState->vcParamsPtr != NULL)
        {
            /* check for events                                                                                     */
            maskW = regStat &
                    (uint32)(CSI2_EVT_FRAME_START | CSI2_EVT_FRAME_END | CSI2_EVT_SHORT_PACKET);
            if (maskW != 0u)
            {  /* there is at least one signal set                                                                  */
                optionalFlags += maskW;                                 /* set the optionalFlags                    */
                if((maskW & CSI2_EVT_FRAME_START) != 0u)
                {
                    /* reset the current received line in buffer counter                                            */
                    pDriverState->workingParamVC[vcId].lastReceivedChirpLine = CSI2_CHIRP_NOT_STARTED;
                }
                if((maskW & CSI2_EVT_FRAME_END) != 0u)
                {
                    /* reset the current received line in buffer counter                                            */
                    pDriverState->workingParamVC[vcId].lastReceivedChirpLine = CSI2_CHIRP_NOT_STARTED;
                    vcIdFe <<= CSI2_VC_NUM_TOTALBITS;
                    vcIdFe += vcId;                                     /* keep the VC id which generated the event */
                }
                if((maskW & CSI2_EVT_SHORT_PACKET) != 0u)
                {
                    errorS->shortPackets[vcId].dataID = (uint8)((pRegs->RX_GNSPR_VC[vcId] &
                            MIPICSI2_RX_GNSPR_VC_DATAID_MASK) >> MIPICSI2_RX_GNSPR_VC_DATAID_SHIFT);
                    errorS->shortPackets[vcId].dataVal = (uint16)((pRegs->RX_GNSPR_VC[vcId] &
                            MIPICSI2_RX_GNSPR_VC_DATA_MASK) >> MIPICSI2_RX_GNSPR_VC_DATA_SHIFT);
                }
                maskEv = maskW & pVCDriverState->eventsMask;
                if(maskEv != 0u)
                {           /* events to be reported    */
                    errorS->evtMaskVC[vcId] |= (uint8)maskEv;
                    toCall++;
                }
            }
        }
        regStat >>= CSI2_VC_NUM_TOTALBITS;
        pVCDriverState++;                                               /* move to next VC driver state             */
    }  /* for(vcId = (uint32)RSDK_CSI2_VC_0;...   */
    *pCallState |= toCall;
    *pOptFlag |= optionalFlags;
    return vcIdFe;
}
/* Csi2_ProcessVcEvents *************************/

#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
/*================================================================================================*/
/*
 * @brief       VC FrameEnd process.
 * @details     Process a Frame End signal for the VC : do the VC general statistics.
 *              Return the status of the bit toggle at VC level and next line not aligned.
 *
 * @param[in]   pRegs
 *              vcId
 * @return      -   0 if no problems
 *                  bits not toggled if necessary and/or 1 on MSbit if NEXTLINE is incorrect
 */
static uint32 Csi2_ProcessChannelFrameEnd(volatile GENERIC_CSI2_Type *pRegs, const uint8 vcId,
                 Csi2_VCDriverStateType *pVCState, Csi2_ErrorReportType *pErrorS,Csi2_AutoDCComputeTimeType statFlag)
{
    uint32              idChannel, numChannel, rez;
    uint16              fToggle, iToggle;

#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
    /* autoDC data processing                   */
    uint32              dcAdj;
    sint64              dcTmp;
    sint64              dcDiv;
    volatile uint32     *pDCReg;

    switch(vcId)
    {
        case 0u:
            pDCReg = (volatile uint32 *)(&pRegs->RX_CBUF0_CHNLOFFSET0_0);
            break;
        case 1u:
            pDCReg = (volatile uint32 *)(&pRegs->RX_CBUF1_CHNLOFFSET0_1);
            break;
        case 2u:
            pDCReg = (volatile uint32 *)(&pRegs->RX_CBUF2_CHNLOFFSET0_2);
            break;
        default:
            pDCReg = (volatile uint32 *)(&pRegs->RX_CBUF3_CHNLOFFSET0_3);
            break;
    }
    numChannel = (uint32)Csi2_GetChannelNum(pVCState);      /* get the real number of channels                      */
    if (numChannel > (uint32)CSI2_MAX_CHANNEL)
    {
        numChannel = (uint32)CSI2_MAX_CHANNEL;
    }
    /* divisor for channel statistic sum                                                                            */
    dcDiv = (sint64)pVCState->vcParamsPtr->expectedNumSamples;
    if (statFlag == CSI2_AUTODC_EVERY_LINE)
    {
        dcDiv *= (sint64)pVCState->vcParamsPtr->expectedNumLines;
    }
    else
    {
        if (statFlag == CSI2_AUTODC_AT_FE)
        {
            dcDiv *= (sint64)pVCState->vcParamsPtr->bufNumLines;
        }
    }
    dcAdj = 0u;
    for (idChannel = (uint32)CSI2_CHANNEL_A; idChannel < numChannel; idChannel++)
    {
        /* change the DC offsets, according the new values                                                          */
        if (pVCState->statDC[idChannel].reqChannelDC == (sint16)CSI2_OFFSET_AUTOCOMPUTE)
        {                                                           /* auto compute DC offset                       */
            dcTmp = pVCState->statDC[idChannel].channelSum / dcDiv; /* the real DC offset                           */
            /* adjust the offset to be able to set the registry correctly                                           */
            dcTmp = dcTmp * (sint64)CSI2_STAT_DC_OFFSET_ADJUST;
            pVCState->statDC[idChannel].channelDC = (sint16)dcTmp;
            pVCState->statDC[idChannel].channelSum = 0;             /* reset the channel statistic sum              */
        }
        if ((idChannel & 1u) == 0u)
        {  /* even number => first channel processed                                                                */
            dcAdj = (((uint32)pVCState->statDC[idChannel].channelDC) & (uint32)0xffff);
        }
        else
        {  /* odd number => second channel                                                                          */
            dcAdj |= (((uint32)pVCState->statDC[idChannel].channelDC) << 16u);
        }
        *pDCReg = dcAdj;                                            /* set the new DC offset value                  */
        if ((idChannel & 1u) == 1u)
        {  /* odd number => second channel processed                                                                */
            pDCReg++;
            dcAdj = 0u;
        }
    }
#endif  /* #if (CSI2_AUTO_DC_COMPENSATION == STD_ON)        */

    rez = 0u;
    /* process bit toggle                                                                                           */
    fToggle = 0u;                                                   /* toggle bits indicator                        */
    for (idChannel = (uint32)CSI2_CHANNEL_A; idChannel < numChannel; idChannel++)
    {
        /* test for bit toggle                                                                                      */
        iToggle = (uint16)((~pVCState->statDC[idChannel].channelBitToggle) & CSI2_TOGGLE_BITS_MASK);
        fToggle |= iToggle;                                         /* reversed toggled bit status                  */
        pErrorS->notToggledBits[idChannel] = iToggle;               /* the current bits not toggled (1=not toggled) */
        pVCState->statDC[idChannel].channelBitToggle = 0u;          /* reset toggle bits for next frame             */
    }
    if (fToggle != 0u)
    {
        rez = (uint32)CSI2_EVT_BIT_NOT_TOGGLE;                      /* report issue to caller                       */
    }
    /* process the next line position   */
    return rez;
}
/* Csi2_ProcessChannelFrameEnd *************************/


/*================================================================================================*/
/*
 * @brief       VC statistics process.
 * @details     At the end of each chirp, the VC statistics are processed.
 *
 * @param[in]   pVCState    - pointer to the unit/VC driver state
 *              crtLine     - the current line/chirp reported by the interface
 *
 */
static void Csi2_ProcessChannelStatistics(Csi2_VCDriverStateType *pVCState, uint16 crtLine)
{
    uint32                  idChannel, numChannel, statOffset;
    const uint8             *pMapMem;
    const Csi2_ChChirpStatType* pStat;                          /* pointer to statistics                        */
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
    uint32                  sum32;
    sint64                  sumBase;
    uint64                  sumU;
#endif

    /* determine the exact position for the received statistics                                                     */
#ifdef linux
    pMapMem = pVCState->pVirtData;
#else
    pMapMem = (const uint8 *)pVCState->vcParamsPtr->bufDataPtr;
#endif

    numChannel = (uint32)Csi2_GetChannelNum(
        (Csi2_VCDriverStateType *)pVCState);               /* get the real number of channels               */
    /* the real pointer to the statistics, according the current line                                               */
    statOffset = ((uint32)pVCState->vcParamsPtr->bufLineLen * (uint32)crtLine) +
            (numChannel * (uint32)CSI2_BYTES_PER_SAMPLE * (uint32)pVCState->vcParamsPtr->expectedNumSamples);
    /*
    * @section Csi2_Irq_c_REF_3
    * Violates MISRA C-2012 Advisory Rule 11.3,
    * #A cast shall not be performed between a pointer to object type and a pointer to a different object type
    * This is the only possibility to get the correct pointer to the data to be processed.
    *
    * @section Csi2_Irq_c_REF_4
    * Violates MISRA C-2012 Required Rule 18.4,
    * #The +, -, += and -= operators should not be applied to an expression of pointer type
    * Necessary pointer operation, not possible to use normal pointer association.
    */
    pStat = (const Csi2_ChChirpStatType *)&pMapMem[statOffset];
    /* analyze statistics for each channel                                                                          */
    for (idChannel = (uint32)CSI2_CHANNEL_A; idChannel < numChannel; idChannel++)
    {
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
        /* total channel samples sum, without unnecessary bits
         * little endian for ARM                                                                                    */
        sum32 = pStat->channelSumScr;
        sumU = (uint64)sum32;
        if ((sumU & 0x80000000u) != 0u)
        {
            sumU += 0xffffffff00000000u;
        }
        sumBase = (sint64)sumU;
        pVCState->statDC[idChannel].channelSum += (sumBase / (sint64)CSI2_STAT_CHANNEL_SUM_ADJUST);
        if (pVCState->statDC[idChannel].channelMin > pStat->channelMin)
        {
            pVCState->statDC[idChannel].channelMin = pStat->channelMin;             /* current min value            */
        }
        if (pVCState->statDC[idChannel].channelMax < pStat->channelMax)
        {
            pVCState->statDC[idChannel].channelMax = pStat->channelMax;             /* current max value            */
        }
#endif
        pVCState->statDC[idChannel].channelBitToggle |= pStat->channelBitToggle;  /* toggled bits                   */

        pStat++;                                                                    /* next channel statistics      */
    }
}
/* Csi2_ProcessChannelStatistics *************************/

#endif  /* #if (CSI2_STATISTIC_DATA_USAGE == STD_ON)        */

#if (CSI2_FRAMES_COUNTER_USED == STD_ON)

/*================================================================================================*/
/*
 * @brief       The function increase the current frames counter
 *
 * @param[in]   unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS)
 * @param[in]   vcId      - VC number, RSDK_CSI2_VC_0 ... MAX
 *
 * @return      void
 *
 */
static void Csi2_IncFramesCounter(const Csi2_UnitIdType unitId, const uint32 vcId)
{
    gsCsi2FramesCounter[(uint8)unitId][(uint8)vcId]++;
    if ((gsCsi2FramesCounter[(uint8)unitId][(uint8)vcId] + 1u) == 0u)
    {
        gsCsi2FramesCounter[(uint8)unitId][(uint8)vcId] = 0u;   /* avoid 0xffffffff value, which is invalid     */
    }
}
/* Csi2_IncFramesCounter *************************/
#endif


/*==================================================================================================
*                                         GLOBAL FUNCTIONS
==================================================================================================*/

/*================================================================================================*/
/*
 * @brief       Generic interrupt procedure for PHY error irq.
 *
 * @param[in]   iUnit   - unit id, CSI2_UNIT_0 ... MAX
 *
 */
void Csi2_IrqHandlerRxErr(const Csi2_UnitIdType iUnit)
{
    uint32                              toCall;
    Csi2_ErrorReportType                errorS = {0};       /* error structure                                      */
    uint32                              vcId;               /* index for VC                                         */
    static uint32                       regStat;
    const Csi2_DriverParamsType         *pDriverParams;
    volatile GENERIC_CSI2_Type          *pRegs;             /* pointer to CSI2 registers                            */
    uint32                              i;
    uint32                              mask;

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PHY_ERR_ISR, (uint32_t)CSI2_SEQ_BEGIN);
    pRegs = gMipiCsi2RegsPtr[(uint8)iUnit];                   /* get the  registry pointer for the unit               */
    pDriverParams = &gCsi2Settings[(uint8)iUnit];
    if (pRegs != (GENERIC_CSI2_Type*)NULL_PTR)
    {
        errorS.unitId = (uint8)iUnit;
        /* FIRST step = check for Unit level DPHY errors                                                            */
        mask = 1u;                                          /* mask for single event                                */
        regStat = pRegs->RX_PHYERRIS;
        pRegs->RX_PHYERRIS = regStat;                       /* clear the bits                                       */
        for (i = (uint32)CSI2_LANE_0; i < (uint32)CSI2_MAX_LANE; i++)
        {
            /* check for Error In Synchronization Pattern                                                           */
            if ((regStat & mask) != 0u)
            {
                errorS.errMaskU |= (uint32)CSI2_ERR_PHY_SYNC;
            }
            mask <<= 1u;
            /* check for Multibit Error In Synchronization Pattern                                                  */
            if ((regStat & mask) != 0u)
            {
                errorS.errMaskU |= (uint32)CSI2_ERR_PHY_NO_SYNC;
            }
            mask <<= 1u;
            /* check for Control Command Error                                                                      */
            if ((regStat & mask) != 0u)
            {
                errorS.errMaskU |= (uint32)CSI2_ERR_PHY_ESC;
            }
            mask <<= 1u;
            /* check for Synchronization Error in escape mode                                                       */
            if ((regStat & mask) != 0u)
            {
                errorS.errMaskU |= (uint32)CSI2_ERR_PHY_SESC;
            }
            mask <<= 1u;
            /* check for Control Command Error                                                                      */
            if ((regStat & mask) != 0u)
            {
                errorS.errMaskU |= (uint32)CSI2_ERR_PHY_CTRL;
            }
            mask <<= 1u;
        }  /* for       */

        /* SECOND step = check for VC level Packet & Protocol errors                                                */
        toCall = (uint32)errorS.errMaskU;                   /* the callback necessity at this point                 */
        for (vcId = (uint32)CSI2_VC_0; vcId < (uint32)CSI2_MAX_VC; vcId++)
        {
            regStat = pRegs->RX_VC[vcId].PPERRIS;           /* read the registry                                    */
            pRegs->RX_VC[vcId].PPERRIS = regStat;           /* clean the registry                                   */
            if (pDriverParams->workingParamVC[vcId].vcParamsPtr != (Csi2_VCParamsType*)NULL_PTR) /* only active VC  */
            {
                Csi2_IrqHandlerRxErrVC(vcId, regStat, &errorS, pRegs);
                toCall |= errorS.errMaskVC[vcId];           /* accumulate the callback necessity                    */
                errorS.errMaskU |= errorS.errMaskVC[vcId];  /* accumulate the error masks                           */
            }
        }
        /* THIRD step - callback to the application                                                                 */
        if (toCall == 0u)
        {   /* if no flags till here                                                                                */
            errorS.errMaskU = (uint32)CSI2_ERR_SPURIOUS_PHY;
        }
        CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PHY_ERR_ISR, (uint32_t)CSI2_SEQ_END);
        pDriverParams->pCallback[RSDK_CSI2_RX_ERR_IRQ_ID](&errorS);
    } /* if (pRegs != NULL)     */
}
/* Csi2_IrqHandlerRxErr *************************/


/*================================================================================================*/
/*
 * @brief       Generic interrupt procedure for errors in protocol & packet level irq.
 *
 * @param[in]   iUnit   - unit id, RSDK_CSI2_UNIT_0 ... MAX
 *
 */
void Csi2_IrqHandlerPathErr(const Csi2_UnitIdType iUnit)
{
    uint32                              vcId;                       /* VC index                                     */
    uint32                              mask, toCall;
#if (CSI2_METADATA_DATA_USAGE == STD_ON)
    uint32                              offset;
#endif
    static uint32                       regStat;
    Csi2_ErrorReportType                errorS = {0};               /* reporting error structure                    */
    volatile GENERIC_CSI2_Type          *pRegs;                     /* pointer to unit registry                     */
    const Csi2_DriverParamsType         *pDriverParams;

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PKT_ERR_ISR, (uint32_t)CSI2_SEQ_BEGIN);

    pRegs = gMipiCsi2RegsPtr[(uint8)iUnit];                           /* get the  registry pointer for the unit       */
    pDriverParams = &gCsi2Settings[(uint8)iUnit];
    if (pRegs != NULL)
    {
        errorS.unitId = (uint8)iUnit;
        /* FIRST step = check for Unit path errors                  */
        regStat = pRegs->RX_CHNL_INTRS;                             /* get the erros status                         */
        pRegs->RX_CHNL_INTRS = regStat;                             /* and clear the bits                           */
        /* check for Internal Buffer Overflow                       */
        if ((regStat & MIPICSI2_RX_CHNL_INTRS_BUFFOVF_MASK) != 0u)
        {
            errorS.errMaskU |= (uint32)CSI2_ERR_BUF_OVERFLOW;
        }
        /* check for latency in AXI write channel response          */
        regStat = pRegs->WR_CHNL_INTRS;
        pRegs->WR_CHNL_INTRS = regStat;
        if ((regStat & MIPICSI2_WR_CHNL_INTRS_BUFFOVFAXI_MASK) != 0u)
        {
            errorS.errMaskU |= (uint32)CSI2_ERR_AXI_OVERFLOW;
        }
        /* check for Error Response on the AXI write channel        */
        if ((regStat & MIPICSI2_WR_CHNL_INTRS_ERRRESP_MASK) != 0u)
        {
            errorS.errMaskU |= (uint32)CSI2_ERR_AXI_RESPONSE;
        }
        /* check for overflow of the internal FIFO in the controller                                                */
        regStat = pRegs->CONTROLLER_ERR_STATUS_REGISTER;
        pRegs->CONTROLLER_ERR_STATUS_REGISTER = regStat;
        if ((regStat & MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_FIFO_OVERFLOW_ERROR_MASK) != 0u)
        {
            errorS.errMaskU |= (uint32)CSI2_ERR_FIFO;
        }
        /* check for "PHY has stopped high speed transmission earlier than normal"                                  */
        if ((regStat & MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_EXIT_HS_ERROR_MASK) != 0u)
        {
            errorS.errMaskU |= (uint32)CSI2_ERR_HS_EXIT;
        }
        toCall = errorS.errMaskU;                                   /* the callback necessity                       */
        /* SECOND step = check for VC level path errors             */
        mask = (uint32)CSI2_IRQ_VC_LIN0LENERR_MASK;
        regStat = pRegs->CBUF_INTRS;                                /* the full error status                        */
        pRegs->CBUF_INTRS = regStat;                                /* clear the bits                               */
        for (vcId = (uint32)CSI2_VC_0; vcId < (uint32)CSI2_MAX_VC; vcId++)
        {
            errorS.errMaskVC[vcId] = 0u;                            /* no errors for the start                      */
            errorS.evtMaskVC[vcId] = 0u;                            /* no events for the start                      */
            if (pDriverParams->workingParamVC[vcId].vcParamsPtr != NULL)     /* only for active VC     */
            {
                if ((regStat & mask) != 0u)
                {       /* line length error        */
                    errorS.errMaskVC[vcId] |= (uint32)CSI2_ERR_LINE_LEN;
                    errorS.lineLengthErr[vcId].linePoz = (uint16)pRegs->RX[vcId].CBUF_ERRLINE;
                    errorS.lineLengthErr[vcId].lineLength = (uint16)pRegs->RX[vcId].CBUF_ERRLEN;
                }
                mask <<= 1u;
                if ((regStat & mask) != 0u)
                {       /* line count error         */
                    errorS.errMaskVC[vcId] |= (uint32)CSI2_ERR_LINE_CNT;
                    errorS.lineLengthErr[vcId].linePoz = (uint16)pRegs->RX[vcId].CBUF_RXLINE;
                }
                toCall |= errorS.errMaskVC[vcId];                   /* accumulate the callback necessity            */
                errorS.errMaskU |= errorS.errMaskVC[vcId];          /* accumulate the error masks                   */
                mask <<= 1u;
            }
            else
            {                                                       /* the VC is not initialized                    */
                mask <<= 2u;                                        /* pass to the next VC                          */
            }
        }   /* for      */
#if (CSI2_METADATA_DATA_USAGE == STD_ON)
        offset = (uint32)CSI2_MAX_VC * 2u;                          /* the  real used buffer for metadata           */
        mask = (uint32)1 << (offset * 2u);                          /* the first bit mask for the metadata error    */
        for (vcId = (uint32)CSI2_VC_0; vcId < (uint32)CSI2_MAX_VC; vcId++)
        {
            if (pDriverParams->workingParamVC[vcId].metaDataUsage!= 0u)              /* only for active metadat      */
            {
                if ((regStat & mask) != (uint32)0)
                {  /* line length error     */
                    errorS.errMaskVC[vcId] |= (uint32)CSI2_ERR_LINE_LEN_MD;
                    errorS.lineLengthErr[vcId].linePoz = (uint16)pRegs->RX[vcId + offset].CBUF_ERRLINE;
                    errorS.lineLengthErr[vcId].lineLength &= 0xffffu;   /* clear the 16 bits MSB                    */
                    errorS.lineLengthErr[vcId].lineLength |= (pRegs->RX[vcId + offset].CBUF_ERRLEN << 16u);
                }
                mask <<= 1u;
                if ((regStat & mask) != (uint32)0)
                {  /* line count error      */
                    errorS.errMaskVC[vcId] |= (uint32)CSI2_ERR_LINE_CNT_MD;
                    errorS.lineLengthErr[vcId].linePoz = (uint16)pRegs->RX[vcId + offset].CBUF_ERRLINE;
                    errorS.lineLengthErr[vcId].linePoz &= 0xffffu;      /* clear the 16 bits MSB                    */
                    errorS.lineLengthErr[vcId].linePoz |= (pRegs->RX[vcId + offset].CBUF_RXLINE << 16u);
                }
                toCall |= errorS.errMaskVC[vcId];                       /* accumulate the callback necessity        */
                errorS.errMaskU |= errorS.errMaskVC[vcId];              /* accumulate the error masks               */
                mask <<= 1u;
            }
            else
            {   /* the MD is not initialized        */
                mask <<= 2u;
            }
        }   /* for  */
#endif  /* #if (CSI2_METADATA_DATA_USAGE == STD_ON)                 */
        /*THIRD step - callback to the application                  */
        if (toCall == 0u)
        {  /* if no flags till here     */
            errorS.errMaskU = (uint32)CSI2_ERR_SPURIOUS_PKT;
        }
        CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_PKT_ERR_ISR, (uint32_t)CSI2_SEQ_END);
        if(pDriverParams->pCallback[RSDK_CSI2_PATH_ERR_IRQ_ID] != NULL)
        {
            pDriverParams->pCallback[RSDK_CSI2_PATH_ERR_IRQ_ID](&errorS);
        }
        else
        {
            pDriverParams->pCallback[RSDK_CSI2_RX_ERR_IRQ_ID](&errorS);
        }
    } /* if (pRegs != NULL)     */
}
/* Csi2_IrqHandlerPathErr *************************/


/*================================================================================================*/
/*
 * @brief       Generic interrupt procedure for events irq.
 *
 * @param[in]   iUnit   - unit id, 0...3
 *
 */
void Csi2_IrqHandlerEvents(const Csi2_UnitIdType iUnit)
{
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
    uint16                              i;
    uint32                              j;
    static uint32                       regStat;
#endif
    uint32                              vcIdFe;                 /* virtual channel ID which generated the FE event  */
    uint32                              workVcIdFe;             /* virtual channel ID updated                       */
    uint32                              toCall, optionalFlags, nextLine;
    Csi2_ErrorReportType                errorS = {0};           /* error structure                                  */
    volatile GENERIC_CSI2_Type          *pRegs;                 /* registry pointer                                 */
    Csi2_DriverParamsType               *pDriverState;          /* unit driver state pointer                        */
    Csi2_VCDriverStateType              *pVCDriverState;        /* VC driver state pointer                          */

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_DATA_ERR_ISR, (uint32_t)CSI2_SEQ_BEGIN);

    pRegs = gMipiCsi2RegsPtr[(uint8)iUnit];                       /* get the  registry pointer for the unit           */
    if (pRegs != NULL)
    {
        (void)memset(&errorS, 0, sizeof(uint32) * 7u);   /* set the masks to 0                           */
        pDriverState = &gCsi2Settings[(uint8)iUnit];
        errorS.unitId = (uint8)iUnit;

        /* FIRST step - process linedone event                                                                      */
        optionalFlags = pRegs->RX_CHNL_INTRS;                       /* read the LINEDONE bit                        */
        if (optionalFlags != 0u)
        {
            pRegs->RX_CHNL_INTRS = optionalFlags;                   /* clear the bit(s)                             */
            /* vc with last received data   */
            workVcIdFe = (pRegs->RX_DATAIDR & MIPICSI2_RX_DATAIDR_VCID_MASK) >> MIPICSI2_RX_DATAIDR_VCID_SHIFT;
            pVCDriverState = &pDriverState->workingParamVC[workVcIdFe];         /* get the pointer to VC params     */
            nextLine = pVCDriverState->lastReceivedChirpLine;                   /* increase the line number         */
            nextLine++;
            /* do processing for application requests (line done trigger)                                           */
            if (((pVCDriverState->eventsMask & (uint32)CSI2_EVT_LINE_END) != 0u) &&
                ((nextLine % pVCDriverState->vcParamsPtr->bufNumLinesTrigger) == 0u))
            {
                errorS.evtMaskVC[workVcIdFe] = (uint8)CSI2_EVT_LINE_END;    /* mask for application             */
                /* callback for LINEDONE only
                 * if both LINEDONE and FrameEnd occurred, the call for FE will be done at handler end          */
                if(pDriverState->pCallback[RSDK_CSI2_EVENTS_IRQ_ID] != NULL)
                {
                    pDriverState->pCallback[RSDK_CSI2_EVENTS_IRQ_ID](&errorS);
                }
                else
                {
                    pDriverState->pCallback[RSDK_CSI2_RX_ERR_IRQ_ID](&errorS);
                }
                errorS.evtMaskVC[workVcIdFe] = 0u;                          /* reset the LINEDONE bit           */
            }
            pVCDriverState->lastReceivedChirpLine = (uint16)nextLine;           /* keep the line for the next irq   */
            nextLine = pVCDriverState->lastReceivedBufLine;                     /* keep the buffer pointer          */
            /* do processing for VC/channels statistics                                                             */
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
            if (pDriverState->statisticsFlag == CSI2_AUTODC_EVERY_LINE)
            {
                Csi2_ProcessChannelStatistics(pVCDriverState, (uint16)nextLine);
            }
#endif
            nextLine++;
            pVCDriverState->lastReceivedBufLine = (uint16)nextLine;
        }

        toCall = 0u;                                                /* no necessary to call the application         */
        /* SECOND step = check for VC level Packet & Protocol errors                                                */
        vcIdFe = Csi2_ProcessVcEvents(pDriverState, &errorS, iUnit, &toCall, &optionalFlags);

        /* THIRD step - process frame end event (final statistics management)                                       */
        while (vcIdFe != (uint32)CSI2_MAX_VC)
        {   /* VC identified, process FE    */
            workVcIdFe = vcIdFe & (uint32)CSI2_VC_NUM_BITSMASK;
            vcIdFe >>= CSI2_VC_NUM_TOTALBITS;
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
            j = 1u;
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
            if (pDriverState->statisticsFlag != CSI2_AUTODC_NO)
            {
                pVCDriverState = &pDriverState->workingParamVC[workVcIdFe];
                if (pDriverState->statisticsFlag == CSI2_AUTODC_AT_FE)
                {
                    j = pVCDriverState->vcParamsPtr->bufNumLines;
                }
                if (pDriverState->statisticsFlag == CSI2_AUTODC_LAST_LINE)
                {
                    j = 1;
                }
#endif
                for (i = 0u; i < j; i++)
                {
                    /* do processing for VC/channels statistics                                                     */
                    Csi2_ProcessChannelStatistics(pVCDriverState, i);
                }
                /* process FrameEnd => statistics, so is possible to have toggle_bit problems                       */
                regStat =
                    Csi2_ProcessChannelFrameEnd(pRegs, (uint8)workVcIdFe, &pDriverState->workingParamVC[workVcIdFe],
                                               &errorS, pDriverState->statisticsFlag);
                /* be sure next time will detect correct the first line if necessary                                */
                pVCDriverState->lastReceivedChirpLine = CSI2_CHIRP_NOT_STARTED;
                pVCDriverState->lastReceivedBufLine = CSI2_CHIRP_NOT_STARTED;
                if (((pVCDriverState->eventsMask & (uint32)CSI2_EVT_BIT_NOT_TOGGLE) != 0u) &&
                    ((regStat & ((uint32)CSI2_EVT_BIT_NOT_TOGGLE)) != 0u))
                {
                    errorS.evtMaskVC[workVcIdFe] |= (uint8)CSI2_EVT_BIT_NOT_TOGGLE;
                    errorS.notToggledBits[workVcIdFe] = (uint16)regStat;
                    toCall++;
                }
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
            }
#endif
#endif /* #if (CSI2_STATISTIC_DATA_USAGE == STD_ON) */
#if (CSI2_FRAMES_COUNTER_USED == STD_ON)
            Csi2_IncFramesCounter(iUnit, workVcIdFe);
#endif
        }   /* while    */
        /* FOURTH step - application callback                                                                       */
        if ((toCall == 0u) && (optionalFlags == 0u))
        {   /* no optional or error flags set => spurious   */
            errorS.errMaskU |= (uint32)CSI2_ERR_SPURIOUS_EVT;
            toCall++;
        }

        CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_DATA_ERR_ISR, (uint32_t)CSI2_SEQ_END);

        if(toCall != 0u)
        {
            if(pDriverState->pCallback[RSDK_CSI2_EVENTS_IRQ_ID] != NULL)
            {
                pDriverState->pCallback[RSDK_CSI2_EVENTS_IRQ_ID](&errorS);
            }
            else
            {
                pDriverState->pCallback[RSDK_CSI2_RX_ERR_IRQ_ID](&errorS);
            }
        }
    }   /* if (pRegs != NULL)   */
}
/* Csi2IrqHandlerEvents *************************/



#ifdef __cplusplus
}
#endif

/** @} */

