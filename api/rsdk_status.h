/*
* Copyright 2019-2021 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RSDK_STATUS_H
#define RSDK_STATUS_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
/** Not API. Internal use only. */
#define RSDK_RFE_STATUS_BASE  0x10000U
#define RSDK_SPT_STATUS_BASE  0x11000U
#if defined(S32R45) || defined(S32R41) || defined(SAF85XX)
#define RSDK_DSP_STATUS_BASE  0x12000U
#endif
#define RSDK_CSI2_STATUS_BASE 0x13000U
#define RSDK_CTE_STATUS_BASE  0x12500U
#if defined(S32R45)
#define RSDK_LAX_STATUS_BASE  0x14000U
#endif
#define RSDK_APP_STATUS_BASE  0x15000U

/**
* @brief          Return datatype for RSDK API calls.
* @details        API functions either return a rsdkStatus_t value or void.
*/
typedef enum
{
    RSDK_SUCCESS = 0, /**< The operation finished successfully */
    RSDK_ERROR,
    RSDK_INVALID_PARAMETER,

    RSDK_RFE_EXCEDDED_MAX_SUPPORTED_DEVICES = RSDK_RFE_STATUS_BASE, /**< The frontendId is bigger than @ref
                                                                          RSDK_RFE_MAX_SUPPORTED_DEVICES */
    RSDK_RFE_UNSUPPORTED_FRONT_END_DEVICE,                          /**< the device specified isn't supported */
    RSDK_RFE_INVALID_MODE,                                          /**< the mode requested isn't supported */
    RSDK_RFE_MR3003_EXCEEDED_LAST_REGISTER_PAGE, /**< driver tried to access register page that doesn't exist */
    RSDK_RFE_CTRL_EXCEEDED_MAX_TRANSFER_SIZE,    /**< the control transfer exceeded the device limits */
    RSDK_RFE_DEVICE_ALREADY_INITIALIZED,         /**< the device was already initialized */
    RSDK_RFE_DRIVER_IN_FAULT_STATE,              /**< the driver is in fault state, a reset of both the driver and
                                                         device is recommended */
    RSDK_RFE_DRIVER_BUSY,                        /**< Another operation is already in progress */
    RSDK_RFE_MR3003_CTRL_E0_ERROR,               /**< CRC error from a previous transfer */
    RSDK_RFE_MR3003_CTRL_E1_ERROR,               /**< parity error from a previous transfer */
    RSDK_RFE_MR3003_CTRL_E2_ERROR,               /**< execution of SST/BIST state */
    RSDK_RFE_MR3003_CTRL_E3_ERROR,               /**< device fault */
    RSDK_RFE_MR3003_CTRL_RX_DATA_CRC_ERROR,      /**< CRC mismatch on received data */
    RSDK_RFE_MR3003_CTRL_PAGE_SET_FAILED,        /**< failed to move to a different register page */
    RSDK_RFE_MR3003_CTRL_REG_UNLOCK_FAILED,      /**< unable to unlock the MR3003 register map */
    RSDK_RFE_MR3003_UNSUPPORTED_REVISION,        /**< the detected revision isn't supported */
    RSDK_RFE_MR3003_FSM_CHANGE_TIMEOUT,          /**< driver time out waiting for the device to change states*/
    RSDK_RFE_0_SAMPLES_PER_CHIRP,                /**< rsdkRfeFrameParams_t::mdt.nrSamplesPerChirp == 0*/
    RSDK_RFE_EXCEEDED_MAX_SAMPLES_PER_CHIRP,     /**< exceeded the maximum number of sample supported */
    RDSK_RFE_INVALID_SAMPLING_FREQUENCY,         /**< the sampling frequency isn't supported */
    RSDK_RFE_INFINITE_CHIRPS_NOT_SUPPORTED,      /**< rsdkRfeFrameParams_t::mdt.nrChirpsInFrame == 0 */
    RSDK_RFE_EXCEEDED_MAX_CHIRPS_PER_FRAME,      /**< exceeded the maximum supported number of chirps per frame*/
    RSDK_RFE_NO_RX_CHANNELS_ENABLED,             /**< no Rx channel was specified */
    RSDK_RFE_EXCEEDED_MAX_RX_CHANNELS,           /**< too many Rx channels requested */
    RSDK_RFE_0_CHIRPS_SHAPES,                    /**< rsdkRfeFrameParams_t::mdt.nrChirpShapes == 0*/
    RSDK_RFE_EXCEEDED_MAX_CHIRP_SHAPES,          /**< exceeded the maximum number of chirp shape supported */
    RSDK_RFE_MR3003_CENTER_FRENQUENCIES_IN_DIFFERENT_RANGES, /**< some of the center frequencies are <77GHz others are >77GHz*/
    RSDK_RFE_MR3003_INVALID_PROFILE_ADDRESS,                 /**< NULL address for a requested profile */
    RSDK_RFE_INSUFFICIENT_CHIRP_PROFILES,                    /**< the number of profiles is less then that of shapes */
    RSDK_RFE_EXCEEDED_MAX_PROFILES,                          /**< too many profiles requested */
    RSDK_RFE_EXCEEDED_MAX_T_START,          /**< rsdkRfeChirpShape_t::tStart is larger than upper limit*/
    RSDK_RFE_EXCEEDED_MIN_T_START,          /**< rsdkRfeChirpShape_t::tStart is smaller than lower limit */
    RSDK_RFE_EXCEEDED_MAX_T_PRESAMPLING,    /**< rsdkRfeChirpShape_t::tPreSampling is larger than upper limit */
    RSDK_RFE_EXCEEDED_MIN_T_PRESAMPLING,    /**< rsdkRfeChirpShape_t::tPreSampling is smaller than lower limit */
    RSDK_RFE_EXCEEDED_MAX_T_POSTSAMPLING,   /**< rsdkRfeChirpShape_t::tPostSampling is larger than upper limit */
    RSDK_RFE_EXCEEDED_MIN_T_POSTSAMPLING,   /**< rsdkRfeChirpShape_t::tPostSampling is smaller than lower limit */
    RSDK_RFE_EXCEEDED_MAX_T_RETURN,         /**< rsdkRfeChirpShape_t::tReturn is larger than upper limit */
    RSDK_RFE_EXCEEDED_MIN_T_RETURN,         /**< rsdkRfeChirpShape_t::tReturn is smaller than lower limit */
    RSDK_RFE_EXCEEDED_MAX_CENTER_FREQUENCY, /**< rsdkRfeChirpShape_t::centerFrequency is larger than upper limit */
    RSDK_RFE_EXCEEDED_MIN_CENTER_FREQUENCY, /**< rsdkRfeChirpShape_t::centerFrequency is smaller than lower limit */
    RSDK_RFE_EXCEEDED_MAX_ACQUISITION_BANDWIDTH, /**< rsdkRfeChirpShape_t::acqBandwidth is larger than upper limit */
    RSDK_RFE_EXCEEDED_MIN_ACQUISITION_BANDWIDTH, /**< rsdkRfeChirpShape_t::acqBandwidth is smaller than lower limit */
    RSDK_RFE_EXCEEDED_MAX_TX_CHANNELS,           /**< rsdkRfeChirpShape_t::txChannelEnable has too many bits enabled */
    RSDK_RFE_INVALID_CHIRP_SLOPE_DIRECTION,      /**< rsdkRfeChirpShape_t::chirpSlopeDirection isn't valid */
    RSDK_RFE_EXCEEDED_MAX_TX_POWER,              /**< rsdkRfeChirpShape_t::txChannelPower is larger than upper limit*/
	RSDK_RFE_EXCEEDED_MAX_RX_GAIN,               /**< rsdkRfeChirpShape_t::rxChannelGain is larger than upper limit*/
	RSDK_RFE_EXCEEDED_MIN_TX_PHASE_SHIFT,		 /**< rsdkTef82XXFrameOptionalParams_t::rsdkTef82XXProfileOptionalParams_t::txPhase is smaller than the lower limit*/
	RSDK_RFE_EXCEEDED_MAX_TX_PHASE_SHIFT,        /**< rsdkTef82XXFrameOptionalParams_t::rsdkTef82XXProfileOptionalParams_t::txPhase is larger than the upper limit*/
    RSDK_RFE_MR3003_SHAPE_START_FREQUENCIES_TOO_DIFFERENT, /**< the difference between start frequencies is too big */
    RSDK_RFE_MR3003_EXCEEDED_MAX_LONG_RANGE_BANDWIDTH,     /**< total chirp bandwidth exceeds the long range limit */
    RSDK_RFE_MR3003_EXCEEDED_LONG_RANGE_MIN_FREQ,          /**< chirp lower frequency is smaller than minimum limit */
    RSDK_RFE_MR3003_EXCEEDED_LONG_RANGE_MAX_FREQ,          /**< chirp upper frequency is larger than maximum limit */
    RSDK_RFE_MR3003_EXCEEDED_MAX_SHORT_RANGE_BANDWIDTH,    /**< total chirp bandwidth exceeds the short range limit */
    RSDK_RFE_MR3003_EXCEEDED_SHORT_RANGE_MIN_FREQ,         /**< chirp lower frequency is smaller than minimum limit */
    RSDK_RFE_MR3003_EXCEEDED_SHORT_RANGE_MAX_FREQ,         /**< chirp upper frequency is larger than maximum limit*/
    RSDK_RFE_MR3003_EXCEEDED_MAX_RSD,                      /**< chirp data slope exceeded device limit */
    RSDK_RFE_MR3003_EXCEEDED_MAX_RSR,                      /**< chirp return slope exceeded device limit */
    RSDK_RFE_MR3003_INVALID_CHIRP_IDX,                     /**< the shape index is out of bounds */
    RSDK_RFE_MR3003_INVALID_RX_CHANNEL_GAIN,               /**< the gain value selected isn't supported */
    RSDK_RFE_MR3003_DEVICE_IN_IDLE,                        /**< when soft reseting the device is in IDLE state */
    RSDK_RFE_MR3003_ADC_LANTECY_EXCEEDS_T_POSTSAMPLING,    /**< the ADC latency is greater than tPostSampling*/
    RSDK_RFE_MR3003_INVALID_SHAPE_IN_PROFILE,              /**< the shape pointed by the profile doesn't exist */
    RSDK_RFE_CTRL_INTERFACE_INVALID_CS,                    /**< the selected chip select signal isn't supported */
    RSDK_RFE_CTRL_INTERFACE_INVALID_SPI,                   /**< the selected spi device isn't supported */
    RSDK_RFE_NULL_API_PARAMETERS,                          /**< mandatory API pointer is NULL */
    RSDK_RFE_NULL_CHIRP_SHAPES,                            /**< pointer to chirp shape is NULL */
    RSDK_RFE_NULL_IRQ_REGISTER_CB,                         /**< isrRegisterCb is NULL */
    RSDK_RFE_USING_DRIVER_IN_WRONG_STATE,                  /**< the driver is in the wrong state for this operation*/
	RSDK_RFE_USING_SLAVE_DRIVER_IN_WRONG_STATE,            /**< the slave driver is in the wrong state for this operation*/
    RSDK_RFE_INVALID_RESET_TYPE,                           /**< the type of soft reset isn't supported */
    RSDK_RFE_OPERATION_NOT_SUPPORTED_BY_DEVICE,            /**< the selected device doesn't support this operation */
    RSDK_RFE_OPERATION_NOT_SUPPORTED_BY_SLAVE_DEVICE,     /**< the slave device doesn't support this operation */
	RSDK_RFE_TEF810X_EXCEEDED_MAX_ISM_DELAY,          	   /**< rsdkTef810XFrameOptionalParams_t::ismDelay greater than tPreSampling */
    RSDK_TEF810X_CALLFAILED,                               /**< DOLPHINIC_EC_CALLFAILED */
    RSDK_TEF810X_CALLTIMEOUT,                              /**< DOLPHINIC_EC_CALLTIMEOUT */
    RSDK_TEF810X_FUNCNOTEXIST,                             /**< DOLPHINIC_EC_FUNCNOTEXIST */
    RSDK_TEF810X_SPICRCFAILED,                             /**< DOLPHINIC_EC_SPICRCFAILED */
    RSDK_TEF810X_SPIWRCHKFAILED,                           /**< DOLPHINIC_EC_SPIWRCHKFAILED */
    RSDK_TEF810X_INPUTOUTOFRANGE,                          /**< DOLPHINIC_EC_INPUTOUTOFRANGE */
    RSDK_TEF810X_INVALIDOTP,                               /**< DOLPHINIC_EC_INVALIDOTP */
	RSDK_TEF810X_OTPNOTREADYTOREAD,
    RSDK_TEF810X_IPNOTACTIVATED,
	RSDK_TEF810X_CANNOTGETMASKVERSION,
    RSDK_TEF810X_INITACT1FAIL,                             /**< DOLPHINIC_EC_INITACT1FAIL */
    RSDK_TEF810X_INITACT2FAIL,                             /**< DOLPHINIC_EC_INITACT2FAIL */
    RSDK_TEF810X_INITACT3FAIL,                             /**< DOLPHINIC_EC_INITACT3FAIL */
    RSDK_TEF810X_INITACT4FAIL,                             /**< DOLPHINIC_EC_INITACT4FAIL */
    RSDK_TEF810X_INITACT5FAIL,                             /**< DOLPHINIC_EC_INITACT5FAIL */
    RSDK_TEF810X_INITACT6FAIL,                             /**< DOLPHINIC_EC_INITACT6FAIL */
    RSDK_TEF810X_INITACT7FAIL,                             /**< DOLPHINIC_EC_INITACT7FAIL */
    RSDK_TEF810X_INITACT8FAIL,                             /**< DOLPHINIC_EC_INITACT8FAIL */
    RSDK_TEF810X_INITACT9FAIL,                             /**< DOLPHINIC_EC_INITACT9FAIL */
    RSDK_TEF810X_INITACT10FAIL,                            /**< DOLPHINIC_EC_INITACT10FAIL */
    RSDK_TEF810X_INITACT11FAIL,                            /**< DOLPHINIC_EC_INITACT11FAIL */
    RSDK_TEF810X_INITACT12FAIL,                            /**< DOLPHINIC_EC_INITACT12FAIL */
    RSDK_TEF810X_INITACT13FAIL,                            /**< DOLPHINIC_EC_INITACT13FAIL */
    RSDK_TEF810X_INITACT14FAIL,                            /**< DOLPHINIC_EC_INITACT14FAIL */
    RSDK_TEF810X_INITACT15FAIL,                            /**< DOLPHINIC_EC_INITACT15FAIL */
    RSDK_TEF810X_INITACT16FAIL,                            /**< DOLPHINIC_EC_INITACT16FAIL */
    RSDK_TEF810X_INITACT17FAIL,                            /**< DOLPHINIC_EC_INITACT17FAIL */
	RSDK_TEF810X_INITACT18FAIL,
    RSDK_TEF810X_ADCPOWERONFAIL,                           /**< DOLPHINIC_EC_ADCPOWERONFAIL */
    RSDK_TEF810X_ADCRECALIBFAIL,                           /**< DOLPHINIC_EC_ADCRECALIBFAIL */
	RSDK_TEF810X_ADCRECALIBTIMEOUT,
    RSDK_TEF810X_DATAPTTRNTESTFAIL,                        /**< DOLPHINIC_EC_DATAPTTRNTESTFAIL */
    RSDK_TEF810X_ISMFITFAIL,                               /**< DOLPHINIC_EC_ISMFITFAIL */
    RSDK_TEF810X_LOGAINCALFAIL,                            /**< DOLPHINIC_EC_LOGAINCALFAIL */
    RSDK_TEF810X_TXGAINCALFAIL,                            /**< DOLPHINIC_EC_TXGAINCALFAIL */
    RSDK_TEF810X_TXSENSORFITFAIL,                          /**< DOLPHINIC_EC_TXSENSORFITFAIL */
    RSDK_TEF810X_CAFCMINSENSORFITFAIL,                     /**< DOLPHINIC_EC_CAFCMINSENSORFITFAIL */
    RSDK_TEF810X_CAFCUNLOCKSENSORFITFAIL,                  /**< DOLPHINIC_EC_CAFCUNLOCKSENSORFITFAIL */
    RSDK_TEF810X_STATUSWARNINGRECOVERFAIL,                 /**< DOLPHINIC_EC_STATUSWARNINGRECOVERFAIL */
    RSDK_TEF810X_INTFTOGGLETESTFAIL,                       /**< DOLPHINIC_EC_INTFTOGGLETESTFAIL */
    RSDK_TEF810X_SHDWREGCHECKFAIL,                         /**< DOLPHINIC_EC_SHDWREGCHECKFAIL */
    RSDK_TEF810X_FAULTINJECTIONRFAIL,                      /**< DOLPHINIC_EC_FAULTINJECTIONRFAIL */
    RSDK_TEF810X_SUBBANDSELECT_BS_FAIL,                    /**< DOLPHINIC_EC_SUBBANDSELECT_BS_FAIL */
	RSDK_TEF810X_SUBBANDNOTFOUND,
	RSDK_TEF810X_MCLKINTEGRITYFAILED,                 
	RSDK_TEF810X_MCLKRECALIBFAILED,
	RSDK_TEF810X_ATBADCREADFAIL,
    RSDK_TEF810X_CSI2_PLL_LOCK_TIMEOUT,
    RSDK_TEF810X_MCLK_FREQ_CNT_TIMEOUT,
    RSDK_TEF810X_CAFC_FREQ_CNT_TIMEOUT,
    RSDK_TEF810X_ERROR_FLAG_MODE,
	RSDK_TEF810X_WRONG_CHIRP_TRIGGER_MODE,
	RSDK_TEF810X_ERRORN_PIN_ACTIVE,
	RSDK_TEF810X_OTPCRCERROR,
    RSDK_TEF810X_CAFC_VCO480MHZCHECKFAILED,
	RSDK_TEF810X_CAFC_DIVIDEFREQCHECKFAILED,
	RSDK_TEF810X_ADCCALERR_INJECT_FIT,
    RSDK_TEF810X_ADCCALERR_RECOVERY_FIT,
    RSDK_TEF810X_UNMAPED_ERROR,                            /**< Code returned by TEF810X unmapped to rsdkStatus_t */
	RSKD_TEF810X_RFBIST_FREQUENCY_FAILURE,
	RSDK_TEF810X_RFBIST_MAGNITUDE_FAILURE,
	RSKD_TEF810X_RFBIST_PHASE_FAILURE,
	RSKD_TEF810X_RFBIST_USER_DEFINED_PROF3,				  /**< RFBIST params were not loaded due to user providing profile 3 data */
	RSDK_TEF810X_CSI2_VIRTUAL_CHANNEL_NOT_CONFIGURED,

	// RSDK RF Abstract Tef82XX Error Codes
	RSDK_RFE_TEF82XX_EXCEEDED_MAX_ISM_DELAY,
	RSDK_TEF82XX_WRONG_CHIRP_TRIGGER_MODE,
	RSDK_TEF82XX_INVALID_CSI2_VIRTUAL_CHANNEL_NO,
	RSDK_TEF82XX_CSI2_VIRTUAL_CHANNEL_NOT_CONFIGURED,
	// Tef82XX Error Codes
	RSDK_TEF82XX_INPUTOUTOFRANGE,
	RSDK_TEF82XX_FUNCNOTEXIST,
	RSDK_TEF82XX_FAILTOINJECTCRCERR,
	RSDK_TEF82XX_WRONGINTERFACE,
	RSDK_TEF82XX_IPDISABLED,
	RSDK_TEF82XX_SYS_IPHASNOATBCONNECTION,
	RSDK_TEF82XX_SYS_CANNOTFINDCAFCDACCODE,
	RSDK_TEF82XX_SYS_CALLOGAINBISTFAILED,
	RSDK_TEF82XX_SYS_SYSCONFNOTSET,
	RSDK_TEF82XX_SYS_ADCINVALID,
	RSDK_TEF82XX_SYS_FILEERROR,
	RSDK_TEF82XX_SYS_JSONSTRINGNOTEXISTED,
	RSDK_TEF82XX_SYS_AAFC_AACFAIL,
	RSDK_TEF82XX_SYS_AAFC_AFCFAIL,
	RSDK_TEF82XX_SYS_TX_TEMP_READ_TIMEOUT,
	RSDK_TEF82XX_SYS_MCLK_INTEGRITY_FAILED,
	RSDK_TEF82XX_SYS_WRONG_APPCONFIG,
	RSDK_TEF82XX_SYS_INVALID_INPUT,
	RSDK_TEF82XX_SYS_INVALID_DDMA,
	RSDK_TEF82XX_SYS_FAULTINJECTIONRFAIL,
	RSDK_TEF82XX_SYS_WRONG_MID_NO_RECOVERY,
	RSDK_TEF82XX_SYS_WRONG_PROFILE_NO_RECOVERY,
	RSDK_TEF82XX_SYS_ERRORRECOVERY_FAILED,
	RSDK_TEF82XX_SYS_CAFCINTEGRITY_CHECK_LEVEL,
	RSDK_TEF82XX_SYS_CAFCINTEGRITY_CHECK_UNLOCK,
	RSDK_TEF82XX_SYS_TXGAINCALIBRATIONFAIL,
	RSDK_TEF82XX_SYS_WRONG_DATAINTERFACE_CONFIG,
	RSDK_TEF82XX_SYS_DATAINTERFACETEST_FAIL,
	RSDK_TEF82XX_SYS_TXPRCURR_SETTING_FAIL,
	RSDK_TEF82XX_SYS_ERROR_N,
	RSDK_TEF82XX_SYS_INVALID_PROFILE,
	RSDK_TEF82XX_SYS_SPIACCESS_CHECKFAIL,
	RSDK_TEF82XX_SYS_CAFC_TEMP_READ_TIMEOUT,
	RSDK_TEF82XX_SYS_CHIRPSTARTOUT_OUTPUTMODE,
	RSDK_TEF82XX_SYS_ERRORN_OUTPUTMODE,
	RSDK_TEF82XX_SYS_ERRORRESET_INPUTMODE,
	RSDK_TEF82XX_SYS_CHIRPSTART_IN_INPUTMODE,
	RSDK_TEF82XX_SYS_TX1_I_PS_INPUTMODE,
	RSDK_TEF82XX_SYS_TX2_I_PS_INPUTMODE,
	RSDK_TEF82XX_SYS_TX3_I_PS_INPUTMODE,
	RSDK_TEF82XX_SYS_TX1_Q_PS_INPUTMODE,
	RSDK_TEF82XX_SYS_TX2_Q_PS_INPUTMODE,
	RSDK_TEF82XX_SYS_TX3_Q_PS_INPUTMODE,
	RSDK_TEF82XX_SYS_SLAVE_WRONG_APPCONFIG,
	RSDK_TEF82XX_SYS_SUBBANDINC_OUTOFRANGE,
	RSDK_TEF82XX_SYS_AMPLITUDEINC_OUTOFRANGE,
	RSDK_TEF82XX_SYS_PTR_ADDR_NULL,
	RSDK_TEF82XX_SYS_AAFC_AUTO_NOT_SUPPORTED,
	RSDK_TEF82XX_ALG_DUMMYERROR,
	RSDK_TEF82XX_SPI_WRTRDCRCERROR,
	RSDK_TEF82XX_SPI_WRTCHECKFAIL,
	RSDK_TEF82XX_SPI_READFAIL,
	RSDK_TEF82XX_CC_SYSCONFNOTSET,
	RSDK_TEF82XX_CC_PTR_ADDR_NULL,
	RSDK_TEF82XX_MIPI_DEVICE_READY_TIMEOUT,
	RSDK_TEF82XX_MIPI_FREQCOUNTTIMEOUT,
	RSDK_TEF82XX_MIPI_FREQCNT_OUTOFRANGE,
	RSDK_TEF82XX_MIPI_PTR_ADDR_NULL,
	RSDK_TEF82XX_TE_PRSAFETYDELAY,
	RSDK_TEF82XX_TE_PRCALIBDELAY,
	RSDK_TEF82XX_TE_RXACTIVEDELAY,
	RSDK_TEF82XX_TE_HPFRESETDELAY,
	RSDK_TEF82XX_TE_DCPOWONDELAY,
	RSDK_TEF82XX_TE_DCSAFETYDELAY,
	RSDK_TEF82XX_TE_DDMASETTING,
	RSDK_TEF82XX_TE_DDMAMODE,
	RSDK_TEF82XX_TE_JUMPBACK,
	RSDK_TEF82XX_TE_TXRXGRPDELAY,
	RSDK_TEF82XX_TE_FINEGRPDELAY,
	RSDK_TEF82XX_TE_PROFILELIST,
	RSDK_TEF82XX_TE_PRCALTIMEOUT,
	RSDK_TEF82XX_TE_BIASDELAY,
	RSDK_TEF82XX_TE_INPUTNULL,
	RSDK_TEF82XX_TE_PRFREPEATCNT,
	RSDK_TEF82XX_TE_CHIRPSEQINTERVAL,
	RSDK_TEF82XX_TE_SETTLETIME,
	RSDK_TEF82XX_TE_OUTPUTSAMPRATE,
	RSDK_TEF82XX_TE_CHIRPINTERVAL,
	RSDK_TEF82XX_TE_VIRTUALCHANNEL,
	RSDK_TEF82XX_TE_DWELLTIME,
	RSDK_TEF82XX_TE_DELAYFROMREFPT,
	RSDK_TEF82XX_TE_FREQCNTTIMEOUT,
	RSDK_TEF82XX_TE_CHIRPSEQUENCE,
	RSDK_TEF82XX_TE_CHIRPTRGMODE,
	RSDK_TEF82XX_TE_CHIRPSTARTOUTMODE,
	RSDK_TEF82XX_TE_PRCALFAILURE,
	RSDK_TEF82XX_TE_PRSAFETYCHECKFAILURE,
	RSDK_TEF82XX_TE_PHASEOUTOFRANGE,
	RSDK_TEF82XX_SC_PROFILE,
	RSDK_TEF82XX_SC_OPERATING_FREQ_OUTOFRANGE,
	RSDK_TEF82XX_SC_PTR_ADDR_NULL,
	RSDK_TEF82XX_CAFC_FREQCNTTIMEOUT,
	RSDK_TEF82XX_CAFC_SBOUTOFRANGE,
	RSDK_TEF82XX_CAFC_INPUTOUTOFRANGE,
	RSDK_TEF82XX_CAFC_BWOUTOFRANGE,
	RSDK_TEF82XX_CAFC_CONFIGLOOPFILTERFAIL,
	RSDK_TEF82XX_CAFC_LOOPBWOUTOFRANGE,
	RSDK_TEF82XX_CAFC_PTR_ADDR_NULL,
	RSDK_TEF82XX_CAFC_WRONGPROFILEID,
	RSDK_TEF82XX_CAFC_AAFCKVCODVDT_TIMEOUT,
	RSDK_TEF82XX_CAFC_VCOCALIB_AAFCFAIL,
	RSDK_TEF82XX_CAFC_VCOCALIB_KVCODVDTFAIL,
	RSDK_TEF82XX_CAFC_NOSTOREINPROFILESELECTED,
	RSDK_TEF82XX_ADC_CALIBRATIONFAILED,
	RSDK_TEF82XX_ADC_CALIBRATIONTIMEOUT,
	RSDK_TEF82XX_ADC_CALIBRATIONCHECKNOTDONE,
	RSDK_TEF82XX_GBIAS_DUMMY,
	RSDK_TEF82XX_LOI_CALIBRATIONFAIL,
	RSDK_TEF82XX_LOI_LOX3GAINCALIBRATIONFAIL,
	RSDK_TEF82XX_LOI_LUTGAINCODE_OUTOFRANGE,
	RSDK_TEF82XX_LOI_DIRECTGAINCODE_OUTOFRANGE,
	RSDK_TEF82XX_LOI_CAL_BUSY,
	RSDK_TEF82XX_LOI_CAL_ERROR,
	RSDK_TEF82XX_LOI_PTR_ADDR_NULL,
	RSDK_TEF82XX_MCLK_PLL_PLL_CALIBRATION_TIMEOUT,
	RSDK_TEF82XX_MCLK_PLL_PLL_CALIBRATION_FAILED,
	RSDK_TEF82XX_MCLK_PLL_CALIBRATION_FAILED,
	RSDK_TEF82XX_MCLK_PLL_CALIBRATION_NOTFINISHED,
	RSDK_TEF82XX_MCLK_MSPC_CALIBRATION_ERROR,
	RSDK_TEF82XX_MCLK_MSPCTIMEOUT,
	RSDK_TEF82XX_MCLK_MSPC_CALIBCHECK_NOTFINISHED,
	RSDK_TEF82XX_MCLK_MSPCCHEK_TIMEOUT,
	RSDK_TEF82XX_MCLK_PTR_ADDR_NULL,
	RSDK_TEF82XX_RX_INPUTOUTOFRANGE,
	RSDK_TEF82XX_RX_CAL_BUSY,
	RSDK_TEF82XX_RX1_CAL_BUSY,
	RSDK_TEF82XX_RX2_CAL_BUSY,
	RSDK_TEF82XX_RX3_CAL_BUSY,
	RSDK_TEF82XX_RX4_CAL_BUSY,
	RSDK_TEF82XX_RX_CAL_ERROR,
	RSDK_TEF82XX_RX1_CAL_ERROR,
	RSDK_TEF82XX_RX2_CAL_ERROR,
	RSDK_TEF82XX_RX3_CAL_ERROR,
	RSDK_TEF82XX_RX4_CAL_ERROR,
	RSDK_TEF82XX_RX1_RX2_CAL_ERROR,
	RSDK_TEF82XX_RX1_RX3_CAL_ERROR,
	RSDK_TEF82XX_RX1_RX4_CAL_ERROR,
	RSDK_TEF82XX_RX2_RX3_CAL_ERROR,
	RSDK_TEF82XX_RX2_RX4_CAL_ERROR,
	RSDK_TEF82XX_RX3_RX4_CAL_ERROR,
	RSDK_TEF82XX_RX1_RX2_RX3_CAL_ERROR,
	RSDK_TEF82XX_RX1_RX2_RX4_CAL_ERROR,
	RSDK_TEF82XX_RX2_RX3_RX4_CAL_ERROR,
	RSDK_TEF82XX_RX1_RX2_RX3_RX4_CAL_ERROR,
	RSDK_TEF82XX_TX_DUMMYERROR,
	RSDK_TEF82XX_TX_INPUTOUTOFRANGE,
	RSDK_TEF82XX_TX_PRCADCTIMEOUT,
	RSDK_TEF82XX_TX_SENSORFITFAIL,
	RSDK_TEF82XX_TX_CAL_BUSY,
	RSDK_TEF82XX_TX1_CAL_BUSY,
	RSDK_TEF82XX_TX2_CAL_BUSY,
	RSDK_TEF82XX_TX3_CAL_BUSY,
	RSDK_TEF82XX_TX_CAL_ERROR,
	RSDK_TEF82XX_TX1_CAL_ERROR,
	RSDK_TEF82XX_TX2_CAL_ERROR,
	RSDK_TEF82XX_TX3_CAL_ERROR,
	RSDK_TEF82XX_TX1_TX2_CAL_ERROR,
	RSDK_TEF82XX_TX1_TX3_CAL_ERROR,
	RSDK_TEF82XX_TX2_TX3_CAL_ERROR,
	RSDK_TEF82XX_TX1_TX2_TX3_CAL_ERROR,
	RSDK_TEF82XX_TX_SAFETY_BUSY,
	RSDK_TEF82XX_TX1_SAFETY_BUSY,
	RSDK_TEF82XX_TX2_SAFETY_BUSY,
	RSDK_TEF82XX_TX3_SAFETY_BUSY,
	RSDK_TEF82XX_TX_SAFETY_ERROR,
	RSDK_TEF82XX_TX1_SAFETY_ERROR,
	RSDK_TEF82XX_TX2_SAFETY_ERROR,
	RSDK_TEF82XX_TX3_SAFETY_ERROR,
	RSDK_TEF82XX_TX1_TX2_SAFETY_ERROR,
	RSDK_TEF82XX_TX1_TX3_SAFETY_ERROR,
	RSDK_TEF82XX_TX2_TX3_SAFETY_ERROR,
	RSDK_TEF82XX_TX1_TX2_TX3_SAFETY_ERROR,
	RSDK_TEF82XX_TX_PTR_ADDR_NULL,
	RSDK_TEF82XX_TX_NOSTOREINPROFILESELECTED,
	RSDK_TEF82XX_TX_POUT_VGA_REDUCETX_NOTSUPPORTED,
	RSDK_TEF82XX_SSB_SIGNALNOTEXISTONSELECTEDATB,
	RSDK_TEF82XX_SSB_INPUTOUTOFRANGE,
	RSDK_TEF82XX_GLDO_DUMMYERROR,
	RSDK_TEF82XX_GLDO_SNSERROR,
	RSDK_TEF82XX_GLDO_SNS_FORCEERROR,
	RSDK_TEF82XX_GLDO_SNS_RESETERROR,
	RSDK_TEF82XX_ATB_BISTADCTIMEOUT,
	RSDK_TEF82XX_ATB_WRONGATBNODESELECTED,
	RSDK_TEF82XX_OTP_FAILTOREAD,
	RSDK_TEF82XX_OTP_CRCCHECKFAILURE,
	RSDK_TEF82XX_OTP_IPISDISABLED,
	RSDK_TEF82XX_OTP_OTPISNOTLOADED,
	RSDK_TEF82XX_OTP_NOTENABLED,
	RSDK_TEF82XX_OTP_ALLMEMDISABLED,
	RSDK_TEF82XX_ISM_INVALID_INPUT,
	RSDK_TEF82XX_ISM_PTR_ADDR_NULL,
	RSDK_TEF82XX_ISM_STARTUP_SEQ_CONFIG_FAILED,
	RSDK_TEF82XX_ISM_FIT_ERROR_N_HIGH,
	RSDK_TEF82XX_ISM_FIT_RESET_STATE_CHANGE_FAILED,
	RSDK_TEF82XX_ISM_FIT_RESET_ERROR_FAILED,
	RSDK_TEF82XX_ISM_FIT_FORCE_STATE_CHANGE_FAILED,
	RSDK_TEF82XX_ISM_FIT_FORCE_ERROR_FAILED,
	RSDK_TEF82XX_ISM_FIT_FAILED,
	RSDK_TEF82XX_ISM_FIT_STATUS_FAILED,
	RSDK_TEF82XX_ISM_FIT_TX_STATUS_FAILED,
	RSDK_TEF82XX_ISM_FIT_RX_STATUS_FAILED,
	RSDK_TEF82XX_ISM_FIT_ADC_STATUS_FAILED,
	RSDK_TEF82XX_ISM_FIT_CHIRP_STATUS_FAILED,
	RSDK_TEF82XX_ISM_FIT_MCLK_LO_ISM_OTP_CC_STATUS_FAILED,
	RSDK_TEF82XX_ISM_FIT_SSB_SER_GB_GLDO_ATB_STATUS_FAILED,
	RSDK_TEF82XX_ISM_INPUTMODE_TOGGLE_TEST_FAILED,
	RSDK_TEF82XX_ISM_OUTPUTMODE_TOGGLE_TEST_FAILED,
	RSDK_TEF82XX_ISM_CAL_BUSY,
	RSDK_TEF82XX_ISM_CAL_ERROR,
	RSDK_TEF82XX_ISM_CAL_SUCCESS,

    RSDK_RFE_RFBIST_RFE_MODE_NOT_SUPPORTED,               /**< TEF82XX RFE RFBist - RFBIST must be run on STANDALONE or MASTER(with one SLAVE) FE*/
    RSDK_RFE_RFBIST_ACQ_CSI2_ERRORS,                      /**< TEF82XX RFE RFBist - too many CSI2 errors on RFBist acquisition*/
	RSDK_RFE_RFBIST_ACQ_TIMEOUT,                      	  /**< TEF82XX RFE RFBist - timeout expired on RFBist frame(s) acquisition*/
	RSDK_RFE_RFBIST_ACQ_TIMER_ERROR,                      /**< TEF82XX RFE RFBist - setting timer error*/
	RSDK_RFE_RFBIST_ADC_DATA_BUFF_NOT_CONFIGURED,		  /**< TEF82XX RFE RFBist - Adc data buff (Frame Optional Params) not configured */
	RSKD_TEF82XX_RFBIST_FREQUENCY_FAILURE,                /**< TEF82XX RFE RFBist - RFBIST_FREQUENCY_FAILURE*/
	RSDK_TEF82XX_RFBIST_MAGNITUDE_FAILURE,                /**< TEF82XX RFE RFBist - RFBIST_MAGNITUDE_FAILURE*/
	RSKD_TEF82XX_RFBIST_PHASE_FAILURE,                    /**< TEF82XX RFE RFBist - RFBIST_PHASE_FAILURE*/

	RSDK_TEF82XX_RFE_PHASEROTATOR_INVALID_DDMA_MODE,
	RSDK_TEF82XX_RFE_PHASEROTATOR_INVALID_DDMA_SOURCE,
	RSDK_TEF82XX_RFE_EXCEEDED_MIN_PHASEROTATOR_DDMA_INITIAL_PHASE,
	RSDK_TEF82XX_RFE_EXCEEDED_MAX_PHASEROTATOR_DDMA_INITIAL_PHASE,
	RSDK_TEF82XX_RFE_EXCEEDED_MIN_PHASEROTATOR_DDMA_PHASE_UPDATE,
	RSDK_TEF82XX_RFE_EXCEEDED_MAX_PHASEROTATOR_DDMA_PHASE_UPDATE,
	RSDK_TEF82XX_RFE_PHASEROTATOR_INVALID_PC_GEN_MODE,
	RSDK_TEF82XX_RFE_PHASEROTATOR_INVALID_BPSK_SOURCE,

	RSDK_TEF82XX_UNMAPED_ERROR,                            /**< Code returned by TEF82XX unmapped to rsdkStatus_t */
	RSDK_TEF82XX_ES_VERSION_UNSUPPORTED,				   /**< Driver was build with unsupported ES (Sample) define */

	RSDK_TEF82XX_PLF_INPUTOUTOFRANGE,                      /**< TEF82XX platform - input of range*/
	RSDK_TEF82XX_PLF_GLUE_SPI_ERROR,                       /**< TEF82XX platform - SPI glue layer error*/
	RSDK_TEF82XX_PLF_SPIWRTRDCRCERROR,                     /**< TEF82XX platform - SPIWRTRDCRCERROR */
	RSDK_TEF82XX_PLF_RSDK_ERRORN_PIN_ACTIVE,               /**< TEF82XX platform - error_n pin is active*/
	RSDK_TEF82XX_PLF_UNMAPPED_ERROR,                       /**< TEF82XX platform - unmapped error */

    RSDK_TEF82XX_RFE_DRIFT_ONEPROFILE,                     /**< TEF82XX Freq auto drift supports only one profile */ 
    RSDK_TEF82XX_RFE_MAX_AUTO_DRIFT,                       /**< TEF82XX Max freq auto drift exceded */
    RSDK_TEF82XX_RFE_MIN_AUTO_DRIFT,                       /**< TEF82XX Min freq auto drift exceded */

    RSDK_RFE_NULL_PHYSICAL_ADDR,                /**< OAL_GetReturnAddress returned NULL when fetching physical addr */
    RSDK_RFE_NULL_VIRTUAL_ADDR,                 /**< OAL_GetReturnAddress returned NULL when fetching virtual addr */
    RSDK_OAL_ATOMIC_OPERATION_FAILED,           /**< OAL_SetAtomic call failed */
    RSDK_RFE_STUCK_IN_BUSY,                     /**< OAL_SetAtomic failed and the driver can't change busy flag */
    RSDK_RFE_STUCK_IN_BUSY_AND_ERROR_ENCOUNTERED, /**< Can't change busy flag and another error was encountered */
    RSDK_RFE_NOT_INITIALIZED,                   /**< The driver wasn't initialized */

    // Leave these at the end of RFE error group in case the ctrl interface changes
    RSDK_RFE_GLUE_SPI_INIT_ERROR,               /**< Error reported by the SPI glue layer */
    RSDK_RFE_GLUE_SPI_TRANSFER_ERROR,           /**< Error reported by the SPI glue layer */
    RSDK_RFE_GLUE_GPIO_ERROR,                   /**< Error reported by the GPIO glue layer */
	RSDK_RFE_GLUE_TIMER_ERROR,                  /**< Error reported by the Timer glue layer */


    //-------------------------------------------------------------------------
    // CSI2 Driver API error codes:
    RSDK_CSI2_DRV_WRG_UNIT_ID = RSDK_CSI2_STATUS_BASE,         /**< Wrong unit ID specified */
    RSDK_CSI2_DRV_NULL_PARAM_PTR,           /**< Wrong parameters pointer (NULL) */
    RSDK_CSI2_DRV_NULL_VC_PARAM_PTR,        /**< Wrong parameters pointer to VC (NULL) */
    RSDK_CSI2_DRV_VC_AUX_MISMATCH,          /**< Mismatch between VC and Auxiliary pointers (VC==NULL, Aux!=NULL) */
    RSDK_CSI2_DRV_NULL_ERR_CB_PTR,          /**< Wrong error callback pointer (NULL) */
    RSDK_CSI2_DRV_NULL_ISR_CB_PTR,          /**< Wrong error callback pointer for isr register (NULL) */
    RSDK_CSI2_DRV_INVALID_CHANNEL_NR,       /**< Wrong channel/antenna number */
    RSDK_CSI2_DRV_INVALID_CALIB_MODE,       /**< Wrong calibration ID */
    RSDK_CSI2_DRV_INVALID_LANES_NR,         /**< Wrong lanes number */
    RSDK_CSI2_DRV_INVALID_CLOCK_FREQ,       /**< Clock frequency incorrect (null) */
    RSDK_CSI2_DRV_INVALID_RX_SWAP,          /**< Invalid Rx lanes swap specification */
    RSDK_CSI2_DRV_INVALID_DATA_TYPE,        /**< Invalid data type specification or mismatch for auxiliary data*/
    RSDK_CSI2_DRV_INVALID_EVT_REQ,          /**< Wrong line end request : bufNumLinesTrigger is 0 */
    RSDK_CSI2_DRV_INVALID_VC_PARAMS,        /**< Invalid data type specification */
    RSDK_CSI2_DRV_INVALID_DC_PARAMS,        /**< Invalid DC params compensation specification */
    RSDK_CSI2_DRV_INVALID_INIT_PARAMS,      /**< Invalid params for DPHY initialization */
    RSDK_CSI2_DRV_TOO_SMALL_BUFFER,         /**< The buffer provided is too small for the requested data */
    RSDK_CSI2_DRV_NO_SAMPLE_PER_CHIRP,      /**< Samples per chirp incorrect (null) */
    RSDK_CSI2_DRV_NO_CHIRPS_PER_FRAME,      /**< Chirps per frame incorrect (null) */
    RSDK_CSI2_DRV_NO_LINE_LENGTH,           /**< The VC buffer length was not provided */
    RSDK_CSI2_DRV_BUF_LEN_NOT_ALIGNED,      /**< The VC buffer length is not aligned to 16 bytes boundaries */
    RSDK_CSI2_DRV_BUF_PTR_NULL,             /**< The VC buffer pointer not provided */
    RSDK_CSI2_DRV_BUF_PTR_NOT_ALIGNED,      /**< The VC buffer pointer not aligned to 16 bytes boundaries */
    RSDK_CSI2_DRV_BUF_NUM_LINES_ERR,        /**< The VC buffer must have at least 2 lines */

    RSDK_CSI2_DRV_ERR_UNIT_0_MUST_BE_FIRST, /**< Error for S32R45 platform : initialize unit_0 before unit_1 */
    RSDK_CSI2_DRV_ERR_UNIT_1_MUST_BE_FIRST, /**< Error for S32R294 platform : initialize unit_1 before unit_0 */
    RSDK_CSI2_DRV_ERR_UNIT_2_MUST_BE_FIRST, /**< Error for S32R45 platform : initialize unit_2 before unit_3 */
    RSDK_CSI2_DRV_ERR_INVALID_INT_NR,       /**< Incorrect IRQ nr used */
    RSDK_CSI2_DRV_ERR_INVALID_CORE_NR,      /**< Incorrect core nr used */
    RSDK_CSI2_DRV_ERR_IRQ_HANDLER_REG,      /**< Irq handler registration error */

    RSDK_CSI2_DRV_SW_RESET_ERROR,           /**< Hardware error to reset sequence. */
    RSDK_CSI2_DRV_CALIBRATION_TIMEOUT,      /**< Autocalibration error. */
    RSDK_CSI2_DRV_TIMER_ERROR,              /**< Timer error waiting for STOP state on data lanes at initialization   */
    RSDK_CSI2_DRV_HW_RESPONSE_ERROR,        /**< Incorrect hardware response  */
    RSDK_CSI2_DRV_NOT_INIT,                 /**< CSI2 driver not initialized */

    RSDK_CSI2_DRV_ERR_INVALID_REQ,          /**< Incorrect request to the driver - Linux usage only */
    RSDK_CSI2_DRV_ERR_COPY_DATA_ERROR,      /**< The data couldn't be copied between kernel<->user_space - Linux only */
    RSDK_CSI2_DRV_ERR_EVT_CONN,             /**< Error when trying to connect to kernel events */
    RSDK_CSI2_DRV_ERR_START_EVT_MGR,        /**< Error starting the events manager */


    RSDK_CSI2_DRV_WRG_STATE,                /**< The interface is in a wrong state and the action can't succeed. */
    RSDK_CSI2_DRV_POWERED_OFF,              /**< The PHY interface if powered off. */
    RSDK_CSI2_DRV_RX_STOPPED,               /**< The Rx interface if stopped. */

    RSDK_CSI2_DRV_STATE_LINE_MARK,          /**< The Rx lane in Mark-1 state. */
    RSDK_CSI2_DRV_STATE_LINE_ULPA,          /**< The Rx lane in Ultra Low Power State. */
    RSDK_CSI2_DRV_STATE_LINE_STOP,          /**< The Rx lane in Stop state. */
    RSDK_CSI2_DRV_STATE_LINE_REC,           /**< The Rx lane is receiving data. */
    RSDK_CSI2_DRV_STATE_LINE_VRX,           /**< The Rx lane is receiving valid data. */
    RSDK_CSI2_DRV_STATE_LINE_ON,            /**< The Rx lane active, but not currently receiving data. */
    RSDK_CSI2_DRV_STATE_LINE_OFF,           /**< The Rx lane inactive. */

    RSDK_CSI2_DRV_STATE_ON,                 /**< The driver status is ON. */

    //-------------------------------------------------------------------------
    // CSI2 Driver API error codes:
    RSDK_CTE_DRV_WRG_MODE = RSDK_CTE_STATUS_BASE,	/**< Wrong working mode requested                               */
    RSDK_CTE_DRV_ZERO_FREQ,                 /**< The required frequency set to zero.                                */
    RSDK_CTE_DRV_NULL_PTR_PARAMS,           /**< The required parameters pointer is NULL.                           */
    RSDK_CTE_DRV_NULL_PTR_TABLE0,           /**< The required time table 0 pointer is NULL.                         */
    RSDK_CTE_DRV_NULL_PTR_SIG_DEF,          /**< The required pointer to the signals definitions is NULL.           */
    RSDK_CTE_DRV_NULL_PTR_EVENTS,           /**< The time table events pointer is NULL.                             */
    RSDK_CTE_DRV_NULL_PTR_ACTIONS,          /**< The event pointer to actions is NULL.                              */
    RSDK_CTE_DRV_NULL_CLK_PEROD,            /**< The specified output clock type period is NULL.                    */
    RSDK_CTE_DRV_NULL_CALLBACK,             /**< The callback pointer is NULL, but irq events were specified.       */
    RSDK_CTE_DRV_NOT_NULL_PTR_SIG_DEF,      /**< The second signal definition table not NULL, but the timing table
                                             is NULL.                                                               */
    RSDK_CTE_DRV_WRG_CSI2_UNIT,             /**< Incorrect MIPI-CSI2 unit specified.                                */
    RSDK_CTE_DRV_WRG_CSI2_VC,               /**< Incorrect MIPI-CSI2 Virtual Channel specified.                     */
    RSDK_CTE_DRV_TABLE_TOO_LONG,            /**< The timing table is too long. This means : more than 64 entries for a
                                              single table, more than 32 entries at least for one of the two tables. */
    RSDK_CTE_DRV_TOO_MANY_ACTIONS,          /**< Too many actions specified for a single event.
                                             This means the total number of actions counted is bigger than
                                             the available output signals.                                          */
    RSDK_CTE_DRV_TOO_MANY_CLOCKS,           /**< Too many different clocks requested. The requested periods are too
                                             different and the software was not able to get a good compromise
                                             (period error less than 40%) in up to 4 clocks. To avoid this issue,
                                             use more similar/identical periods or whithin a little amplitude.      */
    RSDK_CTE_DRV_CLK_DIVIDER_ERROR,         /**< The timing table is too wide in time and the internal CTE timing
                                             is not able to be set. Normally this is generated by a wrong CTE clock
                                             frequency defined, not zero, but incorrect (too low) or an incorrect
                                             timing sequence (the time values ar not in ascending order).           */
    RSDK_CTE_DRV_SIG_OUT_STATE_WRG,         /**< The specified output state is wrong, in the events action chain    */
    RSDK_CTE_DRV_SIG_OUT_WRG_TYPE,          /**< Wrong type defined for output, usually for SPT and FLEX events     */
    RSDK_CTE_DRV_SIG_OUT_DIF_TYPE,          /**< Different types defined for the similar output signal,
                                             used for SPT and FLEX events only.                                     */
    RSDK_CTE_DRV_SIG_NOT_DEF,               /**< The specified output signal in actions table is not defined as
                                             output type */
    RSDK_CTE_DRV_NOT_INITIALIZED,           /**< Driver was not initialized or the initialization failed             */
    RSDK_CTE_DRV_RUNNING,                   /**< CTE is already running                                              */
    RSDK_CTE_DRV_NOT_RUNNING,               /**< CTE is not running                                                  */
    RSDK_CTE_DRV_WRG_PTR_TABLE1,            /**< Table1 pointer is not in accord with the existing CTE initialization
                                             at a table update : Table1 was initialized and now the pointer is NULL,
                                             or table was not initialized and now the pointer is not NULL.           */
    RSDK_CTE_DRV_IRQ_REG_FAILED,            /**< Failed to register CTE irq handler.                                 */
    RSDK_CTE_DRV_LX_NOT_ENOUGH_PARAM,       /**< Linux call - not enough data provided. Usually this means the 
                                             structures provided or the total data length are less than required     */
    RSDK_CTE_DRV_LX_WRG_CALL,               /**< Linux call - wrong call                                             */
    RSDK_CTE_DRV_ERR_INVALID_REQ,           /**< Incorrect request to the driver - Linux usage only */
    RSDK_CTE_DRV_ERR_COPY_DATA_ERROR,       /**< The data couldn't be copied between kernel<->user_space - Linux only */
    RSDK_CTE_DRV_ERR_EVT_CONN,              /**< Error when trying to connect to kernel events */
    RSDK_CTE_DRV_ERR_START_EVT_MGR,         /**< Error starting the events manager */

    //-------------------------------------------------------------------------
    //SPT Driver API error codes:
    RSDK_SPT_RET_ERR_INVALID_PARAM = RSDK_SPT_STATUS_BASE, /**< Driver API error: Parameter value or
                                                                combination of values not supported. */
    RSDK_SPT_RET_ERR_INVALID_STATE, /**< Driver API error: operation not supported in current state. */
    RSDK_SPT_RET_ERR_TIMEOUT_BLOCK, /**< Driver error: SPT blocking call did not finish in due time and did not report any specific error.
                                         This is treated as a faulty state and imposes re-initialization of the driver.*/

    RSDK_SPT_RET_WARN_HW_BUSY, /**< Driver API warning: Input parameters are valid but no action was done
                                       by current function call because the SPT hardware is busy. */
    //SPT hw error codes (signaled by interrupt):
    RSDK_SPT_RET_ERR_MEM,    /**< SPT hw error: internal memory handling, triggers an ECS interrupt.
                                MEM_ERR_STATUS register value is passed to the user callback (rsdkSptIsrCb_t) */
    RSDK_SPT_RET_ERR_DMA,    /**< SPT hw error: SDMA operation, triggers an ECS interrupt.
                                DMA_ERR_STATUS register value is passed to the user callback (rsdkSptIsrCb_t) */
    RSDK_SPT_RET_ERR_HW_ACC, /**< SPT hw error: tried to execute an instruction with illegal operands or configuration, triggered an ECS interrupt.
                                HW_ACC_ERR_STATUS register value is passed to the user callback (rsdkSptIsrCb_t) */
    RSDK_SPT_RET_ERR_ILLOP,  /**< SPT hw error: Illegal SPT instruction, triggers an ECS interrupt.
                                CS_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/

    RSDK_SPT_RET_ERR_TIMEOUT_START, /**< Driver error: SPT kernel launcher function was not able to verify that all SPT hw status bits were properly cleared.
                                         This is treated as a faulty state and imposes re-initialization of the driver*/
    RSDK_SPT_RET_ERR_TIMEOUT_STOP, /**< Driver error: RsdkSptStop function was not able to verify that SPT hw state machine has been reset to "START" state.
                                        This signals a hardware fault.*/

    RSDK_SPT_RET_WARN_UNEXPECTED_STOP, /**< Driver API warning: Spurious SPT_IRQ_ECS interrupt was triggered by the CS_STATUS0[PS_STOP] bit,
                                            while the SPT command sequencer did not reach a STOP instruction, or the SPT Driver
                                            had not launched any kernel. */
    RSDK_SPT_RET_ERR_HIST_OVF0,     /**< SPT hw error: HIST overflow. HIST_OVF_STATUS0 register value is passed to the user callback (rsdkSptIsrCb_t) */
    RSDK_SPT_RET_ERR_HIST_OVF1,     /**< SPT hw error: HIST overflow. HIST_OVF_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t) */
    RSDK_SPT_RET_ERR_IRQ_REG,       /**< SPT error: the irq handler was not registered. */
    RSDK_SPT_RET_ERR_UNMAP_SPT_MEM, /**< Driver error: Failed to unmap SPT registers' addresses. */

#if defined(S32R45) || defined(S32R41) || defined(SAF85XX)
    RSDK_SPT_RET_ERR_WR,            /**< SPT hw error: WR or SPR access error.
                                         WR_ACCESS_ERR_REG register value is passed to the user callback (rsdkSptIsrCb_t)*/
#if defined(S32R45) || defined(S32R41)
    RSDK_SPT_RET_ERR_ILLOP_SCS0,    /**< SPT hw error: Illegal SPT instruction in Slave Command Sequencer0.
                                    SCS0_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/
    RSDK_SPT_RET_ERR_ILLOP_SCS1,    /**< SPT hw error: Illegal SPT instruction in Slave Command Sequencer1.
                                    SCS1_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/
#if defined(S32R45)
    RSDK_SPT_RET_ERR_ILLOP_SCS2,    /**< SPT hw error: Illegal SPT instruction in Slave Command Sequencer2.
                                    SCS2_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/
    RSDK_SPT_RET_ERR_ILLOP_SCS3,    /**< SPT hw error: Illegal SPT instruction in Slave Command Sequencer3.
                                    SCS3_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/
    RSDK_SPT_RET_ERR_HW2_ACC, /**< SPT hw error: tried to execute an instruction with illegal operands or configuration, related to the 2nd instances
                                  of the SPT Accelerator modules (e.g. FFT2, MAXS2 etc). Triggers an ECS interrupt.
                                  HW2_ACC_ERR_STATUS register value is passed to the user callback (rsdkSptIsrCb_t) */
#endif//defined(S32R45)
#endif//defined(S32R45) || defined(S32R41)
    RSDK_SPT_RET_WARN_DRV_BUSY,     /**< Driver API warning: an API call is already in progress on another thread */
    RSDK_SPT_RET_ERR_API_INIT_LOCK_FAIL,    /**< Driver API error: could not init mutex for controlling multithreaded API call sequence */
    RSDK_SPT_RET_ERR_API_ENTER_LOCK_FAIL,   /**< Driver API error: could not lock mutex for controlling multithreaded API call sequence */
    RSDK_SPT_RET_ERR_API_ENTER_UNLOCK_FAIL, /**< Driver API error: could not unlock mutex for controlling multithreaded API call sequence */
    RSDK_SPT_RET_ERR_API_EXIT_LOCK_FAIL,    /**< Driver API error: could not lock mutex for controlling multithreaded API call sequence */
    RSDK_SPT_RET_ERR_API_EXIT_UNLOCK_FAIL,  /**< Driver API error: could not unlock mutex for controlling multithreaded API call sequence */
    RSDK_SPT_RET_ERR_THR_CREATE,  /**< Driver API error: could not create a thread for detecting OS kernel event in user space*/
    RSDK_SPT_RET_ERR_THR_TERM,  /**< Driver API error: could not terminate the thread used for detecting OS kernel event in user space*/
    RSDK_SPT_RET_ERR_OAL_COMM_INIT,  /**< Driver API error: could not initialize OAL communication channel for transmitting OS kernel events to user space*/
    RSDK_SPT_RET_ERR_CHECK_WATERMARK,       /**< Driver error: Failed to check if the watermark instruction is placed at the start of the kernel code. */
#else
    RSDK_SPT_RET_WARN_CS_AHB_BUSY, /**< Warning: during a 'blocking' call, the Driver has detected that the SPT has reached a STOP instruction
                                        (CS_STATUS0[PS_STOP] bit was set), but the SPT Command Sequencer DMA has not completed all of its AHB bus accesses.
                                        In this case there is a short time interval after returning from RsdkSptRun() (< 1us) when Work Register access
                                        by CPU must be avoided, otherwise it can cause the system to become non-responsive. Highly unlikely. */
#endif//defined(S32R45) || defined(S32R41) || defined(SAF85XX)
    RSDK_SPT_RET_ERR_INVALID_KERNEL, /**< Driver error: detected invalid SPT kernel code, which does not start with the mandatory watermarking instruction.
                                        See also #SPT_KERNEL_WATERMARK */
    RSDK_SPT_RET_ERR_HW_RST,         /**< SPT error: hardware is in unexpected RST state */

    RSDK_SPT_RET_ERR_OTHER = RSDK_SPT_STATUS_BASE + 0xFFFU, /**< Any other return status not covered above.
                                       No SPT error codes should be defined with a value greater than this one.*/
#if defined(S32R45) || defined(S32R41) || defined(SAF85XX)
    RSDK_DSP_RET_ERR_CMD_INVALID = RSDK_DSP_STATUS_BASE, /**< DSP Command Error: Command ID not supported. */
	RSDK_DSP_RET_ERR_CRC_INVALID,						 /**< DSP Command CRC Error: Recomputed CRC does not match with the received CRC */
	RSDK_DSP_RET_ERR_CMD_NO_DATA,						  /**< DSP Command Error: New command interrupt received, but no data available in the queue. */
	RSDK_DSP_RET_WARN_CMD_CRC_DISABLED, /**< DSP Command Warning: CRC verification is disabled. This might be intentional, or due to a communication error.*/
	RSDK_DSP_RET_ERR_EXCEPTION,			/**< TODO: remove? DSP Exception error. BBE32 exception context (registers exccause, exvaddr, ps, epc1) is saved to DSP_DEBUG<1..4>_REG.
	 	 	 	 	 	 	 	 	 	 	 "exccause" register value is also passed to the user callback (rsdkSptIsrCb_t dspIsrCb).
	 	 	 	 	 	 	 	 	 	 	 BBE32 execution is halted in the exception handler. */
	RSDK_DSP_RET_ERR_AXI_WRITE,		/**< DSP AXI WriteErr interrupt was received. BBE32 interrupt context (exccause, exvaddr, ps, epc1) is saved to DSP_DEBUG<1..4>_REG.
	 	 	 	 	 	 	 	 	 	 "exccause" register value is also passed to the user callback (rsdkSptIsrCb_t dspIsrCb) */
	RSDK_DSP_RET_ERR_MPU_CONFIG,	/**< DSP MPU map configuration has enountered an error. */
	RSDK_DSP_RET_ERR_INT_CONFIG,    /**< DSP interrupt configuration has enountered an error. */
	RSDK_DSP_RET_ERR_DISP_CONFIG,	/**< DSP dispatcher was not able to start its scheduling loop. It retuns to the caller. */
	RSDK_DSP_RET_ERR_INVALID_PARAM, /**< DSP Dispatcher API error: Input parameter value or combination of values not supported. */
	RSDK_DSP_RET_ERR_UNKNOWN = RSDK_DSP_STATUS_BASE +  + 0xFFFU,  /**< Unexpected error in DSP Dispatcher. Reason unknown.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 No DSP error codes should be defined with a value greater than this one.*/
#endif

#ifdef S32R45
	RSDK_LAX_ERR_INVALID_PARAM = RSDK_LAX_STATUS_BASE, /**< LAX Driver Error: Parameter value or combination not supported. */
	RSDK_LAX_ERR_NOT_INIT,  /**< LAX Driver Error:  Module not initialized.  */
	RSDK_LAX_ERR_CTXT, /**< LAX Driver Error: Invalid context used for this API call. */
	RSDK_LAX_ERR_EXEC, /**< LAX Driver Error: Operation failed during execution phase. */
	RSDK_LAX_ERR_TIMEOUT, /**< LAX Driver Error: Operation did not finish in due time. */
	RSDK_LAX_ERR_INTERNAL_BUF,/**< LAX Driver Error: Initialization failed due to insufficient 
							 memory region provided by user application for driver internal buffers */
	RSDK_LAX_ERR_ELD_BUF_SIZE,/**< LAX Driver Error: half the size of eld buffer is not AXI bus width aligned,
                                 see RSDK_LAX_ELD_BUF_SIZE */
	RSDK_LAX_ERR_ELD_FILE,/**< LAX Driver Error: ELD file error (e.g. ELF file structure/content/misalignment errors ) */
	RSDK_LAX_ERR_INTERNAL, /**< LAX Driver Error: Internal error. */
	RSDK_LAX_ERR_STATE,	/**< LAX Driver Error: API call not supported in current operation mode. */
	RSDK_LAX_ERR_ERROR_STATE, /**< LAX Driver Error: Module in ERROR state, after an unsuccessful recovery.
										Module is permanently disabled */
	RSDK_LAX_ERR_CRC_DIFF,  /**< LAX Driver Error: redundant execution finalized with different CRC computed on the two LAX cores */
	RSDK_LAX_ERR_EVT_THREAD, /**< LAX Driver Error: Could not create the event thread */
	RSDK_LAX_ERR_GRAPH_REG, /**< LAX Driver Error: LAX graph registration finished with error */
	RSDK_LAX_ERR_BOOT_SEQ, /**< LAX Dispathcer Error: Unsuccessful boot sequence in dispatcher. */


	RSDK_LAX_ERR_ENOENT,       		/**< No such file exists */
	RSDK_LAX_ERR_NOCMDBUF,     		/**< No CMDBUF section in ELF file */
	RSDK_LAX_ERR_EINTR,        		/**< Interrupted system call */
	RSDK_LAX_ERR_EIO,          		/**< I/O error */
	RSDK_LAX_ERR_START_FAIL,   		/**< Bad LAX startup sequence msg */
	RSDK_LAX_ERR_OPEN_FAIL,    		/**< Cannot open log, ELF image or /dev/mem */
	RSDK_LAX_ERR_CLOSE_FAIL,   		/**< Error closing log file or device */
	RSDK_LAX_ERR_EBADF,        		/**< Bad LAX handle */
	RSDK_LAX_ERR_ETIMEOUT,     		/**< Shutdown/wakeup hardware time out */
	RSDK_LAX_ERR_ENOMEM,       		/**< No device buffer / queue space left */
	RSDK_LAX_ERR_EACCESS,      		/**< File permission denied */
	RSDK_LAX_ERR_MISALIGNED,   		/**< Command/SPM buffer is not AXI aligned */
	RSDK_LAX_ERR_EBUSY,        		/**< Device is already open */
	RSDK_LAX_ERR_EINVOFFSET,   		/**< ELF buffer is not AXI aligned */
	RSDK_LAX_ERR_ENODEV,       		/**< No such device */
	RSDK_LAX_ERR_OTHER,        		/**< Unknown error */
	RSDK_LAX_ERR_EINVAL,       		/**< Invalid or NULL argument */
	RSDK_LAX_ERR_EMSGSIZE,          /**< command exceeds %d bytes */
	RSDK_LAX_ERR_ENOSR,             /**< No Sequence ID available */
	RSDK_LAX_ERR_ENOBUFS,           /**< No Cmd circular buffer space */

	//DMA Errors
	RSDK_LAX_ERR_DMA_QUEUE_FULL,            /**< Error indicating DMA transmit request queue is full */
	RSDK_LAX_ERR_DMA_ENOMSG,                /**< Error indicating DMA transmit request queue is empty */
	RSDK_LAX_ERR_DMA_EBUSY,                 /**< Error indicating another DMA transmission is in progress */

	RSDK_LAX_ERR_RET_OAL,                   /**< Error indicating a failed OAL api call */
	RSDK_LAX_ERR_OAL_COMM_INIT,             /**< Error initializing OAL */
	RSDK_LAX_ERR_OAL_COMM_EXIT,             /**< Error cleaning up OAL */
	RSDK_LAX_ERR_OAL_COMM_DISPATCH,	        /**< Indicates a communication error at the interface level between kernel-space and user-space */
	RSDK_LAX_ERR_OAL_COMM_KERNEL_DRIVER,    /**< Indicates a communication error to low-level/kernel-space driver */
	RSDK_LAX_ERR_OAL_EVENT_REGISTER,        /**< Error in OAL api registering lax events */
	RSDK_LAX_ERR_OAL_EVENT_DEREGISTER,      /**< Error in OAL api deregistering lax events */
	RSDK_LAX_ERR_OAL_EVENT_TRIGGER,         /**< Error in OAL api while triggering a specific event in low-level driver*/

	RSDK_LAX_ERR_BOOT_MSG_MISSMATCH,        /**< Boot failure due to boot complete message mismatch */
	RSDK_LAX_ERR_BOOT_TIMEOUT,              /**< Boot failure due to timeout waiting for HANDSHAKE Ack msg */
	RSDK_LAX_ERR_BOOT_HANDSHAKE_FAIL,       /**< Boot failure due to HANDSHAKE Ack returned error */
#endif
    //-------------------------------------------------------------------------
    //Application level errors
    RSDK_HEAP_MEM_ALLOC_ERROR = RSDK_APP_STATUS_BASE, /**< Not enough space in heap buffer to allocate desired size*/
    RSDK_HEAP_MEM_FREE_ERROR,   /**< Start adress and array size do not match with current heap buffer state.
									May be due to trying to release a segment that was not the last allocated*/
	RSDK_SPT_RET_ERR_TRAM_CHK_FAIL/**< SPT error: TRAM memory check failed */
} rsdkStatus_t;

#ifdef __cplusplus
}
#endif

#endif /* RSDK_STATUS_H */
