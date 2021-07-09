/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_THREAD_H
#define OAL_TRACE_THREAD_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_CreateThread(...)                                                  \
	OAL_TRACE_FUNCTION(4, int32_t, OAL_CreateThread, __VA_ARGS__)

#define OAL_JoinTask(...)                                                      \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_JoinTask, __VA_ARGS__)

#endif
#endif
