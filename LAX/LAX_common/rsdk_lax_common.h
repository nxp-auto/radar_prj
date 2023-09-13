/*
 * Copyright 2017-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RSDK_LAX_COMMON_H
#define RSDK_LAX_COMMON_H

/**
* @file           rsdk_lax_common.h
* @brief          include file common to both Host and LAX projects;
*                 the parameters in this file are NOT user-configurable
*                 the user-configurable parameters ar in lax_config.h
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#if defined(LAX_OS_linux) || defined(_BSD_SOURCE)
    #include <oal_utils.h>
    #include <oal_log.h>
#endif
#if defined(LAX_OS_sa)
#include <stdint.h>
#endif

//include only for LAX sources
#if !defined(LAX_OS_sa) && !defined(LAX_OS_linux) && !defined(_BSD_SOURCE)
#include <inttypes.h>
#endif

#include <stdbool.h>
#include "lax_platform.h"
#include "rsdk_lax_config.h"

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/**
* @brief          TRUE and FALSE
*
*/
#ifndef TRUE
  #define TRUE 1U
#endif
#ifndef FALSE
  #define FALSE 0U
#endif

/**
* @brief    Macros for registry access.
* @details  The current version of S32R45.h file defines many structures for the same area.
*           This is perhaps as a VSPA inheritance.
*
*/
#define LAX_DMA_CTRL_REG_PTR            ((volatile struct VSPA_DMA_control_and_status_tag *)(pLaxCtrl->pRegs))
#define LAX_MSG_PROF_REG_PTR            ((volatile struct VSPA_Debug_messaging_and_profiling_tag *)(pLaxCtrl->pRegs))
#define LAX_VCPU_REG_PTR                ((volatile struct VSPA_General_VCPU_control_status_tag *)(pLaxCtrl->pRegs))
#define LAX_IPPU_REG_PTR                ((volatile struct VSPA_IPPU_control_and_status_tag *)(pLaxCtrl->pRegs))
#define LAX_INPUT_REG_PTR               ((volatile struct VSPA_Input_output_tag *)(pLaxCtrl->pRegs))
#define LAX_VCPU_GO_REG_PTR             ((volatile struct VSPA_VCPU_Go_ctrl_and_stat_tag *)(pLaxCtrl->pRegs))
#define LAX_VCPU_HOST_REG_PTR           ((volatile struct VSPA_VCPU_Host_Messaging_tag *)(pLaxCtrl->pRegs))
#define LAX_VERS_CFG_REG_PTR            ((volatile struct VSPA_Version_and_configuration_tag *)(pLaxCtrl->pRegs))

/**
* @brief          Specific definitions for LAX registry usage
*/
#define LAX_INPUT_PARITY_FIRST_REG  1u
#define LAX_INPUT_PARITY_LAST_REG   5u


/**
* @brief          size convertors
*
*/
#define BYTES_TO_WORDS16B(x)   ((x) >> 1U)
#define BYTES_TO_WORDS32B(x)   ((x) >> 2U)
#define WORDS16B_TO_BYTES(x)   ((x) << 1U)
#define WORDS32B_TO_BYTES(x)   ((x) << 2U)

/**
* @brief          The maximum number of LAX commands that may be launched simultaneously on each LAX core.
* @details        The maximum number of commands launched from host and which are either queued or 
*                 in execution on a LAX core at any moment. If a new command is received when 
*                 LAX_MAX_CMDS_NUM are already in execution or queued for execution, the new 
*                 command will not be enqueued.
*
*/
#define RSDK_LAX_MAX_CMDS_NUM 16U

/**
* @brief          Maximum number of entries in table of LAX graphs invoked by the SPT
*/
#define RSDK_LAX_SPT_CMD_MAX_NUM    SPT_LAX_EXT_GO_NUM

/**
* @brief          Maximum number of entries in table of LAX graphs invoked by the CTE
*/
#define RSDK_LAX_CTE_CMD_MAX_NUM    CTE_LAX_EXT_GO_NUM

/**
* @brief          Maximum number of entries in table of LAX graphs invoked by the other LAX core
*/
#define RSDK_LAX_LAX_CMD_MAX_NUM    LAX_LAX_EXT_GO_NUM

/**
* @brief        The maximum number of LAX graphs (at LAX sub-system level).
* @details      This is the maximum number of LAX graphs that can be registered on all LAX cores.
*/
#define RSDK_LAX_MAX_GRAPHS   (RSDK_LAX_CORES_NUM * (RSDK_LAX_CORE_MAX_NUM_HOST_FUNC_GRAPHS + \
                               RSDK_LAX_CTE_CMD_MAX_NUM + RSDK_LAX_SPT_CMD_MAX_NUM + RSDK_LAX_LAX_CMD_MAX_NUM))

/**
* @brief          CTE interrupt flag offset for command completion
*                 See also RSDK_LAX_HOST_FLAGS1_CTE_CMD_MASK.
*/
#define RSDK_LAX_CTE_INT_OFFSET    4U

/**
* @brief          Other LAX core interrupt flag offset for command completion 
*                 See also RSDK_LAX_HOST_FLAGS1_LAX_CMD_MASK.
*/
#define RSDK_LAX_LAX_INT_OFFSET    12U


/**
* @brief          Mask for command completion notification double interrupt bits.
*                 All command completion notifications use two interrupt bits.  
*/
#define RSDK_LAX_CMD_DOUBLE_IDX_BITS                    0x00010001U

/**
* @brief          The host-flags1 mask for SPT-triggered commands
*/
#define RSDK_LAX_HOST_FLAGS1_SPT_CMD_MASK (0x000F000FU)

/**
* @brief          The host-flags1 mask for CTE-triggered commands
*/
#define RSDK_LAX_HOST_FLAGS1_CTE_CMD_MASK (0x00F000F0U)

/**
* @brief          The host-flags1 mask for commands triggered by the other LAX core
*/
#define RSDK_LAX_HOST_FLAGS1_LAX_CMD_MASK (0x10001000U)

/**
* @brief          The host-flags1 mask for LAX command content error
*/
#define RSDK_LAX_HOST_FLAGS1_CMD_ERR_MASK (0x00000100U)

/**
* @brief          The host-flags1 mask for LAX graph table error
*/
#define RSDK_LAX_HOST_FLAGS1_TABLE_ERR_MASK (0x00000200U)

/**
* @brief          The host-flags1 mask for LAX graph table error
*/
#define RSDK_LAX_HOST_FLAGS1_PARITY_ERR_MASK (0x00000400U)

/**
* @brief          The DMA channel for ELD download.
* @details        The definition is used on host
*
*/
#define RSDK_LAX_DMA_ELD_CHANNEL 0U

/**
* @brief          The DMA channel for Relaxed mode, data transfers SRAM to LAX DMEM
* @details        The definition is used on LAX
*
*/
#define RSDK_LAX_DMA_TOLAX_CHANNEL 1U

/**
* @brief          The DMA channel for Relaxed mode, data transfers LAX DMEM to SRAM
* @details        The definition is used on LAX
*
*/
#define RSDK_LAX_DMA_FROMLAX_CHANNEL 2U

/**
* @brief          The DMA channel for LAX commands.
* @details        The definition is used on host and on lax
*
*/
#define RSDK_LAX_DMA_CMD_CHANNEL 3U

/**
* @brief          The DMA channel for CRC computation and CRC transfer to host.
* @details        The definition is used on host and on LAX
*
*/
#define RSDK_LAX_DMA_CRC_CHANNEL 4U

/**
* @brief          The maximum size of a DMA transfer.
*
*/
#define RSDK_LAX_MAX_DMA_TRANSFER  65536U

/**
* @brief          The redundancy mask for host commands (used for payload[0])
* @details        (~LAX_CMD_REDUNDANCY_MASK) is the function table index.
*                 This definition is used on host and on LAX.
*/
#define RSDK_LAX_CMD_REDUNDANCY_MASK (0x80000000U)

/**
* @brief          LAX CRC size expressed in bits
* @details        This definition is used on host, in LAX driver and LAX abstraction
*/
#define RSDK_LAX_CRC_BIT_LEN  128U

/**
* @brief          LAX0 core ID
*/
#define RSDK_LAX_CORE_0_ID  0U

/**
* @brief          LAX1 core ID
*/
#define RSDK_LAX_CORE_1_ID  1U

/**
* @brief          SW version.
*/
#define RSDK_LAX_SW_VER                     0xC0DE0000U

/**
* @brief          LAX core id mask in firmware sw version
*/
#define RSDK_LAX_SWVERSION_LAXID_MASK  (0x0000000FU)


/**
* @brief         Command for LAX graph execution
* @see           rsdkLaxCmdType_t
*/
#define RSDK_LAX_CMD_EXEC  (0U)
/**
* @brief         Command for reply buffers configuration (for CRC)
* @see           rsdkLaxCmdType_t
*/
#define RSDK_LAX_CMD_CFG_REPLY_BUF  (1U)
/**
* @brief         Command for CTE table entry configuration
* @see           rsdkLaxCmdType_t
*/
#define RSDK_LAX_CMD_CFG_CTE_GRAPH (2U)
/**
* @brief         Command for SPT table entry configuration
* @see           rsdkLaxCmdType_t
*/
#define RSDK_LAX_CMD_CFG_SPT_GRAPH (3U)
/**
* @brief         Command for LAX table entry configuration
* @see           rsdkLaxCmdType_t
*/
#define RSDK_LAX_CMD_CFG_LAX_GRAPH (4U)

/**
* @brief          Most significant bytes of Lax boot acknowledge message
*/
#define RSDK_LAX_BOOT_MSG_ACK_MB64_MSB                  0xF0700000U

/**
* @brief          Least significant bytes of Lax boot acknowledge message
*/
#define RSDK_LAX_BOOT_MSG_ACK_MB64_LSB                  0x00000000U

/**
* @brief          Error value sent back to host if LAX boot handshake failed
*/
#define RSDK_LAX_BOOT_HANDSHAKE_ERR                     0xF0708100U

/**
* @brief          Most significant bytes of Lax boot complete message
*/
#define RSDK_LAX_BOOT_COMPLETE_MB64_MSB                 0xF1000000U

/**
* @brief          Least significant bytes of Lax boot complete message
*/
#define RSDK_LAX_BOOT_COMPLETE_MB64_LSB                 0x00000000U

/**
* @brief         Boot-time command id for the "check command buffer size" command
*/
#define RSDK_LAX_CMD_BUF_SZ_CHECK_CMD_ID                0x70000000U

/**
* @brief         Circular command buffer address (in LAX DMEM, __cmdbuf_start in linker control file )
*/
#define RSDK_LAX_CMD_BUF_ADDR                0x00000000U


/**
* @brief          Mask used to extract the Command id bits for "check command buffer size" command
*/
#define RSDK_LAX_CMD_ID_MASK                            0xFF000000U



/*==================================================================================================
*                                             ENUMS
==================================================================================================*/
/** Log message severity level.
 *
 * The laxlib driver includes a logging capability that can report error,
 * informational and debug messages to a log file or to stderr. The level
 * of log messages displayed and/or stored is determined by the level set when
 * logging is enabled. All message at a log level lower than the set level
 * will be displayed / logged. Logging is performed at the laxlib level so
 * applies to all LAX cores being accessed through laxlib from the same
 * process.
 *
 * By default logging is disabled and only error messages are reported to the
 * stderr. Message reporting to the stderr is controlled with the
 * laxlib_err_level() function while the log file is created and log level set
 * with the laxlib_log_open() function.
 */
enum laxLogLevel {
    LAX_LIB_LOG_NONE = 0,   ///< No log messages are reported
    LAX_LIB_LOG_ERR,        ///< Only error log messages are reported
    LAX_LIB_LOG_WARN,       ///< Warning and error messages are reported
    LAX_LIB_LOG_INFO,       ///< All messages except debug messages are
                            ///  reported
    LAX_LIB_LOG_DBG         ///< All log messages are reported
};

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
#if defined(DEBUG_VERSION)       // only for debug
    extern uint8_t gLaxConsoleLogLevel;

    #define LAX_LOG_ERROR(...)                              \
        if(gLaxConsoleLogLevel >= (uint8_t)LAX_LIB_LOG_ERR) \
        {                                                   \
            (void)OAL_LOG_ERROR(__VA_ARGS__);               \
        }                                                   \

#else
    #define LAX_LOG_ERROR(...)
#endif

#if defined(DEBUG_VERSION)       // only for debug
    #define LAX_LOG_WARNING(...)                              \
        if(gLaxConsoleLogLevel >= (uint8_t)LAX_LIB_LOG_WARN)   \
        {                                                   \
            (void)OAL_LOG_WARNING(__VA_ARGS__);               \
        }                                                   \

#else
    #define LAX_LOG_WARNING(...)
#endif

#if defined(DEBUG_VERSION)       // only for debug
    #define LAX_LOG_DEBUG(...)                              \
        if(gLaxConsoleLogLevel >= (uint8_t)LAX_LIB_LOG_DBG)   \
        {                                                   \
            (void)OAL_LOG_DEBUG(__VA_ARGS__);               \
        }                                                   \

#else
    #define LAX_LOG_DEBUG(...)
#endif

#if defined(DEBUG_VERSION)       // only for debug
    #define LAX_LOG_INFO(...)                              \
        if(gLaxConsoleLogLevel >= (uint8_t)LAX_LIB_LOG_INFO)   \
        {                                                   \
            (void)OAL_LOG_NOTE(__VA_ARGS__);               \
        }                                                   \

#else
    #define LAX_LOG_INFO(...)
#endif

/**
* @brief          LAX command types sent to LAX dispatcher
*/
typedef uint32_t rsdkLaxCmdType_t;


/**
* @brief          LAX event types sent by LAX kernel-space/driver to user-space/library
*/
typedef enum
{   /**< @brief There are RSDK_LAX_MAX_CMDS_NUM events for each LAX core: */
    /**< @brief Host-triggered LAX 0 command completion events */
    RSDK_LAX_EVENT_LAX0_CMD0_DONE  = 0U,
    RSDK_LAX_EVENT_LAX0_CMD1_DONE,
    RSDK_LAX_EVENT_LAX0_CMD2_DONE,
    RSDK_LAX_EVENT_LAX0_CMD3_DONE,
    RSDK_LAX_EVENT_LAX0_CMD4_DONE,
    RSDK_LAX_EVENT_LAX0_CMD5_DONE,
    RSDK_LAX_EVENT_LAX0_CMD6_DONE,
    RSDK_LAX_EVENT_LAX0_CMD7_DONE,
    RSDK_LAX_EVENT_LAX0_CMD8_DONE,
    RSDK_LAX_EVENT_LAX0_CMD9_DONE,
    RSDK_LAX_EVENT_LAX0_CMD10_DONE,
    RSDK_LAX_EVENT_LAX0_CMD11_DONE,
    RSDK_LAX_EVENT_LAX0_CMD12_DONE,
    RSDK_LAX_EVENT_LAX0_CMD13_DONE,
    RSDK_LAX_EVENT_LAX0_CMD14_DONE,
    RSDK_LAX_EVENT_LAX0_CMD15_DONE,
    /**< @brief Host-triggered LAX 1 command completion events */
    RSDK_LAX_EVENT_LAX1_CMD0_DONE,
    RSDK_LAX_EVENT_LAX1_CMD1_DONE,
    RSDK_LAX_EVENT_LAX1_CMD2_DONE,
    RSDK_LAX_EVENT_LAX1_CMD3_DONE,
    RSDK_LAX_EVENT_LAX1_CMD4_DONE,
    RSDK_LAX_EVENT_LAX1_CMD5_DONE,
    RSDK_LAX_EVENT_LAX1_CMD6_DONE,
    RSDK_LAX_EVENT_LAX1_CMD7_DONE,
    RSDK_LAX_EVENT_LAX1_CMD8_DONE,
    RSDK_LAX_EVENT_LAX1_CMD9_DONE,
    RSDK_LAX_EVENT_LAX1_CMD10_DONE,
    RSDK_LAX_EVENT_LAX1_CMD11_DONE,
    RSDK_LAX_EVENT_LAX1_CMD12_DONE,
    RSDK_LAX_EVENT_LAX1_CMD13_DONE,
    RSDK_LAX_EVENT_LAX1_CMD14_DONE,
    RSDK_LAX_EVENT_LAX1_CMD15_DONE,

    /**< @brief externally-triggered LAX 0 command completion events */
    RSDK_LAX_EVENT_LAX0_SPT_CMD_0_DONE,   /**< @brief SPT-triggered LAX 0 command 0 completion  */
    RSDK_LAX_EVENT_LAX0_SPT_CMD_1_DONE,   /**< @brief SPT-triggered LAX 0 command 1 completion  */
    RSDK_LAX_EVENT_LAX0_SPT_CMD_2_DONE,   /**< @brief SPT-triggered LAX 0 command 2 completion  */
    RSDK_LAX_EVENT_LAX0_SPT_CMD_3_DONE,   /**< @brief SPT-triggered LAX 0 command 3 completion  */
    RSDK_LAX_EVENT_LAX0_CTE_CMD_0_DONE,   /**< @brief CTE-triggered LAX 0 command 0 completion  */
    RSDK_LAX_EVENT_LAX0_CTE_CMD_1_DONE,   /**< @brief CTE-triggered LAX 0 command 1 completion  */
    RSDK_LAX_EVENT_LAX0_CTE_CMD_2_DONE,   /**< @brief CTE-triggered LAX 0 command 2 completion  */
    RSDK_LAX_EVENT_LAX0_CTE_CMD_3_DONE,   /**< @brief CTE-triggered LAX 0 command 3 completion  */
    RSDK_LAX_EVENT_LAX0_LAX_CMD_0_DONE,   /**< @brief LAX1-triggered LAX 0 command 0 completion */

    /**< @brief externally-triggered LAX 1 command completion events */
    RSDK_LAX_EVENT_LAX1_SPT_CMD_0_DONE,   /**< @brief SPT-triggered LAX 1 command 0 completion  */
    RSDK_LAX_EVENT_LAX1_SPT_CMD_1_DONE,   /**< @brief SPT-triggered LAX 1 command 1 completion  */
    RSDK_LAX_EVENT_LAX1_SPT_CMD_2_DONE,   /**< @brief SPT-triggered LAX 1 command 2 completion  */
    RSDK_LAX_EVENT_LAX1_SPT_CMD_3_DONE,   /**< @brief SPT-triggered LAX 1 command 3 completion  */
    RSDK_LAX_EVENT_LAX1_CTE_CMD_0_DONE,   /**< @brief CTE-triggered LAX 1 command 0 completion  */
    RSDK_LAX_EVENT_LAX1_CTE_CMD_1_DONE,   /**< @brief CTE-triggered LAX 1 command 1 completion  */
    RSDK_LAX_EVENT_LAX1_CTE_CMD_2_DONE,   /**< @brief CTE-triggered LAX 1 command 2 completion  */
    RSDK_LAX_EVENT_LAX1_CTE_CMD_3_DONE,   /**< @brief CTE-triggered LAX 1 command 3 completion  */
    RSDK_LAX_EVENT_LAX1_LAX_CMD_0_DONE,   /**< @brief LAX0-triggered LAX 1 command 0 completion */


    RSDK_LAX_EVENT_DMA_DONE,  /**< @brief Host-triggered DMA completion, for both LAX0 and LAX1 */

    RSDK_LAX_EVENT_LAX_DRIVER_INTERNAL_ERR, /**< @brief LAX driver internal error in LaxEventHandlerThread */

    /**< @brief below: error/fault events may be ordered with the one of lowest priority first */
    RSDK_LAX_EVENT_LAX0_ILLEGALOP,          /**< @brief LAX 0 core executed an illegal operation */
    RSDK_LAX_EVENT_LAX1_ILLEGALOP,          /**< @brief LAX 1 core executed an illegal operation */
    RSDK_LAX_EVENT_DMA_FLAG_XFRERR,         /**< @brief the the LAX driver notifies about DMA transfer error */
    RSDK_LAX_EVENT_DMA_FLAG_CFGERR,         /**< @brief the the LAX driver notifies about DMA config error */
    RSDK_LAX_EVENT_CMD_CONTENT_ERR,         /**< @brief the LAX dispatcher identified a LAX command content error */
    RSDK_LAX_EVENT_TABLE_ERR,               /**< @brief the LAX dispatcher identified graph table error */
    RSDK_LAX_EVENT_PARITY_ERR,              /**< @brief the LAX dispatcher identified parity error */
    RSDK_LAX_EVENT_UNEXPECTED_INT,          /**< @brief unexpected interrupt received by LAX driver */
    RSDK_LAX_EVENT_UNEXP_DMA_COMP,          /**< @brief unexpected DMA transfer complete received by LAX driver */
    RSDK_LAX_EVENT_REGIF_ERR ,              /**< @brief potential HW fault in LAX-Host communication registers 
                                                        detected by LAX driver */

    RSDK_LAX_EVENT_CLOSE,                /**< @brief LAX driver close event */
    RSDK_LAX_MAX_EVENTS
}rsdkLaxEventType_t;

#define RSDK_LAX_EVENT_CMD_DONE_NUM         RSDK_LAX_EVENT_DMA_DONE
#define RSDK_LAX_EVENT_SAFETYCB_FIRST_EV    RSDK_LAX_EVENT_LAX0_ILLEGALOP

/**
* @brief          DMA channel identifiers.
*/
typedef enum {
    RSDK_LAX_DMA_CHANNEL_0 = 0 ,  //!< Identifies DMA channel #0.
    RSDK_LAX_DMA_CHANNEL_1 ,      //!< Identifies DMA channel #1.
    RSDK_LAX_DMA_CHANNEL_2 ,      //!< Identifies DMA channel #2.
    RSDK_LAX_DMA_CHANNEL_3 ,      //!< Identifies DMA channel #3.
    RSDK_LAX_DMA_CHANNEL_4 ,      //!< Identifies DMA channel #4.
    RSDK_LAX_DMA_CHANNEL_5 ,      //!< Identifies DMA channel #5.
    RSDK_LAX_DMA_CHANNEL_6 ,      //!< Identifies DMA channel #6.
    RSDK_LAX_DMA_CHANNEL_7 ,      //!< Identifies DMA channel #7.
    RSDK_LAX_DMA_CHANNEL_8 ,      //!< Identifies DMA channel #8.
    RSDK_LAX_DMA_CHANNEL_9 ,      //!< Identifies DMA channel #9.
    RSDK_LAX_DMA_CHANNEL_10,      //!< Identifies DMA channel #10.
    RSDK_LAX_DMA_CHANNEL_11,      //!< Identifies DMA channel #11.
    RSDK_LAX_DMA_CHANNEL_12,      //!< Identifies DMA channel #12.
    RSDK_LAX_DMA_CHANNEL_13,      //!< Identifies DMA channel #13.
    RSDK_LAX_DMA_CHANNEL_14,      //!< Identifies DMA channel #14.
    RSDK_LAX_DMA_CHANNEL_15,      //!< Identifies DMA channel #15.
    RSDK_LAX_DMA_CHANNEL_16,      //!< Identifies DMA channel #16.
    RSDK_LAX_DMA_CHANNEL_17,      //!< Identifies DMA channel #17.
    RSDK_LAX_DMA_CHANNEL_18,      //!< Identifies DMA channel #18.
    RSDK_LAX_DMA_CHANNEL_19,      //!< Identifies DMA channel #19.
    RSDK_LAX_DMA_CHANNEL_20,      //!< Identifies DMA channel #20.
    RSDK_LAX_DMA_CHANNEL_21,      //!< Identifies DMA channel #21.
    RSDK_LAX_DMA_CHANNEL_22,      //!< Identifies DMA channel #22.
    RSDK_LAX_DMA_CHANNEL_23,      //!< Identifies DMA channel #23.
    RSDK_LAX_DMA_CHANNEL_24,      //!< Identifies DMA channel #24.
    RSDK_LAX_DMA_CHANNEL_25,      //!< Identifies DMA channel #25.
    RSDK_LAX_DMA_CHANNEL_26,      //!< Identifies DMA channel #26.
    RSDK_LAX_DMA_CHANNEL_27,      //!< Identifies DMA channel #27.
    RSDK_LAX_DMA_CHANNEL_28,      //!< Identifies DMA channel #28.
    RSDK_LAX_DMA_CHANNEL_29,      //!< Identifies DMA channel #29.
    RSDK_LAX_DMA_CHANNEL_30,      //!< Identifies DMA channel #30.
    RSDK_LAX_DMA_CHANNEL_31,      //!< Identifies DMA channel #31.
    RSDK_LAX_DMA_CHANNEL_COUNT    //!< Number of DMA channels.
} rsdkLaxDmaChannel_t;

/**
* @brief          LAX command structure
*/
typedef struct
{
    unsigned int bufNum: 8;         /**< @brief  Number of data buffers. */
    unsigned int cmdType: 8;        /**< @brief  Command type. */
    unsigned int reserved: 8;       /**< @brief  Reserved for future use. */
    unsigned int sid: 8;            /**< @brief  Command Sequence ID. */
}rsdkLaxCmdHeader_t;

typedef struct
{
    uint32_t ptrVal[AXI_BUS_WORD32_WIDTH];
}rsdkLaxCmdPtr_t;

typedef struct
{
    rsdkLaxCmdPtr_t    nextCmdPtr;    /**< @brief  This is set to 0 when a new LAX command is sent out. 
                                            The next command overlaps it with its own thisCmdPtr field */
    rsdkLaxCmdHeader_t cmdHeader;     /**< @brief  LAX command header. */
    uint32_t cmdFuncInfo;              /**< @brief  LAX command id as registered via the registration API in. */
}rsdkLaxCmdPre_t;

/**
* @brief          Maximal layout of the LAX command structure.
*                 Individual commands may have less buffers, as indicated by bufNum info
*/
typedef struct  
{
    rsdkLaxCmdPre_t common;              
    uint32_t cmdBufInfo[(2U * RSDK_LAX_MAX_FUNC_ARGS)]; 
    rsdkLaxCmdPtr_t thisCmdPtr;  //this area is used only if RSDK_LAX_MAX_FUNC_ARGS     
}rsdkLaxCmdLayout_t;

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* RSDK_LAX_COMMON_H */
