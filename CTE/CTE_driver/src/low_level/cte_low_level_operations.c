/*
 * Copyright 2020-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#ifndef linux
    #include <string.h>
    #include <stdint.h>
#else
    #include "rsdk_cte_driver_module.h"
#endif

#include "rsdk_status.h"
#include "cte_low_level_operations.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 ==================================================================================================*/

/*==================================================================================================
 *                                       LOCAL MACROS
 ==================================================================================================*/

/*==================================================================================================
 *                                      LOCAL CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      LOCAL VARIABLES
 ==================================================================================================*/
static cteDriverState_t gsDriverData = { 0u };              // the driver necessary data

/*==================================================================================================
 *                                      GLOBAL CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      GLOBAL VARIABLES
 ==================================================================================================*/
static volatile struct CTE_tag *gspCTE = NULL;                    // the pointer to the CTE registry

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/

/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 ==================================================================================================*/
/**
 * @brief   Interrupt handler for CTE.
 * @details Low level interrupt handler for CTE driver.
 *
 */
void CteIrqHandler(
#ifdef __ZEPHYR__
    const void *pParams
#else
    void
#endif
)
{
#ifdef __ZEPHYR__
    (void)(pParams);
#endif
    uint32_t cteEvents;

    cteEvents = gspCTE->INTSTAT.R;
    gspCTE->INTSTAT.R = cteEvents;          // clear all bits
    cteEvents &= gsDriverData.cteReqEvents;
    gsDriverData.pCteCallback(cteEvents);
}
//=== CteIrqHandler ===========================

//==================================================================================================
/**
 * @brief   Procedure to check an event of the timing table.
 * @details Is checked the total number of defined actions, which must be less than the total signals number
 *
 * @param[in]   pEvent          = pointer to the event
 * @return      RSDK_SUCCESS    = initialization succeeded
 *              other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
static rsdkStatus_t CteTtEventCheck(rsdkCteTimingEvent_t *pEvent)
{
    rsdkStatus_t rez = RSDK_SUCCESS;
    rsdkCteAction_t *action;
    uint32_t n;

    if (pEvent->pEventActions == NULL)
    {
        rez = RSDK_CTE_DRV_NULL_PTR_ACTIONS;        // event with no actions
    }
    else
    {
        action = pEvent->pEventActions;
        n = 0;
        while ((uint8_t) action->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_MAX)
        {
            action++;                               // pass to the next action definition
            n++;                                    // simple action count
        }
        if (n > (uint32_t) RSDK_CTE_OUTPUT_MAX)
        {
            rez = RSDK_CTE_DRV_TOO_MANY_ACTIONS;    // too many actions defined
        }
    }
    return rez;
}
//=== CteTtEventCheck ===========================

//==================================================================================================
/**
 * @brief   Procedure to check the timing table.
 * @details The check is only syntactic, not semantic.
 *
 * @param[in]   pTable          = pointer to the time table
 * @return      RSDK_SUCCESS    = initialization succeeded
 *              other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
static rsdkStatus_t CteTimeTableCheck(rsdkCteTimeTableDef_t *pTable)
{
    uint32_t i;
    rsdkStatus_t rez = RSDK_SUCCESS;

    // check the table length, single table supposed; if double table used, the length will be checked later
    if (pTable->tableLength > CTE_MAX_LARGE_TIME_TABLE_LEN)
    {
        rez = RSDK_CTE_DRV_TABLE_TOO_LONG;          // table has more than 64 events specified
    }
    else
    {
        if (pTable->pEvents == NULL)
        {
            rez = RSDK_CTE_DRV_NULL_PTR_EVENTS;     // the events pointer is NULL
        }
        else
        {
            for (i = 0; i < pTable->tableLength; i++)
            {
                rez = CteTtEventCheck(&pTable->pEvents[i]);  // check each event definition
                if (rez != RSDK_SUCCESS)
                {
                    break;                          // error detected, so stop the loop
                }
            }
        }
    }
    return rez;
}
//=== CteTimeTableCheck ===========================

//==================================================================================================
/**
 * @brief   Procedure to check the signals definitions table.
 * @details Are checked the types for SPT and FLEX outputs.
 *
 * @param[in]   pSignals        = pointer to the signals table
 * @return      RSDK_SUCCESS    = initialization succeeded
 *              other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
static rsdkStatus_t CteSignalDefsCheck(rsdkCteSingleOutputDef_t *pSignals)
{
    uint8_t defSPT;
    rsdkCteSingleOutputDef_t *pwSignals;
    rsdkStatus_t rez = RSDK_SUCCESS;

    pwSignals = pSignals;
    defSPT = 0xffu;            // the signals/outputs not defined
    while (((uint8_t) pwSignals->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_MAX) && (rez == RSDK_SUCCESS))
    {
        if ((uint8_t) pwSignals->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_CTEP_0)
        {                       // SPT event type
            if ((pwSignals->signalType != RSDK_CTE_OUT_LOGIC) && (pwSignals->signalType != RSDK_CTE_OUT_TOGGLE))
            {
                rez = RSDK_CTE_DRV_SIG_OUT_WRG_TYPE;        // not accepted type
                break;
            }
            else
            {
                if (defSPT == 0xffu)
                {               // SPT type not defined yet
                    defSPT = (uint8_t) pwSignals->signalType;
                }
                else
                {
                    rez = (defSPT != (uint8_t) pwSignals->signalType) ? RSDK_CTE_DRV_SIG_OUT_DIF_TYPE : RSDK_SUCCESS;
                }
            }
        }
        if ((uint8_t) pwSignals->outputSignal > (uint8_t) RSDK_CTE_OUTPUT_SPT_RFS)
        {                       // FLEX event type
            if (pwSignals->signalType != RSDK_CTE_OUT_LOGIC)
            {
                rez = RSDK_CTE_DRV_SIG_OUT_WRG_TYPE;        // not accepted type
                break;
            }
        }
        pwSignals++;
    }
    return rez;
}
//=== CteSignalDefsCheck ===========================

//==================================================================================================
/**
 * @brief   Procedure to check the initialization parameters.
 * @details All necessary checks are done, to assure correct initialization flow.
 *          If many error occurs, only one is reported.
 *
 * @param[in]   pCteInitParams  = pointer to the initialization structure
 * @return      RSDK_SUCCESS    = initialization succeeded
 *              other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
static rsdkStatus_t CteInitParamsCheck(const rsdkCteInitParams_t *pCteInitParams)
{
    rsdkStatus_t rez = RSDK_SUCCESS;

    // check the requested input mode
    if ((uint8_t) pCteInitParams->cteMode.workingMode >= (uint8_t) RSDK_CTE_MODE_MAX)
    {
        rez = RSDK_CTE_DRV_WRG_MODE;
    }
    else if (((uint8_t) pCteInitParams->cteMode.workingMode == (uint8_t) RSDK_CTE_SLAVE_CSI2) &&
            ((uint8_t) pCteInitParams->cteMode.cteWorkingParam0.cteCsi2Unit >= (uint8_t) RSDK_CTE_CSI2_UNIT_MAX))
    {
        rez = RSDK_CTE_DRV_WRG_CSI2_UNIT;
    }
    else
    {
        if (((uint8_t) pCteInitParams->cteMode.workingMode == (uint8_t) RSDK_CTE_SLAVE_CSI2) &&
                ((uint8_t) pCteInitParams->cteMode.cteWorkingParam1.cteCsi2Vc >= (uint8_t) RSDK_CTE_CSI2_VC_MAX))
        {
            rez = RSDK_CTE_DRV_WRG_CSI2_VC;
        }
    }
    // check the specified CTE clock frequency
    if ((rez == RSDK_SUCCESS) && (pCteInitParams->cteClockFrecq == 0u))
    {
        rez = RSDK_CTE_DRV_ZERO_FREQ;
    }
    if (rez == RSDK_SUCCESS)
    {
        // check the first time table pointer
        if (pCteInitParams->pTimeTable0 == NULL)
        {
            rez = RSDK_CTE_DRV_NULL_PTR_TABLE0;
        }
        else
        {
            // good table pointer, check the table content
            rez = CteTimeTableCheck(pCteInitParams->pTimeTable0);
            if (rez == RSDK_SUCCESS)
            {
                // check the output signal definitions
                if (pCteInitParams->pSignalDef0 == NULL)
                {
                    rez = RSDK_CTE_DRV_NULL_PTR_SIG_DEF;
                }
                else
                {
                    rez = CteSignalDefsCheck(pCteInitParams->pSignalDef0);
                }
            }
        }
    }
    // check the second time table if specified
    if (rez == RSDK_SUCCESS)
    {
        if (pCteInitParams->pTimeTable1 != NULL)
        {
            // good table pointer, the table 0 has a correct length ?
            if ((pCteInitParams->pTimeTable0->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN) ||
                    (pCteInitParams->pTimeTable1->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN))
            {
                rez = RSDK_CTE_DRV_TABLE_TOO_LONG;
            }
            if (rez == RSDK_SUCCESS)
            {
                if (pCteInitParams->pSignalDef1 == NULL)
                {
                    rez = RSDK_CTE_DRV_NULL_PTR_SIG_DEF;
                }
                else
                {
                    // check the table 2 content
                    rez = CteTimeTableCheck(pCteInitParams->pTimeTable1);
                }
            }
        }
        else
        {       // second table not defined
            if (pCteInitParams->pSignalDef1 != NULL)
            {
                rez = RSDK_CTE_DRV_NOT_NULL_PTR_SIG_DEF;    // misfit TimeTable/Output signal table
            }
        }
    }
    // check the irq logic
    if (((uint32_t) pCteInitParams->cteIrqEvents != 0u) && (pCteInitParams->pCteCallback == NULL))
    {
        rez = RSDK_CTE_DRV_NULL_CALLBACK;
    }
    return rez;
}
//=== CteInitParamsCheck ===========================

//==================================================================================================
/**
 * @brief   Procedure to find the appropriate CTE clock divider.
 * @details The smallest divider which ensure the full table execution is selected
 *
 * @param[in]   pointers to the required time tables, in the correct order
 * @return      The necessary clock divider
 * @pre         The CTE clock frequency must be set before
 *
 */
static uint32_t Cte64BitCounting(uint32_t initialValue, uint32_t mul1, uint32_t mul2, uint32_t div1, uint32_t div2)
{
    uint64_t val = (uint64_t) initialValue;
    val *= (uint64_t) mul1;
    val *= (uint64_t) mul2;
    val /= (uint64_t) div1;
    val /= (uint64_t) div2;
    return ((uint32_t) val);
}
//=== Cte64BitCounting ===========================

//==================================================================================================
/**
 * @brief   Procedure to find the appropriate CTE clock divider.
 * @details The smallest divider which ensure the full table execution is selected
 *
 * @param[in]   pointers to the required time tables, in the correct order
 * @return      The necessary clock divider
 * @pre         The CTE clock frequency must be set before
 *
 */
static uint8_t CteClockDividerGet(rsdkCteTimeTableDef_t *pTimeTable0, rsdkCteTimeTableDef_t *pTimeTable1)
{
    uint8_t i;
    uint8_t clockDivider;           // final clock divider
    uint32_t tableTimeMax;           // max time value inside the time table [ns]
    uint32_t tableTimeMax1;          // max time value inside the time table 1 [ns]
    uint32_t maxDelay;               // max delay between consecutive events
    uint32_t maxDelay1;              // max delay between consecutive events for table 1
    uint32_t largeIntCount;          // special value to get correct uint32_t computing

    tableTimeMax = 0;
    maxDelay = 0;
    // check the first table
    for (i = 0; i < pTimeTable0->tableLength; i++)
    {
        if (tableTimeMax < pTimeTable0->pEvents[i].absTime)
        {
            if (maxDelay < (pTimeTable0->pEvents[i].absTime - tableTimeMax))
            {
                maxDelay = pTimeTable0->pEvents[i].absTime - tableTimeMax;  // the maximum time difference between
                                                                            // two consecutive events
            }
            tableTimeMax = pTimeTable0->pEvents[i].absTime;                 // keep the maximum referred time
        }
        else
        {
            maxDelay = CTE_TOO_BIG_TIME_DELAY;          // incorrect time sequence, so the delay is a very big number
        }
    }
    // check the second table
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
                    maxDelay1 = pTimeTable1->pEvents[i].absTime - tableTimeMax1;  // the maximum time difference between
                                                                                // two consecutive events
                }
                tableTimeMax1 = pTimeTable1->pEvents[i].absTime;                 // keep the maximum referred time
            }
            else
            {
                maxDelay1 = CTE_TOO_BIG_TIME_DELAY;      // incorrect time sequence, so the delay is a very big number
            }
        }
        // analyze the maximum values for table 0 and 1
        if(maxDelay1 > maxDelay)
        {
            maxDelay = maxDelay1;
        }
    }
    largeIntCount = Cte64BitCounting(maxDelay, gsDriverData.cteWorkingFreq, 1u, // remember, maxDelay is in ns
            CTE_1G_FREQUENCY, CTE_MAX_TIME_COUNTER);    // divider is_equal to (maxDelay*CTE_clk_freq)/(1G*MAX_COUNTER)
    largeIntCount++;
    if (largeIntCount > (uint32_t) CTE_CLOCK_DIVIDER_LIMIT)
    {
        clockDivider = CTE_CLOCK_DIVIDER_LIMIT;         // divider limit exceeded
    }
    else
    {
        clockDivider = (uint8_t) largeIntCount;         // the final divider
    }
    return clockDivider;
}
//=== CteClockDividerGet ===========================

//==================================================================================================
/**
 * @brief   Procedure to set the mask for the new state required.
 * @details The value is set as 64 bits value, as for LSB/MSB registry values concatenated
 *
 * @param[in]   pointers to the signal definitions array
 * @param[in]   pointers to the action array
 * @param[in]   pointer to the result
 * @return      RSDK_SUCCESS if successful, error if other
 *
 */
static rsdkStatus_t CteSignalMaskSet(rsdkCteSingleOutputDef_t *pSignalDef, rsdkCteAction_t *action, uint64_t *pVal)
{
    rsdkCteSingleOutputDef_t *pwSignalDef;       // pointer to signal definitions to be used for process
    uint8_t shiftValue;         // the left shift for the final value
    uint64_t reqMask;            // the mask, "initial" and "final"
    rsdkStatus_t rez = RSDK_SUCCESS;

    pwSignalDef = pSignalDef;
    while (pwSignalDef->outputSignal < RSDK_CTE_OUTPUT_MAX)
    {
        if (pwSignalDef->outputSignal == action->outputSignal)       // find the required signal into defined table
        {
            break;
        }
        pwSignalDef++;                              // pass to next definition
    }
    if (pwSignalDef->outputSignal == action->outputSignal)
    {
        // get the mask according to the type/newState
        switch ((uint8_t) pwSignalDef->signalType)
        {
        case (uint8_t) RSDK_CTE_OUT_TOGGLE:
            switch (action->newToggleState)
            {
            case RSDK_CTE_TOGGLE_SET_TO_LOW:
                reqMask = (uint64_t) CTE_TOGGLE_MASK_TO_LOW;
                break;
            case RSDK_CTE_TOGGLE_SET_TO_HIGH:
                reqMask = (uint64_t) CTE_TOGGLE_MASK_TO_HIGH;
                break;
            case RSDK_CTE_TOGGLE_FLIP:
                reqMask = (uint64_t) CTE_TOGGLE_MASK_TOGGLE;
                break;
            case RSDK_CTE_TOGGLE_DONT_CARE:
                reqMask = (uint64_t) CTE_TOGGLE_MASK_UNCHANGED;
                break;
            default:                                    // wrong value, not in enum
                reqMask = 0;
                rez = RSDK_CTE_DRV_SIG_OUT_STATE_WRG;
                break;
            }
            break;
        case (uint8_t) RSDK_CTE_OUT_CLOCK:
            switch (action->newClockState)
            {
            case RSDK_CTE_CLOCK_SET_TO_LOW:
                reqMask = (uint64_t) CTE_CLOCK_MASK_TO_LOW;
                break;
            case RSDK_CTE_CLOCK_ACTIVE_SYNC:
                reqMask = (uint64_t) CTE_CLOCK_MASK_SYNC_RISING;
                break;
            case RSDK_CTE_CLOCK_ACTIVE:
                reqMask = (uint64_t) CTE_CLOCK_MASK_RUNNING;
                break;
            case RSDK_CTE_CLOCK_SET_TO_HIGH:
                reqMask = (uint64_t) CTE_CLOCK_MASK_TO_HIGH;
                break;
            default:                                    // wrong value, not in enum
                reqMask = 0;
                rez = RSDK_CTE_DRV_SIG_OUT_STATE_WRG;
                break;
            }
            break;
        case (uint8_t) RSDK_CTE_OUT_LOGIC:
            switch (action->newLogicState)
            {
            case RSDK_CTE_LOGIC_SET_TO_LOW:
                reqMask = (uint64_t) CTE_LOGIC_MASK_TO_LOW;
                break;
            case RSDK_CTE_LOGIC_SET_TO_HIGH:
                reqMask = (uint64_t) CTE_LOGIC_MASK_TO_HIGH;
                break;
            case RSDK_CTE_LOGIC_SET_TO_HIGH_Z:
                reqMask = (uint64_t) CTE_LOGIC_MASK_TO_HIZ;
                break;
            case RSDK_CTE_LOGIC_UNCHANGED:              // nothing to do
                reqMask = (uint64_t) CTE_LOGIC_MASK_UNCHANGED;
                break;
            default:                                    // wrong value, not in enum
                reqMask = 0;
                rez = RSDK_CTE_DRV_SIG_OUT_STATE_WRG;
                break;
            }
            break;
        default:                                // HiZ is the default in this case
            reqMask = 0;                        // nothing to do
            break;
        }
        if (rez == RSDK_SUCCESS)
        {                       // only if the result is good
            if ((uint8_t) pwSignalDef->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_CTEP_0)
            {                   // SPT events to be processed
                if (reqMask != 0u)
                {
                    reqMask = CTE_SPT0_SIG_MASK;    // SPT events can be
                }
                shiftValue = (uint8_t) pwSignalDef->outputSignal;
            }
            else
            {                   // other signals, RFS/RCS/CTEP0...7
                shiftValue = (uint8_t)(CTE_OUTPUT_MASK_SHIFT_BASE
                        + (((uint8_t) pwSignalDef->outputSignal - (uint8_t) RSDK_CTE_OUTPUT_CTEP_0) << 1u));
            } // if((uint8_t)pwSignalDef->outputSignal < (uint8_t)RSDK_CTE_OUTPUT_CTEP_0)
            *pVal |= reqMask << shiftValue;
        }
    }
    else
    {
        rez = RSDK_CTE_DRV_SIG_NOT_DEF;
    } // if(pwSignalDef->outputSignal == signal->outputSignal)
    return rez;
}
//=== CteSignalMaskset ===========================

//==================================================================================================
/**
 * @brief   Procedure to get the final mask value for time table.
 * @details The result is on 64 bits, for both
 *
 * @param[in]   pointers to the time table
 * @param[in]   pointers to the signal definitions array
 * @param[in]   pointer to the resulted mask
 * @return      RSDK_SUCCESS if successful, error if other
 *
 */
static rsdkStatus_t CteMaskValuesGet(rsdkCteAction_t *pActions, rsdkCteSingleOutputDef_t *pSignalDef, uint64_t *pVal)
{
    rsdkCteAction_t *pwActions;
    rsdkStatus_t rez = RSDK_SUCCESS;

    *pVal = 0u;
    pwActions = pActions;
    while (((uint8_t) pwActions->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_MAX) && (rez == RSDK_SUCCESS))
    {
        switch ((uint8_t) pwActions->outputSignal)
        {
        case (uint8_t) RSDK_CTE_OUTPUT_FLEX_0:
            if (pwActions->newLogicState != RSDK_CTE_LOGIC_SET_TO_LOW)
            {
                *pVal |= CTE_FLEX_SIG_MASK;
            }
            break;
        case (uint8_t) RSDK_CTE_OUTPUT_FLEX_1:
            if (pwActions->newLogicState != RSDK_CTE_LOGIC_SET_TO_LOW)
            {
                *pVal |= (CTE_FLEX_SIG_MASK << 1u);
            }
            break;
        default:                                       // for all remaining signals, RFS/RCS/SPT0...3/CTEP0...7
            rez = CteSignalMaskSet(pSignalDef, pwActions, pVal);
            break;
        }
        pwActions++;                 // move to the next action
    }
    return rez;
}
//=== GetMaskValues ===========================

//==================================================================================================
/**
 * @brief   Procedure to set te timing table.
 * @details One table at a time, depending the start of the table
 *
 * @param[in]   pointers to the required time tables, in the correct order
 * @return      The necessary clock divider
 * @pre         The CTE clock frequency must be set before
 *
 */
static rsdkStatus_t CteTimingTableSet(rsdkCteTimeTableDef_t *pTimeTable, rsdkCteSingleOutputDef_t *pSignalDef,
        volatile CTE_LUT_tag *pLut)
{
    uint64_t longIntMask;            // values for 64 bits computation
    uint32_t i, j, tmpVal, lastTimeTick, work32U;
    volatile CTE_LUT_tag *pwLut;
    rsdkStatus_t rez = RSDK_SUCCESS;

    if(pTimeTable->pEvents == NULL)
    {
        rez = RSDK_CTE_DRV_NULL_PTR_EVENTS;
    }
    else
    {
        pwLut = pLut;
        // for the first event set all output to low
        pwLut->LSB[0].R = 0u;
        pwLut->MSB[0].R = 0u;
        // set all consecutive events to "unchanged" (respectively high for clocks and low for SPT events)
        for (i = 1; i < CTE_MAX_SMALL_TIME_TABLE_LEN; i++)
        {
            pwLut->LSB[i].R = 0u;           // SPT events to low
            pwLut->MSB[i].R = 0x6fffffu;    // FlexTime to 0, all other to "unchanged"
        }
        // set the new values
        i = 0;
        lastTimeTick = 0;
        while ((i < pTimeTable->tableLength) && (i < CTE_MAX_SMALL_TIME_TABLE_LEN))
        {
            if(pTimeTable->pEvents[i].pEventActions == NULL)
            {
                rez = RSDK_CTE_DRV_NULL_PTR_ACTIONS;
                break;
            }
            rez = CteMaskValuesGet(pTimeTable->pEvents[i].pEventActions, pSignalDef, &longIntMask);
            if (rez != RSDK_SUCCESS)
            {
                break;                              // error to get the signals mask, stop here
            }
            tmpVal = Cte64BitCounting(pTimeTable->pEvents[i].absTime, gsDriverData.cteWorkingFreq, 1u,
                    gsDriverData.cteMainClockDivider, CTE_1G_FREQUENCY);
            work32U = tmpVal - lastTimeTick;
            if (work32U == 0u)
            {
                work32U = 1;                        // time table value must not be 0
            }
            lastTimeTick = tmpVal;
            pLut->LSB[i].R = work32U + (uint32_t) longIntMask;
            longIntMask >>= 32u;                    // get the MSB of the mask
            pLut->MSB[i].R = (uint32_t) longIntMask;
            i++;
        }
        if(rez == RSDK_SUCCESS)
        {
            pwLut++;                                  // go to next LUT, if necessary (more than 32 events)
            if (i < pTimeTable->tableLength)
            {           // second table must be used too
                // erase the CTE time table first
                for (i = 0; i < CTE_MAX_SMALL_TIME_TABLE_LEN; i++)
                {
                    pwLut->LSB[i].R = 0u;
                    pwLut->MSB[i].R = 0u;
                }
            }
            j = 0;
            while (i < pTimeTable->tableLength)
            {
                if(pTimeTable->pEvents[i].pEventActions == NULL)
                {
                    rez = RSDK_CTE_DRV_NULL_PTR_ACTIONS;
                    break;
                }
                rez = CteMaskValuesGet(pTimeTable->pEvents[i].pEventActions, pSignalDef, &longIntMask);
                if (rez != RSDK_SUCCESS)
                {
                    break;                              // error to get the signals mask, stop here
                }
                tmpVal = Cte64BitCounting(pTimeTable->pEvents[i].absTime, gsDriverData.cteWorkingFreq, 1u,
                        gsDriverData.cteMainClockDivider, CTE_1G_FREQUENCY);
                work32U = (uint32_t) tmpVal;
                work32U -= lastTimeTick;
                if (work32U == 0u)
                {
                    work32U = 1;                        // time table value must not be 0
                }
                lastTimeTick = tmpVal;
                pwLut->LSB[j].R = work32U + (uint32_t) longIntMask;
                longIntMask >>= 32u;                    // get the MSB of the mask
                pwLut->MSB[j].R = (uint32_t) longIntMask;
                i++;
                j++;
            }
        }
    }
    return rez;
}
//=== CteTimingTableSet ===========================

//==================================================================================================
/**
 * @brief   Procedure to set the outputs.
 * @details One table at a time, depending the start of the table
 *
 * @param[in]   pointers to the output definitions
 * @param[in]   the table index [0,1]
 * @return      RSDK_SUCCESS = success; other = error
 *
 */
static void CteOutputSetup(rsdkCteSingleOutputDef_t *pSignalDef, uint8_t idx)
{
    uint32_t mask;
    rsdkCteSingleOutputDef_t *pwSignalDef;

    pwSignalDef = pSignalDef;
    gspCTE->SIGTYPE0[idx].R = 0u;            // reset all the types to default after reset (HiZ)
    gspCTE->SIGTYPE1[idx].R = 0u;
    while ((uint8_t) pwSignalDef->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_MAX)
    {
        if ((uint8_t) pwSignalDef->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_CTEP_0)
        {                   // SPT events
            if (pwSignalDef->signalType == RSDK_CTE_OUT_TOGGLE)
            {
                gspCTE->SIGTYPE0[idx].B.SPT_EVT = 1;
            }
        }
        else
        {
            if ((uint8_t) pwSignalDef->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_SPT_RCS)
            {               // CTEP events
                mask = ((uint32_t) pwSignalDef->signalType) <<
                        (16u + (uint8_t) pwSignalDef->outputSignal - (uint8_t) RSDK_CTE_OUTPUT_CTEP_0);
                gspCTE->SIGTYPE0[idx].R |= mask;
            }
            else
            {               // RCS/RFS
                if ((uint8_t) pwSignalDef->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_FLEX_0)
                {
                    mask = ((uint32_t) pwSignalDef->signalType) <<
                            (((uint8_t) pwSignalDef->outputSignal - (uint8_t) RSDK_CTE_OUTPUT_SPT_RCS) * 2u);
                    gspCTE->SIGTYPE1[idx].R |= mask;
                }
            }
        }
        pwSignalDef++;
    }
}
//=== CteOutputSetup ===========================

//==================================================================================================
/**
 * @brief   Procedure to set the running mode, continuous one table or toggle.
 * @details The driver permit only three executions :
 *          - one table of max 32 events
 *          - one table of max 64 events
 *          - two tables in toggle mode
 *
 * @param[in]   pointers to the initialization params
 * @return      RSDK_SUCCESS = success; other = error
 *
 */
static void CteRunModeSet(const rsdkCteInitParams_t *pCteInitParams)
{
    uint32_t mask;

    if (pCteInitParams->pTimeTable1 == NULL)
    {                       // only one table defined
        if (pCteInitParams->pTimeTable0->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN)
        {
            mask = 3u;      // "toggle", but with only one table, to be correlated with the timing
        }
        else
        {
            mask = 2u;      // single table, table 0
        }
    }
    else
    {
        mask = 3u;          // toggle with two tables
    }
    gspCTE->CNTRL.B.OPMOD_SL = mask;
}
//=== CteRunModeSet ===========================

//==================================================================================================
/**
 * @brief   Procedure to set the appropriate timing limits.
 * @details The driver permit only three executions :
 *          - one table of max 32 events
 *          - one table of max 64 events
 *          - two tables in toggle mode
 *          and the procedure must complete the correct settings
 *
 * @param[in]   pointers to the initialization params
 * @return      RSDK_SUCCESS = success; other = error
 *
 */
static void CteTableTimeLimitSet(const rsdkCteInitParams_t *pCteInitParams)
{
    uint32_t lutDur0, lutDur1;

    lutDur0 = Cte64BitCounting(pCteInitParams->pTimeTable0->tableTimeExecLimit, gsDriverData.cteWorkingFreq, 1u,
            gsDriverData.cteMainClockDivider, CTE_1G_FREQUENCY);
    if (pCteInitParams->pTimeTable1 == NULL)
    {                       // only one table defined
        if (pCteInitParams->pTimeTable0->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN)
        {
            gspCTE->LUT_DUR.R = 0u;             // the first LUT duration must be 0
            // use the appropriate value for LUTDUR1: table 0 duration (the normal solution) 
            // or the maximum table duration if all specified durations are 0
            if(lutDur0 != 0u)
            {
                gspCTE->LUT_DUR1.R = lutDur0;
        }
        else
        {
                gspCTE->LUT_DUR1.R = 0xffffffffu;
            }
        }
        else
        {
            // single table, table 0
            gspCTE->LUT_DUR.R = lutDur0;
        }
    }
    else
    {
        // two tables usage
        lutDur1 = Cte64BitCounting(pCteInitParams->pTimeTable1->tableTimeExecLimit, gsDriverData.cteWorkingFreq, 1u,
                gsDriverData.cteMainClockDivider, CTE_1G_FREQUENCY);
        // toggle with two tables
        gspCTE->LUT_DUR.R = lutDur0;
        gspCTE->LUT_DUR1.R = lutDur1;
    }
}
//=== CteTableTimeLimitSet ===========================

//==================================================================================================
/**
 * @brief   Procedure to fill the array of requested periods
 * @details Simple output definitions parsing and clock period collect
 *
 * @param[in]   pointer to the initialization params, pointer to period array, filling start index
 * @return      RSDK_SUCCESS = success; other = error
 *
 */
static rsdkStatus_t CtePeriodArrayFill(rsdkCteSingleOutputDef_t *pDef, uint32_t *pArray, uint32_t *first)
{
    uint32_t idx;
    rsdkCteSingleOutputDef_t *pwDef;
    rsdkStatus_t rez = RSDK_SUCCESS;

    pwDef = pDef;
    idx = *first;
    while ((uint8_t) pwDef->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_MAX)
    {
        if (pwDef->signalType == RSDK_CTE_OUT_CLOCK)
        {
            if (pwDef->clockPeriod == 0u)
            {
                rez = RSDK_CTE_DRV_NULL_CLK_PEROD;  // the clock period can't be 0
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
//=== CtePeriodArrayFill ===========================

//==================================================================================================
/**
 * @brief   Procedure to fill the final periods array
 * @details All requested periods are processed and try to use up to 4 clocks.
 *          If not possible, an error is returned.
 *
 * @param[in]   array length, pointer to the initial periods array
 * @return      RSDK_SUCCESS = success; other = error
 *
 */
static rsdkStatus_t CteFinalPeriodProcess(uint32_t reqClocks, uint32_t *allReqPeriods)
{
    uint32_t i, j, k;
    rsdkStatus_t rez = RSDK_SUCCESS;

    gsDriverData.cteClocksPeriods[0] = allReqPeriods[0];    // initialize the resulting period array
    j = 0u;                         // the working final index
    k = 1;                          // number of clocks inside the limits
    for (i = 1; i < reqClocks; i++)  // process the rest of the array
    {
        if (allReqPeriods[i] < (2u * allReqPeriods[j]))
        {
            k++;                    // is possible to have both periods inside 40% error
        }
        else
        {                           // too different values, so step to next clock, closing the one before
            if (k > 1u)
            {                       // more requested clocks were processed before
                // 80% coeficient applied
                allReqPeriods[j] = Cte64BitCounting(allReqPeriods[j], allReqPeriods[i - 1u], 8u, 10u, allReqPeriods[j]);
                k = 1u;
            }
            gsDriverData.cteClocksPeriods[j] = allReqPeriods[j];
            j++;
            if (j >= CTE_INTERNAL_CLOCKS)
            {
                rez = RSDK_CTE_DRV_TOO_MANY_CLOCKS;
                break;
            }
            allReqPeriods[j] = allReqPeriods[i];
        }
    }
    // process the final values
    if (rez == RSDK_SUCCESS)
    {
        if (k > 1u)
        {                       // more requested clocks were processed before
            allReqPeriods[j] = Cte64BitCounting(allReqPeriods[j], allReqPeriods[i - 1u], 8u, 10u, allReqPeriods[j]);
        }
        gsDriverData.cteClocksPeriods[j] = allReqPeriods[j];
        gsDriverData.cteUsedClockDividers = (uint8_t) j;
        gsDriverData.cteUsedClockDividers++;
    }
    return rez;
}

//==================================================================================================
/**
 * @brief   Procedure to define the necessary clock periods.
 * @details The hardware can use up to 4 clock dividers, so up to 4 periods available.
 *          If more clocks required, intermediate periods will be used, to reduce the deviation below 40%.
 *          If it is not possible to have up to 4 clocks and all the required periods to enter 40% deviation,
 *          an error will be returned.
 *
 * @param[in]   pointers to the initialization params
 * @return      RSDK_SUCCESS = success; other = error
 *
 */
static rsdkStatus_t CteClockDividersSet(const rsdkCteInitParams_t *pCteInitParams)
{
    uint32_t allReqPeriods[RSDK_CTE_OUTPUT_MAX];
    uint32_t reqClocks, i, j, k;
    uint32_t computingVal;
    uint8_t finalDivider;
    rsdkStatus_t rez;

    reqClocks = 0u;
    rez = CtePeriodArrayFill(pCteInitParams->pSignalDef0, allReqPeriods, &reqClocks);
    if ((rez == RSDK_SUCCESS) && (pCteInitParams->pSignalDef1 != NULL))
    {
        rez = CtePeriodArrayFill(pCteInitParams->pSignalDef1, allReqPeriods, &reqClocks);
    }
    if ((rez == RSDK_SUCCESS) && (reqClocks != 0u))
    {           // if there are clocks defined
        // sort the clocks
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
        // remove identical clocks
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
        // process further only if more than 4, compress to have
        if (reqClocks > CTE_INTERNAL_CLOCKS)
        {
            rez = CteFinalPeriodProcess(reqClocks, allReqPeriods);
        }
        else
        {
            for (i = 0; i < reqClocks; i++)
            {
                gsDriverData.cteClocksPeriods[i] = allReqPeriods[i];
            }
            gsDriverData.cteUsedClockDividers = (uint8_t) reqClocks;
        }
        if (rez == RSDK_SUCCESS)
        {
            for (i = 0; i < gsDriverData.cteUsedClockDividers; i++)
            {
                computingVal = Cte64BitCounting(gsDriverData.cteClocksPeriods[i], gsDriverData.cteWorkingFreq, 1u,
                        CTE_1G_FREQUENCY, 1u);
                if (computingVal > (uint32_t) CTE_MAX_REQ_CLK_DIVIDER)
                {
                    rez = RSDK_CTE_DRV_CLK_DIVIDER_ERROR;
                    break;
                }
                finalDivider = (uint8_t) computingVal;
                j = 0u;
                k = 1u;
                computingVal = 1u;
                while(finalDivider > (uint8_t)computingVal)
                {
                    j++;
                    computingVal *= 2u;
                    computingVal += (k & 1u);
                    k <<= 2u;
                }
                switch (i)
                {
                case 0u:                    // first clock
                    gspCTE->CNTRL1.B.CLKDIV_1 = j;
                    break;
                case 1u:                    // second clock
                    gspCTE->CNTRL1.B.CLKDIV_2 = j;
                    break;
                case 2u:                    // third clock
                    gspCTE->CNTRL1.B.CLKDIV_3 = j;
                    break;
                default:                    // fourth clock
                    gspCTE->CNTRL1.B.CLKDIV_4 = j;
                    break;
                }
            }
        }
    }
    return rez;
}
//=== CteClockDividersSet ===========================

//==================================================================================================
/**
 * @brief   Procedure to define the necessary clock periods.
 * @details The hardware can use up to 4 clock dividers, so up to 4 periods available.
 *          If more clocks required, intermediate periods will be used, to reduce the deviation below 40%.
 *          If it is not possible to have up to 4 clocks and all the required periods to enter 40% deviation,
 *          an error will be returned.
 *
 * @param[in]   pointers to the initialization params
 * @return      RSDK_SUCCESS = success; other = error
 *
 */
static void CteOutputClockSelect(const rsdkCteInitParams_t *pCteInitParams)
{
    rsdkCteSingleOutputDef_t *pDef;
    uint32_t i;
    uint32_t tstPeriodLow, tstPeriodHigh;

    pDef = pCteInitParams->pSignalDef0;
    while ((uint8_t) pDef->outputSignal < (uint8_t) RSDK_CTE_OUTPUT_MAX)
    {
        if (pDef->signalType == RSDK_CTE_OUT_CLOCK)
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
            i <<= ((uint8_t) pDef->outputSignal - (uint8_t) RSDK_CTE_OUTPUT_CTEP_0) * 2u;
            gspCTE->CLKSEL.R |= i;
        }
        pDef++;
    }
}
//=== CteOutputClockSelect ===========================

//==================================================================================================
/**
 * @brief   Procedure to define the necessary clock periods.
 * @details The hardware can use up to 4 clock dividers, so up to 4 periods available.
 *          If more clocks required, intermediate periods will be used, to reduce the deviation below 40%.
 *          If it is not possible to have up to 4 clocks and all the required periods to enter 40% deviation,
 *          an error will be returned.
 *
 * @param[in]   pointers to the initialization params
 * @return      RSDK_SUCCESS = success; other = error
 *
 */
static rsdkStatus_t CteIrqInit(const rsdkCteInitParams_t *pCteInitParams)
{
    rsdkStatus_t rez = RSDK_SUCCESS;

    gspCTE->INTEN.R = 0u;       // mask all irq sources
#ifndef linux               // for Linux, the irq are registered in other place
    if (RsdkGlueIrqHandlerRegister(CteIrqHandler, CTE_IRQ_NUMBER,
            pCteInitParams->irqExecCore, pCteInitParams->irqPriority) != IRQ_REGISTER_SUCCESS)
    {
        rez = RSDK_CTE_DRV_IRQ_REG_FAILED;
    }
#endif
    if(((uint32_t)pCteInitParams->cteIrqEvents != 0u)
#ifndef linux        
        && (rez == RSDK_SUCCESS)
#endif
        )
    {                       // events callback requested
        gspCTE->INTEN.R = (uint32_t) pCteInitParams->cteIrqEvents;
    }
    return rez;
}
//=== CteIrqInit ===========================

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 ==================================================================================================*/
/**
 * @brief   Low level initialization procedure for CTE driver
 * @details If called, the CTE function is stopped.
 *          After initialization the CTE is not started, a specific RsdkCteStart call must be used for this.
 *          The operation can be done at any moment; if the CTE is working, it will be stopped.
 *
 * @param[in]   pCteInitParams  = pointer to the initialization structure
 * @param[in]   pLutChecksum    = pointer to a uint64_t value, which will receive the final LUT checksum;
 *                                  this value can be checked later using RsdkCteGetLutChecksum
 * @return      RSDK_SUCCESS    = initialization succeeded
 *              other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleInit(const rsdkCteInitParams_t *pCteInitParams, uint64_t *pLutChecksum)
{
    rsdkStatus_t rez;
    uint8_t cteClockDivider;                           // the clock divider
    uint32_t     len;
#if defined(S32R45) && defined(linux)
    uint32_t    mask;
#endif

    // set the driver status to NOT_INITIALIZED
    gsDriverData.cteDriverStatus = (uint8_t) CTE_DRIVER_STATE_NOT_INIT;
    // check the initialization parameters
    if ((pCteInitParams == NULL) || (pLutChecksum == NULL))
    {
        rez = RSDK_CTE_DRV_NULL_PTR_PARAMS;
    }
    else
    {
        rez = CteInitParamsCheck(pCteInitParams);
    }
    if (gspCTE == NULL)
    {
#ifndef linux
        gspCTE = &CTE;
#else
        if(gpRsdkCteDevice != NULL)
        {
            gspCTE = (volatile struct CTE_tag*)(volatile void*)gpRsdkCteDevice->pMemMapVirtAddr;
        }
#endif
        if (gspCTE == NULL)
        {
            rez = RSDK_CTE_DRV_NOT_INITIALIZED;
        }
    }
    if (rez == RSDK_SUCCESS)
    {
        // initialize the necessary data
        gsDriverData.cteWorkingFreq = pCteInitParams->cteClockFrecq;  // keep the working frequncy for further computing
        cteClockDivider = CteClockDividerGet(pCteInitParams->pTimeTable0, pCteInitParams->pTimeTable1);
        if (cteClockDivider >= CTE_CLOCK_DIVIDER_LIMIT)
        {
            rez = RSDK_CTE_DRV_CLK_DIVIDER_ERROR;
        }
        else
        {
            // reset the CTE
            gspCTE->CNTRL.B.CTE_RST = 1u;          // set the bit to reset CTE
            gsDriverData.cteMainClockDivider = cteClockDivider; // keep the divider; simple delay to get the reset
            gspCTE->CNTRL.B.CTE_RST = 0u;          // reset the bit to be able to use the registry
            gspCTE->CNTRL1.B.CHKSM_MD = 1u;        // compute the LUT checksum, including the MSBit

            // CTE specific initialization, according S32R45 RM, Rev.1
            //1. Configure time instances and corresponding signal states in the timing table register LUT_LSB_0.
            //2. Configure remaining signal states in the timing table register LUT_MSB_0.
            rez = CteTimingTableSet(pCteInitParams->pTimeTable0, pCteInitParams->pSignalDef0, gspCTE->LUT);
            //3. Configure time instances and corresponding signal states in the timing table register LUT_LSB_1.
            //4. Configure remaining signal states in the timing table register LUT_MSB_1.
            if ((rez == RSDK_SUCCESS) && (pCteInitParams->pTimeTable1 != NULL))
            {
                rez = CteTimingTableSet(pCteInitParams->pTimeTable1, pCteInitParams->pSignalDef1, &gspCTE->LUT[1]);
            }
            *pLutChecksum = CtePlatformModuleGetLutChecksum();
            //5. Configure signal types in signal type registers CTE_SIGTYPE0/1.
            if (rez == RSDK_SUCCESS)
            {
                CteOutputSetup(pCteInitParams->pSignalDef0, 0);
                if (pCteInitParams->pSignalDef1 != NULL)
                {
                    CteOutputSetup(pCteInitParams->pSignalDef1, 1);
                }
                if(pCteInitParams->pTimeTable0->tableLength > CTE_MAX_SMALL_TIME_TABLE_LEN)
                {
                    CteOutputSetup(pCteInitParams->pSignalDef0, 1);
                }
            }
            if (rez == RSDK_SUCCESS)
            {
                //6. Configure MA_SL_ST bit in control register CTE_CNTRL.
                if (pCteInitParams->cteMode.workingMode == RSDK_CTE_MASTER)
                {               // Master mode used
                    gspCTE->CNTRL.B.MA_SL_ST = 0u;                  // normally the bit must be already 0 after reset
                }
                else
                {
                    gspCTE->CNTRL.B.MA_SL_ST = 1u;                  // set bit for Slave mode
                    // set the appropriate registry values for the Slave mode
#ifdef S32R45
#ifdef linux
					mask = (uint32_t)pCteInitParams->cteMode.workingMode - (uint32_t)RSDK_CTE_SLAVE_EXTERNAL;
                    mask += (uint32_t)pCteInitParams->cteMode.cteWorkingParam1.cteCsi2Vc << 1u;
                    mask += (uint32_t)pCteInitParams->cteMode.cteWorkingParam0.cteCsi2Unit << 3u;
                    ((volatile struct SRC_1_tag*)(volatile void*)gpRsdkCteDevice->pSrc_1)->CTE_CTRL_REG.R = mask;
#else
                    SRC_1.CTE_CTRL_REG.B.IN_CTE = (uint32_t)pCteInitParams->cteMode.workingMode -
                    (uint32_t)RSDK_CTE_SLAVE_EXTERNAL;
                    SRC_1.CTE_CTRL_REG.B.MIPICSI2_ID = (uint32_t)pCteInitParams->cteMode.cteWorkingParam0.cteCsi2Unit;
                    SRC_1.CTE_CTRL_REG.B.VC_ID = (uint32_t)pCteInitParams->cteMode.cteWorkingParam1.cteCsi2Vc;
#endif // #ifdef linux








#endif
                    gspCTE->CNTRL.B.RFS_DLY = (uint32_t) pCteInitParams->cteMode.cteWorkingParam0.cteInternalRfsDelay;
                    gspCTE->CNTRL.B.RCS_DLY = (uint32_t) pCteInitParams->cteMode.cteWorkingParam1.cteInternalRcsDelay;
                }
                //7. Configure REP_CNT value in control register CTE_CNTRL.
                gspCTE->CNTRL.B.REP_CNT = (uint32_t) pCteInitParams->repeatCount;
                //8. Configure OPMOD_SL bits in control register CTE_CNTRL to define continuous run mode or toggle mode.
                CteRunModeSet(pCteInitParams);
                //9. Configure the duration counter for TT0 and TT1 execution in registers CTE_LUT_DUR and CTE_LUT_DUR1 respectively.
                CteTableTimeLimitSet(pCteInitParams);
                //10. Configure timemode bit in CTE_CNTRL1 to define absolute/relative mode of timing table execution.
                gspCTE->CNTRL1.B.TIMEMODE = 0u;     // only relative timing used
                //11. Configure CTECK_DV in CTE_CNTRL1 to define the CTE data path clock.
                gspCTE->CNTRL1.B.CTECK_DV = gsDriverData.cteMainClockDivider;
                //12. Configure CLKDIV_1-CLKDIV_4 in CTE_CNTRL1 to define clocks for external signals.
                rez = CteClockDividersSet(pCteInitParams);
            }
            //13. Configure CLK_SEL0-CLK_SEL9 in clock select register CTE_CLKSEL to select the clock divider for the external signals.
            if (rez == RSDK_SUCCESS)
            {
                CteOutputClockSelect(pCteInitParams);
            }
            // Other necessary setup : IRQ
            if (rez == RSDK_SUCCESS)
            {
                rez = CteIrqInit(pCteInitParams);
                gsDriverData.cteReqEvents = (uint32_t) pCteInitParams->cteIrqEvents;
                gsDriverData.pCteCallback = pCteInitParams->pCteCallback;
            }
            //14. Enable CTE by setting CTE_EN bit in CTE_CNTRL1.
            // this step will be done in a separate procedure
        }
    }
    if (rez == RSDK_SUCCESS)
    {
        // copy the outputs setup
        len = 0;
        while (pCteInitParams->pSignalDef0[len].outputSignal < RSDK_CTE_OUTPUT_MAX)
        {
            gsDriverData.pSignalDef0[len] = pCteInitParams->pSignalDef0[len];
            len++;
        }
        gsDriverData.pSignalDef0[len].outputSignal = RSDK_CTE_OUTPUT_MAX;
        if (pCteInitParams->pSignalDef1 != NULL)
        {
            len = 0;
            while (pCteInitParams->pSignalDef1[len].outputSignal < RSDK_CTE_OUTPUT_MAX)
            {
                gsDriverData.pSignalDef1[len] = pCteInitParams->pSignalDef1[len];
                len++;
            }
            gsDriverData.pSignalDef1[len].outputSignal = RSDK_CTE_OUTPUT_MAX;
        }
        else
        {
            gsDriverData.pSignalDef1[0].outputSignal = RSDK_CTE_OUTPUT_MAX;    // signal an empty table
        }
        // set the driver status to NOT_INITIALIZED
        gsDriverData.cteDriverStatus = (uint8_t) CTE_DRIVER_STATE_INITIALIZED;
    }
    return rez;
}
//=== CtePlatformModuleInit ===========================

//==================================================================================================
/**
 * @brief   Low level start procedure for CTE
 * @details After this call the CTE will start to work, if successful.
 *          The procedure could be unsuccessful only if the driver was not initialized before.
 *          Start after a Stop will use the previous initialization.
 *
 * @return  RSDK_SUCCESS    = start succeeded
 *          other values    = start failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleStart(void)
{
    rsdkStatus_t rez;

    switch ((uint8_t) gsDriverData.cteDriverStatus)
    {
    case (uint8_t) CTE_DRIVER_STATE_NOT_INIT:
        rez = RSDK_CTE_DRV_NOT_INITIALIZED;
        break;
    case (uint8_t) CTE_DRIVER_STATE_RUNNING:
        if (gspCTE->DBG_REG.B.FSM_ST == 0u)
        {                   // the table execution finished
            gspCTE->CNTRL1.B.CTE_EN = 0u;   // set enable bit in according to the operational mode
            gsDriverData.cteDriverStatus = (uint8_t) CTE_DRIVER_STATE_RUNNING;    // start CTE execution
            gspCTE->CNTRL1.B.CTE_EN = 1u;
            rez = RSDK_SUCCESS;
        }
        else
        {
            rez = RSDK_CTE_DRV_RUNNING; // CTE is really running, so it must be stopped first
        }
        break;
    default:
        gsDriverData.cteDriverStatus = (uint8_t) CTE_DRIVER_STATE_RUNNING;    // start CTE execution
        gspCTE->CNTRL1.B.CTE_EN = 1u;
        rez = RSDK_SUCCESS;
        break;
    }
    return rez;
}
//=== CtePlatformModuleStart ===========================

//==================================================================================================
/**
 * @brief   Low level stop procedure for CTE
 * @details After this call the CTE will stop to work, if successful.
 *          The procedure could be unsuccessful only if the driver is not working at the call time.
 *
 * @return  RSDK_SUCCESS    = stop succeeded
 *          other values    = stop failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleStop(void)
{
    rsdkStatus_t rez;

    switch ((uint8_t) gsDriverData.cteDriverStatus)
    {
    case (uint8_t) CTE_DRIVER_STATE_NOT_INIT:
        rez = RSDK_CTE_DRV_NOT_INITIALIZED;
        break;
    case (uint8_t) CTE_DRIVER_STATE_INITIALIZED:
        rez = RSDK_CTE_DRV_NOT_RUNNING; // CTE is not running, so it must be started first
        break;
    default:
        gspCTE->CNTRL1.B.CTE_EN = 0u;   // set enable bit in according to the operational mode
        gsDriverData.cteDriverStatus = (uint8_t) CTE_DRIVER_STATE_INITIALIZED;    // stop CTE execution
        rez = RSDK_SUCCESS;
        break;
    }
    return rez;
}
//=== CtePlatformModuleStop ===========================

//==================================================================================================
/**
 * @brief   Low level restart CTE.
 * @details This procedure is a single call for RsdkCteStop and a RsdkCteStart.
 *
 * @param[out]  RSDK_SUCCESS    = restart succeeded
 *              other values    = restart failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleRestart(void)
{
    rsdkStatus_t rez;

    rez = CtePlatformModuleStop();              // stop first
    if (rez != RSDK_CTE_DRV_NOT_INITIALIZED)
    {
        rez = CtePlatformModuleStart();         // start if the CTE was initialized before
    }
    return rez;
}
//=== CtePlatformModuleRestart ===========================

//==================================================================================================
/**
 * @brief   Generate a RFS software signal.
 * @details The procedure can be used only in Slave mode, to reset the time table execution.
 *          The real CTE execution must be triggered by a RCS signal.
 *          The CTE must be started before, or an error will be reported
 *
 * @param[out]  RSDK_SUCCESS    = call succeeded
 *              other values    = call failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleRfsGenerate(void)
{
    rsdkStatus_t rez = RSDK_SUCCESS;

    if (gsDriverData.cteDriverStatus == (uint8_t) CTE_DRIVER_STATE_NOT_INIT)
    {
        rez = RSDK_CTE_DRV_NOT_INITIALIZED;
    }
    else
    {
        if (gsDriverData.cteDriverStatus == (uint8_t) CTE_DRIVER_STATE_INITIALIZED)
        {
            rez = RSDK_CTE_DRV_NOT_RUNNING;
        }
    }
    if (rez == RSDK_SUCCESS)
    {
        if (gspCTE->DBG_REG.B.FSM_ST == 0u)
        {                           // CTE in HALT
            rez = RSDK_CTE_DRV_NOT_RUNNING;
            gsDriverData.cteDriverStatus = (uint8_t) CTE_DRIVER_STATE_INITIALIZED;
            gspCTE->CNTRL1.B.CTE_EN = 0u;
        }
        else
        {
            gspCTE->CNTRL.B.RFS_PGEN = 1u;
            gspCTE->CNTRL.B.RFS_PGEN = 0u;
        }
    }
    return rez;
}
//=== CtePlatformModuleRfsGenerate ===========================

//==================================================================================================
/**
 * @brief   Low level procedure to update only the timing tables.
 * @details The procedure can be used only after a previous successful CTE initialization.
 *          If the CTE is working, it will be stopped and restarted after table changed.
 *          If stopped, it will remains in the same state. It is recommendable to do like this.
 *
 * @param[in]   pTable_         = pointer to the new table(s); first pointer must not be NULL;
                                    if second is NULL, only one table used, else two tables used
 * @param[in]   pLutChecksum    = pointer to a uint64_t value, which will receive the final LUT checksum;
 *                                  this value can be checked later using RsdkCteGetLutChecksum
 * @param[out]  RSDK_SUCCESS    = initialization succeeded
 *              other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleUpdateTables(rsdkCteTimeTableDef_t *pTable0, rsdkCteTimeTableDef_t *pTable1,
        uint64_t *pLutChecksum)
{
    rsdkStatus_t rez = RSDK_SUCCESS;

    if (gsDriverData.cteDriverStatus == (uint8_t) CTE_DRIVER_STATE_NOT_INIT)
    {
        rez = RSDK_CTE_DRV_NOT_INITIALIZED;
    }
    else
    {
        if (gsDriverData.cteDriverStatus == (uint8_t) CTE_DRIVER_STATE_RUNNING)
        {
            rez = RSDK_CTE_DRV_RUNNING;
        }
        else
        {
            if(pLutChecksum == NULL)
            {
                rez = RSDK_CTE_DRV_NULL_PTR_PARAMS;
            }
        }
    }
    if (rez == RSDK_SUCCESS)
    {               // ok till here, check the pointer compatibility
        if (pTable0 == NULL)
        {
            rez = RSDK_CTE_DRV_NULL_PTR_TABLE0;
        }
        else
        {
            if (((pTable1 == NULL)              // table1 was provided at init but is not required now
                    && ((uint8_t) gsDriverData.pSignalDef1[0].outputSignal < (uint8_t) RSDK_CTE_OUTPUT_MAX))
                    ||
                    ((pTable1 != NULL)          // table1 was not provided at init and is required now
                            && ((uint8_t) gsDriverData.pSignalDef1[0].outputSignal >= (uint8_t) RSDK_CTE_OUTPUT_MAX)))
            {
                rez = RSDK_CTE_DRV_WRG_PTR_TABLE1;      
            }
        }
    }
    if (rez == RSDK_SUCCESS)
    {
        gspCTE->CNTRL1.B.CKSM_RST = 1u;                 // reset the LUT checksum
        gspCTE->CNTRL1.B.CKSM_RST = 0u;                 // enable the checksum computation
        rez = CteTimingTableSet(pTable0, gsDriverData.pSignalDef0, gspCTE->LUT);
        if ((rez == RSDK_SUCCESS) && (pTable1 != NULL))
        {
            rez = CteTimingTableSet(pTable1, gsDriverData.pSignalDef1, &gspCTE->LUT[1]);
        }
        *pLutChecksum = CtePlatformModuleGetLutChecksum();
    }
    return rez;
}
//=== CtePlatformModuleTimeTablesUpdates ===========================


//==================================================================================================
/**
 * @brief   Get the checksum of the timing LUT.
 * @details This procedure returns the current checksum reported by the hardware, only 40 bits.
 *          The value can be compared to the previous values.
 *
 * @return  LUT checksum
 *
 */
uint64_t CtePlatformModuleGetLutChecksum(void)
{
    uint64_t checkSum = (uint64_t)gspCTE->CKSM_MSB.R;
    
    checkSum <<= 32u;
    checkSum += (uint64_t)gspCTE->CKSM_LSB.R;
    return checkSum;
}
//=== CtePlatformModuleGetLutChecksum ===========================



#ifdef __cplusplus
}
#endif

/******************************************************************************
 * EOF
 ******************************************************************************/
