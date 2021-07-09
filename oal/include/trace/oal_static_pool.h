/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_STATIC_POOL_H
#define OAL_TRACE_STATIC_POOL_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_InitializePool(...)                                                \
	OAL_TRACE_FUNCTION(4, int32_t, OAL_InitializePool, __VA_ARGS__)

#define OAL_IsPoolEmpty(...)                                                   \
	OAL_TRACE_FUNCTION(3, uint8_t, OAL_IsPoolEmpty, __VA_ARGS__)

#define OAL_IsPoolFull(...)                                                    \
	OAL_TRACE_FUNCTION(2, uint8_t, OAL_IsPoolFull, __VA_ARGS__)

#define OAL_GetFirstUnusedPosition(...)                                        \
	OAL_TRACE_FUNCTION(5, int32_t, OAL_GetFirstUnusedPosition, __VA_ARGS__)

#define OAL_SetUnusedPosition(...)                                             \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_SetUnusedPosition, __VA_ARGS__)

#define OAL_SetRangeUsage(...)                                                 \
	OAL_TRACE_FUNCTION(6, int32_t, OAL_SetRangeUsage, __VA_ARGS__)

#define OAL_GetNextUnusedRange(...)                                            \
	OAL_TRACE_FUNCTION(5, int32_t, OAL_GetNextUnusedRange, __VA_ARGS__)

#define OAL_GetUnusedRange(...)                                                \
	OAL_TRACE_FUNCTION(5, int32_t, OAL_GetUnusedRange, __VA_ARGS__)

#define OAL_AllocElemFromPool(...)                                             \
	OAL_TRACE_FUNCTION(7, int32_t, OAL_AllocElemFromPool, __VA_ARGS__)

#define OAL_ReleaseElemFromPool(...)                                           \
	OAL_TRACE_FUNCTION(5, int32_t, OAL_ReleaseElemFromPool, __VA_ARGS__)

#endif
#endif
