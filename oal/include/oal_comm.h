/*
 * Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_COMM_H
#define OAL_COMM_H

#include <oal_utils.h>
#include <oal_timespec.h>

__BEGIN_DECLS

/**
 * @defgroup OAL_Comm OAL Communication layer
 *
 * @brief Portable framework for communication between low level drivers and
 * high level applications
 *
 *  This framework aims to provide an uniform way of communication
 *  regardless of the underlying OS or communication mechanisms. It has two
 *  major components :
 *    - User Space / High Level / client API
 *    - Kernel Space / Low Level / server API
 *
 *  These two components have different interpretation depending on the use
 *  case. For Linux use case the server API is represented by a dynamically
 *  loadable kernel module and the clients by high level, user space
 *  applications.
 *
 *  In QNX case we have a server application (resource manager) for Low Level
 *  API and multiple other processes that are acting as clients (High Level
 *  API).
 *
 *  Generally speaking the server API should be used by a hardware manager /
 *  driver and the client API by its clients.
 *
 * The communication is based on messages and acts similar as a \b RPC
 * (Remote Procedure Call). The client sends a serialized array of data
 * (function ID, input arguments and out arguments). The server interprets the
 * received packages and calls user's dispatcher function (#OAL_dispatch_t),
 * registered after a #OAL_RPCRegister call.
 *
 * All above details are transparent to user. Steps to register and use your
 * own server:
 *
 *  1. Server side &frasl;
 *
 * @code{.c}
 *
 * static OAL_RPCService_t gMyService;
 *
 * static uint32_t my_dispatcher(oal_dispatcher_t *apDisp,
 * 				   uint32_t aFuncID,
 * 				   uint8_t *apInData,
 * 				   int32_t aDataLen)
 * {
 * 	uint32_t lRet = 0;
 * 	switch (aFuncID) {
 * 		case FUNCTION_1: {
         * 		#if 0
 * 			  Calling Function1
         * 		#endif
 * 			lRet = ... ;
 * 		}
 * 		case FUNCTION_2: {
         * 		#if 0
 * 			  Sending back some data
         * 		#endif
 * 			lRet = OAL_RPCAppendReply(apDisp, ...);
 * 			lRet = ... ;
 * 		}
 * 	}
 *
 * 	return lRet;
 * }
 *
 * static void driverInitFunction(void)
 * {
 * 	gMyService = OAL_RPCRegister("my_rpc_server",
 * 		my_dispatcher);
 * }
 * @endcode
 *
 *  2. Client side
 *
 * @code{.c}
 * 	OAL_FuncArgs_t lArgs[] = {
 * 		{
 * 			.mpData = &in_arg,
 * 			.mSize = sizeof(in_arg),
 * 		}
 * 	};
 * 	OAL_DriverHandle_t lHandle = OAL_OpenDriver("my_rpc_server");
 *
 * 	lRet = OAL_DriverInCall(lHandle, (uint32_t)FUNCTION_1,
 * 				lArgs, OAL_ARRAY_SIZE(lArgs));
 * @endcode
 *
 * @section available_comm Available communication mechanisms
 *
 * | Linux    | QNX      | Integrity  | Standalone |
 * | :------: | :------: | :--------: | :--------: |
 * | Write    | Channels | Connection | Memory     |
 * | Netlinks |          |            |            |
 *
 */

/**
 * @addtogroup OAL_Comm
 * @{
 * @name User Space / High Level API
 *
 * Client's API for RPC communication.
 *
 * Usually the client follows the steps:
 * 1. Opens the communication channel using #OAL_OpenDriver
 * 2. Calls a remote function using #OAL_DriverCall or its wrappers
 *    * #OAL_DriverNoArgCall                  - Function with no arguments
 *    * #OAL_DriverInCall, #OAL_SimpleInCall  - Function with input arguments
 *only
 *    * #OAL_DriverOutCall, OAL_SimpleOutCall - Function with output arguments
 *only
 * 3. Ends connection / communication using #OAL_CloseDriver
 **/

/**
 * @brief Driver handle
 */
struct OS_OAL_DriverHandle;
typedef struct OS_OAL_DriverHandle *OAL_DriverHandle_t;

/**
 * @brief Describe the argument of a remote function
 */
struct OAL_FuncArgs {
	void *mpData;  ///< The address of the argument
	size_t mSize;  ///< Size of the argument
};

typedef struct OAL_FuncArgs OAL_FuncArgs_t;

/**
 * @brief Open a communication channel to the driver
 *
 * @param[in] acpChannelName The identification path of the driver
 *
 * @return A handle of the channel if the operation succeeded, NULL otherwise
 */

OAL_DriverHandle_t OAL_OpenDriver(const char8_t *acpChannelName);

/**
 * @brief Call a function exported by the driver.
 *
 * @param[in]  aHandle          Driver handle
 * @param[in]  aFuncID       The ID of the function to be called
 * @param[in]  apInArgs    Array with input parameters
 * @param[in]  aNumInArgs  Number of input parameters
 * @param[out] apOutArgs   Array with outputs
 * @param[in]  aNumOutArgs Number of outputs
 *
 * @return
 *         - 0 if the operation succeeded
 *         - non-zero value otherwise
 * @see #OAL_dispatch_t
 */
uint32_t OAL_DriverCall(OAL_DriverHandle_t aHandle, uint32_t aFuncID,
                        OAL_FuncArgs_t *apInArgs, size_t aNumInArgs,
                        OAL_FuncArgs_t *apOutArgs, size_t aNumOutArgs);

/**
 * @brief Call a function exported by the driver with no parameters.
 *
 * Equivalent of:
 * @code{.c}
 * void driver_function(void)
 * @endcode
 *
 * @param[in]  aHandle       Driver handle
 * @param[in]  aFuncID       The ID of the function to be called
 *
 * @return
 *         - 0 if the operation succeeded
 *         - non-zero value otherwise
 * @see #OAL_dispatch_t
 */

static inline uint32_t OAL_DriverNoArgCall(OAL_DriverHandle_t aHandle,
                                           uint32_t aFuncID)
{
	return OAL_DriverCall(aHandle, aFuncID, NULL, 0, NULL, 0);
}

/**
 * @brief Call a function exported by the driver only with input parameters
 *
 * @param[in]  aHandle     Driver handle
 * @param[in]  aFuncID     The ID of the function to be called
 * @param[in]  apInArgs    Array with input parameters
 * @param[in]  aNumInArgs  Number of input parameters
 *
 * @return
 *         - 0 if the operation succeeded
 *         - non-zero value otherwise
 * @see #OAL_dispatch_t
 */
static inline uint32_t OAL_DriverInCall(OAL_DriverHandle_t aHandle,
                                        uint32_t aFuncID,
                                        OAL_FuncArgs_t *apInArgs,
                                        size_t aNumInArgs)
{
	return OAL_DriverCall(aHandle, aFuncID, apInArgs, aNumInArgs, NULL, 0);
}

/**
 * @brief Call a function exported by the driver with no input parameters
 *
 * @param[in]  aHandle     Driver handle
 * @param[in]  aFuncID     The ID of the function to be called
 * @param[out] apOutArgs   Array with outputs
 * @param[in]  aNumOutArgs Number of outputs
 *
 * @return
 *         - 0 if the operation succeeded
 *         - non-zero value otherwise
 * @see #OAL_dispatch_t
 */
static inline uint32_t OAL_DriverOutCall(OAL_DriverHandle_t aHandle,
                                         uint32_t aFuncID,
                                         OAL_FuncArgs_t *apOutArgs,
                                         size_t aNumOutArgs)
{
	return OAL_DriverCall(aHandle, aFuncID, NULL, 0, apOutArgs,
	                      aNumOutArgs);
}

/**
 * @brief Call a remote function with one single argument. The provided
 * argument is considered an in/out one.
 *
 * @param[in] H           Driver handle
 * @param[in] FUNC        The ID of the function to be called
 * @param[in,out] STRUCT  IN/OUT argument passed by value. The wrapper will
 *                        determine its size and pass down its address to
 *                        #OAL_DriverCall.
 *
 * @return
 *         - 0 if the operation succeeded
 *         - non-zero value otherwise
 * @see #OAL_dispatch_t
 */
#define OAL_SimpleInOutCall(H, FUNC, STRUCT)                                   \
	OAL_COMP_EXTENSION({                                                   \
		uint32_t lIOCRet      = 0;                                     \
		OAL_DriverHandle_t _h = (H);                                   \
		uint32_t _func        = (FUNC);                                \
		OAL_FuncArgs_t _in_arg, _out_arg;                              \
                                                                               \
		_in_arg.mpData = &(STRUCT);                                    \
		_in_arg.mSize  = sizeof((STRUCT));                             \
                                                                               \
		_out_arg.mpData = &(STRUCT);                                   \
		_out_arg.mSize  = sizeof((STRUCT));                            \
                                                                               \
		lIOCRet =                                                      \
		    OAL_DriverCall(_h, _func, &_in_arg, 1U, &_out_arg, 1U);    \
		lIOCRet;                                                       \
	})

/**
 * @brief Call a remote function with one input argument.
 *
 * @param[in] H           Driver handle
 * @param[in] FUNC        The ID of the function to be called
 * @param[in] STRUCT      IN argument passed by value. The wrapper will
 *                        determine its size and pass down its address to
 *                        #OAL_DriverCall.
 *
 * @return
 *         - 0 if the operation succeeded
 *         - non-zero value otherwise
 * @see #OAL_dispatch_t
 */
#define OAL_SimpleInCall(H, FUNC, STRUCT)                                      \
	OAL_COMP_EXTENSION({                                                   \
		uint32_t lICRet       = 0;                                     \
		OAL_DriverHandle_t _h = (H);                                   \
		uint32_t _func        = (FUNC);                                \
		OAL_FuncArgs_t _in_arg;                                        \
                                                                               \
		_in_arg.mpData = &(STRUCT);                                    \
		_in_arg.mSize  = sizeof((STRUCT));                             \
                                                                               \
		lICRet = OAL_DriverInCall(_h, _func, &_in_arg, 1U);            \
		lICRet;                                                        \
	})

/**
 * @brief Call a remote function with one output argument.
 *
 * @param[in] H           Driver handle
 * @param[in] FUNC        The ID of the function to be called
 * @param[out] STRUCT     OUT argument passed by value. The wrapper will
 *                        determine its size and pass down its address to
 *                        #OAL_DriverCall.
 *
 * @return
 *         - 0 if the operation succeeded
 *         - non-zero value otherwise
 * @see #OAL_dispatch_t
 */
#define OAL_SimpleOutCall(H, FUNC, STRUCT)                                     \
	OAL_COMP_EXTENSION({                                                   \
		uint32_t lOCRet       = 0;                                     \
		OAL_DriverHandle_t _h = (H);                                   \
		uint32_t _func        = (FUNC);                                \
		OAL_FuncArgs_t _out_arg;                                       \
                                                                               \
		_out_arg.mpData = &(STRUCT);                                   \
		_out_arg.mSize  = sizeof((STRUCT));                            \
                                                                               \
		lOCRet = OAL_DriverOutCall(_h, _func, &_out_arg, 1U);          \
		lOCRet;                                                        \
	})

/**
 * @brief Close the driver channel
 *
 * @param[in]  apHandle Driver handle
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_CloseDriver(OAL_DriverHandle_t *apHandle);

/** @} */
/**
 * @{
 * @name Asynchronous notifications - User space
 *
 * Notifications of the communication channel are delivered as events.
 *
 * The support is very similar to poll infrastructure and is split into two
 * well defined worlds
 *    * Kernel space / driver
 *    * User space / library
 *
 * It aims to offer an asynchronous mechanism for communication initiated by the
 * kernel space without blocking the main channel (\ref OAL_RPCService_t). It's
 * very useful when it's desired / needed to have interrupt handlers moved in
 * user space (user space drivers).
 *
 * The kernel space registers an event using \ref OAL_RPCRegisterEvent based on
 * an identification number and signals, wakes-up the user space process
 * (using \ref OAL_RPCTriggerEvent) to handle it.
 *
 * The User space side must create the handshake with the kernel space by
 * requesting to be notified when a specific event occurs by calling
 * \ref OAL_SubscribeToEvent. It must pass the same event id as the
 * kernel space. The event identification number is representing the passcode in
 * this case.
 *
 * This new communication tunnel can be used for notifications only.
 * User space will be able to wait notifications from multiple drivers using
 * \ref OAL_WaitForEvents. This function is unblocked only from kernel space
 * when at least one of the events from the passed list is firing after a
 * \ref OAL_RPCTriggerEvent call.
 */

struct OAL_DriverEvent;
typedef struct OAL_DriverEvent *OAL_DriverEvent_t;

/**
 * @brief Subscribe for a particular event of a driver.
 * @pre The event ID has to be exported by the driver before calling this
 * function.
 *
 * @param[in] aHandle    Driver handle
 * @param[in] aEventId   Event's ID
 * @param[out] apEvent   Event reference
 *
 * @return
 *     - 0 if the operation succeeded
 *     - a negative value otherwise
 */
int32_t OAL_SubscribeToEvent(OAL_DriverHandle_t aHandle, uint32_t aEventId,
                             OAL_DriverEvent_t *apEvent);

/**
 * @brief Wait one or multiple events to fire, equivalent of a <tt>poll</tt>
 * call.
 *
 * @param[in] apEvents      An array of events, obtained after
 *                          \ref OAL_SubscribeToEvent calls
 * @param[in] aNumEvents    Number of events contained by \p apEvents
 *
 * @note This call doesn't use busy-waiting loops for events availability checks
 * @return
 *     - 0 if the operation succeeded
 *     - a negative value otherwise
 */
int32_t OAL_WaitForEvents(OAL_DriverEvent_t *apEvents, size_t aNumEvents);

#define OAL_RPC_TIMEOUT 42

/**
 * @brief Wait one or multiple events to fire, equivalent of a <tt>poll</tt>
 * call.
 *
 * @param[in] apEvents      An array of events, obtained after
 *                          \ref OAL_SubscribeToEvent calls
 * @param[in] aNumEvents    Number of events contained by \p apEvents
 * @param[in] apTimeout     Timeout relative to \e now
 *
 * @note This call doesn't use busy-waiting loops for events availability checks
 * @return
 *     - 0 if the operation succeeded
 *     - a negative value otherwise
 *     - \ref OAL_RPC_TIMEOUT if a timeout occured
 */
int32_t OAL_WaitForEventsWithTimeout(OAL_DriverEvent_t *apEvents,
                                     size_t aNumEvents,
                                     OAL_Timespec_t *apTimeout);

/**
 * @brief Check if an event has fired.
 *
 * @param[in] aEvent         The event to be checked
 * @param[out] apEventStatus 1 if the event fired, 0 otherwise
 *
 * @note To be used only after an \ref OAL_WaitForEvents or
 *       \ref OAL_WaitForEventsWithTimeout call
 * @return
 *     - 0 if the operation succeeded
 *     - a negative value otherwise
 */
int32_t OAL_HasEventFired(OAL_DriverEvent_t aEvent, uint8_t *apEventStatus);

/**
 * @brief Retrieve event's ID
 *
 * @param[in] aEvent      Event handle
 * @param[out] apEventId  Event's ID
 *
 * @return
 *     - 0 if the operation succeeded
 *     - a negative value otherwise
 */
int32_t OAL_GetEventId(OAL_DriverEvent_t aEvent, uint32_t *apEventId);

/**
 * @brief Unsubscribe from an event
 *
 * @param[in] aEvent The event obtained after a \ref OAL_SubscribeToEvent call
 *
 * @return
 *     - 0 if the operation succeeded
 *     - a negative value otherwise
 */
int32_t OAL_UnsubscribeFromEvent(OAL_DriverEvent_t aEvent);

/** @} */

/** @} */

__END_DECLS

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_comm.h>
#endif
#endif /* OAL_COMM_H */
