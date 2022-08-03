/*
* Copyright 2022 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CSI2_CFG_H
#define CSI2_CFG_H

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
    #define CSI2_DEV_ERROR_DETECT   STD_ON
    #define CSI2_DEV_HALT_ON_ERROR  STD_OFF
    #define     CSI2_DFS_NOT_USED       1u          /* DFS is not reset by the CSI2 driver          */
    #define     CSI2_DFS_ONCE           2u          /* DFS is reset only once by the CSI2 driver    */
    #define     CSI2_DFS_ALWAYS         3u          /* DFS is reset every init by the CSI2 driver   */
/* Pre-processor switch to define the DFS reset usage during the Csi2 initialization */
    #ifndef CSI2_DFS_USAGE
        #define CSI2_DFS_USAGE                          CSI2_DFS_ONCE
    #endif

/* Pre-processor switch to define single Csi2 management thread or multiple Csi2 management threads.
 * If single thread (which is the normal approach) - there are no necessary exclusive areas for the driver    */
#ifndef CSI2_SINGLE_MANAGEMENT_THREADS
#define CSI2_SINGLE_MANAGEMENT_THREADS          STD_ON
#endif


/* Pre-processor switch to enable/disable version info report for Csi2 API                      */
#ifndef CSI2_VERSION_INFO_API
#define CSI2_VERSION_INFO_API                   STD_ON
#endif

/* Pre-processor switch to enable/disable statistics usage for received data for Csi2 API       */
#ifndef CSI2_STATISTIC_DATA_USAGE
#define CSI2_STATISTIC_DATA_USAGE               STD_ON
#endif

/* Pre-processor switch to enable/disable DC auto compensation for received data on CSI2 API    */
#if (CSI2_STATISTIC_DATA_USAGE == STD_ON)
    #ifndef CSI2_AUTO_DC_COMPENSATION
        #define CSI2_AUTO_DC_COMPENSATION       STD_ON
    #endif
#endif

/* Pre-processor switch to enable/disable auxiliary data usage for received data on CSI2 API    */
#ifndef CSI2_AUXILIARY_DATA_USAGE
#define CSI2_AUXILIARY_DATA_USAGE               STD_ON
#endif

/* Pre-processor switch to enable/disable metadata usage for received data on CSI2 API    */
#ifndef CSI2_METADATA_DATA_USAGE
#define CSI2_METADATA_DATA_USAGE                STD_ON
#endif

/* Pre-processor switch to enable/disable single ISR callback for CSI2 API                      */
#ifndef CSI2_SINGLE_CALLBACK_USAGE
#define CSI2_SINGLE_CALLBACK_USAGE              STD_ON
#endif

/* Pre-processor switch to enable/disable Rx start/stop usage in CSI2 API                       */
#ifndef CSI2_RX_START_STOP_USAGE
#define CSI2_RX_START_STOP_USAGE                STD_ON
#endif

/* Pre-processor switch to enable/disable power on/off usage in CSI2 API                        */
#ifndef CSI2_POWER_ON_OFF_USAGE
#define CSI2_POWER_ON_OFF_USAGE                 STD_ON
#endif

/* Pre-processor switch to enable/disable secondary functions usage in CSI2 API                 */
#ifndef CSI2_SECONDARY_FUNCTIONS_USAGE
#define CSI2_SECONDARY_FUNCTIONS_USAGE          STD_ON
#endif

/* Pre-processor switch to enable/disable internal frames counter usage in CSI2 API             */
#ifndef CSI2_FRAMES_COUNTER_USED
#define CSI2_FRAMES_COUNTER_USED                STD_ON
#endif

/* Pre-processor switch to enable/disable usage of GPIO in CSI2 API                             */
#ifndef CSI2_GPIO_USED
#define CSI2_GPIO_USED                          STD_ON
#endif

/* Pre-processor switch to enable/disable usage of SDMA in CSI2 API                             */
#ifndef CSI2_SDMA_USED
#define CSI2_SDMA_USED                          STD_ON
#endif


/* Formal instance id for CSI2 driver, to be used at development time                           */
#define CSI2_INSTANCE_ID                        0u

#define CSI2_US_DELAY   1000u

/* Formal API id for some CSI2 driver functions groups, to be used at development time          */
#define CSI2_VERSION_INFO_CHECK                 0u      /* error at VersionInfo call                        */
#define CSI2_SETUP_MAIN_PARAM_CHECK             1u      /* error at setup, main parameters check            */
#define CSI2_SETUP_VC_PARAM_CHECK               2u      /* error at setup, VC parameters check              */
#define CSI2_SETUP_AUX_PARAM_CHECK              3u      /* error at setup, auxiliary data parameters check  */
#define CSI2_SETUP_MD_PARAM_CHECK               4u      /* error at setup, metadata parameters check        */
#define CSI2_SETUP_MODULE_INIT                  5u      /* error at setup, during real setup                */
#define CSI2_RX_STOP                            6u      /* error when calling RxStop                        */
#define CSI2_RX_START                           7u      /* error when calling RxStart                       */
#define CSI2_POWER_OFF                          8u      /* error when calling PowerOff                      */
#define CSI2_POWER_ON                           9u      /* error when calling PowerOn                       */
#define CSI2_GET_VALUE                          10u     /* error when calling a get function                */


#if (CSI2_DEV_HALT_ON_ERROR == STD_ON)
/* Infinite loop if error detected and the option is to detect development errors */
    #define CSI2_HALT_ON_ERROR          while(gsCsi2LoopExit != TRUE)   \
                                        {                               \
                                            ;   /* empty loop */        \
                                        }
#else
    #define CSI2_HALT_ON_ERROR
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

#endif /* CSI2_CFG_H */
