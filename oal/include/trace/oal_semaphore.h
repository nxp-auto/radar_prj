/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_SEMA_H
#define OAL_TRACE_SEMA_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_InitializeSemaphore(...)                                           \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_InitializeSemaphore, __VA_ARGS__)

#define OAL_GiveSemaphore(...)                                                 \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_GiveSemaphore, __VA_ARGS__)

#define OAL_TakeSemaphore(...)                                                 \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_TakeSemaphore, __VA_ARGS__)

#define OAL_GetValueSemaphore(...)                                             \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_GetValueSemaphore, __VA_ARGS__)

#define OAL_DestroySemaphore(...)                                              \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_DestroySemaphore, __VA_ARGS__)

#endif
#endif
