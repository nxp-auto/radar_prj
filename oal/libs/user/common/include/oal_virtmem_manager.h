/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_VIRTMEM_MANAGER_H
#define OAL_VIRTMEM_MANAGER_H

#include <oal_utils.h>
#include <oal_free_mem_tree.h>
#include <oal_list.h>
#include <oal_memory_allocator.h>

__BEGIN_DECLS

/**
 * @brief Memory mappings
 */
enum OAL_MemoryMapping {
	OAL_VIRT_NONE,           ///< Not mapped
	OAL_VIRT_CACHEABLE,      ///< Mapped as cacheable memory region
	OAL_VIRT_NON_CACHEABLE,  ///< Mapped as non-cacheable memory region
	OAL_VIRT_INVALID,        ///< Invalid entry
};

struct OSVirtMemAllocation;
struct PhysicalPageMapping;

struct OAL_PhysMemLink {
	struct PhysicalPageMapping
	    *mpPhysAlloc;   ///< Back link to #PhysicalPageMapping
	uintptr_t mOffset;  ///< Offset in #mpPhysAlloc
};

/**
 * @brief Description of a memory handle
 * @note This implementation is opaque in <tt>oal_memory_allocator.h</tt>
 */
struct OAL_MemoryHandle {
	LIST_ENTRY(OAL_MemoryHandle)
	mVirtEntry;  ///< Member of #PhysicalPageMapping list
	struct OSVirtMemAllocation *mpOSData;  ///< OS specific data
	struct PhysicalPageMapping
	    *mpPhysAlloc;   ///< Back link to #PhysicalPageMapping
	uintptr_t mOffset;  ///< Offset in #mpPhysAlloc

	enum OAL_MemoryMapping mMapping;  ///< Type of the mapping
	uintptr_t mSize;                  ///< The allocated size
};

struct OSPhysicalPageMapping;
struct MemoryChunk;

/**
 * @brief Physical allocation contains a list of virtual allocations.
 */
struct PhysicalPageMapping {
	LIST_ENTRY(PhysicalPageMapping)
	mChunkEntry;  ///< Member of #MemoryChunk list
	LIST_HEAD(, OAL_MemoryHandle) mVirtList;  ///< List of physical mappings
	struct OSPhysicalPageMapping *mpOSData;   ///< OS specific data
	struct MemoryChunk *mpChunk;              ///< Back link to #MemoryChunk

	uintptr_t mPhysAddr;  ///< Address of the physical area
	uintptr_t mSize;      ///< Allocated size, multiple of physical page
	uintptr_t mTag;       ///< Physical tag (index of the current instance)
};

/**
 * @brief Process representation of the reserved memory chunks.
 * Each memory chunk contains a list with all physical allocations
 * (multiple of PAGESIZE).
 */
struct MemoryChunk {
	uint32_t mId;                     ///< Chunk ID
	uint32_t mAlign;                  ///< chunk alignment
	struct TreeNode *mpCacheTree;     ///< Availability tree containing free
	                                  /// virtual memory regions - virtual
	                                  /// cacheable
	struct TreeNode *mpNonCacheTree;  ///< Availability tree containing free
	                                  /// virtual memory regions - virtual
	                                  /// non-cacheable
	LIST_HEAD(, PhysicalPageMapping)
	mAllocList;  ///< List of physical mappings
};

/**
 * @brief Check and return the chunk based on a chunk ID
 *
 * @param[in]  aChunkId The ID of the chunk
 * @param[out] apChunk  NULL if the chunk wasn't found, a valid chunk otherwise
 *
 * @return 1 if the chunk with <tt>aChunkId</tt> is available,
 * 0 otherwise
 */
uint8_t OAL_IsChunkAvailable(uint8_t aChunkId, struct MemoryChunk **apChunk);

/**
 * @brief Equivalent of <tt>virt_to_phys</tt>. Search through all mappings of
 * a chunk based on a virtual address <tt>lpVirtAddr</tt> in order go get its
 * physical memory area.
 *
 * @param[in] apMemChunk The source chunk
 * @param[in] apVirtAddr The mapped virtual address
 * @param[out] apPhys    Physical memory region
 * @param[out] apOffset  Offset if <tt>apPhys</tt>
 *
 * @return 0 if the operation succeeded, a negative value otherwise
 */
int32_t OAL_GetPhysicalPageMapping(struct MemoryChunk *apMemChunk,
                                   void *apVirtAddr,
                                   enum OAL_MemoryMapping aMapping,
                                   struct PhysicalPageMapping **apPhys,
                                   uintptr_t *apOffset);

/*
 * Utility functions for #PhysicalPageMapping, #OAL_MemoryHandle and
 * #MemoryChunk
 * pools.
 */
int32_t OAL_InitializeMemoryPools(void);
int32_t OAL_GetUnusedPhysMemAllocation(
    struct PhysicalPageMapping **apPhysAllocation);
int32_t OAL_GetUnusedVirtMemAllocation(struct OAL_MemoryHandle **apAllocation);
void OAL_SetUnusedVirtMemAllocation(struct OAL_MemoryHandle *apAllocation);
void OAL_SetUnusedPhysMemAllocation(
    struct PhysicalPageMapping *apPhysAllocation);
int32_t OAL_GetUnusedMemoryChunk(struct MemoryChunk **apMemChunk);
void OAL_SetUnusedMemoryChunks(struct MemoryChunk *apMemChunk);
void OAL_GetNextUsedMemAllocation(struct OAL_MemoryHandle **apAllocation);

/**
 * @brief Print the content of a chunk
 *
 * @param[in] apMemChunk The chunk to be dumped
 */
void OAL_DumpMemChunk(struct MemoryChunk *apMemChunk);

/**
 * @brief Print all virtual mapping of a #PhysicalPageMapping
 *
 * @param[in] apPhys The physical memory region
 */
void OAL_DumpPhysicalPageMapping(struct PhysicalPageMapping *apPhys);

__END_DECLS

#endif
