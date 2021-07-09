/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_SHARED_MEMORY_H
#define OAL_TRACE_SHARED_MEMORY_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_CreateMemoryToken(...)                                             \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_CreateMemoryToken, __VA_ARGS__)

#define OAL_MapMemoryToken(...)                                                \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_MapMemoryToken, __VA_ARGS__)

#define OAL_UnmapMemoryToken(...)                                              \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_UnmapMemoryToken, __VA_ARGS__)

#define OAL_ReleaseToken(...)                                                  \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_ReleaseToken, __VA_ARGS__)

#endif
#endif
