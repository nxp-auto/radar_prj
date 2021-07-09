/*
* Copyright 2019-2021 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef RSDK_CTE_DRIVER_API_H
#define RSDK_CTE_DRIVER_API_H

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include "typedefs.h"
#endif
#include "rsdk_status.h"

#ifndef linux
#include "rsdk_glue_irq_register_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup cte_driver_api_const
* @{
*/

/*==================================================================================================
 *                                          CONSTANTS
 ==================================================================================================*/
/** @}*/
/** @addtogroup cte_driver_api_data_type
* @{
*/

/*==================================================================================================
 *                                      DEFINES AND MACROS
 ==================================================================================================*/
#define RSDK_CTE_WRONG_TABLE_TIME_LENGTH    0xffffffffu     /**< The value not accepted for table time limit, as 
                                                             * parameter for rsdkCteTimeTableDef_t::tableTimeExecLimit;
                                                             * this value is internally used by the driver, and if 
                                                             * application use it as parameter, unknown results can
                                                             * appear.                                              */

/*==================================================================================================
 *                                             ENUMS
 ==================================================================================================*/
 /**
 * @brief   CTE input mode
 * @details Defines the two possible CTE working modes : using internal CSI2 signals (Master mode) 
 *          or external PAD signals (Slave mode)
 *
 */
typedef enum {
    RSDK_CTE_MASTER = 0u,           /**< CTE in Master mode. This means the timing is decided only by software
                                     control, table execution start or reset.                                       */
    RSDK_CTE_SLAVE_EXTERNAL,        /**< CTE in Slave mode, table execution triggered from PADs
                                     (check IOMUX excel file for RCS_I and/or RFS_I)                                */
    RSDK_CTE_SLAVE_CSI2,            /**< CTE in Slave mode, table execution triggered by MIPI-CSI2 RFS/RCS signals. */
    RSDK_CTE_MODE_MAX,              /**< CTE input mode limit, not to be used                                       */
} rsdkCteMode_t;
 
 /**
 * @brief   MIPI-CSI2 units enumeration as CTE input source
 * @details Used only in RSDK_CTE_INPUT_CSI2/Master mode.
 *          As the driver is intended to be used for S32R294 platform too, a platform differentiation must be done,
 *          as S32R294 has two CSI2 units and S32R45 has four. 
 *
 */
typedef enum {
    RSDK_CTE_CSI2_UNIT_0 = 0u,      /**< MIPI-CSI2 unit 0                                   */
    RSDK_CTE_CSI2_UNIT_1,           /**< MIPI-CSI2 unit 1                                   */
#ifdef S32R45
    RSDK_CTE_CSI2_UNIT_2,           /**< MIPI-CSI2 unit 2,                                  */
    RSDK_CTE_CSI2_UNIT_3,           /**< MIPI-CSI2 unit 3                                   */
#endif
    RSDK_CTE_CSI2_UNIT_MAX,         /**< MIPI-CSI2 unit limit (not used)                    */
} rsdkCteCsi2Unit_t;

 /**
 * @brief   MIPI-CSI2 virtual channels enumeration as CTE input source
 * @details Used only in RSDK_CTE_INPUT_CSI2/Master mode.
 *
 */
typedef enum {
    RSDK_CTE_CSI2_VC_0 = 0u,        /**< MIPI-CSI2 virtual channel 0                        */
    RSDK_CTE_CSI2_VC_1,             /**< MIPI-CSI2 virtual channel 1                        */
    RSDK_CTE_CSI2_VC_2,             /**< MIPI-CSI2 virtual channel 2                        */
    RSDK_CTE_CSI2_VC_3,             /**< MIPI-CSI2 virtual channel 3                        */
    RSDK_CTE_CSI2_VC_MAX,           /**< MIPI-CSI2 virtual channel limit (not used)         */
} rsdkCteCsi2Vc_t;

 /**
 * @brief   CTE available output signals
 * @details All possible CTE output signals. For normal usage only a subset can be used.
  *         Each output must be used separately, no combinations are permitted.
 *
 */
typedef enum {
    RSDK_CTE_OUTPUT_SPT_0 = 0u,     /**< CTE SPT event 0 is the output                      */
    RSDK_CTE_OUTPUT_SPT_1,          /**< CTE SPT event 1 is the output                      */
    RSDK_CTE_OUTPUT_SPT_2,          /**< CTE SPT event 2 is the output                      */
    RSDK_CTE_OUTPUT_SPT_3,          /**< CTE SPT event 3 is the output                      */
    RSDK_CTE_OUTPUT_CTEP_0,         /**< CTE PAD_0 is the output                            */
    RSDK_CTE_OUTPUT_CTEP_1,         /**< CTE PAD_1 is the output                            */
    RSDK_CTE_OUTPUT_CTEP_2,         /**< CTE PAD_2 is the output                            */
    RSDK_CTE_OUTPUT_CTEP_3,         /**< CTE PAD_3 is the output                            */
    RSDK_CTE_OUTPUT_CTEP_4,         /**< CTE PAD_4 is the output                            */
    RSDK_CTE_OUTPUT_CTEP_5,         /**< CTE PAD_5 is the output                            */
    RSDK_CTE_OUTPUT_CTEP_6,         /**< CTE PAD_6 is the output                            */
    RSDK_CTE_OUTPUT_CTEP_7,         /**< CTE PAD_7 is the output                            */
    RSDK_CTE_OUTPUT_SPT_RCS,        /**< CTE SPT RCS event is the output                    */
    RSDK_CTE_OUTPUT_SPT_RFS,        /**< CTE SPT RFS event is the output                    */
    RSDK_CTE_OUTPUT_FLEX_0,         /**< CTE Flextimer event 0 is the output                */
    RSDK_CTE_OUTPUT_FLEX_1,         /**< CTE Flextimer event 1 is the output                */
    RSDK_CTE_OUTPUT_MAX,            /**< CTE output definitions limit (not used)            */
} rsdkCteOutput_t;

 /**
 * @brief   CTE signal output types
 * @details The possible different usage types for the CTE outputs.
 *          RSDK_CTE_OUTPUT_SPT_x  events can be only:
 *              - LOGIC and only TO_LOW or TO_HIGH states to be used.
 *              - TOGGLE, which will work as PULSE; only LOW and HIGH states to be used too
 *          RSDK_CTE_OUTPUT_FLEX_x events can be only LOGIC and only TO_LOW or TO_HIGH states to be used.
 *
 */
typedef enum {
    RSDK_CTE_OUT_HIZ = 0u,          /**< Output type is only High-Z (if possible)           */
    RSDK_CTE_OUT_TOGGLE,            /**< Output type is toggle, low/hi/toggle               */
    RSDK_CTE_OUT_CLOCK,             /**< Output type is clock, low/hi/toggle with[out] sync */
    RSDK_CTE_OUT_LOGIC,             /**< Output type is logic, low/high/Hi-Z levels         */
    RSDK_CTE_OUT_MAX,               /**< Output type limit (not used)                       */
} rsdkCteOutputType_t;

 /**
 * @brief   CTE signal output state for TOGGLE type
 * @details The only possible states for TOGGLE type.
 *
 */
typedef enum {
    RSDK_CTE_TOGGLE_SET_TO_LOW = 0u,/**< Set the TOGGLE output to low                       */
    RSDK_CTE_TOGGLE_SET_TO_HIGH,    /**< Set the TOGGLE output to high                      */
    RSDK_CTE_TOGGLE_FLIP,           /**< Flip the TOGGLE output (high->low; low->high)      */
    RSDK_CTE_TOGGLE_DONT_CARE,      /**< The TOGGLE output remains the same                 */
} rsdkCteToggleOutStates_t;

 /**
 * @brief   CTE signal output state for LOGIC type
 * @details The only possible states for LOGIC type.
 *
 */
typedef enum {
    RSDK_CTE_LOGIC_SET_TO_LOW = 0u, /**< Set the LOGIC output to low                        */
    RSDK_CTE_LOGIC_SET_TO_HIGH,     /**< Set the LOGIC output to high                       */
    RSDK_CTE_LOGIC_SET_TO_HIGH_Z,   /**< Set the LOGIC output to High-Z                     */
    RSDK_CTE_LOGIC_UNCHANGED,   	/**< Set the LOGIC output to High-Z                     */
} rsdkCteLogicOutStates_t;

 /**
 * @brief   CTE signal output state for CLOCK type
 * @details The only possible states for CLOCK type.
 *
 */
typedef enum {
    RSDK_CTE_CLOCK_SET_TO_LOW = 0u, /**< Keep the CLOCK output to low                       */
    RSDK_CTE_CLOCK_ACTIVE_SYNC,     /**< CLOCK output active and synchronized on rising edge*/
    RSDK_CTE_CLOCK_ACTIVE,          /**< CLOCK output                                       */
    RSDK_CTE_CLOCK_SET_TO_HIGH,     /**< Keep the CLOCK output at high level                */
} rsdkCteClockOutStates_t;

 /**
 * @brief   CTE table usage
 * @details This is the user intended usage for the CTE driver/timing table.
 *          The driver implement this as necessary.
 *
 */
typedef enum {
    RSDK_CTE_ONE_TABLE = 0u,        /**< Only one table, the same for all chirps                */
    RSDK_CTE_TWO_TABLES,            /**< Two tables, first for even chirps and second for odds  */
} rsdkCteTableUsage_t;

 /**
 * @brief   The enumeration of the available CTE interrupts
 * @details These are all available interrupts which the CTE can generate.
 *          Only one interrupt callback is used by the driver, but any combination of these signals can be used.
 *
 */
typedef enum {
    RSDK_CTE_IRQ_TT0_START      = 0x01u,    /**< Start execution of TimeTable 0                 */
    RSDK_CTE_IRQ_TT1_START      = 0x02u,    /**< Start execution of TimeTable 1                 */
    RSDK_CTE_IRQ_TT0_END        = 0x04u,    /**< End of TimeTable 0 execution                   */
    RSDK_CTE_IRQ_TT1_END        = 0x08u,    /**< End of TimeTable 1 execution                   */
    RSDK_CTE_IRQ_RCS            = 0x40u,    /**< Raising edge of RCS signal                     */
    RSDK_CTE_IRQ_RFS            = 0x80u,    /**< Raising edge of RFS signal                     */
    RSDK_CTE_IRQ_TABLE_EXEC_END = 0x200u,   /**< Table execution was finished                   */
} rsdkCteIrqDefinition_t;

/*==================================================================================================
 *                                STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
/**
 * @brief   Structure for CTE input source.
 * @details CTE_clock refers to CTE input clock, before any internal divisor, i.e. MC_CGM_0(PER_CLK) for S32R45 
 *
 */
typedef struct {
    rsdkCteMode_t       workingMode;            /**< The source of input trigger signal                     */
    union {
        rsdkCteCsi2Unit_t   cteCsi2Unit;        /**< MIPI-CSI2 unit to be used for input trigger. Value used
                                                    only for RSDK_CTE_SLAVE_CSI2 mode.                      */
        uint8_t             cteInternalRfsDelay;/**< Internal RFS delay, from detecting to usage,
                                                    in CTE_clock ticks, values in [0...15] interval only.
                                                    Used/checked only for RSDK_CTE_SLAVE_EXTERNAL mode.     */
    }cteWorkingParam0;
    union {
        rsdkCteCsi2Vc_t     cteCsi2Vc;          /**< MIPI-CSI2 Virtual Channel to be used for input trigger.
                                                    Value used only for RSDK_CTE_SLAVE_CSI2 mode.           */
        uint8_t             cteInternalRcsDelay;/**< Internal RCS delay, from detecting to usage, in CTE_clock ticks,
                                                    values in [0...15] interval only.
                                                    Used/checked only for RSDK_CTE_SLAVE_EXTERNAL source.   */
    }cteWorkingParam1;
} rsdkCteModeDefinition_t;

/**
 * @brief   Structure for CTE output definition.
 * @details Each used output must be defined into one structure like this.
 *
 */
typedef struct {
    rsdkCteOutput_t     outputSignal;           /**< The referred output signal                         */
    rsdkCteOutputType_t signalType;             /**< The type selected for the specified signal         */
    uint32_t            clockPeriod;            /**< The clock period, in ns, used only for CLOCK type  */
} rsdkCteSingleOutputDef_t;

/**
 * @brief   Structure for single CTE output event action.
 * @details The new state will be interpreted according to the defined type of the output signal.
 *          This structure define a single output change.
 *
 */
typedef struct {
    rsdkCteOutput_t     outputSignal;       /**< The specific output signal which must change the state             */
    union {
        rsdkCteToggleOutStates_t    newToggleState;     /**< New state for TOGGLE output    */
        rsdkCteLogicOutStates_t     newLogicState;      /**< New state for LOGIC output     */
        rsdkCteClockOutStates_t     newClockState;      /**< New state for CLOCK output     */
    };
} rsdkCteAction_t;

/**
 * @brief   Structure for CTE time event structure.
 * @details For each changing time moment at least one event must be added.
 *          For the same time many timing events can be specified, according to the desired signal changes, but not 
 *          necessary to specify all active signals.
 *          If the output is defined/active but not changed at a time, the previous state will be maintained.
 *
 */
typedef struct {
    uint32_t            absTime;            /**< The absolute time of the event, in ns, relative to trigger command */
    rsdkCteAction_t     *pEventActions;     /**< Pointer to the actions to be done at the specified time. The actions 
                                                list must be :  
                                                - not empty
                                                - not more than one action for one output
                                                - no other outputs than the ones defined or next point will be applied
                                                - the end of the list is signaled by an output not defined in 
                                                  rsdkCteOutput_t as normal output (please use RSDK_CTE_OUTPUT_MAX).*/
} rsdkCteTimingEvent_t;

/**
 * @brief   Structure for CTE time table definition.
 * @details This is a structure for only one table. If two tables are used, two structures must be defined.
 *
 */
typedef struct {
    uint8_t                 tableLength;        /**< The number of defined events to be used/added. It must be up to 32 
                                                    for two tables usage and up to 64 if single table used. A zero 
                                                    length means the table is not well defined and an error will be 
                                                    returned.                                               */
    uint32_t                tableTimeExecLimit; /**< The limited time for table processing, in ns. 0 means no 
                                                    table end on time limit.                                */
    rsdkCteTimingEvent_t    *pEvents;           /**< Pointer to the events list, which must have the tableLength 
                                                    corrected defined events, or an error will be reported. 
                                                    A NULL value for pEventActions before the tableLength limit 
                                                    will be used as a pEvents limit and the TableLength will be 
                                                    truncated to the counted number, with no errors. This means is 
                                                    possible to use a big number for tableLength and define the table 
                                                    end using a NULL pointer for pEventActions.             */
} rsdkCteTimeTableDef_t;

/**
 * @brief   Definition of callback function type to be called by the CTE interrupt handler.
 * @details This callback must be used for processing a CTE event interrupt.<br>
 *          Simple callback function definition example : void CallbackCte(rsdkCteIrqDefinition_t irqs) { ... }
 *
 * @param[in]    cteIrqReport - a combination of rsdkCteIrqDefinition_t elements
 * @return       nothing
 *
 *
 * */
typedef void (*rsdkCteIsrCb_t)(uint32_t cteIrqReport);

/**
 * @brief   The structure for CTE initialization.
 * @details All necessary parameters to initialize the CTE hardware.
 *
 */
typedef struct {
    rsdkCteModeDefinition_t     cteMode;        /**< Specify the CTE usage, as Master (input from MIPI-CSI2 RCS/RFS) 
                                                  or Slave (input from external RCS/RFS)                            */
    uint32_t                    cteClockFrecq;  /**< The frequency of the CTE_clock signal, in Hz, at working time  */
    uint16_t                    repeatCount;    /**< The number of times the table execution will be repeated until
                                                    going to HALT state. 0 (zero) means "forever". If two alternative
                                                    tables used, each table execution will decrease the counter.    */
    rsdkCteSingleOutputDef_t    *pSignalDef0;   /**< Pointer to the output signals working mode definitions. Undefined
                                                  outputs will be set as HiZ.
                                                  All used SPT events used must be defined, using the same output type.
                                                  All used FLEX line used must be defined, using the same output type.
                                                  The table end is signaled using a not defined output 
                                                  (please use RSDK_CTE_OUTPUT_MAX).
                                                  This table contains the definitions used for Table0 execution.    */
    rsdkCteSingleOutputDef_t    *pSignalDef1;   /**< Pointer to the output signals working mode definitions.
                                                  Similar to pSignalDef0, but this table contains the definitions
                                                  to be used for Table1 execution.        */
    rsdkCteTimeTableDef_t       *pTimeTable0;   /**< Pointer for the first table to be used. Must be not NULL.      */
    rsdkCteTimeTableDef_t       *pTimeTable1;   /**< Pointer for the second table to be used. If NULL, two alternative 
                                                  tables mode will be used.                                         */
    rsdkCteIrqDefinition_t      cteIrqEvents;   /**< The requested combination of interrupt sequence to be used     */
    rsdkCteIsrCb_t              pCteCallback;   /**< The application callback to be used for the requested events   */
#ifndef linux
    rsdkCoreId_t                irqExecCore;    /**< Processor core to execute the irq code. Usually the current core.*/
    uint8_t                     irqPriority;    /**< Priority for the interrupt request execution                   */
#endif
} rsdkCteInitParams_t;

/*==================================================================================================
 *                                GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/

/** @}*/
/** @addtogroup cte_driver_api_func
* @{
*/

/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
 ==================================================================================================*/
/**
 * @brief   Initialization procedure for CTE driver
 * @details After initialization the CTE is not started, a specific RsdkCteStart call must be used for this.
 *          The operation can be done at any moment; if the CTE is working, it will be stopped.
 *
 * @param[in]   pCteInitParams  = pointer to the initialization structure
 * @param[in]   pLutChecksum    = pointer to a uint64_t value, which will receive the final LUT checksum;
 *                                  this value can be checked later using RsdkCteGetLutChecksum
 * @return      RSDK_SUCCESS    = initialization succeeded
 * @return      other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t RsdkCteInit(const rsdkCteInitParams_t *pCteInitParams, uint64_t *pLutChecksum);

/**
 * @brief   Start procedure for CTE
 * @details After this call the CTE will start to work, if successful.
 *          The procedure could be unsuccessful only if the driver was not initialized before. 
 *          The exit status can be different than RSDK_SUCCESS; RSDK_CTE_DRV_RUNNING is not an error, but only a 
 *          warning, signaling the CTE was already working and this was not interrupt.
 *          Start after a Stop will use the initialization done before.
 * 
 * @return  RSDK_SUCCESS    = start succeeded
 * @return  other values    = start failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t RsdkCteStart(void);

/**
 * @brief   Stop procedure for CTE
 * @details After this call the CTE will stop to work, if successful.
 *          The procedure could be unsuccessful only if the driver is not working at the call time.
 * 
 * @return  RSDK_SUCCESS    = stop succeeded
 * @return  other values    = stop failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t RsdkCteStop(void);

/**
 * @brief   Restart CTE.
 * @details This procedure is a single call for RsdkCteStop and a RsdkCteStart.
 *
 * @return  RSDK_SUCCESS    = restart succeeded
 * @return  other values    = restart failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t RsdkCteRestart(void);

/**
 * @brief   Generate a RFS software signal.
 * @details The procedure can be used only in Slave mode, to reset the time table execution.
 *          The real CTE execution must be triggered by a RCS signal.
 *
 * @return  RSDK_SUCCESS    = call succeeded
 * @return  other values    = call failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t RsdkCteRfsGenerate(void);

/**
 * @brief   Procedure to update only the existing timing tables.
 * @details The procedure can be used only after a previous successful CTE initialization.
 *          If the CTE is working, it will be stopped and restarted after table changed.
 *          If stopped, it will remains in the same state. It is recommendable to do like this.
 *
 * @param[in]   pTable_         = pointer to the new table(s); first pointer must not be NULL; 
                                if second is NULL, only one table used, else two tables used
 * @param[in]   pLutChecksum    = pointer to a uint64_t value, which will receive the final LUT checksum;
 *                                  this value can be checked later using RsdkCteGetLutChecksum
 * @return  RSDK_SUCCESS    = initialization succeeded
 * @return  other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t RsdkCteUpdateTables(rsdkCteTimeTableDef_t *pTable0, rsdkCteTimeTableDef_t *pTable1,
        uint64_t *pLutChecksum);

/**
 * @brief   Get the checksum of the timing LUT.
 * @details This procedure returns the current checksum reported by the hardware, only 40 bits.
 *          The value can be compared to the previous values.
 *
 * @return  LUT checksum
 *
 */
uint64_t RsdkCteGetLutChecksum(void);


/** @}*/

#ifdef __cplusplus
}
#endif



#endif // RSDK_CTE_DRIVER_API_H
