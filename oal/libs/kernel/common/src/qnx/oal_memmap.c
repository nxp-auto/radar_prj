/*
 * Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/neutrino.h>

#include <oal_memmap.h>

static uintptr_t ioremap(uint64_t aOffset, size_t aSize)
{
	uintptr_t lPtr = 0U;

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, NULL) == -1) {
		OAL_LOG_ERROR("Failed to get control: %s\n", strerror(errno));
	} else {
		lPtr = (uintptr_t)mmap_device_io(aSize, aOffset);
		if (lPtr == MAP_DEVICE_FAILED) {
			OAL_LOG_ERROR(
			    "mmap_device_io for physical address 0x%" PRIx64
			    " failed\n",
			    aOffset);
			lPtr = 0U;
		}
	}

	return lPtr;
}

static void iounmap(uintptr_t aAddr, size_t aSize)
{
	(void)munmap_device_io(aAddr, aSize);
}

uintptr_t OAL_MapSystemMemory(uint64_t aOffset, size_t aSize,
                              enum OALMapSource aSrc)
{
	uintptr_t lRet;
	switch (aSrc) {
		case KERNEL_MAP: {
			lRet = ioremap(aOffset, aSize);
			break;
		}
		case USER_MAP: {
			lRet = ioremap(aOffset, aSize);
			break;
		}
		default: {
			OAL_LOG_ERROR("Invalid map type : %d\n", aSrc);
			lRet = 0U;
			break;
		}
	};

	return lRet;
}

int32_t OAL_UnmapSystemMemory(uintptr_t aAddr, size_t aSize,
                              enum OALMapSource aSrc)
{
	int32_t lRet = 0;
	switch (aSrc) {
		case KERNEL_MAP: {
			iounmap(aAddr, aSize);
			break;
		}
		case USER_MAP: {
			iounmap(aAddr, aSize);
			break;
		}
		default: {
			OAL_LOG_ERROR("Invalid unmap type : %d\n", aSrc);
			lRet = -1;
			break;
		}
	};
	return lRet;
}
