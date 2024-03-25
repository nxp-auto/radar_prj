/*
 * Copyright 2020-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <linux/types.h>
#include <linux/stddef.h>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#include "rsdk_cte_driver_module.h"
#include "rsdk_cte_driver_api.h"
#include "CDD_Cte.h"
#include "rsdk_cte_interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
// flag for debug building
// if uncommented, all DebugMessage will produce effect
// #define DEBUG_VERSION 1

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
static OAL_RPCEvent_t   gsRpcEvents[RSDK_CTE_LX_EVT_MAX] = {0};

/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/
static int32_t CtePlatformRegisterEvents(void);
static void    Callback(rsdkCteIrqDefinition_t events);

/*  DEALING WITH INTERRUPT EVENTS IN LINUX ENVIRONMENT
 *
 *  In Linux environment is possible to have a limited communication between kernel-space 
 *  (where the interrupts are catch) and the user-space (where the interrupt significance is checked). 
 *  To make is simple, in the user-space are sent only few events :
    RSDK_CTE_LX_EVT_TT0_START = 0,      = TimeTable 0 start
    RSDK_CTE_LX_EVT_TT1_START,          = TimeTable 1 start
    RSDK_CTE_LX_EVT_TT0_STOP,           = TimeTable 0 stop
    RSDK_CTE_LX_EVT_TT1_STOP,           = TimeTable 1 stop
    RSDK_CTE_LX_EVT_RFS,                = RFS event
    RSDK_CTE_LX_EVT_RCS,                = RCS event
    RSDK_CTE_LX_EVT_TT_END,             = TimeTable ended execution

*/

/**
 * @brief       False callback for Rx interrupt error.
 * @details     The function is called from Rx interrupt. It will remap this to the user-space defined events.
 *
 */
static void Callback(rsdkCteIrqDefinition_t events)
{
    uint32_t i, mask;
    static int32_t          gsRpcEventsConversion[] = {
        (int32_t)RSDK_CTE_LX_EVT_TT0_START,          // TimeTable 0 start
        (int32_t)RSDK_CTE_LX_EVT_TT1_START,          // TimeTable 1 start
        (int32_t)RSDK_CTE_LX_EVT_TT0_STOP,           // TimeTable 0 stop
        (int32_t)RSDK_CTE_LX_EVT_TT1_STOP,           // TimeTable 1 stop
        -1, -1,
        (int32_t)RSDK_CTE_LX_EVT_RFS,                // RFS event
        (int32_t)RSDK_CTE_LX_EVT_RCS,                // RCS event
        -1,
        (int32_t)RSDK_CTE_LX_EVT_TT_END,             // TimeTable ended execution
    };

    mask = 1u;
    for(i = 0u; i < sizeof(gsRpcEventsConversion); i++)
    {
        if((((uint32_t)events) & mask) != 0u)
        {
            if(gsRpcEventsConversion[i] != -1)
            {
                (void)OAL_RPCTriggerEvent(
                    gsRpcEvents[gsRpcEventsConversion[i]]);  // trigger the event
            }
        }
        mask <<= 1u;
    }
}


/**
 * @brief       Replace the user-space callbacks with the kernel callbacks.
 * @details     The function is called before the low level driver initialization.
 *
 */
static uint32_t CteLinuxKernelInit(rsdkCteLinuxTransfer_t *pParams, uint64_t *pUInt64)
{
    uint32_t            i, j, k, rez;
    Cte_SetupParamsType llInitParams;               // the init params in expected low-level format
    uint8_t             *pBuf = kzalloc(2u * ((CTE_MAX_LARGE_TIME_TABLE_LEN * sizeof(rsdkCteLinuxTtEvents_t)) + 
                                ((uint32_t)RSDK_CTE_OUTPUT_MAX * sizeof(rsdkCteSingleOutputDef_t))), GFP_KERNEL);
    uint8_t				*pTmp = pBuf;
    
    // adapt the parameters to be transferred to kernel
    // general parameters
    llInitParams.cteMode            = *(Cte_ModeDefinitionType*)&pParams->cteMode;
    llInitParams.cteClockFrecq      = pParams->cteClockFrecq;
    llInitParams.repeatCount        = pParams->repeatCount;
    llInitParams.cteIrqEvents       = (Cte_IrqDefinitionType)pParams->cteIrqEvents;
    llInitParams.pCteCallback       = &Callback;                 // use the kernel callback
    // signal definitions
    if(pParams->lenSigDef0 == 0u)
    {
        llInitParams.signalDef0Ptr = NULL;
    }
    else
    {
        llInitParams.signalDef0Ptr    = (Cte_SingleOutputDefType*)pBuf;
        for(i = 0; i < pParams->lenSigDef0; i++)
        {
            llInitParams.signalDef0Ptr[i] = *(Cte_SingleOutputDefType*)&pParams->signalDef0[i];
        }
        llInitParams.signalDef0Ptr[i].outputSignal = RSDK_CTE_OUTPUT_MAX;
        i++;
        pBuf += i * sizeof(Cte_SingleOutputDefType);
    }
    if(pParams->lenSigDef1 == 0u)
    {
        llInitParams.signalDef1Ptr = NULL;
    }
    else
    {
        llInitParams.signalDef1Ptr    = (Cte_SingleOutputDefType*)(void*)pBuf;
        for(i = 0; i < pParams->lenSigDef1; i++)
        {
            llInitParams.signalDef1Ptr[i] = *(Cte_SingleOutputDefType*)&pParams->signalDef1[i];
        }
        llInitParams.signalDef1Ptr[i].outputSignal = RSDK_CTE_OUTPUT_MAX;
        i++;
        pBuf += i * sizeof(rsdkCteSingleOutputDef_t);
    }
    // timing tables
    if(pParams->tableLen0 == 0u)
    {
        llInitParams.timeTable0Ptr = NULL;
    }
    else
    {
        llInitParams.timeTable0Ptr                        = (Cte_TimeTableDefType*)pBuf;
        llInitParams.timeTable0Ptr->tableLength           = pParams->tableLen0;
        llInitParams.timeTable0Ptr->tableTimeExecLimit    = pParams->timeLimT0;
        pBuf += sizeof(rsdkCteTimeTableDef_t);
        llInitParams.timeTable0Ptr->eventsPtr = (Cte_TimingEventType*)(void*)pBuf;
        pBuf += pParams->tableLen0 * sizeof(Cte_TimingEventType);
        for(i = 0; (i < pParams->tableLen0) && (i < CTE_MAX_SMALL_TIME_TABLE_LEN); i++)
        {
            llInitParams.timeTable0Ptr->eventsPtr[i].absTime = pParams->timeTable0[i].evTime;
            llInitParams.timeTable0Ptr->eventsPtr[i].eventActionsPtr = (Cte_ActionType*)pBuf;
            for(j = 0; j < pParams->timeTable0[i].actionsNumber; j++)
            {
                llInitParams.timeTable0Ptr->eventsPtr[i].eventActionsPtr[j] = *(Cte_ActionType*)&pParams->timeTable0[i].actions[j];
            }
            llInitParams.timeTable0Ptr->eventsPtr[i].eventActionsPtr[j].outputSignal = RSDK_CTE_OUTPUT_MAX;
            j++;
            pBuf += j * sizeof(rsdkCteAction_t);
        }
        for(; i < pParams->tableLen0; i++)
        {
            k = i - CTE_MAX_SMALL_TIME_TABLE_LEN;
            llInitParams.timeTable0Ptr->eventsPtr[i].absTime = pParams->timeTable1[k].evTime;
            llInitParams.timeTable0Ptr->eventsPtr[i].eventActionsPtr = (Cte_ActionType*)(void*)pBuf;
            for(j = 0; j < pParams->timeTable1[k].actionsNumber; j++)
            {
                llInitParams.timeTable0Ptr->eventsPtr[i].eventActionsPtr[j] = *(Cte_ActionType*)&pParams->timeTable1[k].actions[j];
            }
            llInitParams.timeTable0Ptr->eventsPtr[i].eventActionsPtr[j].outputSignal = (Cte_OutputType)RSDK_CTE_OUTPUT_MAX;
            j++;
            pBuf += j * sizeof(rsdkCteAction_t);
        }
    }
    // timing tables
    if(pParams->tableLen1 == 0u)
    {
        llInitParams.timeTable1Ptr = NULL;
}
    else
    {
        llInitParams.timeTable1Ptr                        = (Cte_TimeTableDefType*)pBuf;
        llInitParams.timeTable1Ptr->tableLength           = pParams->tableLen1;
        llInitParams.timeTable1Ptr->tableTimeExecLimit    = pParams->timeLimT1;
        pBuf += sizeof(rsdkCteTimeTableDef_t);
        llInitParams.timeTable1Ptr->eventsPtr = (Cte_TimingEventType*)pBuf;
        pBuf += pParams->tableLen1 * sizeof(Cte_TimingEventType);
        for(i = 0; i < pParams->tableLen1; i++)
        {
            llInitParams.timeTable1Ptr->eventsPtr[i].absTime = pParams->timeTable1[i].evTime;
            llInitParams.timeTable1Ptr->eventsPtr[i].eventActionsPtr = (Cte_ActionType*)pBuf;
            for(j = 0; j < pParams->timeTable1[i].actionsNumber; j++)
            {
                llInitParams.timeTable1Ptr->eventsPtr[i].eventActionsPtr[j] = *(Cte_ActionType*)&pParams->timeTable1[i].actions[j];
            }
            llInitParams.timeTable1Ptr->eventsPtr[i].eventActionsPtr[j].outputSignal = RSDK_CTE_OUTPUT_MAX;
            j++;
            pBuf += j * sizeof(Cte_ActionType);
        }
    }
	rez = (uint32_t)Cte_Setup(&llInitParams, pUInt64);
	
	
	kvfree(pTmp);
    return(rez);
}

/**
 * @brief       RemoteProcedureCall server dispatcher.
 * @details     The function is called when the user-space library ask for driver action.
 *
 */
static uint32_t RsdkCteRpcDispatcher(oal_dispatcher_t *dispatcher, uint32_t func, uintptr_t inData, int32_t len)
{
    uint32_t                rez;
    uintptr_t               realTableGap, brickMask;
    Cte_TimeTableDefType    *pTable0, *pTable1;
    void                    *pParams = NULL;
    uint64_t                uInt64Val;

    DebugMessage("RsdkCteRpcDispatcher: func=%d, len=%d\n", func, len);
    // the input data must have at least an integer
    if(len > 0)
    {
        pParams = (void **)inData;  // initialize the pointer to received data
    }

    switch (func)
    {
        case (uint32_t)RSDK_CTE_LX_CTE_INIT:                // CTE initialization call
            if ((uint32_t)len < (sizeof(rsdkCteInitParams_t)))
            {
                rez = (uint32_t)RSDK_CTE_DRV_LX_NOT_ENOUGH_PARAM;  // not enough data
            }
            else
            {
                rez = (uint32_t)CteLinuxKernelInit((rsdkCteLinuxTransfer_t*)pParams, &uInt64Val);  // real unit 
            }
            break;
        case (uint32_t)RSDK_CTE_LX_CTE_STOP:                // CTE stop request
            rez = (uint32_t)Cte_Stop();
            break;
        case (uint32_t)RSDK_CTE_LX_CTE_START:               // CTE start request
            rez = (uint32_t)Cte_Start();
            break;
        case (uint32_t)RSDK_CTE_LX_CTE_RESTART:             // CTE restart request
            rez = (uint32_t)Cte_Restart();
            break;
        case (uint32_t)RSDK_CTE_LX_CTE_RFS_SET:             // CTE software generate RFS request
            rez = (uint32_t)Cte_RfsGenerate();
            break;
        case (uint32_t)RSDK_CTE_LX_CTE_TABLE_UPDATE:        // table(s) update request
            if ((uint32_t)len < ((sizeof(uintptr_t)) * 2u))
            {
                rez = (uint32_t)RSDK_CTE_DRV_LX_NOT_ENOUGH_PARAM;  // not enough data
            }
            else
            {
                // align the table pointers to 4 bites chunks
                brickMask = (uintptr_t)(sizeof(uint32_t) - 1u);
                realTableGap = (sizeof(rsdkCteTimeTableDef_t) + brickMask) & (~brickMask);
                pTable0 = (Cte_TimeTableDefType*)pParams;
                pTable1 = (Cte_TimeTableDefType*)(pParams + realTableGap);                
                if(pTable0->tableTimeExecLimit == RSDK_CTE_WRONG_TABLE_TIME_LENGTH)
                {
                    pTable0 = NULL;
                }
                if(pTable1->tableTimeExecLimit == RSDK_CTE_WRONG_TABLE_TIME_LENGTH)
                {
                    pTable1 = NULL;
                }
                DebugMessage("RsdkCteRpcDispatcher: pTable0=%lx, pTable1=%lx\n", (long)pTable0, (long)pTable1);
                rez = (uint32_t)Cte_UpdateTables(pTable0, pTable1, &uInt64Val);  // real unit initialization
            }
            break;
        case (uint32_t)RSDK_CTE_LX_CTE_REG_EVT:             // events registration request
                rez = (uint32_t)CtePlatformRegisterEvents();  // all defined events are registered at a time
            break;
        case (uint32_t)RSDK_CTE_LX_CTE_EVTMGR_STOP:
            if (gpRsdkCteDevice->registeredEvents != 0u)
            {
                (void)OAL_RPCTriggerEvent(gsRpcEvents[(int32_t)RSDK_CTE_LX_EVT_EVTMGR_STOP]);
                rez = (uint32_t)RSDK_SUCCESS;
            }
            else
            {
                rez = (uint32_t)RSDK_CTE_DRV_LX_WRG_CALL;
            }
            break;
        case (uint32_t)RSDK_CTE_LX_CTE_READ_CHECKSUM:                 // get the TT checksum
            uInt64Val = Cte_GetLutChecksum();
            rez = (uint32_t)OAL_RPCAppendReply(dispatcher, (uint8_t*)&uInt64Val, sizeof(uint64_t));
            break;
        default:
            rez = (uint32_t)RSDK_CTE_DRV_LX_WRG_CALL;       // unknown request
            break;
    }
    return rez;
}

int32_t RsdkCteRpcSrvExit(void)
{
    int32_t rez = 0;
    rez = OAL_RPCCleanup(gpRsdkCteDevice->gspRpcServ);
    if (rez == 0)
    {
        gpRsdkCteDevice->gspRpcServ = NULL;
    }
    return rez;
}

int32_t RsdkCteRpcSrvInit(void)
{
    int32_t rc;

    rc = 0;

    gpRsdkCteDevice->gspRpcServ = OAL_RPCRegister(RSDK_CTE_RPC_CHANNEL_NAME, RsdkCteRpcDispatcher);
    if (gpRsdkCteDevice->gspRpcServ == NULL)
    {
        rc = -1;
    }
    return rc;
}

/**
 * @brief       Register the events necessary to communicate the issues to user-space
 * @details     All necessary events are registered at the same time.
 *
 */
static int32_t CtePlatformRegisterEvents(void)
{
    int32_t i, rez;

    rez = (int32_t)RSDK_SUCCESS;
    if (gpRsdkCteDevice->registeredEvents == 0u)
    {  // record the events registration only if not done before
        for (i = 0; i < (int32_t)RSDK_CTE_LX_EVT_MAX; i++)
        {
            if (OAL_RPCRegisterEvent(gpRsdkCteDevice->gspRpcServ, (uint32_t)i, &gsRpcEvents[(uint32_t)i]) != 0)
            {
                (void)CtePlatformUnregisterEvents();
                rez = (int32_t)RSDK_CTE_DRV_IRQ_REG_FAILED;
                break;
            }
        }
        if (rez == (int32_t)RSDK_SUCCESS)
        {
            gpRsdkCteDevice->registeredEvents = 1;  // record the events registration
        }
    }
    return rez;
}

/**
 * @brief       Unregister the events necessary to communicate the issues to user-space
 * @details     All necessary events are registered at the same time.
 *
 */
int32_t CtePlatformUnregisterEvents(void)
{
    int32_t i;

    for (i = 0; i < (int32_t)RSDK_CTE_LX_EVT_MAX; i++)
    {
        if (gsRpcEvents[i] != NULL)
        {
            (void)OAL_RPCDeregisterEvent(gsRpcEvents[i]);
            gsRpcEvents[i] = NULL;
        }
    }
    gpRsdkCteDevice->registeredEvents = 0;  // events unregistered
    return (int32_t)RSDK_SUCCESS;
}

#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
