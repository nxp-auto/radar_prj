/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_DEVTREE_UTILS_H
#define OAL_TRACE_DEVTREE_UTILS_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_FdtNodeByCompatible(...)                                           \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_FdtNodeByCompatible, __VA_ARGS__)

#define OAL_FdtGetNmatchesByCompatible(...)                                    \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_FdtGetNmatchesByCompatible,         \
	                   __VA_ARGS__)

#define OAL_FdtMatchCall(...)                                                  \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_FdtMatchCall, __VA_ARGS__)

#define OAL_GetFdtAddress(...)                                                 \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_GetFdtAddress, __VA_ARGS__)

#define OAL_FdtParsePhandle(...)                                               \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_FdtParsePhandle, __VA_ARGS__)

#define OAL_FdtGetReg(...)                                                     \
	OAL_TRACE_FUNCTION(4, int32_t, OAL_FdtGetReg, __VA_ARGS__)

#define OAL_FdtGetNodeProperty(...)                                            \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_FdtGetNodeProperty, __VA_ARGS__)

#endif
#endif
