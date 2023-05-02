/*
* Copyright 2020-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RSDK_CTE_LINUX_DEF_H
#define RSDK_CTE_LINUX_DEF_H

#include "rsdk_cte_driver_api.h"
#include "Cte_Specific.h"

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#define RSDK_CTE_RPC_CHANNEL_NAME "RsdkCteRpc"  // RPC channel name

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
// calls identifiers for Linux RPC
typedef enum
{
    RSDK_CTE_LX_CTE_INIT = 0,           // CTE initialization
    RSDK_CTE_LX_CTE_STOP,               // CTE stop
    RSDK_CTE_LX_CTE_START,              // unit Rx start
    RSDK_CTE_LX_CTE_RESTART,            // unit power off
    RSDK_CTE_LX_CTE_RFS_SET,            // unit power on
    RSDK_CTE_LX_CTE_TABLE_UPDATE,       // get unit/interface status
    RSDK_CTE_LX_CTE_REG_EVT,            // register unit event
    RSDK_CTE_LX_CTE_EVTMGR_STOP,        // stop events management
    RSDK_CTE_LX_CTE_READ_CHECKSUM,      // read and return the LUT checksum
    RSDK_CTE_LX_CTE_CALL_MAX            // the total number of defined RPC calls
} rsdkCteRpcCalls_t;

// identifiers for Linux RPC irq events
typedef enum
{
    RSDK_CTE_LX_EVT_TT0_START = 0,      // TimeTable 0 start
    RSDK_CTE_LX_EVT_TT1_START,          // TimeTable 1 start
    RSDK_CTE_LX_EVT_TT0_STOP,           // TimeTable 0 stop
    RSDK_CTE_LX_EVT_TT1_STOP,           // TimeTable 1 stop
    RSDK_CTE_LX_EVT_RFS,                // RFS event
    RSDK_CTE_LX_EVT_RCS,                // RCS event
    RSDK_CTE_LX_EVT_TT_END,             // TimeTable ended execution
    RSDK_CTE_LX_EVT_EVTMGR_STOP,        // Call for user-space thread stop
    RSDK_CTE_LX_EVT_MAX                 // the total number of defined events
} rsdkCteRpcEvents_t;

/**
 * @brief   The structure for timing table  initialization, internally used.
 * @details To be used in kernel  space.
 *
 */
typedef struct {
    uint32_t        evTime;                         // the event time
    uint8_t         actionsNumber;                  // the table len
    rsdkCteAction_t actions[RSDK_CTE_OUTPUT_MAX];   // effective actions to be taken
} rsdkCteLinuxTtEvents_t;

/**
 * @brief   The structure for CTE initialization, internally used.
 * @details All necessary parameters to initialize the CTE hardware, to be used in kernel  space.
 *
 */
typedef struct {
    rsdkCteModeDefinition_t     cteMode;        /**< Specify the CTE usage, as Master (input from MIPI-CSI2 RCS/RFS) 
                                                  or Slave (input from external RCS/RFS)                            */
    uint32_t                    cteClockFrecq;  /**< The frequency of the CTE_clock signal, in Hz, at working time  */
    uint16_t                    repeatCount;    /**< The number of times the table execution will be repeated until
                                                    going to HALT state. 0 (zero) means "forever". If two alternative
                                                    tables used, each table execution will decrease the counter.    */
    uint8_t                     lenSigDef0, lenSigDef1; /**< The lengths for table output type definitions          */
    uint8_t                     tableLen0, tableLen1;   /**< The number of defined events to be used/added. 
                                                    It must be up to 32 for two tables usage and up to 64 if single 
                                                    table used. A zero length means the table is not well defined and 
                                                    an error will be returned.                                       */
    uint32_t                    timeLimT0, timeLimT1;   /**< The timing limits for each table                       */
    rsdkCteSingleOutputDef_t    signalDef0[RSDK_CTE_OUTPUT_MAX];   
                                                /**< The output signals working mode definitions. Undefined
                                                  outputs will be set as HiZ.
                                                  All used SPT events used must be defined, using the same output type.
                                                  All used FLEX line used must be defined, using the same output type.
                                                  The table end is signaled using a not defined output 
                                                  (please use RSDK_CTE_OUTPUT_MAX).
                                                  This table contains the definitions used for Table0 execution.    */
    rsdkCteSingleOutputDef_t    signalDef1[RSDK_CTE_OUTPUT_MAX];   
                                                /**< The output signals working mode definitions.
                                                  Similar to pSignalDef0, but this table contains the definitions
                                                  to be used for Table1 execution.        */
    rsdkCteLinuxTtEvents_t      timeTable0[CTE_MAX_SMALL_TIME_TABLE_LEN];   /**< The first table to be used.        */
    rsdkCteLinuxTtEvents_t      timeTable1[CTE_MAX_SMALL_TIME_TABLE_LEN];   /**< The second table to be used.       */
    rsdkCteIrqDefinition_t      cteIrqEvents;   /**< The requested combination of interrupt sequence to be used     */
    rsdkCteIsrCb_t              pCteCallback;   /**< The application callback to be used for the requested events   */
} rsdkCteLinuxTransfer_t;


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
rsdkStatus_t CtePlatformModuleInitLinuxUs(const rsdkCteInitParams_t *pCteInitParam, uint64_t *pLutChecksum);
rsdkStatus_t CtePlatformModuleStartLinuxUs(void);
rsdkStatus_t CtePlatformModuleStopLinuxUs(void);
rsdkStatus_t CtePlatformModuleRestartLinuxUs(void);
rsdkStatus_t CtePlatformModuleRfsGenerateLinuxUs(void);
rsdkStatus_t CtePlatformModuleUpdateTablesLinuxUs(rsdkCteTimeTableDef_t *pTable0, rsdkCteTimeTableDef_t *pTable1, 
                    uint64_t *pUInt64);
uint64_t     CtePlatformModuleLinuxUsGetLutChecksum(void);


#ifdef __cplusplus
}
#endif

#endif  //RSDK_CTE_LINUX_DEF_H
