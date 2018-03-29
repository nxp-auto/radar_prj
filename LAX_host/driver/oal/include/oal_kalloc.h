/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_KALLOC_H
#define OAL_KALLOC_H

/* Unified interface for Kernel memory allocation */

#include "priv_oal_kalloc.h"
#include "common_stringify_macros.h"
#include XSTR(OS/os_oal_kalloc.h)

__BEGIN_DECLS
/**
 * @brief OAL_Kalloc Allocate kernel memory
 *
 * @param size[in]  Size of memory area
 * @param flags[in] Allocation flags
 * @see kflags
 *
 * @return Pointer to the allocated memory or NULL
 * on error.
 */
void *OAL_Kalloc(size_t size, unsigned flags);

/**
 * @brief OAL_Kzalloc Allocate memory and set it to zero.
 *
 * @param size[in]  Size of memory area
 * @param flags[in] Allocation flags
 * @see kflags
 *
 * @return Pointer to the allocated memory or NULL
 * on error.
 */
void *OAL_Kzalloc(size_t size, unsigned flags);

/**
 * @brief OAL_Kfree Free the memory allocated with one of the
 * above functions.
 *
 * @param addr Pointer to the allocated memory
 * @param flags[in] Allocation flags used for allocation
 */
void OAL_Kfree(void *addr, unsigned flags);

__END_DECLS
#endif /* OAL_KALLOC_H */


