/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_SHARED_MEMORY_H
#define OAL_SHARED_MEMORY_H

#include <oal_memory_allocator.h>

__BEGIN_DECLS

/**
 * @addtogroup OAL_MemoryAllocator
 *
 * @{
 * @name Shared memory handler across processes
 *
 * This is an add-on of @ref OAL_MemoryAllocator that allows sharing a memory
 * allocation made from a reserved region across multiple processes using a
 * token.
 *
 * The token represents a global identifier for a memory allocation created on
 * demand. The lifetime of a memory allocation and permitted operations aren't
 * influenced at all by a share, an identifier that represents it at global
 * level. On the other hand, the lifetime of a token is tightly connected
 * to the memory allocation, it shouldn't be used after the release of the
 * linked memory handle.
 *
 * @warning A token is just a link to the original allocation. The user must
 * add his own protection and synchronization logic in order to ensure that the
 * token is used / released after the memory handler / region is released.
 *
 * @warning The user shall protect himself from double free, synchronous memory
 * writes from multiple processes and any other possible concurrent operations
 * on shared memory / buffers.
 *
 */

/**
 * @brief Mark a memory reservation as shared across processes
 *
 * Mark the memory handle as shared and initialize a token that will identify
 * \p apMemHandle at global / system level.
 *
 * @param[in] apMemHandle      The memory handle to be shared
 * @param[out] apToken         The resulted token
 *
 * @return
 *     - <tt>0</tt> for success
 *     - a negative value otherwise
 */
int32_t OAL_CreateMemoryToken(struct OAL_MemoryHandle *apMemHandle,
                              uintptr_t *apToken);

/**
 * @brief Map a memory token
 *
 * Map a memory token in the context of the current process based
 * on \p aAccess flags and return its address.
 *
 * @param[in] aToken           The identifier returned by
 *                             @ref OAL_CreateMemoryToken
 * @param[in] aAccess          Access flags:
 *  - #OAL_ACCESS_PHY   : Returns the physical address of memory.
 *  - #OAL_ACCESS_CH_WB : Returns the address that will be interpreted as cached
 *                        with a  write back policy.
 *  - #OAL_ACCESS_NCH_NB: Returns the address that will be interpreted as
 *                        non-cached and not buffered.
 * @param[out] apMappedAddress Address resulted after mapping based on flags
 *
 * @return
 *     - <tt>0</tt> for success
 *     - a negative value otherwise
 */
int32_t OAL_MapMemoryToken(uintptr_t aToken, OAL_MEMORY_ACCESS aAccess,
                           uintptr_t *apMappedAddress);

/**
 * @brief Unmap all mappings associated with a memory token
 *
 * @param[in] aToken The token to be unmapped
 *
 * @return
 *     - <tt>0</tt> for success
 *     - a negative value otherwise
 */
int32_t OAL_UnmapMemoryToken(uintptr_t aToken);

/**
 * @brief Release a token
 *
 * Release all resources acquired by a token at global level.
 * No other process will be able to map the token into their context.
 *
 * @param[in] aToken The token to be released
 *
 * @return
 *     - <tt>0</tt> for success
 *     - a negative value otherwise
 */
int32_t OAL_ReleaseToken(uintptr_t aToken);

/** @} */

/** @} */

__END_DECLS

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_shared_memory.h>
#endif
#endif
