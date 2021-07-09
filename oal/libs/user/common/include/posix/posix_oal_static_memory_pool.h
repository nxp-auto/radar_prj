/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_POSIX_STATIC_MEMORY_POOL_H
#define OAL_POSIX_STATIC_MEMORY_POOL_H

#include <oal_utils.h>

__BEGIN_DECLS

/**
 * @brief Allocate memory from a common memory pool stored statically
 *
 * @param[in]  aSize  Needed size
 * @param[out] apPtr  A link pointing to the beginning of the requested area
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_AllocFromMemoryPool(size_t aSize, uint8_t **apPtr);


/**
 * @brief Deallocate the memory area received after an
 * \ref OAL_AllocFromMemoryPool call
 *
 * @param[in] aSize The size of the allocated area
 * @param[in] apPtr The beginning of the memory to be released
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_ReleasePoolMemory(size_t aSize, uint8_t *apPtr);

__END_DECLS

#endif /* OAL_POSIX_STATIC_MEMORY_POOL_H */
