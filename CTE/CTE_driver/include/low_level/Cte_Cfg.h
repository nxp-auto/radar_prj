/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CTE_CFG_H
#define CTE_CFG_H

/**
*   @file
*
*   @internal
*   @addtogroup CTE
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
    #define CTE_DEV_ERROR_DETECT   STD_ON
    #define CTE_DEV_HALT_ON_ERROR  STD_OFF

/* Pre-processor switch to define single Cte management thread or multiple Cte management threads.
 * If single thread (which is the normal approach) - there are no necessary exclusive areas for the driver    */
#ifndef CTE_SINGLE_MANAGEMENT_THREADS
#define CTE_SINGLE_MANAGEMENT_THREADS          STD_ON
#endif

/* Pre-processor switch to enable/disable version info report for Cte API                      */
#ifndef CTE_VERSION_INFO_API
#define CTE_VERSION_INFO_API                   STD_ON
#endif

/* Pre-processor switch to enable/disable Rx start/stop usage in CTE API                       */
#ifndef CTE_START_STOP_USAGE
#define CTE_START_STOP_USAGE                STD_ON
#endif



/* Formal instance id for CTE driver, to be used at development time                           */
#define CTE_INSTANCE_ID                        0u

#define CTE_US_DELAY   1000u

/* Formal API id for some CTE driver functions groups, to be used at development time          */
#define CTE_VERSION_INFO_CHECK                 0u      /* error at VersionInfo call                        */
#define CTE_SETUP_PARAM_CHECK                  2u      /* error at setup, VC parameters check              */
#define CTE_SETUP_AUX_PARAM_CHECK              3u      /* error at setup, auxiliary data parameters check  */
#define CTE_SETUP_MD_PARAM_CHECK               4u      /* error at setup, metadata parameters check        */
#define CTE_SETUP_MODULE_INIT                  5u      /* error at setup, during real setup                */
#define CTE_RX_STOP                            6u      /* error when calling RxStop                        */
#define CTE_RX_START                           7u      /* error when calling RxStart                       */
#define CTE_POWER_OFF                          8u      /* error when calling PowerOff                      */
#define CTE_POWER_ON                           9u      /* error when calling PowerOn                       */
#define CTE_GET_VALUE                          10u     /* error when calling a get function                */

/* error report management      */
    #define CTE_REPORT_ERROR(a,b,c)  rez = (a)


#if (CTE_DEV_HALT_ON_ERROR == STD_ON)
/* Infinite loop if error detected and the option is to detect development errors */
    #define CTE_HALT_ON_ERROR          while(gsCteLoopExit != TRUE)   \
                                        {                               \
                                            ;   /* empty loop */        \
                                        }
#else
    #define CTE_HALT_ON_ERROR
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

#endif /* CTE_CFG_H */
