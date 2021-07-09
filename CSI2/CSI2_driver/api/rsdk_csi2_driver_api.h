/*
* Copyright 2019-2021 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RSDK_CSI2_DRIVER_API_H
#define RSDK_CSI2_DRIVER_API_H

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include "typedefs.h"
#include "rsdk_glue_irq_register_api.h"
#endif

#include "rsdk_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                          CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      DEFINES AND MACROS
 ==================================================================================================*/
// clang-format off

/*!
 The macro definitions respect the following conventions :
 <table>
 <tr><th>Menemonic root                </th><th>Explanations </th></tr>
 <tr><td>\b RSDK_CSI2_ERR_PHY_...      </td><td>Errors reported by the PHY level of the MIPI-CSI2 interface,
                                         in rsdkCsi2Report_t::errMaskU</td></tr>
 <tr><td>\b RSDK_CSI2_ERR_PACK_...     </td><td>Errors reported at packet level of the MIPI-CSI2 interface,
                                         in rsdkCsi2Report_t::errMaskVC</td></tr>
 <tr><td>\b RSDK_CSI2_ERR_LINE_...     </td><td>Errors reported at data level of the MIPI-CSI2 interface,
                                         in rsdkCsi2Report_t::errMaskVC</td></tr>
 <tr><td>\b RSDK_CSI2_ERR_SPURIOUS_... </td><td>Spurious interrupt request, reported
                                         in rsdkCsi2Report_t::errMaskU</td></tr>
 <tr><td>\b RSDK_CSI2_ERR_...          </td><td>Other Rx level errors</td></tr>
 <tr><td>\b RSDK_CSI2_EVT_...          </td><td>Events related masks.<br>The same masks will be used for reporting
                                         errors (rsdkCsi2Report_t::evtMaskVC), but only if requested at
                                         interface initialization time (rsdkCsi2VCParams_t::vcEventsReq)</td></tr>
 <tr><td>\b RSDK_CSI2_VC_BUF_...       </td><td>Parameters for Virtual Channel received data specification.<br>
                                         Must be used at initialization time for
                                         rsdkCsi2VCParams_t::outputDataMode</td></tr>
 <tr><td>\b RSDK_CSI2_REQ_ETRG_EVT_... </td><td>Requests for GPIO/SDMA events programming.<br>Used only at
                                         initialization time, by rsdkCsi2VCParams_t::gpio1Mask,
                                         rsdkCsi2VCParams_t::gpio2Mask, rsdkCsi2VCParams_t::sdma1Mask or
                                         rsdkCsi2VCParams_t::sdma2Mask, depending on the specific events pad
                                         request.</td></tr>
 <tr><td>\b RSDK_CSI2_REQ_ETRG_ENA_... </td><td>Requests for GPIO/SDMA enablements.<br>Used only at
                                         initialization time, by rsdkCsi2VCParams_t::gpio1EnaMask,
                                         rsdkCsi2VCParams_t::gpio2EnaMask, rsdkCsi2VCParams_t::sdma1EnaMask or
                                         rsdkCsi2VCParams_t::sdma2EnaMask, depending on the specific events pad
                                         request.</td></tr>
 <tr><td>\b RSDK_CSI2_MIN_...          </td><td>Specific minimum values definition</td></tr>
 <tr><td>\b RSDK_CSI2_MAX_...          </td><td>Specific maximum values definition</td></tr>
 * </table>
\note The error report bits are not overlapping, no matter the reporting level - Unit or Virtual Channel.

*/

/** @addtogroup csi2_driver_api_const
* @{
*/

//<!-- PHY level errors ------------- -->
#define RSDK_CSI2_ERR_PHY_SYNC      (1UL << 0u)  /**< PHY error - one cycle synchronization pattern error (soft err)*/
#define RSDK_CSI2_ERR_PHY_NO_SYNC   (1UL << 1u)  /**< PHY error - synchronization not realized.                     */
#define RSDK_CSI2_ERR_PHY_ESC       (1UL << 2u)  /**< PHY error - escape synchronization not realized               */
#define RSDK_CSI2_ERR_PHY_SESC      (1UL << 3u)  /**< PHY error - illegal command sequence in escape mode           */
#define RSDK_CSI2_ERR_PHY_CTRL      (1UL << 4u)  /**< PHY error - illegal command sequence                          */

//<!-- Packet level errors ------------- -->
#define RSDK_CSI2_ERR_PACK_ECC1     (1UL << 6u)  /**< Packet error - one bit error corrected, the error bit
                                                  * position in rsdkCsi2Report_t::eccOneBitErrPos.                  */
#define RSDK_CSI2_ERR_PACK_ECC2     (1UL << 7u)  /**< Packet error - more than one bit error in packet, not corrected.*/
#define RSDK_CSI2_ERR_PACK_SYNC     (1UL << 8u)  /**< Packet error - frame sync error.                              */
#define RSDK_CSI2_ERR_PACK_DATA     (1UL << 9u)  /**< Packet error - frame data error.                              */
#define RSDK_CSI2_ERR_PACK_CRC      (1UL << 10u) /**< Packet error - frame CRC error.
                                                  * The CRC expected value is reported in rsdkCsi2Report_t::expectedCRC,
                                                  * and the received one in rsdkCsi2Report_t::receivedCRC           */
#define RSDK_CSI2_ERR_PACK_ID       (1UL << 11u) /**< Packet error - ID error, the received ID is reported in
                                                  * rsdkCsi2Report_t::invalidPacketID.                        */

//<!-- protocol level errors ------------- -->
#define RSDK_CSI2_ERR_LINE_LEN      (1UL << 13u) /**< Line length error.                                      */
#define RSDK_CSI2_ERR_LINE_CNT      (1UL << 14u) /**< Line count error.                                       */


//<!-- miscellaneous errors  ------------- -->
#define RSDK_CSI2_ERR_HS_EXIT       (1UL << 15u) /**< RDP error, High Speed exit error (EXIT_HS_ERROR)        */
#define RSDK_CSI2_ERR_FIFO          (1UL << 16u) /**< RDP error, internal FIFO error (FIFO_OVERFLOW_ERROR)    */
#define RSDK_CSI2_ERR_BUF_OVERFLOW  (1UL << 17u) /**< RDP error, internal buffer overrun (BUFFOVF)            */
#define RSDK_CSI2_ERR_AXI_OVERFLOW  (1UL << 18u) /**< RDP error, AXI buffer overrun (BUFFOVFAXI)              */
#define RSDK_CSI2_ERR_AXI_RESPONSE  (1UL << 19u) /**< RDP error, AXI response error (ERRRESP)                 */

//<!-- spurious interrupts ------------- -->
#define RSDK_CSI2_ERR_SPURIOUS_PHY  (1UL << 20u) /**< Spurious error at PHY level.                            */
#define RSDK_CSI2_ERR_SPURIOUS_PKT  (1UL << 21u) /**< Spurious error at Packet level.                         */
#define RSDK_CSI2_ERR_SPURIOUS_EVT  (1UL << 22u) /**< Spurious error at Data level.                           */


//<!-- Defined for future use, when Tx will be used ------ -->
#define RSDK_CSI2_TX_PKT           (1UL << 25u)  /**< TX error, undefined data type (PKTCMDERR)               */
#define RSDK_CSI2_TX_COMPLETION    (1UL << 26u)  /**< TX, transmission completion (IOCS)                      */
#define RSDK_CSI2_TX_TA_START      (1UL << 27u)  /**< TX, turnaround started (ONGOINGTA)                      */
#define RSDK_CSI2_TX_TA_END_MS     (1UL << 28u)  /**< TX, turnaround completed, master to slave (ONGOINGTA)   */
#define RSDK_CSI2_TX_TA_END_SM     (1UL << 29u)  /**< TX, turnaround completed, slave to master (ONGOINGTA)   */


//<!-- Masking parameters for CSI2 callback - events requesting/reporting -------------- -->
#define RSDK_CSI2_EVT_FRAME_START       (1UL << 0u) /**< Frame start event (FS)                                 */
#define RSDK_CSI2_EVT_FRAME_END         (1UL << 1u) /**< Frame end event (FE)                                   */
#define RSDK_CSI2_EVT_SHORT_PACKET      (1UL << 2u) /**< Generic short packet received (GNSP)                   */
#define RSDK_CSI2_EVT_LINE_END          (1UL << 3u) /**< Line end event (LINEDONE)                              */
#ifdef S32R294
#define RSDK_CSI2_EVT_SKEW_CALIB        (1UL << 4u) /**< Final calibration event (RXSKEWCALHS)                  */
#endif
#define RSDK_CSI2_EVT_BIT_NOT_TOGGLE    (1UL << 5u) /**< Bit not toggled on a channel,
                                                     * reported in rsdkCsi2Report_t::notToggledBits.            */
#define RSDK_CSI2_EVT_NEXT_START_NOT_0  (1UL << 6u) /**< Bit signaling that the next frame start will not be at
                                                     * the beginning of the buffer*/
#define RSDK_CSI2_EVT_IN_FRAME          (1UL << 7u) /**< Frame already started at the call moment               */

 //<!-- Specific min/max API working parameters. ------------- -->
#define RSDK_CSI2_MIN_NR_LANES                  1u      /**< Minimum number of physical lanes to be used        */
#define RSDK_CSI2_MIN_VC_BUF_NR_LINES           1u      /**< Minimum number of VC to be defined                 */
#define RSDK_CSI2_MAX_ANTENNA_NR                4u      /**< Maximum number of receiving channels (real/complex)*/

//<!-- Frequency limits for each platform --------------- -->
#define RSDK_CSI2_MAX_RX_FREQ                   2500u   /**< Max. 2.5Gbps input data for \b S32R45/S32R294      */
#define RSDK_CSI2_MAX_TX_FREQ                   2500u   /**< Max. 2.5Gbps output data for \b S32R45/S32R294     */

#define RSDK_CSI2_MIN_RX_FREQ                   80u     /**< Min. 80Mbps input data for \b S32R45/S32R294       */
#define RSDK_CSI2_MIN_TX_FREQ                   80u     /**< Min. 80Mbps output data for \b S32R45/S32R294      */


//<!-- Specific masking parameters for S32R45/S32R294 CSI2 programming. -------- -->

//<!-- Various parameters defining input/output data for outputDataMode ----------- -->
#define RSDK_CSI2_VC_BUF_REAL_DATA         0x00u       /**< The data coming is real (default)                   */
#define RSDK_CSI2_VC_BUF_COMPLEX_DATA      0x10u       /**< The data coming is complex                          */
#define RSDK_CSI2_VC_BUF_FIFTH_CH_OFF      0x00u       /**< Disable the fifth channel (default)                 */
#define RSDK_CSI2_VC_BUF_FIFTH_CH_ON       0x20u       /**< Enable the fifth channel                            */
#define RSDK_CSI2_VC_BUF_5TH_CH_M0_NODROP  0x00u       /**< The data coming from fifth channel not dropped (default)*/
#define RSDK_CSI2_VC_BUF_5TH_CH_M0_1_OF_2  0x40u       /**< The data coming from fifth channel dropped one of two*/
#define RSDK_CSI2_VC_BUF_5TH_CH_M0_3_OF_4  0x80u       /**< The data coming from fifth channel dropped three of four*/
#define RSDK_CSI2_VC_BUF_5TH_CH_MODE_1     0xc0u       /**< The data coming from fifth channel not dropped (mode one)*/
#define RSDK_CSI2_VC_BUF_OUT_ILEAVED       0x00u       /**< The data is output interleaved (default)            */
#define RSDK_CSI2_VC_BUF_OUT_TILE8         0x100u      /**< The data is output tile8                            */
#define RSDK_CSI2_VC_BUF_OUT_TILE16        0x200u      /**< The data is output tile16                           */
#define RSDK_CSI2_VC_BUF_FLIP_SIGN         0x400u      /**< The data sign is fliped                             */
#define RSDK_CSI2_VC_BUF_NO_FLIP_SIGN      0x000u      /**< The data is output as unsigned (default)            */
#define RSDK_CSI2_VC_BUF_SWAP_RAW8         0x800u      /**< Swap bytes for RAW8 data received                   */
#define RSDK_CSI2_VC_BUF_RAW16_MSB_F       0x1000u     /**< For RAW16, first is the MSB (not the LSB, as default)*/

#define RSDK_CSI2_OFFSET_AUTOCOMPUTE       0x7fff      /**< Definition for auto computing offset for incoming data*/
#define RSDK_CSI2_MIN_VAL_UNSIGNED         0x0000u     /**< Definition for minimum unsigned sample value (default)*/
#define RSDK_CSI2_MIN_VAL_SIGNED           0xc000      /**< Definition for minimum signed sample value          */
#define RSDK_CSI2_MAX_VAL_UNSIGNED         0xffffu     /**< Definition for maximum unsigned sample value        */
#define RSDK_CSI2_MAX_VAL_SIGNED           0x7ff8      /**< Definition for maximum signed sample value          */

//<!-- Identifiers for GPIO/SDMA (external triggers) events to be handled --------- -->
#define RSDK_CSI2_REQ_ETRG_EVT_FRAME_START      0x00u   /**< Default, frame start will do a GPIO trigger        */
#define RSDK_CSI2_REQ_ETRG_EVT_FRAME_END        0x10u   /**< Trigger at frame end                               */
#define RSDK_CSI2_REQ_ETRG_EVT_CHIRP_START      0x20u   /**< Trigger at chirp start                             */
#define RSDK_CSI2_REQ_ETRG_EVT_CHIRP_END        0x30u   /**< Trigger at chirp end                               */
#define RSDK_CSI2_REQ_ETRG_EVT_PACK_EMBD        0x00u   /**< Default, trigger on embedded data reception start  */
#define RSDK_CSI2_REQ_ETRG_EVT_PACK_USER        0x04u   /**< Trigger on user data packet                        */
#define RSDK_CSI2_REQ_ETRG_EVT_PACK_RAW         0x08u   /**< Trigger on RAW packet reception                    */
#define RSDK_CSI2_REQ_ETRG_EVT_PACK_RGB         0x0cu   /**< Trigger on RGB data packet                         */
#define RSDK_CSI2_REQ_ETRG_EVT_ERR_LINECNT      0x00u   /**< Default, trigger on line count error               */
#define RSDK_CSI2_REQ_ETRG_EVT_ERR_LINLEN       0x01u   /**< Trigger on line length error                       */
#define RSDK_CSI2_REQ_ETRG_EVT_ERR_CRCECC       0x02u   /**< Trigger on CRC or ECC error                        */
#define RSDK_CSI2_REQ_ETRG_EVT_ERR_NOSYNC       0x03u   /**< Trigger on synchronization missing                 */
#define RSDK_CSI2_REQ_ETRG_EVT_TRND_MTS         0x80u   /**< Trigger on master to slave turnaround              */
#define RSDK_CSI2_REQ_ETRG_EVT_TRND_STM         0x40u   /**< Trigger on slave to master turnaround              */

//<!-- Identifiers for GPIO/SDMA (external triggers) enablements ----------- -->
#define RSDK_CSI2_REQ_ETRG_ENA_ON_ERROR         0x01u   /**< Enable external trigger on VC error
                                                         *  (linecount, line len, crc/ecc, no sync)             */
#define RSDK_CSI2_REQ_ETRG_ENA_ON_PACKET        0x02u   /**< Enable external trigger on VC packet received      */
#define RSDK_CSI2_REQ_ETRG_ENA_ON_PF            0x04u   /**< Enable external trigger on VC packet and frame     */

#define RSDK_CSI2_VC_BUFF_STATISTICS            80u     /**< The necessary buffer length for channels statistics
                                                         * at chirp buffer end                                  */

/** @}*/

// clang-format off
/*==================================================================================================
 *                                             ENUMS
 ==================================================================================================*/

/*!
Enumerations data types usage :
<table>
<tr><th>Type</th><th>Usage</th></tr>
<tr><td>\ref rsdkCsi2DataStreamType_t</td><td>Must be used at initialization time (rsdkCsi2VCParams_t::streamDataType).
                                    </td></tr>
<tr><td>\ref rsdkCsi2UnitId_t       </td><td>Must be used at any time when necessary to identify the MIPI-CSI2 unit
                                    to be used (i.e. for any API call).<br>It is used to identify the unit at a callback
                                    origin (rsdkCsi2Report_t::unitId).</td></tr>
<tr><td>\ref rsdkCsi2VirtChnlId_t   </td><td>Used to identify the Virtual Channel (i.e. \ref RsdkCsi2GetFramesCounter).
                                    </td></tr>
<tr><td>\ref rsdkCsi2LaneEnum_t     </td><td>Used to identify the lanes. At initialization time is used for lanes
                                    swapping definitions in rsdkCsi2InitParams_t::lanesMapRx array.</td></tr>
<tr><td>\ref rsdkCsi2Channel_t      </td><td>Parameter for channel identification for buffer output data.<br>
                                    \ref RSDK_CSI2_MAX_CHANNEL element define the maximum channel number available for
                                    each platform. For \b S32R45 or \b S32R294, \ref RSDK_CSI2_MAX_CHANNEL can be 8 only
                                    if complex data is received. For real data, the received number of channels is
                                    maximum 4.</td></tr>
<tr><td>\ref rsdkCsi2IrqId_t        </td><td>Used for interrupt request management/identification. Mainly, \ref
                                    RSDK_CSI2_MAX_IRQ_ID is used to define length of the interrupts array.  </td></tr>
</table>

*/


/** @addtogroup csi2_driver_api_data_type
* @{
*/

/**
 * @brief       The data types accepted by the MIPI-CSI2 interface.
 * @details     The data types accepted by the MIPI-CSI2 interface.
 *
 */
typedef enum {
    RSDK_CSI2_DATA_TYPE_EMBD      = 0x12u,         /**< type embedded           */
    RSDK_CSI2_DATA_TYPE_YUV422_8  = 0x1Eu,         /**< YUV422 - 8 bits         */
    RSDK_CSI2_DATA_TYPE_YUV422_10 = 0x1Fu,         /**< YUV422 - 10 bits        */
    RSDK_CSI2_DATA_TYPE_RGB565    = 0x22u,         /**< RGB565                  */
    RSDK_CSI2_DATA_TYPE_RGB888    = 0x24u,         /**< RGB888                  */
    RSDK_CSI2_DATA_TYPE_RAW8      = 0x2Au,         /**< RAW-8                   */
    RSDK_CSI2_DATA_TYPE_RAW10     = 0x2Bu,         /**< RAW-10                  */
    RSDK_CSI2_DATA_TYPE_RAW12     = 0x2Cu,         /**< RAW-12  - radar default */
    RSDK_CSI2_DATA_TYPE_RAW14     = 0x2Du,         /**< RAW-14                  */
    RSDK_CSI2_DATA_TYPE_USR0      = 0x30u,         /**< user defined data ...   */
    RSDK_CSI2_DATA_TYPE_USR1      = 0x31u,
    RSDK_CSI2_DATA_TYPE_USR2      = 0x32u,
    RSDK_CSI2_DATA_TYPE_USR3      = 0x33u,
    RSDK_CSI2_DATA_TYPE_USR4      = 0x34u,
    RSDK_CSI2_DATA_TYPE_USR5      = 0x35u,
    RSDK_CSI2_DATA_TYPE_16_FROM_8 = 0x36u,         /**< specific RSDK data type; data will be received as RAW8 data,
                                                        but the output will be as 16 bits data,
                                                        one sample will have 2 bytes of data */
    RSDK_CSI2_DATA_TYPE_USR7      = 0x37u,         /**< ... user defined data  */
    // up here - normal MIPI-CSI2 data types
    // down here, special "data types" for auxiliary data
    // it must not be specified for the buffer to receive the normal data
    // it must be specified only for auxiliary buffer to receive auxiliary data and must be
    // similar to the RSDK_CSI2_VC_BUF_5TH_CH_... mask specified for outputDataMode
    RSDK_CSI2_DATA_TYPE_AUX_0_NO_DROP           = 0x40u,  /**< auxiliary, mode 0, no drop, mask*/
    RSDK_CSI2_DATA_TYPE_R12_A0_NO_DROP          = 0x6Cu,  /**< RAW12 + auxiliary, mode 0, no drop, <b>type to use</b>*/
    RSDK_CSI2_DATA_TYPE_AUX_0_DR_1OF2           = 0x80u,  /**< auxiliary, mode 0, drop 1 of 2, mask*/
    RSDK_CSI2_DATA_TYPE_R12_A0_DR_1OF2          = 0xACu,  /**< RAW12 + auxiliary, mode 0, drop 1 of 2, 
                                                                                                    <b>type to use</b>*/
    RSDK_CSI2_DATA_TYPE_AUX_0_DR_3OF4           = 0xc0u,  /**< auxiliary, mode 0, drop 3 of 4, mask*/
    RSDK_CSI2_DATA_TYPE_R12_A0_DR_3OF4          = 0xECu,  /**< RAW12 + auxiliary, mode 0, drop 3 of 4, 
                                                                                                    <b>type to use</b>*/
    RSDK_CSI2_DATA_TYPE_AUX_1_NO_DROP           = 0x100u, /**< auxiliary, mode 1, no drop, mask*/
    RSDK_CSI2_DATA_TYPE_R12_A1_NO_DROP          = 0x12Cu, /**< RAW12 + auxiliary, mode 1, no drop, <b>type to use</b>*/

    RSDK_CSI2_DATA_TYPE_MAX                     = 0x13fu, /**< stream type maximum, not used */

}rsdkCsi2DataStreamType_t;


/**
 * @brief       CSI2 units enumeration.
 * @details     Adjusted to the specific platform.
 *
 */
typedef enum
{
    RSDK_CSI2_UNIT_0 = 0,           /**< First unit (MIPICSI2_0)    */
    RSDK_CSI2_UNIT_1,               /**< Second unit (MIPICSI2_1)   */
    #if defined(S32R45)
        RSDK_CSI2_UNIT_2,           /**< Third unit (MIPICSI2_2)    */
        RSDK_CSI2_UNIT_3,           /**< Fourth unit (MIPICSI2_3)   */
    #endif
    RSDK_CSI2_MAX_UNITS             /**< The units limit, to not use in procedure call */
}rsdkCsi2UnitId_t;


/**
 * @brief       CSI2 Virtual Channel enumeration.
 * @details     Adjusted to the specific platform.
 *
 */
typedef enum
{
    RSDK_CSI2_VC_0 = 0,             /**< Virtual Channel 0                                  */
    RSDK_CSI2_VC_1,                 /**< Virtual Channel 1                                  */
    RSDK_CSI2_VC_2,                 /**< Virtual Channel 2                                  */
    RSDK_CSI2_VC_3,                 /**< Virtual Channel 3                                  */
    RSDK_CSI2_MAX_VC                /**< Virtual channels number limit for Csi2 unit, to not use in procedure call */
}rsdkCsi2VirtChnlId_t;


/**
 * @brief       CSI2 lanes enumeration.
 * @details     Depending the usage, this can means the lane position or number of lines used
 *
 */
typedef enum
{
    RSDK_CSI2_LANE_0 = 0,           /**< first lane / one lane  */
    RSDK_CSI2_LANE_1,               /**< second lane / two lanes */
    RSDK_CSI2_LANE_2,               /**< third lane / three lanes */
    RSDK_CSI2_LANE_3,               /**< fourth lane / four lanes */
    RSDK_CSI2_MAX_LANE              /**< lanes (maximum) per CSI2 unit, to not use in procedure call  */
}rsdkCsi2LaneEnum_t;


/**
 * @brief       Enum for ADC channels.
 * @details     Channels E/F/G and H are available only for complex data input from the Front End (see
 *              \ref RSDK_CSI2_VC_BUF_REAL_DATA / \ref RSDK_CSI2_VC_BUF_COMPLEX_DATA).
 *              For real data only channels A/B/C and D are available, according to the settings.<br>
 *              Do not use \b RSDK_CSI2_MAX_CHANNEL in procedures calls.<br>
 *
 */
typedef enum {
    RSDK_CSI2_CHANNEL_A = 0,
    RSDK_CSI2_CHANNEL_B,
    RSDK_CSI2_CHANNEL_C,
    RSDK_CSI2_CHANNEL_D,
    RSDK_CSI2_CHANNEL_E,
    RSDK_CSI2_CHANNEL_F,
    RSDK_CSI2_CHANNEL_G,
    RSDK_CSI2_CHANNEL_H,
    RSDK_CSI2_MAX_CHANNEL,
}rsdkCsi2Channel_t;


/**
 * @brief       Auto DC offset calculation (statistics management) type enum.
 * @details     If auto computation for DC offset is required, only one of these value is accepted.
 *              If no auto computation for DC offset required, these values are ignored.
 *
 */
typedef enum {
    RSDK_CSI2_STAT_NO = 0u,             /**< Statistics not managed                                         */
    RSDK_CSI2_STAT_EVERY_LINE = 1u,     /**< Statistics management after every line received                */
    RSDK_CSI2_STAT_AT_FE,               /**< Statistics management at Frame End. \if (S32R45_DOCS || S32R294_DOCS)
                                         * All statistics in the receiving buffer (according to \ref bufNumLines for
                                         * the Virtual Channel) will be processed \endif                    */
    RSDK_CSI2_STAT_LAST_LINE,           /**< Statistics management only for the last line received          */
    RSDK_CSI2_STAT_MAX                  /**< Statistics max limit, to not be used in application            */
}rsdkCsi2AutoDCComputeTime_t;


/**
 * @brief       IRQ definitions enumeration.
 * @details     According to the platform. The mnemonics can differ somehow from the names in RM.
 *
 */
typedef enum {
    RSDK_CSI2_RX_ERR_IRQ_ID = 0,    /**< IRQ ID for Receive errors (D-PHY related)                      */
    RSDK_CSI2_PATH_ERR_IRQ_ID,      /**< IRQ ID for Receive Data Path and Protocol errors               */
    RSDK_CSI2_EVENTS_IRQ_ID,        /**< IRQ ID for Events and line errors interrupt                    */
    RSDK_CSI2_TX_ERR_IRQ_ID,        /**< IRQ ID for Tournaround and Transmit errors                     */
    RSDK_CSI2_MAX_IRQ_ID            /**< Maximum IRQ ID for platform, to not use in procedure call      */
}rsdkCsi2IrqId_t;


/*==================================================================================================
 *                                STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
/**
 * @brief       Structure for short packet received.
 * @details     Usually this data is used in an error callback, related to \ref RSDK_CSI2_PATH_ERR_IRQ_ID interrupt,
 *              in rsdkCsi2Report_t::shortPackets.
 *
 */
typedef struct {
    uint8_t     dataID;             /**< Identifier (ID) of the packet   */
    uint16_t    dataVal;            /**< Data value (16 bits)       */
} rsdkCsi2ShortPacket_t;


/**
 * @brief       Structure for line length error or number of lines received error in the received frame.
 * @details     Usually this data is used in an error callback, related to \ref RSDK_CSI2_EVENTS_IRQ_ID interrupt,
 *              in rsdkCsi2Report_t::lineLengthErr.
 *
 */
typedef struct {
    uint16_t    linePoz;            /**< The first line with length problem (if line/chirp length error) or the total
                                     * number of lines received (if number of lines error)  */
    uint16_t    lineLength;         /**< The received length (different from the expected one), valid only if line
                                     * length error is signaled */
} rsdkCsi2LineLenErr_t;


/**
 * @brief       Structure for VirtualChannel configuration.
 * @details     The structure hold all necessary parameters to initialize the reception of the data for the
 *              Virtual Channel (VC).
 *
 * @note        Assuming any VC is receiving only one data_type, so using only one buffer.
 *
 */
typedef struct {
    rsdkCsi2DataStreamType_t  streamDataType;   /**< Data type to be received                                        */
    uint8_t     channelsNum;                    /**< Channels number for this VC; equal to antennas number for radar */
    uint8_t     vcEventsReq;                    /**< Mask for IRQ requested events for VC (i.e. \ref
                                                 * RSDK_CSI2_EVT_FRAME_END or similar)                               */
    uint16_t    expectedNumSamples;             /**< Expected line length for receiving in terms of samples/pixels/etc.
                                                 * per channel (antenna for radar)                                   */
    uint16_t    expectedNumLines;               /**< Expected number of lines/chirps per frame                       */
    uint16_t    bufNumLines;                    /**< Available number of complete length lines/chirps (including chirp
                                                 * statistics to be received in buffer, at least 1<br>
                                                 * \if (S32R45_DOCS || S32R294_DOCS) @note For internal software
                                                 * reasons, (\ref expectedNumLines % \ref bufNumLines ) must not be
                                                 * \b 1 \endif                                                       */
    uint16_t    bufLineLen;                     /**< The available line length in buffer, per line (16 bytes aligned)
                                                 * must include 80 supplementary bytes for line/chirp statistics     */
    uint16_t    outputDataMode;                 /**< The mode data is output in the buffer and other information
                                                 * about input data, see above : RSDK_CSI2_VC_BUF_... family definitions
                                                 * i.e. \ref RSDK_CSI2_VC_BUF_COMPLEX_DATA.
                                                 * This parameter must be identical for data buffer and for
                                                 * correspondent auxiliary buffer.               */
    void        *pBufData;                      /**< Pointer to data buffer (16 bytes aligned), physical address     */
    int16_t     offsetCompReal[RSDK_CSI2_MAX_CHANNEL];
                                                /**< Channel offset compensation for real part,<br>
                                                 * \ref RSDK_CSI2_OFFSET_AUTOCOMPUTE => auto config_DC,<br>
                                                 * any other = compensation (0 => no compensation).<br>
                                                 * The compensation must be specified at AD input level (12 bits).   */
    int16_t     offsetCompImg[RSDK_CSI2_MAX_CHANNEL];
                                                /**< Channel offset compensation for imaginary part, see
                                                 * \ref offsetCompReal  */
    uint8_t     gpio1Mask, gpio2Mask;           /**< masks for GPIO trigger handling (see RSDK_CSI2_REQ_ETRG_EVT_...
                                                 * family, i.e. \ref RSDK_CSI2_REQ_ETRG_EVT_FRAME_START); to be
                                                 * functional, the triggers must be enabled (see \ref  gpio1EnaMask and
                                                 * \ref gpio2EnaMask                                                 */
    uint8_t     gpio1EnaMask, gpio2EnaMask;     /**< Mask for GPIO triggers enablement (see RSDK_CSI2_REQ_ETRG_ENA_...
                                                 * family, i.e. \ref RSDK_CSI2_REQ_ETRG_ENA_ON_ERROR); must be set
                                                 * correctly to have external trigger of the related events.         */
    uint8_t     sdma1Mask, sdma2Mask;           /**< masks for SDMA trigger handling (see RSDK_CSI2_REQ_ETRG_EVT_...
                                                 * family, i.e. \ref RSDK_CSI2_REQ_ETRG_EVT_FRAME_START); to be
                                                 * functional, the triggers must be enabled (see \ref  sdma1EnaMask and
                                                 * \ref sdma2EnaMask)                                                */
    uint8_t     sdma1EnaMask, sdma2EnaMask;     /**< masks for SDMA trigger handling (see RSDK_CSI2_REQ_ETRG_EVT_...
                                                 * family, i.e. \ref RSDK_CSI2_REQ_ETRG_EVT_FRAME_START); to be
                                                 * functional, the triggers must be enabled (see \ref  sdma1EnaMask and
                                                 * \ref sdma2EnaMask)                                               */
    uint8_t     bufNumLinesTrigger;             /**< Number of lines received in buffer to generate a callback
                                                 * to application, if the \ref RSDK_CSI2_EVT_LINE_END is required in
                                                 * vcEventsReq. It correspond to RM - Table 81-7 -
                                                 * LINEDONE field and cap. 81.8.41                                  */
}rsdkCsi2VCParams_t;


/**
 * @brief       Structure which describe the errors/events signaled by an interrupt request.
 * @details     The cumulated errors/events are put in this structure.
 *              Normally only the configured Virtual Channels will be reported.
 *
 */
typedef struct {
    rsdkCsi2UnitId_t    unitId;                     /**< Unit number reporting the error,
                                                     * similar to \ref rsdkCsi2UnitId_t                             */
    uint32_t    errMaskU;                           /**< The cumulated masks for signaled errors at unit level;<br>
                                                     * \if (S32R45_DOCS || S32R294_DOCS) the possible masks are for:
                                                     * - PHY level errors, i.e. \ref RSDK_CSI2_ERR_PHY_SYNC
                                                     * - packet errors, i.e. \ref RSDK_CSI2_ERR_PACK_ECC1
                                                     * \endif                                                       */
    uint32_t    errMaskVC[RSDK_CSI2_MAX_VC];        /**< The cumulated masks for signaled errors at VC level;<br>
                                                     * \if (S32R45_DOCS || S32R294_DOCS)  the possible masks are for:
                                                     * - line errors definitions, i.e. \ref RSDK_CSI2_ERR_LINE_LEN
                                                     * - all other miscelaneous
                                                     * \endif                                                       */
    uint8_t     evtMaskVC[RSDK_CSI2_MAX_VC];        /**< Events mask for VCs, according to the requested events as
                                                     * required at initialization - rsdkCsi2VCParams_t->vcEventsReq<br>
                                                     * the possible masks are defined by:
                                                     * - events masks :
                                                     *  \ref RSDK_CSI2_EVT_FRAME_START \ref RSDK_CSI2_EVT_FRAME_END
                                                     *  \if (S32R45_DOCS || S32R294_DOCS)
                                                     *  \ref RSDK_CSI2_EVT_LINE_END
                                                     *  \ref RSDK_CSI2_EVT_SKEW_CALIB
                                                     *  \ref RSDK_CSI2_EVT_SHORT_PACKET
                                                     *  \ref RSDK_CSI2_EVT_BIT_NOT_TOGGLE
                                                     *  \ref RSDK_CSI2_EVT_NEXT_START_NOT_0
                                                     *  \endif                                                      */
    uint8_t     invalidPacketID[RSDK_CSI2_MAX_VC];  /**< The wrong packet ID received, reported if \ref
                                                     * RSDK_CSI2_ reported                               */
    uint8_t     eccOneBitErrPos[RSDK_CSI2_MAX_VC];  /**< The recovered position for ECC one bit error, if \ref
                                                     * RSDK_CSI2_ERR_PACK_ECC1 reported                             */
    uint16_t    expectedCRC[RSDK_CSI2_MAX_VC];      /**< Computed CRC for received data, if \ref RSDK_CSI2_ERR_PACK_CRC
                                                     * reported                                                     */
    uint16_t    receivedCRC[RSDK_CSI2_MAX_VC];      /**< Real, received CRC if \ref RSDK_CSI2_ERR_PACK_CRC reported */
    uint16_t    notToggledBits[RSDK_CSI2_MAX_CHANNEL];  /**< Bits mask for not toggled bits, for the channel, if \ref
                                                     * RSDK_CSI2_EVT_BIT_NOT_TOGGLE reported; the bit mask set to 1
                                                     * means bit not toggled                                        */
    uint8_t     lastDroppedData[RSDK_CSI2_MAX_VC];  /**< The last data type dropped on each VC                      */
    rsdkCsi2LineLenErr_t  lineLengthErr[RSDK_CSI2_MAX_VC];  /**< Specific data for line length error, if \ref
                                                     * RSDK_CSI2_ERR_LINE_LEN or \ref RSDK_CSI2_ERR_LINE_CNT bits
                                                     * are set                                                      */
    rsdkCsi2ShortPacket_t shortPackets[RSDK_CSI2_MAX_VC];   /**< Specific data for short packet received
                                                     * \if (S32R45_DOCS || S32R294_DOCS)
                                                     * , if \ref RSDK_CSI2_EVT_SHORT_PACKET bit is set.  \endif     */
} rsdkCsi2Report_t;


/**
 * @brief   Definition of callback function type to be called by the CSI2 interrupt handler.
 * @details This callback must be used for processing a CSI2 error/event interrupt.<br>
 *          Simple callback function definition example : void CallbackCsi2(rsdkCsi2Report_t *pStruct) { ... }
 *
 * @param[in]    rsdkCsi2Report_t* - the callback receives a pointer to a complete description of the error/event
 * @return       nothing
 *
 *
 * */
typedef void (*rsdkCsi2IsrCb_t)(rsdkCsi2Report_t *pReportingStruct);


/**
 * @brief       Structure for unit configuration.
 * @details     The structure accepts initialization of all available Virtual Channels (VC), but using only one data
 *              type for each VC.
 *
 */
typedef struct {
    uint8_t     numLanesRx;                     /**< Number of lanes used to receive data; use \ref rsdkCsi2LaneEnum_t
                                                 *  to configure this */
    rsdkCsi2LaneEnum_t     lanesMapRx[RSDK_CSI2_MAX_LANE]; /**< Lanes mapping :
                                                 *  - first byte - the physical lane to be used as lane 1,
                                                 *  - second byte - the physical lane to be used as lane 2,
                                                 *  - etc.<br>
                                                 *  use \ref rsdkCsi2LaneEnum_t to set these fields                 */
    uint8_t     numLanesTx;                     /**< Number of lanes used to transmit data, 1...4 (TBD)             */
    rsdkCsi2AutoDCComputeTime_t statManagement; /**< How the channel statistics are managed. Only channels having
                                                 * \ref RSDK_CSI2_OFFSET_AUTOCOMPUTE DC offset specified will be
                                                 * managed. Use one of RSDK_CSI2_STAT_... definitions. The value is
                                                 * used only at least one of the VC channels has the DC offset
                                                 * specified as \ref RSDK_CSI2_OFFSET_AUTOCOMPUTE .                 */
    uint32_t    txClkFreq;                      /**< Transmit (Tx) clock frequency for CSI2 (17...80 Mbps), in Mbps.*/
    uint32_t    rxClkFreq;                      /**< Receiving (Rx) frequency (between \ref RSDK_CSI2_MIN_RX_FREQ
                                                 * and \ref RSDK_CSI2_MAX_RX_FREQ), in Mbps.                        */
    rsdkCsi2VCParams_t      *pVCconfig[RSDK_CSI2_MAX_VC];   /**< pointers to VC configurations; ignored if NULL.
                                                            * At least VC 0 must be configured. */
    rsdkCsi2VCParams_t      *pAuxConfig[RSDK_CSI2_MAX_VC];  /**< pointers to VC configurations, for auxiliary data only;
                                                            * The corresponding VC must have set 
                                                            * RSDK_CSI2_VC_BUF_FIFTH_CH_ON; if this pointer is NULL,
                                                            * the the auxiliary data will be dropped. If this pointer
                                                            * is not NULL, and the corresponding VC has
                                                            * RSDK_CSI2_VC_BUF_FIFTH_CH_ON, it is assumed auxiliary data 
                                                            * type 0; so, for auxiliary data type 1 the buffer must be 
                                                            * configured, even the data is not used. */
    rsdkCsi2IsrCb_t         pCallback[RSDK_CSI2_MAX_IRQ_ID];    /**< the necessary callbacks :
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
}rsdkCsi2InitParams_t;


/** @}*/


/*==================================================================================================
 *                                GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/

/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
 ==================================================================================================*/
/** @addtogroup csi2_driver_api_func
* @{
*/

/**
 * @brief          This function initializes the CSI2 interface.
 * @details        The detailed actions done by this function :
 *                   - verify the input parameters; if any wrong parameter, an error value is returned
 *                   - reset the interface
 *                   - initialize the CSI2 interface
 *                   - initialize the interrupt vectors
 *                   - start the interface
 *                   - reset the frame counters <br>
 *                  The function can be called at any time, if necessary.<br>
 *                  If the result is not \ref RSDK_SUCCESS, the interface status is \b NOT_INITIALIZED,
 *                  no matter the status at call time.
 *
 * @param[in] unitId            - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) 
                                                            \ref RSDK_CSI2_UNIT_1 , \endif \ref RSDK_CSI2_MAX_UNITS )
 * @param[in] pCsi2InitParam    - pointer to the CSI2 Driver initialization structure
 *
 * @return \ref rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 * @retval RSDK_CSI2_DRV_INVALID_ANTENNA_NR - Wrong antenna number
 * @retval RSDK_CSI2_DRV_INVALID_CALIB_MODE - Wrong calibration ID
 * @retval RSDK_CSI2_DRV_INVALID_LANES_NR - Wrong lanes number
 * @retval RSDK_CSI2_DRV_INVALID_CLOCK_FREQ - Clock frequency incorrect (null)
 * @retval RSDK_CSI2_DRV_NO_SAMPLE_PER_CHIRP - Samples per chirp incorrect (null)
 * @retval RSDK_CSI2_DRV_NO_CHIRPS_PER_FRAME - Chirps per frame incorrect (null)
 * @retval RSDK_RFE_NULL_IRQ_REGISTER_CB - Incorrect parameters to record CSI2 - IRQ, isrRegisterCb is NULL
 * @retval RSDK_CSI2_DRV_NULL_ERR_CB_PTR - Incorrect parameter - error callback is NULL
 * @retval RSDK_CSI2_DRV_ERR_INVALID_INT_NR - Incorrect IRQ nr used
 * @retval RSDK_CSI2_DRV_ERR_INVALID_CORE_NR - Incorrect core nr used
 * @retval RSDK_CSI2_DRV_CALIBRATION_TIMEOUT - time-out on DPHY calibration
 * @retval RSDK_CSI2_DRV_SW_RESET_ERROR - error on resetting the interface
 *
 * @pre     It must be called in the following situations:
 *           - at application start (e.g. after boot)
 *           - whenever the system parameters described in \ref rsdkCsi2InitParams_t need to be changed
 *           - every time the \ref RsdkCsi2GetInterfaceStatus returns an error
 *
 */
rsdkStatus_t RsdkCsi2Init(const rsdkCsi2UnitId_t unitId, const rsdkCsi2InitParams_t *pCsi2InitParam);


/**
 * @brief          The function stop the CSI2 receive module.
 * @details        The receive module of the CSI2 interface is stopped.
 *                 The receive will stop only after the end of the currently received packet.
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) 
                                                            \ref RSDK_CSI2_UNIT_1 , \endif \ref RSDK_CSI2_MAX_UNITS )
 *
 * @return \ref rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 * @retval RSDK_CSI2_DRV_NOT_INIT          - the unit was not initialized before
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the stopped status
 * @retval RSDK_CSI2_DRV_WRG_UNIT_ID       - if a wrong unit was specified
 *
 * @pre    It can be called when the reception need to be stopped. A \ref RsdkCsi2Init or \ref RsdkCsi2PowerOn call must
 *          be done previously.
 *
 */
rsdkStatus_t RsdkCsi2RxStop(const rsdkCsi2UnitId_t unitId);


/**
 * @brief          The function start the CSI2 receive module.
 * @details        The receive module of the CSI2 interface is started.
 *                 This action must be done if a CSI2_RxStop was done before,
 *                 and the reception must be done at the same parameters.
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) 
                                                            \ref RSDK_CSI2_UNIT_1 , \endif \ref RSDK_CSI2_MAX_UNITS )
 *
 * @return \ref rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful
 * @retval RSDK_CSI2_DRV_NOT_INIT          - the unit was not initialized before
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the running status
 * @retval RSDK_CSI2_DRV_WRG_UNIT_ID       - if a wrong unit was specified
 *
 * @pre   It can be called when the reception is stopped, after an previous \ref RsdkCsi2RxStop.
 *
 */
rsdkStatus_t RsdkCsi2RxStart(const rsdkCsi2UnitId_t unitId);


/**
 * @brief          The function power off the CSI2 module.
 * @details        The interface is powered off.
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) \ref RSDK_CSI2_UNIT_1 , 
                                                                    \endif \ref RSDK_CSI2_MAX_UNITS )
 *
 * @return \ref rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful.
 * @retval RSDK_CSI2_DRV_NOT_INIT          - the unit was not initialized before
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the running status.
 * @retval RSDK_CSI2_DRV_WRG_UNIT_ID       - if a wrong unit was specified
 *
 * @pre   It can be called when the reception need to be stopped and/or the interface to be powered off.
 *          A \ref RsdkCsi2Init must be done previously.
 *
 */
rsdkStatus_t RsdkCsi2PowerOff(const rsdkCsi2UnitId_t unitId);


/**
 * @brief          The function power on the CSI2 module.
 * @details        The interface is powered on.
 *                 This action must be done after a Csi2PowerOff was done before, or in place of a \ref RsdkCsi2Init,
 *                 if a successful initialization was done before.
 *                 After this call, the last initialization parameters will be used to reinitialize the interface.
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) \ref RSDK_CSI2_UNIT_1 , 
                                                                    \endif \ref RSDK_CSI2_MAX_UNITS )
 *
 * @return \ref rsdkStatus_t - success or error status information.
 * @retval RSDK_SUCCESS - execution was successful.
 * @retval RSDK_CSI2_DRV_NOT_INIT          - the unit was not initialized before
 * @retval RSDK_CSI2_DRV_HW_RESPONSE_ERROR - if the module is not reporting the running status.
 *
 * @pre   It can be called when the reception is stopped, or a new RsdkCsi2Init is intended.
 *        If the \ref RsdkCsi2Init call was not done before, an error will be reported, else
 *        the unit will be reinitialized using the previous initialization parameters.
 */
rsdkStatus_t RsdkCsi2PowerOn(const rsdkCsi2UnitId_t unitId);


/**
 * @brief          The function ask for the current status of the interface.
 * @details        The function return the status of the interface at the moment of the call.
 *
 * @return \ref rsdkStatus_t - the current status of the interface.
 * @retval   RSDK_CSI2_DRV_POWERED_OFF     - The PHY interface if powered off.
 * @retval   RSDK_CSI2_DRV_RX_STOPPED      - The Rx interface if stopped.
 * @retval   RSDK_CSI2_DRV_LINE_ON         - The Rx interface if on, out of reception.
 * @retval   RSDK_CSI2_DRV_LINE_RECEIVING  - The Rx interface is receiving data.
 * @retval   RSDK_CSI2_DRV_STATE_LINE_MARK - The Rx lane is in MARK state
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) \ref RSDK_CSI2_UNIT_1 , 
                                                                    \endif \ref RSDK_CSI2_MAX_UNITS )
 *
 * @pre   It can be called at any time, after a successful initialization, to check the current status of the interface.
 *        Depending of the returned status, some times is necessary to loop on this call to really
 *        understand how interface is working.
 *
 */
rsdkStatus_t RsdkCsi2GetInterfaceStatus(const rsdkCsi2UnitId_t unitId);


/**
 * @brief          The function ask for the current status of the specific lane.
 * @details        The function return the status of the interface or specific lane at the moment of the call.
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) \ref RSDK_CSI2_UNIT_1 , 
                                                                    \endif \ref RSDK_CSI2_MAX_UNITS )
 * @param[in] laneNum   - lane number, \ref rsdkCsi2LaneEnum_t
 *
 * @return \ref rsdkStatus_t - the current status of the interface.
 * @retval     RSDK_CSI2_DRV_INVALID_LANES_NR  - incorrect lane number
 * @retval     RSDK_CSI2_DRV_STATE_LINE_MARK   - The Rx lane in Mark-1 state.
 * @retval     RSDK_CSI2_DRV_LINE_ON           - The Rx lane active, but not currently receiving data.
 * @retval     RSDK_CSI2_DRV_LINE_RECEIVING    - The Rx lane is receiving data.
 * @retval     RSDK_CSI2_DRV_LINE_OFF          - The lane is not used.
 *
 * @pre   It can be called at any time, to check the current status of the interface/lane.
 *        Depending of the returned status, some times is necessary to loop on this call to
 *        really understand how lane is working.
 */
rsdkStatus_t RsdkCsi2GetLaneStatus(const rsdkCsi2UnitId_t unitId, const uint32_t laneNum);


/**
 * @brief       The function return the current frames counter
 * @details     The application can read the counter of frames, to decide how to manage the data. The counter is
 *              increased after handling the Frame End interrupt request. <br>
 *              A correct \b unitId and \b vcId must be provided.
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) \ref RSDK_CSI2_UNIT_1 , 
                                                                    \endif \ref RSDK_CSI2_MAX_UNITS )
 * @param[in] vcId      - VC ID &isin; [ \ref RSDK_CSI2_VC_0 , \ref RSDK_CSI2_MAX_VC )
 *
 * @return uint32_t - the current frames counter
 *
 */
uint32_t RsdkCsi2GetFramesCounter(const rsdkCsi2UnitId_t unitId, const rsdkCsi2VirtChnlId_t vcId);


/**
 * @brief       Procedure to get the real buffer length for a line/chirp.
 * @details     The procedure compute the necessary length according to the input parameters.
 * \if (S32R45_DOCS || S32R294_DOCS)
 * @note        Because the length include the line/chirp statistics, autoStatistics must be 1 even only one channel
 *              in the entire Virtual Channel need the autostatistic computation.
 * \endif
 *
 * @param[in]   dataType        Type of data to receive, normally \ref RSDK_CSI2_DATA_TYPE_RAW12 for radar
 * @param[in]   nrChannels      Number of channels, 1 ... 8; usually this number is
 *                              the number of active antennas \if (S32R45_DOCS || S32R294_DOCS) ,
 *                              multiply by 2 if complex (Re & Im) acquisition \endif
 * @param[in]   samplesPerChirp Samples per chirp and channel
 * @param[in]   autoStatistics  \if (S32R45_DOCS || S32R294_DOCS) The driver must calculate the statistics per chirp
 *                              (1) or not (0) \else Must be 0 for \b S32R294 / \b S32R372. \endif
 *
 * @return      The necessary memory amount for one line (chirp) if not 0. <br>
 *              Wrong input parameter if 0.
 *
 */
uint32_t    RsdkCsi2GetBufferRealLineLen(const rsdkCsi2DataStreamType_t dataType, const uint32_t nrChannels,
        const uint32_t samplesPerChirp, const uint8_t autoStatistics);


#if (defined S32R294)
/**
 * @brief       Procedure to get the buffer start for the next frame.
 * @details     The procedure returns the offset from the buffer start
 *                  where the first byte of the frame will be written.
 *              The procedure must be called after the previous frame was received,
 *                  but before the start of the expected frame.
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) \ref RSDK_CSI2_UNIT_1 , 
                                                                    \endif \ref RSDK_CSI2_MAX_UNITS )
 * @param[in] vcId      - VC ID &isin; [ \ref RSDK_CSI2_VC_0 , \ref RSDK_CSI2_MAX_VC )
 * @param[in] pOffset   - pointer to a uint32_t which will receive the real offset
 *
 * @return      RSDK_SUCCESS - if driver status is correct; the buffer offset of the first byte is passed to pOffset
 *              error if the driver is in an inappropriate state; pOffset is not updated
 *
 */
rsdkStatus_t    RsdkCsi2GetFirstByteOffset(const rsdkCsi2UnitId_t unitId, const rsdkCsi2VirtChnlId_t vcId,
        uint32_t *pOffset);

/**
 * @brief       Procedure to get the buffer start for the next frame.
 * @details     The procedure returns the buffer line
 *                  where the first line of the frame will be written.
 *              The procedure must be called after the previous frame was received,
 *                  but before the start of the expected frame.
 * @note        To get the exact address, must be used the buffer line length
 *                  declared in the unit initialization parameters (rsdkCsi2VCParams_t::bufLineLen).
 *
 * @param[in] unitId    - unit : rsdkCsi2UnitID_t &isin; [ \if (S32R45_DOCS || S32R294_DOCS) \ref RSDK_CSI2_UNIT_1 , 
                                                                    \endif \ref RSDK_CSI2_MAX_UNITS )
 * @param[in] vcId      - VC ID &isin; [ \ref RSDK_CSI2_VC_0 , \ref RSDK_CSI2_MAX_VC )
 * @param[in] pFirstLine - pointer to a uint32_t which will receive the real first line position
 *
 * @return      RSDK_SUCCESS - if driver status is correct; the frame first line index is passed to pFirstLine
 *              error if the driver is in an inappropriate state; pOffset is not updated
 *
 */
rsdkStatus_t    RsdkCsi2GetFirstLinePos(const rsdkCsi2UnitId_t unitId, const rsdkCsi2VirtChnlId_t vcId,
        uint32_t *pFirstLine);

#endif


#ifdef __cplusplus
}
#endif

#endif /*RSDK_CSI2_DRIVER_API_H*/

/** @} */
