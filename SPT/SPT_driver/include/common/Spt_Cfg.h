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
#include "rsdk_status_helper.h"
#include "rsdk_toolchain_helper.h"
#include "rsdk_osenv.h"

#if !defined(RSDK_OSENV_SA)
#error "RSDK_OSENV_SA not defined! Please include rsdk_osenv.h before using this macro"
#endif

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

/*================================================================================================*/

#if (SPT_DEV_ERROR_DETECT == STD_ON) && (SPT_DEV_HALT_ON_ERROR == STD_ON)
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

/* Formal API id for some SPT driver functions groups, to be used at development time             */
typedef enum
{
    SPT_VERSION_INFO_CHECK = 0u,    /* error at VersionInfo call                  */
    SPT_API_CALL,                   /* error in API call                          */
    SPT_PARAM_SET,                  /* error when setting parameters              */
    SPT_PARAM_CHECK,                /* error when checking parameters             */
    SPT_EXEC_START,                 /* error at SPT execution start               */
    SPT_EXEC_POLL,                  /* error when polling for SPT execution end   */
    SPT_HW_STOP,                    /* error when stopping HW                     */
    SPT_HW_CHECK,                   /* error when checking for HW errors          */
    SPT_IRQ_CHECK,                  /* error in IRQ handlers                      */
} Spt_ApiIdType;

/*================================================================================================*/

/* Development error codes (passed to DET). */

/** @ingroup spt_driver_api_const
* @defgroup spt_driver_api_err SPT Error codes
* @{
*/

/**
* @brief          Return datatype for SPT API calls.
* @details        API functions either return a Spt_ErrStatusType value or void. These error codes are derived from rsdkStatus_t.
*/
typedef enum
{
    SPT_E_INVALID_PARAM = RSDK_SPT_RET_ERR_INVALID_PARAM - RSDK_SPT_STATUS_BASE, /**< Driver API error: Parameter value or
                                                                                    combination of values not supported. */
    SPT_E_INVALID_STATE, /**< Driver API error: operation not supported in current state. */
    SPT_E_TIMEOUT_BLOCK, /**< Driver error: SPT blocking call did not finish in due time and did not report any specific error.
                            This is treated as a faulty state and imposes re-initialization of the driver.*/

    SPT_E_WARN_HW_BUSY, /**< Driver API warning: Input parameters are valid but no action was done
                            by current function call because the SPT hardware is busy. */

    /*SPT hw error codes (signaled by interrupt):*/
    SPT_E_MEM,    /**< SPT hw error: internal memory handling, triggers an ECS interrupt.
                    MEM_ERR_STATUS register value is passed to the user callback (rsdkSptIsrCb_t) */
    SPT_E_DMA,    /**< SPT hw error: SDMA operation, triggers an ECS interrupt.
                    GBL_STATUS register value is passed to the user callback (rsdkSptIsrCb_t) */
    SPT_E_HW_ACC, /**< SPT hw error: tried to execute an instruction with illegal operands or configuration, triggered an ECS interrupt.
                    HW_ACC_ERR_STATUS register value is passed to the user callback (rsdkSptIsrCb_t) */
    SPT_E_ILLOP,  /**< SPT hw error: Illegal SPT instruction, triggers an ECS interrupt.
                    CS_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/

    SPT_E_TIMEOUT_START, /**< Driver error: SPT kernel launcher function was not able to verify that all SPT hw status bits were properly cleared.
                            This is treated as a faulty state and imposes re-initialization of the driver*/
    SPT_E_TIMEOUT_STOP, /**< Driver error: RsdkSptStop function was not able to verify that SPT hw state machine has been reset to "START" state.
                            This signals a hardware fault.*/

    SPT_E_WARN_UNEXPECTED_STOP, /**< Driver API warning: Spurious SPT_IRQ_ECS interrupt was triggered by the CS_STATUS0[PS_STOP] bit,
                                    while the SPT command sequencer did not reach a STOP instruction, or the SPT Driver
                                    had not launched any kernel. */
    SPT_E_HIST_OVF0,     /**< SPT hw error: HIST overflow. HIST_OVF_STATUS0 register value is passed to the user callback (rsdkSptIsrCb_t) */
    SPT_E_HIST_OVF1,     /**< SPT hw error: HIST overflow. HIST_OVF_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t) */
    SPT_E_IRQ_REG,       /**< SPT error: the irq handler was not registered. */
    SPT_E_UNMAP_SPT_MEM, /**< Driver error: Failed to unmap SPT registers' addresses. */

    SPT_E_WR,            /**< SPT hw error: WR or SPR access error.
                            WR_ACCESS_ERR_REG register value is passed to the user callback (rsdkSptIsrCb_t)*/
    SPT_E_ILLOP_SCS0,    /**< SPT hw error: Illegal SPT instruction in Slave Command Sequencer0.
                            SCS0_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/
    SPT_E_ILLOP_SCS1,    /**< SPT hw error: Illegal SPT instruction in Slave Command Sequencer1.
                            SCS1_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/
    SPT_E_ILLOP_SCS2,    /**< SPT hw error: Illegal SPT instruction in Slave Command Sequencer2.
                            SCS2_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/
    SPT_E_ILLOP_SCS3,    /**< SPT hw error: Illegal SPT instruction in Slave Command Sequencer3.
                            SCS3_STATUS1 register value is passed to the user callback (rsdkSptIsrCb_t)*/
    SPT_E_HW2_ACC, /**< SPT hw error: tried to execute an instruction with illegal operands or configuration, related to the 2nd instances
                            of the SPT Accelerator modules (e.g. FFT2, MAXS2 etc). Triggers an ECS interrupt.
                            HW2_ACC_ERR_STATUS register value is passed to the user callback (rsdkSptIsrCb_t) */
    SPT_E_WARN_DRV_BUSY,            /**< Driver API warning: an API call is already in progress on another thread */
    SPT_E_API_INIT_LOCK_FAIL,       /**< Driver API error: could not init mutex for controlling multithreaded API call sequence */
    SPT_E_API_ENTER_LOCK_FAIL,      /**< Driver API error: could not lock mutex for controlling multithreaded API call sequence */
    SPT_E_API_ENTER_UNLOCK_FAIL,    /**< Driver API error: could not unlock mutex for controlling multithreaded API call sequence */
    SPT_E_API_EXIT_LOCK_FAIL,       /**< Driver API error: could not lock mutex for controlling multithreaded API call sequence */
    SPT_E_API_EXIT_UNLOCK_FAIL,     /**< Driver API error: could not unlock mutex for controlling multithreaded API call sequence */
    SPT_E_THR_CREATE,               /**< Driver API error: could not create a thread for detecting OS kernel event in user space*/
    SPT_E_THR_TERM,                 /**< Driver API error: could not terminate the thread used for detecting OS kernel event in user space*/
    SPT_E_OAL_COMM_INIT,        /**< Driver API error: could not initialize OAL communication channel for transmitting OS kernel events to user space*/
    SPT_E_CHECK_WATERMARK,      /**< Driver error: Failed to check if the watermark instruction is placed at the start of the kernel code. */
    SPT_E_INIT_Q_FAIL,          /**< Driver error: Failed to init the queue used to handle irq data processing on separate thread. */
    SPT_E_BBE32_REBOOT,         /**< Driver API error: could not reboot the BBE32 */
    SPT_E_INVALID_KERNEL, /**< Driver error: detected invalid SPT kernel code, which does not start with the mandatory watermarking instruction.
                            See also #SPT_KERNEL_WATERMARK */
    SPT_E_HW_RST,         /**< SPT error: hardware is in unexpected RST state */

    SPT_E_OTHER = 0xFFU, /**< Any other return status not covered above.
                                       No SPT error codes should be defined with a value greater than this one.*/
} Spt_ErrStatusType;

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
