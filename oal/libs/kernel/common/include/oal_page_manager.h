/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_PAGE_MANAGER_H
#define OAL_PAGE_MANAGER_H

#include <oal_kernel_memory_allocator.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <oal_utils.h>
#include <os_oal_page_manager.h>

#define OAL_AUTOBALANCE_CHUNK_ID 0xffU

__BEGIN_DECLS

/**
 * @brief Physical memory pool where the smallest chunk is represented
 * by a page
 */
struct OALMemoryChunk {
	struct OAL_MemoryAllocatorRegion
	    mMemoryRegion;                 ///< Pool description from fdt
	struct OSOALMemoryChunk mOSChunk;  ///< OS specific data
	OAL_DECLARE_STATIC_POOL_UNINITIALIZED(
	    mPagesPool,
	    OAL_MAX_REGION_SIZE /
	        OAL_PAGE_SIZE);  ///< A bitset with all pages from a chunk
	uint64_t mFreeSpace;
	uint64_t mSearchHint;
};

/**
 * The page allocator mechanism is based on multiple pools described by
 * #OALMemoryChunk.All of them must be initialized. This is what this function
 * does.
 *
 * @param[in] aForce Force pools re-initialization
 *
 * @return 0 if the call ended successfully, a negative value otherwise
 */
int32_t OAL_InitMemoryPools(uint8_t aForce);


/**
 * @brief Deinitializes all added memory pools
 */
void OAL_ResetMemoryPools(void);

/**
 * @brief Get a pointer to all memory pools. An iteration over all memory pools
 * can be made based on the returned pointer.
 *
 * @return A pointer to all memory pools
 */
struct OALMemoryChunk *OAL_GetMemoryPools(void);

/**
 * @brief Get number of initialized pools.
 *
 * @return Number of initialized pools
 */
uint32_t OAL_GetNumberOfPools(void);

/**
 * @brief Get chunk's base address
 *
 * @param[in] aChunkId    Chunk's ID
 * @param[out] apBaseAddr Base address of the chunk
 *
 * @return 0 if the call ended successfully, a negative value otherwise
 */
int32_t OAL_GetChunkBaseAddress(uint64_t aChunkId, uint64_t *apBaseAddr);

/**
 * @brief Get chunk's size
 *
 * @param[in] aChunkId    Chunk's ID
 * @param[out] apBaseSize Size of the chunk
 *
 * @return 0 if the call ended successfully, a negative value otherwise
 */
int32_t OAL_GetChunkSize(uint64_t aChunkId, uint64_t *apSize);

/**
 * @brief Check if the chunk is auto-balanced
 *
 * @param[in] aChunkId    Chunk's ID
 * @param[out] apBalanced <tt>1</tt> is the chunk has auto-balance flag,
 *                        <tt>0</tt> otherwise
 *
 * @return 0 if the call ended successfully, a negative value otherwise
 */
int32_t OAL_IsChunkAutobalanced(uint64_t aChunkId, uint32_t *apBalanced);

/**
 * @brief Add and initialize a memory pool based on the given
 * #OAL_ReservedMemory.
 *
 * @param[in] apResMemory  Initialization data for the memory pool
 * @param[out] apMemChunk  A pointer to the pool
 *
 * @return 0 if the call ended successfully, a negative value otherwise
 */
int32_t OAL_AddMemoryPool(struct OAL_MemoryAllocatorRegion *apResMemory,
                      struct OALMemoryChunk **apMemChunk);

/**
 * @brief Get a memory Chunk based on its ID
 *
 * @param[in] aChunkID     Chunk ID
 * @param[out] apMemChunk  A pointer to the pool with the given ID
 *
 * @return 0 if the call ended successfully, a negative value otherwise
 */
int32_t OAL_GetMemoryPool(uint32_t aChunkID, struct OALMemoryChunk **apMemChunk);

/**
 * @brief Allocate a chunk using contiguous physical pages
 *
 * @param[in] aChunkId       Chunk ID
 * @param[in] aSize          The size to be allocated, OAL_PAGE_SIZE aligned
 * @param[out] apStartIndex  Index of the first page
 * @param[out] apPages       Number of allocated pages
 * @param[out] apMemChunk    A reference to the requested chunk, associated to
 *                           <tt>aChunkId</tt>
 *
 * @return 0 if the call ended successfully, a negative value otherwise
 */
int32_t OAL_GetFreePagesFromChunk(uint32_t aChunkId, uint64_t aSize,
                              uint64_t *apStartIndex, uint64_t *apPages,
                              struct OALMemoryChunk **apMemChunk);

/**
 * @brief Release pages allocated using #OAL_GetFreePagesFromChunk.
 *
 * @param[in] apChunk       Chunk reference
 * @param[in] aStartIndex   The index of the first page
 * @param[in] aPages        Number of pages to be released
 *
 * @return 0 if the call ended successfully, a negative value otherwise
 */
int32_t OAL_SetFreePages(struct OALMemoryChunk *apChunk, uint64_t aStartIndex,
                     uint64_t aPages);

/**
 * @brief Identify memory chunk based on a physical range described by
 * a start address <tt>aPhysAddr</tt> and size <tt>aSize</tt>.
 *
 * @param[in]  aPhysAddr   Start address of the range
 * @param[in]  aSize       Size of the physical memory region
 * @param[out] apMemChunk  A reference to the chunk
 *
 * @return 0 if the call ended successfully, a negative value otherwise
 */
int32_t OAL_GetMemoryChunkBasedOnAddr(uint64_t aPhysAddr, uint64_t aSize,
                                  struct OALMemoryChunk **apMemChunk);
__END_DECLS

#endif /* OAL_PAGE_MANAGER_H */
