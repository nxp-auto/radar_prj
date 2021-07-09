/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_COMM_KERNEL_H
#define OAL_COMM_KERNEL_H

#include <oal_utils.h>

__BEGIN_DECLS

/**
 * @addtogroup OAL_Comm
 * @{
 * @name Kernel Space / Low Level API
 *
 * This API allows to export a set of driver's services to clients.
 * The service provider / dispatcher is registered using #OAL_RPCRegister.
 **/

struct oal_dispatcher;
typedef struct oal_dispatcher oal_dispatcher_t;

struct OAL_RPCService;
typedef struct OAL_RPCService *OAL_RPCService_t;

/**
 * @brief This type must be defined by the user and will be used by the
 * server in order to keep user's private data.
 */
struct OAL_ServiceData;
typedef struct OAL_ServiceData *OAL_ServiceData_t;

/**
 * @brief Dispatch function that will be called every time when a new
 * message / packages is received from clients.
 *
 * @param[in] apDisp    The dispatcher
 * @param[in] aFuncID   The ID of the function to be called via RPC
 * @param[in] aInData   The input arguments (linearized)
 * @param[in] aDataLen  The size of the input arguments
 *
 * @return
 * 	- 0 for success
 * 	- non-zero value otherwise
 *
 * @note This value will be passed to user space application as a return of RPC.
 */
typedef uint32_t (*OAL_dispatch_t)(oal_dispatcher_t *apDisp, uint32_t aFuncID,
                                   uintptr_t aInData, int32_t aDataLen);

/**
 * @brief Register and start a RPC service
 *
 * @param[in] acpName  The name of the service
 * @param[in] aDisp  The dispatch function
 *
 * @return
 * 	- <tt>NULL</tt> if the call fails
 * 	- a non <tt>NULL</tt> value otherwise
 */
OAL_RPCService_t OAL_RPCRegister(const char8_t *acpName, OAL_dispatch_t aDisp);

/**
 * @brief Set service's private data. Usually called after #OAL_RPCRegister
 * in order to save driver's context using @a aData. The saved data can be
 * retrieved later in #OAL_dispatch_t function using #OAL_RPCGetPrivateData
 *
 * @param[in] aServ   The service
 * @param[in] aData   The data to be keept
 *
 * @return
 * 	- 0 for success
 * 	- a negative value otherwise
 * @see #OAL_RPCGetPrivateData
 */
int32_t OAL_RPCSetPrivateData(OAL_RPCService_t aServ, OAL_ServiceData_t aData);

/**
 * @brief Get service's private data. Usually called from #OAL_dispatch_t
 * to retrieve driver's context after saving it with #OAL_RPCSetPrivateData
 *
 * @param[in]  aServ   The service
 * @param[out] apData  A reference to service's data
 *
 * @return
 * 	- 0 for success
 * 	- a negative value otherwise
 */
int32_t OAL_RPCGetPrivateData(OAL_RPCService_t aServ,
                              OAL_ServiceData_t *apData);

/**
 * @brief Stops the RPC service
 *
 * @param[in] acServ   The service
 *
 * @return
 * 	- 0 for success
 * 	- a negative value otherwise
 */
int32_t OAL_RPCCleanup(const OAL_RPCService_t acServ);

/**
 * @brief Append data to OS call reply.
 * Usually it is used by dispatch function as a helper to send data
 * to client.
 *
 * @param[in] apDisp    The dispatcher
 * @param[in] apData    The data to be appended
 * @param[in] aDataSize The size of the data in bytes
 *
 * @return
 * 	- 0 for success
 * 	- a negative value otherwise
 */
int32_t OAL_RPCAppendReply(oal_dispatcher_t *apDisp, uint8_t *apData,
                           size_t aDataSize);

/**
 * @brief Get source service that issued the call
 *
 * @param[in] apDisp The dispatcher
 *
 * @return
 * 	- service instance for success
 * 	- <tt>NULL</tt> otherwise
 */
OAL_RPCService_t OAL_RPCGetService(oal_dispatcher_t *apDisp);

/**
 * @brief Get process identifier of a calling process (client)
 *
 * @param[in]  apDispatch    The dispatcher
 * @param[out] apClientPID   The PID of the client
 *
 * @return
 * 	- 0 for success
 * 	- a negative value otherwise
 */
int32_t OAL_RPCGetClientPID(oal_dispatcher_t *apDispatch, pid_t *apClientPID);

/** @} */
/**
 * @{
 * @name Asynchronous notifications - Kernel space
 */

struct OAL_RPCEvent;
typedef struct OAL_RPCEvent *OAL_RPCEvent_t;

/**
 * @brief Register an asynchronous event on \p aServ
 *
 * @param[in] aServ     Service to attach the event
 * @param[in] aEventID   Event ID
 * @param[out] apEvent   Event metadata to be initialized
 *
 * @return
 * 	- 0 for success
 * 	- a negative value otherwise
 */
int32_t OAL_RPCRegisterEvent(OAL_RPCService_t aServ, uint32_t aEventID,
                             OAL_RPCEvent_t *apEvent);

/**
 * @brief Generate an event.
 * @pre \p aEvent must be obtained using @ref OAL_RPCRegisterEvent
 *
 * @param[in] aEvent   The event to be generated
 *
 * @return
 * 	- 0 for success
 * 	- a negative value otherwise
 */
int32_t OAL_RPCTriggerEvent(OAL_RPCEvent_t aEvent);

/**
 * @brief Revert #OAL_RPCRegisterEvent operation
 *
 * @param[in] aEvent The event to deregister.
 *
 * @warning This function will be called only after all clients have been
 * unsubscribed.
 * @see OAL_RPCRegisterEvent
 * @return
 * 	- 0 for success
 * 	- a negative value otherwise
 */
int32_t OAL_RPCDeregisterEvent(OAL_RPCEvent_t aEvent);

/** @} */

/** @} */

__END_DECLS

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_comm_kernel.h>
#endif
#endif /* OAL_COMM_KERNEL_H */
