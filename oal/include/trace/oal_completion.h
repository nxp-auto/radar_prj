/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_COMPLETION_H
#define OAL_TRACE_COMPLETION_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_InitCompletion(...)                                                \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_InitCompletion, __VA_ARGS__)

#define OAL_WaitForCompletion(...)                                             \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_WaitForCompletion, __VA_ARGS__)

#define OAL_WaitForCompletion(...)                                             \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_WaitForCompletion, __VA_ARGS__)

#define OAL_WaitForCompletionTimeout(...)                                      \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_WaitForCompletionTimeout,           \
	                   __VA_ARGS__)

#define OAL_Complete(...)                                                      \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_Complete, __VA_ARGS__)

#define OAL_CompleteAll(...)                                                   \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_CompleteAll, __VA_ARGS__)

#define OAL_DestroyCompletion(...)                                             \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_DestroyCompletion, __VA_ARGS__)

#endif
#endif
