/*
* Copyright 2022 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CSI2_H
#define CSI2_H


/**
*   @file
*   @implements Csi2.h_Artifact
*
*   @addtogroup CSI2_ASR
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*
* @page misra_violations MISRA-C:2012 violations
*
* @section Csi2_h_REF_1
* Violates MISRA 2012 Advisory Rule 20.1, #Include directives should only be preceded by preprocessor directives or comments.
* <MA>_MemMap.h is included after each section define in order to set the current memory section as defined by AUTOSAR.
*/


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Csi2_Cfg.h"
#include "Csi2_Types.h"
#include "Csi2_Irq.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/


/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/*
* @brief Development error codes (passed to DET).
*/

    /**
    * @brief API request called with an invalid parameter (Nullpointer).
    * */
    #define CSI2_E_PARAM_POINTER                ((uint8)0x01U)

    /**
    * @brief API request called with invalid parameter (invalid value).
    * */
    #define CSI2_E_PARAM_VALUE                  ((uint8)0x02U)

    /**
    * @brief API request called with invalid parameter (out of range).
    * */
    #define CSI2_E_PARAM_HANDLE                 ((uint8)0x03U)

    /**
    * @brief Setup of Csi2 Driver failed.
    * */
    #define CSI2_E_SETUP_FAILED                 ((uint8)0x04U)

    /**
    * @brief Incorrect driver status.
    * */
    #define CSI2_E_WRONG_STATE                  ((uint8)0x05U)

    /**
    * @brief Hardware error.
    * */
    #define CSI2_E_HW_ERROR                     ((uint8)0x06U)


/* update a MIPI-CSI2 32 bits registry                              */
#define CSI2_SET_REGISTRY32(registryPtr, alignedMask, alignedValue) \
                *(registryPtr) = (((*(registryPtr)) & (~((uint32)alignedMask))) | ((uint32)alignedValue))

/* update a MIPI-CSI2 8 bits registry                               */
#define CSI2_SET_REGISTRY8(registryPtr, alignedMask, alignedValue) \
                *(registryPtr) = (((*(registryPtr)) & (~((uint8)alignedMask))) | ((uint8)alignedValue))


/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
    extern Csi2_DriverParamsType gCsi2Settings[CSI2_MAX_UNITS];
    extern volatile GENERIC_CSI2_Type *gpMipiCsi2Regs[CSI2_MAX_UNITS];
    extern volatile uint32 gsCsi2FramesCounter[CSI2_MAX_UNITS][CSI2_MAX_VC];


/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/


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
 * @return      Std_ReturnType  - success or error, detailed status information reported on Det.
 *
 * @pre         It must be called in the following situations:
 *              - at application start (e.g. after boot)
 *              - whenever the system parameters described in setupParamPtr need to be changed
 *              - every time the Csi2_GetInterfaceStatus returns an error
 *
 */
Std_ReturnType Csi2_Setup(const Csi2_UnitIdType unitId, const Csi2_SetupParamsType *setupParamPtr);

#if (CSI2_RX_START_STOP_USAGE == STD_ON)
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
Std_ReturnType Csi2_RxStop(const Csi2_UnitIdType unitId);


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
Std_ReturnType Csi2_RxStart(const Csi2_UnitIdType unitId);
#endif  /* (CSI2_RX_START_STOP_USAGE == STD_ON)     */


#if (CSI2_POWER_ON_OFF_USAGE == STD_ON)
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
Std_ReturnType Csi2_PowerOff(const Csi2_UnitIdType unitId);


/**
 * @brief       The function power on the CSI2 module.
 * @details     The interface is powered on.
 *              This action must be done only after a Csi2PowerOff was done before.
 *              After this call, the last setup parameters will be used to reinitialize the interface.
 *
 * @param[in]   unitId    - unit identifier
 *
 * @return      Std_ReturnType - success or error, detailed status information reported on Det.
 *
 * @pre         It can be called when the unit is powered off, or a restart is intended.
 *              If the Csi2_Setup call was not done before, an error will be reported, else
 *               the unit will be resetup using the previous setup parameters.
 */
Std_ReturnType Csi2_PowerOn(const Csi2_UnitIdType unitId);
#endif  /* (CSI2_POWER_ON_OFF_USAGE == STD_ON)      */


#if (CSI2_SECONDARY_FUNCTIONS_USAGE == STD_ON)
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
Csi2_LaneStatusType Csi2_GetInterfaceStatus(const Csi2_UnitIdType unitId);


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
Csi2_LaneStatusType Csi2_GetLaneStatus(const Csi2_UnitIdType unitId, const Csi2_LaneIdType laneId);

#endif  /* (CSI2_SECONDARY_FUNCTIONS_USAGE == STD_ON)       */

/**
 * @brief       Procedure to get the real buffer length for a line/chirp.
 * @details     The procedure compute the necessary buffer length according to the input parameters.
 * @note        Because the length include the line/chirp statistics, autoStatistics must be 1 even only one channel
 *              in the entire Virtual Channel need the autoStatistics computation.
 *
 * @param[in]   dataType        Type of data to receive, normally RSDK_CSI2_DATA_TYPE_RAW12 for radar
 * @param[in]   numChannels     Number of channels, 1 ... 8; usually this number is
 *                              the number of active antennas, multiply by 2 if complex (Re & Im) acquisition
 * @param[in]   samplesPerChirp Samples per chirp and channel
 * @param[in]   autoStatistics  Value to inform that statistics must be added at the end of the chirp
 *
 * @return      The necessary memory amount for one line (chirp) if not 0. <br>
 *              Wrong input parameter if 0.
 *
 */
uint32 Csi2_GetBufferRealLineLen(const Csi2_DataStreamType dataType, const uint32 numChannels,
        const uint32 samplesPerChirp
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
        , const Csi2_StatisticsType autoStatistics
#endif
        );


#if (CSI2_FRAMES_COUNTER_USED == STD_ON)

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
uint32 Csi2_GetFramesCounter(const Csi2_UnitIdType unitId, const Csi2_VirtChnlIdType vcId);
#endif


/**
 * @brief       Get the real channels number of the VC..
 *
 * @param[in]   pVCState - pointer to VC driver state
 *
 * @return      The channels number
 *
 */
uint8 Csi2_GetChannelNum(const Csi2_VCDriverStateType *pVCState);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CSI2_H */
