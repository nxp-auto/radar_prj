/*
* Copyright 2019-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "spt_driver_module.h"
#include "Spt_Oal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define SIGNED_ERROR_CONVERT(x) ((uint32_t)((-1) * x))

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/
static uint32_t SptOalCommCh0Dispatcher(oal_dispatcher_t *d, uint32_t func, uintptr_t in, int32_t len)
{
    uint32_t ret = 0;
    int32_t oalStatus;

    UNUSED_ARG(in);

    PR_ALERT("spt_driver module: SptOalCommCh0Dispatcher cmd: %d, irqsNotServed = %d \n", func, atomic_read(&(sptDevice.irqsNotServed)));
    switch (func)
    {
        case (uint32_t)SPT_OAL_RPC_WAIT_FOR_IRQ:
        {
            if (len != 0)
            {
                PR_ERR("spt_driver module: SptOalCommCh0Dispatcher error: size of input arg should be 0!\n");
                ret = EFAULT;
                break;
            }

            /*
             * Wait for the evtData queue to have at least one element. atomic_dec_if_positive is performed before the
             * evaluation of the '>= 0' condition. The thread will start executing only if this call was successful.
             * If the return value of atomic_dec_if_positive is < 0, it means that the irqsNotServed variable is already
             * 0 and it is no longer decremented. This allows the interrupt handler to signal the need for data transfer
             * by performing only an increment on the irqsNotServed variable.
             */
            oalStatus = OAL_WaitEventInterruptible(sptDevice.irqWaitQ, atomic_dec_if_positive(&(sptDevice.irqsNotServed)) >= 0);

            if (oalStatus == (-ERESTARTSYS))
            {
                /*
                * Remap to EINTR which can be read in user-space from errno.h This code indicates that the system call has been
                * interrupted by a POSIX signal and can be restarted. No error actually occurred.
                */
                ret = EINTR;
                break;
            }
            else if (oalStatus != 0)
            {
                PR_ERR("spt_driver module: SptOalCommCh0Dispatcher error: OAL_WaitEventInterruptible() failed!\n");
                ret = SIGNED_ERROR_CONVERT(oalStatus);
                break;
            }

            if (sptDevice.queueIdxRd >= SPT_DATA_Q_SIZE)
            {
                PR_ERR("spt_driver module: SptOalCommCh0Dispatcher: Tail of data queue corrupted!\n");
                ret = EFAULT;
                break;
            }

            /* Transfer evtData from the tail of the queue & update tail */
            oalStatus = OAL_RPCAppendReply(d, (char *)&(sptDevice.queueEvtData[sptDevice.queueIdxRd]), sizeof(evtSharedData_t));
            sptDevice.queueIdxRd = (sptDevice.queueIdxRd + 1u) % SPT_DATA_Q_SIZE;

            if (oalStatus != 0)
            {
                PR_ALERT("spt_driver module: SptOalCommCh0Dispatcher: cannot send back RPC reply.\n");
                ret = SIGNED_ERROR_CONVERT(oalStatus);
            }

            PR_ALERT("spt_driver module: SptOalCommCh0Dispatcher: wake up.\n");
        
            break;
        }
        default:
        {
            ret = EPERM;
            PR_ALERT("spt_driver module: SptOalCommCh0Dispatcher error: unsupported command ID: %d.\n", func);
            break;
        }
    }

    return ret;
}

static uint32_t SptOalCommCh1Dispatcher(oal_dispatcher_t *d, uint32_t func, uintptr_t in, int32_t len)
{
    evtSharedData_t     evtData;
    uint32_t            ret = 0;
    int32_t             oalStatus; 
    volatile uint32_t   timeoutCnt = 100000;
    uint32_t            queueIdxWr;

    UNUSED_ARG(in);
    UNUSED_ARG(len);
    UNUSED_ARG(d);

    PR_ALERT("spt_driver module: SptOalCommCh1Dispatcher cmd: %d, irqsNotServed = %d \n", func, atomic_read(&(sptDevice.irqsNotServed)));
    switch (func)
    {
        case (uint32_t)SPT_OAL_RPC_TERM_SPTIRQCAP:
        {
            //send 'dummy' notification to wake up SptIrqCapture thread which is blocked in OAL_DriverOutCall(SPT_OAL_RPC_WAIT_FOR_IRQ):
            evtData.evtType = SPT_OAL_RPC_EVT_TERM_SPTIRQCAP;

            /* Get and update the queue write index.
             * By using 'atomic_inc_return' we can be sure that each IRQ handler that executes concurrently
             * will get a different value for 'queueIdxWr'. By calling 'atomic_cmpxchg' the value of the index
             * will be wrapped if needed. This will be performed successfully only by the handler that has the
             * most up to date value of 'queueIdxWr'. The 'atomic_cmpxchg' call ensures a correct wrap-around
             * of the index, regardless of the queue size (if the queue size is not a power of 2, the default
             * wrap-around of the atomic_t variable - int - will corrupt the queue).
             */
            queueIdxWr = (uint32_t)atomic_inc_return(&(sptDevice.queueIdxWr));
            (void)atomic_cmpxchg(&(sptDevice.queueIdxWr), (int32_t)queueIdxWr, (int32_t)(queueIdxWr % SPT_DATA_Q_SIZE));

            /* Compute local index ('atomic_inc_return' return the incremented value) */
            queueIdxWr = (queueIdxWr - 1u) % SPT_DATA_Q_SIZE;

            //put the notification in the evtData queue
            sptDevice.queueEvtData[queueIdxWr] = evtData;
            
            if ((uint32_t)atomic_inc_return(&(sptDevice.irqsNotServed)) > (SPT_DATA_Q_SIZE - SPT_DATA_Q_CNT_LAST))
            {
                PR_ALERT("spt_driver module: SptOalCommCh1Dispatcher: Data queue almost full!\n");
            }

            //signal the SptIrqCapture thread
            oalStatus = OAL_WakeUpInterruptible(&(sptDevice.irqWaitQ));
            if (oalStatus != 0)
            {
                PR_ERR("spt_driver module: SptOalCommCh1Dispatcher:: Cannot wake up user process!\n");
                ret = SIGNED_ERROR_CONVERT(oalStatus);
                break;
            }

            //wait until SptOalCommCh0Dispatcher is released from its wait:
            while (atomic_read(&(sptDevice.irqsNotServed)) == 0)
            {
                if (timeoutCnt > 0u)
                {
                    timeoutCnt--;
                }
                else
                {
                    ret = ETIME;
                    break;
                }
            }
            break;
        }
        case (uint32_t)SPT_OAL_RPC_BBE32_REBOOT:
        {
            oalStatus = reset_control_assert(sptDevice.dtsInfo.bbe32Reset);
            if (oalStatus != 0) 
            {
                PR_ERR("spt_driver module: SptOalCommCh1Dispatcher: Cannot assert reset for RADAR partition!\n");
                ret = SIGNED_ERROR_CONVERT(oalStatus);
            }
            else
            {
                oalStatus = reset_control_deassert(sptDevice.dtsInfo.bbe32Reset);
                if (oalStatus != 0) 
                {
                    PR_ERR("spt_driver module: SptOalCommCh1Dispatcher: Cannot deassert reset for RADAR partition!\n");
                    ret = SIGNED_ERROR_CONVERT(oalStatus);
                }
            } 		
            break;
        }
        default:
        {
            ret = EPERM;
            break;
        }
    }

    return ret;
}

int32_t SptOalCommInit(void)
{
    int32_t rc = 0;

    //First channel is used together with SptIrqCapture user thread for detecting interrupts
    //and other async events and forwarding related information to user space.
    //It lies in WAITING state most of the time.
    sptDevice.gsOalCommServ[0] = OAL_RPCRegister(SPT_OAL_COMM_CHANNEL1_NAME, SptOalCommCh0Dispatcher);
    if (sptDevice.gsOalCommServ[0] == NULL)
    {
        rc = -1;
    }
    else
    {
        //Second channel is used for general non-blocking RPC commands.
        //One such command is used for unblocking SPT_OAL_COMM_CHANNEL1_NAME, to allow termination of SptIrqCapture thread.
        //It's ok to use the same dispatcher, since it will execute in a separate thread
        sptDevice.gsOalCommServ[1] = OAL_RPCRegister(SPT_OAL_COMM_CHANNEL2_NAME, SptOalCommCh1Dispatcher);
        if (sptDevice.gsOalCommServ[1] == NULL)
        {
            (void)OAL_RPCCleanup(sptDevice.gsOalCommServ[0]);
            rc = -2;
        }
    }
    return rc;
}

int32_t SptOalCommExit(void)
{
    int32_t retVal;

    retVal = OAL_RPCCleanup(sptDevice.gsOalCommServ[0]);
    retVal += OAL_RPCCleanup(sptDevice.gsOalCommServ[1]);

    return retVal;
}

#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
