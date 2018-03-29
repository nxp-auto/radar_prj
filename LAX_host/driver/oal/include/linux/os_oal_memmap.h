/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OS_OAL_MEMMAP_H
#define OS_OAL_MEMMAP_H

#include "oal_utils.h"

#ifdef __KERNEL__
#include <linux/io.h>
#else
#include "posix/devmem_map.h"
#endif

#include "oal_utils.h"
#include "priv_oal_memmap.h"

__BEGIN_DECLS

static inline void *OAL_memmap(uint64_t offset, size_t size, enum OALMapSource src)
{
	UNUSED_ARG(src);
#ifdef __KERNEL__
	return ioremap(offset, size);
#else
	return devm_map(offset, size);
#endif
}


static inline void OAL_memunmap(void *addr, size_t size, enum OALMapSource src)
{
	UNUSED_ARG(src);
#ifdef __KERNEL__
	UNUSED_ARG(size);
	iounmap(addr);
#else
	devm_unmap(addr, size);
#endif
}

static inline uint8_t OAL_read8(void *addr)
{
#ifdef __KERNEL__
	void __iomem *_addr = addr;
	return readb(_addr);
#else
	return *((uint8_t*)addr);
#endif
}

static inline uint16_t OAL_read16(void *addr)
{
#ifdef __KERNEL__
	void __iomem *_addr = addr;
	return readw(_addr);
#else
	return *((uint16_t*)addr);
#endif
}

static inline uint32_t OAL_read32(void *addr)
{
#ifdef __KERNEL__
	void __iomem *_addr = addr;
	return readl(_addr);
#else
	return *((uint32_t*)addr);
#endif
}

static inline uint64_t OAL_read64(void *addr)
{
#ifdef __KERNEL__
	void __iomem *_addr = addr;
	return readq(_addr);
#else
	return *((uint64_t*)addr);
#endif
}

static inline void OAL_write8(void *addr, uint8_t value)
{
#ifdef __KERNEL__
	void __iomem *_addr = addr;
	writeb(value, _addr);
#else
	*((uint8_t*)addr) = value;
#endif
}

static inline void OAL_write16(void *addr, uint16_t value)
{
#ifdef __KERNEL__
	void __iomem *_addr = addr;
	writew(value, _addr);
#else
	*((uint16_t*)addr) = value;
#endif
}

static inline void OAL_write32(void *addr, uint32_t value)
{
#ifdef __KERNEL__
	void __iomem *_addr = addr;
	writel(value, _addr);
#else
	*((uint32_t*)addr) = value;
#endif
}

static inline void OAL_write64(void *addr, uint64_t value)
{
#ifdef __KERNEL__
	void __iomem *_addr = addr;
	writeq(value, _addr);
#else
	*((uint64_t*)addr) = value;
#endif
}

__END_DECLS

#endif /* OS_OAL_MEMMAP_H */

