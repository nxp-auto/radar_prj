/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OS_OAL_KALLOC_H
#define OS_OAL_KALLOC_H

#include <linux/vmalloc.h>
#include <linux/slab.h>
#include "oal_utils.h"
#include "priv_oal_kalloc.h"

static inline gfp_t to_linux_flags(unsigned flags)
{
	switch (flags) {
		case KERNEL_MEM:
			return GFP_KERNEL;
		case DMA_MEM:
			return GFP_DMA;
		default:
			return GFP_KERNEL;
	};

	return 0;
}

static inline void *OAL_Kalloc(size_t size, unsigned flags)
{
	void *ret;
	if (flags == VIRT_MEM) {
		ret = vmalloc(size);
	} else {
		ret = kmalloc(size, to_linux_flags(flags));
	}

	return ret;
}

static inline void *OAL_Kzalloc(size_t size, unsigned flags)
{
	void *ret;

	if (flags == VIRT_MEM) {
		ret = vzalloc(size);
	} else {
		ret = kzalloc(size, to_linux_flags(flags));
	}

	return ret;
}

static inline void OAL_Kfree(void *addr, unsigned flags)
{
	if (flags == VIRT_MEM) {
		vfree(addr);
	} else {
		kfree(addr);
	}
}

#endif /* OS_OAL_KALLOC_H */
