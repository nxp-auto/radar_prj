/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**
*   @file
*   @implements Csi2.c_Artifact
*
*   @addtogroup CSI2
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*
* @page misra_violations MISRA-C:2012 violations
*
* @section Csi2_c_REF_1
* Violates MISRA 2012 Advisory Directive 4.9,
*   #A function should be used in preference to a function-like macro where they are interchangeable
* Using a macro produce a smaller code; there is a clear distinction between functions and macros,
* which use only UPPERCASE
*
* @section Csi2_c_REF_2
* Violates MISRA 2012 Required Directive 4.10,
*   #Precautions shall be taken in order to prevent the contents of a header file being included more than once.
* This violation is not fixed since the inclusion of <MA>_MemMap.h is as per AUTOSAR requirement [SWS_MemMap_00003].
*
* @section Csi2_c_REF_3
* Violates MISRA C-2012 Advisory Rule 8.9,
*   #An object should be defined at block scope if its identifier only appears in a single function.
* To not mix code and data storage, for proper memory mapping,
* some of the static variables are not defined at block scope.
*
* @section Csi2_c_REF_4
* Violates MISRA C-2012 Advisory Rule 11.4,
*   #A conversion should not be performed between a pointer to object and an integer type
* Some initialization need to be done for pointers.
*
* @section Csi2_c_REF_5
* Violates MISRA C-2012 Required Rule 11.6,
*   #A cast shall not be performed between pointer to void and an arithmetic type
* In some contexts, is necessary to process pointers using arithmetic operations.
*
* @section Csi2_c_REF_6
* Violates MISRA C-2012 Required Rule 18.1,
*   #A pointer resulting from arithmetic on a pointer operand shall address an element of the same array
*   as that pointer operand
* Some initialization need to be done for pointers.
*
* @section Csi2_c_REF_7
* Violates MISRA C-2012 Required Rule 18.4,
*   #The +, -, += and -= operators should not be applied to an  expression of pointer type
* Necessary pointer operation, not possible to use normal pointer association.
*
* @section Csi2_c_REF_8
* Violates MISRA 2012 Advisory Rule 20.1,
*   #Include directives should only be preceded by preprocessor directives or comments.
* <MA>_MemMap.h is included after each section define in order to set the current memory section as defined by AUTOSAR.
*
*/

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "CDD_Csi2.h"
#include "rsdk_status.h"
#include "rsdk_status_helper.h"

    #include "S32R45_MIPICSI2.h"
    #include "S32R45_DFS.h"


/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/
/* error report management      */
#if !defined(CSI2_REPORT_ERROR)
        #define CSI2_REPORT_ERROR(a,b,c)  (a)
#endif

/* upper and lower alignment of data to specified value                                                     */
#define CSI2_UPPER_ALIGN_TO_(x, y)          ((((x) + (y) - 1u) / (y)) * (y))

/* definitions for DPHY registry, not existing in the NXP header file,
 * but defined in the MIPI-CSI2 errata for manual calibration                                               */
#define DPHY_CLKCALVAL_COMPS_OFFSET             0x09a0U /* offset toDPHY_CLKCALVAL_COMPS registry           */
#define DPHY_DATALOFFSETCAL_VALUE0_OFFSET       0x0ba5U /* offset to DPHY_DATALOFFSETCAL_VALUE0 registry    */
#define DPHY_DATALOFFSETCAL_VALUE1_OFFSET       0x0da5U /* offset to DPHY_DATALOFFSETCAL_VALUE1 registry    */
#define DPHY_DATALOFFSETCAL_VALUE2_OFFSET       0x0fa5U /* offset to DPHY_DATALOFFSETCAL_VALUE2 registry    */
#define DPHY_DATALOFFSETCAL_VALUE3_OFFSET       0x11a5U /* offset to DPHY_DATALOFFSETCAL_VALUE3 registry    */
#define DPHY_TX_TERM_CAL_0_OFFSET               0x1520U /* offset to DPHY_TX_TERM_CAL_0 registry            */
#define DPHY_TX_RDWR_TERM_CAL_0_OFFSET          0x080dU /* offset to DPHY_TX_RDWR_TERM_CAL_0 registry       */
#define DPHY_TX_RDWR_TERM_CAL_1_OFFSET          0x080eU /* offset to DPHY_TX_RDWR_TERM_CAL_1 registry       */
#define DPHY_CLKOFFSETCAL_OVRRIDE_OFFSET        0x097fU /* offset to DPHY_CLKOFFSETCAL_OVRRIDE registry     */
#define DPHY_CLKOFFSETCAL_OVRRIDEVAL_OFFSET     0x0980U /* offset to DPHY_CLKOFFSETCAL_OVRRIDEVAL registry  */
#define DPHY_DATALOFFSETCAL_OVRVALUE0_OFFSET    0x0b80U /* offset to DPHY_DATALOFFSETCAL_OVRVALUE0 registry */
#define DPHY_DATALOFFSETCAL_OVRVALUE1_OFFSET    0x0d80U /* offset to DPHY_DATALOFFSETCAL_OVRVALUE1 registry */
#define DPHY_DATALOFFSETCAL_OVRVALUE2_OFFSET    0x0f80U /* offset to DPHY_DATALOFFSETCAL_OVRVALUE2 registry */
#define DPHY_DATALOFFSETCAL_OVRVALUE3_OFFSET    0x1180U /* offset to DPHY_DATALOFFSETCAL_OVRVALUE3 registry */
#define DPHY_DATAL0OFFSETCAL_OVRCNTRL_OFFSET    0x0b7fU /* offset to DPHY_DATAL0OFFSETCAL_OVRCNTRL registry */
#define DPHY_DATAL1OFFSETCAL_OVRCNTRL_OFFSET    0x0d7fU /* offset to DPHY_DATAL1OFFSETCAL_OVRCNTRL registry */
#define DPHY_DATAL2OFFSETCAL_OVRCNTRL_OFFSET    0x0f7fU /* offset to DPHY_DATAL2OFFSETCAL_OVRCNTRL registry */
#define DPHY_DATAL3OFFSETCAL_OVRCNTRL_OFFSET    0x117fU /* offset to DPHY_DATAL3OFFSETCAL_OVRCNTRL registry */
#define DPHY_RX_STARTUP_OVERRIDE_OFFSET         0x06e4U /* offset to DPHY_RX_STARTUP_OVERRIDE registry      */

#define CSI2_SOFTRESET_BIT              0x80000000UL    /* the 32 bits value to reset all registry          */
#define CSI2_FLUSH_CNT_FREQ_LIMIT               200U    /* the max value for flash_cnt limit (200MHz)       */
#define CSI2_CHIRP_NOT_STARTED                  0U      /* value for first chirp line received              */
#define CSI2_VC_CFG_OFFSET                      0x04U   /* offset between two similar VC config registry    */
#define CSI2_MAX_WAIT_FOR_STOP                  1200    /* maximum time to wait for stop state [us]         */
#define CSI2_RXEN_RXEN_DISABLED                 0UL     /* MIPICSI2_RXEN register, RXEN field, Rx disabled. */
#define CSI2_RXEN_RXEN_ENABLED                  1UL     /* MIPICSI2_RXEN register, RXEN field, Rx enabled.  */

#define CSI2_LAN_CS_MARK            (1UL << 5U) /* MIPICSI2_LANxCS register, DxULMA field, Data lane in Mark state. */
#define CSI2_LAN_CS_ULPA            (1UL << 4U) /* MIPICSI2_LANxCS register, DxULPA field, Data lane ULPS active.   */
#define CSI2_LAN_CS_STOP            (1UL << 3U) /* MIPICSI2_LANxCS register, DxSTOP field, Data lane in stop state.
                                                 *  data reception ongoing.                                         */
#define CSI2_LAN_CS_RXACTH          (1UL << 1U) /* MIPICSI2_LANxCS register, RXACTH field, High Speed data reception on.
                                                 *  data being driven.                                              */
#define CSI2_LAN_CS_RXVALH          (1UL << 0U) /* MIPICSI2_LANxCS register, RXVALH field, Valid High Speed data on.*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/* copies of the Virtual Channel data parameters                                */
/*
* @section Csi2_c_REF_3
* Violates MISRA C-2012 Advisory Rule 8.9,
* #An object should be defined at block scope if its identifier only appears in a single function.
* To not mix code and data storage, for proper memory mapping,
* some of the static variables are not defined at block scope.
*/
    static Csi2_VCParamsType gsCsi2VCParamCopy[CSI2_MAX_UNITS][CSI2_MAX_VC];


#if (CSI2_POWER_ON_OFF_USAGE == STD_ON)

    /* Copy for CSI2 actual params, to use if reset needed.                         */
    static Csi2_SetupParamsType gsCsi2UnitParamCopy[CSI2_MAX_UNITS];

    #if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
        /* copies of the auxiliary data parameters                                  */
        /*
        * @section Csi2_c_REF_3
        * Violates MISRA C-2012 Advisory Rule 8.9,
        * #An object should be defined at block scope if its identifier only appears in a single function.
        * To not mix code and data storage, for proper memory mapping,
        * some of the static variables are not defined at block scope.
        */
        static Csi2_VCParamsType gsCsi2AuxParamCopy[CSI2_MAX_UNITS][CSI2_MAX_VC];
    #endif /* (CSI2_AUXILIARY_DATA_USAGE == STD_ON)                                 */

#if (CSI2_METADATA_DATA_USAGE == STD_ON)
    /* copies of the metadata parameters                                  */
    /*
    * @section Csi2_c_REF_3
    * Violates MISRA C-2012 Advisory Rule 8.9,
    * #An object should be defined at block scope if its identifier only appears in a single function.
    * To not mix code and data storage, for proper memory mapping,
    * some of the static variables are not defined at block scope.
    */
    static Csi2_MetaDataParamsType gsCsi2MdParamCopy[CSI2_MAX_UNITS][CSI2_MAX_VC];
#endif /* (CSI2_AUXILIARY_DATA_USAGE == STD_ON)                                 */

#endif /* (CSI2_POWER_ON_OFF_USAGE == STD_ON)                                       */

#if (CSI2_DEV_ERROR_DETECT == STD_ON)

#endif  /* #if (CSI2_DEV_ERROR_DETECT == STD_ON)                    */

    #if (CSI2_DFS_USAGE == CSI2_DFS_ONCE)
        static uint8_t gsCsi2DfsReset = 0u;         /* variable to manage the DFS usage         */
    #endif


/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

#if (CSI2_FRAMES_COUNTER_USED == STD_ON)
    /* Counter for received frames.                                             */
        volatile uint32 gsCsi2FramesCounter[CSI2_MAX_UNITS][CSI2_MAX_VC];
#endif /* (CSI2_FRAMES_COUNTER_USED == STD_ON)                                  */

/* Pointer to MIPICSI2 memory map                                               */
        volatile GENERIC_CSI2_Type *gMipiCsi2RegsPtr[CSI2_MAX_UNITS];


    /* settings to be kept during the execution
     * only run-time necessary parameters are kept                                  */
    Csi2_DriverParamsType gCsi2Settings[CSI2_MAX_UNITS] = {{
            .driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED,
            .pCallback = { NULL_PTR },
    }};

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

#if (CSI2_DEV_ERROR_DETECT == STD_ON)
static Std_ReturnType Csi2_CheckBaseParams(const Csi2_UnitIdType unitId, const Csi2_SetupParamsType *pCsi2SetupParam);
static Std_ReturnType Csi2_CheckVCParam(const Csi2_VCParamsType *pVC, uint8 *pAutoStat);
static Std_ReturnType Csi2_CheckSetupParams(const Csi2_UnitIdType      unitId,
                                                const Csi2_SetupParamsType *pCsi2SetupParam);
#endif

static void HaltOnError(void);

    /* DFS reset                                */
    #if (CSI2_DFS_USAGE == CSI2_DFS_NOT_USED)
        #warning CSI2_DFS_NOT_USED defined, so DFS will not be used
    #elif (CSI2_DFS_USAGE == CSI2_DFS_ONCE)
        #warning CSI2_DFS_ONCE defined, so DFS will be reset only at the first init
        static void DfsTilt(void);
    #elif (CSI2_DFS_USAGE == CSI2_DFS_ALWAYS)
        #warning CSI2_DFS_ALWAYS defined, so DFS will be reset at every init
        static void DfsTilt(void);
    #else
        #error Not defined DFS usage, choose one of : #>CSI2_DFS_NOT_USED, #>CSI2_DFS_ONCE, #>CSI2_DFS_ALWAYS
    #endif


/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/
/*================================================================================================*/
/*
 * Function to stop the execution thread
 */
static void HaltOnError(void)
{
    /* Loop exit signal.                                             */
    static volatile boolean gsCsi2LoopExit = FALSE;

    while(gsCsi2LoopExit != TRUE)
    {
        ;   /* empty loop */
    }
    gsCsi2LoopExit = FALSE;
}

#if (CSI2_DEV_ERROR_DETECT == STD_ON)
/*================================================================================================*/
/*
 * @brief       The function check the base CSI2 specific parameters
 *
 * @param[in]   unitId          - the unit id
 * @param[in]   pCsi2SetupParam - pointer to the CSI2 setup structure
 *
 * @return Std_ReturnType       - success or error
 *
 */
static Std_ReturnType Csi2_CheckBaseParams(const Csi2_UnitIdType unitId, const Csi2_SetupParamsType *pCsi2SetupParam)
{
    uint32          i;
    Std_ReturnType  rez;

    if ((uint8)unitId >= (uint8)CSI2_MAX_UNITS)

    {   /* the required unit is bigger than the platform general specs              */
        rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_UNIT_ID, (uint8)CSI2_API_ID_SETUP, (uint8)CSI2_E_DRV_WRG_UNIT_ID);
        CSI2_HALT_ON_ERROR;
    }
    else
    {
        if (pCsi2SetupParam == (Csi2_SetupParamsType*)NULL_PTR)
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_NULL_PARAM_PTR, (uint8)CSI2_API_ID_SETUP,
                                                            (uint8)CSI2_E_DRV_NULL_PARAM_PTR);
            CSI2_HALT_ON_ERROR;
        }
        else if (pCsi2SetupParam->numLanesRx >= (uint8)CSI2_MAX_LANE)
        {  /* wrong number of lanes         */
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_LANES_NR, (uint8)CSI2_API_ID_SETUP,
                                                            (uint8)CSI2_E_DRV_INVALID_LANES_NR);
            CSI2_HALT_ON_ERROR;
        }
        else
        {
            rez = (Std_ReturnType)E_OK;
        }
    }
    if(rez == (Std_ReturnType)E_OK)
    {
        if((uint8)pCsi2SetupParam->initOptions >= (uint8)CSI2_DPHY_INIT_MAX)
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_INIT_PARAMS, (uint8)CSI2_API_ID_SETUP,
                                                            (uint8)CSI2_E_DRV_INVALID_INIT_PARAMS);
            CSI2_HALT_ON_ERROR;
        }
    }
    if(rez == (Std_ReturnType)E_OK)
    {
        for (i = 0; i < (uint32)CSI2_MAX_VC; i++)
        {
            if (pCsi2SetupParam->vcConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR)
            {
                break;
            }
        }
        if (i >= (uint32)CSI2_MAX_VC)
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_VC_PARAMS, (uint8)CSI2_API_ID_SETUP,
                                                            (uint8)CSI2_E_DRV_INVALID_VC_PARAMS);
            CSI2_HALT_ON_ERROR;
        }
    }
    return rez;
}
/* Csi2_CheckBaseParams *************************/

#if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
/*================================================================================================*/
/*
 * @brief   The function check the correctness of auxiliary data setup.
 *          It checks only the match between streamDataType and outputDataMode,
 *          but assumes CSI2_VC_BUF_FIFTH_CH_ON bit is set.
 *
 * @param[in] pVC - pointer to the virtual channel setup structure
 *
 * @return Std_ReturnType   - success or error
 *
 */
static Std_ReturnType Csi2_CheckAuxiliaryMode(const Csi2_VCParamsType *pVC)
{
    Std_ReturnType  rez;
    uint32          streamType;
    uint16          outputType;

    rez = (Std_ReturnType)E_OK;
    streamType = ((uint32)pVC->streamDataType) & (~CSI2_NORM_DTYPE_MASK);
    if (streamType != 0u)
    {
        /* it is  auxiliary data type       */
        outputType = (uint16)(pVC->outputDataMode & (uint16)CSI2_VC_BUF_5TH_CH_MODE_1);
        if (((streamType == (uint32)CSI2_DATA_TYPE_AUX_0_NO_DROP) &&
             (outputType != (uint32)CSI2_VC_BUF_5TH_CH_M0_NODROP)) ||
            ((streamType == (uint32)CSI2_DATA_TYPE_AUX_0_DR_1OF2) &&
             (outputType != (uint32)CSI2_VC_BUF_5TH_CH_M0_1_OF_2)) ||
            ((streamType == (uint32)CSI2_DATA_TYPE_AUX_0_DR_3OF4) &&
             (outputType != (uint32)CSI2_VC_BUF_5TH_CH_M0_3_OF_4)) ||
            ((streamType == (uint32)CSI2_DATA_TYPE_AUX_1_NO_DROP) &&
             (outputType != (uint32)CSI2_VC_BUF_5TH_CH_MODE_1)) ||
             (streamType > (uint32)CSI2_DATA_TYPE_MAX))
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_DATA_TYPE, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_INVALID_DATA_TYPE);
            CSI2_HALT_ON_ERROR;
        }
    }
    return rez;
}
/* Csi2CheckAuxiliaryMode *************************/
#endif

/*================================================================================================*/
/*
 * @brief       The function check the input parameters for CSI2 setup.
 * @details     All parameters for CSI2 are checked and the appropriate status is returned.
 *              The extended error code is set on Det.
 *
 * @param[in]   unitId         - the unit id
 * @param[in]   pCsi2SetupParam - pointer to CSI2 setup parameters
 *
 * @return Std_ReturnType - success or error
 *
 */
static Std_ReturnType Csi2_CheckSetupParams(const Csi2_UnitIdType      unitId,
                                                const Csi2_SetupParamsType *pCsi2SetupParam)
{
    Std_ReturnType  rez;
    uint8           ref, mask, t, autoDC;           /* variables to be used for tests   */
    uint32          i;

    rez = Csi2_CheckBaseParams(unitId, pCsi2SetupParam);
    if (rez == (Std_ReturnType)E_OK)
    {       /* lanes checked an ok      */
        /* checking the receive lanes configuration                                     */
        ref = 0;
        for (i = (uint32)CSI2_LANE_0; i <= pCsi2SetupParam->numLanesRx; i++)
        {
            ref |= ((uint8)1 << (uint8)pCsi2SetupParam->lanesMapRx[i]);
        } /* for    */
        t = 0xffu;                                  /* total good channels              */
        mask = 1;                                   /* test mask                        */
        for (i = (uint32)CSI2_LANE_0; i < (uint32)CSI2_MAX_LANE; i++)
        {
            if ((ref & mask) != 0u)
            {
                t++;
            }
            mask <<= (uint8)1;
        }

        if (t != pCsi2SetupParam->numLanesRx)
        {       /* wrong configuration for receiving lanes swap                         */
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_RX_SWAP, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_INVALID_RX_SWAP);
            CSI2_HALT_ON_ERROR;
        }
    }
    if (rez == (Std_ReturnType)E_OK)
    {       /* good configuration for lanes swap                                        */
        if (((pCsi2SetupParam->rxClkFreq < (uint32)CSI2_MIN_RX_FREQ) ||
            (pCsi2SetupParam->rxClkFreq > (uint32)CSI2_MAX_RX_FREQ))
                )
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_CLOCK_FREQ, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_INVALID_CLOCK_FREQ);
            CSI2_HALT_ON_ERROR;
        }
        else
        {
            /* checking the VCs and the callbacks (to be careful at index for both)     */
            autoDC = 0;
            for (i = 0; i < (uint32)CSI2_MAX_VC; i++)
            {
                rez = Csi2_CheckVCParam(pCsi2SetupParam->vcConfigPtr[i], &autoDC);
#if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
                if (rez == (Std_ReturnType)E_OK)
                {  /* check for auxiliary data only if successful before                */
                    rez = Csi2_CheckVCParam(pCsi2SetupParam->auxConfigPtr[i], &autoDC);
                }
                if ((rez == (Std_ReturnType)E_OK) &&
                    (((pCsi2SetupParam->vcConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR) &&
                      (pCsi2SetupParam->vcConfigPtr[i]->streamDataType > (uint16)CSI2_DATA_TYPE_USR7)) ||
                     ((pCsi2SetupParam->auxConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR) &&
                      (pCsi2SetupParam->auxConfigPtr[i]->streamDataType < (uint16)CSI2_DATA_TYPE_AUX_0_NO_DROP))))
                {       /* check for correct stream data type on normal/auxiliary flow  */
                    rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_DATA_TYPE, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_INVALID_DATA_TYPE);
                    CSI2_HALT_ON_ERROR;
                }
                if ((pCsi2SetupParam->vcConfigPtr[i] == (Csi2_VCParamsType*)NULL_PTR) &&
                        (pCsi2SetupParam->auxConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR))
                {
                    rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_VC_AUX_MISMATCH, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_VC_AUX_MISMATCH);
                    CSI2_HALT_ON_ERROR;
                }
#endif
                if (rez != (Std_ReturnType)E_OK)
                {
                    break;
                }
            }  /* for   */
            if(pCsi2SetupParam->pCallback[(uint8)RSDK_CSI2_RX_ERR_IRQ_ID] == (Csi2_IsrCbType)NULL_PTR)
            {
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_NULL_ERR_CB_PTR, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_NULL_ERR_CB_PTR);
                CSI2_HALT_ON_ERROR;
            }
#if !defined(linux)
            else
            {
                if(pCsi2SetupParam->irqExecCore != RSDK_CURRENT_CORE)
                {
                    rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_ERR_INVALID_CORE_NR, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_ERR_INVALID_CORE_NR);
                    CSI2_HALT_ON_ERROR;
                }
            }
#endif
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
            if (rez == (Std_ReturnType)E_OK)
            {
                /* check for autoDC status                                              */
                if ((autoDC != 0u) && ((pCsi2SetupParam->statManagement == (uint8)CSI2_AUTODC_NO) ||
                                       (pCsi2SetupParam->statManagement >= (uint8)CSI2_AUTODC_MAX)))
                {
                    rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_DC_PARAMS, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_INVALID_DC_PARAMS);
                    CSI2_HALT_ON_ERROR;
                }
            }
#endif
        } /* if((pStart->rxClkFreq < CSI2_MIN_RX_CLK_FREQ) ||...  */
    } /* if (rez == (Std_ReturnType)E_OK)     */
    return rez;
}
/* Csi2_CheckSetupParams *************************/


/*================================================================================================*/
/*
 * @brief       The function check the CSI2 virtual channel setup parameters for correctness.
 *
 * @param[in] pVC - pointer to the virtual channel setup structure
 *
 * @return Std_ReturnType   - success or error
 *
 */
static Std_ReturnType Csi2_CheckVCParam(const Csi2_VCParamsType *pVC, uint8 *pAutoStat)
{
    Std_ReturnType  rez;
    uintptr_t       addrAlign;
    uint32          bufLen;
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
    uint8           i;
#endif

    rez = (Std_ReturnType)E_OK;                 /* default SUCCESS                  */
    if (pVC != (Csi2_VCParamsType*)NULL_PTR)
    {       /* check only for valid/defined VC  */
        bufLen = Csi2_GetBufferRealLineLen((Csi2_DataStreamType)pVC->streamDataType, pVC->channelsNum,
                pVC->expectedNumSamples
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
                , CSI2_STAT_NO
#endif
                );
        if ((bufLen == 0u) && ((pVC->streamDataType * pVC->expectedNumSamples) != 0u))
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_DATA_TYPE, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_INVALID_DATA_TYPE);
            CSI2_HALT_ON_ERROR;
        }
        else
        {
            /*
            * @section Csi2_c_REF_5
            * Violates MISRA C-2012 Required Rule 11.6,
            * #A cast shall not be performed between pointer to void and an arithmetic type
            * In some contexts, is necessary to process pointers using arithmetic operations.
            */
            addrAlign = (uintptr_t)(pVC->bufDataPtr);
            addrAlign &= 0xfu;
#if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
            if (((uint32)pVC->outputDataMode & (uint32)CSI2_VC_BUF_FIFTH_CH_ON) != 0u)
            {       /* check for correct auxiliary data setup, rez can be SUCCESS here      */
                rez = Csi2_CheckAuxiliaryMode(pVC);
            }
#endif
            if (    (pVC->expectedNumSamples == 0u)                             ||
                    (pVC->expectedNumLines == 0u)                               ||
                    (pVC->bufLineLen == 0u)                                     ||
                    (pVC->bufNumLines < (uint16)CSI2_MIN_VC_BUF_NR_LINES)       ||
                    (((pVC->outputDataMode & (uint16)CSI2_VC_BUF_COMPLEX_DATA) == (uint16)CSI2_VC_BUF_REAL_DATA ) &&
                            (pVC->channelsNum > (uint8)CSI2_MAX_ANTENNA_NR))    ||
                    (((pVC->outputDataMode & (uint16)CSI2_VC_BUF_COMPLEX_DATA) == (uint16)CSI2_VC_BUF_COMPLEX_DATA) &&
                            (pVC->channelsNum > ((uint8)CSI2_MAX_ANTENNA_NR * (uint8)2u)))
                            )
            {
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_VC_PARAMS, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_INVALID_VC_PARAMS);
                CSI2_HALT_ON_ERROR;
            }
            if (    ((pVC->bufLineLen & 0xfu) != (uint16)0)         ||
                    (addrAlign != (uintptr_t)0u)                      ||
                    (   (((uint8)pVC->vcEventsReq & (uint8)CSI2_EVT_LINE_END) != 0u) &&
                        (pVC->bufNumLinesTrigger == 0u) )   )
            {
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_BUF_PTR_NOT_ALIGNED, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_BUF_PTR_NOT_ALIGNED);
                CSI2_HALT_ON_ERROR;
            }
            if (pVC->bufDataPtr == (void*)NULL_PTR)
            {
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_BUF_PTR_NULL, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_BUF_PTR_NULL);
                CSI2_HALT_ON_ERROR;
            }

#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
            for (i = 0; i < pVC->channelsNum; i++)
            {       /* check for all channels       */
                if ((pVC->offsetCompReal[i] == CSI2_OFFSET_AUTOCOMPUTE) ||
                        ((((uint32)pVC->outputDataMode & (uint32)CSI2_VC_BUF_COMPLEX_DATA) != 0u) &&
                                            (pVC->offsetCompImg[i] == CSI2_OFFSET_AUTOCOMPUTE)))
                {
                    *pAutoStat |= 1u;
                    break;
                }
            }
#endif
        }  /* if (bufLen == 0u)     */
    }      /* if (pVC != NULL_PTR)  */
    return rez;
}
/* Csi2_CheckVCParam *************************/
#endif /* (CSI2_DEV_ERROR_DETECT == STD_ON) */


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
static uint32 Csi2_GetOutBytesPerSample(const Csi2_DataStreamType dataType)
{
    uint32  rez;

    /* get the conversion rate input data -> memory data, per sample        */
    switch ((uint16)dataType & CSI2_NORM_DTYPE_MASK)
    {
        case (uint16)CSI2_DATA_TYPE_EMBD:
        case (uint16)CSI2_DATA_TYPE_USR0:
        case (uint16)CSI2_DATA_TYPE_USR1:
        case (uint16)CSI2_DATA_TYPE_USR2:
        case (uint16)CSI2_DATA_TYPE_USR3:
        case (uint16)CSI2_DATA_TYPE_USR4:
        case (uint16)CSI2_DATA_TYPE_USR5:
        case (uint16)CSI2_DATA_TYPE_USR7:
            rez = 1u;  /* 1 byte per sample                                 */
            break;
        case (uint16)CSI2_DATA_TYPE_RAW8:
        case (uint16)CSI2_DATA_TYPE_RAW10:
        case (uint16)CSI2_DATA_TYPE_RAW12:
        case (uint16)CSI2_DATA_TYPE_RAW14:
        case (uint16)CSI2_DATA_TYPE_RAW16:
        case (uint16)CSI2_DATA_TYPE_YUV422_8:
        case (uint16)CSI2_DATA_TYPE_16_FROM_8:
            rez = 2u;  /* 2 bytes per sample                                */
            break;
        case (uint16)CSI2_DATA_TYPE_RGB888:
        case (uint16)CSI2_DATA_TYPE_YUV422_10:
        case (uint16)CSI2_DATA_TYPE_RGB565:
            rez = 3u;  /* 3 bytes per sample                                */
            break;
        default:
            rez = 0u;  /* unknown/unacceptable data type                    */
            break;
    }
    return rez;
}
/* Csi2_GetOutBytesPerSample *************************/

/*================================================================================================*/
/*
 * @brief           Function which return the appropriate CSI2 memory map pointer.
 * @details         The function return the pointer to CSI2 registry.
 *
 * @return Std_ReturnType   - success or error
 *
 */
static Std_ReturnType Csi2_RegsMap(const Csi2_UnitIdType unitId, volatile GENERIC_CSI2_Type **pRegs)
{
    Std_ReturnType rez = (Std_ReturnType)E_OK;
#if defined(linux)
        *pRegs = (volatile GENERIC_CSI2_Type*)gpRsdkCsi2Device[unitId]->pMemMapVirtAddr;
#else
        switch (unitId)
        {
            case CSI2_UNIT_0:
                /*
                * @section Csi2_c_REF_4
                * Violates MISRA C-2012 Advisory Rule 11.4,
                * #A conversion should not be performed between a pointer to object and an integer type
                * Some initialization need to be done for pointers.
                */
                *pRegs = (volatile GENERIC_CSI2_Type *)IP_MIPICSI2_0;
                break;
            case CSI2_UNIT_1:
                /*
                * @section Csi2_c_REF_4
                * Violates MISRA C-2012 Advisory Rule 11.4,
                * #A conversion should not be performed between a pointer to object and an integer type
                * Some initialization need to be done for pointers.
                */
                *pRegs = (volatile GENERIC_CSI2_Type *)IP_MIPICSI2_1;
                break;
            case CSI2_UNIT_2:
                /*
                * @section Csi2_c_REF_4
                * Violates MISRA C-2012 Advisory Rule 11.4,
                * #A conversion should not be performed between a pointer to object and an integer type
                * Some initialization need to be done for pointers.
                */
                *pRegs = (volatile GENERIC_CSI2_Type *)IP_MIPICSI2_2;
                break;
            case CSI2_UNIT_3:
                /*
                * @section Csi2_c_REF_4
                * Violates MISRA C-2012 Advisory Rule 11.4,
                * #A conversion should not be performed between a pointer to object and an integer type
                * Some initialization need to be done for pointers.
                */
                *pRegs = (volatile GENERIC_CSI2_Type *)IP_MIPICSI2_3;
                break;
            default:
                rez = RSDK_CSI2_DRV_WRG_UNIT_ID;
                break;
        }  /* switch    */
#endif  /* #if defined(linux)   */
    return rez;
}
/* Csi2_RegsMap *************************/

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
 * @return      void
 *
 */
static void Csi2_GetOperatingSpeedMask(const uint16 speed, uint8 *pHSFREQRNG, uint16 *pDDLOSCFREQ)
{
    /*
     * @brief       Various tables to get correct speed masks.
     *
     */
    /* programmable operating speed in Mbps, according to RM, Rev.1 Draft I, pag. 6225-6227     */
    static const uint16 gsCsi2OperatingSpeed[] = {
        80,   90,   100,  110,  120,  130,  140,  150,  160,  170,  180,  190,  205,  220,  235,  250,
        275,  300,  325,  350,  400,  450,  500,  550,  600,  650,  700,  750,  800,  850,  900,  950,
        1000, 1050, 1100, 1150, 1200, 1250, 1300, 1350, 1400, 1450, 1500, 1550, 1600, 1650, 1700, 1750,
        1800, 1850, 1900, 1950, 2000, 2050, 2100, 2150, 2200, 2250, 2300, 2350, 2400, 2450, 2500, 0};
    /* HsFreqRange values table, correlated to Csi2MOperatingSpeed                              */
    static const uint8 gsCsi2OperatingSMask[] = {0,  16, 32, 48, 1,  17, 33, 49, 2,  18, 34, 50, 3,  19, 35, 51,
                                                   4,  20, 37, 53, 37, 22, 38, 55, 7,  24, 40, 57, 9,  25, 41, 58,
                                                   10, 26, 42, 59, 11, 27, 43, 60, 12, 28, 44, 61, 13, 29, 46, 62,
                                                   14, 30, 47, 63, 15, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73

    };
    /* OsFreqTarget values table, correlated to Csi2MOperatingSpeed                             */
    static const uint16 gsCsi2OscFreqTarget[] = {
        489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489,
        489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489, 489,
        489, 303, 313, 323, 333, 342, 352, 362, 372, 381, 391, 401, 411, 420, 430, 440, 450, 459, 469, 479, 489};

    uint32 i;

    i = 0u;
    while (gsCsi2OperatingSpeed[i] != 0u)  /* 0 speed is the marker for the end                     */
    {
        if (speed <= gsCsi2OperatingSpeed[i])
        {
            if (i != 0u)
            {
                /* check for the closest speed value                                                */
                if ((speed - gsCsi2OperatingSpeed[i - 1u]) < (gsCsi2OperatingSpeed[i] - speed))
                {
                    i--;
                }
            }
            break;                          /* the current speed is the first bigger or equal       */
        }
        i++;
    }  /* while     */
    if (gsCsi2OperatingSpeed[i] == 0u)
    {
        i--;                                /* stopped on the final position, so back one step      */
    }
    *pHSFREQRNG = gsCsi2OperatingSMask[i];
    *pDDLOSCFREQ = gsCsi2OscFreqTarget[i];
}
/* Csi2_GetOperatingSpeedMask *************************/

#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
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
static uint8 Csi2_SetOffsetMan(Csi2_VCDriverStateType *pVCDrvState, const Csi2_VCParamsType *pVCparams)
{
    uint32 i;
    uint32 numChannel;
    uint8  statFlag;                            /* channel statistics required                      */
    uint8  mask;
    uint8  hasComplex;                          /* indicator for real/complex channels              */
    uint16 dcRef;                               /* DC reference                                     */

    statFlag = 0;                               /* statistics status (0 = not necessary)            */
    if (pVCparams != (Csi2_VCParamsType*)NULL_PTR)  /* only if the pointer is defined               */
    {
        mask = 1;                               /* mask for auto update for DC                      */
        numChannel = (uint32)Csi2_GetChannelNum(pVCDrvState);
        if (numChannel == pVCparams->channelsNum)
        {
            hasComplex = 0u;                    /* only real channels                               */
        }
        else
        {
            hasComplex = 1;                     /* complex channels                                 */
        }
        for (i = (uint32)CSI2_CHANNEL_A; i < numChannel; i++)
        {
            if (hasComplex != 0u)
            {       /* channels are complex     */
                if ((i & (uint32)1) == (uint32)0)
                {
                    dcRef = (uint16)pVCparams->offsetCompReal[i / 2u]; /* even channels are real                      */
                }
                else
                {
                    dcRef = (uint16)pVCparams->offsetCompImg[i / 2u];  /* odd channels are imaginary if((i & 1) == 0) */
                }
            }
            else
            {
                dcRef = (uint16)pVCparams->offsetCompReal[i];          /* only real channels if(hasComplex != 0)      */
            }
            if (dcRef == (uint16)CSI2_OFFSET_AUTOCOMPUTE)
            {
                statFlag |= 1u;                 /* need statistics                                  */
            }
            pVCDrvState->statDC[i].reqChannelDC = (sint16)dcRef;
            dcRef <<= 4u;
            pVCDrvState->statDC[i].channelDC = (sint16)dcRef;
            mask <<= 1u;
        }  /* for */
    }      /* if(pVCparams != NULL_PTR) */
    /* return a centralized situation for statistics    */
    return (statFlag);
}
/* Csi2_SetOffsetMan *************************/
#endif


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
static uint16 Csi2_GetDataTypeBits(const Csi2_DataStreamType dataType)
{
    uint16 rez;

    /* get the conversion rate input data -> memory data, per sample        */
    switch ((uint16)dataType & CSI2_NORM_DTYPE_MASK)
    {
        case (uint16)CSI2_DATA_TYPE_EMBD:
        case (uint16)CSI2_DATA_TYPE_USR0:
        case (uint16)CSI2_DATA_TYPE_USR1:
        case (uint16)CSI2_DATA_TYPE_USR2:
        case (uint16)CSI2_DATA_TYPE_USR3:
        case (uint16)CSI2_DATA_TYPE_USR4:
        case (uint16)CSI2_DATA_TYPE_USR5:
        case (uint16)CSI2_DATA_TYPE_USR7:
        case (uint16)CSI2_DATA_TYPE_RAW8:
            rez = 8;                        /* 8 bits per sample                */
            break;
        case (uint16)CSI2_DATA_TYPE_RAW10:
            rez = 10;                       /* 10 bits per sample               */
            break;
        case (uint16)CSI2_DATA_TYPE_RAW12:
            rez = 12;                       /* 12 bits per sample               */
            break;
        case (uint16)CSI2_DATA_TYPE_RAW14:
            rez = 14;                       /* 14 bits per sample               */
            break;
        case (uint16)CSI2_DATA_TYPE_RAW16:
        case (uint16)CSI2_DATA_TYPE_YUV422_8:
        case (uint16)CSI2_DATA_TYPE_RGB565:
        case (uint16)CSI2_DATA_TYPE_16_FROM_8:
            rez = 16;                       /* 2 bytes per sample               */
            break;
        case (uint16)CSI2_DATA_TYPE_RGB888:
            rez = 24;                       /* 24 bits per sample               */
            break;
        case (uint16)CSI2_DATA_TYPE_YUV422_10:
            rez = 20;                       /* 20 bits per sample               */
            break;
        default:
            rez = 0;                        /* unknown/unacceptable data type   */
            break;
    }
    return rez;
}
/* Csi2_GetDataTypeBits *************************/


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
static void Csi2_UnitConfigVCSpec(volatile GENERIC_CSI2_Type *pRegs, const Csi2_VCParamsType *pVCparams,
                                 uint32 bufNr, const uint16 rxBufChirpData, const uint16 rxBufDataLen,
                                 const uint16 rxBufLen)
{
    uint8               i;
    uint32              valDC, intDC, dataType;
    volatile uint32     *pBufRegistry;

    /* circular buffer config                                                                           */
    uint32              dataInput;

    /* buffer configuration                                                                             */
    dataType = ((uint32)pVCparams->streamDataType & CSI2_NORM_DTYPE_MASK);
    /* check for data translation                                                                       */
    if(dataType == (uint32)CSI2_DATA_TYPE_16_FROM_8)
    {
        dataType = (uint32)CSI2_DATA_TYPE_RAW8;      /* CSI2_DATA_TYPE_16_FROM_8 is only a translation type   */
    }
    dataInput = ((bufNr & CSI2_MAX_VC_MASK) << 8u) + (dataType << 2u);
    if ((((uint32)pVCparams->outputDataMode & (uint32)CSI2_VC_BUF_FIFTH_CH_ON) != 0u) && (bufNr >= (uint32)CSI2_MAX_VC))
    {       /* fifth channel on     */
        dataInput |= CSI2_5TH_CHANNEL_ON;
    }
    pRegs->RX[bufNr].CBUF_CONFIG = dataInput;

    /* buffer pointer                                                                                   */
    /*
    * @section Csi2_c_REF_5
    * Violates MISRA C-2012 Required Rule 11.6,
    * #A cast shall not be performed between pointer to void and an arithmetic type
    * In some contexts, is necessary to process pointers using arithmetic operations.
    */
    pRegs->RX[bufNr].CBUF_SRTPTR = (uint32)pVCparams->bufDataPtr;

    /* buffer number of lines                                                                           */
    pRegs->RX[bufNr].CBUF_NUMLINE = pVCparams->bufNumLines
            ;

    /* buffer line length                                                                               */
    pRegs->RX[bufNr].CBUF_BUFLEN = rxBufLen;

    /* input interface config                                                                           */
    pRegs->RX[bufNr].INPLINELEN_CONFIG = rxBufChirpData;

    /* number of chirps to be received                                                                  */
    pRegs->RX[bufNr].NUMLINES_CONFIG = pVCparams->expectedNumLines;

    /* buffer chirp data length                                                                         */
    pRegs->RX[bufNr].LINELEN_CONFIG = rxBufDataLen;

    /* line done trigger                                                                                */
    if (pVCparams->bufNumLinesTrigger != 0u)        /* LINEDONE trigger required by application         */
    {
        /* set the value for LINEDONE                                                                   */
        pRegs->RX[bufNr].CBUF_LPDI = (uint32)pVCparams->bufNumLinesTrigger;
    }

    /* get the mask for channel enablement and set the DC offset compensation                           */
    if (bufNr < (uint32)CSI2_MAX_VC)
    {
        switch(bufNr)
        {
            case 0u:
                pBufRegistry = (volatile uint32 *)(&pRegs->RX_CBUF0_CHNLOFFSET0_0);
                break;
            case 1u:
                pBufRegistry = (volatile uint32 *)(&pRegs->RX_CBUF1_CHNLOFFSET0_1);
                break;
            case 2u:
                pBufRegistry = (volatile uint32 *)(&pRegs->RX_CBUF2_CHNLOFFSET0_2);
                break;
            default:
                pBufRegistry = (volatile uint32 *)(&pRegs->RX_CBUF3_CHNLOFFSET0_3);
                break;
        }
        if (((uint16)pVCparams->outputDataMode & (uint16)CSI2_VC_BUF_COMPLEX_DATA) != (uint16)0)
        {       /* complex data, so double number of channels */
            for (i = 0; i < pVCparams->channelsNum; i++)  /* for each defined channel                   */
            {
                intDC = ((uint32)pVCparams->offsetCompReal[i] & (uint32)0xffffu);
                if (intDC == (uint32)CSI2_OFFSET_AUTOCOMPUTE)
                {       /* a kind of autocompute        */
                    valDC = 0u;
                }
                else
                {
                    valDC = intDC & 0xfffu;    /* keep the new offset value for first channel           */
                }   /* if(intDC == CSI2_OFFSET_AUTOCOMPUTE)     */
                intDC = (uint32)pVCparams->offsetCompImg[i];
                if (intDC != (uint32)CSI2_OFFSET_AUTOCOMPUTE)
                {
                    valDC |= (intDC << 16u);  /* keep the offset value, on highest bits                 */
                }   /* if(intDC == CSI2_OFFSET_AUTOCOMPUTE)     */
                /* set the offset compensations when the data is complex                                */
                *(pBufRegistry) = (valDC << 4u);    /* adjust the value from 12 bits to the real output */
                pBufRegistry++;
            }  /* for   */
        }
        else
        {
            /* set the offset compensations when the data is only real                                  */
            valDC = 0u;
            for (i = 0; i < pVCparams->channelsNum; i++)  /* for each defined channel                   */
            {
                intDC = ((uint32)pVCparams->offsetCompReal[i] & (uint32)0xffff);
                if (intDC != (uint32)CSI2_OFFSET_AUTOCOMPUTE)
                {
                    valDC |= intDC << (16u * (i & 1u)); /* keep the new offset value, at right position */
                }   /* if(intDC == (uint32)CSI2_OFFSET_AUTOCOMPUTE)   */
                *pBufRegistry = (valDC << 4u);          /* adjust from 12 bits                          */
                if ((i & 1u) != 0u)
                {
                    pBufRegistry++;                     /* after even index go to next value            */
                    valDC = 0u;
                }
            }  /* for   */
        }   /* if((pVCparams->outputDataMode & CSI2_VC_BUF_COMPLEX_DATA) != 0)  */
    }
}
/* Csi2_UnitConfigVCSpec *************************/


/*================================================================================================*/
/*
 * @brief       Procedure for general programming a VC.
 * @details     This procedure cofigure all "common" parameters of a VC.
 *              For specific VC initialization: Csi2_UnitConfigVCSpec
 *
 * @param[in]   csi2UnitNum - unit number, CSI2_UNIT_0 ... MAX
 * @param[in]   vcId        - VC number, CSI2_VC_0 ... MAX
 * @param[in]   pRegs       - unit registry pointer
 * @param[in]   pVCparams   - pointer to VC parameters
 *
 * @return      success or error
 *
 * @NOTE        Assuming any VC is receiving only one data_type.
 *
 */
static Std_ReturnType Csi2_ConfigVC(const Csi2_UnitIdType csi2UnitNum, const uint8 vcId,
                                 volatile GENERIC_CSI2_Type *pRegs, const Csi2_VCParamsType *pVCparams)
{
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
    uint32                  i;
#endif
    uint32                  val1, val2, cfg2;
    uint16                  lenChirpData, lenBufData;       /* data lengths                 */
    Std_ReturnType          rez;                            /* result                       */
    Csi2_VCDriverStateType  *pDriverStateVC = (Csi2_VCDriverStateType  *)(NULL_PTR);    /* pointer to VC driver state */
    Csi2_DriverParamsType   *pDriverParams;

    rez = (Std_ReturnType)E_OK;
    /* compute the necessary buffer space, according to chirp values and other requirements
     * chirp length in bytes                                                                                */
    lenChirpData = pVCparams->expectedNumSamples *
            Csi2_GetDataTypeBits((Csi2_DataStreamType)pVCparams->streamDataType) * (uint16)pVCparams->channelsNum /
            (uint16)8;
    /* buffer length in bytes, without statistics                                                           */
    lenBufData = (uint16)Csi2_GetBufferRealLineLen((Csi2_DataStreamType)pVCparams->streamDataType,
            pVCparams->channelsNum, pVCparams->expectedNumSamples
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
                                                            ,CSI2_STAT_NO
#endif
                                                            );
    if (((uint32)pVCparams->outputDataMode & (uint32)CSI2_VC_BUF_FIFTH_CH_ON) != 0u)
    {
        if ((((uint32)pVCparams->outputDataMode & ((uint32)CSI2_VC_BUF_5TH_CH_MODE_1)) ==
                (uint32)CSI2_VC_BUF_5TH_CH_MODE_1))
        {  /* correct chirp length for auxiliary mode_1     */
            lenChirpData = (lenChirpData * 3u) / 2u;
        }
        else
        {
            lenChirpData *= 2u;                 /* chirp length for all auxiliary data mode 0               */
        }
    }

    val1 = ((uint32)1u) << (vcId & CSI2_MAX_VC_MASK);
    val2 = ((uint32)CSI2_CBUF0_ENA_MASK) << vcId;
    pRegs->RX_VCENABLE |= val1 + val2;     /* enable the VC and the buffer                                 */
    if (vcId < (uint8)CSI2_MAX_VC)
    {
        pDriverParams = &gCsi2Settings[(uint8)csi2UnitNum];
        pDriverStateVC = &pDriverParams->workingParamVC[vcId];
        /*
        * @section Csi2_c_REF_1
        * Violates MISRA 2012 Advisory Directive 4.9,
        * #A function should be used in preference to a function-like macro where they are interchangeable
        * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
        * which use only UPPERCASE
        */
        lenBufData = CSI2_UPPER_ALIGN_TO_(lenBufData, 16u);     /* align the lengths to 16 bytes            */
        if ((
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
             (pDriverParams->statisticsFlag != CSI2_AUTODC_NO) &&
             ((lenBufData + CSI2_LINE_STAT_LENGTH) <= pVCparams->bufLineLen)) ||
             ((pDriverParams->statisticsFlag == CSI2_AUTODC_NO) &&
#endif
             (lenBufData <= pVCparams->bufLineLen)))
        {
            /* save the working params for future usage                                                     */
            pDriverStateVC->outputDataMode = (uint16)pVCparams->outputDataMode;
            pDriverStateVC->eventsMask = (uint8)pVCparams->vcEventsReq;
            /* wrong buffer line number at start                                                            */
            pDriverStateVC->lastReceivedBufLine = CSI2_CHIRP_NOT_STARTED;
            pDriverStateVC->lastReceivedChirpLine = pVCparams->expectedNumLines;
            cfg2 = (uint32)Csi2_GetChannelNum(pDriverStateVC);   /* the real number for channels     */
            cfg2 = ((uint32)1 << cfg2) - ((uint32)1);                        /* mask for enable the real channels */
            pRegs->RX_CBUF_CHNLENBL[vcId] = cfg2;                       /* the receiving channels           */
            /* no 5th channel influence
             * check for data translation and add SWAPRAWDATA, even it was asked or not explicitly          */
            cfg2 = (pVCparams->streamDataType == (uint16)CSI2_DATA_TYPE_16_FROM_8) ?
                                                    (uint32)CSI2_VC_BUF_SWAP_RAW8 : 0u;
            /* the output data mode                                                                         */
            pRegs->RX_CBUF_OUTCFG[vcId] = (cfg2 | (uint32)pVCparams->outputDataMode) >> 4u;
        }
        else
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_TOO_SMALL_BUFFER, (uint8)CSI2_API_ID_SETUP,
                                                    (uint8)CSI2_E_DRV_TOO_SMALL_BUFFER);
            CSI2_HALT_ON_ERROR;
        }
    }
    else
    {           /* auxiliary data       */
        if(lenBufData > pVCparams->bufLineLen)
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_TOO_SMALL_BUFFER, (uint8)CSI2_API_ID_SETUP,
                                                    (uint8)CSI2_E_DRV_TOO_SMALL_BUFFER);
            CSI2_HALT_ON_ERROR;
        }
    }
    if ((rez == (Std_ReturnType)E_OK) && (vcId < (uint8)CSI2_MAX_VC))
    {
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
        for (i = (uint32)CSI2_CHANNEL_A; i < (uint32)CSI2_MAX_CHANNEL; i++)
        {
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
            pDriverStateVC->statDC[i].channelSum = 0;       /* init of VC struct for DC statistics          */
#endif
            pDriverStateVC->statDC[i].channelMax =
                ((pDriverStateVC->outputDataMode & (uint32)CSI2_VC_BUF_FLIP_SIGN) == 0u) ?
                    (sint16)CSI2_MIN_VAL_SIGNED : (sint16)CSI2_MIN_VAL_UNSIGNED;
            pDriverStateVC->statDC[i].channelMin =
                ((pDriverStateVC->outputDataMode & (uint32)CSI2_VC_BUF_FLIP_SIGN) == 0u) ?
                    (sint16)CSI2_MAX_VAL_SIGNED : (sint16)CSI2_MAX_VAL_UNSIGNED;
            pDriverStateVC->statDC[i].channelBitToggle = (uint16)0;
        }  /* for   */
#endif
    }
    if(rez == (Std_ReturnType)E_OK)
    {
        /* configure each VC with the very specific params                                                  */
        Csi2_UnitConfigVCSpec(pRegs, pVCparams, vcId, lenChirpData, lenBufData, pVCparams->bufLineLen);
    }
    return rez;
}
/* Csi2_ConfigVC *************************/


#if (CSI2_METADATA_DATA_USAGE == STD_ON)
/*================================================================================================*/
/*
 * @brief       Procedure for general programming a MetaData channel.
 * @details     This procedure initialize all "common" parameters of a MetaData channel.
 *
 * @param[in]   csi2UnitNum - unit number, RSDK_CSI2_UNIT_0 ... MAX
 * @param[in]   vcId        - VC number, RSDK_CSI2_VC_0 ... MAX
 * @param[in]   pRegs       - unit registry pointer
 * @param[in]   pMDparams   - pointer to MetaData parameters
 *
 * @return      RSDK_SUCCESS   Successful initialization
 *              RSDK_CSI2_WRG_START_PARAMS       Wrong unit id
 *
 */
static Std_ReturnType Csi2_ConfigMD(const uint8 vcId, volatile GENERIC_CSI2_Type *pRegs,
                    const Csi2_MetaDataParamsType *pMDparams)
{
    uint32                   val1, bufNr, dataType;
    uint16                   lenBufData;                    /* data lengths     */
    Std_ReturnType           rez;                           /* result           */

    rez = E_OK;
    bufNr = (uint32)vcId + ((uint32)CSI2_MAX_VC * (uint32)2);
    /* compute the necessary buffer space, according to chirp values and other requirements
       buffer length in bytes   */
    lenBufData = pMDparams->expectedNumBytes;
    dataType = ((uint32)pMDparams->streamDataType & (uint32)CSI2_NORM_DTYPE_MASK);
    if (lenBufData > pMDparams->bufLineLen)
    {   /* wrong buffer  length, the buffer line length is not enough   */
        rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_TOO_SMALL_BUFFER, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_TOO_SMALL_BUFFER);
        CSI2_HALT_ON_ERROR;
    }
    else
    {
        if((CSI2_UPPER_ALIGN_TO_(pMDparams->bufLineLen, 16u) != pMDparams->bufLineLen) ||
                (CSI2_UPPER_ALIGN_TO_(lenBufData, 16u) != lenBufData))
        {
            /* buffer not aligned       */
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_BUF_LEN_NOT_ALIGNED, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_BUF_LEN_NOT_ALIGNED);
            CSI2_HALT_ON_ERROR;
        }
    }
    if(rez == (Std_ReturnType)E_OK)
    {
        val1 = 1u;
        pRegs->RX_VCENABLE |= (val1 << vcId) + (val1 << (8u + bufNr));               /* enable the VC, for sure  */
        /* buffer configuration         */
        dataType = ((uint32)pMDparams->streamDataType & (uint32)CSI2_NORM_DTYPE_MASK);
        /* check for data translation   */
        if(dataType == (uint32)CSI2_DATA_TYPE_16_FROM_8)
        {
            dataType = (uint32)CSI2_DATA_TYPE_RAW8;  /* RSDK_CSI2_DATA_TYPE_16_FROM_8 is only a translation type    */
        }
        pRegs->RX[bufNr].CBUF_CONFIG = ((bufNr & CSI2_MAX_VC_MASK) << 8u) + (dataType << 2u);

        /* buffer pointer           */
        pRegs->RX[bufNr].CBUF_SRTPTR = (uint32)pMDparams->bufDataPtr;

        /* buffer number of lines   */
        pRegs->RX[bufNr].CBUF_NUMLINE = pMDparams->bufNumLines;

        /* buffer line length       */
        pRegs->RX[bufNr].CBUF_BUFLEN = pMDparams->bufLineLen;

        /* input interface config   */
        pRegs->RX[bufNr].INPLINELEN_CONFIG = pMDparams->expectedNumBytes;

        /* number of chirps to be received  */
        pRegs->RX[bufNr].NUMLINES_CONFIG = pMDparams->expectedNumLines;

        /* buffer chirp data length     */
        pRegs->RX[bufNr].LINELEN_CONFIG = lenBufData;

    }
    return rez;
}
/* Csi2ConfigMD *************************/
#endif /*         #if (CSI2_METADATA_DATA_USAGE == STD_ON)      */

/*================================================================================================*/
/*
 * @brief       Procedure for VC setup.
 * @details     This procedure setup a VirtualChannel, normal or auxiliary.
 *
 * @param[in]   csi2UnitNum = MIPI CSI2 unit ID
 * @param[in]   pRegs       = pointer to unit registry
 * @param[in]   pParams     = pointer to the unit initialization parameters
 *
 * @return      result status, success or error
 *
 */
static Std_ReturnType Csi2_SetupAllVcAux(const Csi2_UnitIdType csi2UnitNum, volatile GENERIC_CSI2_Type *pRegs,
                                     const Csi2_SetupParamsType *pParams)
{
    Std_ReturnType  rez;
    uint32          i;
#if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
    uint32          val;
#endif
    rez = (Std_ReturnType)E_OK;
    for (i = 0u; i < (uint32)CSI2_MAX_VC; i++)
    {
        if(pParams->vcConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR)
        {
            /* virtual channel real configuration                       */
            rez = Csi2_ConfigVC(csi2UnitNum, (uint8)i, pRegs, pParams->vcConfigPtr[i]);
        }
#if (CSI2_AUXILIARY_DATA_USAGE == STD_ON) || (CSI2_METADATA_DATA_USAGE == STD_ON)
        if (rez == (Std_ReturnType)E_OK)
        {
#if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
            /* check for auxiliary data buffer                          */
            val = 0u;
            if (pParams->vcConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR)
            {
                val = (uint32)pParams->vcConfigPtr[i]->outputDataMode;
                val &= (uint32)CSI2_VC_BUF_FIFTH_CH_ON;
            }
            if ((val != 0u) && (pParams->auxConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR))
            {
                rez = Csi2_ConfigVC(csi2UnitNum, (uint8)i + (uint8)CSI2_MAX_VC, pRegs, pParams->auxConfigPtr[i]);
            }
#endif
#if (CSI2_METADATA_DATA_USAGE == STD_ON)
            if ((rez == (Std_ReturnType)E_OK) && (pParams->metaDataPtr[i] != (Csi2_MetaDataParamsType*)NULL_PTR))
            {
                if(((uint16)pParams->metaDataPtr[i]->streamDataType != (uint16)CSI2_DATA_TYPE_EMBD) &&
                (((uint16)pParams->metaDataPtr[i]->streamDataType < (uint16)CSI2_DATA_TYPE_USR0) ||
                ((uint16)pParams->metaDataPtr[i]->streamDataType > (uint16)CSI2_DATA_TYPE_USR7)))
                {
                    /*  only embedded or image data can be received   */
                    rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_DATA_TYPE, (uint8)CSI2_API_ID_SETUP,
                                                        (uint8)CSI2_E_DRV_INVALID_DATA_TYPE);
                    CSI2_HALT_ON_ERROR;
                }
                else
                {
                    rez = Csi2_ConfigMD((uint8)i, pRegs, pParams->metaDataPtr[i]);
                }
            }
#endif
        }
#endif /* #if (CSI2_AUXILIARY_DATA_USAGE == STD_ON) || (CSI2_METADATA_DATA_USAGE == STD_ON) */
        if (rez != (Std_ReturnType)E_OK)
        {
            break;      /* error in VC initialization, so stop here and report the error        */
        }
    }
    return rez;
}
/* Csi2_SetupAllVcAux *************************/


/*================================================================================================*/
/*
 * @brief       Procedure to introduce a simple and very short delay in execution.
 * @details     The execution counts loops, not time divisions
 *
 * @param[in]   loops   Number of loops to wait (usually considered as ns)
 *
 */
static void Csi2_WaitLoopNs(uint32 loops)
{
    uint32 iLoop;

    iLoop = loops;
    while (iLoop != 0u)
    {
        iLoop--;
    }
}
/* Csi2_WaitLoopNs *************************/


/*================================================================================================*/
/*
 * @brief       Procedure to introduce a microseconds delay in execution.
 * @details     For AUTOSAR, the internal OsOf timer implementation is used
 *
 * @param[in]   loops   Number of microseconds to wait
 *
 * @pre         The OsIf_Init must be called before and the appropriate timer type to be selected for CSI2_TIMER_TYPE
 */
static uint8 Csi2_WaitLoopUs(uint32 uSec)
{
    uint8   rez = 0u;               /* no error     */
    volatile uint32 i = CSI2_US_DELAY * uSec;
    while(i != 0u)
    {
        i--;
    }
    return rez;
}
/* Csi2_WaitLoopUs *************************/


/*================================================================================================*/
/*
 * @brief       Procedure which wait for STOP state on data lanes.
 * @details     The time-out is set to 1.2 ms.
 *
 * @param[in]   pInitParams     - pointer to the parameters structure
 *
 * @return      Std_ReturnType  - success or error
 *
 */
static Std_ReturnType Csi2_WaitForStopState(const volatile
        GENERIC_CSI2_Type
        *pRegs, const uint32 maxLane)
{
    Std_ReturnType  rez = (Std_ReturnType)E_OK;
    uint32          i, stp;
    uint32          j;

    /* Wait for about 1.2ms for STOP state on data lines or error reported
     * If the frontend CSI2 interface isn't powered up this check (step 25) could fail.
     * But the interface will start working correctly once the front-end CSI2 powers up.    */
    for(i = 0u; i < (uint32)CSI2_MAX_WAIT_FOR_STOP; i++)
    {
        (void)Csi2_WaitLoopUs(1u);

        stp = pRegs->RX_LANCS[0];
        for(j = (uint32)CSI2_LANE_1; j < maxLane; j++)
        {
            stp &= pRegs->RX_LANCS[j];
        }
        stp &= MIPICSI2_RX_LANCS_DSTOP_MASK;
        if((stp != 0u)
            )
        {
            break;
        }
    }
    if((stp == 0u) && (i >= (uint32)CSI2_MAX_WAIT_FOR_STOP))
    {       /* time-out waiting for STOP state      */
        rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_CALIBRATION_TIMEOUT, (uint8)CSI2_API_ID_SETUP,
                                                                (uint8)CSI2_E_DRV_CALIBRATION_TIMEOUT);

        CSI2_HALT_ON_ERROR;
    }
    return rez;
}
/* Csi2_WaitForStopState *************************/


#if (CSI2_DFS_USAGE == CSI2_DFS_ONCE) || (CSI2_DFS_USAGE == CSI2_DFS_ALWAYS)
/*================================================================================================*/
/*
 * @brief       Procedure for correct start of the CSI2 clock.
 * @details     This procedure reset the MIPI_CSI_TXRX_LI_CLK, using the DFS bit responsible.
 *
 */
static void DfsTilt(void)
{
    uint32_t regVal, i, testVal = (1u << 2u);
    volatile DFS_Type *pDfs;

#ifdef linux
    /* assume unit 0 is already initialized                                                             */
    pDfs = (volatile DFS_Type *)(volatile void *)gpRsdkCsi2Device[0]->pMemMapVirtDfs;
#else
    pDfs = (volatile DFS_Type *)IP_PERIPH_DFS;
#endif /* linux                                    */
    regVal = pDfs->PORTRESET;
    regVal |= testVal;
    pDfs->PORTRESET = regVal;                               /* step 2 - Gate the MIPI_CSI_TXRX_LI_CLK   */
    i = 3u;
    while (((pDfs->PORTSR & testVal) != 0u) && (i > 0u))    /* wait for the clock output to lock        */
    {
        (void)Csi2_WaitLoopUs(15);
        i--;
    }
    regVal &= ~testVal;
    pDfs->PORTRESET = regVal;                               /* step 3 - Ungate the MIPI_CSI_TXRX_LI_CLK */
}
/* DfsTilt *************************/
#endif /* #if defined(CSI2_DFS_ONCE) || defined(CSI2_DFS_ALWAYS)    */


#if (CSI2_GPIO_USED == STD_ON)
/*================================================================================================*/
/*
 * @brief       Prepare the output GPIO for the application requirements.
 *
 * @param[in] pRegs     - pointer to unit registry
 * @param[in] csi2VC    - virtual channel number
 * @param[in] pVCparams - pointer to VC parameters
 *
 */
static void Csi2_SetGpio(volatile GENERIC_CSI2_Type *pRegs, const uint8 csi2VC,
                        const Csi2_VCParamsType *pVCparams)
{
    uint32  mask1, maskE;                  /* masks for GPIO1 & Enable                 */
    uint32  eraseMask, eraseMaskE;         /* erase masks for the same as above        */

    if (pVCparams != (Csi2_VCParamsType*)NULL_PTR)
    {
        /* initialize the masks                                                         */
        mask1 = (uint32)pVCparams->gpio1Mask;
        maskE = (uint32)pVCparams->gpio2EnaMask;
        maskE <<= 12u;
        maskE |= (uint32)pVCparams->gpio1EnaMask;
        eraseMask = 0xff;
        eraseMaskE = 0x7007;
        /* get the final usable masks, according the VC                                 */
        switch (csi2VC)
        {
            case (uint8)CSI2_VC_0:          /* for VC_0 the masks remains as is         */
                break;
            case (uint8)CSI2_VC_1:          /* shifts for VC_1                          */
                mask1 <<= 8u;
                maskE <<= 3u;
                eraseMask <<= 8u;
                eraseMaskE <<= 3u;
                break;
            case (uint8)CSI2_VC_2:          /* shifts for VC_2                          */
                mask1 <<= 16u;
                maskE <<= 6u;
                eraseMask <<= 16u;
                eraseMaskE <<= 6u;
                break;
            default:                        /* shifts for VC_3                          */
                mask1 <<= 24u;
                maskE <<= 9u;
                eraseMask <<= 24u;
                eraseMaskE <<= 9u;
                break;
        }
        /* set all targeted bits to 0                                                   */
        pRegs->TRIGGER_GPIO1 &= eraseMask;
        pRegs->TRIGGEREN_GPIO &= eraseMaskE;
        /* set the required values                                                      */
        pRegs->TRIGGER_GPIO1 |= mask1;
        pRegs->TRIGGEREN_GPIO |= maskE;
    }
}
/* Csi2_SetGpio *************************/
#endif /* #if (CSI2_GPIO_USED == STD_ON) */


#if (CSI2_SDMA_USED == STD_ON)
/*================================================================================================*/
/*
 * @brief       Prepare the output SDMA for the application requirements.
 * @details     Prepare the output SDMA for the application requirements.
 *
 * @param[in] pRegs     - pointer to unit registry
 * @param[in] csi2VC    - virtual channel number
 * @param[in] pVCparams - pointer to VC parameters
 *
 */
static void Csi2_SetSdma(volatile GENERIC_CSI2_Type *pRegs, const uint8 csi2VC,
                        const Csi2_VCParamsType *pVCparams)
{
    uint32 mask1, maskE;                    /* masks for SDMA1 & Enable                 */
    uint32 eraseMask, eraseMaskE;           /* erase masks for the same as above        */
    if (pVCparams != (Csi2_VCParamsType*)NULL_PTR)
    {
        /* initialize the masks                                                         */
        mask1 = (uint32)pVCparams->sdma1Mask;
        maskE = (uint32)pVCparams->sdma2EnaMask;
        maskE <<= 12u;
        maskE |= (uint32)pVCparams->sdma1EnaMask;
        eraseMask = 0xff;
        eraseMaskE = 0x7007;
        /* get the final usable masks, according the VC                                 */
        switch (csi2VC)
        {
            case (uint8)CSI2_VC_0:
                break;
            case (uint8)CSI2_VC_1:
                mask1 <<= 8u;
                maskE <<= 3u;
                eraseMask <<= 8u;
                eraseMaskE <<= 3u;
                break;
            case (uint8)CSI2_VC_2:
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
        /* set all targeted bits to 0                                                   */
        pRegs->TRIGGER_SDMA1 &= eraseMask;
        pRegs->TRIGGEREN_SDMA &= eraseMaskE;
        /* set the required values                                                      */
        pRegs->TRIGGER_SDMA1 |= mask1;
        pRegs->TRIGGEREN_SDMA |= maskE;
    }
}
/* Csi2_SetSdma *************************/
#endif


/*================================================================================================*/
/*
 * @brief       Procedure for specific unit setup.
 * @details     The parameters must be all correct at this point.
 *
 * @param[in]   csi2UnitNum     - unit number
 * @param[in]   pUnitParams     - pointer to the parameters structure
 *
 * @return      Std_ReturnType  - success or error
 *
 */
static Std_ReturnType Csi2_ModuleSetup(const Csi2_UnitIdType csi2UnitNum, const Csi2_SetupParamsType *pParams)
{
    volatile GENERIC_CSI2_Type          *pRegs;
    Std_ReturnType                      rez;
    Csi2_DriverParamsType *             pDriverState;
    Csi2_UnitIdType                     usedUnit = csi2UnitNum;
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON) || (CSI2_SAF85XX_PHY_USED == STD_ON)
    uint32                              i;
#endif
#if ((CSI2_GPIO_USED == STD_ON) || (CSI2_SDMA_USED == STD_ON))
    uint8                               vcId;
#endif

    rez = (Std_ReturnType)E_OK;

    volatile uint8 *                    pDphyRegs = (volatile uint8*)(NULL_PTR);    /* pointer to DPHY registry     */
                                        /* temporary values for quick DPHY calibration                              */
    uint8                               lVal0 = 0u, lVal1 = 0u, lVal2 = 0u, lVal3 = 0u, lVal4 = 0u;
    uint16                              DdlOscFreq;
    uint8                               laneId;
    uint32                              val;
    uint8                               HsFreqRange, lanesSwap;
    Csi2_UnitIdType                     pllUnit;

    pDriverState = &gCsi2Settings[(uint8)usedUnit];
    if((uint8)usedUnit < (uint8)CSI2_UNIT_2)
    {
        pllUnit = CSI2_UNIT_0;
    }
    else
    {
        pllUnit = CSI2_UNIT_2;
    }
    /* check first to have UNIT_1 used/setup before UNIT_0                                                          */
    if (usedUnit != pllUnit)
    {
        if (gCsi2Settings[(uint8)pllUnit].driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)     /* unit 0 not setup */
        {
            /* error, unit 0 must be setup first                                                                    */
            if(pllUnit == CSI2_UNIT_2)
            {
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_ERR_UNIT_2_MUST_BE_FIRST, (uint8)CSI2_API_ID_SETUP,
                                                                            (uint8)CSI2_E_DRV_ERR_UNIT_2_MUST_BE_FIRST);
            }
            else
            {
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_ERR_UNIT_0_MUST_BE_FIRST, (uint8)CSI2_API_ID_SETUP,
                                                                            (uint8)CSI2_E_DRV_ERR_UNIT_0_MUST_BE_FIRST);
            }
            CSI2_HALT_ON_ERROR;
        }
    }
    if (rez == (Std_ReturnType)E_OK)
    {
#ifdef linux
        if(pDriverState->driverState != CSI2_DRIVER_STATE_NOT_INITIALIZED)
        {
            for(i =  0; i < (uint32_t)RSDK_CSI2_MAX_VC; i++)
            {
                if((pDriverState->workingParamVC[i].vcParamsPtr != NULL) && 
                                (pDriverState->workingParamVC[i].pVirtData != NULL))
                {
                    iounmap(pDriverState->workingParamVC[i].pVirtData);     /* unmap the mapped memory          */
                    pDriverState->workingParamVC[i].pVirtData = NULL;
                }
            }
        }
#endif
        pDriverState->driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED;  /* unit not setup                           */
        rez = Csi2_RegsMap(usedUnit, &pRegs);                           /* get the  registry pointer for the unit   */
        if (rez == (Std_ReturnType)E_OK)
        {       /* valid registry pointer   */
            gMipiCsi2RegsPtr[(uint8)usedUnit] = pRegs;                    /* keep the registry pointer for future     */
            /* execute short calibration only if required by application                                            */
            if(((uint8)pParams->initOptions & (uint8)CSI2_DPHY_INIT_SHORT_CALIB) != 0u)
            {
                /* supplementary step : read some registry to check for previous setup                              */
                pDphyRegs = (volatile uint8 *)pRegs;                    /* initialize the DPY registry pointer      */
                lVal0 = pDphyRegs[DPHY_DATALOFFSETCAL_VALUE0_OFFSET];
                lVal1 = pDphyRegs[DPHY_DATALOFFSETCAL_VALUE1_OFFSET];
                lVal2 = pDphyRegs[DPHY_DATALOFFSETCAL_VALUE2_OFFSET];
                lVal3 = pDphyRegs[DPHY_DATALOFFSETCAL_VALUE3_OFFSET];
                lVal4 = pDphyRegs[DPHY_CLKCALVAL_COMPS_OFFSET];
            }

            /* supplementary step : SoftReset
             * not present in RM (at least for the moment)
             * necessary to reset the NEXTLINE registry and implicitly the start of the next frame                  */
            pRegs->RX_SR = (uint32)CSI2_SOFTRESET_BIT;
            pRegs->RX_SR = 0u;

            /* start of DPHY subsystem initialization ===========================================================   */
            /* setup CSI2 subsystem : RM/rev.1/draft O pag.1957
             * according to the latest RM, only Rx can be used, no Tx available                                     */

            pRegs->RX_RXNULANE = (uint32)pParams->numLanesRx + 1u;  /* step 1 - set the Rx num. lanes               */
        #if (CSI2_DFS_USAGE == CSI2_DFS_ONCE)
            if(gsCsi2DfsReset == 0u)
            {
                DfsTilt();  /* step 2&3 - Gate/ungate the MIPI_CSI_TXRX_LI_CLK                                      */
                gsCsi2DfsReset = 1u;
            }
        #elif (CSI2_DFS_USAGE == CSI2_DFS_ALWAYS)
            DfsTilt();      /* step 2&3 - Gate/ungate the MIPI_CSI_TXRX_LI_CLK                                      */
        #endif  /* #if (CSI2_DFS_USAGE == CSI2_DFS_ONCE)    */

            pRegs->DPHY_RSTCFG = 0u;                                /* step 4 & 5 - clear RSTZ & SHUTDWNZ           */

            pRegs->DPHY_CLEAR = 1u;                                 /* step 6 - set CLRREG                          */
            Csi2_WaitLoopNs(15);                                    /* step 7 - wait for 15ns                       */

            pRegs->DPHY_CLEAR = 0u;                                 /* step 8 - clear CLRREG                        */

            /* continue with short calibration only if required by application                                      */
            if(((uint8)pParams->initOptions & (uint8)CSI2_DPHY_INIT_SHORT_CALIB) != 0u)
            {
                /* DPHY manual setup using the previous claibration values
                 * use the previous values for a quicker initialization                                             */
                /* enable calibration override and set the appropriate value                                        */

                pDphyRegs[DPHY_TX_RDWR_TERM_CAL_1_OFFSET] = 0x1u;           /* enable more calibration override     */
                pDphyRegs[DPHY_CLKOFFSETCAL_OVRRIDE_OFFSET] = 0x1u;         /* enable clock calibration override    */
                pDphyRegs[DPHY_CLKOFFSETCAL_OVRRIDEVAL_OFFSET] = (uint8)((lVal4 & (uint8)0x7fu));
                                                                            /* compensation for clock calibration   */
                pDphyRegs[DPHY_DATALOFFSETCAL_OVRVALUE0_OFFSET] = (uint8)(((lVal0 & (uint8)0x7fu) << 1u) + 0x1u);
                                                                            /* set the value and enable override    */
                pDphyRegs[DPHY_DATALOFFSETCAL_OVRVALUE1_OFFSET] = (uint8)(((lVal1 & (uint8)0x7fu) << 1u) + 0x1u);
                                                                            /* set the value and enable override    */
                pDphyRegs[DPHY_DATALOFFSETCAL_OVRVALUE2_OFFSET] = (uint8)(((lVal2 & (uint8)0x7fu) << 1u) + 0x1u);
                                                                            /* set the value and enable override    */
                pDphyRegs[DPHY_DATALOFFSETCAL_OVRVALUE3_OFFSET] = (uint8)(((lVal3 & (uint8)0x7fu) << 1u) + 0x1u);
                                                                            /* set the value and enable override    */
                pDphyRegs[DPHY_DATAL0OFFSETCAL_OVRCNTRL_OFFSET] = 0x4u;     /* enable lane calibration override     */
                pDphyRegs[DPHY_DATAL1OFFSETCAL_OVRCNTRL_OFFSET] = 0x4u;     /* enable lane calibration override     */
                pDphyRegs[DPHY_DATAL2OFFSETCAL_OVRCNTRL_OFFSET] = 0x4u;     /* enable lane calibration override     */
                pDphyRegs[DPHY_DATAL3OFFSETCAL_OVRCNTRL_OFFSET] = 0x4u;     /* enable lane calibration override     */
                pDphyRegs[DPHY_RX_STARTUP_OVERRIDE_OFFSET] = 0x4u;          /* bypass calibration                   */
            }

            /* dummy step - PLL activation                                                                          */
            if (usedUnit == pllUnit)
            {
                pRegs->DPHY_PLL_VREF_CONFIG = 3u;                   /* enable both units PLL from the beginning     */
            }

            /* step 9 - Set the operating frequency HSFREQRNG                                                       */
            Csi2_GetOperatingSpeedMask((uint16)pParams->rxClkFreq, &HsFreqRange, &DdlOscFreq);

            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(&pRegs->DPHY_FREQCFG, MIPICSI2_DPHY_FREQCFG_HSFREQRNG_MASK,
                                                        MIPICSI2_DPHY_FREQCFG_HSFREQRNG(HsFreqRange));

            /* step 10 - Configure register Common Block Control (DPHY_ATB_CB_ATB_VBE_SEL)                          */
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY8(&pRegs->DPHY_ATB_CB_ATB_VBE_SEL,
                    MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREF_LPRX_RW_1_0_MASK,
                    MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREF_LPRX_RW_1_0(2u));

            /* step 11 - Configure register Common Block Control (DPHY_CB_VBE_SEL)                                  */
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY8(&pRegs->DPHY_CB_VBE_SEL, MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_VREFCD_LPRX_RW__1__0___MASK,
                    MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_VREFCD_LPRX_RW__1__0__(2u));

            /* step 12 - Write DPHY_CLOCK_LANE_CNTRL                                                                */
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY8(&pRegs->DPHY_CLOCK_LANE_CNTRL,
                    MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_PULL_LONG_CHANNEL_IF_RW_MASK,
                    MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_PULL_LONG_CHANNEL_IF_RW(1u));

            /* step 13 - Configure the DDL target oscillation frequency                                             */
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY8(&pRegs->DPHY_DDLOSCFREQ_CFG1,
                    MIPICSI2_DPHY_DDLOSCFREQ_CFG1_DDL_OSC_FREQ_TARGET_OVR_RW__7__0___MASK,
                    MIPICSI2_DPHY_DDLOSCFREQ_CFG1_DDL_OSC_FREQ_TARGET_OVR_RW__7__0__((uint8)(DdlOscFreq)));
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY8(&pRegs->DPHY_DDLOSCFREQ_CFG2,
                    MIPICSI2_DPHY_DDLOSCFREQ_CFG2_DDL_OSC_FREQ_TARGET_OVR_RW__11__8___MASK,
                    MIPICSI2_DPHY_DDLOSCFREQ_CFG2_DDL_OSC_FREQ_TARGET_OVR_RW__11__8__(DdlOscFreq >> 8u));

            /* step 14 - Enable override for configured DDL oscillation frequency to take effect                    */
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY8(&pRegs->DPHY_DDLOSCFREQ_OVREN,
                    MIPICSI2_DPHY_DDLOSCFREQ_OVREN_DDL_OSC_FREQ_TARGET_OVR_EN_RW_MASK,
                    MIPICSI2_DPHY_DDLOSCFREQ_OVREN_DDL_OSC_FREQ_TARGET_OVR_EN_RW(1u));

            /* step 15 - Configure the config clock frequency CLKFREQRNG                                            */
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(&pRegs->DPHY_FREQCFG, MIPICSI2_DPHY_FREQCFG_CLKFREQRNG_MASK,
                    MIPICSI2_DPHY_FREQCFG_CLKFREQRNG(0xc));

            /* step 16 - Set FORCERXMODE = 1 (on all lanes)                                                         */
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(&pRegs->TURNCFG, MIPICSI2_TURNCFG_FORCERXMODE1_MASK, MIPICSI2_TURNCFG_FORCERXMODE1(1u));
            if (pParams->numLanesRx >= (uint8)CSI2_LANE_1)
            {
                /*
                * @section Csi2_c_REF_1
                * Violates MISRA 2012 Advisory Directive 4.9,
                * #A function should be used in preference to a function-like macro where they are interchangeable
                * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
                * which use only UPPERCASE
                */
                CSI2_SET_REGISTRY32(&pRegs->TURNCFG, MIPICSI2_TURNCFG_FORCERXMODE2_MASK,
                        MIPICSI2_TURNCFG_FORCERXMODE2(1u));
            }
            if (pParams->numLanesRx >= (uint8)CSI2_LANE_2)
            {
                /*
                * @section Csi2_c_REF_1
                * Violates MISRA 2012 Advisory Directive 4.9,
                * #A function should be used in preference to a function-like macro where they are interchangeable
                * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
                * which use only UPPERCASE
                */
                CSI2_SET_REGISTRY32(&pRegs->TURNCFG, MIPICSI2_TURNCFG_FORCERXMODE3_MASK,
                        MIPICSI2_TURNCFG_FORCERXMODE3(1u));
            }
            if (pParams->numLanesRx == (uint8)CSI2_LANE_3)
            {
                /*
                * @section Csi2_c_REF_1
                * Violates MISRA 2012 Advisory Directive 4.9,
                * #A function should be used in preference to a function-like macro where they are interchangeable
                * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
                * which use only UPPERCASE
                */
                CSI2_SET_REGISTRY32(&pRegs->TURNCFG, MIPICSI2_TURNCFG_FORCERXMODE4_MASK,
                        MIPICSI2_TURNCFG_FORCERXMODE4(1u));
            }

            /* step 17 - Configure the relevant enables for clock and data lanes for start MIPICSI2 data
             *          reception and reverse high speed transmission CLOCK_LANE_EN
             *          DATA_LANE_EN for clock and data lanes respectively                                          */
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(&pRegs->RX_RXENABLE, MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_MASK,
                    MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN(1u));
            /* enable all active lanes                                                                              */
            val = 1u;
            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(&pRegs->RX_RXENABLE, MIPICSI2_RX_RXENABLE_CFG_DATA_LANE_EN_MASK,
                    MIPICSI2_RX_RXENABLE_CFG_DATA_LANE_EN((val << (pParams->numLanesRx + 1u)) - 1u));

            /* step 18 - Set the lane swap as per physical lane connection                                          */
            lanesSwap = 0;
            for (laneId = (uint8)CSI2_LANE_0; laneId < (uint8)CSI2_MAX_LANE; laneId++)
            {
                lanesSwap |= ((uint8)pParams->lanesMapRx[laneId]) << (laneId * 2u);
            }
            pRegs->RX_RXLANESWAP = lanesSwap;

            /* step 19 - Configure CFG_FLUSH_CNT                                                                    */
            if (pParams->rxClkFreq < (uint32)CSI2_FLUSH_CNT_FREQ_LIMIT)
            {
                /*
                * @section Csi2_c_REF_1
                * Violates MISRA 2012 Advisory Directive 4.9,
                * #A function should be used in preference to a function-like macro where they are interchangeable
                * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
                * which use only UPPERCASE
                */
                CSI2_SET_REGISTRY32(&pRegs->RX_RXENABLE, MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT_MASK,
                        MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT(7u)); /* rates up to 200MHz   */
            }
            else
            {
                /*
                * @section Csi2_c_REF_1
                * Violates MISRA 2012 Advisory Directive 4.9,
                * #A function should be used in preference to a function-like macro where they are interchangeable
                * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
                * which use only UPPERCASE
                */
                CSI2_SET_REGISTRY32(&pRegs->RX_RXENABLE, MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT_MASK,
                        MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT(3u)); /* rates over to 200MHz */
            }  /* if(pParams->dphyClkFreq < CSI2_FLUSH_CNT_FREQ_LIMIT)  */
            /* end of DPHY subsystem initialization ===========================================================     */

            /* step 20 - configure the circular buffers                                                             */
            pRegs->RX_VCENABLE = 0u;                                    /* disable all                              */
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
            pDriverState->statisticsFlag = CSI2_AUTODC_NO;              /* disable statistics for unit              */
    #if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
            /* process first the DC management for each channel on each VC                                          */
            for (i = 0; i < (uint32)CSI2_MAX_VC; i++)
            {
                pDriverState->workingParamVC[i].eventsMask = 0;
                if (Csi2_SetOffsetMan(&pDriverState->workingParamVC[i], pParams->vcConfigPtr[i]) != 0u)
                {
                    pDriverState->statisticsFlag = (Csi2_AutoDCComputeTimeType)CSI2_AUTODC_LAST_LINE;
                }
            }
            if (pDriverState->statisticsFlag != CSI2_AUTODC_NO)
            {                                                            /* at least one channel with AUTO_DC       */
                /* just copy the value, checked before     */
                pDriverState->statisticsFlag = (Csi2_AutoDCComputeTimeType)pParams->statManagement;
            }
    #endif
#endif
            rez = Csi2_SetupAllVcAux(usedUnit, pRegs, pParams);

            if (rez == (Std_ReturnType)E_OK)
            {       /* successful VC setup     */
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
                if (pDriverState->statisticsFlag != CSI2_AUTODC_NO)
                {
                    pRegs->RX_STAT_CONFIG = (uint32)1
                    ; /* at least one channel with auto DC offset or statistics required  */
                }
                else
                {
#endif
                    pRegs->RX_STAT_CONFIG = (uint32)0
                    ;                         /* no statistics at all                     */
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
                }
#endif

                /* step 21 - Configure noext_burnin_res_cal_rw                                                      */
                /*
                * @section Csi2_c_REF_1
                * Violates MISRA 2012 Advisory Directive 4.9,
                * #A function should be used in preference to a function-like macro where they are interchangeable
                * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
                * which use only UPPERCASE
                */
                CSI2_SET_REGISTRY8(&pRegs->DPHY_CALTYPE_CNTRL,
                        MIPICSI2_DPHY_CALTYPE_CNTRL_NOEXT_BURNIN_RES_CAL_RW_MASK, 0u); /* external resistor   */

                Csi2_WaitLoopNs(5);                                     /* step 22 - wait for 5ns                   */
                pRegs->DPHY_RSTCFG = 1u;                                /* step 23 - set SHUTDWNZ                   */
                Csi2_WaitLoopNs(5);                                     /* step 24 - wait for 5ns                   */
                pRegs->DPHY_RSTCFG = 3u;                                /* step 25 - Set the field RSTZ             */

                /* step 26 - Wait till stop state is observed                                                       */
                        if(((uint8)pParams->initOptions & (uint8)CSI2_DPHY_INIT_W_STOP_STATE) != 0u)
                {
                    rez = Csi2_WaitForStopState(
                            pRegs
                            , (uint32)pParams->numLanesRx + 1u);
                }
            }
            if (rez == (Std_ReturnType)E_OK)
            {
#if (CSI2_GPIO_USED == STD_ON) || (CSI2_SDMA_USED == STD_ON)
                /* assume step 26 done with success => setup ok
                 * do the other VC setup - GPIO, SDMA, irq handling                                                 */
                for (vcId = (uint8)CSI2_VC_0; vcId < (uint8)CSI2_MAX_VC; vcId++)
                {
                    /* gpio & sdma programming per VC                                                               */
        #if (CSI2_GPIO_USED == STD_ON)
                    Csi2_SetGpio(pRegs, vcId, pParams->vcConfigPtr[vcId]);
        #endif
        #if (CSI2_SDMA_USED == STD_ON)
                    Csi2_SetSdma(pRegs, vcId, pParams->vcConfigPtr[vcId]);
        #endif
                }  /* for   */
#endif /* #if (CSI2_GPIO_USED == STD_ON) || (CSI2_SDMA_USED == STD_ON) */
                /* interrupts programming                                                                           */
                rez =
                        Csi2_SetupUIrq(usedUnit, pRegs, pParams);
                pDriverState->driverState = CSI2_DRIVER_STATE_ON;   /* setup done ok                                */

                /* step 27 - Clear FORCERXMODE                                                                      */
                /*
                * @section Csi2_c_REF_1
                * Violates MISRA 2012 Advisory Directive 4.9,
                * #A function should be used in preference to a function-like macro where they are interchangeable
                * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
                * which use only UPPERCASE
                */
                CSI2_SET_REGISTRY32(&pRegs->TURNCFG,
                        MIPICSI2_TURNCFG_FORCERXMODE1_MASK |
                        MIPICSI2_TURNCFG_FORCERXMODE2_MASK |
                        MIPICSI2_TURNCFG_FORCERXMODE3_MASK |
                        MIPICSI2_TURNCFG_FORCERXMODE4_MASK, 0u);

                /* at this moment the interface is setup                                                            */
            } /* if (rez == (Std_ReturnType)E_OK)   */
        } /* if (rez == (Std_ReturnType)E_OK)       */
    } /* if if (rez == (Std_ReturnType)E_OK)        */
    return rez;
}
/* Csi2_ModuleSetup *************************/


/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

/*================================================================================================*/
/*
 * @brief       Get the real channels number of the VC..
 *
 * @param[in]   pVCState - pointer to VC driver state
 *
 * @return      The channels number
 *
 */
uint8 Csi2_GetChannelNum(const Csi2_VCDriverStateType *pVCState)
{
    uint8 rez;

    if ((pVCState->outputDataMode & (uint16)CSI2_VC_BUF_COMPLEX_DATA) != 0u)
    {
        rez = pVCState->vcParamsPtr->channelsNum * 2u;    /* complex data, so double the channel number               */
    }
    else
    {
        rez = pVCState->vcParamsPtr->channelsNum;         /* only real data, the specified number                     */
    }
    return rez;
}
/* Csi2_GetChannelNum *************************/


/*================================================================================================*/
/**
 * @brief       This function prepare the CSI2 interface to receive data
 * @details     The detailed actions done by this function :
 *                  - verify the input parameters; if any wrong parameter, error is returned
 *                  - reset the interface
 *                  - setup the CSI2 interface
 *                  - setup the interrupt vectors
 *                  - start the interface
 *                  - reset the frame counters
 *                  The function can be called at any time, if necessary.
 *                  If the result is not E_OK, the interface status is NOT_INITIALIZED,
 *                  no matter was the status at call time.
 *
 * @param[in]   unitId          - the unit id to be used
 * @param[in]   setupParamPtr   - pointer to the CSI2 Driver setup structure
 *
 * @return      Std_ReturnType - success or error, detailed status information reported on Det.
 *
 * @pre         It must be called in the following situations:
 *              - at application start (e.g. after boot)
 *              - whenever the system parameters described in setupParamPtr need to be changed
 *              - every time the Csi2_GetInterfaceStatus returns an error
 *
 */
Std_ReturnType Csi2_Setup(const Csi2_UnitIdType unitId, const Csi2_SetupParamsType *setupParamPtr)
{
#if (CSI2_POWER_ON_OFF_USAGE == STD_ON)
    Csi2_SetupParamsType    *pStaticParams;
    #if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
        Csi2_VCParamsType       *pStaticAuxParams;
    #endif
    #if (CSI2_METADATA_DATA_USAGE == STD_ON)
        Csi2_MetaDataParamsType *pStaticMdParams;
    #endif
    #ifdef linux
        Csi2_VCDriverStateType  *pVcDriverState;
        uint64_t     dataRange;
    #endif
#endif
#if (CSI2_FRAMES_COUNTER_USED == STD_ON)
    volatile uint32         *pFramesCntr;
#endif
    uint32                  i;
    Csi2_VCParamsType       *pStaticVcParams;
    Csi2_DriverParamsType   *pDrvParams;

    Std_ReturnType  rez = (Std_ReturnType)E_OK;                     /*call result, intialized as good               */

    CSI2_TRACE(RSDK_TRACE_EVENT_FUNC_START, (uint16_t)RSDK_TRACE_DBG_CSI2_INIT, (uint32_t)CSI2_SEQ_BEGIN);
#if (CSI2_DEV_ERROR_DETECT == STD_ON)
    /* Check the parameters                                                                                         */
    rez = Csi2_CheckSetupParams(unitId, setupParamPtr);
    if (rez == (Std_ReturnType)E_OK)
    {       /* correct parameters, continue     */
#endif /* (CSI2_DEV_ERROR_DETECT == STD_ON)     */

        /* initialize the driver parameters pointer and the VC specific parameters, needed at run-time    */
        pDrvParams = &gCsi2Settings[(uint8)unitId];
        pStaticVcParams = &gsCsi2VCParamCopy[(uint8)unitId][0u];
    #if (CSI2_FRAMES_COUNTER_USED == STD_ON)
        pFramesCntr = &gsCsi2FramesCounter[(uint8)unitId][0];
    #endif
        for (i = 0; i < (uint32)CSI2_MAX_VC; i++)
        {
            if (setupParamPtr->vcConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR)
            {
                /* copy the VC parameters                                                                       */
                pStaticVcParams[i] = *setupParamPtr->vcConfigPtr[i];
                pDrvParams->workingParamVC[i].vcParamsPtr = &pStaticVcParams[i];
            }
            else
            {
                pDrvParams->workingParamVC[i].vcParamsPtr = (Csi2_VCParamsType*)NULL_PTR;
            }
        }


#if (CSI2_POWER_ON_OFF_USAGE == STD_ON)

        pStaticParams = &gsCsi2UnitParamCopy[(uint8)unitId];
        #if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
            pStaticAuxParams = &gsCsi2AuxParamCopy[(uint8)unitId][0u];
        #endif
        #if (CSI2_METADATA_DATA_USAGE == STD_ON)
            pStaticMdParams = &gsCsi2MdParamCopy[(uint8)unitId][0u];
        #endif
        /* copy the current parameters for future use, if it will be required                                       */
        if (pStaticParams != setupParamPtr)
        {
            *pStaticParams = *setupParamPtr;                        /* base setup                                   */
            for (i = 0; i < (uint32)CSI2_MAX_VC; i++)
            {
                if (setupParamPtr->vcConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR)
                {
                    pStaticParams->vcConfigPtr[i] = &pStaticVcParams[i];
                    /* clear the frames counters                                                                    */
    #if (CSI2_FRAMES_COUNTER_USED == STD_ON)
                    pFramesCntr[i] = 0;
    #endif
                }
    #if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
                if (setupParamPtr->auxConfigPtr[i] != (Csi2_VCParamsType*)NULL_PTR)
                {
                    /* copy the aux VC parameters                                                                   */
                    pStaticAuxParams[i] = *setupParamPtr->auxConfigPtr[i];
                    pStaticParams->auxConfigPtr[i] = &pStaticAuxParams[i];
                }
                else
                {
                    pStaticParams->auxConfigPtr[i] = (Csi2_VCParamsType*)NULL_PTR;
                }
    #endif
    #if (CSI2_METADATA_DATA_USAGE == STD_ON)
                if (setupParamPtr->metaDataPtr[i] != (Csi2_MetaDataParamsType*)NULL_PTR)
                {
                    /* copy the aux VC parameters                                                                   */
                    pStaticMdParams[i] = *setupParamPtr->metaDataPtr[i];
                    pStaticParams->metaDataPtr[i] = &pStaticMdParams[i];
                    pDrvParams->workingParamVC[i].metaDataUsage = 1u;
                }
                else
                {
                    pStaticParams->metaDataPtr[i] = (Csi2_MetaDataParamsType*)NULL_PTR;
                    pDrvParams->workingParamVC[i].metaDataUsage = 0u;
                }
    #endif
            }
        }
#endif /* (CSI2_POWER_ON_OFF_USAGE == STD_ON)   */
        rez = Csi2_ModuleSetup(unitId, setupParamPtr);
#ifdef linux
        if(rez == (Std_ReturnType)E_OK)
        {
            /* initialization of the data buffer virtual pointers  */
            for(i = 0; i < (uint32_t)RSDK_CSI2_MAX_VC; i++)
            {
                pVcDriverState = &gCsi2Settings[unitId].workingParamVC[i];
                if(pVcDriverState->vcParamsPtr!= NULL)
                {
                    dataRange = (uint64_t)pVcDriverState->vcParamsPtr->bufNumLines;
                    dataRange *= (uint64_t)pVcDriverState->vcParamsPtr->bufLineLen;
                    /* map the data, for statistics usage/computation                                               */
                    pVcDriverState->pVirtData = ioremap_cache((uintptr_t)pVcDriverState->vcParamsPtr->bufDataPtr,
                            dataRange + 0x100UL);
                }
            }
        }
#endif

#if (CSI2_DEV_ERROR_DETECT == STD_ON)
    }
#endif
    CSI2_TRACE(RSDK_TRACE_EVENT_FUNC_END, (uint16_t)RSDK_TRACE_DBG_CSI2_INIT, (uint32_t)CSI2_SEQ_END);

    return rez;
}
/* Csi2_Setup *************************/

/**************************************************************/
/* Using the DPHY                                             */

#if (CSI2_RX_START_STOP_USAGE == STD_ON) || (CSI2_POWER_ON_OFF_USAGE == STD_ON)
/*================================================================================================*/
/**
 * @brief       The function stop the CSI2 receive module.
 * @details     The receive module of the CSI2 interface is stopped.
 *              The receive will stop only after the end of the currently received packet.
 *
 * @param[in]   unitId    - unit identifier
 *
 * @return      Std_ReturnType - success or error, detailed status information reported on Det.
 *
 * @pre         It can be called when the reception need to be stopped. The unit must be in INITIALIZED state.
 *
 */
Std_ReturnType Csi2_RxStop(const Csi2_UnitIdType unitId)
{
    Std_ReturnType              rez;
    Csi2_DriverParamsType       *pDriverState;

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_RX_STOP, (uint32_t)CSI2_SEQ_BEGIN);
    if (((uint8)unitId >= (uint8)CSI2_MAX_UNITS))
    {
        rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_UNIT_ID, (uint8)CSI2_API_ID_RX_STOP, (uint8)CSI2_E_DRV_WRG_UNIT_ID);
        CSI2_HALT_ON_ERROR;
    }
    else
    {
        pDriverState = &gCsi2Settings[(uint8)unitId];
        if ((pDriverState->driverState != CSI2_DRIVER_STATE_ON) &&
            (pDriverState->driverState != CSI2_DRIVER_STATE_STOP))
        {
            if(pDriverState->driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)
            {
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_NOT_INIT, (uint8)CSI2_API_ID_RX_STOP, (uint8)CSI2_E_DRV_NOT_INIT);
                CSI2_HALT_ON_ERROR;
            }
            else
            {
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_STATE, (uint8)CSI2_API_ID_RX_STOP,
                                                                (uint8)CSI2_E_DRV_WRG_STATE);
                CSI2_HALT_ON_ERROR;
            }
        }
        else
        {
            rez = (Std_ReturnType)E_OK;                             /*default - success result                      */

            /*
            * @section Csi2_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(
                    &gMipiCsi2RegsPtr[(uint8)unitId]->RX_RXENABLE,
                    MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_MASK,
                    MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN((uint32)CSI2_RXEN_RXEN_DISABLED)
                    );
            if (((gMipiCsi2RegsPtr[(uint8)unitId]->RX_RXENABLE & MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_MASK) >>
                    MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_SHIFT) != (uint32)CSI2_RXEN_RXEN_DISABLED)
            {
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_HW_RESPONSE_ERROR, (uint8)CSI2_API_ID_RX_STOP,
                                                                (uint8)CSI2_E_DRV_HW_RESPONSE_ERROR);
                CSI2_HALT_ON_ERROR;
            }
            else
            {
                pDriverState->driverState = CSI2_DRIVER_STATE_STOP;  /* unit stopped                                */
            }
            if (rez != (Std_ReturnType)E_OK)
            {
                pDriverState->driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED;  /* the state for interface on error */
            }
        }
    }
    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_RX_STOP, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2_RxStop *************************/
#endif /* #if (CSI2_RX_START_STOP_USAGE == STD_ON) || (CSI2_POWER_ON_OFF_USAGE == STD_ON) */



#if (CSI2_RX_START_STOP_USAGE == STD_ON)
/*================================================================================================*/
/**
 * @brief       The function start the CSI2 receive module.
 * @details     The receive module of the CSI2 interface is started.
 *              This action must be done if a CSI2_RxStop was done before,
 *               and the reception must be done at the same parameters.
 *
 * @param[in]   unitId    - unit identifier
 *
 * @return      Std_ReturnType - success or error, detailed status information reported on Det.
 *
 * @pre         It can be called when the reception is stopped, after an previous Csi2_RxStop.
 *
 */
Std_ReturnType Csi2_RxStart(const Csi2_UnitIdType unitId)
{
    Std_ReturnType              rez;
    Csi2_DriverParamsType       *pDriverState;

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_RX_START, (uint32_t)CSI2_SEQ_BEGIN);
    if (((uint8)unitId >= (uint8)CSI2_MAX_UNITS))
    {
        rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_UNIT_ID, (uint8)CSI2_API_ID_RX_START, (uint8)CSI2_E_DRV_WRG_UNIT_ID);
        CSI2_HALT_ON_ERROR;
    }
    else
    {
        pDriverState = &gCsi2Settings[(uint8)unitId];
        rez = (Std_ReturnType)E_OK;                         /*default - success result                              */
        switch (pDriverState->driverState)
        {
            case CSI2_DRIVER_STATE_STOP:                    /* interface really stopped                             */
                /*
                * @section Csi2_c_REF_1
                * Violates MISRA 2012 Advisory Directive 4.9,
                * #A function should be used in preference to a function-like macro where they are interchangeable
                * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
                * which use only UPPERCASE
                */
                CSI2_SET_REGISTRY32(
                        &gMipiCsi2RegsPtr[(uint8)unitId]->RX_RXENABLE,
                        MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_MASK,
                        MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN((uint32)CSI2_RXEN_RXEN_ENABLED)
                        );                                  /* enable clk                                           */
                if (((gMipiCsi2RegsPtr[(uint8)unitId]->RX_RXENABLE & MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_MASK) >>
                        MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_SHIFT) != (uint32)CSI2_RXEN_RXEN_ENABLED)
                {
                    rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_HW_RESPONSE_ERROR, (uint8)CSI2_API_ID_RX_START,
                                                                (uint8)CSI2_E_DRV_HW_RESPONSE_ERROR);
                    CSI2_HALT_ON_ERROR;
                    pDriverState->driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED;  /* interface in an wrong state  */
                }
                else
                {
                    pDriverState->driverState = CSI2_DRIVER_STATE_ON;  /* interface on now                          */
                }
                break;
            case CSI2_DRIVER_STATE_ON:                      /* nothing to do, the interface is already on           */
                break;
            case CSI2_DRIVER_STATE_NOT_INITIALIZED:
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_NOT_INIT, (uint8)CSI2_API_ID_RX_START,
                                                                    (uint8)CSI2_E_DRV_NOT_INIT);
                CSI2_HALT_ON_ERROR;
                break;
            default:                                        /* wrong interface status (not init or off)             */
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_STATE, (uint8)CSI2_API_ID_RX_START,
                                                                    (uint8)CSI2_E_DRV_WRG_STATE);
                CSI2_HALT_ON_ERROR;
                break;
        }
    }
    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_RX_START, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2_RxStart *************************/

    #endif  /* (CSI2_RX_START_STOP_USAGE == STD_ON) */


    #if (CSI2_POWER_ON_OFF_USAGE == STD_ON)
/*================================================================================================*/
/**
 * @brief       The function power off the CSI2 module.
 * @details     The interface is powered off.
 *
 * @param[in]   unitId    - unit identifier
 *
 * @return      Std_ReturnType - success or error, detailed status information reported on Det.
 *
 * @pre         It can be called when the reception need to be stopped and/or the interface to be powered off.
 *              The unit must be in INITIALIZED state.
 *
 */
Std_ReturnType Csi2_PowerOff(const Csi2_UnitIdType unitId)
{
    Std_ReturnType              rez;
    Csi2_DriverParamsType       *pDriverState;

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_POWER_OFF, (uint32_t)CSI2_SEQ_BEGIN);
    rez = (Std_ReturnType)E_OK;
    if (((uint8)unitId >= (uint8)CSI2_MAX_UNITS))
    {
        rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_UNIT_ID, (uint8)CSI2_API_ID_POWER_OFF, (uint8)CSI2_E_DRV_WRG_UNIT_ID);
        CSI2_HALT_ON_ERROR;
    }
    else
    {
        pDriverState = &gCsi2Settings[(uint8)unitId];
        if (pDriverState->driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED)
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_NOT_INIT, (uint8)CSI2_API_ID_POWER_OFF, (uint8)CSI2_E_DRV_NOT_INIT);
            CSI2_HALT_ON_ERROR;
        }
        else
        {   /* only in an appropriate state     */
            if (pDriverState->driverState != CSI2_DRIVER_STATE_OFF)
            {
                rez = Csi2_RxStop(unitId);                              /* first - stop the clock                   */
            }
            if (rez == (Std_ReturnType)E_OK)
            {
                gMipiCsi2RegsPtr[(uint8)unitId]->DPHY_RSTCFG = 0u;        /* poweroff PHY                             */
                if ((gMipiCsi2RegsPtr[(uint8)unitId]->DPHY_RSTCFG & MIPICSI2_DPHY_RSTCFG_RSTZ_MASK) != 0u)
                {
                    rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_HW_RESPONSE_ERROR, (uint8)CSI2_API_ID_POWER_OFF,
                                                                    (uint8)CSI2_E_DRV_HW_RESPONSE_ERROR);
                    CSI2_HALT_ON_ERROR;
                }
                else
                {
                    pDriverState->driverState = CSI2_DRIVER_STATE_OFF;  /* the new state for interface              */
                }
            }
            if (rez != (Std_ReturnType)E_OK)
            {
                pDriverState->driverState = CSI2_DRIVER_STATE_NOT_INITIALIZED;  /* the state for interface on error */
            }
        }
    }
    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_POWER_OFF, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2_PowerOff *************************/


/*================================================================================================*/
/**
 * @brief       The function power on the CSI2 module.
 * @details     The interface is powered on.
 *              This action must be done only after a Csi2PowerOff was done before.
 *              After this call, the last setup parameters will be used to resetup the interface.
 *
 * @param[in]   unitId    - unit identifier
 *
 * @return      Std_ReturnType - success or error, detailed status information reported on Det.
 *
 * @pre         It can be called when the unit is powered off, or a restart is intended.
 *              If the Csi2_Setup call was not done before, an error will be reported, else
 *               the unit will be resetup using the previous setup parameters.
 */
Std_ReturnType Csi2_PowerOn(const Csi2_UnitIdType unitId)
{
    Std_ReturnType              rez;

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_POWER_ON, (uint32_t)CSI2_SEQ_BEGIN);
    if (((uint8)unitId >= (uint8)CSI2_MAX_UNITS))
    {
        rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_UNIT_ID, (uint8)CSI2_API_ID_POWER_ON, (uint8)CSI2_E_DRV_WRG_UNIT_ID);
        CSI2_HALT_ON_ERROR;
    }
    else
    {
        if (gCsi2Settings[(uint8)unitId].driverState == CSI2_DRIVER_STATE_OFF)
        {   /* only in an appropriate state     */
            rez = Csi2_ModuleSetup(unitId,
                    &gsCsi2UnitParamCopy[(uint8)unitId]
                                         );    /* return the init status       */
        }
        else
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_NOT_INIT, (uint8)CSI2_API_ID_POWER_ON, (uint8)CSI2_E_DRV_NOT_INIT);
            CSI2_HALT_ON_ERROR;
        }
    }
    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_POWER_ON, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2_PowerOn *************************/

    #endif  /* (CSI2_POWER_ON_OFF_USAGE == STD_ON)  */


    #if (CSI2_SECONDARY_FUNCTIONS_USAGE == STD_ON)

/*================================================================================================*/
/**
 * @brief       The function ask for the current status of the interface.
 * @details     The function return the status of the interface at the moment of the call.
 *
 * @param[in]   unitId    - unit identifier
 *
 * @return      Csi2_LaneStatusType - the current status of lane 0
 *
 * @pre         It can be called at any time, after a successful setup, to check the current status of the interface.
 *              Depending of the returned status, some times is necessary to loop on this call to really
 *              understand how interface is working.
 *
 */
Csi2_LaneStatusType Csi2_GetInterfaceStatus(const Csi2_UnitIdType unitId)
{
    Std_ReturnType      rez = (Std_ReturnType)E_OK;
    Csi2_LaneStatusType ret = CSI2_LANE_STATE_ERR;
    Csi2_DriverParamsType *pState;
    pState = &gCsi2Settings[unitId];

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_IFACE_STAT, (uint32_t)CSI2_SEQ_BEGIN);
    if (((uint8)unitId >= (uint8)CSI2_MAX_UNITS))
    {
        rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_UNIT_ID, (uint8)CSI2_API_ID_GET_INTERFACE,
                                                                    (uint8)CSI2_E_DRV_WRG_UNIT_ID);
        CSI2_HALT_ON_ERROR;
    }
    if (rez == (Std_ReturnType)E_OK)
    {
        if(
                (gMipiCsi2RegsPtr[(uint8)unitId] == (GENERIC_CSI2_Type*)NULL_PTR)
                || (pState->driverState == CSI2_DRIVER_STATE_NOT_INITIALIZED))
        {
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_NOT_INIT, (uint8)CSI2_API_ID_GET_INTERFACE,
                                                                   (uint8)CSI2_E_DRV_NOT_INIT);
            CSI2_HALT_ON_ERROR;
        }
        else
        {
            if (pState->driverState == CSI2_DRIVER_STATE_STOP)
            {
                ret = CSI2_LANE_STATE_STOP;
            }
            else if (pState->driverState == CSI2_DRIVER_STATE_ON)
            {
                ret = CSI2_LANE_STATE_ON;
            }
            else
            {
                ret = CSI2_LANE_STATE_OFF;
            }
        }
    }
    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_IFACE_STAT, (uint32_t)CSI2_SEQ_END);
    return ret;                 /* The interface status is the driver status                                        */
}
/* Csi2_GetInterfaceStatus *************************/

/*================================================================================================*/
/**
 * @brief       The function ask for the current status of the specific lane.
 * @details     The function return the status of the interface or specific lane at the moment of the call.
 *
 * @param[in]   unitId    - unit identifier
 * @param[in]   laneId    - lane identifier
 *
 * @return      Csi2_LaneStatusType - the current status of lane 0
 *
 * @pre         It can be called at any time, to check the current status of the interface/lane.
 *              Depending of the returned status, some times is necessary to loop on this call to
 *              really understand how lane is working.
 */
Csi2_LaneStatusType Csi2_GetLaneStatus(const Csi2_UnitIdType unitId, const Csi2_LaneIdType laneId)
{
    Std_ReturnType      rez = (Std_ReturnType)E_OK;
    static uint32       regStat;
    Csi2_LaneStatusType ret = CSI2_LANE_STATE_ERR;

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_LANE_STAT, (uint32_t)CSI2_SEQ_BEGIN);
    if (((uint8)unitId >= (uint8)CSI2_MAX_UNITS))
    {
        rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_UNIT_ID, (uint8)CSI2_API_ID_GET_INTERFACE,
                                                                    (uint8)CSI2_E_DRV_WRG_UNIT_ID);
        CSI2_HALT_ON_ERROR;
    }
    if (rez == (Std_ReturnType)E_OK)
    {
        switch(gCsi2Settings[unitId].driverState)
        {
            case CSI2_DRIVER_STATE_NOT_INITIALIZED:
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_NOT_INIT, (uint8)CSI2_API_ID_GET_INTERFACE,
                                                                    (uint8)CSI2_E_DRV_NOT_INIT);
                CSI2_HALT_ON_ERROR;
                break;
            case CSI2_DRIVER_STATE_OFF:
                ret = CSI2_LANE_STATE_OFF;
                rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_WRG_STATE, (uint8)CSI2_API_ID_GET_INTERFACE,
                                                                    (uint8)CSI2_E_DRV_WRG_STATE);
                CSI2_HALT_ON_ERROR;
                break;
            case CSI2_DRIVER_STATE_STOP:
                ret = CSI2_LANE_STATE_STOP;
                rez = (Std_ReturnType)E_NOT_OK;
                break;
            default:
                ret = CSI2_LANE_STATE_STOP;
                break;
        }
    }
    if (rez == (Std_ReturnType)E_OK)
    {
        regStat = gMipiCsi2RegsPtr[unitId]->RX_RXNULANE;
        if ((uint32)laneId < regStat)
        {
            ret = CSI2_LANE_STATE_ON;                                   /* default - line on                        */
            regStat = gMipiCsi2RegsPtr[unitId]->RX_LANCS[(uint8)laneId];
            if ((regStat & CSI2_LAN_CS_MARK) != 0u)
            {
                ret = CSI2_LANE_STATE_MARK;                             /* default - line on                        */
            }
            if ((regStat & CSI2_LAN_CS_ULPA) != 0u)
            {
                ret = CSI2_LANE_STATE_ULPA;
            }
            if ((regStat & CSI2_LAN_CS_STOP) != 0u)
            {
                ret = CSI2_LANE_STATE_STOP;
            }
            if ((regStat & CSI2_LAN_CS_RXACTH) != 0u)
            {
                ret = CSI2_LANE_STATE_REC;
            }
            if ((regStat & CSI2_LAN_CS_RXVALH) != 0u)
            {
                ret = CSI2_LANE_STATE_VRX;
            }
        }
        else
        {
            ret = CSI2_LANE_STATE_ERR;
            rez = CSI2_REPORT_ERROR(RSDK_CSI2_DRV_INVALID_LANES_NR, (uint8)CSI2_API_ID_GET_INTERFACE,
                                                                (uint8)CSI2_E_DRV_INVALID_LANES_NR);
            CSI2_HALT_ON_ERROR;
        }
    }
    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_LANE_STAT, (uint32_t)CSI2_SEQ_END);
    return ret;
}
/* Csi2_GetLaneStatus *************************/
    #endif  /* (CSI2_SECONDARY_FUNCTIONS_USAGE == STD_ON)   */

/* Using the DPHY                                             */
/**************************************************************/

/*================================================================================================*/
/**
 * @brief       Procedure to get the real buffer length for a line/chirp.
 * @details     The procedure compute the necessary buffer length according to the input parameters.
 * @note        Because the length include the line/chirp statistics, autoStatistics must be 1 even only one channel
 *              in the entire Virtual Channel need the autoStatistics computation.
 *
 * @param[in]   dataType        Type of data to receive, normally CSI2_DATA_TYPE_RAW12 for radar
 * @param[in]   nrChannels      Number of channels, 1 ... 8; usually this number is
 *                              the number of active antennas, multiply by 2 if complex (Re & Im) acquisition
 * @param[in]   samplesPerChirp Samples per chirp and channel
 * @param[in]   autoStatistics  Value to inform that statistics must be added at the end of the chirp
 *
 * @return      The necessary memory amount for one line (chirp) if not 0. <br>
 *              For wrong input, result is 0.
 *
 */
uint32 Csi2_GetBufferRealLineLen(const Csi2_DataStreamType dataType, const uint32 numChannels,
        const uint32 samplesPerChirp
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
        , const Csi2_StatisticsType autoStatistics
#endif /* (CSI2_STATISTIC_DATA_USAGE == STD_ON) */
        )
{
    uint32 rez;

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_REAL_LEN, (uint32_t)CSI2_SEQ_BEGIN);
    if (
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
            ((uint8)autoStatistics >= (uint8)CSI2_STAT_MAX) ||
#endif
            ((uint32)numChannels > (uint32)CSI2_MAX_CHANNEL) ||
            ((uint32)numChannels == (uint32)0) ||
            ((uint32)samplesPerChirp == (uint32)0)      )
    {
        rez = 0u;
    }
    else
    {
        rez = Csi2_GetOutBytesPerSample(dataType);  /* get a magic number for the length of each sample         */
        if (rez != (uint32)0)
        {
            /* good base data type
             * all parameters are good, do the final computation                                                */
            rez *= numChannels * samplesPerChirp;
#if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
            if (dataType > CSI2_DATA_TYPE_AUX_0_NO_DROP)
            {       /* it is auxiliary data, not regular data   */
                switch ((uint32)dataType & (~((uint32)CSI2_NORM_DTYPE_MASK)))
                {
                    default:                        /* not recognized auxiliary mode => error                   */
                        rez = 0u;
                        break;
                    case (uint32)CSI2_DATA_TYPE_AUX_0_NO_DROP:
                        break;
                    case (uint32)CSI2_DATA_TYPE_AUX_0_DR_1OF2:
                    case (uint32)CSI2_DATA_TYPE_AUX_1_NO_DROP:
                        rez = (rez + 1u) / 2u;
                        break;
                    case (uint32)CSI2_DATA_TYPE_AUX_0_DR_3OF4:
                        rez = (rez + 3u) / 4u;
                        break;
                }
            }
            else
#endif
            {   /* it is regular data
                 * add the required space for statistics                                                        */
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
                rez += (uint32)autoStatistics * (uint32)CSI2_LINE_STAT_LENGTH;  /* adding the stats             */
#endif

                /*
                * @section Csi2_c_REF_1
                * Violates MISRA 2012 Advisory Directive 4.9,
                * #A function should be used in preference to a function-like macro where they are interchangeable
                * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
                * which use only UPPERCASE
                */
                rez = CSI2_UPPER_ALIGN_TO_(rez, (uint32)16);    /* align to upper 16 bytes                      */
            }
        }  /* if(rez != 0)  */
    }
    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_REAL_LEN, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2_GetBufferRealLineLen *************************/


#if (CSI2_FRAMES_COUNTER_USED == STD_ON)

/*================================================================================================*/
/**
 * @brief       The function return the current frames counter
 * @details     The application can read the counter of frames, to decide how to manage the data. The counter is
 *              increased after handling the Frame End interrupt request. <br>
 *              A correct unitId and vcId must be provided.
 *
 * @param[in]   unitId    - unit identifier
 * @param[in]   vcId      - Virtual Channe identifier
 *
 * @return      uint32    - the current frames counter, 0xffffffff is not a valid value
 *
 */
uint32 Csi2_GetFramesCounter(const Csi2_UnitIdType unitId, const Csi2_VirtChnlIdType vcId)
{
    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_FRM_CNTR, (uint32_t)CSI2_SEQ_BEGIN);

    uint32 rez = 0xffffffffu;
    if (((uint8)unitId < (uint8)CSI2_MAX_UNITS) && ((uint8)vcId < (uint8)CSI2_MAX_VC))
    {
        rez = gsCsi2FramesCounter[(uint8)unitId][(uint8)vcId];
    }

    CSI2_TRACE(RSDK_TRACE_EVENT_DBG_INFO, (uint16_t)RSDK_TRACE_DBG_CSI2_GET_FRM_CNTR, (uint32_t)CSI2_SEQ_END);
    return rez;
}
/* Csi2_GetFramesCounter *************************/
#endif  /* #if (CSI2_FRAMES_COUNTER_USED == STD_ON)     */


#ifdef __cplusplus
}
#endif

/** @} */



