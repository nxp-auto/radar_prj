/*
 * Copyright 2019-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#if !defined(S32R45) && !defined(S32R294)
#error "This file is used only on S32R45 or S32R294 platforms"
#endif

#ifdef linux
#include <linux/of_address.h>
#include <linux/highmem.h>
#include "rsdk_csi2_driver_module.h"
#else
#include <string.h>
#include "rsdk_glue_timer_api.h"
#endif

#ifdef TRACE_ENABLE
#include "csi2_driver_platform_trace.h"
#endif

#include "rsdk_status.h"
#include "csi2_driver_platform_specific.h"
#include "csi2_driver_irq_management.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 ==================================================================================================*/

/*==================================================================================================
 *                                       LOCAL MACROS
 ==================================================================================================*/
#define CSI2_LOOP_CONSTANT 10  // constant for the local delay

#define RSDK_CSI2_RX_FREQ_STEP 500u  // step for adjusting oscillator parameters

// definitions for DPHY registry, not existing in the NXP header file,
// but defined in the MIPI-CSI2 errata for manual calibration
#define DPHY_CLKCALVAL_COMPS_OFFSET 0x09a0u           // offset toDPHY_CLKCALVAL_COMPS registry
#define DPHY_DATALOFFSETCAL_VALUE0_OFFSET 0x0ba5u     // offset to DPHY_DATALOFFSETCAL_VALUE0 registry
#define DPHY_DATALOFFSETCAL_VALUE1_OFFSET 0x0da5u     // offset to DPHY_DATALOFFSETCAL_VALUE1 registry
#define DPHY_DATALOFFSETCAL_VALUE2_OFFSET 0x0fa5u     // offset to DPHY_DATALOFFSETCAL_VALUE2 registry
#define DPHY_DATALOFFSETCAL_VALUE3_OFFSET 0x11a5u     // offset to DPHY_DATALOFFSETCAL_VALUE3 registry
#define DPHY_TX_TERM_CAL_0_OFFSET 0x1520u             // offset to DPHY_TX_TERM_CAL_0 registry
#define DPHY_TX_RDWR_TERM_CAL_0_OFFSET 0x080du        // offset to DPHY_TX_RDWR_TERM_CAL_0 registry
#define DPHY_TX_RDWR_TERM_CAL_1_OFFSET 0x080eu        // offset to DPHY_TX_RDWR_TERM_CAL_1 registry
#define DPHY_CLKOFFSETCAL_OVRRIDE_OFFSET 0x097fu      // offset to DPHY_CLKOFFSETCAL_OVRRIDE registry
#define DPHY_CLKOFFSETCAL_OVRRIDEVAL_OFFSET 0x0980u   // offset to DPHY_CLKOFFSETCAL_OVRRIDEVAL registry
#define DPHY_DATALOFFSETCAL_OVRVALUE0_OFFSET 0x0b80u  // offset to DPHY_DATALOFFSETCAL_OVRVALUE0 registry
#define DPHY_DATALOFFSETCAL_OVRVALUE1_OFFSET 0x0d80u  // offset to DPHY_DATALOFFSETCAL_OVRVALUE1 registry
#define DPHY_DATALOFFSETCAL_OVRVALUE2_OFFSET 0x0f80u  // offset to DPHY_DATALOFFSETCAL_OVRVALUE2 registry
#define DPHY_DATALOFFSETCAL_OVRVALUE3_OFFSET 0x1180u  // offset to DPHY_DATALOFFSETCAL_OVRVALUE3 registry
#define DPHY_DATAL0OFFSETCAL_OVRCNTRL_OFFSET 0x0b7fu  // offset to DPHY_DATAL0OFFSETCAL_OVRCNTRL registry
#define DPHY_DATAL1OFFSETCAL_OVRCNTRL_OFFSET 0x0d7fu  // offset to DPHY_DATAL1OFFSETCAL_OVRCNTRL registry
#define DPHY_DATAL2OFFSETCAL_OVRCNTRL_OFFSET 0x0f7fu  // offset to DPHY_DATAL2OFFSETCAL_OVRCNTRL registry
#define DPHY_DATAL3OFFSETCAL_OVRCNTRL_OFFSET 0x117fu  // offset to DPHY_DATAL3OFFSETCAL_OVRCNTRL registry
#define DPHY_RX_STARTUP_OVERRIDE_OFFSET 0x06e4u       // offset to DPHY_RX_STARTUP_OVERRIDE registry

/*==================================================================================================
 *                                      LOCAL CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      LOCAL VARIABLES
 ==================================================================================================*/
// Copy for CSI2 actual params, to use if reset needed.
static rsdkCsi2InitParams_t gsCsi2UnitParamCopy[RSDK_CSI2_MAX_UNITS]
    __attribute__((section(".RSDK_CSI2_INTERNAL_MEMORY")));
// Counter for received frames.
static volatile uint32_t gsCsi2FramesCounter[RSDK_CSI2_MAX_UNITS][RSDK_CSI2_MAX_VC]
    __attribute__((section(".RSDK_CSI2_INTERNAL_MEMORY")));

/*==================================================================================================
 *                                      GLOBAL CONSTANTS
 ==================================================================================================*/
// Pointer to MIPICSI2 memory map
volatile struct MIPICSI2_REG_STRUCT *gpMipiCsi2Regs[RSDK_CSI2_MAX_UNITS]
    __attribute__((section(".RSDK_CSI2_INTERNAL_MEMORY")));

// settings to be kept during the execution
// only run-time necessary parameters are kept
rsdkCsi2DriverParams_t gCsi2Settings[RSDK_CSI2_MAX_UNITS] __attribute__((section(".RSDK_CSI2_INTERNAL_MEMORY"))) = {
    {.driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED, RSDK_CSI2_STAT_NO},
    {.driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED, RSDK_CSI2_STAT_NO},
#ifdef S32R45
    {.driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED, RSDK_CSI2_STAT_NO},
    {.driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED, RSDK_CSI2_STAT_NO}
#endif
};

/*==================================================================================================
 *                                      GLOBAL VARIABLES
 ==================================================================================================*/

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/
// Get the masks according to the CSI2 declared communication speed.
static void Csi2GetOperatingSpeedMask(const uint16_t speed, uint8_t *pHSFREQRNG, uint16_t *pDDLOSCFREQ);
// Set the GPIO associated interface
static void Csi2SetGpio(volatile struct MIPICSI2_REG_STRUCT *pRegs, const uint8_t csi2VC,
                                                const rsdkCsi2VCParams_t *pVCparams);
// Set the SDMA associated events
static void Csi2SetSdma(volatile struct MIPICSI2_REG_STRUCT *pRegs, const uint8_t csi2VC,
                                                const rsdkCsi2VCParams_t *pVCparams);
// Set the correct offset for all channels
static uint8_t Csi2SetOffsetMan(rsdkCsi2VCDriverState_t *pVCDrvState, const rsdkCsi2VCParams_t *pVCparams);
// Verify the initialization parameters
static rsdkStatus_t Csi2PlatformCheckInitParams(const rsdkCsi2UnitId_t      unitId,
                                                const rsdkCsi2InitParams_t *pCsi2InitParam);
// Get the corresponding registry pointer
static rsdkStatus_t Csi2RegsMap(const rsdkCsi2UnitId_t unitId, volatile struct MIPICSI2_REG_STRUCT **pRegs);
// Local wait
static void Csi2WaitLoop(uint32_t loops);
// DFS reset
static void DfsTilt(void);
// Get the corresponding bits number for the data type received
static uint16_t Csi2GetDataTypeBits(const rsdkCsi2DataStreamType_t dataType);
// Check the callback parameters
static rsdkStatus_t Csi2CheckCbParam(const rsdkCsi2InitParams_t *pCsi2InitParam);
// Check the base parameters : unitId, initialization structure pointer and lanes number
static rsdkStatus_t Csi2CheckBaseParams(const rsdkCsi2UnitId_t unitId, const rsdkCsi2InitParams_t *pCsi2InitParam);

/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 ==================================================================================================*/

/*================================================================================================*/
/*
 * @brief   The function check the correctness of auxiliary data setup.
 *          It checks only the match between streamDataType and outputDataMode,
 *          but assumes RSDK_CSI2_VC_BUF_FIFTH_CH_ON bit is set.
 *
 * @param[in] pVC - pointer to the virtual channel initialization structure
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 *          RSDK_CSI2_DRV_INVALID_DATA_TYPE - settings mismatch
 *
 */
static rsdkStatus_t Csi2CheckAuxiliaryMode(const rsdkCsi2VCParams_t *pVC)
{
    rsdkStatus_t rez;
    uint32_t     streamType;
    uint16_t     outputType;

    rez = RSDK_SUCCESS;
    streamType = ((uint32_t)pVC->streamDataType) & (~RSDK_CSI2_NORM_DTYPE_MASK);
    if (streamType != 0u)
    {
        outputType = (uint16_t)(pVC->outputDataMode & RSDK_CSI2_VC_BUF_5TH_CH_MODE_1);
        if (((streamType == (uint32_t)RSDK_CSI2_DATA_TYPE_AUX_0_NO_DROP) &&
             (outputType != (uint32_t)RSDK_CSI2_VC_BUF_5TH_CH_M0_NODROP)) ||
            ((streamType == (uint32_t)RSDK_CSI2_DATA_TYPE_AUX_0_DR_1OF2) &&
             (outputType != (uint32_t)RSDK_CSI2_VC_BUF_5TH_CH_M0_1_OF_2)) ||
            ((streamType == (uint32_t)RSDK_CSI2_DATA_TYPE_AUX_0_DR_3OF4) &&
             (outputType != (uint32_t)RSDK_CSI2_VC_BUF_5TH_CH_M0_3_OF_4)) ||
            ((streamType == (uint32_t)RSDK_CSI2_DATA_TYPE_AUX_1_NO_DROP) &&
             (outputType != (uint32_t)RSDK_CSI2_VC_BUF_5TH_CH_MODE_1)) ||
            (streamType > (uint32_t)RSDK_CSI2_DATA_TYPE_MAX))
        {
            rez = RSDK_CSI2_DRV_INVALID_DATA_TYPE;
        }
    }
    return rez;
}
/* Csi2CheckAuxiliaryMode *************************/

/*================================================================================================*/
/*
 * @brief       The function check the CSI2 virtual channel initialization parameters for correctness.
 *
 * @param[in] pVC - pointer to the virtual channel initialization structure
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 *          RSDK_CSI2_DRV_INVALID_VC_PARAMS - wrong parameters
 *          RSDK_CSI2_DRV_INVALID_DATA_TYPE - wrong data type specified
 *
 */
static rsdkStatus_t Csi2CheckVCParam(const rsdkCsi2VCParams_t *pVC, uint8_t *pAutoStat)
{
    rsdkStatus_t rez;
    uintptr_t    addrAlign;
    uint8_t      i;
    uint32_t     bufLen;

    rez = RSDK_SUCCESS;  // default SUCCESS
    if (pVC != NULL)
    {  // check only for valid/defined VC
        bufLen = Csi2PlatformGetBufferRealLineLen(pVC->streamDataType, pVC->channelsNum, pVC->expectedNumSamples, 0u);
        if (bufLen == 0u)
        {
            rez = RSDK_CSI2_DRV_INVALID_DATA_TYPE;
        }
        else
        {
            addrAlign = (uintptr_t)((uint8_t *)pVC->pBufData);
            addrAlign &= 0xfu;
            if ((pVC->outputDataMode & RSDK_CSI2_VC_BUF_FIFTH_CH_ON) != 0u)
            {  // check for correct auxiliary data setup, rez can be SUCCESS here
                rez = Csi2CheckAuxiliaryMode(pVC);
            }
            if (pVC->expectedNumSamples == (uint16_t)0)
            {
                rez = RSDK_CSI2_DRV_NO_SAMPLE_PER_CHIRP;
            }
            if (pVC->expectedNumLines == (uint16_t)0)
            {
                rez = RSDK_CSI2_DRV_NO_CHIRPS_PER_FRAME;
            }
            if ((pVC->bufLineLen == (uint16_t)0))
            {
                rez = RSDK_CSI2_DRV_NO_LINE_LENGTH;
            }
            if ((pVC->bufLineLen & 0xfu) != (uint16_t)0)
            {
                rez = RSDK_CSI2_DRV_BUF_LEN_NOT_ALIGNED;
            }
            if (pVC->pBufData == NULL)
            {
                rez = RSDK_CSI2_DRV_BUF_PTR_NULL;
            }
            if (addrAlign != (uintptr_t)0u)
            {
                rez = RSDK_CSI2_DRV_BUF_PTR_NOT_ALIGNED;
            }
            if (pVC->bufNumLines < (uint16_t)RSDK_CSI2_MIN_VC_BUF_NR_LINES)
            {
                rez = RSDK_CSI2_DRV_BUF_NUM_LINES_ERR;
            }
            if (pVC->channelsNum > (uint8_t)RSDK_CSI2_MAX_ANTENNA_NR)
            {
                rez = RSDK_CSI2_DRV_INVALID_CHANNEL_NR;
            }
            if (((pVC->vcEventsReq & (uint8_t)RSDK_CSI2_EVT_LINE_END) != (uint8_t)0) &&
                (pVC->bufNumLinesTrigger == (uint8_t)0))
            {
                rez = RSDK_CSI2_DRV_INVALID_EVT_REQ;
            }
            for (i = 0; i < pVC->channelsNum; i++)
            {  // check for all channels
                if (pVC->offsetCompReal[i] == RSDK_CSI2_OFFSET_AUTOCOMPUTE)
                {
                    *pAutoStat |= 1u;
                    break;
                }
                if (((pVC->outputDataMode & RSDK_CSI2_VC_BUF_COMPLEX_DATA) != 0u) &&
                    (pVC->offsetCompImg[i] == RSDK_CSI2_OFFSET_AUTOCOMPUTE))
                {  // complex data, so check for complex part too
                    *pAutoStat |= 1u;
                    break;
                }
            }
        }  // if ((uint8_t) pVC->streamDataType >= (uint8_t) RSDK_CSI2_DATA_TYPE_LIMIT)
    }      // if (pVC != 0)
    return rez;
}
/* Csi2CheckVCParam *************************/

/*================================================================================================*/
/*
 * @brief       The function check the CSI2 specific callback parameters
 *
 * @param[in] pCsi2InitParam - pointer to the CSI2 initialization structure
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 *          RSDK_CSI2_DRV_NULL_ERR_CB_PTR - wrong callback pointer
 *          RSDK_CSI2_DRV_ERR_INVALID_CORE_NR - wrong core identifier
 *
 */
static rsdkStatus_t Csi2CheckCbParam(const rsdkCsi2InitParams_t *pCsi2InitParam)
{
    rsdkStatus_t rez;

    rez = RSDK_SUCCESS;
    // check for callbacks
    if (pCsi2InitParam->pCallback[(uint32_t)RSDK_CSI2_RX_ERR_IRQ_ID] == NULL)
    {  // at least RxErr callback must be specified
        rez = RSDK_CSI2_DRV_NULL_ERR_CB_PTR;
    }
#ifdef S32R294
    if (pCsi2InitParam->irqExecCore != RSDK_CURRENT_CORE)
    {
        rez = RSDK_CSI2_DRV_ERR_INVALID_CORE_NR;
    }
#endif
    return rez;
}
/* Csi2CheckCbParam *************************/

/*================================================================================================*/
/*
 * @brief       The function check the base CSI2 specific parameters
 *
 * @param[in]   unitId          - the unit number, 0...max_id
 * @param[in] pCsi2InitParam    - pointer to the CSI2 initialization structure
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 *          RSDK_CSI2_DRV_WRG_UNIT_ID - wrong unit
 *          RSDK_CSI2_DRV_NULL_PARAM_PTR - no initialization parameters
 *          RSDK_CSI2_DRV_INVALID_LANES_NR - wrong number of lanes
 *          RSDK_CSI2_DRV_NULL_VC_PARAM_PTR - all VC pointers are NULL
 *
 */
static rsdkStatus_t Csi2CheckBaseParams(const rsdkCsi2UnitId_t unitId, const rsdkCsi2InitParams_t *pCsi2InitParam)
{
    uint32_t     i;
    rsdkStatus_t rez;

    if ((uint8_t)unitId >= (uint8_t)RSDK_CSI2_MAX_UNITS)
    {
        rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
    }
    else if (pCsi2InitParam == NULL)
    {
        rez = RSDK_CSI2_DRV_NULL_PARAM_PTR;
    }
    else if (pCsi2InitParam->numLanesRx >= (uint8_t)RSDK_CSI2_MAX_LANE)
    {  // wrong number of lanes
        rez = RSDK_CSI2_DRV_INVALID_LANES_NR;
    }
    else
    {
        rez = RSDK_SUCCESS;
    }
    if (rez == RSDK_SUCCESS)
    {
        for (i = 0; i < (uint32_t)RSDK_CSI2_MAX_VC; i++)
        {
            if (pCsi2InitParam->pVCconfig[i] != NULL)
            {
                break;
            }
        }
        if (i >= (uint32_t)RSDK_CSI2_MAX_VC)
        {
            rez = RSDK_CSI2_DRV_NULL_VC_PARAM_PTR;
        }
    }
    return rez;
}
/* Csi2CheckBaseParams *************************/

/*================================================================================================*/
/*
 * @brief       The function check the input parameters for CSI2 initialization.
 * @details     All parameters for CSI2 are checked and the appropriate status is returned.
 *
 * @param[in]   unitId         - the unit number, 0...max_id
 * @param[in]   pCsi2InitParam - pointer to CSI2 initialization parameters
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 * @retval  RSDK_CSI2_DRV_ERR_INVALID_CORE_NR - invalid core for interrupt processing
 *          RSDK_CSI2_DRV_INVALID_CHANNEL_NR - Wrong antenna number
 *          RSDK_CSI2_DRV_INVALID_CLOCK_FREQ - incorrect clock frequency specified
 *          RSDK_CSI2_DRV_INVALID_DATA_TYPE - wrong VC or auxiliary data type specified
 *          RSDK_CSI2_DRV_INVALID_DC_PARAMS - wrong parameters for DC compensation
 *          RSDK_CSI2_DRV_INVALID_LANES_NR - Wrong lanes number
 *          RSDK_CSI2_DRV_INVALID_RX_SWAP - Invalid Rx lanes swap definition
 *          RSDK_CSI2_DRV_NULL_ERR_CB_PTR - no error callback specified
 *          RSDK_CSI2_DRV_NULL_ISR_CB_PTR - no callback specified for irq handler recording
 *          RSDK_CSI2_DRV_NULL_VC_PARAM_PTR - all VC pointers are null
 *          RSDK_CSI2_DRV_VC_AUX_MISMATCH - auxiliary data specified for VC not defined
 *          RSDK_CSI2_DRV_WRG_UNIT_ID - wrong unit id
 *
 */
static rsdkStatus_t Csi2PlatformCheckInitParams(const rsdkCsi2UnitId_t      unitId,
                                                const rsdkCsi2InitParams_t *pCsi2InitParam)
{
    rsdkStatus_t rez;
    uint8_t      ref, mask, t, autoDC;  // number to be used for tests
    uint32_t     i;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_CHK_INIT_PARAM, 
                                            (uint32_t)CSI2_SEQ_BEGIN);
    rez = Csi2CheckBaseParams(unitId, pCsi2InitParam);
    if (rez == RSDK_SUCCESS)
    {  // lanes ok
        // checking the receive lanes configuration
        ref = 0;
        for (i = (uint32_t)RSDK_CSI2_LANE_0; i <= pCsi2InitParam->numLanesRx; i++)
        {
            ref |= ((uint8_t)1 << (uint8_t)pCsi2InitParam->lanesMapRx[i]);
        }           // for
        t = 0xffu;  // total good channels
        mask = 1;   // test mask
        for (i = (uint32_t)RSDK_CSI2_LANE_0; i < (uint32_t)RSDK_CSI2_MAX_LANE; i++)
        {
            if ((ref & mask) != 0u)
            {
                t++;
            }
            mask <<= (uint8_t)1;
        }

        if (t != pCsi2InitParam->numLanesRx)
        {  // wrong configuration for receiving lanes swap
            rez = RSDK_CSI2_DRV_INVALID_RX_SWAP;
        }
    }
    if (rez == RSDK_SUCCESS)
    {  // good configuration for lanes swap
        if ((pCsi2InitParam->txClkFreq < (uint32_t)RSDK_CSI2_MIN_TX_FREQ) ||
            (pCsi2InitParam->txClkFreq > (uint32_t)RSDK_CSI2_MAX_TX_FREQ) ||
            (pCsi2InitParam->rxClkFreq < (uint32_t)RSDK_CSI2_MIN_RX_FREQ) ||
            (pCsi2InitParam->rxClkFreq > (uint32_t)RSDK_CSI2_MAX_RX_FREQ))
        {
            rez = RSDK_CSI2_DRV_INVALID_CLOCK_FREQ;  // the clock freq. is wrong
        }
        else
        {
            // checking the VCs and the callbacks (to be careful at index for both)
            autoDC = 0;
            for (i = 0; i < (uint32_t)RSDK_CSI2_MAX_VC; i++)
            {
                rez = Csi2CheckVCParam(pCsi2InitParam->pVCconfig[i], &autoDC);
                if (rez == RSDK_SUCCESS)
                {  // check for auxiliary data only if successful before
                    rez = Csi2CheckVCParam(pCsi2InitParam->pAuxConfig[i], &autoDC);
                }
                if ((rez == RSDK_SUCCESS) &&
                    (((pCsi2InitParam->pVCconfig[i] != NULL) &&
                      (pCsi2InitParam->pVCconfig[i]->streamDataType > RSDK_CSI2_DATA_TYPE_USR7)) ||
                     ((pCsi2InitParam->pAuxConfig[i] != NULL) &&
                      (pCsi2InitParam->pAuxConfig[i]->streamDataType < RSDK_CSI2_DATA_TYPE_AUX_0_NO_DROP))))
                {  // check for correct stream data type on normal/auxiliary flow
                    rez = RSDK_CSI2_DRV_INVALID_DATA_TYPE;
                }
#ifdef S32R294
                if ((rez == RSDK_SUCCESS) && (pCsi2InitParam->irqExecCore != RSDK_CURRENT_CORE))
                {  // limit the irq handler execution only to the current core
                    rez = RSDK_CSI2_DRV_ERR_INVALID_CORE_NR;
                }
#endif
                if ((pCsi2InitParam->pVCconfig[i] == NULL) && (pCsi2InitParam->pAuxConfig[i] != NULL))
                {
                    rez = RSDK_CSI2_DRV_VC_AUX_MISMATCH;  // VC / Aux mismatch
                }
                if (rez != RSDK_SUCCESS)
                {
                    break;
                }
            }  // for
            if (rez == RSDK_SUCCESS)
            {
                // check for autoDC status
                if ((autoDC != 0u) && ((pCsi2InitParam->statManagement == RSDK_CSI2_STAT_NO) ||
                                       (pCsi2InitParam->statManagement >= RSDK_CSI2_STAT_MAX)))
                {
                    rez = RSDK_CSI2_DRV_INVALID_DC_PARAMS;
                }
            }
            if (rez == RSDK_SUCCESS)
            {
                rez = Csi2CheckCbParam(pCsi2InitParam);
            }  // if(rez == RSDK_SUCCESS)
        }      // if((pStart->txClkFreq < RSDK_CSI2_MIN_TX_CLK_FREQ) ||...
    }          // if (t != pStart->numLanesRx)
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_CHK_INIT_PARAM, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2CheckInitParams *************************/

/*================================================================================================*/
/*
 * @brief       Get the final output number of bytes in the receiving buffer.
 * @details     For each accepted data type.
 *
 * @param[in]   dataType    - expected data type
 *
 * @return      0           = wrong data type
 *              anything else, the number of bytes for each sample
 *
 */
static uint32_t Csi2GetOutBytesPerSample(const rsdkCsi2DataStreamType_t dataType)
{
    uint32_t rez;

    // get the conversion rate input data -> memory data, per sample
    switch ((uint16_t)dataType & RSDK_CSI2_NORM_DTYPE_MASK)
    {
        case (uint16_t)RSDK_CSI2_DATA_TYPE_EMBD:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR0:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR1:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR2:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR3:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR4:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR5:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR7:
            rez = 1;  // 1 byte per sample
            break;
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RAW8:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RAW10:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RAW12:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RAW14:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_YUV422_8:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_16_FROM_8:
            rez = 2;  // 2 bytes per sample
            break;
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RGB888:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_YUV422_10:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RGB565:
            rez = 3;  // 3 bytes per sample
            break;
        default:
            rez = 0;  // unknown/unacceptable data type
            break;
    }
    return rez;
}
/* Csi2GetOutBytesPerSample *************************/

/*================================================================================================*/
/*
 * @brief       Procedure for specific VC parameters programming.
 *
 * @param[in]   pRegs       - unit registry pointer
 *              pVCparams   - pointer to VC parameters
 *              rxBufChirpData - expected length of the received data (expected chirp length) - bytes
 *              rxBufDataLen   - expected length of the data to be written in the memory buffer - bytes
 *              rxBufLen       - total buffer line length reserved by the application - bytes
 *
 */
static void Csi2UnitConfigVCSpec(volatile struct MIPICSI2_REG_STRUCT *pRegs, const rsdkCsi2VCParams_t *pVCparams,
                                 uint32_t bufNr, const uint16_t rxBufChirpData, const uint16_t rxBufDataLen,
                                 const uint16_t rxBufLen)
{
    uint8_t            i;
    uint32_t           valDC, intDC, dataType;
    volatile uint32_t *pBufRegistry;

    // circular buffer config
    uint32_t bufferOffset;  // specific buffer offset
    uint32_t dataInput;

    // buffer configuration
    dataType = ((uint32_t)pVCparams->streamDataType & RSDK_CSI2_NORM_DTYPE_MASK);
    // check for data translation
    if(dataType == (uint32_t)RSDK_CSI2_DATA_TYPE_16_FROM_8)
    {
        dataType = (uint32_t)RSDK_CSI2_DATA_TYPE_RAW8;      // RSDK_CSI2_DATA_TYPE_16_FROM_8 is only a translation type
    }
    dataInput = ((bufNr & RSDK_CSI2_MAX_VC_MASK) << 8u) + (dataType << 2u);
    if (((pVCparams->outputDataMode & RSDK_CSI2_VC_BUF_FIFTH_CH_ON) != 0u) && (bufNr >= (uint32_t)RSDK_CSI2_MAX_VC))
    {  // fifth channel on
        dataInput |= RSDK_CSI2_5TH_CHANNEL_ON;
    }
    pRegs->RX[bufNr].CBUF_CONFIG.R = dataInput;

    // buffer pointer
    pRegs->RX[bufNr].CBUF_SRTPTR.R = (uint32_t)(uint8_t*)pVCparams->pBufData;

    // buffer number of lines
    pRegs->RX[bufNr].CBUF_NUMLINE.R = pVCparams->bufNumLines;

    // buffer line length
    pRegs->RX[bufNr].CBUF_BUFLEN.R = rxBufLen;

    // input interface config
    pRegs->RX[bufNr].INPLINELEN_CONFIG.R = rxBufChirpData;

    // number of chirps to be received
    pRegs->RX[bufNr].NUMLINES_CONFIG.R = pVCparams->expectedNumLines;

    // buffer chirp data length
    pRegs->RX[bufNr].LINELEN_CONFIG.R = rxBufDataLen;

    // line done trigger
    if (pVCparams->bufNumLinesTrigger != (uint8_t)0)  // LINEDONE trigger required by application
    {
        // set the value for LINEDONE
        pRegs->RX[bufNr].CBUF_LPDI.R = (uint32_t)pVCparams->bufNumLinesTrigger;
    }

    // get the mask for channel enablement and set the DC offset compensation
    bufferOffset = RSDK_CSI2_VC_CFG_OFFSET * bufNr;
    pBufRegistry =
        (volatile uint32_t *)(volatile void *)(((volatile uint8_t *)&pRegs->RX_CBUF0_CHNLOFFSET0_0) + bufferOffset);
    if (bufNr < (uint32_t)RSDK_CSI2_MAX_VC)
    {
        if ((pVCparams->outputDataMode & (uint16_t)RSDK_CSI2_VC_BUF_COMPLEX_DATA) != (uint16_t)0)
        {                                                 // complex data, so double number of channels
            for (i = 0; i < pVCparams->channelsNum; i++)  // for each defined channel
            {
                intDC = ((uint32_t)pVCparams->offsetCompReal[i] & (uint32_t)0xffffu);
                if (intDC == (uint32_t)RSDK_CSI2_OFFSET_AUTOCOMPUTE)
                {  // a kind of autocompute
                    valDC = (uint32_t)0;
                }
                else
                {
                    valDC = intDC & 0xfffu;    // keep the new offset value for first channel
                }                   // if(intDC == RSDK_CSI2_OFFSET_AUTOCOMPUTE)
                intDC = (uint32_t)pVCparams->offsetCompImg[i];
                if (intDC != (uint32_t)RSDK_CSI2_OFFSET_AUTOCOMPUTE)
                {
                    valDC |= (intDC << 16u);  // keep the offset value, on highest bits
                }                             // if(intDC == RSDK_CSI2_OFFSET_AUTOCOMPUTE)
                // set the offset compensations when the data is complex
                *(pBufRegistry) = (valDC << 4u);    // adjust the value from 12 bits to the real output
                pBufRegistry++;
            }  // for
        }
        else
        {
            // set the offset compensations when the data is only real
            valDC = 0u;
            for (i = 0; i < pVCparams->channelsNum; i++)  // for each defined channel
            {
                intDC = ((uint32_t)pVCparams->offsetCompReal[i] & (uint32_t)0xffff);
                if (intDC != (uint32_t)RSDK_CSI2_OFFSET_AUTOCOMPUTE)
                {
                    valDC |= intDC << (16u * (i & 1u)); // keep the new offset value, at right position
                }                                       // if(intDC == (uint32_t)RSDK_CSI2_OFFSET_AUTOCOMPUTE)
                *pBufRegistry = (valDC << 4u);          // adjust from 12 bits
                if ((i & 1u) != 0u)
                {
                    pBufRegistry++;  // after even index go to next value
                    valDC = 0u;
                }
            }  // for
        }      // if((pVCparams->outputDataMode & RSDK_CSI2_VC_BUF_COMPLEX_DATA) != 0)
    }
}
/* Csi2UnitConfigVCSpec *************************/

/*================================================================================================*/
/*
 * @brief       Procedure for general programming a VC.
 * @details     This procedure initialize all "common" parameters of a VC.
 *              For specific VC initialization: Csi2UnitConfigVCSpec
 *
 * @param[in]   csi2UnitNum - unit number, RSDK_CSI2_UNIT_0 ... MAX
 * @param[in]   vcId        - VC number, RSDK_CSI2_VC_0 ... MAX
 * @param[in]   pRegs       - unit registry pointer
 * @param[in]   pVCparams   - pointer to VC parameters
 *
 * @return      RSDK_SUCCESS   Successful initialization
 *              RSDK_CSI2_WRG_START_PARAMS       Wrong unit id
 *
 * @NOTE        Assuming any VC is receiving only one data_type.
 *
 */
static rsdkStatus_t Csi2ConfigVC(const rsdkCsi2UnitId_t csi2UnitNum, const uint8_t vcId,
                                 volatile struct MIPICSI2_REG_STRUCT *pRegs, const rsdkCsi2VCParams_t *pVCparams)
{
    uint32_t                 i, val1, cfg2;
    uint16_t                 lenChirpData, lenBufData;  // data lengths
    rsdkStatus_t             rez;                       // result
    rsdkCsi2VCDriverState_t *pDriverStateVC = NULL;     // pointer to VC driver state

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_CFG_VC, (uint32_t)CSI2_SEQ_BEGIN);
    rez = RSDK_SUCCESS;
    if (pVCparams != NULL)
    {  // valid VC pointer, so initialize
        if ((pVCparams->channelsNum <= (uint8_t)RSDK_CSI2_MAX_CHANNEL) && (pVCparams->channelsNum > (uint8_t)0))
        {
            // compute the necessary buffer space, according to chirp values and other requirements
            // chirp length in bytes
            lenChirpData = pVCparams->expectedNumSamples * Csi2GetDataTypeBits(pVCparams->streamDataType) *
                           (uint16_t)pVCparams->channelsNum / (uint16_t)8;
            // buffer length in bytes, without statistics
            lenBufData = (uint16_t)Csi2PlatformGetBufferRealLineLen(pVCparams->streamDataType, pVCparams->channelsNum,
                                                                    pVCparams->expectedNumSamples, 0u);
            if ((pVCparams->outputDataMode & RSDK_CSI2_VC_BUF_FIFTH_CH_ON) != 0u)
            {
                if (((pVCparams->outputDataMode & ((uint32_t)RSDK_CSI2_VC_BUF_5TH_CH_MODE_1)) ==
                     RSDK_CSI2_VC_BUF_5TH_CH_MODE_1))
                {  // correct chirp length for auxiliary mode_1
                    lenChirpData = (lenChirpData * 3u) / 2u;
                    pRegs->RX[vcId & RSDK_CSI2_MAX_VC_MASK].INPLINELEN_CONFIG.R =
                        lenChirpData;  // adjust the chirp length for base buffer
                }
                else
                {
                    lenChirpData *= 2u;  // chirp length for all auxiliary data mode 0
                }
            }

            val1 = 1;
            if (vcId < (uint8_t)RSDK_CSI2_MAX_VC)
            {
                pRegs->RX_VCENABLE.R |= val1 << vcId;  // enable the VC
                pDriverStateVC = &gCsi2Settings[(uint8_t)csi2UnitNum].workingParamVC[vcId];
                lenBufData = RSDK_CSI2_UPPER_ALIGN_TO_(lenBufData, 16u);  // align the lengths to 16 bytes
                if (((gCsi2Settings[(uint8_t)csi2UnitNum].statisticsFlag != RSDK_CSI2_STAT_NO) &&
                     ((lenBufData + RSDK_CSI2_LINE_STAT_LENGTH) <= pVCparams->bufLineLen)) ||
                    ((gCsi2Settings[(uint8_t)csi2UnitNum].statisticsFlag == RSDK_CSI2_STAT_NO) &&
                     (lenBufData <= pVCparams->bufLineLen)))
                {
                    // save the working params for future usage
                    pDriverStateVC->outputDataMode = pVCparams->outputDataMode;
                    pDriverStateVC->eventsMask = pVCparams->vcEventsReq;
                    // wrong buffer line number at start
                    pDriverStateVC->lastReceivedBufLine = RSDK_CSI2_CHIRP_NOT_STARTED;
                    pDriverStateVC->lastReceivedChirpLine = pVCparams->expectedNumLines;
                    cfg2 = (uint32_t)Csi2PlatformGetChannelNum(pDriverStateVC);  // the real number for channels
                    cfg2 = (val1 << cfg2) - ((uint32_t)1);                       // mask for enable the real channels
                    pRegs->RX_CBUF_CHNLENBL[vcId].R = cfg2;                      // the receiving channels
                    // no 5th channel influence
                    // check for data translation and add SWAPRAWDATA, even it was asked or not explicitly
                    cfg2 = (pVCparams->streamDataType == RSDK_CSI2_DATA_TYPE_16_FROM_8) ? 
                                                            (uint32_t)RSDK_CSI2_VC_BUF_SWAP_RAW8 : 0u;
                    // the output data mode
                    pRegs->RX_CBUF_OUTCFG[vcId].R = (cfg2 | (uint32_t)pVCparams->outputDataMode) >> 4u;  
                }
                else
                {
                    rez = RSDK_CSI2_DRV_TOO_SMALL_BUFFER;  // the buffer line length is not enough
                }
            }
            if ((rez == RSDK_SUCCESS) && (vcId < (uint8_t)RSDK_CSI2_MAX_VC))
            {
                for (i = (uint32_t)RSDK_CSI2_CHANNEL_A; i < (uint32_t)RSDK_CSI2_MAX_CHANNEL; i++)
                {
                    pDriverStateVC->statDC[i].channelSum = 0;  // init of VC struct for DC statistics
                    pDriverStateVC->statDC[i].channelBitToggle = (uint16_t)0;
                    pDriverStateVC->statDC[i].channelMax =
                        ((pDriverStateVC->outputDataMode & (uint32_t)RSDK_CSI2_VC_BUF_FLIP_SIGN) == 0u) ?
                            (int16_t)RSDK_CSI2_MIN_VAL_SIGNED :
                            (int16_t)RSDK_CSI2_MIN_VAL_UNSIGNED;
                    pDriverStateVC->statDC[i].channelMin =
                        ((pDriverStateVC->outputDataMode & (uint32_t)RSDK_CSI2_VC_BUF_FLIP_SIGN) == 0u) ?
                            (int16_t)RSDK_CSI2_MAX_VAL_SIGNED :
                            (int16_t)RSDK_CSI2_MAX_VAL_UNSIGNED;
                }  // for
            }
            if(rez == RSDK_SUCCESS)
            {
                // configure each VC with the very specific params
                Csi2UnitConfigVCSpec(pRegs, pVCparams, vcId, lenChirpData, lenBufData, pVCparams->bufLineLen);
            }
        }
        else
        {
            rez = RSDK_CSI2_DRV_INVALID_CHANNEL_NR;  // error, wrong antenna number
        }  // if((pVCparams->channelsNum <= RSDK_CSI2_MAX_CHANNEL_NR) && (pVCparams->channelsNum > 0))
    }      // if(pVCparams != NULL)
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_CFG_VC, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2ConfigVC *************************/

/*================================================================================================*/
/*
 * @brief       Procedure for correct start of the CSI2 clock.
 * @details     This procedure reset the MIPI_CSI_TXRX_LI_CLK, using the DFS bit responsible. 
 *
 */
static void DfsTilt(void)
{
    volatile struct DFS_tag *pDfs;
    uint32_t                 i;

#ifdef S32R45

#ifdef linux
    // assume unit 0 is already initialized
    pDfs = (volatile struct DFS_tag *)(volatile void *)gpRsdkCsi2Device[0]->pMemMapVirtDfs;
#else
    pDfs = (volatile struct DFS_tag *)&PERIPH_DFS;
#endif                              // linux
    pDfs->PORTRESET.B.RESET2 = 1u;  // step 2 - Gate the MIPI_CSI_TXRX_LI_CLK
    i = 3;
    while ((pDfs->PORTSR.B.PORTSTAT2 != 0u) && (i > 0u))  //  wait for the clock output to lock
    {
        Csi2WaitLoop(15);
        i--;
    }
    pDfs->PORTRESET.B.RESET2 = 0u;  // step 3 - Ungate the MIPI_CSI_TXRX_LI_CLK
#else                               // S32R45
    uint32_t regVal;

    pDfs = (volatile struct DFS_tag *)&DFS;
    regVal = pDfs->PORTRESET.R;
    regVal |= (1u << 2u);  // step 2 - Gate the MIPI_CSI_TXRX_LI_CLK
    pDfs->PORTRESET.R = regVal;
    i = 3;
    while (((pDfs->PORTSR.R & (1u << 2u)) != 0u) && (i > 0u))  //  wait for the clock output to lock
    {
        Csi2WaitLoop(15);
        i--;
    }
    regVal &= (~(1u << 2u));
    pDfs->PORTRESET.R = regVal;  // step 3 - Ungate the MIPI_CSI_TXRX_LI_CLK
#endif                              // S32R45
}
/* DfsTilt *************************/

/*================================================================================================*/
/*
 * @brief       Procedure for VC initialization.
 * @details     This procedure initialize a VirtualChannel, normal or auxiliary.
 *
 * @param[in]   csi2UnitNum = MIPI CSI2 unit ID
 * @param[in]   pRegs       = pointer to unit registry
 * @param[in]   pParams     = pointer to the unit initialization parameters
 *
 * @return      result status, RSDK_SUCCESS or the appropriate error
 *
 */
static rsdkStatus_t Csi2InitAllVcAux(const rsdkCsi2UnitId_t csi2UnitNum, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                                     const rsdkCsi2InitParams_t *pParams)
{
    rsdkStatus_t rez;
    uint32_t     val, i;

    rez = RSDK_SUCCESS;
    for (i = 0; i < (uint32_t)RSDK_CSI2_MAX_VC; i++)
    {
        if(pParams->pVCconfig[i] != NULL)
        {
            // virtual channel real configuration
            rez = Csi2ConfigVC(csi2UnitNum, (uint8_t)i, pRegs, pParams->pVCconfig[i]);
            if (rez != RSDK_SUCCESS)
            {
                break;  // error in VC initialization, so stop here and report the error
            }
        }
        // check for auxiliary data buffer
        val = 0u;
        if (pParams->pVCconfig[i] != NULL)
        {
            val = (uint32_t)pParams->pVCconfig[i]->outputDataMode;
            val &= RSDK_CSI2_VC_BUF_FIFTH_CH_ON;
        }
        if ((val != 0u) && (pParams->pAuxConfig[i] != NULL))
        {
            rez = Csi2ConfigVC(csi2UnitNum, (uint8_t)i + (uint8_t)RSDK_CSI2_MAX_VC, pRegs, pParams->pAuxConfig[i]);
        }
        if (rez != RSDK_SUCCESS)
        {
            break;  // error in VC initialization, so stop here and report the error
        }
    }
    return rez;
}
/* Csi2InitAllVcAux *************************/

/*================================================================================================*/
/*
 * @brief       Procedure for specific unit initialization.
 * @details     The parameters must be all correct at this point.
 *              TODO : check how to enable less lanes (than 4) with different mapping.
 *
 * @param[in]   csi2UnitNum     - unit number
 * @param[in]   pUnitParams     - pointer to the parameters structure
 *
 * @return      RSDK_SUCCESS   Successful initialization
 *              RSDK_CSI2_INIT_ERROR       Init error occurred
 *
 */
static rsdkStatus_t Csi2ModuleInit(const rsdkCsi2UnitId_t csi2UnitNum, const rsdkCsi2InitParams_t *pParams)
{
    volatile struct MIPICSI2_REG_STRUCT *pRegs;
    uint8_t                              laneId;
    uint8_t                              vcId;
    uint32_t                             i, val;
    uint8_t                              HsFreqRange, lanesSwap;
    uint16_t                             DdlOscFreq;
    rsdkStatus_t                         rez;
    rsdkCsi2DriverParams_t *             pDriverState;
    volatile uint8_t *                   pDphyRegs;  // temporary pointer to DPHY registry
    uint8_t                              lVal0, lVal1, lVal2, lVal3, lVal4, lVal5;  // temporary values

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_INIT_STEP1, (uint32_t)CSI2_SEQ_BEGIN);
    if (csi2UnitNum >= RSDK_CSI2_MAX_UNITS)
    {
        rez = RSDK_CSI2_DRV_WRG_UNIT_ID;  // an unit number too big
    }
    else
    {
        rez = RSDK_SUCCESS;
        // check first to have UNIT_1 used/initialized before UNIT_0
#ifdef S32R294
        if (csi2UnitNum == RSDK_CSI2_UNIT_0)
        {
            pDriverState = &gCsi2Settings[(uint8_t)RSDK_CSI2_UNIT_1];
            if (pDriverState->driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)  // unit 1 not initialized
            {
                rez = RSDK_CSI2_DRV_ERR_UNIT_1_MUST_BE_FIRST;
            }
        }
#elif defined(S32R45)
        if (csi2UnitNum == RSDK_CSI2_UNIT_1)
        {
            pDriverState = &gCsi2Settings[(uint8_t)RSDK_CSI2_UNIT_0];
            if (pDriverState->driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)     // unit 0 not initialized
            {
                rez = RSDK_CSI2_DRV_ERR_UNIT_0_MUST_BE_FIRST;
            }
        }
        else
        {
            if (csi2UnitNum == RSDK_CSI2_UNIT_3)
            {
                pDriverState = &gCsi2Settings[(uint8_t)RSDK_CSI2_UNIT_2];
                if (pDriverState->driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)  // unit 3 not initialized
                {
                    rez = RSDK_CSI2_DRV_ERR_UNIT_2_MUST_BE_FIRST;
                }
            }
        }
#else
#error "Wrong or no defined platform"
#endif
    } // if (csi2UnitNum >= RSDK_CSI2_MAX_UNITS)
    if (rez == RSDK_SUCCESS)
    {
        pDriverState = &gCsi2Settings[(uint8_t)csi2UnitNum];
#ifdef linux
        if(pDriverState->driverState != CSI2_DRIVER_STATE_NOT_INITIALIZED)
        {
            for(i =  0; i < (uint32_t)RSDK_CSI2_MAX_VC; i++)
            {
                if(pDriverState->workingParamVC[i].pVCParams != NULL)
                {
                    iounmap(pDriverState->workingParamVC[i].pVirtData);                   // unmap the mapped memory
                }
            }
        }
#endif
        pDriverState->driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED;  // unit not initialized
        rez = Csi2RegsMap(csi2UnitNum, &pRegs);                         // get the  registry pointer for the unit
        if (rez == RSDK_SUCCESS)
        {                                                  // valid registry pointer
            gpMipiCsi2Regs[(uint8_t)csi2UnitNum] = pRegs;  // keep the registry pointer for future

            // supplementary step : read some registry to check for previous initialization
            pDphyRegs = (volatile uint8_t *)(volatile void *)pRegs;  // initialize the DPY registry pointer
            lVal0 = pDphyRegs[DPHY_DATALOFFSETCAL_VALUE0_OFFSET];
            lVal1 = pDphyRegs[DPHY_DATALOFFSETCAL_VALUE1_OFFSET];
            lVal2 = pDphyRegs[DPHY_DATALOFFSETCAL_VALUE2_OFFSET];
            lVal3 = pDphyRegs[DPHY_DATALOFFSETCAL_VALUE3_OFFSET];
            lVal4 = pDphyRegs[DPHY_CLKCALVAL_COMPS_OFFSET];
            lVal5 = pDphyRegs[DPHY_TX_TERM_CAL_0_OFFSET];

            // supplementary step : SoftReset
            // not present in RM (at least for the moment)
            // necessary to reset the NEXTLINE registry and implicitly the start of the next frame
            pRegs->RX_SR.R = RSDK_CSI2_SOFTRESET_BIT;
            pRegs->RX_SR.R = 0u;

            // init CSI2 subsystem : RM/rev.1/draft O pag.1957
            // according to the latest RM, only Rx can be used, no Tx available

            pRegs->RX_RXNULANE.R = (uint32_t)pParams->numLanesRx + 1u;  // step 1 - set the Rx num. lanes

            DfsTilt();  // step 2&3 - Gate/ungate the MIPI_CSI_TXRX_LI_CLK

            pRegs->DPHY_RSTCFG.B.RSTZ = 0u;      // step 4 - clear RSTZ
            pRegs->DPHY_RSTCFG.B.SHUTDWNZ = 0u;  // step 5 - clear SHUTDWNZ

            pRegs->DPHY_CLEAR.R = 1u;  // step 6 - set CLRREG
            Csi2WaitLoop(15);          // step 7 - wait for 15ns

            pRegs->DPHY_CLEAR.R = 0u;  // step 8 - clear CLRREG

            // DPHY manual initialization from first initialization
            // check first to see it is first initialization or not
            if ((lVal0 | lVal1 | lVal2 | lVal3 | lVal4) != 0u)
            {
                // already initialized, so use the previous values for a quicker initialization
                pDphyRegs[DPHY_TX_RDWR_TERM_CAL_0_OFFSET] = (uint8_t)(0x3u + ((lVal5 & 0x3cu) << 2u));  // enable 
                                                                // calibration override and set the appropriate value
                pDphyRegs[DPHY_TX_RDWR_TERM_CAL_1_OFFSET] = 0x1u;       // enable more calibration override
                pDphyRegs[DPHY_CLKOFFSETCAL_OVRRIDE_OFFSET] = 0x1u;     // enable clock calibration override
                pDphyRegs[DPHY_CLKOFFSETCAL_OVRRIDEVAL_OFFSET] = (uint8_t)((lVal4 & (uint8_t)0x7fu));   // compensation 
                                                                        // for clock calibration
                pDphyRegs[DPHY_DATALOFFSETCAL_OVRVALUE0_OFFSET] = (uint8_t)(((lVal0 & (uint8_t)0x7fu) << 1u) + 0x1u);
                                                                        // set the value and enable override
                pDphyRegs[DPHY_DATALOFFSETCAL_OVRVALUE1_OFFSET] = (uint8_t)(((lVal1 & (uint8_t)0x7fu) << 1u) + 0x1u);
                                                                        // set the value and enable override
                pDphyRegs[DPHY_DATALOFFSETCAL_OVRVALUE2_OFFSET] = (uint8_t)(((lVal2 & (uint8_t)0x7fu) << 1u) + 0x1u);
                                                                        // set the value and enable override
                pDphyRegs[DPHY_DATALOFFSETCAL_OVRVALUE3_OFFSET] = (uint8_t)(((lVal3 & (uint8_t)0x7fu) << 1u) + 0x1u);
                                                                        // set the value and enable override
                pDphyRegs[DPHY_DATAL0OFFSETCAL_OVRCNTRL_OFFSET] = 0x4u;  // enable lane calibration override
                pDphyRegs[DPHY_DATAL1OFFSETCAL_OVRCNTRL_OFFSET] = 0x4u;  // enable lane calibration override
                pDphyRegs[DPHY_DATAL2OFFSETCAL_OVRCNTRL_OFFSET] = 0x4u;  // enable lane calibration override
                pDphyRegs[DPHY_DATAL3OFFSETCAL_OVRCNTRL_OFFSET] = 0x4u;  // enable lane calibration override
                pDphyRegs[DPHY_RX_STARTUP_OVERRIDE_OFFSET] = 0x4u;       // bypass calibration
            }

            // dummy step - PLL activation
#ifdef S32R294
            if (csi2UnitNum == RSDK_CSI2_UNIT_1)  // if unit_1 for S32R294
#elif defined(S32R45)
            if ((csi2UnitNum == RSDK_CSI2_UNIT_0) || (csi2UnitNum == RSDK_CSI2_UNIT_2))  // if unit_0 or 2 for S32R45
#endif
            {
                pRegs->DPHY_PLL_VREF_CONFIG.R = 3;  // enable both units PLL from the beginning
            }

            // step 9 - Set the operating frequency HSFREQRNG
            Csi2GetOperatingSpeedMask((uint16_t)pParams->rxClkFreq, &HsFreqRange, &DdlOscFreq);
            pRegs->DPHY_FREQCFG.B.HSFREQRNG = HsFreqRange;

            // step 10 - Configure register Common Block Control (DPHY_ATB_CB_ATB_VBE_SEL)
            pRegs->DPHY_ATB_CB_ATB_VBE_SEL.B.CB_SEL_VREF_LPRX_RW_1_0 = 2u;

            // step 11 - Configure register Common Block Control (DPHY_CB_VBE_SEL)
            pRegs->DPHY_CB_VBE_SEL.B.CB_SEL_VREFCD_LPRX_RW__1__0__ = 2u;

            // step 12 - Write DPHY_CLOCK_LANE_CNTRL
            pRegs->DPHY_CLOCK_LANE_CNTRL.B.RXCLK_RXHS_PULL_LONG_CHANNEL_IF_RW = 1u;

            // step 13 - Configure the DDL target oscillation frequency
            pRegs->DPHY_DDLOSCFREQ_CFG1.B.DDL_OSC_FREQ_TARGET_OVR_RW__7__0__ = (uint8_t)(DdlOscFreq & 0xffu);
            pRegs->DPHY_DDLOSCFREQ_CFG2.B.DDL_OSC_FREQ_TARGET_OVR_RW__11__8__ = (uint8_t)((DdlOscFreq >> 8u) & 0xffu);

            // step 14 - Enable override for configured DDL oscillation frequency to take effect
            pRegs->DPHY_DDLOSCFREQ_OVREN.B.DDL_OSC_FREQ_TARGET_OVR_EN_RW = 1u;

            // step 15 - Configure the config clock frequency CLKFREQRNG
            pRegs->DPHY_FREQCFG.B.CLKFREQRNG = 0xc;

            // step 16 - Set FORCERXMODE = 1 (on all lanes)
            pRegs->TURNCFG.B.FORCERXMODE1 = 1u;
            if (pParams->numLanesRx >= (uint8_t)RSDK_CSI2_LANE_1)
            {
                pRegs->TURNCFG.B.FORCERXMODE2 = 1u;
            }
            if (pParams->numLanesRx >= (uint8_t)RSDK_CSI2_LANE_2)
            {
                pRegs->TURNCFG.B.FORCERXMODE3 = 1u;
            }
            if (pParams->numLanesRx == (uint8_t)RSDK_CSI2_LANE_3)
            {
                pRegs->TURNCFG.B.FORCERXMODE4 = 1u;
            }

            // step 17 - Configure the relevant enables for clock and data lanes for initiating MIPICSI2 data
            //          reception and reverse high speed transmission CLOCK_LANE_EN
            //          DATA_LANE_EN for clock and data lanes respectively
            pRegs->RX_RXENABLE.B.CFG_CLK_LANE_EN = 1u;  // clock enable
            // enable all active lanes
            val = 1u;
            pRegs->RX_RXENABLE.B.CFG_DATA_LANE_EN = (val << (pParams->numLanesRx + 1u)) - 1u;

            // step 18 - Set the lane swap as per physical lane connection
            lanesSwap = 0;
            for (laneId = (uint8_t)RSDK_CSI2_LANE_0; laneId < (uint8_t)RSDK_CSI2_MAX_LANE; laneId++)
            {
                lanesSwap |= ((uint8_t)pParams->lanesMapRx[laneId]) << (laneId * 2u);
            }
            pRegs->RX_RXLANESWAP.R = lanesSwap;

            // step 19 - Configure CFG_FLUSH_CNT
            if (pParams->rxClkFreq < (uint32_t)RSDK_CSI2_FLUSH_CNT_FREQ_LIMIT)
            {
                pRegs->RX_RXENABLE.B.CFG_FLUSH_CNT = (uint32_t)7;  // rates up to 200MHz
            }
            else
            {
                pRegs->RX_RXENABLE.B.CFG_FLUSH_CNT = (uint32_t)3;  // rates over 200MHz
            }  // if(pParams->dphyClkFreq < RSDK_CSI2_FLUSH_CNT_FREQ_LIMIT)

            // step 20 - configure the circular buffers
            pRegs->RX_VCENABLE.R = (uint32_t)0;                // disable all
            pDriverState->statisticsFlag = RSDK_CSI2_STAT_NO;  // disable statistics for unit
            // process first the DC management for each channel on each VC
            for (i = 0; i < (uint32_t)RSDK_CSI2_MAX_VC; i++)
            {
                pDriverState->workingParamVC[i].eventsMask = 0;
                if (Csi2SetOffsetMan(&pDriverState->workingParamVC[i], pParams->pVCconfig[i]) != 0u)
                {
                    pDriverState->statisticsFlag = RSDK_CSI2_STAT_LAST_LINE;
                }
            }
            if (pDriverState->statisticsFlag != RSDK_CSI2_STAT_NO)
            {                                                            // at least one channel with AUTO_DC
                pDriverState->statisticsFlag = pParams->statManagement;  // just copy the value, it was checked before
            }
            rez = Csi2InitAllVcAux(csi2UnitNum, pRegs, pParams);

            if (rez == RSDK_SUCCESS)
            {  // successful VC initialization
                if (pDriverState->statisticsFlag == RSDK_CSI2_STAT_NO)
                {
                    pRegs->RX_STAT_CONFIG.B.STATEN = 0u;  // no statistics at all
                }
                else
                {
                    pRegs->RX_STAT_CONFIG.B.STATEN = 1u;  // at least one channel with auto DC offset
                }

                // step 21 - Configure noext_burnin_res_cal_rw
                pRegs->DPHY_CALTYPE_CNTRL.B.NOEXT_BURNIN_RES_CAL_RW = 1u;  // no external resistor

                Csi2WaitLoop(5);                     // step 22 - wait for 5ns
                pRegs->DPHY_RSTCFG.B.SHUTDWNZ = 1u;  // step 23 - set SHUTDWNZ
                Csi2WaitLoop(5);                     // step 24 - wait for 5ns
                pRegs->DPHY_RSTCFG.B.RSTZ = 1u;      // step 25 - Set the field RSTZ

                // step 26 - Wait till stopstate is observed
                // If the frontend CSI2 interface isn't powered up this check (step 25) will fail.
                // But the interface will start working correctly once the front-end CSI2 powers up.

                // assume step 26 done with success => initialization ok
                // do the other VC initalization - GPIO, SDMA, irq handling
                for (vcId = (uint8_t)RSDK_CSI2_VC_0; vcId < (uint8_t)RSDK_CSI2_MAX_VC; vcId++)
                {
                    // gpio & sdma programming per VC
                    Csi2SetGpio(pRegs, vcId, pParams->pVCconfig[vcId]);
                    Csi2SetSdma(pRegs, vcId, pParams->pVCconfig[vcId]);
                }  // for

                // interrupts programming
                rez = Csi2InitUIrq(csi2UnitNum, pRegs, pParams);
                if (rez == RSDK_SUCCESS)
                {
                    pDriverState->driverState = CSI2_DRIVER_STATE_ON;  // initialized
                }

                // step 27 - Clear FORCERXMODE
                pRegs->TURNCFG.B.FORCERXMODE1 = 0u;
                if (pParams->numLanesRx >= (uint8_t)RSDK_CSI2_LANE_1)
                {
                    pRegs->TURNCFG.B.FORCERXMODE2 = 0u;
                }
                if (pParams->numLanesRx >= (uint8_t)RSDK_CSI2_LANE_2)
                {
                    pRegs->TURNCFG.B.FORCERXMODE3 = 0u;
                }
                if (pParams->numLanesRx == (uint8_t)RSDK_CSI2_LANE_3)
                {
                    pRegs->TURNCFG.B.FORCERXMODE4 = 0u;
                }

                // at this moment the interface is initialized
            } // if (rez == RSDK_SUCCESS)
        } // if (rez == RSDK_SUCCESS)
    } // if (rez == RSDK_SUCCESS)
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_INIT_STEP1, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2ModuleInit *************************/

/*================================================================================================*/
/*
 * @brief       Prepare the output GPIO for the application requirements.
 *
 * @param[in] pRegs     - pointer to unit registry
 * @param[in] csi2VC    - rsdkCsi2VirtChnlId_t value
 * @param[in] pVCparams - pointer to VC parameters
 *
 */
static void Csi2SetGpio(volatile struct MIPICSI2_REG_STRUCT *pRegs, const uint8_t csi2VC,
                        const rsdkCsi2VCParams_t *pVCparams)
{
    uint32_t mask1, maskE;           // masks for GPIO1 & Enable
    uint32_t eraseMask, eraseMaskE;  // erase masks for the same as above

    if (pVCparams != NULL)
    {
        // initialize the masks
        mask1 = (uint32_t)pVCparams->gpio1Mask;
        maskE = (uint32_t)pVCparams->gpio2EnaMask;
        maskE <<= 12u;
        maskE |= (uint32_t)pVCparams->gpio1EnaMask;
        eraseMask = 0xff;
        eraseMaskE = 0x7007;
        // get the final usable masks, according the VC
        switch (csi2VC)
        {
            case (uint8_t)RSDK_CSI2_VC_0:  // for VC_0 the masks remains as is
                break;
            case (uint8_t)RSDK_CSI2_VC_1:  // shifts for VC_1
                mask1 <<= 8u;
                maskE <<= 3u;
                eraseMask <<= 8u;
                eraseMaskE <<= 3u;
                break;
            case (uint8_t)RSDK_CSI2_VC_2:  // shifts for VC_2
                mask1 <<= 16u;
                maskE <<= 6u;
                eraseMask <<= 16u;
                eraseMaskE <<= 6u;
                break;
            default:  // shifts for VC_3
                mask1 <<= 24u;
                maskE <<= 9u;
                eraseMask <<= 24u;
                eraseMaskE <<= 9u;
                break;
        }
        // set all targeted bits to 0
        pRegs->TRIGGER_GPIO1.R &= eraseMask;
        pRegs->TRIGGEREN_GPIO.R &= eraseMaskE;
        // set the required values
        pRegs->TRIGGER_GPIO1.R |= mask1;
        pRegs->TRIGGEREN_GPIO.R |= maskE;
    }
}
/* Csi2SetGpio *************************/

/*================================================================================================*/
/*
 * @brief       Prepare the output SDMA for the application requirements.
 * @details     Prepare the output SDMA for the application requirements.
 *
 * @param[in] pRegs     - pointer to unit registry
 * @param[in] csi2VC    - rsdkCsi2VirtChnlId_t value
 * @param[in] pVCparams - pointer to VC parameters
 *
 */
static void Csi2SetSdma(volatile struct MIPICSI2_REG_STRUCT *pRegs, const uint8_t csi2VC,
                        const rsdkCsi2VCParams_t *pVCparams)
{
    uint32_t mask1, maskE;           // masks for SDMA1 & Enable
    uint32_t eraseMask, eraseMaskE;  // erase masks for the same as above
    if (pVCparams != NULL)
    {
        // initialize the masks
        mask1 = (uint32_t)pVCparams->sdma1Mask;
        maskE = (uint32_t)pVCparams->sdma2EnaMask;
        maskE <<= 12u;
        maskE |= (uint32_t)pVCparams->sdma1EnaMask;
        eraseMask = 0xff;
        eraseMaskE = 0x7007;
        // get the final usable masks, according the VC
        switch (csi2VC)
        {
            case (uint8_t)RSDK_CSI2_VC_0:
                break;
            case (uint8_t)RSDK_CSI2_VC_1:
                mask1 <<= 8u;
                maskE <<= 3u;
                eraseMask <<= 8u;
                eraseMaskE <<= 3u;
                break;
            case (uint8_t)RSDK_CSI2_VC_2:
                mask1 <<= 16u;
                maskE <<= 6u;
                eraseMask <<= 16u;
                eraseMaskE <<= 6u;
                break;
            default:
                mask1 <<= 24u;
                maskE <<= 9u;
                eraseMask <<= 24u;
                eraseMaskE <<= 9u;
                break;
        }
        // set all targeted bits to 0
        pRegs->TRIGGER_SDMA1.R &= eraseMask;
        pRegs->TRIGGEREN_SDMA.R &= eraseMaskE;
        // set the required values
        pRegs->TRIGGER_SDMA1.R |= mask1;
        pRegs->TRIGGEREN_SDMA.R |= maskE;
    }
}
/* Csi2MSetSdma *************************/

/*================================================================================================*/
/*
 * @brief       Procedure for getting the masks to use for the communication speed.
 * @details     Procedure for getting the masks to use for the communication speed.
 *              For the moment, for any speed bigger than the maximum in documentation,
 *              the biggest available value is returned and always is successful.
 *
 * @param[in]   speed       - communication speed, in MHz
 * @param[in]   pHSFREQRNG  - pointer to specific mask value
 * @param[in]   pDDLOSCFREQ - pointer to specific mask value
 *
 * @return      RSDK_SUCCESS   Successful initialization
 *              RSDK_CSI2_WRG_START_PARAMS       Wrong unit id
 *
 */
static void Csi2GetOperatingSpeedMask(const uint16_t speed, uint8_t *pHSFREQRNG, uint16_t *pDDLOSCFREQ)
{
    /*
     * @brief       Various tables to get correct speed masks.
     *
     */
    // programmable operating speed in Mbps, according to RM, Rev.1 Draft I, pag. 6225-6227
    static const uint16_t gsCsi2OperatingSpeed[] = {
        80,   90,   100,  110,  120,  130,  140,  150,  160,  170,  180,  190,  205,  220,  235,  250,
        275,  300,  325,  350,  400,  450,  500,  550,  600,  650,  700,  750,  800,  850,  900,  950,
        1000, 1050, 1100, 1150, 1200, 1250, 1300, 1350, 1400, 1450, 1500, 1550, 1600, 1650, 1700, 1750,
        1800, 1850, 1900, 1950, 2000, 2050, 2100, 2150, 2200, 2250, 2300, 2350, 2400, 2450, 2500, 0};
    // HsFreqRange values table, correlated to Csi2MOperatingSpeed
    static const uint8_t gsCsi2OperatingSMask[] = {0,  16, 32, 48, 1,  17, 33, 49, 2,  18, 34, 50, 3,  19, 35, 51,
                                                   4,  20, 37, 53, 37, 22, 38, 55, 7,  24, 40, 57, 9,  25, 41, 58,
                                                   10, 26, 42, 59, 11, 27, 43, 60, 12, 28, 44, 61, 13, 29, 46, 62,
                                                   14, 30, 47, 63, 15, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73

    };
    // OsFreqTarget values table, correlated to Csi2MOperatingSpeed
    static const uint16_t gsCsi2OscFreqTarget[] = {
        489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489,
        489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489,
        489, 303, 313, 323, 333, 342, 352, 362, 372, 381, 391, 401, 411, 420, 430, 440, 450, 459, 469, 479, 489};

    uint32_t i;

    i = 0u;
    while (gsCsi2OperatingSpeed[i] != 0u)  // 0 speed is the marker for the end
    {
        if (speed <= gsCsi2OperatingSpeed[i])
        {
            if (i != 0u)
            {
                // check for the closest speed value
                if ((speed - gsCsi2OperatingSpeed[i - 1u]) < (gsCsi2OperatingSpeed[i] - speed))
                {
                    i--;
                }
            }
            break;  // the current speed is the first bigger or equal
        }
        i++;
    }  // while
    if (gsCsi2OperatingSpeed[i] == 0u)
    {
        i--;  // stopped on the final position, so back one step
    }
    *pHSFREQRNG = gsCsi2OperatingSMask[i];
    *pDDLOSCFREQ = gsCsi2OscFreqTarget[i];
}
/* Csi2GetOperatingSpeedMask *************************/

/*================================================================================================*/
/*
 * @brief       Procedure to introduce a simple delay in execution.
 * @details     According to the platform/setup and CSI2_LOOP_CONSTANT this delay may signify different time.
 *              But in CSI2 interface this delay is used only for ns delays,
 *              which must not be precise. It is adapted to platform execution time;
 *
 * @param[in]   loops   Number of loops to wait (ns)
 *
 */
static void Csi2WaitLoop(uint32_t loops)
{
    uint32_t iLoop;

    iLoop = loops;
    while (iLoop != (uint32_t)0)
    {
        iLoop--;
    }
}
/* Csi2WaitLoop *************************/

/*================================================================================================*/
/*
 * @brief       Get the number of bits per each received sample.
 * @details     Get the number of bits per each received sample.
 *
 * @param[in]   dataType    - expected data type
 *
 * @return      0           = wrong data type
 *              anything else, the number of bits per sample
 *
 */
static uint16_t Csi2GetDataTypeBits(const rsdkCsi2DataStreamType_t dataType)
{
    uint16_t rez;

    // get the conversion rate input data -> memory data, per sample
    switch ((uint16_t)dataType & RSDK_CSI2_NORM_DTYPE_MASK)
    {
        case (uint16_t)RSDK_CSI2_DATA_TYPE_EMBD:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR0:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR1:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR2:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR3:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR4:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR5:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_USR7:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RAW8:
            rez = 8;  // 8 bits per sample
            break;
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RAW10:
            rez = 10;  // 10 bits per sample
            break;
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RAW12:
            rez = 12;  // 12 bits per sample
            break;
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RAW14:
            rez = 14;  // 14 bits per sample
            break;
        case (uint16_t)RSDK_CSI2_DATA_TYPE_YUV422_8:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RGB565:
        case (uint16_t)RSDK_CSI2_DATA_TYPE_16_FROM_8:
            rez = 16;  // 2 bytes per sample
            break;
        case (uint16_t)RSDK_CSI2_DATA_TYPE_RGB888:
            rez = 24;  // 24 bits per sample
            break;
        case (uint16_t)RSDK_CSI2_DATA_TYPE_YUV422_10:
            rez = 20;  // 20 bits per sample
            break;
        default:
            rez = 0;  // unknown/unacceptable data type
            break;
    }
    return rez;
}
/* Csi2GetDataTypeBits *************************/

/*================================================================================================*/
/*
 * @brief       Copy the offset data for future use.
 * @details     Copy the offset data for future use.
 *
 * @param[in]   pVCDrvState - pointer to VC driver state
 * @param[in]   pVCparams   - pointer to VC parameters
 *
 * @return      0 = no statistics required, 1 = statistics required
 *
 */
static uint8_t Csi2SetOffsetMan(rsdkCsi2VCDriverState_t *pVCDrvState, const rsdkCsi2VCParams_t *pVCparams)
{
    uint32_t i;
    uint32_t numChannel;
    uint8_t  statFlag;  // channel statistics required
    uint8_t  mask;
    uint8_t  hasComplex;  // indicator for real/complex channels
    uint16_t dcRef;       // DC reference

    statFlag = 0;           // statistics status (0 = not necessary)
    if (pVCparams != NULL)  // only if the pointer is defined
    {
        mask = 1;  // mask for auto update for DC
        numChannel = (uint32_t)Csi2PlatformGetChannelNum(pVCDrvState);
        if (numChannel == pVCparams->channelsNum)
        {
            hasComplex = (uint8_t)0;  // only real channels
        }
        else
        {
            hasComplex = 1;  // complex channels
        }
        for (i = (uint32_t)RSDK_CSI2_CHANNEL_A; i < numChannel; i++)
        {
            if (hasComplex != (uint8_t)0)
            {  // channels are complex
                if ((i & (uint32_t)1) == (uint32_t)0)
                {
                    dcRef = (uint16_t)pVCparams->offsetCompReal[i / (uint32_t)2];  // even channels are real
                }
                else
                {
                    dcRef = (uint16_t)pVCparams->offsetCompImg[i / (uint32_t)2];  // odd channels are imaginary
                }                                                                 // if((i & 1) == 0)
            }
            else
            {
                dcRef = (uint16_t)pVCparams->offsetCompReal[i];  // only real channels
            }                                                    // if(hasComplex != 0)
            if (dcRef == (uint16_t)RSDK_CSI2_OFFSET_AUTOCOMPUTE)
            {
                statFlag |= 1u;  // need statistics
            }
            pVCDrvState->statDC[i].reqChannelDC = (int16_t)dcRef;
            dcRef <<= 4u;
            pVCDrvState->statDC[i].channelDC = (int16_t)dcRef;
            mask <<= 1u;
        }  // for
    }      // if(pVCparams != NULL)
    // return a centralized situation for statistics
    return (statFlag);
}
/* Csi2SetOffsetMan *************************/

/*================================================================================================*/
/*
 * @brief           Function for CSI2 memory map initialization.
 * @details         The function initialize the pointer to CSI2 registry.
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 *         RSDK_CSI2_DRV_WRG_UNIT_ID - an incorrect unit was specified
 *
 */
static rsdkStatus_t Csi2RegsMap(const rsdkCsi2UnitId_t unitId, volatile struct MIPICSI2_REG_STRUCT **pRegs)
{
    rsdkStatus_t rez = RSDK_SUCCESS;
    switch (unitId)
    {
        case RSDK_CSI2_UNIT_0:
            *pRegs = (volatile struct MIPICSI2_REG_STRUCT *)((volatile void *)
#ifdef S32R45
#ifdef linux
                gpRsdkCsi2Device[unitId]->pMemMapVirtAddr
#else
                &MIPICSI2_0
#endif
#else
                &MIPICSI_0
#endif
            );
            break;
        case RSDK_CSI2_UNIT_1:
            *pRegs = (volatile struct MIPICSI2_REG_STRUCT *)((volatile void *)
#ifdef S32R45
#ifdef linux
                gpRsdkCsi2Device[unitId]->pMemMapVirtAddr
#else
                &MIPICSI2_1
#endif
#else
                &MIPICSI_1
#endif
            );
            break;
#ifdef S32R45
        case RSDK_CSI2_UNIT_2:
            *pRegs = (volatile struct MIPICSI2_REG_STRUCT *)((volatile void *)
#ifdef linux
                gpRsdkCsi2Device[unitId]->pMemMapVirtAddr
#else
                &MIPICSI2_2
#endif
            );
            break;
        case RSDK_CSI2_UNIT_3:
            *pRegs = (volatile struct MIPICSI2_REG_STRUCT *)((volatile void *)
#ifdef linux
                gpRsdkCsi2Device[unitId]->pMemMapVirtAddr
#else
                &MIPICSI2_3
#endif
            );
            break;
#endif
        default:
            rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
            break;
    }  // switch
    return rez;
}
/* Csi2RegsMap *************************/

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 ==================================================================================================*/

/*================================================================================================*/
/*
 * @brief       The function called from API to initialize the module.
 * @details     The function check the parameters, initialize the interface and
 *              save a copy of the parameters for future use.
 *
 * @param[in]   unitId         - the unit number, RSDK_CSI2_UNIT_0 ... MAX
 * @param[in]   pCsi2InitParam - pointer to CSI2 initialization parameters
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 *          other error values, according to the failure
 */
rsdkStatus_t Csi2PlatformModuleInit(const rsdkCsi2UnitId_t unitId, const rsdkCsi2InitParams_t *pCsi2InitParam)
{
    // copies of the auxiliary data parameters
    static rsdkCsi2VCParams_t gsCsi2AuxParamCopy[RSDK_CSI2_MAX_UNITS][RSDK_CSI2_MAX_VC];
    static rsdkCsi2VCParams_t gsCsi2VCParamCopy[RSDK_CSI2_MAX_UNITS][RSDK_CSI2_MAX_VC];
#ifdef linux
    rsdkCsi2VCDriverState_t   *pVcDriverState;
    uint64_t     dataRange;
#endif

    rsdkStatus_t rez;  //call result
    uint32_t     i;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_INIT, (uint32_t)CSI2_SEQ_BEGIN);
    //Check the parameters
    rez = Csi2PlatformCheckInitParams(unitId, pCsi2InitParam);
    if (rez == RSDK_SUCCESS)
    {  // correct parameters, continue
        // copy the current parameters for future use, if it will be required
        if (&gsCsi2UnitParamCopy[(uint8_t)unitId] != pCsi2InitParam)
        {
            gsCsi2UnitParamCopy[(uint8_t)unitId] = *pCsi2InitParam;  // base initialization
            for (i = 0; i < (uint32_t)RSDK_CSI2_MAX_VC; i++)
            {
                if (pCsi2InitParam->pVCconfig[i] != NULL)
                {
                    // copy the VC parameters
                    gsCsi2VCParamCopy[(uint8_t)unitId][i] = *pCsi2InitParam->pVCconfig[i];
                    gsCsi2UnitParamCopy[(uint8_t)unitId].pVCconfig[i] = &gsCsi2VCParamCopy[(uint8_t)unitId][i];
                    gCsi2Settings[unitId].workingParamVC[i].pVCParams = &gsCsi2VCParamCopy[(uint8_t)unitId][i];
                    // init the frames counters
                    gsCsi2FramesCounter[(uint8_t)unitId][i] = 0;
                }
                else
                {
                    gsCsi2UnitParamCopy[(uint8_t)unitId].pVCconfig[i] = NULL;
                    gCsi2Settings[unitId].workingParamVC[i].pVCParams = NULL;
                }
                if (pCsi2InitParam->pAuxConfig[i] != NULL)
                {
                    // copy the aux VC parameters
                    gsCsi2AuxParamCopy[(uint8_t)unitId][i] = *pCsi2InitParam->pAuxConfig[i];
                    gsCsi2UnitParamCopy[(uint8_t)unitId].pAuxConfig[i] = &gsCsi2AuxParamCopy[(uint8_t)unitId][i];
                }
                else
                {
                    gsCsi2UnitParamCopy[(uint8_t)unitId].pAuxConfig[i] = NULL;
                }
            }
        }
        rez = Csi2ModuleInit(unitId, pCsi2InitParam);
    }
    if (rez == RSDK_SUCCESS)
    {  // good module initialization
#ifdef linux
        // initialization of the 
        for(i = 0; i < (uint32_t)RSDK_CSI2_MAX_VC; i++)
        {
            pVcDriverState = &gCsi2Settings[unitId].workingParamVC[i];
            if(pVcDriverState->pVCParams!= NULL) 
            {
                dataRange = (uint64_t)pVcDriverState->pVCParams->bufNumLines;
                dataRange *= (uint64_t)pVcDriverState->pVCParams->bufLineLen;
                // map the data, for statistics usage/computation
                pVcDriverState->pVirtData = ioremap_nocache((uintptr_t)pVcDriverState->pVCParams->pBufData, 
                        dataRange + 0x100UL);
            }
        }
#endif
    
        // manage the irq callbacks
        for (i = (uint32_t)RSDK_CSI2_PATH_ERR_IRQ_ID; i < (uint32_t)RSDK_CSI2_MAX_IRQ_ID; i++)
        {
            if (gsCsi2UnitParamCopy[(uint8_t)unitId].pCallback[i] == NULL)
            {
                gsCsi2UnitParamCopy[(uint8_t)unitId].pCallback[i] =
                    gsCsi2UnitParamCopy[(uint8_t)unitId].pCallback[(uint8_t)RSDK_CSI2_RX_ERR_IRQ_ID];
                gCsi2Settings[(uint8_t)unitId].pCallback[i] =
                    gsCsi2UnitParamCopy[(uint8_t)unitId].pCallback[(uint8_t)RSDK_CSI2_RX_ERR_IRQ_ID];
            }
        }
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_INIT, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2PlatformModuleInit *************************/

/*================================================================================================*/
/*
 * @brief          The function stop the CSI2 receive module.
 * @details        The receive module of the CSI2 interface is stopped.
 *                 The action succeed only if the interface is ON or already stopped.
 *                 The receive will stop after the end of the current packet is finished.
 *                 Assume the unitId was checked before.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS)
 *
 * @return  rsdkStatus_t - success or error status information.
 * @retval  RSDK_SUCCESS - execution was successful
 *          RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the stopped status
 *          RSDK_CSI2_DRV_WRG_STATE         - if the interface is in a wrong state
 *
 * @pre   It can be called when the reception need to be stopped. A CSI2_Init(...) call must be done previously.
 *
 */
rsdkStatus_t Csi2PlatformRxStop(const rsdkCsi2UnitId_t unitId)
{
    rsdkStatus_t            rez;
    rsdkCsi2DriverParams_t *pDriverState;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_RX_STOP, (uint32_t)CSI2_SEQ_BEGIN);
    if ((uint8_t)unitId >= (uint8_t)RSDK_CSI2_MAX_UNITS)
    {
        rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
    }
    else
    {
        pDriverState = &gCsi2Settings[(uint8_t)unitId];
        if ((pDriverState->driverState != CSI2_DRIVER_STATE_ON) &&
            (pDriverState->driverState != CSI2_DRIVER_STATE_STOP))
        {
            if (pDriverState->driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)
            {
                rez = RSDK_CSI2_DRV_NOT_INIT;
            }
            else
            {
                rez = RSDK_CSI2_DRV_WRG_STATE;  // the driver was not initialized before
            }
        }
        else
        {
            rez = RSDK_SUCCESS;  //default - success result
            gpMipiCsi2Regs[(uint8_t)unitId]->RX_RXENABLE.B.CFG_CLK_LANE_EN =
                (uint32_t)CSI2_RXEN_RXEN_DISABLED;  // disable PHY clk
            if (gpMipiCsi2Regs[(uint8_t)unitId]->RX_RXENABLE.B.CFG_CLK_LANE_EN != (uint32_t)CSI2_RXEN_RXEN_DISABLED)
            {
                rez = RSDK_CSI2_DRV_HW_RESPONSE_ERROR;  // incorrect answer from hardware
            }
            else
            {
                pDriverState->driverState = CSI2_DRIVER_STATE_STOP;  // unit stopped
            }
        }
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_RX_STOP, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2RxStop *************************/

/*================================================================================================*/
/*
 * @brief          The function start the CSI2 receive module.
 * @details        The receive module of the CSI2 interface is started.
 *                 This action succeed only after a successful Csi2RxStop or the unit is actually ON.
 *                 Assume the unitId was checked before.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS)
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the running status
 *         RSDK_CSI2_DRV_WRG_STATE         - the interface is in a wrong state and a start is not appropriate
 *
 * @pre   It can be called when the reception is stopped, using an previous CSI2_RxStop() call.
 *
 */
rsdkStatus_t Csi2PlatformRxStart(const rsdkCsi2UnitId_t unitId)
{
    rsdkStatus_t            rez;
    rsdkCsi2DriverParams_t *pDriverState;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_RX_START, (uint32_t)CSI2_SEQ_BEGIN);
    if ((uint8_t)unitId >= (uint8_t)RSDK_CSI2_MAX_UNITS)
    {
        rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
    }
    else
    {
        pDriverState = &gCsi2Settings[(uint8_t)unitId];
        rez = RSDK_SUCCESS;  //default - success result
        switch (pDriverState->driverState)
        {
            case CSI2_DRIVER_STATE_STOP:  // interface really stopped
                gpMipiCsi2Regs[(uint8_t)unitId]->RX_RXENABLE.B.CFG_CLK_LANE_EN =
                    (uint32_t)CSI2_RXEN_RXEN_ENABLED;  // enable clk
                if (gpMipiCsi2Regs[(uint8_t)unitId]->RX_RXENABLE.B.CFG_CLK_LANE_EN != (uint32_t)CSI2_RXEN_RXEN_ENABLED)
                {
                    rez = RSDK_CSI2_DRV_HW_RESPONSE_ERROR;  // incorrect answer from hardware
                }
                else
                {
                    pDriverState->driverState = CSI2_DRIVER_STATE_ON;  // interface on now
                }
                break;
            case CSI2_DRIVER_STATE_NOT_INITIALIZED:  // nothing to do, the interface is already on
                rez = RSDK_CSI2_DRV_NOT_INIT;
                break;
            case CSI2_DRIVER_STATE_ON:  // nothing to do, the interface is already on
                break;
            default:  // wrong interface status (not init or off)
                rez = RSDK_CSI2_DRV_WRG_STATE;
                break;
        }
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_RX_START, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2RxStart *************************/

/*================================================================================================*/
/*
 * @brief       The function power off the CSI2 module.
 * @details     All modules of the CSI2-PHY interface are placed in reset mode and kept there.
 *              The action succeed only if the interface is initialized.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS)
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful.
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the running status
 *         RSDK_CSI2_DRV_NOT_INIT          - the interface is initialized
 *
 * @pre   It can be called when the reception need to be stopped and/or the interface to be powered off.
 */
rsdkStatus_t Csi2PlatformPowerOff(const rsdkCsi2UnitId_t unitId)
{
    rsdkStatus_t            rez;
    rsdkCsi2DriverParams_t *pDriverState;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_POWER_OFF, (uint32_t)CSI2_SEQ_BEGIN);
    rez = RSDK_SUCCESS;

    if ((uint8_t)unitId >= (uint8_t)RSDK_CSI2_MAX_UNITS)
    {
        rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
    }
    else
    {
        pDriverState = &gCsi2Settings[(uint8_t)unitId];
        if (pDriverState->driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)
        {
            rez = RSDK_CSI2_DRV_NOT_INIT;
        }
        else
        {  // only in an appropriate state
            if (pDriverState->driverState != CSI2_DRIVER_STATE_OFF)
            {
                rez = Csi2PlatformRxStop(unitId);  // first - stop the clock
            }
            if (rez == RSDK_SUCCESS)
            {
                gpMipiCsi2Regs[(uint8_t)unitId]->DPHY_RSTCFG.B.SHUTDWNZ = 0;  // poweroff PHY
                if (gpMipiCsi2Regs[(uint8_t)unitId]->DPHY_RSTCFG.B.SHUTDWNZ != (uint32_t)0)
                {
                    rez = RSDK_CSI2_DRV_HW_RESPONSE_ERROR;  // incorrect answer from hardware
                }
                else
                {
                    pDriverState->driverState = CSI2_DRIVER_STATE_OFF;  // the new state for interface
                }
            }
        }
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_POWER_OFF, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2PlatformPowerOff *************************/

/*================================================================================================*/
/*
 * @brief       The function power on the CSI2 module.
 * @details     All modules of the interface is restarted.
 *              This action can be done only if the interface is in OFF state.
 *              If the action fail, the new state for interface will be NOT_INITIALIZED.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS)
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful.
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the running status.
 *         RSDK_CSI2_DRV_WRG_STATE         - the interface is not OFF
 *
 * @pre   It can be called only when the reception is off.
 *
 */
rsdkStatus_t Csi2PlatformPowerOn(const rsdkCsi2UnitId_t unitId)
{
    rsdkStatus_t            rez;
    rsdkCsi2DriverParams_t *pDriverState;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_POWER_ON, (uint32_t)CSI2_SEQ_BEGIN);
    if ((uint8_t)unitId >= (uint8_t)RSDK_CSI2_MAX_UNITS)
    {
        rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
    }
    else
    {
        pDriverState = &gCsi2Settings[(uint8_t)unitId];
        if (pDriverState->driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)
        {
            rez = RSDK_CSI2_DRV_NOT_INIT;
        }
        else
        {
            if (pDriverState->driverState == CSI2_DRIVER_STATE_OFF)
            {  // only in an appropriate state
                rez = Csi2PlatformModuleInit(unitId,
                                             &gsCsi2UnitParamCopy[(uint8_t)unitId]);  // return the init status
            }
            else
            {
                rez = RSDK_CSI2_DRV_WRG_STATE;  // the driver was not initialized before
            }
        }
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_POWER_OFF, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2PlatformPowerOn *************************/

/*================================================================================================*/
/*
 * @brief          The function ask for the current status of the interface.
 * @details        The function return the status of the interface at the moment of the call.
 *                 The return status will return one of the possible status of the interface.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS)
 *
 * @return rsdkStatus_t - the current status of the interface.
 * @retval   RSDK_CSI2_DRV_POWERED_OFF - The PHY interface if powered off.
 * @retval   RSDK_CSI2_DRV_RX_STOPPED - The Rx interface if stopped.
 * @retval   RSDK_CSI2_DRV_LINE_ON - The Rx interface if on, out of reception.
 * @retval   RSDK_CSI2_DRV_LINE_RECEIVING - The Rx interface is receiving data.
 * @retval   RSDK_CSI2_DRV_STATE_LINE_MARK - The Rx lane is in MARK state
 *
 * @pre   It can be called at any time, to check the current status of the interface.
 *           Depending of the returned status, some times is necessary to loop on this call to really
 *           understand how interface is working.
 *
 */
rsdkStatus_t Csi2PlatformGetInterfaceStatus(const rsdkCsi2UnitId_t unitId)
{
    rsdkStatus_t    rez = RSDK_SUCCESS;
    
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_IFACE_STAT, 
                                                                (uint32_t)CSI2_SEQ_BEGIN);
    if (((int8_t)unitId < (int8_t)RSDK_CSI2_UNIT_0) || ((int8_t)unitId >= (int8_t)RSDK_CSI2_MAX_UNITS))
    {
        rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
    }
    if ((rez == RSDK_SUCCESS) && ((gpMipiCsi2Regs[(uint8_t)unitId] == NULL) ||
                                  (gCsi2Settings[(uint8_t)unitId].driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)))
    {
        rez = RSDK_CSI2_DRV_NOT_INIT;  // the driver was not initialized before
    }
    if (rez == RSDK_SUCCESS)
    {
        if (gCsi2Settings[(uint8_t)unitId].driverState == CSI2_DRIVER_STATE_OFF)
        {
            rez = RSDK_CSI2_DRV_POWERED_OFF;
        }
        else if (gCsi2Settings[(uint8_t)unitId].driverState == CSI2_DRIVER_STATE_STOP)
        {
            rez = RSDK_CSI2_DRV_RX_STOPPED;
        }
        else
        {
            rez = RSDK_CSI2_DRV_STATE_ON;
        }
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_IFACE_STAT, (uint32_t)CSI2_SEQ_END);
    return rez;  // The interface status is Lane0 status.
}
/* Csi2PlatformGetInterfaceStatus *************************/

/*================================================================================================*/
/*
 * @brief          The function ask for the current status of the specific lane.
 * @details        The function return the status of the interface or specific lane at the moment of the call.
 *                 The return status will return one of the possible status of the interface/lane.
 *
 * @param[in]   unitID      - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS)
 * @param[in]   laneNr      - lane number, 0..max_lane
 *
 * @return rsdkStatus_t - the current status of the interface.
 * @retval     RSDK_CSI2_DRV_INVALID_LANES_NR - incorrect lane number
 * @retval     RSDK_CSI2_DRV_STATE_LINE_MARK - The Rx lane in Mark-1 state.
 * @retval     RSDK_CSI2_DRV_LINE_ON - The Rx lane active, but not currently receiving data.
 * @retval     RSDK_CSI2_DRV_LINE_RECEIVING - The Rx lane is receiving data.
 * @retval     RSDK_CSI2_DRV_LINE_OFF - The lane is not used.
 *
 * @pre   It can be called at any time, to check the current status of the interface/lane.
 *           Depending of the returned status, some times is necessary to loop on this call to
 *           really understand how lane is working.
 */
rsdkStatus_t Csi2PlatformGetLaneStatus(const rsdkCsi2UnitId_t unitId, const uint32_t laneNr)
{
    rsdkStatus_t       rez;
    volatile uint32_t *pRegs;
    uint32_t           regStat;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_LANE_STAT, (uint32_t)CSI2_SEQ_BEGIN);
    if ((uint8_t)unitId >= (uint8_t)RSDK_CSI2_MAX_UNITS)
    {
        rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
    }
    else if ((uint32_t)laneNr >= (uint32_t)RSDK_CSI2_MAX_LANE)
    {
        rez = RSDK_CSI2_DRV_INVALID_LANES_NR;  // Incorrect lane number.
    }                                          //if (laneNr <= (uint32_t) RSDK_CSI2_MAX_LANE_NUMBER)
    else
    {
        rez = Csi2PlatformGetInterfaceStatus(unitId);
    }
    if ((rez == RSDK_CSI2_DRV_STATE_ON) && (gCsi2Settings[(uint8_t)unitId].driverState != CSI2_DRIVER_STATE_ON))
    {
        rez = RSDK_CSI2_DRV_WRG_STATE;
    }
    if (rez == RSDK_CSI2_DRV_STATE_ON)
    {
        regStat = *(volatile uint32_t *)(volatile void *)(&gpMipiCsi2Regs[(uint8_t)unitId]->RX_RXNULANE);
        if (laneNr >= regStat)
        {
            rez = RSDK_CSI2_DRV_STATE_LINE_OFF;
        }
        else
        {
            rez = RSDK_CSI2_DRV_STATE_LINE_ON;  // default - line on
            pRegs = (volatile uint32_t *)(volatile void *)(&gpMipiCsi2Regs[(uint8_t)unitId]->RX_LANCS[laneNr]);
            regStat = *pRegs;
            if ((regStat & CSI2_LAN_CS_MARK) != 0u)
            {
                rez = RSDK_CSI2_DRV_STATE_LINE_MARK;  // default - line on
            }
            if ((regStat & CSI2_LAN_CS_ULPA) != 0u)
            {
                rez = RSDK_CSI2_DRV_STATE_LINE_ULPA;
            }
            if ((regStat & CSI2_LAN_CS_STOP) != 0u)
            {
                rez = RSDK_CSI2_DRV_STATE_LINE_STOP;
            }
            if ((regStat & CSI2_LAN_CS_RXACTH) != 0u)
            {
                rez = RSDK_CSI2_DRV_STATE_LINE_REC;
            }
            if ((regStat & CSI2_LAN_CS_RXVALH) != 0u)
            {
                rez = RSDK_CSI2_DRV_STATE_LINE_VRX;
            }
        }
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_LANE_STAT, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2PlatformGetLaneStatus *************************/

/*================================================================================================*/
/*
 * @brief       Get the real channels number of the VC..
 *
 * @param[in]   pVCState - pointer to VC driver state
 *
 * @return      The channels number
 *
 */
uint8_t Csi2PlatformGetChannelNum(const rsdkCsi2VCDriverState_t *pVCState)
{
    uint8_t rez;

    if ((pVCState->outputDataMode & (uint16_t)RSDK_CSI2_VC_BUF_COMPLEX_DATA) != (uint16_t)0)
    {
        rez = pVCState->pVCParams->channelsNum * (uint8_t)2;  // complex data, so double the channel number
    }
    else
    {
        rez = pVCState->pVCParams->channelsNum;  // only real data, the specified number
    }                                            // if((pVCState->outputDataMode & RSDK_CSI2_VC_BUF_COMPLEX_DATA) != 0)
    return rez;
}
/* Csi2PlatformGetChannelNum *************************/

/*================================================================================================*/
/*
 * @brief       Procedure to get the real buffer length for a line.
 * @details     The procedure compute the necessary length according to the input parameters.
 *              NOTE : autoStatistics must be 1 even only one channel in the entire unit need the auto statistic.
 *
 * @param[in]   dataType        Type of data to receive, normally RSDK_CSI2M_DATA_TYPE_RAW12 for radar
 *                              and possible auxiliary adjustments (only for auxiliary buffer line length).
 * @param[in]   nrChannels      Number of channels, 1...8; usually this number is
 *                              the number of active antennas, multiply by 2 if complex acquisition.
 *                              It must be specified even for auxiliary data
 * @param[in]   samplesPerChirp Samples per chirp and channel. It must be specified even for auxiliary data.
 * @param[in]   autoStatistics  The driver must calculate the statistics per chirp (1) or not (0).
 *                              No necessary for auxiliary data.
 *
 * @return      The necessary memory amount for one line (chirp) if not 0.
 *              Wrong input parameter if 0.
 *
 */
uint32_t Csi2PlatformGetBufferRealLineLen(const rsdkCsi2DataStreamType_t dataType, const uint32_t numChannels,
                                          const uint32_t samplesPerChirp, const uint8_t autoStatistics)
{
    uint32_t rez;

    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_REAL_LEN, (uint32_t)CSI2_SEQ_BEGIN);
    if ((autoStatistics > (uint8_t)1) || (numChannels > (uint32_t)RSDK_CSI2_MAX_CHANNEL) ||
        (numChannels == (uint32_t)0) || (samplesPerChirp == (uint32_t)0))
    {
        rez = 0;
    }
    else
    {
        rez = Csi2GetOutBytesPerSample(dataType);  // get a magic number for the length of each sample
        if (rez != (uint32_t)0)
        {
            // good base data type
            // all parameters are good, do the final computation
            rez *= numChannels * samplesPerChirp;
            if (dataType > RSDK_CSI2_DATA_TYPE_AUX_0_NO_DROP)
            {  // it is auxiliary data, not regular data
                switch ((uint32_t)dataType & (~RSDK_CSI2_NORM_DTYPE_MASK))
                {
                    default:  // not recognized auxiliary mode => error
                        rez = 0;
                        break;
                    case (uint32_t)RSDK_CSI2_DATA_TYPE_AUX_0_NO_DROP:
                        break;
                    case (uint32_t)RSDK_CSI2_DATA_TYPE_AUX_0_DR_1OF2:
                    case (uint32_t)RSDK_CSI2_DATA_TYPE_AUX_1_NO_DROP:
                        rez = (rez + 1u) / 2u;
                        break;
                    case (uint32_t)RSDK_CSI2_DATA_TYPE_AUX_0_DR_3OF4:
                        rez = (rez + 3u) / 4u;
                        break;
                }
            }
            else
            {  // it is regular data
                // add the required space for statistics
                rez += (uint32_t)autoStatistics * (uint32_t)RSDK_CSI2_LINE_STAT_LENGTH;  // adding the stats
                rez = RSDK_CSI2_UPPER_ALIGN_TO_(rez, (uint32_t)16);                      // align to upper 16 bytes
            }
        }  // if(rez != 0)
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_REAL_LEN, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2PlatformGetBufferRealLineLen *************************/

/*================================================================================================*/
/*
 * @brief       The function increase the current frames counter
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS)
 * @param[in] vcId      - VC number, RSDK_CSI2_VC_0 ... MAX
 *
 * @return uint32_t - the current frames counter
 *
 */
void Csi2PlatformIncFramesCounter(const rsdkCsi2UnitId_t unitId, const uint32_t vcId)
{
    if (((uint8_t)unitId < (uint8_t)RSDK_CSI2_MAX_UNITS) && (vcId < (uint32_t)RSDK_CSI2_MAX_VC))
    {
        gsCsi2FramesCounter[(uint8_t)unitId][(uint8_t)vcId]++;
        if ((gsCsi2FramesCounter[(uint8_t)unitId][(uint8_t)vcId] + 1u) == 0u)
        {
            gsCsi2FramesCounter[(uint8_t)unitId][(uint8_t)vcId] = 0u;  // avoid 0xffffffff value, which is invalid
        }
    }
}
/* Csi2PlatformIncFramesCounter *************************/

/*================================================================================================*/
/*
 * @brief       The function return the current frames counter
 * @details     The application can read the counter of frames, to decide how to manage the data.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS)
 * @param[in] vcId      - VC number, RSDK_CSI2_VC_0 ... MAX
 *
 * @return uint32_t - the current frames counter, 0xffffffff means error
 *
 */
uint32_t Csi2PlatformGetFramesCounter(const rsdkCsi2UnitId_t unitId, const rsdkCsi2VirtChnlId_t vcId)
{
    uint32_t rez = 0xffffffffu;
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_FRM_CNTR, (uint32_t)CSI2_SEQ_BEGIN);

    if (((uint8_t)unitId < (uint8_t)RSDK_CSI2_MAX_UNITS) && ((uint8_t)vcId < (uint8_t)RSDK_CSI2_MAX_VC))
    {
        rez = gsCsi2FramesCounter[(uint8_t)unitId][(uint8_t)vcId];
    }
    RsdkTraceLogEvent(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_FRM_CNTR, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2PlatformGetFramesCounter *************************/

#ifdef S32R294
// Get the error for the expected On state for the unit driver
static rsdkStatus_t Csi2GetOnStatusMismatch(const rsdkCsi2UnitId_t unitId);

/*================================================================================================*/
/*
 * @brief       Procedure to get the error when the expected driver state is ON.
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \ref RSDK_CSI2_UNIT_1 , \ref RSDK_CSI2_MAX_UNITS )
 *
 * @return      RSDK_SUCCESS - if driver status is correct
 *              appropriate error if state is not ON
 *
 */
static rsdkStatus_t Csi2GetOnStatusMismatch(const rsdkCsi2UnitId_t unitId)
{
    rsdkStatus_t rez;

    switch (gCsi2Settings[(uint8_t)unitId].driverState)
    {
        case CSI2_DRIVER_STATE_OFF:
            rez = RSDK_CSI2_DRV_POWERED_OFF;
            break;
        case CSI2_DRIVER_STATE_STOP:
            rez = RSDK_CSI2_DRV_RX_STOPPED;
            break;
        case CSI2_DRIVER_STATE_NOT_INITIALIZED:
            rez = RSDK_CSI2_DRV_NOT_INIT;
            break;
        default:
            rez = RSDK_SUCCESS;
            break;
    }
    return rez;
}
/* Csi2GetOnStatusMismatch *************************/

/*================================================================================================*/
/*
 * @brief       Procedure to get the buffer start for the next frame.
 * @details     The procedure returns the offset from the buffer start
 *                  where the first byte of the frame will be written.
 *              The procedure must be called after the previous frame was received,
 *                  but before the start of the expected frame.
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \ref RSDK_CSI2_UNIT_1 , \ref RSDK_CSI2_MAX_UNITS )
 * @param[in] vcId      - VC ID &isin; [ \ref RSDK_CSI2_VC_0 , \ref RSDK_CSI2_MAX_VC )
 * @param[in] pOffset   - pointer to a uint32_t which will receive the real first byte offset
 *
 * @return      RSDK_SUCCESS - if driver status is correct; the offset will be passed
 *              error if the driver is in an inappropriate state; pOffset is not updated
 *
 */
rsdkStatus_t Csi2GetFirstByteOffset(const rsdkCsi2UnitId_t unitId, const rsdkCsi2VirtChnlId_t vcId, uint32_t *pOffset)
{
    rsdkStatus_t rez;
    uint32_t     line;

    if (pOffset == NULL)
    {
        rez = RSDK_CSI2_DRV_NULL_PARAM_PTR;
    }
    else
    {
        rez = Csi2GetFirstLinePos(unitId, vcId, &line);
        if (rez == RSDK_SUCCESS)
        {
            // if the result is successful, multiply the line with the line length
            *pOffset = line * gCsi2Settings[(uint8_t)unitId].workingParamVC[vcId].pVCParams->bufLineLen;
        }
    }
    return rez;
}
/* Csi2GetFirstByteOffset *************************/

/*================================================================================================*/
/*
 * @brief       Procedure to get the buffer start for the next frame.
 * @details     The procedure returns the buffer line
 *                  where the first line of the frame will be written.
 *              The procedure must be called after the previous frame was received,
 *                  but before the start of the expected frame.
 * @note        To get the exact address, must be used the buffer line length
 *                  declared in the unit initialization parameters (rsdkCsi2VCParams_t::bufLineLen).
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \ref RSDK_CSI2_UNIT_1 , \ref RSDK_CSI2_MAX_UNITS )
 * @param[in] vcId      - VC ID &isin; [ \ref RSDK_CSI2_VC_0 , \ref RSDK_CSI2_MAX_VC )
 * @param[in] pFirstLine - pointer to a uint32_t which will receive the real first line position
 *
 * @return      RSDK_SUCCESS - if driver status is correct; the line will be passed (counting from 0)
 *              error if the driver is in an inappropriate state; pFirstLine is not updated
 *
 */
rsdkStatus_t Csi2GetFirstLinePos(const rsdkCsi2UnitId_t unitId, const rsdkCsi2VirtChnlId_t vcId, uint32_t *pFirstLine)
{
    rsdkStatus_t rez;
    uint32_t     regStat;

    if ((uint8_t)unitId >= (uint8_t)RSDK_CSI2_MAX_UNITS)
    {
        rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
    }
    else if ((uint8_t)vcId >= (uint8_t)RSDK_CSI2_MAX_VC)
    {
        rez = RSDK_CSI2_DRV_INVALID_VC_PARAMS;  // Incorrect lane number.
    }
    else if (pFirstLine == NULL)
    {
        rez = RSDK_CSI2_DRV_NULL_PARAM_PTR;
    }
    else
    {
        rez = RSDK_SUCCESS;
    }
    if (rez == RSDK_SUCCESS)
    {
        rez = Csi2GetOnStatusMismatch(unitId);  // check the driver status, which must be ON
    }
    if (rez == RSDK_SUCCESS)
    {
        if (gCsi2Settings[unitId].workingParamVC[vcId].pVCParams == (void *)NULL)
        {
            rez = RSDK_CSI2_DRV_INVALID_VC_PARAMS;  // not initialized VC
        }
    }
    if (rez == RSDK_SUCCESS)
    {  // everything is ok, let's get the next line buffer position
        regStat = *(volatile uint32_t *)(volatile void *)(&gpMipiCsi2Regs[(uint8_t)unitId]->RX[vcId].CBUF_NXTLINE.R);
        if (regStat != 0u)
        {
            regStat--;
        }
        *pFirstLine = regStat;
    }
    return rez;
}
/* Csi2GetFirstLinePoz *************************/

#endif // #ifdef S32R294

/*================================================================================================*/

#ifdef __cplusplus
}
#endif

/******************************************************************************
 * EOF
 ******************************************************************************/
