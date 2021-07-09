/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_COMM_H
#define OAL_TRACE_COMM_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_SubscribeToEvent(...)                                              \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_SubscribeToEvent, __VA_ARGS__)

#define OAL_WaitForEvents(...)                                                 \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_WaitForEvents, __VA_ARGS__)

#define OAL_WaitForEventsWithTimeout(...)                                      \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_WaitForEventsWithTimeout,           \
	                   __VA_ARGS__)

#define OAL_HasEventFired(...)                                                 \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_HasEventFired, __VA_ARGS__)

#define OAL_GetEventId(...)                                                    \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_GetEventId, __VA_ARGS__)

#define OAL_UnsubscribeFromEvent(...)                                          \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_UnsubscribeFromEvent, __VA_ARGS__)
#endif
#endif
