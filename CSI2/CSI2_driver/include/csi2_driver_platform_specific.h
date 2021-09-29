/*
* Copyright 2019-2020 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CSI2_DRIVER_PLATFORM_SPECIFIC_H
#define CSI2_DRIVER_PLATFORM_SPECIFIC_H

/**
 * @file           csi2_driver_internals.h
 *
 */

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#ifdef S32R274
#include "rsdk_S32R274.h"
#elif defined(S32R372)
#include "rsdk_S32R372.h"
#elif defined(S32R294)
#include "rsdk_S32R294.h"
#elif defined(S32R45)
#include "rsdk_S32R45.h"
#else
#error "Platform not defined, or incorrect definition."
#endif

#include "rsdk_csi2_driver_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                          CONSTANTS
 ==================================================================================================*/
// clang-format off
/**
 * @brief          Masks and values for CSI2 registters.
 * @details        As defined in >> 49.8 Memory map and register definition,
 *                   from S32R274 Reference Manual Rev. 2, 10/2016
 *                   and according to CSI2 registers definition in S32R274.h
 *
 */

#define CSI2_PHYC_RTERM_SEL_LPR     0   // MIPICSI2_PHYC register, RTERM_SEL field, Low Power Receive.
#define CSI2_PHYC_RTERM_SEL_LPCD    1   // MIPICSI2_PHYC register, RTERM_SEL field, Low Power Contention Detector.
#define CSI2_PHYC_PDRX_POWER_ON     0   // MIPICSI2_PHYC register, PDRX field, Power down disabled.
#define CSI2_PHYC_PDRX_POWER_OFF    1   // MIPICSI2_PHYC register, PDRX field, Power down enabled.

#define CSI2_CLKCS_HSRA_OFF         0   // MIPICSI2_CLKCS register, HSRA field, DDR clock not being received
                                        //  on the clock lane currently.
#define CSI2_CLKCS_HSRA_ON          1   // MIPICSI2_CLKCS register, HSRA field, Clock lane is receiving DDR clock.

#define CSI2_LAN_CS_MARK        (1UL << 5u) // MIPICSI2_LANxCS register, DxULMA field, Data lane in Mark state.
#define CSI2_LAN_CS_ULPA        (1UL << 4u) // MIPICSI2_LANxCS register, DxULPA field, Data lane ULPS active.
#define CSI2_LAN_CS_STOP        (1UL << 3u) // MIPICSI2_LANxCS register, DxSTOP field, Data lane in stop state.
                                            //  data reception ongoing.
#define CSI2_LAN_CS_RXACTH      (1UL << 1u) // MIPICSI2_LANxCS register, RXACTH field, High Speed data reception on.
                                            //  data being driven.
#define CSI2_LAN_CS_RXVALH      (1UL << 0u) // MIPICSI2_LANxCS register, RXVALH field, Valid High Speed data on.

#define CSI2_RESCS_CALIB_BIT    (1UL << 5u) // MIPICSI2_RESCS register, Calibration complete field
#define CSI2_RESCS_NOCAL_BIT    (1UL << 0u) // MIPICSI2_RESCS register, NOCAL field, manually done calibration

#define CSI2_RESCS_NOCAL_AUTO       0U      // request for autocalibration
#define CSI2_RESCS_NOCAL_MAN        1U      // manual calibration
#define CSI2_RESCS_CALCOM_COMPLETE  1U      // calibration complete

#define CSI2_SR_SOFRST_MASK     (1UL << 31u)    // MIPICSI2_SR register, SOFRST field, soft reset requested.
#define CSI2_SR_GNSPR_MASK      (1UL << 11u)    // MIPICSI2_SR register, GNSPR field, No Generic Short Packet received.

#define CSI2_ERRPPREG_INVID_BIT     0x20u   // MIPICSI2_ERRPPREG register, INVID field, Invalid data type detected.
#define CSI2_ERRPPREG_CRCERR_BIT    0x10u   // MIPICSI2_ERRPPREG register, CRCERR field, CRC error.
#define CSI2_ERRPPREG_ERFDAT_BIT    8u      // MIPICSI2_ERRPPREG register, ERFDAT field, Frame data error.
#define CSI2_ERRPPREG_ERFSYN_BIT    4u      // MIPICSI2_ERRPPREG register, ERFSYN field, Frame data synchro error.
#define CSI2_ERRPPREG_ECCTWO_BIT    2u      // MIPICSI2_ERRPPREG register, ECCTWO field, ECC double bit error.
#define CSI2_ERRPPREG_ECCONE_BIT    1u      // MIPICSI2_ERRPPREG register, ECCONE field, ECC single bit error.

#define CSI2_ERRPHY_ERRSY_BIT       1u      // MIPICSI2_ERRPHY register, lane synchronization error
#define CSI2_ERRPHY_NOSYN_BIT       2u      // MIPICSI2_ERRPHY register, no lane synchronization error
#define CSI2_ERRPHY_ERRESC_BIT      4u      // MIPICSI2_ERRPHY register, lane escape sequence error
#define CSI2_ERRPHY_ERRSYES_BIT     8u      // MIPICSI2_ERRPHY register, lane escape synchronization error
#define CSI2_ERRPHY_ERRCTRL_BIT     0x10u   // MIPICSI2_ERRPHY register, lane control error
#define CSI2_ERRPHY_LANE_SHIFT      5u      // MIPICSI2_ERRPHY register, shift between two consecutive lanes

#define CSI2_RXEN_RXEN_DISABLED     0   // MIPICSI2_RXEN register, RXEN field, Rx disabled.
#define CSI2_RXEN_RXEN_ENABLED      1   // MIPICSI2_RXEN register, RXEN field, Rx enabled.

#define CSI2_INTRS_LINCNTERR_BIT    8u  // MIPICSI2_INTRS register, LINCNTERR field, no line count error.
#define CSI2_INTRS_LINLENERR_BIT    4u  // MIPICSI2_INTRS register, LINLENERR bit
#define CSI2_INTRS_FRAME_END_BIT    2u  // MIPICSI2_INTRS register, frame end bit
#define CSI2_INTRS_FRAME_STR_BIT    1u  // MIPICSI2_INTRS register, frame start bit

#define CSI2_INTERRUPT_ENABLE       1   // Activation of interrupt generation for the specific error.
#define CSI2_INTERRUPT_DISABLE      0   // Disactivation of interrupt generation for the specific error.
#define CSI2_CLEAR_ONE_BIT          1   // Clear one data bit in special register. */

#define CSI2_ERROR_REPORTED         1   // General value to signal an error in interface register.

#define RSDK_CSI2_FLUSH_CNT_FREQ_LIMIT      200U    // the limit for flash_cnt limit (200MHz)
#define RSDK_CSI2_LINE_STAT_LENGTH          80U     // the length of statistic data
#define RSDK_CSI2_INVALID_UINT8             0xffu   // the biggest uint8_t
#define RSDK_CSI2_MAX_WAIT_FOR_STOP         1000000 // maximum time to wait for stop state
#define RSDK_CSI2_FIRST_BUF_LINE_NUM        1u      // the invalid value for the buffer line
#define RSDK_CSI2_CHIRP_NOT_STARTED         0u      // value for first chirp line received

#define RSDK_CSI2_BUF_CFG_OFFSET    0x30u           // offset between two similar buffer config registry
#define RSDK_CSI2_VC_CFG_OFFSET     0x10u           // offset between two similar VC config registry

#define RSDK_CSI2_SOFTRESET_BIT     0x80000000u     // the 32 bits value to reset all registry

#define RSDK_CSI2_5TH_CHANNEL_ON    0x400u          // bit to enable the fifth channel
#define RSDK_CSI2_NORM_DTYPE_MASK   0x3fu           // the mask for the normal data types to be received
#define RSDK_CSI2_MAX_VC_MASK       0x3u            // the mask for the normal data types to be received


// upper and lower alignment of data to specified value
#define RSDK_CSI2_UPPER_ALIGN_TO_(x, y)        ((((x) + (y) - 1u) / (y)) * (y))

/*==================================================================================================
 *                                      DEFINES AND MACROS
 ==================================================================================================*/
#define UNUSED_ARG(x)       ((void)x)

#ifndef TRACE_ENABLE
#define RsdkTraceLogEvent(a, b, c)
#endif

#ifdef S32R45
#define MIPICSI2_REG_STRUCT MIPICSI2_tag
#else
#define MIPICSI2_REG_STRUCT CSI_tag
#endif

/*==================================================================================================
 *                                             ENUMS
 ==================================================================================================*/
typedef enum {
    CSI2_DRIVER_STATE_NOT_INITIALIZED = 0,      // driver not initialized
    CSI2_DRIVER_STATE_ON,                       // driver initialized
    CSI2_DRIVER_STATE_OFF,                      // driver off
    CSI2_DRIVER_STATE_STOP,                     // driver stopped
}csi2DriverState_t;

typedef enum {
    CSI2_INTRS_FS = 0u,                         // FrameEnd bit
    CSI2_INTRS_FE,                              // FrameStart bit
    CSI2_INTRS_GNSP,                            // Generic Short Packet bit
    CSI2_INTRS_MAX                              // masks limit
}csi2IntrsBit_t;

/*==================================================================================================
 *                                STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
/**
 * @brief       Structure for channel statistics, added to the chirp data by the hardware.
 * @details     It is added at the end of each chirp.
 *
 */
typedef struct __attribute__((__packed__)) {
    uint32_t channelSumScr;         // chirp channel sum, scrambled :
                                    //  in memory (LSB+1)(LSB)(MSB)(MSB-1)
                                    //  must be translated to (signed)(MSB,MSB-1,LSB+1,LSB)
    int16_t channelMin;             // chirp channel min
    int16_t channelMax;             // chirp channel max
    uint16_t channelBitToggle;      // chirp channel bit toggle
}rsdkCsi2ChChirpStat_t;


/**
 * @brief       Structure for channel statistics, for the entire frame.
 * @details     This is for driver internal purposes at frame level.
 *
 */
typedef struct {
    int64_t channelSum;             // chirp channel sum
    int16_t channelMin;             // chirp channel min
    int16_t channelMax;             // chirp channel max
    uint16_t channelBitToggle;      // chirp channel bit toggle
    int16_t channelDC;              // current channel DC offset
    int16_t reqChannelDC;           // current channel DC offset
}rsdkCsi2ChFrameStat_t;


/**
 * @brief       Structure for VC driver parameters.
 * @details     Internal parameters for easy driver usage.
 *
 */
typedef struct {
    uint8_t     eventsMask;                     // Rx events to be called back
    uint16_t    outputDataMode;                 // the mode data is output in the buffer and other information
                                                // about input data, see rsdk_csi2_driver_api.h file :
                                                // "various parameters concerning input data for outputDataMode"
    uint16_t    lastReceivedBufLine;            // the last buffer line written by the interface
    uint16_t    lastReceivedChirpLine;          // the last chirp line received
#ifdef linux
    void        *pVirtData;                     // virtual memory pointer to buf data, from kernel
#endif
    rsdkCsi2VCParams_t      *pVCParams;         // pointer to actual VC initialization params
    rsdkCsi2ChFrameStat_t   statDC[RSDK_CSI2_MAX_CHANNEL];      // statistics for DC computation for all channels
}rsdkCsi2VCDriverState_t;


/**
 * @brief       Structure which keep the necessary parameters for run-time.
 *
 */
typedef struct {
    // unit level parameters
    csi2DriverState_t           driverState;        // driver state
    rsdkCsi2AutoDCComputeTime_t statisticsFlag;     // flag to process or not the channels statistics
    rsdkCsi2IsrCb_t             pCallback[RSDK_CSI2_MAX_IRQ_ID];    // the necessary callbacks :
                                                    // - errors in receive path
                                                    // - errors in datapath level
                                                    // - events
                                                    // - turnaround and tx errors/events

    // VC level working params
    rsdkCsi2VCDriverState_t    workingParamVC[RSDK_CSI2_MAX_VC];
}rsdkCsi2DriverParams_t;


/*==================================================================================================
 *                                GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/
// clang-format on

// settings to be kept during the execution
// only run-time necessary parameters are kept
#if (defined(S32R294) || defined(S32R45)) && (!defined(SAF85XX))
extern rsdkCsi2DriverParams_t gCsi2Settings[RSDK_CSI2_MAX_UNITS];
extern volatile struct MIPICSI2_REG_STRUCT *gpMipiCsi2Regs[RSDK_CSI2_MAX_UNITS];
#else
extern rsdkCsi2DriverParams_t gCsi2Settings;
extern volatile struct MIPICSI2_REG_STRUCT *gpMipiCsi2Regs;
#endif
/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
 ==================================================================================================*/

/**
 * @brief       The function called from API to initialize the module.
 * @details     The function check the parameters, initialize the interface and
 *              save a copy of the parameters for future use.
 *
 * @param[in]   unitId         - the unit number, RSDK_CSI2_UNIT_1 ... MAX
 * @param[in]   pCsi2InitParam - pointer to CSI2 initialization parameters
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 *          other error values, according to the failure
 */
rsdkStatus_t Csi2PlatformModuleInit(const rsdkCsi2UnitId_t unitId, const rsdkCsi2InitParams_t *pCsi2InitParam);

/**
 * @brief          The function stop the CSI2 receive module.
 * @details        The receive module of the CSI2 interface is stopped.
 *                 The action succeed only if the interface is ON or already stopped.
 *                 The receive will stop after the end of the current packet is finished.
 *                 Assume the unitId was checked before.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_1 ... RSDK_CSI2_MAX_UNITS)
 *
 * @return  rsdkStatus_t - success or error status information.
 * @retval  RSDK_SUCCESS - execution was successful
 *          RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the stopped status
 *          RSDK_CSI2_DRV_WRG_STATE         - if the interface is in a wrong state
 *
 * @pre   It can be called when the reception need to be stopped. A CSI2_Init(...) call must be done previously.
 *
 */
rsdkStatus_t Csi2PlatformRxStop(const rsdkCsi2UnitId_t unitId);

/**
 * @brief          The function start the CSI2 receive module.
 * @details        The receive module of the CSI2 interface is started.
 *                 This action succeed only after a successful Csi2RxStop or the unit is actually ON.
 *                 Assume the unitId was checked before.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_1 ... RSDK_CSI2_MAX_UNITS)
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the running status
 *         RSDK_CSI2_DRV_WRG_STATE         - the interface is in a wrong state and a start is not appropriate
 *
 * @pre   It can be called when the reception is stopped, using an previous CSI2_RxStop() call.
 *
 */
rsdkStatus_t Csi2PlatformRxStart(const rsdkCsi2UnitId_t unitId);

/*================================================================================================*/
/**
 * @brief       The function power off the CSI2 module.
 * @details     All modules of the CSI2-PHY interface are placed in reset mode and kept there.
 *              The action succeed only if the interface is initialized.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_1 ... RSDK_CSI2_MAX_UNITS)
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful.
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the running status
 *         RSDK_CSI2_DRV_NOT_INIT          - the interface is initialized
 *
 * @pre   It can be called when the reception need to be stopped and/or the interface to be powered off.
 */
rsdkStatus_t Csi2PlatformPowerOff(const rsdkCsi2UnitId_t unitId);

/**
 * @brief       The function power on the CSI2 module.
 * @details     All modules of the interface is restarted.
 *              This action can be done only if the interface is in OFF state.
 *              If the action fail, the new state for interface will be NOT_INITIALIZED.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_1 ... RSDK_CSI2_MAX_UNITS)
 *
 * @return rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful.
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the running status.
 *         RSDK_CSI2_DRV_WRG_STATE         - the interface is not OFF
 *
 * @pre   It can be called only when the reception is off.
 *
 */
rsdkStatus_t Csi2PlatformPowerOn(const rsdkCsi2UnitId_t unitId);

/**
 * @brief          The function ask for the current status of the interface.
 * @details        The function return the status of the interface at the moment of the call.
 *                 The return status will return one of the possible status of the interface.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_1 ... RSDK_CSI2_MAX_UNITS)
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
rsdkStatus_t Csi2PlatformGetInterfaceStatus(const rsdkCsi2UnitId_t unitId);

/**
 * @brief          The function ask for the current status of the specific lane.
 * @details        The function return the status of the interface or specific lane at the moment of the call.
 *                 The return status will return one of the possible status of the interface/lane.
 *
 * @param[in]   unitID      - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_1 ... RSDK_CSI2_MAX_UNITS)
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
rsdkStatus_t Csi2PlatformGetLaneStatus(const rsdkCsi2UnitId_t unitId, const uint32_t laneNr);

/**
 * @brief       Procedure to get the real buffer length for a line.
 * @details     The procedure compute the necessary length according to the input parameters.
 *              NOTE : autoStatistics must be 1 even only one channel in the entire unit need the auto statistic.
 *
 * @param[in]   dataType        Type of data to receive, normally RSDK_CSI2M_DATA_TYPE_RAW12 for radar
 * @param[in]   nrChannels      Number of channels, 1...8; usually this number is
 *                              the number of active antennas, multiply by 2 if complex acquisition
 * @param[in]   samplesPerChirp Samples per chirp and channel
 * @param[in]   autoStatistics  The driver must calculate the statistics per chirp (1) or not (0)
 *
 * @return      The necessary memory amount for one line (chirp) if not 0.
 *              Wrong input parameter if 0.
 *
 */
uint32_t Csi2PlatformGetBufferRealLineLen(const rsdkCsi2DataStreamType_t dataType, const uint32_t numChannels,
                                          const uint32_t samplesPerChirp, const uint8_t autoStatistics);

/**
 * @brief       The function return the current frames counter
 * @details     The application can read the counter of frames, to decide how to manage the data.
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_1 ... RSDK_CSI2_MAX_UNITS)
 * @param[in] vcId      - VC number, RSDK_CSI2_VC_0 ... MAX
 *
 * @return uint32_t - the current frames counter, 0xffffffff means error
 *
 */
uint32_t Csi2PlatformGetFramesCounter(const rsdkCsi2UnitId_t unitId, const rsdkCsi2VirtChnlId_t vcId);

#if defined(S32R45) || defined(S32R294)
/**
 * @brief       Get the real channels number of the VC..
 *
 * @param[in]   pVCState - pointer to VC driver state
 *
 * @return      The channels number
 *
 */
uint8_t Csi2PlatformGetChannelNum(const rsdkCsi2VCDriverState_t *pVCState);

/**
 * @brief       The function increase the current frames counter
 *
 * @param[in] unitID    - unit : rsdkCsi2UnitID_t (RSDK_CSI2_UNIT_1 ... RSDK_CSI2_MAX_UNITS)
 * @param[in] vcId      - VC number, RSDK_CSI2_VC_0 ... MAX
 *
 * @return uint32_t - the current frames counter
 *
 */
void Csi2PlatformIncFramesCounter(const rsdkCsi2UnitId_t unitId, const uint32_t vcId);

/**
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
rsdkStatus_t Csi2GetFirstByteOffset(const rsdkCsi2UnitId_t unitId, const rsdkCsi2VirtChnlId_t vcId, uint32_t *pOffset);

/**
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
rsdkStatus_t Csi2GetFirstLinePos(const rsdkCsi2UnitId_t unitId, const rsdkCsi2VirtChnlId_t vcId, uint32_t *pFirstLine);

#endif

#ifdef __cplusplus
}
#endif

#endif /*CSI2_DRIVER_PLATFORM_SPECIFIC_H*/
