/*
 * Copyright 2017-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "oal_log.h"
#include "posix/devmem_map.h"

#define DEV_MEM "/dev/mem"

#ifndef PROT_NOCACHE
#define PROT_NOCACHE 0
#endif

uintptr_t OAL_MapDevm(uint64_t aOffset, size_t aSize)
{
	uintptr_t lMapAddr = 0U;
	int32_t lRet;

	int32_t lFd     = open(DEV_MEM, O_RDWR);
	uint32_t lProto = (((uint16_t)PROT_READ) | ((uint16_t)PROT_WRITE) |
	                   ((uint16_t)PROT_NOCACHE));

	if (lFd < 0) {
		OAL_LOG_ERROR("Failed to open /dev/mem\n");
		goto map_dev_mem_exit;
	}

	// if failed to open /dev/mem
	lMapAddr = (uintptr_t)mmap(NULL, aSize, (int32_t)lProto, MAP_SHARED,
	                           lFd, (off_t)aOffset);

	if (lMapAddr == (uintptr_t)MAP_FAILED) {
		lMapAddr = 0U;
		OAL_LOG_ERROR("Can't map memory: %s\n", strerror(errno));
		lRet = close(lFd);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to close /dev/mem\n");
		}
		goto map_dev_mem_exit;
	}

	lRet = close(lFd);
	if (lRet != 0) {
		(void)munmap((void *)lMapAddr, aSize);
		lMapAddr = 0U;
		OAL_LOG_ERROR("Failed to close /dev/mem\n");
	}

map_dev_mem_exit:
	return lMapAddr;
}

int32_t OAL_UnmapDevm(uintptr_t aVirtAddr, size_t aSize)
{
	int32_t lRet = munmap((void *)aVirtAddr, aSize);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to unmap /dev/mem memory %s\n",
		              strerror(errno));
	}

	return lRet;
}
