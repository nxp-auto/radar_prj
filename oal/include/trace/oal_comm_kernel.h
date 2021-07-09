/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_COMM_KERNEL_H
#define OAL_TRACE_COMM_KERNEL_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_RPCRegister(...)                                                   \
	OAL_TRACE_FUNCTION(2, OAL_RPCService_t, OAL_RPCRegister, __VA_ARGS__)

#define OAL_RPCSetPrivateData(...)                                             \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_RPCSetPrivateData, __VA_ARGS__)

#define OAL_RPCGetPrivateData(...)                                             \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_RPCGetPrivateData, __VA_ARGS__)

#define OAL_RPCCleanup(...)                                                    \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_RPCCleanup, __VA_ARGS__)

#define OAL_RPCAppendReply(...)                                                \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_RPCAppendReply, __VA_ARGS__)

#define OAL_RPCGetService(...)                                                 \
	OAL_TRACE_FUNCTION(1, OAL_RPCService_t, OAL_RPCGetService, __VA_ARGS__)

#define OAL_RPCGetClientPID(...)                                               \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_RPCGetClientPID, __VA_ARGS__)

#define OAL_RPCRegisterEvent(...)                                              \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_RPCRegisterEvent, __VA_ARGS__)

#define OAL_RPCTriggerEvent(...)                                               \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_RPCTriggerEvent, __VA_ARGS__)

#define OAL_RPCDeregisterEvent(...)                                            \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_RPCDeregisterEvent, __VA_ARGS__)

#endif
#endif
