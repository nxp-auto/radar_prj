/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_driver_functions.h>
#include <os_oal_memory_allocator.h>
#include <os_oal_shared_memory.h>
#include <posix_oal_memory_allocator.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define OAL_PAGE_SIZE (0x1000U)

static int32_t gsOalNonCacheableFd = -1;
static int32_t gsOalCacheableFd    = -1;
static uint32_t gsCacheLineSize    = 0x0U;
static uint32_t gsPageSize         = 0x0U;

int32_t OAL_InitializeMemoryFd(void)
{
	int32_t lRet     = OAL_SUCCESS;
	gsOalCacheableFd = open("/dev/oal_cached", O_RDWR);
	if (gsOalCacheableFd < 0) {
		OAL_LOG_ERROR("Failed to open /dev/oal_cached\n");
		lRet = OAL_FAILURE;
		goto initialize_fd_exit;
	}

	gsOalNonCacheableFd = open("/dev/oal_noncached", O_RDWR);
	if (gsOalNonCacheableFd < 0) {
		OAL_LOG_ERROR("Failed to open /dev/oal_noncached\n");
		lRet = OAL_FAILURE;
	}

	if (lRet != OAL_SUCCESS) {
		(void)close(gsOalCacheableFd);
		gsOalCacheableFd = -1;
	}

initialize_fd_exit:
	return lRet;
}

int32_t OAL_DeinitializeMemoryFd(void)
{
	int32_t lRet = 0, lStatus;
	if (gsOalNonCacheableFd > 0) {
		lStatus = close(gsOalNonCacheableFd);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed to close non-cacheable FD: %s\n",
			              strerror(lStatus));
			lRet = -lStatus;
		}
		gsOalNonCacheableFd = -1;
	}
	if (gsOalCacheableFd > 0) {
		lStatus = close(gsOalCacheableFd);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed to close cacheable FD: %s\n",
			              strerror(lStatus));
			lRet = -lStatus;
		}
		gsOalCacheableFd = -1;
	}

	gsCacheLineSize = 0x0U;
	gsPageSize      = 0x0U;
	return lRet;
}

static int32_t getFd(enum OAL_MemoryMapping aVirtType)
{
	int32_t lFd;
	if (aVirtType == OAL_VIRT_CACHEABLE) {
		lFd = gsOalCacheableFd;
	} else if (aVirtType == OAL_VIRT_NON_CACHEABLE) {
		lFd = gsOalNonCacheableFd;
	} else {
		lFd = -1;
		OAL_LOG_ERROR("Unknown memory flag %d", (int32_t)aVirtType);
	}
	return lFd;
}

int32_t __attribute__((weak))
OAL_MapPhysMem(uint64_t aPhysAddr, uint64_t aSize, uint8_t **apVirtAddr,
               enum OAL_MemoryMapping aVirtType)
{
	int32_t lRet = 0;
	int32_t lFd;

	lFd = getFd(aVirtType);
	if (lFd < 0) {
		lRet = -EINVAL;
	}

	if ((apVirtAddr != NULL) && (lRet == 0)) {
		*apVirtAddr =
		    mmap(NULL, aSize, PROT_READ | PROT_WRITE | PROT_EXEC,
		         MAP_SHARED, lFd, (off_t)aPhysAddr);
		if (*apVirtAddr == (uint8_t *)(uintptr_t)MAP_FAILED) {
			lRet = -EIO;
		}
	}

	return lRet;
}

int32_t __attribute__((weak))
OAL_MapPhysArea(struct PhysicalPageMapping *apPhys, uint8_t **apVirtAddr,
                enum OAL_MemoryMapping aVirtType)
{
	return OAL_MapPhysMem(apPhys->mPhysAddr, apPhys->mSize, apVirtAddr,
	                      aVirtType);
}

int32_t __attribute__((weak))
OAL_UnmapMemArea(struct PhysicalPageMapping *apPhys, uint8_t *apVirtAddr)
{
	int32_t lRet = 0;
	if ((apVirtAddr != NULL) && (apPhys != NULL)) {
		(void)OAL_FlushAndInvalidateMemory((uintptr_t)apVirtAddr,
		                                   (uint32_t)apPhys->mSize);
		lRet = munmap(apVirtAddr, (size_t)apPhys->mSize);
	} else {
		lRet = -EINVAL;
	}

	return lRet;
}

uint32_t __attribute__((weak)) OAL_OS_GetCacheLineSize(void)
{
	if (gsCacheLineSize == 0U) {
		(void)OAL_Initialize();
		if (gOalH != NULL) {
			if (OAL_SimpleOutCall(gOalH,
			                      (uint32_t)CMD_GET_CACHE_LINE,
			                      gsCacheLineSize) != 0U) {
				gsCacheLineSize = 0;
			}
		} else {
			OAL_LOG_ERROR(
			    "OAL: Initialization failed. "
			    "Please check the OAL kernel module.\n");
		}
	}

	if (gsCacheLineSize == 0U) {
		gsCacheLineSize = OAL_L1_CACHE_LINE_SIZE;
	}

	return gsCacheLineSize;
}

uint32_t __attribute__((weak)) OAL_OS_GetPageSize(void)
{
	if (gsPageSize == 0U) {
		(void)OAL_Initialize();
		if (gOalH != NULL) {
			if (OAL_SimpleOutCall(gOalH,
			                      (uint32_t)CMD_GET_PAGE_SIZE,
			                      gsPageSize) != 0U) {
				gsPageSize = 0;
			}
		} else {
			OAL_LOG_ERROR(
			    "OAL: Initialization failed. "
			    "Please check the OAL kernel module.\n");
		}
	}

	if (gsPageSize == 0U) {
		gsPageSize = OAL_PAGE_SIZE;
	}
	return gsPageSize;
}

int32_t __attribute__((weak))
OAL_OS_FlushMemory(uintptr_t aAddr, uint32_t aSize)
{
	uint32_t lRet = 0;
	CMD_FLUSH_SPECIFIC_TYPE lDrvCmd;
	uint8_t *lpAddr = (uint8_t *)aAddr;

	lDrvCmd.mVirtualPointer = (uintptr_t)lpAddr;
	lDrvCmd.mSize           = aSize;

	lRet = OAL_SimpleInCall(gOalH, (uint32_t)CMD_FLUSH_SPECIFIC, lDrvCmd);
	if (lRet != 0U) {
		OAL_LOG_ERROR(
		    "OAL: Initialization failed. "
		    "Please check the OAL kernel module.\n");
	}
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_InvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	uint32_t lRet = 0;
	CMD_FLUSH_SPECIFIC_TYPE lDrvCmd;
	uint8_t *lpAddr = (uint8_t *)aAddr;

	lDrvCmd.mVirtualPointer = (uintptr_t)lpAddr;
	lDrvCmd.mSize           = aSize;

	lRet =
	    OAL_SimpleInCall(gOalH, (uint32_t)CMD_INVALIDATE_SPECIFIC, lDrvCmd);
	if (lRet != 0U) {
		OAL_LOG_ERROR(
		    "OAL: Initialization failed. "
		    "Please check the OAL kernel module.\n");
	}
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_FlushAndInvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	uint32_t lRet = 0;
	CMD_FLUSH_SPECIFIC_TYPE lDrvCmd;
	uint8_t *lpAddr = (uint8_t *)aAddr;

	lDrvCmd.mVirtualPointer = (uintptr_t)lpAddr;
	lDrvCmd.mSize           = aSize;

	lRet = OAL_SimpleInCall(gOalH, (uint32_t)CMD_FLUSHINVALIDATE_SPECIFIC,
	                        lDrvCmd);
	if (lRet != 0U) {
		OAL_LOG_ERROR(
		    "OAL: Initialization failed. "
		    "Please check the OAL kernel module.\n");
	}
	return lRet;
}
