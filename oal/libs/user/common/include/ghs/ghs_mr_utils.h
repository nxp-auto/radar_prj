/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_GHS_MR_UTILS_H
#define OAL_GHS_MR_UTILS_H

#include <oal_utils.h>

__BEGIN_DECLS

/**
 * @brief Allocate a virtual memory region and map it to <tt>apPhysMR</tt>
 *
 * @param[in]  aSize        The size of the virtual memory region
 * @param[out] apVirtMR     Mapped virtual memory region
 * @param[in]  apPhysMR     Physical memory region
 * @param[in]  aUsage       Copy or not usage of <tt>apPhysMR</tt>
 *
 * @return <tt>Success</tt> if the operation succeeds, a negative value
 * otherwise
 */
Error OAL_GHS_AllocAndMapMemoryReg(uint64_t aSize, MemoryRegion *apVirtMR,
                                   MemoryRegion *apPhysMR, Value aUsage);

/**
 * @brief Unmap a virtual memory region allocated and mapped by
 * #OAL_GHS_AllocAndMapMemoryReg
 *
 * @param[in] aVirtMR     Mapped virtual memory region
 *
 * @return <tt>Success</tt> if the operation succeeds, a negative value
 * otherwise
 */
Error OAL_GHS_UnmapAndReleaseVirtual(MemoryRegion aVirtMR);

__END_DECLS
#endif
