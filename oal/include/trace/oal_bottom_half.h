/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_BOTTOM_HALF_H
#define OAL_TRACE_BOTTOM_HALF_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_InitializeBottomHalf(...)                                          \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_InitializeBottomHalf, __VA_ARGS__)

#define OAL_ScheduleBottomHalf(...)                                            \
	OAL_TRACE_VOID_FUNCTION(1, void, OAL_ScheduleBottomHalf, __VA_ARGS__)

#define OAL_DestroyBottomHalf(...)                                             \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_DestroyBottomHalf, __VA_ARGS__)
#endif
#endif
