/*
 * Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef RSDK_LAX_CONFIG_H
#define RSDK_LAX_CONFIG_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @file           rsdk_lax_config.h
* @brief          lax framework static configuration for both Host and LAX projects  
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/** @addtogroup lax_static_config
* @{
*/

/**
* @brief        The maximum number of host-triggered LAX graphs (per LAX core).
* @details      This is the maximum number of host-triggered LAX graphs that
*               can be registered on a LAX core. 
*/
#define RSDK_LAX_CORE_MAX_NUM_HOST_FUNC_GRAPHS             64u


/**
* @brief        The maximum number of LAX functions parameters.
* @details      The chosen value needs to take into account the size of the LAX command buffer.
*               Note: Should be larger than 24U
*               Note: For Linux, LAX driver user space library and LAX-side dispatcher
*               need to be rebuilt if values other than default are needed. 
*/
#define RSDK_LAX_MAX_FUNC_ARGS               32U


/**
* @brief        Timeout used in DMA transfers for ELD sections download.
* @details      Timeout in miliseconds.
*/
#define RSDK_LAX_DMA_TRANSFER_TIMEOUT_MSEC               100U

/**
* @brief          Size of the buffer used to download LAX elf sections to LAX cores. (bytes)
* @details        Half its size should be AXI bus width aligned. 
*                 Not larger than maximum DMA transfer size (64 kilobytes).
*                 Using a larger buffer speeds-up the text&data sections download to LAX cores.
* 
*/
#define RSDK_LAX_ELD_BUF_SIZE 0x1000U


/**
* @brief        Command Buffer Size (am ARM-LAX Interface configuration parameter).
* @details      To asses the required size of the LAX Command Buffer, one may use LAX driver
*               API function RsdkLaxCmdSizeCompute to find out the space consumed
*               in the command buffer by each of the commands used by an application.
*               To explain the size requirements for the command buffer consider a few 
*               definitios as below.
*               Define a synchronization point, Pk, as a point in the program when the command 
*               buffer is empty (no command has been launched or all commands have completed).
*               Consider a set P0, P1, P2, ..,Pn of synchronization points in the application.
*               Further, define Ck as the set of commands running between two consecutive
*               synchronization points Pk-1 and Pk, with k=1..n.
*               For a set of commands Ck, define Sk as the sum of sizes of commands in Ck.
*               To reserve enough space in the circular command buffer, its size should be 
*               larger than MAX(Sk), with k=1..n.
*
*               Note: For Linux, LAX driver user space library and LAX dispatcher
*               need to be rebuilt if values other than default are needed. 
*/

/**
* @brief        The maximum number of available trace entries.
* @details      The chosen value needs to take into account the memory requirements of the graph
*               since this adds to it.
 */
#define NUM_LOG_DATA                    30U

/**
* @brief          Mask for command buffer size value bits
*/
#define RSDK_LAX_CMD_BUF_SZ_MASK                        0x000001FFU

/**
* @brief          Command buffer size value
*/
#define RSDK_LAX_CMD_BUF_SZ_WORDS_32                    256U

/** @}*/
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

#ifdef __cplusplus
}
#endif

#endif /* RSDK_LAX_CONFIG_H */ 
