/*
* Copyright 2022 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CSI2_TYPES_H
#define CSI2_TYPES_H

/**
*   @file
*
*   @internal
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
    #include "typedefs.h"
    #include "rsdk_csi2_driver_api.h"
    #if defined(linux)
        #include "Csi2_Driver_Module.h"
    #endif
    #include "Csi2_Defs.h"
#include "Csi2_Cfg.h"
    #include "S32R45_MIPICSI2.h"


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
/**
 * @brief       Errors definitions.
 * @details     All defined errors for a unit, at all levels of the protocol.
 *
 */
#define CSI2_ERR_NO             0UL             /**< No errors                                                      */
/*<!-- PHY level errors ------------- -->                                                                           */
#define CSI2_ERR_PHY_SYNC       (1UL << 0u)     /**< PHY error - one cycle synchronization pattern error (soft err) */
#define CSI2_ERR_PHY_NO_SYNC    (1UL << 1u)     /**< PHY error - synchronization not realized.                      */
#define CSI2_ERR_PHY_ESC        (1UL << 2u)     /**< PHY error - escape synchronization not realized                */
#define CSI2_ERR_PHY_SESC       (1UL << 3u)     /**< PHY error - illegal command sequence in escape mode            */
#define CSI2_ERR_PHY_CTRL       (1UL << 4u)     /**< PHY error - illegal command sequence                           */
/*<!-- Packet level errors ------------- -->                                                                        */
#define CSI2_ERR_PACK_ECC1      (1UL << 6u)     /**< Packet error - one bit error corrected, the error bit
                                                 * position in Csi2_ErrorReportType::eccOneBitErrPos.               */
#define CSI2_ERR_PACK_ECC2      (1UL << 7u)     /**< Packet error - more than one bit error in packet, not corrected. */
#define CSI2_ERR_PACK_SYNC      (1UL << 8u)     /**< Packet error - frame sync error.                               */
#define CSI2_ERR_PACK_DATA      (1UL << 9u)     /**< Packet error - frame data error.                               */
#define CSI2_ERR_PACK_CRC       (1UL << 10u)    /**< Packet error - frame CRC error.
                                                 * The CRC expected value is reported in rsdkCsi2Report_t::expectedCRC,
                                                 * and the received one in rsdkCsi2Report_t::receivedCRC            */
#define CSI2_ERR_PACK_ID        (1UL << 11u)    /**< Packet error - ID error, the received ID is reported in
                                                 * rsdkCsi2Report_t::invalidPacketID.                               */
/*<!-- protocol level errors ------------- -->                                                                      */
#define CSI2_ERR_LINE_LEN       (1UL << 13u)    /**< Line length error.                                             */
#define CSI2_ERR_LINE_CNT       (1UL << 14u)    /**< Line count error.                                              */
#define CSI2_ERR_LINE_LEN_MD    (1UL << 15u)    /**< Line length error for metadata.                                */
#define CSI2_ERR_LINE_CNT_MD    (1UL << 16u)    /**< Line count error for metadata.                                 */
/*<!-- miscellaneous errors  ------------- -->                                                                      */
#define CSI2_ERR_HS_EXIT        (1UL << 17u)    /**< RDP error, High Speed exit error (EXIT_HS_ERROR)               */
#define CSI2_ERR_FIFO           (1UL << 18u)    /**< RDP error, internal FIFO error (FIFO_OVERFLOW_ERROR)           */
#define CSI2_ERR_BUF_OVERFLOW   (1UL << 19u)    /**< RDP error, internal buffer overrun (BUFFOVF)                   */
#define CSI2_ERR_AXI_OVERFLOW   (1UL << 20u)    /**< RDP error, AXI buffer overrun (BUFFOVFAXI)                     */
#define CSI2_ERR_AXI_RESPONSE   (1UL << 21u)    /**< RDP error, AXI response error (ERRRESP)                        */
/*<!-- spurious interrupts ------------- -->                                                                        */
#define CSI2_ERR_SPURIOUS_PHY   (1UL << 22u)    /**< Spurious error at PHY level.                                   */
#define CSI2_ERR_SPURIOUS_PKT   (1UL << 23u)    /**< Spurious error at Packet level.                                */
#define CSI2_ERR_SPURIOUS_EVT   (1UL << 24u)    /**< Spurious error at Data level.                                  */


/**
 * @brief       Events definitions.
 * @details     All defined events for a Virtual Channel.
 *
 */
#define CSI2_EVT_FRAME_START        (1UL << 0u) /**< Frame start event (FS)                                         */
#define CSI2_EVT_FRAME_END          (1UL << 1u) /**< Frame end event (FE)                                           */
#define CSI2_EVT_SHORT_PACKET       (1UL << 2u) /**< Generic short packet received (GNSP)                           */
#define CSI2_EVT_LINE_END           (1UL << 3u) /**< Line end event (LINEDONE)                                      */
#define CSI2_EVT_BIT_NOT_TOGGLE     (1UL << 5u) /**< Bit not toggled on a channel,
                                                 * reported in rsdkCsi2Report_t::notToggledBits.                    */
#define CSI2_EVT_NEXT_START_NOT_0   (1UL << 6u) /**< Bit signaling that the next frame start will not be at
                                                 * the beginning of the buffer                                      */


/*<!-- Specific min/max API working parameters. -------------- -->                                              */
#define CSI2_MIN_NR_LANES              1u          /**< Minimum number of physical lanes to be used             */
#define CSI2_MIN_VC_BUF_NR_LINES       1u          /**< Minimum number of VC to be defined                      */
#define CSI2_MAX_ANTENNA_NR            4u          /**< Maximum number of receiving channels (real/complex)     */
#define CSI2_MIN_VAL_UNSIGNED          0x0000u     /**< Definition for minimum unsigned sample value (default)  */
#define CSI2_MIN_VAL_SIGNED            0xc000      /**< Definition for minimum signed sample value              */
#define CSI2_MAX_VAL_UNSIGNED          0xffffu     /**< Definition for maximum unsigned sample value            */
#define CSI2_MAX_VAL_SIGNED            0x7ff8      /**< Definition for maximum signed sample value              */

/*<!-- Frequency limits for each platform   ------------------ -->                                              */
#define CSI2_MAX_RX_FREQ               2500u       /**< Max. 2.5Gbps input data for \b S32R45/S32R294           */
#define CSI2_MIN_RX_FREQ               80u         /**< Min. 80Mbps input data for \b S32R45/S32R294            */

/*<!-- Some specific statistics definitions ------------------ -->                                              */
#define CSI2_OFFSET_AUTOCOMPUTE       0x7fff       /**< Definition for auto computing offset for incoming data  */
#define CSI2_LINE_STAT_LENGTH          80U         /* the length of statistic data                         */

#define CSI2_5TH_CHANNEL_ON         0x400u         /* bit to enable the fifth channel                          */
#define CSI2_NORM_DTYPE_MASK        0x3fu          /* the mask for the normal data types to be received        */
#define CSI2_MAX_VC_MASK            0x3u           /* the mask for max Virtual Channel                         */
#define CSI2_CBUF0_ENA_MASK         0x100u         /* the mask for first buffer                                */

#if defined(TRACE_ENABLE)
    #define CSI2_TRACE(a,b,c) RsdkTraceLogEvent(a,b,c)
#else
    #define CSI2_TRACE(a,b,c)
#endif


/*==================================================================================================
*                                              ENUMS
==================================================================================================*/
/**
 * @brief       The data types accepted by the MIPI-CSI2 interface.
 * @details     The data types accepted by the MIPI-CSI2 interface.
 *
 */
typedef enum {
    CSI2_DATA_TYPE_EMBD      = 0x12u,         /**< type embedded           */
    CSI2_DATA_TYPE_YUV422_8  = 0x1Eu,         /**< YUV422 - 8 bits         */
    CSI2_DATA_TYPE_YUV422_10 = 0x1Fu,         /**< YUV422 - 10 bits        */
    CSI2_DATA_TYPE_RGB565    = 0x22u,         /**< RGB565                  */
    CSI2_DATA_TYPE_RGB888    = 0x24u,         /**< RGB888                  */
    CSI2_DATA_TYPE_RAW8      = 0x2Au,         /**< RAW-8                   */
    CSI2_DATA_TYPE_RAW10     = 0x2Bu,         /**< RAW-10                  */
    CSI2_DATA_TYPE_RAW12     = 0x2Cu,         /**< RAW-12  - radar default */
    CSI2_DATA_TYPE_RAW14     = 0x2Du,         /**< RAW-14                  */
    CSI2_DATA_TYPE_RAW16     = 0x2Eu,         /**< RAW-14                  */
    CSI2_DATA_TYPE_USR0      = 0x30u,         /**< user defined data ...   */
    CSI2_DATA_TYPE_USR1      = 0x31u,
    CSI2_DATA_TYPE_USR2      = 0x32u,
    CSI2_DATA_TYPE_USR3      = 0x33u,
    CSI2_DATA_TYPE_USR4      = 0x34u,
    CSI2_DATA_TYPE_USR5      = 0x35u,
    CSI2_DATA_TYPE_16_FROM_8 = 0x36u,         /**< specific  data type; data will be received as RAW8 data,
                                                        but the output will be as 16 bits data,
                                                        one sample will have 2 bytes of data */
    CSI2_DATA_TYPE_USR7      = 0x37u,         /**< ... user defined data  */
#if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
    /* up here - normal MIPI-CSI2 data types
     * down here, special "data types" for auxiliary data
     * it must not be specified for the buffer to receive the normal data
     * it must be specified only for auxiliary buffer to receive auxiliary data and must be
     * similar to the CSI2_VC_BUF_5TH_CH_... mask specified for outputDataMode                                  */
    CSI2_DATA_TYPE_AUX_0_NO_DROP    = 0x40u,  /**< auxiliary, mode 0, no drop, mask                             */
    CSI2_DATA_TYPE_R12_A0_NO_DROP   = 0x6Cu,  /**< RAW12 + auxiliary, mode 0, no drop, type to be used          */
    CSI2_DATA_TYPE_AUX_0_DR_1OF2    = 0x80u,  /**< auxiliary, mode 0, drop 1 of 2, mask                         */
    CSI2_DATA_TYPE_R12_A0_DR_1OF2   = 0xACu,  /**< RAW12 + auxiliary, mode 0, drop 1 of 2, type to be used      */

    CSI2_DATA_TYPE_AUX_0_DR_3OF4    = 0xc0u,  /**< auxiliary, mode 0, drop 3 of 4, mask                         */
    CSI2_DATA_TYPE_R12_A0_DR_3OF4   = 0xECu,  /**< RAW12 + auxiliary, mode 0, drop 3 of 4, type to be used      */
    CSI2_DATA_TYPE_AUX_1_NO_DROP    = 0x100u, /**< auxiliary, mode 1, no drop, mask                             */
    CSI2_DATA_TYPE_R12_A1_NO_DROP   = 0x12Cu, /**< RAW12 + auxiliary, mode 1, no drop, <b>type to be used       */
#endif
    CSI2_DATA_TYPE_MAX              = 0x13Fu, /**< stream type maximum, not used                                */
} Csi2_DataStreamType;


/**
 * @brief       CSI2 units enumeration.
 * @details     Adjusted to the specific platform.
 *
 */
typedef enum
{
    CSI2_UNIT_0 = 0,           /**< First unit (MIPICSI2_0)                         */
    CSI2_UNIT_1,               /**< Second unit (MIPICSI2_1)                        */
    CSI2_UNIT_2,               /**< Third unit (MIPICSI2_2)                         */
    CSI2_UNIT_3,               /**< Fourth unit (MIPICSI2_3)                        */
    CSI2_MAX_UNITS             /**< The units limit, to not use in procedure call   */
} Csi2_UnitIdType;


/**
 * @brief       CSI2 Virtual Channel enumeration.
 * @details     Adjusted to the specific platform.
 *
 */
typedef enum
{
    CSI2_VC_0 = 0,             /**< Virtual Channel 0                                  */
    CSI2_VC_1,                 /**< Virtual Channel 1                                  */
    CSI2_VC_2,                 /**< Virtual Channel 2                                  */
    CSI2_VC_3,                 /**< Virtual Channel 3                                  */
    CSI2_MAX_VC                /**< Virtual channels number limit for Csi2 unit, to not use in procedure call */
} Csi2_VirtChnlIdType;


/**
 * @brief       CSI2 lanes enumeration.
 * @details     Depending the usage, this can means the lane position or number of lines used
 *
 */
typedef enum
{
    CSI2_LANE_0 = 0,           /**< first lane / one lane                                       */
    CSI2_LANE_1,               /**< second lane / two lanes                                     */
    CSI2_LANE_2,               /**< third lane / three lanes                                    */
    CSI2_LANE_3,               /**< fourth lane / four lanes                                    */
    CSI2_MAX_LANE              /**< lanes (maximum) per CSI2 unit, to not use in procedure call */
} Csi2_LaneIdType;

/**
 * @brief       CSI2 lanes status enumeration.
 * @details     Depending the interface moment, a lane can be in one of these states.
 *
 */
typedef enum
{
    CSI2_LANE_STATE_MARK,          /**< The Rx lane in Mark-1 state.                            */
    CSI2_LANE_STATE_ULPA,          /**< The Rx lane in Ultra Low Power State.                   */
    CSI2_LANE_STATE_STOP,          /**< The Rx lane in Stop state.                              */
    CSI2_LANE_STATE_REC,           /**< The Rx lane is receiving data.                          */
    CSI2_LANE_STATE_VRX,           /**< The Rx lane is receiving valid data.                    */
    CSI2_LANE_STATE_ON,            /**< The Rx lane active, but not currently receiving data.   */
    CSI2_LANE_STATE_OFF,           /**< The Rx lane inactive.                                   */
    CSI2_LANE_STATE_ERR,           /**< The requested parameters are not correct.               */
} Csi2_LaneStatusType;


/**
 * @brief       Enum for ADC channels.
 * @details     Channels E/F/G and H are available only for complex data input from the Front End (see
 *              \ref CSI2_VC_BUF_REAL_DATA / \ref CSI2_VC_BUF_COMPLEX_DATA).
 *              For real data only channels A/B/C and D are available, according to the settings.<br>
 *              Do not use \b CSI2_MAX_CHANNEL in procedures calls.<br>
 *
 */
typedef enum {
    CSI2_CHANNEL_A = 0,
    CSI2_CHANNEL_B,
    CSI2_CHANNEL_C,
    CSI2_CHANNEL_D,
    CSI2_CHANNEL_E,
    CSI2_CHANNEL_F,
    CSI2_CHANNEL_G,
    CSI2_CHANNEL_H,
    CSI2_MAX_CHANNEL,
} Csi2_ChannelIdType;


#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
/**
 * @brief       Auto DC offset calculation (statistics management) type enum.
 * @details     If auto computation for DC offset is required, only one of these value is accepted.
 *              If no auto computation for DC offset required, these values are ignored.
 *
 */
typedef enum {
    CSI2_AUTODC_NO = 0u,             /**< Auto compute DC not required                                          */
    CSI2_AUTODC_EVERY_LINE = 1u,     /**< Auto compute DC management after every line received                  */
    CSI2_AUTODC_AT_FE,               /**< Auto compute DC management only at Frame End.
                                         * All statistics in the receiving buffer (according to bufNumLines for
                                         * the Virtual Channel) will be processed                               */
    CSI2_AUTODC_LAST_LINE,           /**< Statistics management only for the last line received                 */
    CSI2_AUTODC_MAX                  /**< Statistics max limit, to not be used in application                   */
} Csi2_AutoDCComputeTimeType;


/**
 * @brief       Auto DC offset calculation (statistics management) type enum.
 * @details     If auto computation for DC offset is required, only one of these value is accepted.
 *              If no auto computation for DC offset required, these values are ignored.
 *
 */
typedef enum {
    CSI2_STAT_NO = 0u,              /**< Statistics not required to be added by the interface           */
    CSI2_STAT_YES = 1u,             /**< Statistics required to be added after the received data        */
    CSI2_STAT_MAX                   /**< Statistics max limit, to not be used in application            */
} Csi2_StatisticsType;
#endif


/**
 * @brief       Structure which keep the possible values for driver status
 *
 */
typedef enum {
    CSI2_DRIVER_STATE_NOT_INITIALIZED = 0,      /* driver not initialized           */
    CSI2_DRIVER_STATE_ON,                       /* driver initialized               */
    CSI2_DRIVER_STATE_OFF,                      /* driver off                       */
    CSI2_DRIVER_STATE_STOP,                     /* driver stopped                   */
} Csi2_DriverStateType;


/**
 * @brief       Setup options for DPHY layer
 * @details     There are few options for DPHY setup which application can ask.
 *              The two main options are : calibration speed-up and STOP state on data lanes at setup time
 *
 */
typedef enum {
    CSI2_DPHY_INIT_SIMPLE = 0u,    /**< Normal calibration , no STOP state to be reached                    */
    CSI2_DPHY_INIT_SHORT_CALIB,    /**< Short calibration, with previous calibration values,
                                         * no STOP state to be reached                                      */
    CSI2_DPHY_INIT_W_STOP_STATE,   /**< Normal calibration, STOP state to be reached in about 1.2ms or
                                         * setup error reported */
    CSI2_DPHY_INIT_SHORT_AND_STOP = CSI2_DPHY_INIT_SHORT_CALIB | CSI2_DPHY_INIT_W_STOP_STATE,
                                        /**< Short calibration, STOP state to be reached in about 1.2ms     */
    CSI2_DPHY_INIT_MAX = CSI2_DPHY_INIT_SHORT_AND_STOP + 1,
                                        /**< Setup max limit, not to be used in application                  */
} Csi2_DphySetupOptionsType;


/**
 * @brief       Input/output data definitions enumeration.
 * @details     All defined parameters for data flow manipulation.
 *
 */
typedef enum {
    /* data type, real or complex                                                                               */
    CSI2_VC_BUF_REAL_DATA         = 0x00u,      /**< The data coming is real (default)                          */
    CSI2_VC_BUF_COMPLEX_DATA      = 0x10u,      /**< The data coming is complex                                 */
    /* fifth channel/auxiliary data on/off switch                                                               */
    CSI2_VC_BUF_FIFTH_CH_OFF      = 0x00u,      /**< Disable the fifth channel (default)                        */
    CSI2_VC_BUF_FIFTH_CH_ON       = 0x20u,      /**< Enable the fifth channel                                   */
    /* fifth channel/auxilary data mode                                                                         */
    CSI2_VC_BUF_5TH_CH_M0_NODROP  = 0x00u,      /**< The data coming from fifth channel not dropped (default)   */
    CSI2_VC_BUF_5TH_CH_M0_1_OF_2  = 0x40u,      /**< The data coming from fifth channel dropped one of two      */
    CSI2_VC_BUF_5TH_CH_M0_3_OF_4  = 0x80u,      /**< The data coming from fifth channel dropped three of four   */
    CSI2_VC_BUF_5TH_CH_MODE_1     = 0xc0u,      /**< The data coming from fifth channel not dropped (mode one)  */
    /* output data mode                                                                                         */
    CSI2_VC_BUF_OUT_ILEAVED       = 0x00u,      /**< The data is output interleaved (default)                   */
    CSI2_VC_BUF_OUT_TILE8         = 0x100u,     /**< The data is output tile8                                   */
    CSI2_VC_BUF_OUT_TILE16        = 0x200u,     /**< The data is output tile16                                  */
    /* data sign flip                                                                                           */
    CSI2_VC_BUF_NO_FLIP_SIGN      = 0x000u,     /**< The data is output as unsigned (default)                   */
    CSI2_VC_BUF_FLIP_SIGN         = 0x400u,     /**< The data sign is fliped                                    */
    /* other data output parameters                                                                             */
    CSI2_VC_BUF_SWAP_RAW8         = 0x800u,     /**< Swap bytes for RAW8 data received                          */
    CSI2_VC_BUF_RAW16_MSB_F       = 0x1000u,    /**< For RAW16, first is the MSB (not the LSB, as default)      */
    CSI2_VC_BUF_WRITE_ALL_DATA    = 0x2000u,    /**< Received data before FrameStart, but after setup,
                                                 * is written into the memory                                   */
} Csi2_DataManipulationType;


#if (CSI2_GPIO_USED == STD_ON) || (CSI2_SDMA_USED == STD_ON)
/**
 * @brief       GPIO/SDMA trigger enablement enumeration.
 * @details     All defined GPIO/SDMA triggers to be handled.
 *
 */
typedef enum {
/*<!-- Identifiers for GPIO/SDMA (external triggers) enablements ----------- -->                                */
    CSI2_REQ_ETRG_ENA_ON_ERROR     = 0x01u,     /**< Enable external trigger on VC error
                                                             *  (linecount, line len, crc/ecc, no sync)         */
    CSI2_REQ_ETRG_ENA_ON_PACKET    = 0x02u,     /**< Enable external trigger on VC packet received              */
    CSI2_REQ_ETRG_ENA_ON_PF        = 0x04u,     /**< Enable external trigger on VC packet and frame             */
} Csi2_ExternalTriggerType;


/**
 * @brief       GPIO/SDMA events definitions enumeration.
 * @details     All defined GPIO/SDMA events to be handled, for all defined triggers.
 *
 */
typedef enum {
    /* Frame boundaries events                                                                                  */
    CSI2_REQ_ETRG_EVT_FRAME_START      = 0x00u,  /**< Default, frame start will do a GPIO trigger               */
    CSI2_REQ_ETRG_EVT_FRAME_END        = 0x10u,  /**< Trigger at frame end                                      */
    /* Chirp boundaries events                                                                                  */
    CSI2_REQ_ETRG_EVT_CHIRP_START      = 0x20u,  /**< Trigger at chirp start                                    */
    CSI2_REQ_ETRG_EVT_CHIRP_END        = 0x30u,  /**< Trigger at chirp end                                      */
    /* Packet management events                                                                                 */
    CSI2_REQ_ETRG_EVT_PACK_EMBD        = 0x00u,  /**< Default, trigger on embedded data reception start         */
    CSI2_REQ_ETRG_EVT_PACK_USER        = 0x04u,  /**< Trigger on user data packet                               */
    CSI2_REQ_ETRG_EVT_PACK_RAW         = 0x08u,  /**< Trigger on RAW packet reception                           */
    CSI2_REQ_ETRG_EVT_PACK_RGB         = 0x0cu,  /**< Trigger on RGB data packet                                */
    /* Error trigers                                                                                            */
    CSI2_REQ_ETRG_EVT_ERR_LINECNT      = 0x00u,  /**< Default, trigger on line count error                      */
    CSI2_REQ_ETRG_EVT_ERR_LINLEN       = 0x01u,  /**< Trigger on line length error                              */
    CSI2_REQ_ETRG_EVT_ERR_CRCECC       = 0x02u,  /**< Trigger on CRC or ECC error                               */
    CSI2_REQ_ETRG_EVT_ERR_NOSYNC       = 0x03u,  /**< Trigger on synchronization missing                        */
} Csi2_ExternalEventsType;
#endif


/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
 *  @brief      Necessary CSI2 pointer to memory, as defined in the platform header files
 */
typedef MIPICSI2_Type    GENERIC_CSI2_Type;

/**
 *  @brief      Necessary typedef for uintptr, as AUTOSAR doesn't offer one.
 */
typedef uintptr_t   uintptr;


/**
 * @brief       Structure for short packet received.
 * @details     Usually this data is used in an error callback, related to CSI2_PATH_ERR_IRQ_ID interrupt,
 *              in Csi2_ErrorReportType::shortPackets.
 *
 */
typedef struct {
    uint8       dataID;             /**< Identifier (ID) of the packet      */
    uint16      dataVal;            /**< Data value (16 bits)               */
} Csi2_ShortPacketType;


/**
 * @brief       Structure for line length error or number of lines received error in the received frame.
 * @details     Usually this data is used in an error callback, related to CSI2_EVENTS_IRQ_ID interrupt,
 *              in Csi2ErrorReportType::lineLengthErr.
 *
 */
typedef struct {
    uint32      linePoz;            /**< The first line with length problem (if line/chirp length error) or the total
                                     * number of lines received (if number of lines error)                      */
    uint32      lineLength;         /**< The received length (different from the expected one), valid only if line
                                     * length error is signaled                                                 */
} Csi2_LineLenErrType;


#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
/**
 * @brief       Structure for channel statistics, for the entire frame.
 * @details     This is for driver internal purposes at frame level.
 *
 */
typedef struct {
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
    sint64   channelSum;            /* chirp channel sum                */
    sint16   channelDC;             /* current channel DC offset        */
    sint16   reqChannelDC;          /* current channel DC offset        */
#endif
    sint16   channelMin;            /* chirp channel min                */
    sint16   channelMax;            /* chirp channel max                */
    uint16   channelBitToggle;      /* chirp channel bit toggle         */
} Csi2_ChFrameStatType;
#endif


/**
 * @brief       Structure for VirtualChannel configuration.
 * @details     The structure hold all necessary parameters to setup the reception of the data for the
 *              Virtual Channel (VC).
 *
 * @note        Assuming any VC is receiving only one data_type, so using only one buffer.
 *
 */
typedef struct {
    uint16      streamDataType;                 /**< Data type to be received                                       */
    uint8       channelsNum;                    /**< Channels number for this VC; equal to antennas number for radar */
    uint8       vcEventsReq;                    /**< Mask for IRQ requested events for VC (i.e. \ref
                                                 * CSI2_EVT_FRAME_END or similar)                                    */

    uint16      expectedNumSamples;             /**< Expected line length for receiving in terms of samples/pixels/etc.
                                                 * per channel (antenna for radar)                                   */
    uint16      expectedNumLines;               /**< Expected number of lines/chirps per frame                       */
    uint16      bufNumLines;                    /**< Available number of complete length lines/chirps (including chirp
                                                 * statistics to be received in buffer, at least 1<br>
                                                 * @note For internal software reasons,
                                                 * (expectedNumLines % bufNumLines) must not be 1                    */
    uint16      bufLineLen;                     /**< The available line length in buffer, per line (16 bytes aligned)
                                                 * must include 80 supplementary bytes for line/chirp statistics     */
    uint16      outputDataMode;                 /**< The mode data is output in the buffer and other information
                                                 * about input data, see above : CSI2_VC_BUF_... family definitions
                                                 * i.e. CSI2_VC_BUF_COMPLEX_DATA.
                                                 * This parameter must be identical for data buffer and for
                                                 * correspondent auxiliary buffer.                                   */
    void        *bufDataPtr;                    /**< Pointer to data buffer (16 bytes aligned), physical address     */
    sint16      offsetCompReal[CSI2_MAX_CHANNEL];
                                                /**< Channel offset compensation for real part,<br>
                                                 * CSI2_OFFSET_AUTOCOMPUTE => auto config_DC,<br>
                                                 * any other = compensation (0 => no compensation).<br>
                                                 * The compensation must be specified at AD input level (12 bits).   */
    sint16      offsetCompImg[CSI2_MAX_CHANNEL];
                                                /**< Channel offset compensation for imaginary part, see
                                                 * \ref offsetCompReal  */
#if (CSI2_GPIO_USED == STD_ON)
    uint8       gpio1Mask, gpio2Mask;
                                                /**< masks for GPIO trigger handling (see CSI2_REQ_ETRG_EVT_...
                                                 * family, i.e. CSI2_REQ_ETRG_EVT_FRAME_START); to be
                                                 * functional, the triggers must be enabled (see gpio1EnaMask and
                                                 * gpio2EnaMask                                                      */
    uint8       gpio1EnaMask, gpio2EnaMask;
                                                /**< Mask for GPIO triggers enablement (see CSI2_REQ_ETRG_ENA_...
                                                 * family, i.e. CSI2_REQ_ETRG_ENA_ON_ERROR); must be set
                                                 * correctly to have external trigger of the related events.         */
#endif
#if (CSI2_SDMA_USED == STD_ON)
    uint8       sdma1Mask, sdma2Mask;
                                                /**< masks for SDMA trigger handling (see CSI2_REQ_ETRG_EVT_...
                                                 * family, i.e. CSI2_REQ_ETRG_EVT_FRAME_START); to be
                                                 * functional, the triggers must be enabled (see sdma1EnaMask and
                                                 * sdma2EnaMask)                                                     */
    uint8       sdma1EnaMask, sdma2EnaMask;
                                                /**< masks for SDMA trigger handling (see CSI2_REQ_ETRG_EVT_...
                                                 * family, i.e. CSI2_REQ_ETRG_EVT_FRAME_START); to be
                                                 * functional, the triggers must be enabled (see sdma1EnaMask and
                                                 * sdma2EnaMask)                                                     */
#endif
    uint8       bufNumLinesTrigger;             /**< Number of lines received in buffer to generate a callback
                                                 * to application, if the CSI2_EVT_LINE_END is required in
                                                 * vcEventsReq. It correspond to RM - Table 81-7 -
                                                 * LINEDONE field and cap. 81.8.41                                   */
} Csi2_VCParamsType;


/**
 * @brief       Structure for VirtualChannel configuration of metadata.
 * @details     The structure hold all necessary parameters to initialize the reception of other kind of data than
 *              radar data. The main purpose of this structure is to transmit the necessary information
 *              to manage the MetaData sent by the Radar Front-End after the data frame was sent.
 *
 * @note        There is possible to use only one metadata flow for each front-end Virtual Channel.
 *              The data is written into the buffer in the received order.
 *
 */
typedef struct {
    uint16      streamDataType;                 /**< Data type to be received                                       */
    uint16      expectedNumBytes;               /**< Expected line length, in bytes, for receiving data             */
    uint16      expectedNumLines;               /**< Expected number of lines/chirps per frame                      */
    uint16      bufNumLines;                    /**< Available number of complete length lines/chirps (including chirp
                                                 * statistics to be received in buffer, at least 1<br>
                                                 * \if (S32R45_DOCS || S32R294_DOCS) @note For internal software
                                                 * reasons, (\ref expectedNumLines % \ref bufNumLines ) must not be
                                                 * \b 1 \endif                                                      */
    uint16      bufLineLen;                     /**< The available line length in buffer, per line (16 bytes aligned)
                                                 * must include 80 supplementary bytes for line/chirp statistics    */
    void        *pBufData;                      /**< Pointer to data buffer (16 bytes aligned), physical address    */
} Csi2_MetaDataParamsType;


/**
 * @brief       Structure which describe the errors/events signaled by an interrupt request.
 * @details     The cumulated errors/events are put in this structure.
 *              Normally only the configured Virtual Channels will be reported.
 *
 */
typedef struct {
    uint8               unitId;                     /**< Unit id reporting the error                                 */
    uint32              errMaskU;                   /**< The cumulated masks for signaled errors at unit level;
                                                     * the possible masks are for:
                                                     * - PHY level errors, i.e. RSDK_CSI2_ERR_PHY_SYNC
                                                     * - packet errors, i.e. RSDK_CSI2_ERR_PACK_ECC1                 */
    uint32              errMaskVC[CSI2_MAX_VC];             /**< The cumulated masks for signaled errors at VC level;
                                                     *  the possible masks are for:
                                                     * - line errors definitions, i.e. RSDK_CSI2_ERR_LINE_LEN
                                                     * - all other miscellaneous                                     */
    uint8               evtMaskVC[CSI2_MAX_VC];     /**< Events mask for VCs, according to the requested events as
                                                     * required at setup - Csi2_VCParamsType->vcEventsReq
                                                     * the possible masks are defined by:
                                                     * - events masks :
                                                     *  CSI2_EVT_FRAME_START
                                                     *  CSI2_EVT_FRAME_END
                                                     *  CSI2_EVT_LINE_END
                                                     *  CSI2_EVT_SKEW_CALIB
                                                     *  CSI2_EVT_SHORT_PACKET
                                                     *  CSI2_EVT_BIT_NOT_TOGGLE
                                                     *  CSI2_EVT_NEXT_START_NOT_0                                    */
    uint8       invalidPacketID[CSI2_MAX_VC];       /**< The wrong packet ID received, reported if
                                                     * CSI2_ERR_PACK_ID - reported                                   */
    uint8       eccOneBitErrPos[CSI2_MAX_VC];       /**< The recovered position for ECC one bit error, if
                                                     * CSI2_ERR_PACK_ECC1 reported                                   */
    uint16      expectedCRC[CSI2_MAX_VC];           /**< Computed CRC for received data, if CSI2_ERR_PACK_CRC
                                                     * reported                                                      */
    uint16      receivedCRC[CSI2_MAX_VC];           /**< Real, received CRC if CSI2_ERR_PACK_CRC reported */
    uint16      notToggledBits[CSI2_MAX_CHANNEL];   /**< Bits mask for not toggled bits, for the channel, if
                                                     * CSI2_EVT_BIT_NOT_TOGGLE reported; the bit mask set to 1
                                                     * means bit not toggled                                         */
    uint8       lastDroppedData[CSI2_MAX_VC];       /**< The last data type dropped on each VC                       */
    Csi2_LineLenErrType  lineLengthErr[CSI2_MAX_VC]; /**< Specific data for line length error, if
                                                     * CSI2_ERR_LINE_LEN or CSI2_ERR_LINE_CNT bits
                                                     * are set                                                       */
    Csi2_ShortPacketType shortPackets[CSI2_MAX_VC];  /**< Specific data for short packet received,
                                                     * if CSI2_EVT_SHORT_PACKET bit is set.                          */
} Csi2_ErrorReportType;

/**
 * @brief   Definition of callback function type to be called by the CSI2 interrupt handler.
 * @details This callback must be used for processing a CSI2 error/event interrupt.<br>
 *          Simple callback function definition example : void CallbackCsi2(rsdkCsi2Report_t *pStruct) { ... }
 *
 * @param[in]    pReportingStruct* - the callback receives a pointer to a complete description of the error/event
 * @return       nothing
 *
 *
 * */
typedef void (*Csi2_IsrCbType)(Csi2_ErrorReportType *pReportingStruct);


/**
 * @brief       Structure for unit configuration.
 * @details     The structure accepts setup of all available Virtual Channels (VC), but using only one data
 *              type for each VC.
 *
 */
typedef struct {
    uint8           numLanesRx;                     /**< Number of lanes used to receive data; use Csi2_LaneIdType
                                                     *  to configure this.                                           */
    uint8           lanesMapRx[CSI2_MAX_LANE];      /**< Lanes mapping :
                                                     *  - first byte - the physical lane to be used as lane 1,
                                                     *  - second byte - the physical lane to be used as lane 2,
                                                     *  - etc.
                                                     *  use Csi2_LaneIdType to set these fields                      */
    uint8           initOptions;                    /**< Specific setup options for the unit, required by
                                                     * the application :
                                                     * - simple/normal calibration or quick/short calibration
                                                     * - wait for 5us or not wait for STOP states on data lanes
                                                     * use Csi2_DphySetupOptionsType to set this field.              */
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
    uint8           statManagement;                 /**< How the channel statistics are managed. Only channels having
                                                     * CSI2_OFFSET_AUTOCOMPUTE DC offset specified will be
                                                     * managed. Use one of CSI2_STAT_... definitions.
                                                     * Use Csi2_AutoDCComputeTimeType to set this field.             */
#endif
    uint32          rxClkFreq;                      /**< Receiving (Rx) frequency (between CSI2_MIN_RX_FREQ
                                                     * and CSI2_MAX_RX_FREQ), in Mbps.                               */
    Csi2_VCParamsType   *vcConfigPtr[CSI2_MAX_VC];  /**< pointers to VC configurations; ignored if NULL.
                                                     * At least VC 0 must be configured.                             */
#if (CSI2_AUXILIARY_DATA_USAGE == STD_ON)
    Csi2_VCParamsType   *auxConfigPtr[CSI2_MAX_VC]; /**< pointers to VC configurations, for auxiliary data only;
                                                     * The corresponding VC must have set
                                                     * CSI2_VC_BUF_FIFTH_CH_ON; if this pointer is NULL,
                                                     * the the auxiliary data will be dropped. If this pointer
                                                     * is not NULL, and the corresponding VC has
                                                     * CSI2_VC_BUF_FIFTH_CH_ON, it is assumed auxiliary data
                                                     * type 0; so, for auxiliary data type 1 the buffer must be
                                                     * configured, even the data is not used.                        */
#endif
#if (CSI2_METADATA_DATA_USAGE == STD_ON)
    Csi2_MetaDataParamsType *pMetaData[CSI2_MAX_VC]; /**< pointers to VC configurations for other data
                                                     * to be received on the other channels, with no
                                                     * statistics support, mainly for MetaData; ignored if NULL.
                                                     * It is not a must to use one of these for normal
                                                     * application, but if used it must be correlated with the
                                                     * Virtual Channel of the appropriate RadarData.                 */
#endif
    Csi2_IsrCbType          pCallback[RSDK_CSI2_MAX_IRQ_ID];    /**< the necessary callbacks :
                                                 * <table>
                            * <tr><td>errors in receive path</td><td>\ref RSDK_CSI2_RX_ERR_IRQ_ID</td></tr>
                            * <tr><td>errors in protocol & packet level</td><td>\ref RSDK_CSI2_PATH_ERR_IRQ_ID</td></tr>
                            * <tr><td>events</td><td>\ref RSDK_CSI2_EVENTS_IRQ_ID</td></tr>
                            * \if (S32R45_DOCS || S32R294_DOCS) <tr><td>turnaround and tx errors/events</td><td>
                            * \ref RSDK_CSI2_TX_ERR_IRQ_ID (for \b S32R45/S32R294 only) </td></tr> \endif
                            * </table>
                             * At least first callback pointer (\ref RSDK_CSI2_RX_ERR_IRQ_ID) must be not NULL.
                             * When an error is signaled, the appropriate callback is used to signal the occurrence and
                             * the associated data. So, any other necessary callback will use the first pointer if NULL
                             * specified for that interrupt position.<br> For events : only when a requested event
                             * signal appeared, the \ref RSDK_CSI2_EVENTS_IRQ_ID callback is used, if specified; if not,
                             * \ref RSDK_CSI2_RX_ERR_IRQ_ID callback will be used.                                  */
#if !defined(linux)
    rsdkCoreId_t                irqExecCore;    /**< Processor core to execute the irq code. Usually the current core.*/
    uint8_t                     irqPriority;    /**< Priority for the interrupt request execution                   */
#endif
} Csi2_SetupParamsType;


/**
 * @brief       Structure for VC driver parameters.
 * @details     Internal parameters for easy driver usage.
 *
 */
typedef struct {
    uint8       eventsMask;                     /* Rx events to be called back                                      */
    uint16      outputDataMode;                 /* the mode data is output in the buffer and other information      */
                                                /* about input data, see csi2.h file :                              */
                                                /* "various parameters concerning input data for outputDataMode"    */
    uint16      lastReceivedBufLine;            /* the last buffer line written by the interface                    */
    uint16      lastReceivedChirpLine;          /* the last chirp line received                                     */
#ifdef linux
    void        *pVirtData;                     // virtual memory pointer to buf data, from kernel
#endif
    Csi2_VCParamsType       *vcParamsPtr;       /* pointer to actual VC setup params                                */
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
    Csi2_ChFrameStatType    statDC[CSI2_MAX_CHANNEL];      /* statistics for DC computation for all channels        */
#endif
#if (CSI2_METADATA_DATA_USAGE == STD_ON)
    uint8       metaDataUsage;                  /* mask for metadata usage                                          */
#endif
} Csi2_VCDriverStateType;


/**
 * @brief       Structure which keep the necessary parameters for run-time.
 *
 */
typedef struct {
    /* unit level parameters                                                                            */
    Csi2_DriverStateType        driverState;        /* driver state                                     */
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
    Csi2_AutoDCComputeTimeType  statisticsFlag;     /* flag to process or not the channels statistics   */
#endif
    rsdkCsi2IsrCb_t             pCallback[RSDK_CSI2_MAX_IRQ_ID];    // the necessary callbacks :
                                                    // - errors in receive path
                                                    // - errors in datapath level
                                                    // - events
                                                    // - turnaround and tx errors/events
    /* VC level working params                                                                          */
    Csi2_VCDriverStateType      workingParamVC[CSI2_MAX_VC];
} Csi2_DriverParamsType;



/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/




#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CSI2_TYPES_H */
