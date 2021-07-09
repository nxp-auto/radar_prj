/*
 * Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_DEVM_MEMORY_MAP_H
#define OAL_DEVM_MEMORY_MAP_H

#include <stddef.h>
#include <stdint.h>

__BEGIN_DECLS

/*
 * User space utility functions to map memory from /dev/mem
 */

/**
 * @brief Bind a memory area defined by a physical address (offset)
 * and a size to a virtual address.
 *
 * @param[in] aOffset The physical address
 * @param[in] aSize   The size to be mapped
 *
 * @return The virtual address
 */
uintptr_t OAL_MapDevm(uint64_t aOffset, size_t aSize);

/**
 * @brief Unmap the memory reserved with
 * #OAL_MapDevm
 *
 * @param[in] aVirtAddr The virtual address
 * @param[in] aSize     The size
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int32_t OAL_UnmapDevm(uintptr_t aVirtAddr, size_t aSize);

__END_DECLS

#endif /* OAL_DEVM_MEMORY_MAP_H */
