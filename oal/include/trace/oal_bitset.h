/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_TRACE_BITSET_H
#define OAL_TRACE_BITSET_H
#ifndef OAL_FUNC_IMPLEMENTATION

#include <trace/oal_trace_func.h>

#define OAL_InitBitset(...)                                                    \
	OAL_TRACE_FUNCTION(2, int32_t, OAL_InitBitset, __VA_ARGS__)

#define OAL_BitsetGetUnusedBit(...)                                            \
	OAL_TRACE_FUNCTION(3, int32_t, OAL_BitsetGetUnusedBit, __VA_ARGS__)

#define OAL_GetBitsetChunk(...)                                                \
	OAL_TRACE_FUNCTION(2, OALBitsetChunk_t *, OAL_GetBitsetChunk,          \
	                   __VA_ARGS__)
#endif
#endif
