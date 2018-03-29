/*
 * Copyright 2017-2018 NXP
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
* @brief          lax framework static configuration for both Host and VSPA projects  
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
* @brief        The maximum number of LAX functions parameters.
* @details      The chosen value needs to take into account the size of the LAX command buffer.
*               Note: For Linux, the VSPA driver, VSPA user space library and VSPA-side dispatcher
*               need to be rebuilt if values other than default are needed. 
*/
#define RSDK_LAX_MAX_FUNC_ARGS               64U


/**
* @brief        AVI (ARM-VSPA Interface) configuration parameter.
* @details      Command Buffer Size must match aviDmemCmdBufSzWords in linker lcf.
*               Make sure AVI_CMD_BUF_SZ_WORDS_32 == linker's lcf aviDmemCmdBufSzWords
*               Note: For Linux, the VSPA driver, VSPA user space library and VSPA-side dispatcher
*               need to be rebuilt if values other than default are needed. 
*/

#define AVI_CMD_BUF_SZ_WORDS_32         256U 


/**
* @brief        Enable/Disable Lax Trace
* @details      Comment or decomment the define in order to include or
*               exclude the trace module from building.
*
 */
//#define LAX_TRACE                       1U

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

#endif /* LAX_CONFIG_H */ 
