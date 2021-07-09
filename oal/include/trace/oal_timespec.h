/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_TIMESPEC_H
#define OAL_TRACE_TIMESPEC_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_GetTime(...)                                                       \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_GetTime, __VA_ARGS__)

#define OAL_Sleep(...) OAL_TRACE_FUNCTION(1, int32_t, OAL_Sleep, __VA_ARGS__)

#endif
#endif
