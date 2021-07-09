/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_FDT_UTILS_H
#define OAL_TRACE_FDT_UTILS_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_GetName(...)                                                       \
	OAL_TRACE_FUNCTION(4, int32_t, OAL_GetName, __VA_ARGS__)

#define OAL_GetProp(...)                                                       \
	OAL_TRACE_FUNCTION(5, int32_t, OAL_GetProp, __VA_ARGS__)

#define OAL_NodeOffsetByPhandle(...)                                           \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_NodeOffsetByPhandle, __VA_ARGS__)

#define OAL_NodeOffsetByCompatible(...)                                        \
	OAL_TRACE_FUNCTION(4, int32_t, OAL_NodeOffsetByCompatible, __VA_ARGS__)

#endif
#endif
