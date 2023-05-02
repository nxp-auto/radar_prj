/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CTE_TYPES_H
#define CTE_TYPES_H

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
    #include "rsdk_cte_driver_api.h"
    #ifndef linux
        #include "rsdk_glue_irq_register_api.h"
    #endif
#include "Cte_Cfg.h"





    #include "S32R45_CTE.h"





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
#define CTE_WRONG_TABLE_TIME_LENGTH    0xffffffffu     /**< The value not accepted for table time limit, as 
                                                         * parameter for Cte_TimeTableDefType::tableTimeExecLimit;
                                                         * this value is internally used by the driver, and if 
                                                         * application use it as parameter, unknown results can
                                                         * appear.                                              */

#define CTE_DEFAULT_INPUT_CLOCK         80000000u      /**< The default input clock for CTE                     */

#if defined(TRACE_ENABLE)
        #define CTE_TRACE(a,b,c) RsdkTraceLogEvent(a,b,c)
#else
    #define CTE_TRACE(a,b,c)
#endif


/*==================================================================================================
*                                              ENUMS
==================================================================================================*/
 /**
 * @brief   CTE input mode
 * @details Defines the two possible CTE working modes : using internal CSI2 signals (Master mode) 
 *          or external PAD signals (Slave mode)
 *
 */
typedef enum {
    CTE_MASTER = 0u,           /**< CTE in Master mode. This means the timing is decided only by software
                                     control, table execution start or reset.                                       */
    CTE_SLAVE_EXTERNAL,        /**< CTE in Slave mode, table execution triggered from PADs
                                     (check IOMUX excel file for RCS_I and/or RFS_I)                                */
    CTE_SLAVE_CSI2,            /**< CTE in Slave mode, table execution triggered by MIPI-CSI2 RFS/RCS signals. */
    CTE_MODE_MAX,              /**< CTE input mode limit, not to be used                                       */
} Cte_ModeType;
 
 /**
 * @brief   MIPI-CSI2 units enumeration as CTE input source
 * @details Used only in CTE_INPUT_CSI2/Master mode.
 *          As the driver is intended to be used for S32R294 platform too, a platform differentiation must be done,
 *          as S32R294 has two CSI2 units and S32R45 has four. 
 *
 */
typedef enum {
    CTE_CSI2_UNIT_0 = 0u,      /**< MIPI-CSI2 unit 0                                   */
    CTE_CSI2_UNIT_1,           /**< MIPI-CSI2 unit 1                                   */

    CTE_CSI2_UNIT_2,           /**< MIPI-CSI2 unit 2,                                  */
    CTE_CSI2_UNIT_3,           /**< MIPI-CSI2 unit 3                                   */

    CTE_CSI2_UNIT_MAX,         /**< MIPI-CSI2 unit limit (not used)                    */
} Cte_Csi2UnitType;

 /**
 * @brief   MIPI-CSI2 virtual channels enumeration as CTE input source
 * @details Used only in CTE_INPUT_CSI2/Master mode.
 *
 */
typedef enum {
    CTE_CSI2_VC_0 = 0u,        /**< MIPI-CSI2 virtual channel 0                        */
    CTE_CSI2_VC_1,             /**< MIPI-CSI2 virtual channel 1                        */
    CTE_CSI2_VC_2,             /**< MIPI-CSI2 virtual channel 2                        */
    CTE_CSI2_VC_3,             /**< MIPI-CSI2 virtual channel 3                        */
    CTE_CSI2_VC_MAX,           /**< MIPI-CSI2 virtual channel limit (not used)         */
} Cte_Csi2VcType;

 /**
 * @brief   CTE available output signals
 * @details All possible CTE output signals. For normal usage only a subset can be used.
  *         Each output must be used separately, no combinations are permitted.
 *
 */
typedef enum {
    CTE_OUTPUT_SPT_0 = 0u,     /**< CTE SPT event 0 is the output                      */
    CTE_OUTPUT_SPT_1,          /**< CTE SPT event 1 is the output                      */
    CTE_OUTPUT_SPT_2,          /**< CTE SPT event 2 is the output                      */
    CTE_OUTPUT_SPT_3,          /**< CTE SPT event 3 is the output                      */
    CTE_OUTPUT_CTEP_0,         /**< CTE PAD_0 is the output                            */
    CTE_OUTPUT_CTEP_1,         /**< CTE PAD_1 is the output                            */
    CTE_OUTPUT_CTEP_2,         /**< CTE PAD_2 is the output                            */
    CTE_OUTPUT_CTEP_3,         /**< CTE PAD_3 is the output                            */
    CTE_OUTPUT_CTEP_4,         /**< CTE PAD_4 is the output                            */
    CTE_OUTPUT_CTEP_5,         /**< CTE PAD_5 is the output                            */
    CTE_OUTPUT_CTEP_6,         /**< CTE PAD_6 is the output                            */
    CTE_OUTPUT_CTEP_7,         /**< CTE PAD_7 is the output                            */
    CTE_OUTPUT_SPT_RCS,        /**< CTE SPT RCS event is the output                    */
    CTE_OUTPUT_SPT_RFS,        /**< CTE SPT RFS event is the output                    */
    CTE_OUTPUT_FLEX_0,         /**< CTE Flextimer event 0 is the output                */
    CTE_OUTPUT_FLEX_1,         /**< CTE Flextimer event 1 is the output                */
    CTE_OUTPUT_MAX,            /**< CTE output definitions limit (not used)            */
} Cte_OutputType;

 /**
 * @brief   CTE signal output types
 * @details The possible different usage types for the CTE outputs.
 *          CTE_OUTPUT_SPT_x  events can be only:
 *              - LOGIC and only TO_LOW or TO_HIGH states to be used.
 *              - TOGGLE, which will work as PULSE; only LOW and HIGH states to be used too
 *          CTE_OUTPUT_FLEX_x events can be only LOGIC and only TO_LOW or TO_HIGH states to be used.
 *
 */
typedef enum {
    CTE_OUT_HIZ = 0u,          /**< Output type is only High-Z (if possible)           */
    CTE_OUT_TOGGLE,            /**< Output type is toggle, low/hi/toggle               */
    CTE_OUT_CLOCK,             /**< Output type is clock, low/hi/toggle with[out] sync */
    CTE_OUT_LOGIC,             /**< Output type is logic, low/high/Hi-Z levels         */
    CTE_OUT_MAX,               /**< Output type limit (not used)                       */
} Cte_OutputTypeType;

 /**
 * @brief   CTE signal output state for TOGGLE type
 * @details The only possible states for TOGGLE type.
 *
 */
typedef enum {
    CTE_TOGGLE_SET_TO_LOW = 0u,/**< Set the TOGGLE output to low                       */
    CTE_TOGGLE_SET_TO_HIGH,    /**< Set the TOGGLE output to high                      */
    CTE_TOGGLE_FLIP,           /**< Flip the TOGGLE output (high->low; low->high)      */
    CTE_TOGGLE_DONT_CARE,      /**< The TOGGLE output remains the same                 */
} Cte_ToggleOutStatesType;

 /**
 * @brief   CTE signal output state for LOGIC type
 * @details The only possible states for LOGIC type.
 *
 */
typedef enum {
    CTE_LOGIC_SET_TO_LOW = 0u, /**< Set the LOGIC output to low                        */
    CTE_LOGIC_SET_TO_HIGH,     /**< Set the LOGIC output to high                       */
    CTE_LOGIC_SET_TO_HIGH_Z,   /**< Set the LOGIC output to High-Z                     */
    CTE_LOGIC_UNCHANGED,   	   /**< Set the LOGIC output to High-Z                     */
} Cte_LogicOutStatesType;

 /**
 * @brief   CTE signal output state for CLOCK type
 * @details The only possible states for CLOCK type.
 *
 */
typedef enum {
    CTE_CLOCK_SET_TO_LOW = 0u, /**< Keep the CLOCK output to low                       */
    CTE_CLOCK_ACTIVE_SYNC,     /**< CLOCK output active and synchronized on rising edge*/
    CTE_CLOCK_ACTIVE,          /**< CLOCK output                                       */
    CTE_CLOCK_SET_TO_HIGH,     /**< Keep the CLOCK output at high level                */
} Cte_ClockOutStatesType;

 /**
 * @brief   CTE table usage
 * @details This is the user intended usage for the CTE driver/timing table.
 *          The driver implement this as necessary.
 *
 */
typedef enum {
    CTE_ONE_TABLE = 0u,        /**< Only one table, the same for all chirps                */
    CTE_TWO_TABLES,            /**< Two tables, first for even chirps and second for odds  */
} Cte_TableUsageType;

 /**
 * @brief   The enumeration of the available CTE interrupts
 * @details These are all available interrupts which the CTE can generate.
 *          Only one interrupt callback is used by the driver, but any combination of these signals can be used.
 *
 */
typedef enum {
    CTE_IRQ_TT0_START      = 0x01u,    /**< Start execution of TimeTable 0                 */
    CTE_IRQ_TT1_START      = 0x02u,    /**< Start execution of TimeTable 1                 */
    CTE_IRQ_TT0_END        = 0x04u,    /**< End of TimeTable 0 execution                   */
    CTE_IRQ_TT1_END        = 0x08u,    /**< End of TimeTable 1 execution                   */
    CTE_IRQ_RCS            = 0x40u,    /**< Raising edge of RCS signal                     */
    CTE_IRQ_RFS            = 0x80u,    /**< Raising edge of RFS signal                     */
    CTE_IRQ_TABLE_EXEC_END = 0x200u,   /**< Table execution was finished                   */
} Cte_IrqDefinitionType;



/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
 * @brief   Structure for CTE input source.
 * @details CTE_clock refers to CTE input clock, before any internal divisor, i.e. MC_CGM_0(PER_CLK) for S32R45 
 *
 */
typedef struct {
    Cte_ModeType            workingMode;           /**< The source of input trigger signal                     */
    union {
        Cte_Csi2UnitType    cteCsi2Unit;           /**< MIPI-CSI2 unit to be used for input trigger. Value used
                                                   only for CTE_SLAVE_CSI2 mode.                      */
        uint8               cteInternalRfsDelay;   /**< Internal RFS delay, from detecting to usage,
                                                    in CTE_clock ticks, values in [0...15] interval only.
                                                    Used/checked only for CTE_SLAVE_EXTERNAL mode.     */
    }cteWorkingParam0;
    union {
        Cte_Csi2VcType      cteCsi2Vc;             /**< MIPI-CSI2 Virtual Channel to be used for input trigger.
                                                    Value used only for CTE_SLAVE_CSI2 mode.           */
        uint8               cteInternalRcsDelay;   /**< Internal RCS delay, from detecting to usage, in CTE_clock ticks,
                                                    values in [0...15] interval only.
                                                    Used/checked only for CTE_SLAVE_EXTERNAL source.   */
    }cteWorkingParam1;
} Cte_ModeDefinitionType;

/**
 * @brief   Structure for CTE output definition.
 * @details Each used output must be defined into one structure like this.
 *
 */
typedef struct {
    Cte_OutputType      outputSignal;           /**< The referred output signal                         */
    Cte_OutputTypeType  signalType;             /**< The type selected for the specified signal         */
    uint32              clockPeriod;            /**< The clock period, in ns, used only for CLOCK type  */
} Cte_SingleOutputDefType;

/**
 * @brief   Structure for single CTE output event action.
 * @details The new state will be interpreted according to the defined type of the output signal.
 *          This structure define a single output change.
 *
 */
typedef struct {
    Cte_OutputType     outputSignal;       /**< The specific output signal which must change the state             */
    union {
        Cte_ToggleOutStatesType    newToggleState;     /**< New state for TOGGLE output    */
        Cte_LogicOutStatesType     newLogicState;      /**< New state for LOGIC output     */
        Cte_ClockOutStatesType     newClockState;      /**< New state for CLOCK output     */
    };
} Cte_ActionType;

/**
 * @brief   Structure for CTE time event structure.
 * @details For each changing time moment at least one event must be added.
 *          For the same time many timing events can be specified, according to the desired signal changes, but not 
 *          necessary to specify all active signals.
 *          If the output is defined/active but not changed at a time, the previous state will be maintained.
 *
 */
typedef struct {
    uint32          absTime;            /**< The absolute time of the event, in ns, relative to trigger command */
    Cte_ActionType  *pEventActions;     /**< Pointer to the actions to be done at the specified time. The actions 
                                            list must be :  
                                            - not empty
                                            - not more than one action for one output
                                            - no other outputs than the ones defined or next point will be applied
                                            - the end of the list is signaled by an output not defined in 
                                              Cte_OutputType as normal output (please use CTE_OUTPUT_MAX).*/
} Cte_TimingEventType;

/**
 * @brief   Structure for CTE time table definition.
 * @details This is a structure for only one table. If two tables are used, two structures must be defined.
 *
 */
typedef struct {
    uint8               tableLength;            /**< The number of defined events to be used/added. It must be up to 32 
                                                    for two tables usage and up to 64 if single table used. A zero 
                                                    length means the table is not well defined and an error will be 
                                                    returned.                                               */
    uint32              tableTimeExecLimit;     /**< The limited time for table processing, in ns. 0 means no 
                                                    table end on time limit.                                */
    Cte_TimingEventType *pEvents;               /**< Pointer to the events list, which must have the tableLength 
                                                    corrected defined events, or an error will be reported. 
                                                    A NULL value for pEventActions before the tableLength limit 
                                                    will be used as a pEvents limit and the TableLength will be 
                                                    truncated to the counted number, with no errors. This means is 
                                                    possible to use a big number for tableLength and define the table 
                                                    end using a NULL pointer for pEventActions.             */
} Cte_TimeTableDefType;

/**
 * @brief   Definition of callback function type to be called by the CTE interrupt handler.
 * @details This callback must be used for processing a CTE event interrupt.<br>
 *          Simple callback function definition example : void CallbackCte(Cte_IrqDefinitionType irqs) { ... }
 *
 * @param[in]    cteIrqReport - a combination of Cte_IrqDefinitionType elements
 * @return       nothing
 *
 *
 * */
typedef void (*Cte_IsrCbType)(uint32 cteIrqReport);

/**
 * @brief   The structure for CTE setup.
 * @details All necessary parameters to initialize the CTE hardware.
 *
 */
typedef struct {
    Cte_ModeDefinitionType  cteMode;        /**< Specify the CTE usage, as Master (input from MIPI-CSI2 RCS/RFS) 
                                                  or Slave (input from external RCS/RFS)                            */
    uint32                  cteClockFrecq;  /**< The frequency of the CTE_clock signal, in Hz, at working time  */
    uint16                  repeatCount;    /**< The number of times the table execution will be repeated until
                                                    going to HALT state. 0 (zero) means "forever". If two alternative
                                                    tables used, each table execution will decrease the counter.    */
    Cte_SingleOutputDefType *pSignalDef0;   /**< Pointer to the output signals working mode definitions. Undefined
                                                  outputs will be set as HiZ.
                                                  All used SPT events used must be defined, using the same output type.
                                                  All used FLEX line used must be defined, using the same output type.
                                                  The table end is signaled using a not defined output 
                                                  (please use CTE_OUTPUT_MAX).
                                                  This table contains the definitions used for Table0 execution.    */
    Cte_SingleOutputDefType *pSignalDef1;   /**< Pointer to the output signals working mode definitions.
                                                  Similar to pSignalDef0, but this table contains the definitions
                                                  to be used for Table1 execution.        */
    Cte_TimeTableDefType    *pTimeTable0;   /**< Pointer for the first table to be used. Must be not NULL.      */
    Cte_TimeTableDefType    *pTimeTable1;   /**< Pointer for the second table to be used. If NULL, two alternative 
                                                  tables mode will be used.                                         */
    Cte_IrqDefinitionType   cteIrqEvents;   /**< The requested combination of interrupt sequence to be used     */
    Cte_IsrCbType           pCteCallback;   /**< The application callback to be used for the requested events   */
#ifndef linux
    int8                    irqExecCore;    /**< Processor core to execute the irq code. Usually the current core.*/
    uint8                   irqPriority;    /**< Priority for the interrupt request execution                   */
#endif
} Cte_SetupParamsType;


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

#endif /* CTE_TYPES_H */
