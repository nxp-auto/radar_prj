/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef RSDK_LAX_COMMON_H
#define RSDK_LAX_COMMON_H

/**
* @file           rsdk_lax_common.h
* @brief          include file common to both Host and VSPA projects;  
*                 the parameters in this file are NOT user-configurable
*                 the user-configurable parameters ar in lax_config.h
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
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
#define TRUE 1U
#define FALSE 0U

/**
* @brief          The maximum number of inflight LAX commands.
* @details        The maximum number of commands (running and queued) accepted from host, at 
*                 any moment. If a new command is received when LAX_MAX_CMDS_NUM are already in
*                 execution or queued for execution, the new command will not be enqueued.
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
* @brief          CTE interrupt flag offset
*/
#define RSDK_LAX_CTE_INT_OFFSET    4U

/**
* @brief          The DMA channel for LAX commands.
* @details        The definition is used on host and on vspa
*
*/
#define RSDK_LAX_DMA_CMD_CHANNEL 3U

/**
* @brief          The DMA channel for CRC computation and CRC transfer to host.
* @details        The definition is used on host and on vspa
*
*/
#define RSDK_LAX_DMA_CRC_CHANNEL 4U

/**
* @brief          The size in bytes of struct libvspa_cmd.
* @details        This needs to stay in sync with definition of libvspa_cmd on host
*                 and aviCmdHdr_t on vspa side.
*
*/
#define RSDK_LAX_SIZEOF_LIBVSPACMD 36

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
#define RSDK_LAX_CRC_BIT_LEN  128

/**
* @brief          LAX signal to user-space
* @details        This definition is used on host, in LAX driver and LAX abstraction
*/
#define RSDK_LAX_EVENT_SIGNAL  45

/**
* @brief          The signal type mask 
* @details        (~LAX_SIGNAL_TYPE_MASK) is the additional singal info.
*                 This definition is used on host (abstraction and driver).
*/
#define RSDK_LAX_SIGNAL_TYPE_MASK (0xFF000000U)

/**
* @brief          The signal type shift 
*/
#define RSDK_LAX_SIGNAL_TYPE_SHIFT (24)

/**
* @brief          LAX signal completion flag
* @details        Configure the program to be notified of the command completion through signals.
*                 This definition is used on host (abstraction and driver).
*/
#define RSDK_LAX_SEND_COMPLETION_SIGNAL (0x1U)
/**
* @brief          The event completion flag
* @details        Configure the program to be notified of the command completion through events.
*                 This definition is used on host (abstraction and driver).
*/
#define RSDK_LAX_SEND_COMPLETION_EVENT (0x0U)
/**
* @brief          The event completion flag mask
* @details        The mask is used to select the bit in flags corresponding to command completion.
*                 Defaults to LAX_SEND_COMPLETION_EVENT
*                 This definition is used on host (abstraction and driver).
*/
#define RSDK_LAX_SEND_COMPLETION_MASK  (0x1U)


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
* @brief          Success and error codes in LAX operations.
*                 todo:    rename type as laxErrorCode_t, eventually unify to other rsdk drivers error codes
*/
typedef enum
{
    RSDK_SUCCESS = 0U,               /**< @brief Indicates successful operation. */

    //Driver API error codes:
    RSDK_LAX_ERR_INVALID_PARAM,          /**< @brief Parameter value or combination not supported. */
    RSDK_LAX_ERR_NOT_INIT,               /**< @brief Module not initialized. */
    RSDK_LAX_ERR_SEQUENCE,               /**< @brief Wrong API call sequence. */
    RSDK_LAX_ERR_CTXT,                   /**< @brief Invalid context used for this API call. */
    RSDK_LAX_ERR_CFG,                    /**< @brief Operation failed during configuration phase. */
    RSDK_LAX_ERR_EXEC,                   /**< @brief Operation failed during execution phase. */
    RSDK_LAX_ERR_TIMEOUT,                /**< @brief Operation did not finish in due time. */
    RSDK_LAX_ERR_NO_RES,                 /**< @brief Operation failed due to unavailable resource 
                                                (e.g. full circular buffer, no sequence Id, full DMA queue)  */
    RSDK_LAX_ERR_INTERNAL,               /**< @brief Internal error. */
    RSDK_LAX_ERR_STATE,                  /**< @brief API call not supported in current operation mode. */
    RSDK_LAX_ERR_NO_EVENT,               /**< @brief API no expected event type in the queue. */
    RSDK_LAX_ERR_ERROR_STATE,            /**< @brief Module in ERROR state, after an unsuccessful recovery. 
                                                Module is permanently disabled*/

    RSDK_LAX_ERR_OTHER = 100,            /**< @brief Any other return status not covered above. */

    // warnings:
    RSDK_LAX_WARN_NO_ACTION = 0x200,     /**< @brief Input parameters are valid but no action was done by current function call. */

    // user-defined status codes:
    RSDK_LAX_USER_DEF = 0x1000           /**< @brief User-defined codes start from this offset.*/
} rsdkLaxResult_t;


/**
* @brief          LAX command types sent to LAX dispatcher
*/
typedef enum
{
    RSDK_LAX_CMD_EXEC = 0U,               /**< @brief Command for LAX graph execution  */
    RSDK_LAX_CMD_CFG_REPLY_BUF,           /**< @brief Command for reply buffers configuration  */
    RSDK_LAX_CMD_CFG_CTE_GRAPH,           /**< @brief CTE table entry configuration command */
    RSDK_LAX_CMD_CFG_SPT_GRAPH,           /**< @brief SPT table entry configuration command */
}rsdkLaxCmdType_t;

/**
* @brief          LAX signal types sent by LAX driver
*/
typedef enum
{
    RSDK_LAX_SIG_CRC_FAIL = 0U,      /**< @brief CRC failure signal  */
    RSDK_LAX_SIG_COMMAND_DONE,       /**< @brief Host-triggered command completion signal */
    RSDK_LAX_SIG_DMA_DONE,           /**< @brief Host-triggered DMA completion signal */
    RSDK_LAX_SIG_CTE_COMMAND_DONE,   /**< @brief CTE-triggered command completion signal  */
    RSDK_LAX_SIG_SPT_COMMAND_DONE,   /**< @brief SPT-triggered command completion signal  */
}rsdkLaxSigType_t;

/**
* @brief          DMA channel identifiers.
*/


typedef enum {
    RSDK_LAX_DMA_CHANNEL_NONE = -1,
    RSDK_LAX_DMA_CHANNEL_0 ,      //!< Identifies DMA channel #0.
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
