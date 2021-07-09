/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_TIMER_H
#define OAL_TRACE_TIMER_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_SetupTimer(...)                                                    \
	OAL_TRACE_FUNCTION(4, int32_t, OAL_SetupTimer, __VA_ARGS__)

#define OAL_SetTimerTimeout(...)                                               \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_SetTimerTimeout, __VA_ARGS__)

#define OAL_AddTimer(...)                                                      \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_AddTimer, __VA_ARGS__)

#define OAL_DelTimer(...)                                                      \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_DelTimer, __VA_ARGS__)

#define OAL_DestroyTimer(...)                                                  \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_DestroyTimer, __VA_ARGS__)

#endif
#endif
