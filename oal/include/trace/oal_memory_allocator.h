/*
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_TRACE_MEMORY_ALLOCATOR_H
#define OAL_TRACE_MEMORY_ALLOCATOR_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_AllocMemory(...)                                                   \
	OAL_TRACE_FUNCTION(2, struct OAL_MemoryHandle *, OAL_AllocMemory,      \
	                   __VA_ARGS__)

#define OAL_AllocAndMapMemory(...)                                             \
	OAL_TRACE_FUNCTION(3, struct OAL_MemoryHandle *,                       \
	                   OAL_AllocAndMapMemory, __VA_ARGS__)

#define OAL_FreeMemory(...)                                                    \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_FreeMemory, __VA_ARGS__)

#define OAL_CheckAllocated(...)                                                \
	OAL_TRACE_FUNCTION(1, uint8_t, OAL_CheckAllocated, __VA_ARGS__)

#define OAL_GetReturnAddress(...)                                              \
	OAL_TRACE_FUNCTION(2, uintptr_t, OAL_GetReturnAddress, __VA_ARGS__)

#define OAL_UnmapMemory(...)                                                   \
	OAL_TRACE_FUNCTION(1, int32_t, OAL_UnmapMemory, __VA_ARGS__)

#define OAL_FlushMemory(...)                                                   \
	OAL_TRACE_VOID_FUNCTION(2, int32_t, OAL_FlushMemory, __VA_ARGS__)

#define OAL_InvalidateMemory(...)                                              \
	OAL_TRACE_VOID_FUNCTION(2, int32_t, OAL_InvalidateMemory, __VA_ARGS__)

#define OAL_FlushAndInvalidateMemory(...)                                      \
	OAL_TRACE_VOID_FUNCTION(2, int32_t, OAL_FlushAndInvalidateMemory,      \
	                        __VA_ARGS__)

#define OAL_FlushAndInvalidateAll(...)                                         \
	OAL_TRACE_VOID_FUNCTION(0, int32_t, OAL_FlushAndInvalidateAll,         \
	                        __VA_ARGS__)

#endif
#endif
