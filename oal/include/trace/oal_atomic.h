/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_ATOMIC_H
#define OAL_TRACE_ATOMIC_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_SetAtomic(...)                                                     \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_SetAtomic, __VA_ARGS__)

#define OAL_GetAtomic(...)                                                     \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_GetAtomic, __VA_ARGS__)

#define OAL_IncrementAtomic(...)                                               \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_IncrementAtomic, __VA_ARGS__)

#define OAL_DecrementAtomic(...)                                               \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_DecrementAtomic, __VA_ARGS__)

#define OAL_AddAtomic(...)                                                     \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_AddAtomic, __VA_ARGS__)

#define OAL_SubtractAtomic(...)                                                \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_SubtractAtomic, __VA_ARGS__)

#endif
#endif
