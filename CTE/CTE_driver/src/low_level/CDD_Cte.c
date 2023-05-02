/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**
*   @file
*   @implements Cte.c_Artifact
*
*   @addtogroup CTE
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*
* @page misra_violations MISRA-C:2012 violations
*
* @section Cte_c_REF_1
* Violates MISRA 2012 Advisory Directive 4.9,
*   #A function should be used in preference to a function-like macro where they are interchangeable
* Using a macro produce a smaller code; there is a clear distinction between functions and macros,
* which use only UPPERCASE
*
* @section Cte_c_REF_2
* Violates MISRA 2012 Required Directive 4.10,
*   #Precautions shall be taken in order to prevent the contents of a header file being included more than once.
* This violation is not fixed since the inclusion of <MA>_MemMap.h is as per AUTOSAR requirement [SWS_MemMap_00003].
*
* @section Cte_c_REF_3
* Violates MISRA C-2012 Advisory Rule 8.9,
*   #An object should be defined at block scope if its identifier only appears in a single function.
* To not mix code and data storage, for proper memory mapping,
* some of the static variables are not defined at block scope.
*
* @section Cte_c_REF_4
* Violates MISRA C-2012 Advisory Rule 11.4,
*   #A conversion should not be performed between a pointer to object and an integer type
* Some initialization need to be done for pointers.
*
* @section Cte_c_REF_5
* Violates MISRA C-2012 Required Rule 11.6,
*   #A cast shall not be performed between pointer to void and an arithmetic type
* In some contexts, is necessary to process pointers using arithmetic operations.
*
* @section Cte_c_REF_6
* Violates MISRA C-2012 Required Rule 18.1,
*   #A pointer resulting from arithmetic on a pointer operand shall address an element of the same array
*   as that pointer operand
* Some initialization need to be done for pointers.
*
* @section Cte_c_REF_7
* Violates MISRA C-2012 Required Rule 18.4,
*   #The +, -, += and -= operators should not be applied to an  expression of pointer type
* Necessary pointer operation, not possible to use normal pointer association.
*
* @section Cte_c_REF_8
* Violates MISRA 2012 Advisory Rule 20.1,
*   #Include directives should only be preceded by preprocessor directives or comments.
* <MA>_MemMap.h is included after each section define in order to set the current memory section as defined by AUTOSAR.
*
*/

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "CDD_Cte.h"
#include "Cte_Specific.h"
    #ifndef linux
        #include <string.h>
        #include <stdint.h>
    #else
        #include "rsdk_cte_driver_module.h"
    #endif
    #include "rsdk_status.h"






    #include "S32R45_CTE.h"
    #include "S32R45_SRC_1.h"








/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
typedef struct LUT{
    __IO uint32 LSB[CTE_LUT_LUT_LSB_COUNT];        /**< TT 0 (LSB)..TT 1 (LSB), array */
    __IO uint32 MSB[CTE_LUT_LUT_MSB_COUNT];        /**< TT 0 (MSB)..TT 1 (MSB), array */
} LutType;

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

#if (CTE_DEV_ERROR_DETECT == STD_ON)

    /* Loop exit signal.                                             */
    static volatile boolean gsCteLoopExit = FALSE;

#endif  /* #if (CTE_DEV_ERROR_DETECT == STD_ON)                    */


/*==================================================================================================
 *                                      GLOBAL CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      GLOBAL VARIABLES
 ==================================================================================================*/

volatile CTE_Type *gspCTE = NULL;                    /* the pointer to the CTE registry            */
Cte_DriverStateType gsDriverData = { 0u };              /* the driver necessary data                     */


/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/
#if (CTE_DEV_ERROR_DETECT == STD_ON)
static Std_ReturnType Cte_TtEventCheck(Cte_TimingEventType *pEvent);
static Std_ReturnType Cte_TimeTableCheck(Cte_TimeTableDefType *pTable);
static Std_ReturnType Cte_SignalDefsCheck(Cte_SingleOutputDefType *pSignals);
static Std_ReturnType Cte_InitParamsCheck(const Cte_SetupParamsType *pCteInitParams);
#endif

/*==================================================================================================
 *                                   LOCAL FUNCTION
 ==================================================================================================*/

#if (CTE_DEV_ERROR_DETECT == STD_ON)

/*==================================================================================================*/
/**
 * @brief   Procedure to check an event of the timing table.
 * @details Is checked the total number of defined actions, which must be less than the total signals number
 *
 * @param[in]   pEvent            = pointer to the event
 * @return      E_OK/RSDK_SUCCESS = initialization succeeded
 *              other values      = initialization failed, use the appropriate tools to detect the issue
 *
 */
static Std_ReturnType Cte_TtEventCheck(Cte_TimingEventType *pEvent)
{
    Std_ReturnType  rez = (Std_ReturnType)E_OK;
    Cte_ActionType  *action;
    uint32          n;

    if (pEvent->pEventActions == NULL)
    {
        /* no actions defined       */
        CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_ACTIONS, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
        CTE_HALT_ON_ERROR;
    }
    else
    {
        action = pEvent->pEventActions;
        n = 0;
        while ((uint8) action->outputSignal < (uint8)CTE_OUTPUT_MAX)
        {
            action++;                               /* pass to the next action definition               */
            n++;                                    /* simple action count                              */
        }
        if (n > (uint32) CTE_OUTPUT_MAX)
        {
            /* too many actions defined                         */
            CTE_REPORT_ERROR(RSDK_CTE_DRV_TOO_MANY_ACTIONS, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
            CTE_HALT_ON_ERROR;
        }
    }
    return rez;
}
/*=== Cte_TtEventCheck ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to check the timing table.
 * @details The check is only syntactic, not semantic.
 *
 * @param[in]   pTable            = pointer to the time table
 * @return      E_OK/RSDK_SUCCESS = initialization succeeded
 *              other values      = initialization failed, use the appropriate tools to detect the issue
 *
 */
static Std_ReturnType Cte_TimeTableCheck(Cte_TimeTableDefType *pTable)
{
    uint32          i;
    Std_ReturnType  rez = (Std_ReturnType)E_OK;

    /* check the table length, single table supposed; if double table used, the length will be checked later        */
    if (pTable->tableLength > CTE_MAX_LARGE_TIME_TABLE_LEN)
    {
        /* table has more than 64 events specified      */
        CTE_REPORT_ERROR(RSDK_CTE_DRV_TABLE_TOO_LONG, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
        CTE_HALT_ON_ERROR;
    }
    else
    {
        if (pTable->pEvents == NULL)
        {
            /* the events pointer is NULL       */
            CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_EVENTS, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
            CTE_HALT_ON_ERROR;
        }
        else
        {
            for (i = 0; i < pTable->tableLength; i++)
            {
                rez = Cte_TtEventCheck(&pTable->pEvents[i]);    /* check each event definition      */
                if (rez != (Std_ReturnType)E_OK)
                {
                    break;                                      /* error detected, so stop the loop */
                }
            }
        }
    }
    return rez;
}
/*=== Cte_TimeTableCheck ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to check the signals definitions table.
 * @details Are checked the types for SPT and FLEX outputs.
 *
 * @param[in]   pSignals          = pointer to the signals table
 * @return      E_OK/RSDK_SUCCESS = initialization succeeded
 *              other values      = initialization failed, use the appropriate tools to detect the issue
 *
 */
static Std_ReturnType Cte_SignalDefsCheck(Cte_SingleOutputDefType *pSignals)
{
    uint8                       defSPT;
    Cte_SingleOutputDefType     *pwSignals;
    Std_ReturnType              rez = (Std_ReturnType)E_OK;

    pwSignals = pSignals;
    defSPT = 0xffu;            /* the signals/outputs not defined       */
    while ((uint8) pwSignals->outputSignal < (uint8)CTE_OUTPUT_MAX)
    {
        if ((uint8) pwSignals->outputSignal < (uint8)CTE_OUTPUT_CTEP_0)
        {                       /* SPT event type                       */
            if ((pwSignals->signalType != CTE_OUT_LOGIC) && (pwSignals->signalType != CTE_OUT_TOGGLE))
            {
                /* not accepted type        */
                CTE_REPORT_ERROR(RSDK_CTE_DRV_SIG_OUT_WRG_TYPE, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
            }
            else
            {
                if (defSPT == 0xffu)
                {               /* SPT type not defined yet     */
                    defSPT = (uint8) pwSignals->signalType;
                }
                else
                {
                    rez = (defSPT != (uint8) pwSignals->signalType) ? RSDK_CTE_DRV_SIG_OUT_DIF_TYPE : RSDK_SUCCESS;
                }
            }
        }
        if ((uint8) pwSignals->outputSignal > (uint8) CTE_OUTPUT_SPT_RFS)
        {                       /* FLEX event type      */
            if (pwSignals->signalType != CTE_OUT_LOGIC)
            {
                /* not accepted type        */
                CTE_REPORT_ERROR(RSDK_CTE_DRV_SIG_OUT_WRG_TYPE, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
            }
        }
        if(rez != (Std_ReturnType)E_OK)
        {
            break;
        }
        pwSignals++;
    }
    return rez;
}
/*=== Cte_SignalDefsCheck ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to check the initialization parameters.
 * @details All necessary checks are done, to assure correct initialization flow.
 *          If many error occurs, only one is reported.
 *
 * @param[in]   pCteInitParams    = pointer to the initialization structure
 * @return      E_OK/RSDK_SUCCESS = initialization succeeded
 *              other values      = initialization failed, use the appropriate tools to detect the issue
 *
 */
static Std_ReturnType Cte_InitParamsCheck(const Cte_SetupParamsType *pCteInitParams)
{
    Std_ReturnType rez = (Std_ReturnType)E_OK;

    /* check the requested input mode       */
    if ((uint8) pCteInitParams->cteMode.workingMode >= (uint8) CTE_MODE_MAX)
    {
        CTE_REPORT_ERROR(RSDK_CTE_DRV_WRG_MODE, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
        CTE_HALT_ON_ERROR;
    }
    else if (((uint8) pCteInitParams->cteMode.workingMode == (uint8) CTE_SLAVE_CSI2) &&
            ((uint8) pCteInitParams->cteMode.cteWorkingParam0.cteCsi2Unit >= (uint8) CTE_CSI2_UNIT_MAX))
    {
        CTE_REPORT_ERROR(RSDK_CTE_DRV_WRG_CSI2_UNIT, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
        CTE_HALT_ON_ERROR;
    }
    else
    {
        if (((uint8) pCteInitParams->cteMode.workingMode == (uint8) CTE_SLAVE_CSI2) &&
                ((uint8) pCteInitParams->cteMode.cteWorkingParam1.cteCsi2Vc >= (uint8) CTE_CSI2_VC_MAX))
        {
            CTE_REPORT_ERROR(RSDK_CTE_DRV_WRG_CSI2_VC, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
            CTE_HALT_ON_ERROR;
        }
    }
    /* check the specified CTE clock frequency      */
    if ((rez == (Std_ReturnType)E_OK) && (pCteInitParams->cteClockFrecq == 0u))
    {
        CTE_REPORT_ERROR(RSDK_CTE_DRV_ZERO_FREQ, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
        CTE_HALT_ON_ERROR;
    }
    if (rez == (Std_ReturnType)E_OK)
    {
        /* check the first time table pointer       */
        if (pCteInitParams->pTimeTable0 == NULL)
        {
            CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_TABLE0, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
            CTE_HALT_ON_ERROR;
        }
        else
        {
            /* good table pointer, check the table content      */
            rez = Cte_TimeTableCheck(pCteInitParams->pTimeTable0);
            if (rez == (Std_ReturnType)E_OK)
            {
                /* check the output signal definitions      */
                if (pCteInitParams->pSignalDef0 == NULL)
                {
                    CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_SIG_DEF, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
                    CTE_HALT_ON_ERROR;
                }
                else
                {
                    rez = Cte_SignalDefsCheck(pCteInitParams->pSignalDef0);
                }
            }
        }
    }
    /* check the second time table if specified     */
    if (rez == (Std_ReturnType)E_OK)
    {
        if (pCteInitParams->pTimeTable1 != NULL)
        {
            /* good table pointer, the table 0 has a correct length ?       */
            if ((pCteInitParams->pTimeTable0->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN) ||
                    (pCteInitParams->pTimeTable1->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN))
            {
                CTE_REPORT_ERROR(RSDK_CTE_DRV_TABLE_TOO_LONG, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
            }
            if (rez == (Std_ReturnType)E_OK)
            {
                if (pCteInitParams->pSignalDef1 == NULL)
                {
                    CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_SIG_DEF, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
                    CTE_HALT_ON_ERROR;
                }
                else
                {
                    /* check the table 2 content        */
                    rez = Cte_TimeTableCheck(pCteInitParams->pTimeTable1);
                }
            }
        }
        else
        {       /* second table not defined     */
            if (pCteInitParams->pSignalDef1 != NULL)
            {
                /* misfit TimeTable/Output signal table     */
                CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_NULL_PTR_SIG_DEF, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
            }
        }
    }
    /* check the irq logic      */
    if (((uint32) pCteInitParams->cteIrqEvents != 0u) && (pCteInitParams->pCteCallback == NULL))
    {
        rez = RSDK_CTE_DRV_NULL_CALLBACK;
    }
    return rez;
}
/*=== Cte_InitParamsCheck ===========================*/

#endif  /* #if (CTE_DEV_ERROR_DETECT == STD_ON)     */

/*==================================================================================================*/
/**
 * @brief   Procedure to find the appropriate CTE clock divider.
 * @details The smallest divider which ensure the full table execution is selected
 *
 * @param[in]   pointers to the required time tables, in the correct order
 * @return      The necessary clock divider
 * @pre         The CTE clock frequency must be set before
 *
 */
static uint32 Cte_64BitCounting(uint32 initialValue, uint32 mul1, uint32 mul2, uint32 div1, uint32 div2)
{
    uint64 val = (uint64) initialValue;
    val *= (uint64) mul1;
    val *= (uint64) mul2;
    val /= (uint64) div1;
    val /= (uint64) div2;
    return ((uint32) val);
}
/*=== Cte_64BitCounting ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to find the appropriate CTE clock divider.
 * @details The smallest divider which ensure the full table execution is selected
 *
 * @param[in]   pointers to the required time tables, in the correct order
 * @return      The necessary clock divider
 * @pre         The CTE clock frequency must be set before
 *
 */
static uint8 Cte_ClockDividerGet(Cte_TimeTableDefType *pTimeTable0, Cte_TimeTableDefType *pTimeTable1)
{
    uint8  i;
    uint8  clockDivider;            /* final clock divider                                  */
    uint32 tableTimeMax;            /* max time value inside the time table [ns]            */
    uint32 tableTimeMax1;           /* max time value inside the time table 1 [ns]          */
    uint32 maxDelay;                /* max delay between consecutive events                 */
    uint32 maxDelay1;               /* max delay between consecutive events for table 1     */
    uint32 largeIntCount;           /* special value to get correct uint32 computing        */

    tableTimeMax = 0;
    maxDelay = 0;
    /* check the first table        */
    for (i = 0; i < pTimeTable0->tableLength; i++)
    {
        if (tableTimeMax < pTimeTable0->pEvents[i].absTime)
        {
            if (maxDelay < (pTimeTable0->pEvents[i].absTime - tableTimeMax))
            {
                maxDelay = pTimeTable0->pEvents[i].absTime - tableTimeMax;  /* the maximum time difference between
                                                                               two consecutive events               */
            }
            tableTimeMax = pTimeTable0->pEvents[i].absTime;                 /* keep the maximum referred time       */
        }
        else
        {
            maxDelay = CTE_TOO_BIG_TIME_DELAY;          /* incorrect time sequence, so the delay is a very big number */
        }
    }
    /* check the second table       */
    if ((pTimeTable1 != NULL) && (maxDelay != CTE_TOO_BIG_TIME_DELAY))
    {
        tableTimeMax1 = 0;
        maxDelay1 = 0;
        for (i = 0; i < pTimeTable1->tableLength; i++)
        {
            if (tableTimeMax1 < pTimeTable1->pEvents[i].absTime)
            {
                if (maxDelay1 < (pTimeTable1->pEvents[i].absTime - tableTimeMax1))
                {
                    maxDelay1 = pTimeTable1->pEvents[i].absTime - tableTimeMax1; /* the maximum time difference between
                                                                                    two consecutive events  */
                }
                tableTimeMax1 = pTimeTable1->pEvents[i].absTime;                 /* keep the maximum referred time  */
            }
            else
            {
                maxDelay1 = CTE_TOO_BIG_TIME_DELAY;     /* incorrect time sequence, so the delay is a very big number */
            }
        }
        /* analyze the maximum values for table 0 and 1     */
        if(maxDelay1 > maxDelay)
        {
            maxDelay = maxDelay1;
        }
    }
    largeIntCount = Cte_64BitCounting(maxDelay, gsDriverData.cteWorkingFreq, 1u, /* remember, maxDelay is in ns      */
            CTE_1G_FREQUENCY, CTE_MAX_TIME_COUNTER); /* divider is_equal to (maxDelay*CTE_clk_freq)/(1G*MAX_COUNTER) */
    largeIntCount++;
    if (largeIntCount > (uint32) CTE_CLOCK_DIVIDER_LIMIT)
    {
        clockDivider = CTE_CLOCK_DIVIDER_LIMIT;         /* divider limit exceeded       */
    }
    else
    {
        clockDivider = (uint8) largeIntCount;           /* the final divider            */
    }
    return clockDivider;
}
/*=== Cte_ClockDividerGet ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to set the mask for the new state required.
 * @details The value is set as 64 bits value, as for LSB/MSB registry values concatenated
 *
 * @param[in]   pointers to the signal definitions array
 * @param[in]   pointers to the action array
 * @param[in]   pointer to the result
 * @return      E_OK/RSDK_SUCCESS if successful, error if other
 *
 */
static Std_ReturnType Cte_SignalMaskSet(Cte_SingleOutputDefType *pSignalDef, Cte_ActionType *action, uint64 *pVal)
{
    Cte_SingleOutputDefType *pwSignalDef;   /* pointer to signal definitions to be used for process     */
    uint8                   shiftValue;     /* the left shift for the final value                       */
    uint64                  reqMask;        /* the mask, "initial" and "final"                          */
    Std_ReturnType          rez = (Std_ReturnType)E_OK;

    pwSignalDef = pSignalDef;
    while (pwSignalDef->outputSignal < CTE_OUTPUT_MAX)
    {
        if (pwSignalDef->outputSignal == action->outputSignal)  /* find the required signal into defined table  */
        {
            break;
        }
        pwSignalDef++;                                          /* pass to next definition                      */
    }
    if (pwSignalDef->outputSignal == action->outputSignal)
    {
        /* get the mask according to the type/newState      */
        switch ((uint8) pwSignalDef->signalType)
        {
        case (uint8) CTE_OUT_TOGGLE:
            switch (action->newToggleState)
            {
            case CTE_TOGGLE_SET_TO_LOW:
                reqMask = (uint64) CTE_TOGGLE_MASK_TO_LOW;
                break;
            case CTE_TOGGLE_SET_TO_HIGH:
                reqMask = (uint64) CTE_TOGGLE_MASK_TO_HIGH;
                break;
            case CTE_TOGGLE_FLIP:
                reqMask = (uint64) CTE_TOGGLE_MASK_TOGGLE;
                break;
            case CTE_TOGGLE_DONT_CARE:
                reqMask = (uint64) CTE_TOGGLE_MASK_UNCHANGED;
                break;
            default:                                        /* wrong value, not in enum     */
                reqMask = 0;
                CTE_REPORT_ERROR(RSDK_CTE_DRV_SIG_OUT_STATE_WRG, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
                break;
            }
            break;
        case (uint8) CTE_OUT_CLOCK:
            switch (action->newClockState)
            {
            case CTE_CLOCK_SET_TO_LOW:
                reqMask = (uint64) CTE_CLOCK_MASK_TO_LOW;
                break;
            case CTE_CLOCK_ACTIVE_SYNC:
                reqMask = (uint64) CTE_CLOCK_MASK_SYNC_RISING;
                break;
            case CTE_CLOCK_ACTIVE:
                reqMask = (uint64_t) CTE_CLOCK_MASK_RUNNING;
                break;
            case CTE_CLOCK_SET_TO_HIGH:
                reqMask = (uint64) CTE_CLOCK_MASK_TO_HIGH;
                break;
            default:                                    /* wrong value, not in enum     */
                reqMask = 0;
                CTE_REPORT_ERROR(RSDK_CTE_DRV_SIG_OUT_STATE_WRG, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
                break;
            }
            break;
        case (uint8) CTE_OUT_LOGIC:
            switch (action->newLogicState)
            {
            case CTE_LOGIC_SET_TO_LOW:
                reqMask = (uint64) CTE_LOGIC_MASK_TO_LOW;
                break;
            case CTE_LOGIC_SET_TO_HIGH:
                reqMask = (uint64) CTE_LOGIC_MASK_TO_HIGH;
                break;
            case CTE_LOGIC_SET_TO_HIGH_Z:
                reqMask = (uint64) CTE_LOGIC_MASK_TO_HIZ;
                break;
            case CTE_LOGIC_UNCHANGED:               /* nothing to do                    */
                reqMask = (uint64) CTE_LOGIC_MASK_UNCHANGED;
                break;
            default:                                /* wrong value, not in enum         */
                reqMask = 0;
                CTE_REPORT_ERROR(RSDK_CTE_DRV_SIG_OUT_STATE_WRG, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
                break;
            }
            break;
        default:                                    /* HiZ is the default in this case  */
            reqMask = 0;                            /* nothing to do                    */
            break;
        }
        if (rez == (Std_ReturnType)E_OK)
        {                       /* only if the result is good       */
            if ((uint8) pwSignalDef->outputSignal < (uint8) CTE_OUTPUT_CTEP_0)
            {                   /* SPT events to be processed       */
                if (reqMask != 0u)
                {
                    reqMask = CTE_SPT0_SIG_MASK;    /* SPT events can be    */
                }
                shiftValue = (uint8) pwSignalDef->outputSignal;
            }
            else
            {                   /* other signals, RFS/RCS/CTEP0...7     */
                shiftValue = (uint8)(CTE_OUTPUT_MASK_SHIFT_BASE
                        + (((uint8) pwSignalDef->outputSignal - (uint8) CTE_OUTPUT_CTEP_0) << 1u));
            } /* if((uint8)pwSignalDef->outputSignal < (uint8)CTE_OUTPUT_CTEP_0)    */
            *pVal |= reqMask << shiftValue;
        }
    }
    else
    {
        CTE_REPORT_ERROR(RSDK_CTE_DRV_SIG_NOT_DEF, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
        CTE_HALT_ON_ERROR;
    } /* if(pwSignalDef->outputSignal == signal->outputSignal)  */
    return rez;
}
/*=== Cte_SignalMaskset ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to get the final mask value for time table.
 * @details The result is on 64 bits, for both
 *
 * @param[in]   pointers to the time table
 * @param[in]   pointers to the signal definitions array
 * @param[in]   pointer to the resulted mask
 * @return      E_OK/RSDK_SUCCESS if successful, error if other
 *
 */
static Std_ReturnType Cte_MaskValuesGet(Cte_ActionType *pActions, Cte_SingleOutputDefType *pSignalDef, uint64 *pVal)
{
    Cte_ActionType  *pwActions;
    Std_ReturnType  rez = (Std_ReturnType)E_OK;

    *pVal = 0u;
    pwActions = pActions;
    while (((uint8) pwActions->outputSignal < (uint8) CTE_OUTPUT_MAX) && (rez == (Std_ReturnType)E_OK))
    {
        switch ((uint8) pwActions->outputSignal)
        {
        case (uint8) CTE_OUTPUT_FLEX_0:
            if (pwActions->newLogicState != CTE_LOGIC_SET_TO_LOW)
            {
                *pVal |= CTE_FLEX_SIG_MASK;
            }
            break;
        case (uint8) CTE_OUTPUT_FLEX_1:
            if (pwActions->newLogicState != CTE_LOGIC_SET_TO_LOW)
            {
                *pVal |= (CTE_FLEX_SIG_MASK << 1u);
            }
            break;
        default:                        /* for all remaining signals, RFS/RCS/SPT0...3/CTEP0...7    */
            rez = Cte_SignalMaskSet(pSignalDef, pwActions, pVal);
            break;
        }
        pwActions++;                    /* move to the next action      */
    }
    return rez;
}
/*=== Get_MaskValues ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to set te timing table.
 * @details One table at a time, depending the start of the table
 *
 * @param[in]   pointers to the required time tables, in the correct order
 * @return      The necessary clock divider
 * @pre         The CTE clock frequency must be set before
 *
 */
static Std_ReturnType Cte_TimingTableSet(Cte_TimeTableDefType *pTimeTable, Cte_SingleOutputDefType *pSignalDef,
        volatile void *pLut)
{
    uint64              longIntMask;                /* values for 64 bits computation       */
    uint32              i, j, tmpVal, lastTimeTick, work32U;
    volatile struct LUT *pwLut, *pLutW;
    Std_ReturnType rez = (Std_ReturnType)E_OK;

    if(pTimeTable->pEvents == NULL)
    {
        CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_EVENTS, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
        CTE_HALT_ON_ERROR;
    }
    else
    {
        pwLut = (volatile struct LUT *)pLut;
        pLutW = (volatile struct LUT *)pLut;
        /* for the first event set all output to low    */
        pwLut->LSB[0] = 0u;
        pwLut->MSB[0] = 0u;
        /* set all consecutive events to "unchanged" (respectively high for clocks and low for SPT events)  */
        for (i = 1; i < CTE_MAX_SMALL_TIME_TABLE_LEN; i++)
        {
            pwLut->LSB[i] = 0u;           /* SPT events to low                          */
            pwLut->MSB[i] = 0x6fffffu;    /* FlexTime to 0, all other to "unchanged"    */
        }
        /* set the new values       */
        i = 0;
        lastTimeTick = 0;
        while ((i < pTimeTable->tableLength) && (i < CTE_MAX_SMALL_TIME_TABLE_LEN))
        {
            if(pTimeTable->pEvents[i].pEventActions == NULL)
            {
                CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_ACTIONS, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
            }
            if(rez == (Std_ReturnType)E_OK)
            {
                rez = Cte_MaskValuesGet(pTimeTable->pEvents[i].pEventActions, pSignalDef, &longIntMask);
            }
            if (rez != (Std_ReturnType)E_OK)
            {
                break;                              /* error to get the signals mask, stop here */
            }
            tmpVal = Cte_64BitCounting(pTimeTable->pEvents[i].absTime, gsDriverData.cteWorkingFreq, 1u,
                    gsDriverData.cteMainClockDivider, CTE_1G_FREQUENCY);
            work32U = tmpVal - lastTimeTick;
            if (work32U == 0u)
            {
                work32U = 1;                        /* time table value must not be 0   */
            }
            lastTimeTick = tmpVal;
            pLutW->LSB[i] = work32U + (uint32) longIntMask;
            longIntMask >>= 32u;                    /* get the MSB of the mask          */
            pLutW->MSB[i] = (uint32) longIntMask;
            i++;
        }
        if(rez == (Std_ReturnType)E_OK)
        {
            pwLut++;                                /* go to next LUT, if necessary (more than 32 events)   */
            if (i < pTimeTable->tableLength)
            {           /* second table must be used too        */
                /* erase the CTE time table first       */
                for (i = 0; i < CTE_MAX_SMALL_TIME_TABLE_LEN; i++)
                {
                    pwLut->LSB[i] = 0u;
                    pwLut->MSB[i] = 0u;
                }
            }
            j = 0;
            while (i < pTimeTable->tableLength)
            {
                if(pTimeTable->pEvents[i].pEventActions == NULL)
                {
                    CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_ACTIONS, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
                    CTE_HALT_ON_ERROR;
                }
                if(rez == (Std_ReturnType)E_OK)
                {
                    rez = Cte_MaskValuesGet(pTimeTable->pEvents[i].pEventActions, pSignalDef, &longIntMask);
                }
                if(rez != (Std_ReturnType)E_OK)
                {
                    break;                              /* error to get the signals mask, stop here     */
                }
                tmpVal = Cte_64BitCounting(pTimeTable->pEvents[i].absTime, gsDriverData.cteWorkingFreq, 1u,
                        gsDriverData.cteMainClockDivider, CTE_1G_FREQUENCY);
                work32U = (uint32_t) tmpVal;
                work32U -= lastTimeTick;
                if (work32U == 0u)
                {
                    work32U = 1;                        /* time table value must not be 0               */
                }
                lastTimeTick = tmpVal;
                pwLut->LSB[j] = work32U + (uint32) longIntMask;
                longIntMask >>= 32u;                    /* get the MSB of the mask                      */
                pwLut->MSB[j] = (uint32) longIntMask;
                i++;
                j++;
            }
        }
    }
    return rez;
}
/*=== CteTimingTableSet ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to set the outputs.
 * @details One table at a time, depending the start of the table
 *
 * @param[in]   pointers to the output definitions
 * @param[in]   the table index [0,1]
 * @return      E_OK/RSDK_SUCCESS = success; other = error
 *
 */
static void Cte_OutputSetup(Cte_SingleOutputDefType *pSignalDef, uint8 idx)
{
    uint32                  mask;
    Cte_SingleOutputDefType *pwSignalDef;

    pwSignalDef = pSignalDef;
    gspCTE->SIGTYPE0[idx] = 0u;            /* reset all the types to default after reset (HiZ)      */
    gspCTE->SIGTYPE1[idx] = 0u;
    while ((uint8) pwSignalDef->outputSignal < (uint8) CTE_OUTPUT_MAX)
    {
        if ((uint8) pwSignalDef->outputSignal < (uint8) CTE_OUTPUT_CTEP_0)
        {                   /*  SPT events      */
            if (pwSignalDef->signalType == CTE_OUT_TOGGLE)
            {



                CTE_SET_REGISTRY32(&gspCTE->SIGTYPE0[idx], CTE_SIGTYPE0_SPT_EVT_MASK, CTE_SIGTYPE0_SPT_EVT(1u));

            }
        }
        else
        {
            if ((uint8) pwSignalDef->outputSignal < (uint8) CTE_OUTPUT_SPT_RCS)
            {               /* CTEP events      */
                mask = ((uint32) pwSignalDef->signalType) <<
                        (16u + (uint8) pwSignalDef->outputSignal - (uint8) CTE_OUTPUT_CTEP_0);
                gspCTE->SIGTYPE0[idx] |= mask;
            }
            else
            {               /* RCS/RFS      */
                if ((uint8) pwSignalDef->outputSignal < (uint8) CTE_OUTPUT_FLEX_0)
                {
                    mask = ((uint32) pwSignalDef->signalType) <<
                            (((uint8) pwSignalDef->outputSignal - (uint8) CTE_OUTPUT_SPT_RCS) * 2u);
                    gspCTE->SIGTYPE1[idx] |= mask;
                }
            }
        }
        pwSignalDef++;
    }
}
/*=== Cte_OutputSetup ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to set the running mode, continuous one table or toggle.
 * @details The driver permit only three executions :
 *          - one table of max 32 events
 *          - one table of max 64 events
 *          - two tables in toggle mode
 *
 * @param[in]   pointers to the initialization params
 * @return      E_OK/RSDK_SUCCESS = success; other = error
 *
 */
static void Cte_RunModeSet(const Cte_SetupParamsType *pCteInitParams)
{
    uint32 mask;

    if (pCteInitParams->pTimeTable1 == NULL)
    {                       /* only one table defined       */
        if (pCteInitParams->pTimeTable0->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN)
        {
            mask = 3u;      /* "toggle", but with only one table, to be correlated with the timing      */
        }
        else
        {
            mask = 2u;      /* single table, table 0        */
        }
    }
    else
    {
        mask = 3u;          /* toggle with two tables       */
    }
    CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_OPMOD_SL_MASK, CTE_CNTRL_OPMOD_SL(mask));
}
/*=== Cte_RunModeSet ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to set the appropriate timing limits.
 * @details The driver permit only three executions :
 *          - one table of max 32 events
 *          - one table of max 64 events
 *          - two tables in toggle mode
 *          and the procedure must complete the correct settings
 *
 * @param[in]   pointers to the initialization params
 * @return      E_OK/RSDK_SUCCESS = success; other = error
 *
 */
static void Cte_TableTimeLimitSet(const Cte_SetupParamsType *pCteInitParams)
{
    uint32 lutDur0, lutDur1;

    lutDur0 = Cte_64BitCounting(pCteInitParams->pTimeTable0->tableTimeExecLimit, gsDriverData.cteWorkingFreq, 1u,
            gsDriverData.cteMainClockDivider, CTE_1G_FREQUENCY);








    if (pCteInitParams->pTimeTable1 == NULL)
    {                       /* only one table defined       */
        if (pCteInitParams->pTimeTable0->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN)
        {
            gspCTE->LUT_DUR = 0u;               /* the first LUT duration must be 0     */
            /* use the appropriate value for LUTDUR1: table 0 duration (the normal solution)
               or the maximum table duration if all specified durations are 0       */
            if(lutDur0 != 0u)
            {
                gspCTE->LUT_DUR1 = lutDur0;
        }
        else
        {
                gspCTE->LUT_DUR1 = 0xffffffffu;
            }
        }
        else
        {
            /* single table, table 0        */
            gspCTE->LUT_DUR = lutDur0;
        }
    }
    else
    {
        /* two tables usage             */
        lutDur1 = Cte_64BitCounting(pCteInitParams->pTimeTable1->tableTimeExecLimit, gsDriverData.cteWorkingFreq, 1u,
                gsDriverData.cteMainClockDivider, CTE_1G_FREQUENCY);








        /* toggle with two tables       */
        gspCTE->LUT_DUR = lutDur0;
        gspCTE->LUT_DUR1 = lutDur1;
    }
}
/*=== Cte_TableTimeLimitSet ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to fill the array of requested periods
 * @details Simple output definitions parsing and clock period collect
 *
 * @param[in]   pointer to the initialization params, pointer to period array, filling start index
 * @return      E_OK/RSDK_SUCCESS = success; other = error
 *
 */
static Std_ReturnType Cte_PeriodArrayFill(Cte_SingleOutputDefType *pDef, uint32 *pArray, uint32 *first)
{
    uint32                  idx;
    Cte_SingleOutputDefType *pwDef;
    Std_ReturnType          rez = (Std_ReturnType)E_OK;

    pwDef = pDef;
    idx = *first;
    while ((uint8) pwDef->outputSignal < (uint8) CTE_OUTPUT_MAX)
    {
        if (pwDef->signalType == CTE_OUT_CLOCK)
        {
            if (pwDef->clockPeriod == 0u)
            {
                /* the clock period can't be 0      */
                CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_CLK_PEROD, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
                break;
            }
            pArray[idx] = pwDef->clockPeriod;
            idx++;
        }
        pwDef++;
    }
    *first = idx;
    return rez;
}
/*=== Cte_PeriodArrayFill ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to fill the final periods array
 * @details All requested periods are processed and try to use up to 4 clocks.
 *          If not possible, an error is returned.
 *
 * @param[in]   array length, pointer to the initial periods array
 * @return      E_OK/RSDK_SUCCESS = success; other = error
 *
 */
static Std_ReturnType Cte_FinalPeriodProcess(uint32 reqClocks, uint32 *allReqPeriods)
{
    uint32          i, j, k;
    Std_ReturnType  rez = (Std_ReturnType)E_OK;

    gsDriverData.cteClocksPeriods[0] = allReqPeriods[0];    /* initialize the resulting period array        */
    j = 0u;                                                 /* the working final index                      */
    k = 1;                                                  /* number of clocks inside the limits           */
    for (i = 1; i < reqClocks; i++)                         /* process the rest of the array                */
    {
        if (allReqPeriods[i] < (2u * allReqPeriods[j]))
        {
            k++;                            /* is possible to have both periods inside 40% error                    */
        }
        else
        {                                   /* too different values, so step to next clock, closing the one before  */
            if (k > 1u)
            {                               /* more requested clocks were processed before                          */
                /* 80% coeficient applied       */
                allReqPeriods[j] = Cte_64BitCounting(allReqPeriods[j], allReqPeriods[i-1u], 8u, 10u, allReqPeriods[j]);
                k = 1u;
            }
            gsDriverData.cteClocksPeriods[j] = allReqPeriods[j];
            j++;
            if (j >= CTE_INTERNAL_CLOCKS)
            {
                CTE_REPORT_ERROR(RSDK_CTE_DRV_TOO_MANY_CLOCKS, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
                break;
            }
            allReqPeriods[j] = allReqPeriods[i];
        }
    }
    /* process the final values     */
    if (rez == (Std_ReturnType)E_OK)
    {
        if (k > 1u)
        {                       /* more requested clocks were processed before      */
            allReqPeriods[j] = Cte_64BitCounting(allReqPeriods[j], allReqPeriods[i - 1u], 8u, 10u, allReqPeriods[j]);
        }
        gsDriverData.cteClocksPeriods[j] = allReqPeriods[j];
        gsDriverData.cteUsedClockDividers = (uint8_t) j;
        gsDriverData.cteUsedClockDividers++;
    }
    return rez;
}
/*=== Cte_FinalPeriodProcess ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to define the necessary clock periods.
 * @details The hardware can use up to 4 clock dividers, so up to 4 periods available.
 *          If more clocks required, intermediate periods will be used, to reduce the deviation below 40%.
 *          If it is not possible to have up to 4 clocks and all the required periods to enter 40% deviation,
 *          an error will be returned.
 *
 * @param[in]   pointers to the initialization params
 * @return      E_OK/RSDK_SUCCESS = success; other = error
 *
 */
static Std_ReturnType Cte_ClockDividersSet(const Cte_SetupParamsType *pCteInitParams)
{
    uint32          allReqPeriods[CTE_OUTPUT_MAX];
    uint32          reqClocks, i, j, k;
    uint32          computingVal;
    uint8           finalDivider;
    Std_ReturnType  rez;

    reqClocks = 0u;
    rez = Cte_PeriodArrayFill(pCteInitParams->pSignalDef0, allReqPeriods, &reqClocks);
    if ((rez == (Std_ReturnType)E_OK) && (pCteInitParams->pSignalDef1 != NULL))
    {
        rez = Cte_PeriodArrayFill(pCteInitParams->pSignalDef1, allReqPeriods, &reqClocks);
    }
    if ((rez == (Std_ReturnType)E_OK) && (reqClocks != 0u))
    {           /* if there are clocks defined      */
        /* sort the clocks      */
        j = 1;
        while (j != 0u)
        {
            j = 0u;
            for (i = 1; i < reqClocks; i++)
            {
                if (allReqPeriods[i - 1u] > allReqPeriods[i])
                {
                    j = 1;
                    k = allReqPeriods[i - 1u];
                    allReqPeriods[i - 1u] = allReqPeriods[i];
                    allReqPeriods[i] = k;
                }
            }
        }
        /* remove identical clocks      */
        k = 1u;
        while (k != 0u)
        {
            k = 0u;
            for (i = 1; i < reqClocks; i++)
            {
                if (allReqPeriods[i] == allReqPeriods[i - 1u])
                {
                    k = 1u;
                    break;
                }
            }
            if (k != 0u)
            {
                for (j = i + 1u; j < reqClocks; j++)
                {
                    allReqPeriods[j - 1u] = allReqPeriods[j];
                }
                reqClocks--;
            }
        }
        /* process further only if more than 4, compress to have    */
        if (reqClocks > CTE_INTERNAL_CLOCKS)
        {
            rez = Cte_FinalPeriodProcess(reqClocks, allReqPeriods);
        }
        else
        {
            for (i = 0; i < reqClocks; i++)
            {
                gsDriverData.cteClocksPeriods[i] = allReqPeriods[i];
            }
            gsDriverData.cteUsedClockDividers = (uint8) reqClocks;
        }
        if (rez == (Std_ReturnType)E_OK)
        {
            for (i = 0; i < gsDriverData.cteUsedClockDividers; i++)
            {
                computingVal = Cte_64BitCounting(gsDriverData.cteClocksPeriods[i], gsDriverData.cteWorkingFreq, 1u,
                        CTE_1G_FREQUENCY, 1u);
                if (computingVal > (uint32) CTE_MAX_REQ_CLK_DIVIDER)
                {
                    CTE_REPORT_ERROR(RSDK_CTE_DRV_CLK_DIVIDER_ERROR, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
                    CTE_HALT_ON_ERROR;
                    break;
                }
                finalDivider = (uint8) computingVal;
                j = 0u;
                k = 1u;
                computingVal = 1u;
                while(finalDivider > (uint8)computingVal)
                {
                    j++;
                    computingVal *= 2u;
                    computingVal += (k & 1u);
                    k <<= 2u;
                }
                switch (i)
                {
                case 0u:                    /* first clock      */
                    CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CLKDIV_1_MASK, CTE_CNTRL1_CLKDIV_1(j));
                    break;
                case 1u:                    /* second clock     */
                    CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CLKDIV_2_MASK, CTE_CNTRL1_CLKDIV_2(j));
                    break;
                case 2u:                    /* third clock      */
                    CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CLKDIV_3_MASK, CTE_CNTRL1_CLKDIV_3(j));
                    break;
                default:                    /* fourth clock     */
                    CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CLKDIV_4_MASK, CTE_CNTRL1_CLKDIV_4(j));
                    break;
                }
            }
        }
    }
    return rez;
}
/*=== Cte_ClockDividersSet ===========================*/

/*==================================================================================================*/
/**
 * @brief   Procedure to define the necessary clock periods.
 * @details The hardware can use up to 4 clock dividers, so up to 4 periods available.
 *          If more clocks required, intermediate periods will be used, to reduce the deviation below 40%.
 *          If it is not possible to have up to 4 clocks and all the required periods to enter 40% deviation,
 *          an error will be returned.
 *
 * @param[in]   pointers to the initialization params
 * @return      E_OK/RSDK_SUCCESS = success; other = error
 *
 */
static void Cte_OutputClockSelect(const Cte_SetupParamsType *pCteInitParams)
{
    Cte_SingleOutputDefType *pDef;
    uint32                  i;
    uint32                  tstPeriodLow, tstPeriodHigh;

    pDef = pCteInitParams->pSignalDef0;
    while ((uint8) pDef->outputSignal < (uint8) CTE_OUTPUT_MAX)
    {
        if (pDef->signalType == CTE_OUT_CLOCK)
        {
            tstPeriodLow = pDef->clockPeriod;
            tstPeriodHigh = tstPeriodLow + (tstPeriodLow / 2u);
            tstPeriodLow -= tstPeriodLow / 2u;
            for (i = 0; i < gsDriverData.cteUsedClockDividers; i++)
            {
                if ((gsDriverData.cteClocksPeriods[i] >= tstPeriodLow)
                        && (gsDriverData.cteClocksPeriods[i] <= tstPeriodHigh))
                {
                    break;
                }
            }
            i <<= ((uint8) pDef->outputSignal - (uint8) CTE_OUTPUT_CTEP_0) * 2u;
            gspCTE->CLKSEL |= i;
        }
        pDef++;
    }
}
/*=== Cte_OutputClockSelect ===========================*/

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 ==================================================================================================*/

/*================================================================================================*/
/**
 * @brief   Low level initialization procedure for CTE driver
 * @details If called, the CTE function is stopped.
 *          After initialization the CTE is not started, a specific Cte_Start call must be used for this.
 *          The operation can be done at any moment; if the CTE is working, it will be stopped.
 *
 * @param[in]   pCteInitParams    = pointer to the initialization structure
 * @param[in]   pLutChecksum      = pointer to a uint64 value, which will receive the final LUT checksum;
 *                                  this value can be checked later using Cte_GetLutChecksum
 * @return      E_OK/RSDK_SUCCESS = initialization succeeded
 *              other values      = initialization failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_Setup(const Cte_SetupParamsType *pCteInitParams, uint64 *pLutChecksum)
{
    Std_ReturnType  rez;
    uint8           cteClockDivider;            /* the clock divider        */
    uint32          len;

    #if defined(linux)
        uint32          mask;
    #else
        volatile SRC_1_Type * pSrc_1;
    #endif




    /* set the driver status to NOT_INITIALIZED     */
    gsDriverData.cteDriverStatus = (uint8_t) CTE_DRIVER_STATE_NOT_INIT;
    /* check the initialization parameters          */
    if ((pCteInitParams == NULL) || (pLutChecksum == NULL))
    {
        CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_PARAMS, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
        CTE_HALT_ON_ERROR;
    }
    else
    {
        rez = Cte_InitParamsCheck(pCteInitParams);
    }
    if (gspCTE == NULL)
    {
#ifndef linux
        gspCTE = IP_CTE;
#else
        if(gpRsdkCteDevice != NULL)
        {
            gspCTE = (volatile CTE_Type*)gpRsdkCteDevice->pMemMapVirtAddr;
        }
#endif
        if (gspCTE == NULL)
        {
            CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_INITIALIZED, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
            CTE_HALT_ON_ERROR;
        }
    }
    if (rez == (Std_ReturnType)E_OK)
    {
        /* initialize the necessary data        */
        gsDriverData.cteWorkingFreq = pCteInitParams->cteClockFrecq; /* the working frequency for further computing */
        cteClockDivider = Cte_ClockDividerGet(pCteInitParams->pTimeTable0, pCteInitParams->pTimeTable1);
        if (cteClockDivider >= CTE_CLOCK_DIVIDER_LIMIT)
        {
            CTE_REPORT_ERROR(RSDK_CTE_DRV_CLK_DIVIDER_ERROR, CTE_E_PARAM_VALUE, CTE_SETUP_PARAM_CHECK);
            CTE_HALT_ON_ERROR;
        }
        else
        {
            /* reset the CTE        */
            /* set the bit to reset CTE     */
            CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_CTE_RST_MASK, CTE_CNTRL_CTE_RST(1u));
            gsDriverData.cteMainClockDivider = cteClockDivider; /* keep the divider; simple delay to get the reset  */
            /* reset the bit to be able to use the registry     */
            CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_CTE_RST_MASK, CTE_CNTRL_CTE_RST(0u));
            /* compute the LUT checksum, including the MSBit    */
            CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL1_CHKSM_MD_MASK, CTE_CNTRL1_CHKSM_MD(1u));

            /* CTE specific initialization              */
            /* 1. Configure time instances and corresponding signal states in the timing table register LUT_LSB_0.  */
            /* 2. Configure remaining signal states in the timing table register LUT_MSB_0.                         */
            rez = Cte_TimingTableSet(pCteInitParams->pTimeTable0, pCteInitParams->pSignalDef0,
                    (volatile void *)gspCTE->LUT);
            /* 3. Configure time instances and corresponding signal states in the timing table register LUT_LSB_1.  */
            /* 4. Configure remaining signal states in the timing table register LUT_MSB_1.                         */
            if ((rez == (Std_ReturnType)E_OK) && (pCteInitParams->pTimeTable1 != NULL))
            {
                rez = Cte_TimingTableSet(pCteInitParams->pTimeTable1, pCteInitParams->pSignalDef1,
                        (volatile void *)&gspCTE->LUT[1]);
            }
            *pLutChecksum = Cte_GetLutChecksum();
            /* 5. Configure signal types in signal type registers CTE_SIGTYPE0/1.                                   */
            if (rez == (Std_ReturnType)E_OK)
            {
                Cte_OutputSetup(pCteInitParams->pSignalDef0, 0);
                if (pCteInitParams->pSignalDef1 != NULL)
                {
                    Cte_OutputSetup(pCteInitParams->pSignalDef1, 1);
                }
                if(pCteInitParams->pTimeTable0->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN)
                {
                    Cte_OutputSetup(pCteInitParams->pSignalDef0, 1);
                }
            }
            if (rez == (Std_ReturnType)E_OK)
            {
                /* 6. Configure MA_SL_ST bit in control register CTE_CNTRL.                                         */
                if (pCteInitParams->cteMode.workingMode == CTE_MASTER)
                {               /* Master mode used     */
                    /* normally the bit must be already 0 after reset       */
                    CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_MA_SL_ST_MASK, CTE_CNTRL_MA_SL_ST(0u));
                }
                else
                {
                    /* set bit for Slave mode       */
                    CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_MA_SL_ST_MASK, CTE_CNTRL_MA_SL_ST(0u));
                    /* set the appropriate registry values for the Slave mode       */

#ifdef linux
                    mask = (uint32)pCteInitParams->cteMode.workingMode - (uint32_t)RSDK_CTE_SLAVE_EXTERNAL;
                    mask += (uint32)pCteInitParams->cteMode.cteWorkingParam1.cteCsi2Vc << 1u;
                    mask += (uint32)pCteInitParams->cteMode.cteWorkingParam0.cteCsi2Unit << 3u;
                    ((volatile SRC_1_Type*)gpRsdkCteDevice->pSrc_1)->CTE_CTRL_REG = mask;
#else
                    pSrc_1 = IP_SRC_1;
                    CTE_SET_REGISTRY32(&pSrc_1->CTE_CTRL_REG, SRC_1_CTE_CTRL_REG_IN_CTE_MASK,
                            SRC_1_CTE_CTRL_REG_IN_CTE(
                                    (uint32)pCteInitParams->cteMode.workingMode - (uint32)RSDK_CTE_SLAVE_EXTERNAL));
                    CTE_SET_REGISTRY32(&pSrc_1->CTE_CTRL_REG, SRC_1_CTE_CTRL_REG_MIPICSI2_ID_MASK,
                          SRC_1_CTE_CTRL_REG_MIPICSI2_ID((uint32)pCteInitParams->cteMode.cteWorkingParam0.cteCsi2Unit));
                    CTE_SET_REGISTRY32(&pSrc_1->CTE_CTRL_REG, SRC_1_CTE_CTRL_REG_VC_ID_MASK,
                            SRC_1_CTE_CTRL_REG_VC_ID((uint32_t)pCteInitParams->cteMode.cteWorkingParam1.cteCsi2Vc));
#endif // #ifdef linux
























                    CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_RFS_DLY_MASK,
                            CTE_CNTRL_RFS_DLY((uint32) pCteInitParams->cteMode.cteWorkingParam0.cteInternalRfsDelay));
                    CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_RCS_DLY_MASK,
                            CTE_CNTRL_RCS_DLY((uint32) pCteInitParams->cteMode.cteWorkingParam1.cteInternalRcsDelay));
                }
                /* 7. Configure REP_CNT value in control register CTE_CNTRL.                                        */
                CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_REP_CNT_MASK,
                        CTE_CNTRL_REP_CNT((uint32) pCteInitParams->repeatCount));
                /* 8. Configure OPMOD_SL bits in register CTE_CNTRL to define continuous run mode or toggle mode.   */
                Cte_RunModeSet(pCteInitParams);
                /* 9. Configure the duration counter for TT0 and TT1 execution in registers CTE_LUT_DUR and
                 * CTE_LUT_DUR1 respectively.       */
                Cte_TableTimeLimitSet(pCteInitParams);
                /* 10. Configure timemode bit in CTE_CNTRL1 for absolute/relative mode of timing table execution.   */
                /* only relative timing used        */
                CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_TIMEMODE_MASK, CTE_CNTRL1_TIMEMODE(0u));
                /* 11. Configure CTECK_DV in CTE_CNTRL1 to define the CTE data path clock.                          */
                CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CTECK_DV_MASK,
                        CTE_CNTRL1_CTECK_DV(gsDriverData.cteMainClockDivider));
                /* 12. Configure CLKDIV_1-CLKDIV_4 in CTE_CNTRL1 to define clocks for external signals.             */
                rez = Cte_ClockDividersSet(pCteInitParams);
            }
            /* 13. Configure CLK_SEL0-CLK_SEL9 in clock select register CTE_CLKSEL to select the clock divider
             * for the external signals.            */
            if (rez == (Std_ReturnType)E_OK)
            {
                Cte_OutputClockSelect(pCteInitParams);
            }
            /* Other necessary setup : IRQ      */
            if (rez == (Std_ReturnType)E_OK)
            {
                rez = Cte_IrqInit(pCteInitParams);
                gsDriverData.cteReqEvents = (uint32) pCteInitParams->cteIrqEvents;
                gsDriverData.pCteCallback = pCteInitParams->pCteCallback;
            }
            /* 14. Enable CTE by setting CTE_EN bit in CTE_CNTRL1.                                                  */
            /* this step will be done in a separate procedure       */
        }
    }
    if (rez == (Std_ReturnType)E_OK)
    {
        /* copy the outputs setup           */
        len = 0u;
        while (pCteInitParams->pSignalDef0[len].outputSignal < CTE_OUTPUT_MAX)
        {
            gsDriverData.pSignalDef0[len] = pCteInitParams->pSignalDef0[len];
            len++;
        }
        gsDriverData.pSignalDef0[len].outputSignal = CTE_OUTPUT_MAX;
        if (pCteInitParams->pSignalDef1 != NULL)
        {
            len = 0u;
            while (pCteInitParams->pSignalDef1[len].outputSignal < CTE_OUTPUT_MAX)
            {
                gsDriverData.pSignalDef1[len] = pCteInitParams->pSignalDef1[len];
                len++;
            }
            gsDriverData.pSignalDef1[len].outputSignal = CTE_OUTPUT_MAX;
        }
        else
        {
            gsDriverData.pSignalDef1[0].outputSignal = CTE_OUTPUT_MAX;      /* signal an empty table        */
        }
        /* set the driver status to NOT_INITIALIZED         */
        gsDriverData.cteDriverStatus = (uint8) CTE_DRIVER_STATE_INITIALIZED;
    }
    return rez;
}
/*=== Cte_Setup ===========================*/


#if (CTE_START_STOP_USAGE == STD_ON)
/*==================================================================================================*/
/**
 * @brief   Low level start procedure for CTE
 * @details After this call the CTE will start to work, if successful.
 *          The procedure could be unsuccessful only if the driver was not initialized before.
 *          Start after a Stop will use the previous initialization.
 *
 * @return  E_OK/RSDK_SUCCESS = start succeeded
 *          other values      = start failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_Start(void)
{
    Std_ReturnType rez;

    switch ((uint8) gsDriverData.cteDriverStatus)
    {
    case (uint8) CTE_DRIVER_STATE_NOT_INIT:
        CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_INITIALIZED, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
        CTE_HALT_ON_ERROR;
        break;
    case (uint8) CTE_DRIVER_STATE_RUNNING:
        if (CTE_GET_REGISTRY32(&gspCTE->DBG_REG, CTE_DBG_REG_FSM_ST_MASK, CTE_DBG_REG_FSM_ST_SHIFT) == 0u)
        {                   /* the table execution finished         */
            /* set enable bit in according to the operational mode      */
            CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CTE_EN_MASK, CTE_CNTRL1_CTE_EN(0u));
            gsDriverData.cteDriverStatus = (uint8) CTE_DRIVER_STATE_RUNNING;    /* start CTE execution          */
            CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CTE_EN_MASK, CTE_CNTRL1_CTE_EN(1u));
            rez = (Std_ReturnType)E_OK;
        }
        else
        {
            /* CTE is really running, so it must be stopped first       */
            CTE_REPORT_ERROR(RSDK_CTE_DRV_RUNNING, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
            CTE_HALT_ON_ERROR;
        }
        break;
    default:
        gsDriverData.cteDriverStatus = (uint8) CTE_DRIVER_STATE_RUNNING;    /* start CTE execution          */
        CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CTE_EN_MASK, CTE_CNTRL1_CTE_EN(1u));
        rez = (Std_ReturnType)E_OK;
        break;
    }
    return rez;
}
/*=== Cte_Start ===========================*/

/*==================================================================================================*/
/**
 * @brief   Low level stop procedure for CTE
 * @details After this call the CTE will stop to work, if successful.
 *          The procedure could be unsuccessful only if the driver is not working at the call time.
 *
 * @return  E_OK/RSDK_SUCCESS = stop succeeded
 *          other values       = stop failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_Stop(void)
{
    Std_ReturnType rez;

    switch ((uint8) gsDriverData.cteDriverStatus)
    {
    case (uint8) CTE_DRIVER_STATE_NOT_INIT:
        CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_INITIALIZED, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
        CTE_HALT_ON_ERROR;
        break;
    case (uint8) CTE_DRIVER_STATE_INITIALIZED:
        CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_RUNNING, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
        CTE_HALT_ON_ERROR;
        break;
    default:
        /* set enable bit in according to the operational mode      */
        CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CTE_EN_MASK, CTE_CNTRL1_CTE_EN(0u));
        gsDriverData.cteDriverStatus = (uint8) CTE_DRIVER_STATE_INITIALIZED;    /* stop CTE execution       */
        rez = (Std_ReturnType)E_OK;
        break;
    }
    return rez;
}
/*=== Cte_Stop ===========================*/

/*==================================================================================================*/
/**
 * @brief   Low level restart CTE.
 * @details This procedure is a single call for Cte_Stop and a Cte_Start.
 *
 * @param[out]  E_OK/RSDK_SUCCESS = restart succeeded
 *              other values      = restart failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_Restart(void)
{
    Std_ReturnType rez;

    switch ((uint8) gsDriverData.cteDriverStatus)
    {
    case (uint8) CTE_DRIVER_STATE_NOT_INIT:             /* not initialized, nothing to do       */
        CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_INITIALIZED, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
        CTE_HALT_ON_ERROR;
        break;
    default:
        (void)Cte_Stop();                 /* stop first           */
        rez = Cte_Start();                /* start if the CTE was initialized before      */
        break;
    }
    return rez;
}
/*=== Cte_Restart ===========================*/

#endif  /* #if defined (CTE_START_STOP_USAGE == STD_ON)   */

/*==================================================================================================*/
/**
 * @brief   Generate a RFS software signal.
 * @details The procedure can be used only in Slave mode, to reset the time table execution.
 *          The real CTE execution must be triggered by a RCS signal.
 *          The CTE must be started before, or an error will be reported
 *
 * @param[out]  E_OK/RSDK_SUCCESS = call succeeded
 *              other values      = call failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_RfsGenerate(void)
{
    Std_ReturnType rez = (Std_ReturnType)E_OK;

    if (gsDriverData.cteDriverStatus == (uint8) CTE_DRIVER_STATE_NOT_INIT)
    {
        CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_INITIALIZED, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
        CTE_HALT_ON_ERROR;
    }
    else
    {
        if (gsDriverData.cteDriverStatus == (uint8) CTE_DRIVER_STATE_INITIALIZED)
        {
            CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_RUNNING, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
            CTE_HALT_ON_ERROR;
        }
    }
    if (rez == (Std_ReturnType)E_OK)
    {
        if (CTE_GET_REGISTRY32(&gspCTE->DBG_REG, CTE_DBG_REG_FSM_ST_MASK, CTE_DBG_REG_FSM_ST_SHIFT) == 0u)
        {                           /* CTE in HALT      */
            CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_RUNNING, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
            CTE_HALT_ON_ERROR;
            gsDriverData.cteDriverStatus = (uint8) CTE_DRIVER_STATE_INITIALIZED;
            CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CTE_EN_MASK, CTE_CNTRL1_CTE_EN (0u));
        }
        else
        {
            CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_RFS_PGEN_MASK, CTE_CNTRL_RFS_PGEN(1u));
            CTE_SET_REGISTRY32(&gspCTE->CNTRL, CTE_CNTRL_RFS_PGEN_MASK, CTE_CNTRL_RFS_PGEN(0u));
        }
    }
    return rez;
}
/*=== Cte_RfsGenerate ===========================*/

/*==================================================================================================*/
/**
 * @brief   Low level procedure to update only the timing tables.
 * @details The procedure can be used only after a previous successful CTE initialization.
 *          If the CTE is working, it will be stopped and restarted after table changed.
 *          If stopped, it will remains in the same state. It is recommendable to do like this.
 *
 * @param[in]   pTable_           = pointer to the new table(s); first pointer must not be NULL;
                                    if second is NULL, only one table used, else two tables used
 * @param[in]   pLutChecksum      = pointer to a uint64 value, which will receive the final LUT checksum;
 *                                  this value can be checked later using Cte_GetLutChecksum
 * @param[out]  E_OK/RSDK_SUCCESS = initialization succeeded
 *              other values      = initialization failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_UpdateTables(Cte_TimeTableDefType *pTable0, Cte_TimeTableDefType *pTable1,
        uint64 *pLutChecksum)
{
    Std_ReturnType rez = (Std_ReturnType)E_OK;

    if (gsDriverData.cteDriverStatus == (uint8) CTE_DRIVER_STATE_NOT_INIT)
    {
        CTE_REPORT_ERROR(RSDK_CTE_DRV_NOT_INITIALIZED, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
        CTE_HALT_ON_ERROR;
    }
    else
    {
        if (gsDriverData.cteDriverStatus == (uint8) CTE_DRIVER_STATE_RUNNING)
        {
            CTE_REPORT_ERROR(RSDK_CTE_DRV_RUNNING, CTE_E_WRONG_STATE, CTE_SETUP_MODULE_INIT);
            CTE_HALT_ON_ERROR;
        }
        else
        {
            if(pLutChecksum == NULL)
            {
                CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_PARAMS, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
            }
        }
    }
    if (rez == (Std_ReturnType)E_OK)
    {               /* ok till here, check the pointer compatibility        */
        if (pTable0 == NULL)
        {
            CTE_REPORT_ERROR(RSDK_CTE_DRV_NULL_PTR_TABLE0, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
            CTE_HALT_ON_ERROR;
        }
        else
        {
            if (((pTable1 == NULL)              /* table1 was provided at init but is not required now          */
                    && ((uint8) gsDriverData.pSignalDef1[0].outputSignal < (uint8) CTE_OUTPUT_MAX))
                    ||
                    ((pTable1 != NULL)          /* table1 was not provided at init and is required now          */
                            && ((uint8) gsDriverData.pSignalDef1[0].outputSignal >= (uint8) CTE_OUTPUT_MAX)))
            {
                CTE_REPORT_ERROR(RSDK_CTE_DRV_WRG_PTR_TABLE1, CTE_E_PARAM_POINTER, CTE_SETUP_PARAM_CHECK);
                CTE_HALT_ON_ERROR;
            }
        }
    }
    if (rez == (Std_ReturnType)E_OK)
    {
        /* reset the LUT checksum       */
        CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CKSM_RST_MASK, CTE_CNTRL1_CKSM_RST(1u));
        /* enable the checksum computation      */
        CTE_SET_REGISTRY32(&gspCTE->CNTRL1, CTE_CNTRL1_CKSM_RST_MASK, CTE_CNTRL1_CKSM_RST(0u));
        rez = Cte_TimingTableSet(pTable0, gsDriverData.pSignalDef0, (volatile void *)gspCTE->LUT);
        if ((rez == (Std_ReturnType)E_OK) && (pTable1 != NULL))
        {
            rez = Cte_TimingTableSet(pTable1, gsDriverData.pSignalDef1, (volatile void *)&gspCTE->LUT[1]);
        }
        *pLutChecksum = Cte_GetLutChecksum();
    }
    return rez;
}
/*=== Cte_UpdateTables ===========================*/


/*==================================================================================================*/
/**
 * @brief   Get the checksum of the timing LUT.
 * @details This procedure returns the current checksum reported by the hardware, only 40 bits.
 *          The value can be compared to the previous values.
 *
 * @return  LUT checksum
 *
 */
uint64 Cte_GetLutChecksum(void)
{
    uint64 checkSum = (uint64)gspCTE->CKSM_MSB;

    checkSum <<= 32u;
    checkSum += (uint64_t)gspCTE->CKSM_LSB;
    return checkSum;
}
/*=== Cte_GetLutChecksum ===========================*/



#ifdef __cplusplus
}
#endif

