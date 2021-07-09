/*
 * Copyright 2019-2020 NXP
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

#include "rsdk_csi2_driver_module.h"
#include "rsdk_csi2_driver_api.h"
#include "csi2_driver_platform_specific.h"
#include "rsdk_csi2_interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
static char eServerName[RSDK_MIPICSI2_INTERFACES][sizeof(RSDK_CSI2_RPC_ERROR) + 3];

/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/
static void    Callback1(rsdkCsi2Report_t *pErrors);
static void    Callback2(rsdkCsi2Report_t *pErrors);
static void    Callback3(rsdkCsi2Report_t *pErrors);
static void    RsdkCsi2RpcErrExit(int32_t unitId);

static inline rsdkCsi2VirtChnlId_t GetVirtChnlId(uint32_t chnl)
{
    return *(rsdkCsi2VirtChnlId_t *)(void *)&chnl;
}

/*  DEALING WITH INTERRUPT ERRORS IN LINUX ENVIRONMENT
 *
 *  In Linux environment is possible to have a limited communication between kernel-space 
 *  (where the interrupts are catch) and the user-space (where the interrupt significance is checked). 
 *  To make is simple, in the user-space are sent only few events :
 *      RSDK_CSI2_LX_EVT_INFRM_GOOD_DATA    => an event occurred, but data not affected
 *      RSDK_CSI2_LX_EVT_ERR_OUT_FRAME,     => event outside data reception, so no data affected
 *      RSDK_CSI2_LX_EVT_INFRM_BAD_DATA,    => data error (ECC2, CRC, LINE_LEN, LINE_COUNT, frame data error) 
 *      RSDK_CSI2_LX_EVT_FRAME_START,       => frame start event
 *      RSDK_CSI2_LX_EVT_FRAME_END,         => frame end event
 *      RSDK_CSI2_LX_EVT_LINEDONE,          => line end event (as required)
 *      RSDK_CSI2_LX_EVT_EVTMGR_STOP,       => event manager stopped (in kernel)

*/

/*******************************************************************************/
/**
 * @brief       Function to save the reported errors to pipe.
 * @details     The function is called at the end of a callback in kernel space
 *
 */
 static void RecordErrorAndWakeup(uint32_t irqId, rsdkCsi2Report_t *pErrors)
 {
    uint32_t unitId;
    uint8_t  pWrite, pRead;
    
    if(irqId >= RSDK_CSI2_ERROR_TO_EXIT)
    {
        unitId = irqId ^ RSDK_CSI2_ERROR_TO_EXIT;
    }
    else
    {
        unitId = pErrors->unitId;
    }
    pWrite = gpRsdkCsi2Device[unitId]->irqWriteP;
    gpRsdkCsi2Device[unitId]->irqErrorPipe[pWrite].errType = irqId;
    if(pErrors != NULL)
    {
        gpRsdkCsi2Device[unitId]->irqErrorPipe[pWrite].errReport = *pErrors;
    }
    pWrite = (pWrite + 1) % RSDK_CSI2_ERROR_QUEUE_LEN;
    pRead = gpRsdkCsi2Device[unitId]->irqReadP;
    // only if the signal to stop is not set
    if(pWrite == pRead)
    {       // the oldest error was overwritten, move the read pointer too
        gpRsdkCsi2Device[unitId]->irqReadP = (pRead + 1) % RSDK_CSI2_ERROR_QUEUE_LEN;
        (void)pr_err("RecordErrorAndWakeup: CSI2 error lost - unit %d.\n", unitId);
    }
    gpRsdkCsi2Device[unitId]->irqWriteP = pWrite;
    (void)OAL_WakeUpInterruptible(&(gpRsdkCsi2Device[unitId]->irqWaitQ));
 }

/*******************************************************************************/
/**
 * @brief       False callback for Rx interrupt error.
 * @details     The function is called from Rx interrupt. It will remap this to the user-space defined events.
 *
 */
static void Callback1(rsdkCsi2Report_t *pErrors)
{
    // put the error into the pipe
    RecordErrorAndWakeup(RSDK_CSI2_RX_ERR_IRQ_ID, pErrors);
}

/*******************************************************************************/
/**
 * @brief       False callback for data interrupt error.
 * @details     The function is called from path error interrupt. 
 *              It will remap this to the user-space defined events.
 *
 */
static void Callback2(rsdkCsi2Report_t *pErrors)
{
    // put the error into the pipe
    RecordErrorAndWakeup(RSDK_CSI2_PATH_ERR_IRQ_ID, pErrors);
}

/*******************************************************************************/
/**
 * @brief       False callback for events interrupt.
 * @details     The function is called from events interrupt. It will remap this to the user-space defined events.
 *
 */
static void Callback3(rsdkCsi2Report_t *pErrors)
{
    // put the error into the pipe
    RecordErrorAndWakeup(RSDK_CSI2_EVENTS_IRQ_ID, pErrors);
}

/*******************************************************************************/
/**
 * @brief       Replace the user-space callbacks with the kernel callbacks.
 * @details     The function is called before the low level driver initialization.
 *
 */
static void UpdateCallbacks(rsdkCsi2InitParams_t *pParam)
{
    pParam->pCallback[RSDK_CSI2_RX_ERR_IRQ_ID] = (rsdkCsi2IsrCb_t)Callback1;
    pParam->pCallback[RSDK_CSI2_PATH_ERR_IRQ_ID] = (rsdkCsi2IsrCb_t)Callback2;
    pParam->pCallback[RSDK_CSI2_EVENTS_IRQ_ID] = (rsdkCsi2IsrCb_t)Callback3;
    pParam->pCallback[RSDK_CSI2_TX_ERR_IRQ_ID] = (rsdkCsi2IsrCb_t)Callback3;
}

/*******************************************************************************/
/**
 * @brief       RemoteProcedureCall server dispatcher for normal work.
 * @details     The function is called when the user-space library ask for driver action.
 *
 */
static uint32_t RsdkCsi2RpcDispatcherW(oal_dispatcher_t *d, uint32_t func, uintptr_t in, int32_t len)
{
    uint32_t             rez;
    int32_t             *pUnitId;
    void                *pSecondParam;
    rsdkCsi2UnitId_t     unitId;
    rsdkCsi2VirtChnlId_t vcId;

    // the input data must have at least an integer
    if ((uint32_t)len < sizeof(int32_t))
    {
        rez = (uint32_t)RSDK_CSI2_DRV_NULL_PARAM_PTR;
    }
    else
    {
        pUnitId = (int32_t *)in;  // initialize the pointer to received data
        unitId = *(rsdkCsi2UnitId_t *)(void *)pUnitId;

        switch (func)
        {
            case (uint32_t)RSDK_CSI2_LX_CALL_INIT:  // unit initialization call
                if ((uint32_t)len < (sizeof(int32_t) + sizeof(rsdkCsi2InitParams_t)))
                {
                    rez = (uint32_t)RSDK_CSI2_DRV_NULL_PARAM_PTR;  // not enough data
                }
                else
                {
                    pSecondParam = (void *)(((char *)(in)) + sizeof(int32_t));  // the pointer to initialization data
                    UpdateCallbacks(pSecondParam);      // switch the irq callbacks to kernel space
                    rez = (uint32_t)Csi2PlatformModuleInit(unitId, pSecondParam);  // real unit initialization
                }
                break;
            case (uint32_t)RSDK_CSI2_LX_RX_STOP:  // Rx unit stop request
                rez = (uint32_t)Csi2PlatformRxStop(unitId);
                break;
            case (uint32_t)RSDK_CSI2_LX_RX_START:  // Rx unit start request
                rez = (uint32_t)Csi2PlatformRxStart(unitId);
                break;
            case (uint32_t)RSDK_CSI2_LX_POWER_OFF:  // power-off unit request
                RsdkCsi2RpcErrExit(unitId);         // close the request for error descriptor
                rez = (uint32_t)Csi2PlatformPowerOff(unitId);
                break;
            case (uint32_t)RSDK_CSI2_LX_POWER_ON:  // power-on unit request
                rez = (uint32_t)Csi2PlatformPowerOn(unitId);
                break;
            case (uint32_t)RSDK_CSI2_LX_GET_IFACE:  // interface status request
                rez = (uint32_t)Csi2PlatformGetInterfaceStatus(unitId);
                break;
            case (uint32_t)RSDK_CSI2_LX_GET_LANE:  // lane status request
                if ((uint32_t)len < (2u * sizeof(int32_t)))
                {
                    rez = (uint32_t)RSDK_CSI2_DRV_NULL_PARAM_PTR;  // not enough data received
                }
                else
                {
                    rez = (uint32_t)Csi2PlatformGetLaneStatus(unitId, (uint32_t)pUnitId[1]);
                }
                break;
            case (uint32_t)RSDK_CSI2_LX_GET_CHIRP_LEN:  // real buffer chirp length calculation request
                if ((uint32_t)len < (4u * sizeof(int32_t)))
                {
                    rez = (uint32_t)RSDK_CSI2_DRV_NULL_PARAM_PTR;  // not enough data received
                }
                else
                {
                    rez = (uint32_t)Csi2PlatformGetBufferRealLineLen(
                        *(rsdkCsi2DataStreamType_t *)(void *)pUnitId, (const uint32_t)pUnitId[1],
                        (const uint32_t)pUnitId[2], (const uint8_t)pUnitId[3]);
                }
                break;
            case (uint32_t)RSDK_CSI2_LX_GET_FRAMES:  // number of frames request
                if ((uint32_t)len < (2u * sizeof(int32_t)))
                {
                    rez = (uint32_t)RSDK_CSI2_DRV_NULL_PARAM_PTR;
                }
                else
                {
                    vcId = GetVirtChnlId((uint32_t)pUnitId[1]);
                    rez = (uint32_t)Csi2PlatformGetFramesCounter(unitId, vcId);
                }
                break;
            default:
                rez = (uint32_t)RSDK_CSI2_DRV_ERR_INVALID_REQ;  // unknown request
                break;
        } // switch (func)
    } // if ((uint32_t)len < sizeof(int32_t))
    return rez;
}

/*******************************************************************************/
/**
 * @brief       RemoteProcedureCall server dispatcher for error return.
 * @details     The function is called when the user-space library/error thread ask for error wait.
 *
 */
static uint32_t RsdkCsi2RpcDispatcherE(oal_dispatcher_t *d, uint32_t func, uintptr_t in, int32_t len, uint32_t unitId)
{
    uint32_t            rez = (uint32_t)RSDK_SUCCESS;
    uint32_t            pRead;
    
    if(func != RSDK_CSI2_LX_GET_ERRORS)
    {
        return RSDK_SUCCESS;            // any other request than GET_ERRORS is skipped
    }
    while(true)
    {
        pRead = gpRsdkCsi2Device[unitId]->irqReadP;             // the current read popinter
        if(pRead != gpRsdkCsi2Device[unitId]->irqWriteP)        // if read != write, process it
        {
            if(gpRsdkCsi2Device[unitId]->irqErrorPipe[pRead].errType >= RSDK_CSI2_ERROR_TO_EXIT)
            {                   // close request
                gpRsdkCsi2Device[unitId]->irqReadP = (pRead + 1) % RSDK_CSI2_ERROR_QUEUE_LEN;   // skip this "error"
                rez = RSDK_CSI2_DRV_POWERED_OFF;
                break;
            }
            // copy the error to the response
            rez = OAL_RPCAppendReply(d, 
                            (char *)&(gpRsdkCsi2Device[unitId]->irqErrorPipe[pRead]), sizeof(rsdkCsi2KernelErr_t));
            gpRsdkCsi2Device[unitId]->irqReadP = (pRead + 1) % RSDK_CSI2_ERROR_QUEUE_LEN;   // error processed
            if(rez != 0u)
            {
                rez = RSDK_CSI2_DRV_ERR_COPY_DATA_ERROR;        // signal the copy error
            }
            break;
        }
        // no errors to report, so wait for wakeup
        OAL_WaitEventInterruptible(gpRsdkCsi2Device[unitId]->irqWaitQ, 
                                gpRsdkCsi2Device[unitId]->irqReadP != gpRsdkCsi2Device[unitId]->irqWriteP);
    }
    return rez;
}

/*******************************************************************************/
/**
 * @brief       RemoteProcedureCall server dispatcher for error return, unit_0
 * @details     The function is called when the user-space library/error thread ask for error wait.
 *
 */
static uint32_t RsdkCsi2RpcDispatcherErr0(oal_dispatcher_t *d, uint32_t func, uintptr_t in, int32_t len)
{
    return (RsdkCsi2RpcDispatcherE(d, func, in, len, (uint32_t)RSDK_CSI2_UNIT_0));
}

/*******************************************************************************/
/**
 * @brief       RemoteProcedureCall server dispatcher for error return, unit_1
 * @details     The function is called when the user-space library/error thread ask for error wait.
 *
 */
static uint32_t RsdkCsi2RpcDispatcherErr1(oal_dispatcher_t *d, uint32_t func, uintptr_t in, int32_t len)
{
    return (RsdkCsi2RpcDispatcherE(d, func, in, len, (uint32_t)RSDK_CSI2_UNIT_1));
}

/*******************************************************************************/
/**
 * @brief       RemoteProcedureCall server dispatcher for error return, unit_2
 * @details     The function is called when the user-space library/error thread ask for error wait.
 *
 */
static uint32_t RsdkCsi2RpcDispatcherErr2(oal_dispatcher_t *d, uint32_t func, uintptr_t in, int32_t len)
{
    return (RsdkCsi2RpcDispatcherE(d, func, in, len, (uint32_t)RSDK_CSI2_UNIT_2));
}

/*******************************************************************************/
/**
 * @brief       RemoteProcedureCall server dispatcher for error return, unit_3
 * @details     The function is called when the user-space library/error thread ask for error wait.
 *
 */
static uint32_t RsdkCsi2RpcDispatcherErr3(oal_dispatcher_t *d, uint32_t func, uintptr_t in, int32_t len)
{
    return (RsdkCsi2RpcDispatcherE(d, func, in, len, (uint32_t)RSDK_CSI2_UNIT_3));
}

/*******************************************************************************/
/**
 * @brief       Error RPC Server exit for the waiting call
 * @details     Server cleanup called, server address set to NULL.
 *
 */
static void RsdkCsi2RpcErrExit(int32_t unitId)
{
    RecordErrorAndWakeup(RSDK_CSI2_ERROR_TO_EXIT + unitId, NULL);   // specific call for communication stop
}

/*******************************************************************************/
/**
 * @brief       RPC Server exit
 * @details     Server cleanup called, server address set to NULL.
 *
 */
int32_t RsdkCsi2RpcSrvExit(uint32_t unitId)
{
    int32_t rez = 0;
    
    if(unitId == 0)
    {
        rez = OAL_RPCCleanup(gpRsdkCsi2Device[0]->gspRpcServW);
        gpRsdkCsi2Device[0]->gspRpcServW = NULL;
    }
    if (rez == 0)
    {
            RsdkCsi2RpcErrExit(unitId);
            (void)OAL_RPCCleanup(gpRsdkCsi2Device[unitId]->gspRpcServE);
            gpRsdkCsi2Device[unitId]->gspRpcServE = NULL;
    }
    return rez;
}

/*******************************************************************************/
/**
 * @brief       RPC Servers initialization
 * @details     Servers initialized (work and errors) and marked as started.
 *
 */
int32_t RsdkCsi2RpcSrvInit(uint32_t unitId)
{
    int32_t rc;

    rc = 0;

    if(unitId == 0u)
    {
        // initialize the only one work RPC server
        gpRsdkCsi2Device[0]->gspRpcServW = OAL_RPCRegister(RSDK_CSI2_RPC_WORK, RsdkCsi2RpcDispatcherW);
    }
    else
    {
        gpRsdkCsi2Device[unitId]->gspRpcServW = gpRsdkCsi2Device[0]->gspRpcServW;
    }
    if (gpRsdkCsi2Device[0]->gspRpcServW == NULL)
    {
        rc = -1;            // error to initialize the work RPC server
    }
    else
    {
        // initialize the errors RPC server if the work RPC server is on
        sprintf(eServerName[unitId], "%s_%d", RSDK_CSI2_RPC_ERROR, unitId);
        switch(unitId)
        {
            case (uint32_t)RSDK_CSI2_UNIT_0:
                gpRsdkCsi2Device[unitId]->gspRpcServE = 
                                    OAL_RPCRegister(eServerName[unitId], RsdkCsi2RpcDispatcherErr0);
                break;
            case (uint32_t)RSDK_CSI2_UNIT_1:
                gpRsdkCsi2Device[unitId]->gspRpcServE = 
                                    OAL_RPCRegister(eServerName[unitId], RsdkCsi2RpcDispatcherErr1);
                break;
            case (uint32_t)RSDK_CSI2_UNIT_2:
                gpRsdkCsi2Device[unitId]->gspRpcServE = 
                                    OAL_RPCRegister(eServerName[unitId], RsdkCsi2RpcDispatcherErr2);
                break;
            case (uint32_t)RSDK_CSI2_UNIT_3:
                gpRsdkCsi2Device[unitId]->gspRpcServE = 
                                    OAL_RPCRegister(eServerName[unitId], RsdkCsi2RpcDispatcherErr3);
                break;
        }
        if (gpRsdkCsi2Device[unitId]->gspRpcServE == NULL)
        {
            rc = -1;
            if(unitId == 0u)
            {
                RsdkCsi2RpcSrvExit(0);
            }
        }
        else
        {
            // reset the pipe pointers
            gpRsdkCsi2Device[unitId]->irqReadP  = 0u;
            gpRsdkCsi2Device[unitId]->irqWriteP = 0u;
        }
    }
    return rc;
}


#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
