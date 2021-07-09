/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_DRIVER_FUNCTIONS_H
#define OAL_DRIVER_FUNCTIONS_H

#include <oal_memory_allocator.h>
#include <oal_utils.h>

__BEGIN_DECLS

#define OAL_SERVICE_NAME "oal_cached"
#define OAL_MAX_LEN_RESERVED_REG 30U

/*======================================================================*/
/* Driver global functions */
/*======================================================================*/

enum { CMD_ALLOC = 0,                 ///< Add new allocation into the list
       CMD_FREE,                      ///< Remove the allocation from the list
       CMD_SIZE,                      ///< Get the size of the allocation
       CMD_BUFFER,                    ///< Get specific buffer
       CMD_FLUSH,                     ///< Flush and invalidate the whole cache
       CMD_FLUSH_SPECIFIC,            ///< Flush the specified cache
       CMD_INVALIDATE_SPECIFIC,       ///< Invalidate the specified cache
       CMD_FLUSHINVALIDATE_SPECIFIC,  ///< Flush and invalidate the specified
	                              /// cache
       CMD_INFO,  ///< Number of current allocations from all processes
       CMD_MEMORY_SIZE_FREE_GET,   ///< Free size in bytes remaining in OAL
	                           /// Memory
       CMD_MEMORY_SIZE_TOTAL_GET,  ///< Total size in bytes being managed by OAL
	                           /// Memory
       CMD_MEMORY_GET_DEVICES,     ///< Enabled devices
       CMD_MEMORY_GET_AUTOBALANCE,  ///< Autobalanced devices
       CMD_MEMORY_GET_BASE,         ///< Base address of a chunk
       CMD_MEMORY_GET_SIZE,         ///< Size of a chunk
       CMD_GET_N_CHUNKS,            ///< Get number of memory chunks
       CMD_GET_CHUNKS,              ///< Get reserved chunks
       CMD_GET_CACHE_LINE,          ///< Get cache line size
       CMD_GET_PAGE_SIZE,           ///< Get page size
       CMD_CREATE_TOKEN,            ///< Create memory token
       CMD_GET_TOKEN,               ///< Retrieve a token based on its ID
       CMD_RELEASE_TOKEN,           ///< Release token
};

typedef struct {
	uint64_t mSize;
	uint32_t mAlign;
	uint64_t mRetPhysPointer;
	uint32_t mChunkId;
	OAL_MEMORY_FLAG mFlags;
} CMD_ALLOC_TYPE;

typedef struct {
	uint64_t mSize;
	uint64_t mRetPhysPointer;
} CMD_FREE_TYPE;

typedef struct {
	uint64_t mHandlePointer;
	uint64_t mType;
	uint64_t mRetVirtPointer;
} CMD_BUFFER_TYPE;

typedef struct {
	uintptr_t mVirtualPointer;
	uint64_t mSize;
} CMD_FLUSH_SPECIFIC_TYPE;

typedef struct {
	uint64_t mPhysAddr;
	uint64_t mSize;
	uintptr_t mTokenID;
} OAL_TOKEN_TYPE;

__END_DECLS

#endif /* OAL_DRIVER_FUNCTIONS_H */
