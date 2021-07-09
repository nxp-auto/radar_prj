/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_WAITQUEUE_H
#define OAL_TRACE_WAITQUEUE_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_InitWaitQueue(...)                                                 \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_InitWaitQueue, __VA_ARGS__)

#define OAL_WakeUp(...) OAL_TRACE_FUNCTION(1, int32_t, OAL_WakeUp, __VA_ARGS__)

#define OAL_WakeUpInterruptible(...)                                           \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_WakeUpInterruptible, __VA_ARGS__)

#define OAL_DestroyWaitQueue(...)                                              \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_DestroyWaitQueue, __VA_ARGS__)

#endif
#endif
