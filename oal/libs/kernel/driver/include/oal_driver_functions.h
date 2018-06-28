/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_DRIVER_FUNCTIONS_H
#define OAL_DRIVER_FUNCTIONS_H

#include "oal_utils.h"
#include "oal.h"

__BEGIN_DECLS


#define OAL_SERVICE_NAME "oal_cached"

/*======================================================================*/
/* Driver global functions */
/*======================================================================*/

enum {
	CMD_ALLOC = 0, ///< Add new allocation into the list
	CMD_FREE, ///< Remove the allocation from the list
	CMD_SIZE, ///< Get the size of the allocation
	CMD_BUFFER, ///< Get specific buffer
	CMD_FLUSH, ///< Flush and invalidate the whole cache
	CMD_FLUSH_SPECIFIC, ///< Flush the specified cache 
	CMD_INVALIDATE_SPECIFIC, ///< Invalidate the specified cache
	CMD_FLUSHINVALIDATE_SPECIFIC, ///< Flush and invalidate the specified cache
	CMD_INFO, ///< Number of current allocations from all processes
	CMD_MEMORY_SIZE_FREE_GET, ///< Free size in bytes remaining in OAL Memory
	CMD_MEMORY_SIZE_TOTAL_GET, ///< Total size in bytes being managed by OAL Memory
	CMD_MEMORY_GET_DEVICES, ///< Enabled devices
	CMD_MEMORY_GET_AUTOBALANCE, ///< Autobalanced devices
	CMD_MEMORY_GET_BASE, ///< Base address of a chunk
	CMD_MEMORY_GET_SIZE, ///< Size of a chunk
	CMD_GET_N_CHUNKS, ///< Get number of memory chunks
	CMD_GET_CHUNKS, ///< Get reserved chunks
};

typedef struct
{
  uint64_t   size;
  uint64_t   align;
  uint64_t   ret_phys_pointer;
  uint8_t    chunk_id;
  OAL_MEMORY_FLAG flags;
} CMD_ALLOC_TYPE;

typedef struct
{
  uint64_t   handle_pointer;
  uint64_t   type;
  uint64_t   ret_virt_pointer;
} CMD_BUFFER_TYPE;

typedef struct
{
  uint64_t   virtual_pointer;
  uint64_t   size;
} CMD_FLUSH_SPECIFIC_TYPE;

#define MAX_LEN_RESERVED_REG 30

typedef struct {
	char mName[MAX_LEN_RESERVED_REG];
	uint32_t mId;
	uint64_t mStartAddr;
	uint64_t mSize;
} OAL_ReservedRegion_t;

__END_DECLS

#endif	/* OAL_DRIVER_FUNCTIONS_H */
