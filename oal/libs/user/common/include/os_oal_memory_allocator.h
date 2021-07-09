/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OS_MEMORY_ALLOCATOR_H
#define OAL_OS_MEMORY_ALLOCATOR_H

#include <oal_utils.h>
#include <oal_common.h>
#include <oal_virtmem_manager.h>

__BEGIN_DECLS

/**
 * @brief Initializes OAL (public call deprecated)
 *
 * @return
 *    - On Success: OAL_SUCCESS
 *    - On Failure: OAL_FAILURE
 */
int32_t OAL_OS_Initialize(void);

/**
 * @brief Deinitializes OAL (public call deprecated)
 *
 * @return
 *    - On Success: OAL_SUCCESS
 *    - On Failure: OAL_FAILURE
 */
int32_t OAL_OS_Deinitialize(void);

/**
 * @brief Allocate physical memory
 *
 * @note The implementation of this function will fill the following fields of
 * <tt>struct PhysicalPageMapping *apMapping</tt> :
 *   * <tt>mpOSData</tt>
 *   * <tt>mSize</tt>
 *
 * @param[in/out] apMapping Physical Memory region
 * @param[in/out] apSize    The requested and allocated size
 * @param[in/out] apAlign   Memory alignment
 * @param[in] aFlags        Memory flags
 * @param[in] aChunkId      Memory Chunk ID
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_OS_AllocPhysicalMemory(struct PhysicalPageMapping *apMapping,
                                   uintptr_t *apSize, uint32_t *apAlign,
                                   OAL_MEMORY_FLAG aFlags, uint32_t aChunkId);

/**
 * @brief Release previously allocated memory using #OAL_OS_AllocPhysicalMemory()
 *
 * @param[in] apMapping The mapping returned by #OAL_OS_AllocPhysicalMemory()
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_OS_ReleasePhysicalMemory(struct PhysicalPageMapping *apMapping);

/**
 * @brief Map virtual memory to physical
 *
 * @param[in] apPhys     Physical Memory region
 * @param[in,out] apVirt Virtual memory region
 * @param[in] aMapping   Mapping type
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 * @note This function will set <tt>mpOSData</tt> field of <tt>apVirt</tt>.
 */
int32_t OAL_OS_MapPhysicalMemory(struct PhysicalPageMapping *apPhys,
                                 struct OAL_MemoryHandle *apVirt,
                                 enum OAL_MemoryMapping aMapping);

/**
 * @brief Unmap memory mapped using #OAL_OS_MapPhysicalMemory()
 *
 * @param[in] apVirt Virtual memory region
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_OS_UnmapPhysicalMemory(struct OAL_MemoryHandle *apVirt);

/**
 * @brief Retrieve the cacheable virtual address of a mapping resulted after
 * a ##OAL_OS_MapPhysicalMemory() call
 *
 * @param[in] apVirt   Virtual memory region
 * @param[out] apAddr  Start address of <tt>apVirt</tt>
 * @param[in] aMapping Mapping type
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_OS_GetVirtAddress(struct OAL_MemoryHandle *apVirt, uint8_t **apAddr,
                              enum OAL_MemoryMapping aMapping);

/**
 * @brief Copy all OS attributes from <tt>apSrcVirt</tt> to <tt>apDstVirt</tt>
 *
 * @param[in,out] apDstVirt The destination
 * @param[in] apSrcVirt     The source of the attributes
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_OS_CopyMappingInfo(struct OAL_MemoryHandle *apDstVirt,
                               struct OAL_MemoryHandle *apSrcVirt);

/**
 * @brief Flush a memory area
 *
 * @param[in] aAddr Start address of the memory region
 * @param[in] aSize  Size of the region
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int32_t OAL_OS_FlushMemory(uintptr_t aAddr, uint32_t aSize);

/**
 * @brief Invalidate caches for a memory area
 *
 * @param[in] aAddr Start address of the memory region
 * @param[in] aSize  Size of the region
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int32_t OAL_OS_InvalidateMemory(uintptr_t aAddr, uint32_t aSize);

/**
 * @brief Flush and invalidate caches for a memory area
 *
 * @param[in] aAddr Start address of the memory region
 * @param[in] aSize  Size of the region
 *
 * @return 0 if the call ends successfully, a negative value otherwise
 */
int32_t OAL_OS_FlushAndInvalidateMemory(uintptr_t aAddr, uint32_t aSize);

/**
 * @brief Get system's cache line size
 *
 * @return The size of a cache line
 */
uint32_t OAL_OS_GetCacheLineSize(void);

/**
 * @brief Get size of a physical memory page
 *
 * @return The size of the page
 */
uint32_t OAL_OS_GetPageSize(void);

__END_DECLS

#endif
