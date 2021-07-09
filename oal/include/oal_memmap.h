/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_MEMMAP_H
#define OAL_MEMMAP_H

#include <oal_utils.h>
#include <os_oal_memmap.h>

/**
 * @defgroup OAL_MemoryMapping OAL Memory Mapper
 *
 * @{
 * @brief Maps physical addresses
 * @details
 * Unified interface for memory map operations. There are two kinds
 * of mappingis from the interface's perspective: kernel and user space.
 *
 * Kernel space (KERNEL_MAP) operations are triggered by a kernel module /
 * resource manager and the USER_MAP by a user space library.
 */

__BEGIN_DECLS

enum OALMapSource {
	KERNEL_MAP,  ///< For kernel space operations
	USER_MAP,    ///< For user space operations
};

/**
 * @brief Bind a memory area defined by a physical address (offset)
 * and a size to a virtual address.
 *
 * @param[in] aOffset The physical address
 * @param[in] aSize   The size to be mapped
 * @param[in] aSrc    Mapping source
 *
 * @return The virtual address
 */

uintptr_t OAL_MapSystemMemory(uint64_t aOffset, size_t aSize,
                              enum OALMapSource aSrc);

/**
 * @brief Unmap the memory reserved with #OAL_MapSystemMemory
 *
 * @param[in] aAddr The virtual address
 * @param[in] aSize  The aSize
 * @param[in] aSrc   Mapping source
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_UnmapSystemMemory(uintptr_t aAddr, size_t aSize,
                              enum OALMapSource aSrc);

/**
 * @brief Map kernel memory
 * Kernel wrapper over #OAL_MapSystemMemory.
 *
 * @param[in] aOffset The physical address
 * @param[in] aSize   The aSize to be mapped
 *
 * @return The virtual address
 */
static inline uintptr_t OAL_MapKernelSpace(uint64_t aOffset, size_t aSize)
{
	return OAL_MapSystemMemory(aOffset, aSize, KERNEL_MAP);
}

/**
 * @brief Unmap kernel memory
 * Kernel wrapper over #OAL_UnmapSystemMemory.
 *
 * @param[in] aAddr The virtual address
 * @param[in] aSize The aSize
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
static inline int32_t OAL_UnmapKernelSpace(uintptr_t aAddr, size_t aSize)
{
	return OAL_UnmapSystemMemory(aAddr, aSize, KERNEL_MAP);
}

/**
 * @brief Map user space memory
 * User space wrapper over #OAL_MapSystemMemory.
 *
 * @param[in] aOffset The physical address
 * @param[in] aSize   The aSize to be mapped
 *
 * @return The virtual address
 */
static inline uintptr_t OAL_MapUserSpace(uint64_t aOffset, size_t aSize)
{
	return OAL_MapSystemMemory(aOffset, aSize, USER_MAP);
}

/**
 * @brief Unmap user space memory
 * User space wrapper over #OAL_UnmapSystemMemory.
 *
 * @param[in] aAddr The virtual address
 * @param[in] aSize The aSize
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
static inline int32_t OAL_UnmapUserSpace(uintptr_t aAddr, size_t aSize)
{
	return OAL_UnmapSystemMemory(aAddr, aSize, USER_MAP);
}

/**
 * @brief Read one byte from an IO address obtained using #OAL_MapSystemMemory
 *
 *
 * @param[in] aAddr  The address obtained using #OAL_MapSystemMemory
 *
 * @return The value read from \p aAddr
 */
uint8_t OAL_Read8(uintptr_t aAddr);

/**
 * @brief Read two bytes from an IO address obtained using #OAL_MapSystemMemory
 *
 *
 * @param[in] aAddr  The address obtained using #OAL_MapSystemMemory
 *
 * @return The value read from \p aAddr
 */
uint16_t OAL_Read16(uintptr_t aAddr);

/**
 * @brief Read 4 bytes from an IO address obtained using #OAL_MapSystemMemory
 *
 *
 * @param[in] aAddr The address obtained using #OAL_MapSystemMemory
 *
 * @return The value read from \p aAddr
 */
uint32_t OAL_Read32(uintptr_t aAddr);

/**
 * @brief Read 8 bytes from an IO address obtained using #OAL_MapSystemMemory
 *
 *
 * @param[in] aAddr  The address obtained using #OAL_MapSystemMemory
 *
 * @return The value read from \p aAddr
 */
uint64_t OAL_Read64(uintptr_t aAddr);

/**
 * @brief Write one byte to an IO address obtained using #OAL_MapSystemMemory
 *
 *
 * @param[in] aAddr  The address obtained using #OAL_MapSystemMemory
 * @param[in] aValue The value to be written
 */
void OAL_Write8(uintptr_t aAddr, uint8_t aValue);

/**
 * @brief Write two bytes to an IO address obtained using #OAL_MapSystemMemory
 *
 *
 * @param[in] aAddr  The address obtained using #OAL_MapSystemMemory
 * @param[in] aValue The value to be written
 */
void OAL_Write16(uintptr_t aAddr, uint16_t aValue);

/**
 * @brief Write four bytes to an IO address obtained using #OAL_MapSystemMemory
 *
 *
 * @param[in] aAddr  The address obtained using #OAL_MapSystemMemory
 * @param[in] aValue The value to be written
 */
void OAL_Write32(uintptr_t aAddr, uint32_t aValue);

/**
 * @brief Write four bytes to an IO address obtained using #OAL_MapSystemMemory
 *
 *
 * @param[in] aAddr  The address obtained using #OAL_MapSystemMemory
 * @param[in] aValue The value to be written
 */
void OAL_Write64(uintptr_t aAddr, uint64_t aValue);

__END_DECLS

/* @} */

#include <legacy/oal_memmap_1_0.h>

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_memmap.h>
#endif
#endif /* OAL_MEMMAP_H */
