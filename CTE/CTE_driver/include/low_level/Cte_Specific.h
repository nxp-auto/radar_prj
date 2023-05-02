/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CTE_SPECIFIC_H
#define CTE_SPECIFIC_H

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#include "Cte_Types.h"
    #include "rsdk_cte_driver_api.h"
    #ifdef __KERNEL__
    #include <linux/types.h>
    #else
    #include "typedefs.h"
    #endif




    #include "S32R45_CTE.h"








#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
 *                                          CONSTANTS
 ==================================================================================================*/
#define CTE_MAX_LARGE_TIME_TABLE_LEN    64u     /* maximum length for single time table usage                       */
#define CTE_MAX_SMALL_TIME_TABLE_LEN    32u     /* maximum length for double time table usage (single table)        */





    #define CTE_IRQ_NUMBER      263u                /* hardware interrupt number                                        */









/*==================================================================================================
 *                                      DEFINES AND MACROS
 ==================================================================================================*/
#define CTE_CLOCK_DIVIDER_LIMIT     0x40u       /* the clock divider limit (the value must be less than this value) */
#define CTE_TOO_BIG_TIME_DELAY      0xffffffffu /* a time delay which exceed the maximum admisible                  */
#define CTE_MAX_TIME_COUNTER        0x10000u    /* the internal timecounter limit for CTE events                    */
#define CTE_1G_FREQUENCY            1000000000u /* 1GHz frequency                                                   */

#define CTE_SPT0_SIG_MASK           0x10000000LU                /* mask for SPT0 signal                             */
#define CTE_FLEX_SIG_MASK           ((uint64_t)0x1LU << 33u)    /* mask for first FLEX signal                       */
#define CTE_OUTPUT_MASK_SHIFT_BASE  32u                         /* the minimum shift for the output signal mask     */

#define CTE_INTERNAL_CLOCKS         4u                          /* CTE has 4 internal clock dividers                */
#define CTE_MAX_REQ_CLK_DIVIDER     192u                        /* Max clock divider                                */
#define CTE_MAX_CLK_DIVIDERS        8u                          // the number of possible dividers for clocks       */

/* macro definition for error reporting        */
    #define CTE_REPORT_ERROR(a,c,d)  rez = (a)


/*==================================================================================================
 *                                             ENUMS
 ==================================================================================================*/
/* enum for toggle signal manipulations         */
typedef enum {
    CTE_TOGGLE_MASK_TO_LOW = 0u,
    CTE_TOGGLE_MASK_TO_HIGH,
    CTE_TOGGLE_MASK_TOGGLE,
    CTE_TOGGLE_MASK_UNCHANGED,
}Cte_ToggleMaskType;

/* enum for clock signal manipulations          */
typedef enum {
    CTE_CLOCK_MASK_TO_LOW = 0u,
    CTE_CLOCK_MASK_SYNC_RISING,
    CTE_CLOCK_MASK_RUNNING,
    CTE_CLOCK_MASK_TO_HIGH,
}Cte_ClockMaskType;

/* enum for logic signal manipulations          */
typedef enum {
    CTE_LOGIC_MASK_TO_LOW = 0u,
    CTE_LOGIC_MASK_TO_HIGH,
    CTE_LOGIC_MASK_TO_HIZ,
    CTE_LOGIC_MASK_UNCHANGED,
}Cte_LogicMaskType;

/* enum for the driver states                   */
typedef enum {
    CTE_DRIVER_STATE_NOT_INIT = 0u,
    CTE_DRIVER_STATE_INITIALIZED,
    CTE_DRIVER_STATE_RUNNING,
}Cte_DriverStatusType;


/*==================================================================================================
 *                                STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
/* Structure to keep the necessary data for low-level driver        */
typedef struct {
    uint8                   cteDriverStatus;        /* the current status of the driver                             */
    uint8                   cteMainClockDivider;    /* the main divider, for main CTE clock divider                 */
    uint8                   cteUsedClockDividers;   /* the number of used clocks                                    */
    uint32                  cteWorkingFreq;         /* the CTE working frequency, in Hz                             */
    uint32                  cteReqEvents;           /* the CTE events requested by application to be signaled       */
    Cte_IsrCbType           pCteCallback;           /* the application callback to be used for the requested events */
    uint32                  cteClocksPeriods[CTE_INTERNAL_CLOCKS];  /* resulting clock period                       */
    Cte_SingleOutputDefType pSignalDef0[CTE_OUTPUT_MAX + 1u];       /* copy of the existing signals definitions     */
    Cte_SingleOutputDefType pSignalDef1[CTE_OUTPUT_MAX + 1u];
} Cte_DriverStateType;

/*==================================================================================================
 *                                GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/
extern Cte_DriverStateType gsDriverData;            /* the driver necessary data                */
extern volatile CTE_Type *gspCTE;                   /* the pointer to the CTE registry          */


/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
 ==================================================================================================*/


#ifdef __cplusplus
}
#endif



#endif /* CTE_SPECIFIC_H    */
