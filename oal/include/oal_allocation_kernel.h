/*
 * Copyright 2014-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ALLOCATION_KERNEL_OAL_H__
#define __ALLOCATION_KERNEL_OAL_H__

#include "oal_allocator.h"
#include "oal_utils.h"

#ifndef ULONG_MAX
#define ULONG_MAX 0xFFFFFFFFFFFFFFFFUL
#endif

__BEGIN_DECLS

/**
 * @brief Allocates a contiguous memory block.
 *
 * Allocates a block of memory with specified size and alignment
 * and add an item into management list.
 *
 * @param   size    [in] Amount of memory to allocate in bytes
 * @param   align   [in] Alignment of allocated block
 * @param   chunk_id [in] Memory region where the allocation should take place (see oal_chunk_ids.h)
 * @param   pid     [in] Process id
 * @returns allocated HW address if success, 0 otherwise.
 */
uint64_t oal_alloc(uint64_t size, uint64_t align, uint8_t chunk_id, int32_t pid);


/**
 * @brief Removes a contiguous memory block.
 *
 * Function removes a contiguous memory block and
 * removes an entry from the management list.
 * Does not free virtual mappings.
 *
 * @param   handle_pointer  [in] handle to memory block
 * @returns 0 if success and ULONG_MAX if fail.
 */
uint64_t oal_free (uint64_t handle_pointer);

uint64_t oal_free_phys (uint64_t physical_pointer);

/**
 * Function returns number of allocations.
 */
uint64_t oal_get_num_allocations (void);

/**
 * Function returns number of mappings.
 */
uint64_t oal_get_num_mappings (void);

/**
 * Function returns a size of allocated memory block.
 */
uint64_t oal_getsize(uint64_t handle_pointer);

/**
 * Function returns a specified buffer based on handle pointer.
 */
uint64_t oal_getbuffer(uint64_t handle_pointer, uint64_t type);

/* Function returns total size in bytes being managed by OAL Memory */
int64_t oal_memorysizetotal(void);
/* Function returns free size in bytes remaining in OAL Memory */
int64_t oal_memorysizefree(void);

/* Function to get memory information */
uint8_t oal_get_chunkid(uint64_t handle_pointer);

__END_DECLS

#endif
