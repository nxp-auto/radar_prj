/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CSI2_CFG_H
#define CSI2_CFG_H


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
    #define CSI2_DEV_ERROR_DETECT       STD_ON
    #define CSI2_DEV_HALT_ON_ERROR      STD_OFF
    #define     CSI2_DFS_NOT_USED       1u          /* DFS is not reset by the CSI2 driver          */
    #define     CSI2_DFS_ONCE           2u          /* DFS is reset only once by the CSI2 driver    */
    #define     CSI2_DFS_ALWAYS         3u          /* DFS is reset every init by the CSI2 driver   */
/* Pre-processor switch to define the DFS reset usage during the Csi2 initialization */
    #ifndef CSI2_DFS_USAGE
        #define CSI2_DFS_USAGE                      CSI2_DFS_ONCE
    #endif

/* Pre-processor definitions for OSIF_COUNTER                                                       */
#define OSIF_COUNTER_DUMMY      0
#define OSIF_COUNTER_SYSTEM     1
#define OSIF_COUNTER_CUSTOM     2

    #define CSI2_SINGLE_MANAGEMENT_THREADS          STD_ON
    #define CSI2_STATISTIC_DATA_USAGE               STD_ON
    #define CSI2_AUTO_DC_COMPENSATION               STD_ON
    #define CSI2_AUXILIARY_DATA_USAGE               STD_ON
    #define CSI2_METADATA_DATA_USAGE                STD_ON
    #define CSI2_RX_START_STOP_USAGE                STD_ON
    #define CSI2_POWER_ON_OFF_USAGE                 STD_ON
    #define CSI2_SECONDARY_FUNCTIONS_USAGE          STD_ON
    #define CSI2_FRAMES_COUNTER_USED                STD_ON
    #define CSI2_GPIO_USED                          STD_ON
    #define CSI2_SDMA_USED                          STD_ON

    #define CSI2_US_DELAY   1000u


/** @} */

#if !defined(CSI2_HALT_ON_ERROR)
    #if (CSI2_DEV_HALT_ON_ERROR == STD_ON)
    /* Infinite loop if error detected and the option is to detect development errors */
        #define CSI2_HALT_ON_ERROR          while(gsCsi2LoopExit != TRUE)   \
                                            {                               \
                                                ;   /* empty loop */        \
                                            }
    #else
        #define CSI2_HALT_ON_ERROR
    #endif
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

#endif /* CSI2_CFG_H */
