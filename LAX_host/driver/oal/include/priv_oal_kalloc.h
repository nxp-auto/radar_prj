/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PRIV_OAL_KALLOC_H
#define PRIV_OAL_KALLOC_H

/**
 * @brief Flags used for memory allocation
 */
enum kflags {
	KERNEL_MEM, ///< Kernel memory
	DMA_MEM, ///< Allocation for DMA
	VIRT_MEM, ///< Virtual memory
};

#endif /* PRIV_OAL_KALLOC_H */
