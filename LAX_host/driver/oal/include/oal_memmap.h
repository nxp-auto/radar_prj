/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_MEMMAP_H
#define OAL_MEMMAP_H

#include "oal_utils.h"
#include "priv_oal_memmap.h"
#include "common_stringify_macros.h"
#include XSTR(OS/os_oal_memmap.h)

__BEGIN_DECLS

/* Unified interface for memory map operations. There are two kinds
 * of mappingis from the interface's perspective: kernel and user space.
 *
 * Kernel space (KERNEL_MAP) operations are triggered by a kernel module /
 * resource manager and the USER_MAP by a user space library.
 *
 * The driver is supposed to call one of flavor of OAL_memmap function and
 * after get / set IO memory using OAL_read OAL_write functions.
 */

/**
 * @brief OAL_memmap Bind a memory area defined by a physical address (offset)
 * and a size to a virtual address.
 *
 * @param[in] offset The physical address
 * @param[in] size   The size to be mapped
 * @param[in] src    Mapping source
 * @see OALMapSource
 *
 * @return The virtual address
 */

void *OAL_memmap(uint64_t offset, size_t size, enum OALMapSource src);

/**
 * @brief OAL_memunmap Unmap the memory reserved with <code>OAL_memmap</code>
 *
 * @param[in] addr The virtual address
 * @param[in] size The size
 * @param[in] src  Mapping source
 * @see OALMapSource
 */
void OAL_memunmap(void *addr, size_t size, enum OALMapSource src);

/**
 * @brief Map kernel memory
 * Kernel wrapper over OAL_memmap.
 *
 * @param[in] offset The physical address
 * @param[in] size   The size to be mapped
 *
 * @return The virtual address
 */
static inline void *OAL_kmemmap(uint64_t offset, size_t size)
{
	return OAL_memmap(offset, size, KERNEL_MAP);
}

/**
 * @brief Unmap kernel memory
 * Kernel wrapper over OAL_memunmap.
 *
 * @param[in] addr The virtual address
 * @param[in] size The size
 */
static inline void OAL_kmemunmap(void *addr, size_t size)
{
	OAL_memunmap(addr, size, KERNEL_MAP);
}

/**
 * @brief Map user space memory
 * User space wrapper over OAL_memmap.
 *
 * @param[in] offset The physical address
 * @param[in] size   The size to be mapped
 *
 * @return The virtual address
 */
static inline void *OAL_umemmap(uint64_t offset, size_t size)
{
	return OAL_memmap(offset, size, USER_MAP);
}

/**
 * @brief Unmap user space memory
 * User space wrapper over OAL_memunmap.
 *
 * @param[in] addr The virtual address
 * @param[in] size The size
 */
static inline void OAL_umemunmap(void *addr, size_t size)
{
	OAL_memunmap(addr, size, USER_MAP);
}

/**
 * @brief OAL_read8 Read one byte from an IO address obtained using OAL_memmap
 * and friends
 *
 * @param addr[in]  The address obtained using OAL_memmap
 *
 * @return The value read from addr
 */
uint8_t OAL_read8(void *addr);

/**
 * @brief OAL_read16 Read two bytes from an IO address obtained using OAL_memmap
 * and friends
 *
 * @param addr[in]  The address obtained using OAL_memmap
 *
 * @return The value read from addr
 */
uint16_t OAL_read16(void *addr);

/**
 * @brief OAL_read32 Read 4 bytes from an IO address obtained using OAL_memmap
 * and friends
 *
 * @param addr[in]  The address obtained using OAL_memmap
 *
 * @return The value read from addr
 */
uint32_t OAL_read32(void *addr);

/**
 * @brief OAL_read64 Read 8 bytes from an IO address obtained using OAL_memmap
 * and friends
 *
 * @param addr[in]  The address obtained using OAL_memmap
 *
 * @return The value read from addr
 */
uint64_t OAL_read64(void *addr);

/**
 * @brief OAL_write8 Write one byte to an IO address obtained using OAL_memmap
 * and friends
 *
 * @param addr[in]  The address obtained using OAL_memmap
 * @param value[in] The value to be written
 */
void OAL_write8(void *addr, uint8_t value);

/**
 * @brief OAL_write16 Write two bytes to an IO address obtained using OAL_memmap
 * and friends
 *
 * @param addr[in]  The address obtained using OAL_memmap
 * @param value[in] The value to be written
 */
void OAL_write16(void *addr, uint16_t value);

/**
 * @brief OAL_write32 Write four bytes to an IO address obtained using OAL_memmap
 * and friends
 *
 * @param addr[in]  The address obtained using OAL_memmap
 * @param value[in] The value to be written
 */
void OAL_write32(void *addr, uint32_t value);

/**
 * @brief OAL_write64 Write four bytes to an IO address obtained using OAL_memmap
 * and friends
 *
 * @param addr[in]  The address obtained using OAL_memmap
 * @param value[in] The value to be written
 */
void OAL_write64(void *addr, uint64_t value);

__END_DECLS

#endif /* OAL_MEMMAP_H */
