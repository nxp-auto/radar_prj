/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_ADD_RESERVED_MEM_H
#define OAL_ADD_RESERVED_MEM_H

#include <oal_utils.h>

__BEGIN_DECLS

/**
 * @brief Add a reserved memory region to pools managed by OAL driver.
 *
 * This function represents an alternative to OAL memory driver initialization
 * with DTB.
 *
 * @param[in] apName       Name of the region
 * @param[in] aPhysAddr    Start address
 * @param[in] aSize        Size of the memory region
 * @param[in] aStartID     ID of the pool
 * @param[in] aAlign       Alignment, in bytes
 * @param[in] aInit        1U for memory zeroization, 0U otherwise
 * @param[in] aAutobalance 1U if it's part of an autobalanced memory
 *
 * @return
 *    * 0 if the operation completed successfully
 *    * a negative value otherwise
 */
int32_t OAL_AddReservedMemoryRegion(char8_t *apName, uintptr_t aPhysAddr,
                                    uintptr_t aSize, uint32_t aStartID,
                                    uint32_t aAlign, uint8_t aInit,
                                    uint8_t aAutobalance);

__END_DECLS

#endif /* OAL_ADD_RESERVED_MEM_H */
