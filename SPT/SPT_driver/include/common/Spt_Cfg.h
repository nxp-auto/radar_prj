/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SPT_CFG_H
#define SPT_CFG_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/




#include "S32R45_SPT.h"






#include "rsdk_status.h"
#include "rsdk_toolchain_helper.h"
#include "rsdk_osenv.h"





#if defined(TRACE_ENABLE)
#include "trace.h"
#endif

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

/* Pre-processor switch to enable/disable development error detection for Spt API */
#ifndef SPT_DEV_ERROR_DETECT
#define SPT_DEV_ERROR_DETECT                    STD_ON
#endif

/* Pre-processor switch to enable/disable support for exclusive areas in the SPT driver
 * If all SPT driver calls are made from the same thread, there is no need for exclusive ares.    */
#ifndef SPT_SINGLE_THREAD
#define SPT_SINGLE_THREAD                       STD_ON
#endif

/* Pre-processor switch to enable/disable version info report for Spt API                         */
#ifndef SPT_VERSION_INFO_API
#define SPT_VERSION_INFO_API                    STD_ON
#endif

/* Pre-processor switch to enable/disable DSP support from the SPT driver                         */
#ifndef SPT_DSP_ENABLE
#define SPT_DSP_ENABLE                          STD_ON
#endif

/* Pre-processor switch to enable/disable polling support from the SPT driver                     */
#ifndef SPT_RUN_POLL
#define SPT_RUN_POLL                            STD_ON
#endif

/*================================================================================================*/

/* Formal instance id for SPT driver, to be used at development time                              */
#define SPT_INSTANCE_ID                         0u

/* Formal API id for some SPT driver functions groups, to be used at development time             */
#define SPT_VERSION_INFO_CHECK                  0u  /* error at VersionInfo call                  */
#define SPT_API_CALL                            1u  /* error in API call                          */
#define SPT_PARAM_SET                           2u  /* error when setting parameters              */
#define SPT_PARAM_CHECK                         3u  /* error when checking parameters             */
#define SPT_EXEC_START                          4u  /* error at SPT execution start               */
#define SPT_EXEC_POLL                           5u  /* error when polling for SPT execution end   */
#define SPT_HW_STOP                             6u  /* error when stopping HW                     */
#define SPT_HW_CHECK                            7u  /* error when checking for HW errors          */
#define SPT_IRQ_CHECK                           8u  /* error in IRQ handlers                      */

/*================================================================================================*/

/* Development error codes (passed to DET). */

/**
* @brief [ERROR] Driver API error: Parameter value or combination of values not supported.
* @implements    SPT_DET_ERRORS_define
* */
#define SPT_E_INVALID_PARAM                         ((uint8)0x00U)

/**
* @brief [ERROR] Driver API error: operation not supported in current state.
* */
#define SPT_E_INVALID_STATE                         ((uint8)0x01U)

/**
* @brief [ERROR] Operation reached timeout. This is treated as a faulty state and imposes re-initialization of the driver.
* */
#define SPT_E_TIMEOUT                               ((uint8)0x02U)

/**
* @brief [ERROR] Driver failed to check for SPT_KERNEL_WATERMARK at the start of the SPT kernel code.
* */
#define SPT_E_WATERMARK                             ((uint8)0x03U)

/**
* @brief [WARNING] Input parameters are valid but no action was done by current function call because the SPT hardware is busy.
* */
#define SPT_E_WARN_HW_BUSY                          ((uint8)0x04U)

/**
* @brief [WARNING] Spurious SPT_IRQ_ECS interrupt was triggered by the CS_STATUS0[PS_STOP] bit.
* */
#define SPT_E_WARN_UNEXPECTED_STOP                  ((uint8)0x05U)

/**
* @brief [ERROR] SPT hardware error. Hardware is in unexpected RST state.
* */
#define SPT_E_HW_RST                                ((uint8)0x06U)

/**
* @brief [ERROR] SPT hardware error. Internal memory handling, triggers an ECS interrupt.
*                            MEM_ERR_STATUS register value is passed to the user callback.
* */
#define SPT_E_MEM                                   ((uint8)0x07U)

/**
* @brief [ERROR] SPT hardware error. Tried to execute an instruction with illegal operands or configuration, triggered an ECS interrupt.
*                            HW_ACC_ERR_STATUS register value is passed to the user callback.
* */
#define SPT_E_HW_ACC                                ((uint8)0x08U)

/**
* @brief [ERROR] SPT hardware error. Illegal SPT instruction, triggers an ECS interrupt.
*                            CS_STATUS1 register value is passed to the user callback.
* */
#define SPT_E_ILLOP                                 ((uint8)0x09U)

/**
* @brief [ERROR] SPT hardware error. HIST overflow. HIST_OVF_STATUS0 register value is passed to the user callback.
* */
#define SPT_E_HIST_OVF0                             ((uint8)0x10U)

/**
* @brief [ERROR] SPT hardware error. HIST overflow. HIST_OVF_STATUS1 register value is passed to the user callback.
* */
#define SPT_E_HIST_OVF1                             ((uint8)0x11U)

/**
* @brief [ERROR] SPT hardware error. PDMA operation, triggers an ECS interrupt. DMA_ERR_STATUS register value is passed to the user callback.
* */
#define SPT_E_DMA                                   ((uint8)0x12U)

/**
* @brief [ERROR] SPT hardware error. WR or SPR access error. WR_ACCESS_ERR_REG register value is passed to the user callback.
* */
#define SPT_E_WR                                    ((uint8)0x13U)

/**
* @brief [ERROR] SPT hardware error. Undefined SPT error.
* */
#define SPT_E_OTHER                                 ((uint8)0x14U)















#if (SPT_SINGLE_THREAD == STD_OFF)
/**
* @brief [WARNING] An API call is already in progress on another thread.
* */
#define SPT_E_WARN_DRV_BUSY                         ((uint8)0x17U)
#endif

/*================================================================================================*/

#if (SPT_DEV_ERROR_DETECT == STD_ON)
/* Infinite loop if error detected and the option is to detect development errors */
#define SPT_HALT_ON_ERROR          while(1)                         \
                                    {                               \
                                        ;   /* empty loop */        \
                                    }
#else
#define SPT_HALT_ON_ERROR
#endif

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

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

#endif /* SPT_CFG_H */
