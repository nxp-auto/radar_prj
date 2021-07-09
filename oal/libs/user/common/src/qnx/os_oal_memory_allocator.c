/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_comm.h>
#include <oal_driver_functions.h>
#include <oal_kernel_memory_allocator.h>
#include <oal_mem_constants.h>
#include <oal_mutex.h>
#include <oal_once.h>
#include <os_oal_memory_allocator.h>
#include <os_oal_shared_memory.h>
#include <extra_oal_allocator.h>
#include <posix_oal_memory_allocator.h>

#include <fcntl.h>
#include <sys/cache.h>
#include <sys/mman.h>

typedef struct {
	struct OAL_MemoryAllocatorRegion mDesc;
	int32_t mFd;
} OAL_MemoryReg_t;

/* Lock for below global data */
static struct OAL_Mutex gsInitFdMutex;
static struct OAL_OnceControl gsInitializedFdMutex = OAL_ONCE_INIT;

/* cache info structure */
static struct cache_ctrl gsCacheInfo;
static OAL_MemoryReg_t gsMemRegions[OAL_MAX_RESERVED_REGIONS];
static uint32_t gsNumMemReg = 0;

static int32_t getMemoryRegions(uint32_t *apNumRegs, OAL_MemoryReg_t *apMemRegs,
                                size_t aMemRegsElem)
{
	int32_t lRet;
	uint32_t lChunks;
	uint32_t lIdx;
	OAL_FuncArgs_t *lpArgs = NULL;
	static OAL_FuncArgs_t lsGetChunkArgs[OAL_ARRAY_SIZE(gsMemRegions)];

	lChunks = OAL_GetNumberOfReserverdChunks();
	if (lChunks > aMemRegsElem) {
		OAL_LOG_ERROR(
		    "The device tree contains more reserved memory "
		    "regions than expected. Please adjust "
		    "OAL_MAX_RESERVED_REGIONS constant.\n");
		lRet = -EINVAL;
		goto get_mem_reg;
	}

	*apNumRegs = lChunks;
	lpArgs     = &lsGetChunkArgs[0];

	for (lIdx = 0; lIdx < lChunks; lIdx++) {
		lpArgs[lIdx].mpData = &(apMemRegs[lIdx].mDesc);
		lpArgs[lIdx].mSize  = sizeof(apMemRegs[lIdx].mDesc);
	}

	lRet = (int32_t)OAL_DriverOutCall(gOalH, (uint32_t)CMD_GET_CHUNKS,
	                                  lpArgs, lChunks);
	if (lRet != 0) {
		lRet = -EIO;
	}

get_mem_reg:
	return lRet;
}

int32_t OAL_MapPhysMem(uint64_t aPhysAddr, uint64_t aSize, uint8_t **apVirtAddr,
                       enum OAL_MemoryMapping aVirtType)
{
	int32_t lRet = 0;
	int lProt    = PROT_READ | PROT_WRITE | PROT_EXEC;

	if (aVirtType == OAL_VIRT_NON_CACHEABLE) {
		lProt |= PROT_NOCACHE;
	}

	if ((apVirtAddr != NULL) && (aVirtType != OAL_VIRT_NONE)) {
		*apVirtAddr = mmap_device_memory(
		    NULL, aSize, lProt, MAP_PHYS | MAP_SHARED, aPhysAddr);
		if (*apVirtAddr == (uint8_t *)(uintptr_t)MAP_FAILED) {
			lRet = -EIO;
		}
	}

	return lRet;
}

static int32_t gsOSOalmemAllocError;
static void initializeMutex(void)
{
	int32_t lRet = OAL_InitializeMutex(&gsInitFdMutex);
	if (lRet != 0) {
		gsOSOalmemAllocError = lRet;
		OAL_LOG_ERROR("Failed to initialize mutex\n");
	}
}

static void close_mem_regions(void)
{
	uint32_t lIdx;
	/* close memory zones */
	for (lIdx = 0; lIdx < gsNumMemReg; lIdx++) {
		if (gsMemRegions[lIdx].mFd > 0) {
			(void)close(gsMemRegions[lIdx].mFd);
		}
	}
}

int32_t OAL_InitializeMemoryFd(void)
{
	uint32_t lIdx;
	int32_t lRetVal = OAL_SUCCESS;

	(void)OAL_ExecuteOnce(&gsInitializedFdMutex, initializeMutex);
	if (gsOSOalmemAllocError != 0) {
		OAL_LOG_ERROR("Failed to initialize OAL library\n");
		lRetVal              = OAL_FAILURE;
		gsOSOalmemAllocError = 0;
		goto initialize_exit;
	}
	lRetVal = OAL_LockMutex(&gsInitFdMutex);
	if (lRetVal != 0) {
		OAL_LOG_ERROR("Failed to take lock\n");
		goto initialize_exit;
	}

	/* initialize cache info structure */
	if (cache_init(0, &gsCacheInfo, NULL) == -1) {
		OAL_LOG_ERROR("Failed to initialize cache info\n");
		lRetVal = OAL_FAILURE;
		goto release_lock;
	}

	lRetVal = getMemoryRegions(&gsNumMemReg, &gsMemRegions[0],
	                           OAL_ARRAY_SIZE(gsMemRegions));
	if (lRetVal != 0) {
		OAL_LOG_ERROR("Failed to get memory regions information\n");
		goto release_lock;
	}

	/* Open each memory chunk as posix typed memory */
	for (lIdx = 0; lIdx < gsNumMemReg; lIdx++) {
		gsMemRegions[lIdx].mFd =
		    posix_typed_mem_open(gsMemRegions[lIdx].mDesc.mName, O_RDWR,
		                         POSIX_TYPED_MEM_MAP_ALLOCATABLE);
		if (gsMemRegions[lIdx].mFd < 0) {
			/* Need only to check if it exists */
			if (errno != EPERM) {
				OAL_LOG_ERROR(
				    "Failed to open posix memory: `%s`(%s)\n",
				    gsMemRegions[lIdx].mDesc.mName,
				    strerror(errno));
				lRetVal = OAL_FAILURE;
				break;
			}
		}
	}

	if (lRetVal == OAL_FAILURE) {
		close_mem_regions();
	}

release_lock:
	if (OAL_UnlockMutex(&gsInitFdMutex) != 0) {
		if (lRetVal != 0) {
			lRetVal = OAL_FAILURE;
		}
		OAL_LOG_ERROR("Failed to release lock\n");
	}

initialize_exit:
	return lRetVal;
}

/* Deinitializes OAL */
int32_t OAL_DeinitializeMemoryFd(void)
{
	int32_t lRet = 0;

	(void)OAL_ExecuteOnce(&gsInitializedFdMutex, initializeMutex);
	if (gsOSOalmemAllocError != 0) {
		OAL_LOG_ERROR("Failed to deinitialize OAL library\n");
		lRet                 = OAL_FAILURE;
		gsOSOalmemAllocError = 0;
		goto deinitialize_exit;
	}

	lRet = OAL_LockMutex(&gsInitFdMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to take lock\n");
		goto deinitialize_exit;
	}

	/* close memory zones */
	close_mem_regions();

	gsNumMemReg = 0;

	/* clear cache info */
	(void)cache_fini(&gsCacheInfo);

	if (OAL_UnlockMutex(&gsInitFdMutex) != 0) {
		lRet = OAL_FAILURE;
		OAL_LOG_ERROR("Failed to release lock\n");
	}

deinitialize_exit:
	return lRet;
}

uint32_t __attribute__((weak)) OAL_OS_GetCacheLineSize(void)
{
	int32_t lRet            = 0;
	uint32_t lCacheLineSize = 0U;

	if (gOalH == NULL) {
		lRet = OAL_Initialize();
		if (lRet != 0) {
			OAL_LOG_ERROR(
			    "Failed to initialize OAL "
			    "memory allocator\n");
			goto cache_line_exit;
		}
	}

	lCacheLineSize = gsCacheInfo.cache_line_size;

cache_line_exit:
	return lCacheLineSize;
}

uint32_t __attribute__((weak)) OAL_OS_GetPageSize(void) { return __PAGESIZE; }
int32_t OAL_MapPhysArea(struct PhysicalPageMapping *apPhys,
                        uint8_t **apVirtAddr, enum OAL_MemoryMapping aVirtType)
{
	int32_t lRet = 0;

	if ((apPhys == NULL) || (aVirtType == OAL_VIRT_NONE)) {
		lRet = -EINVAL;
		goto map_area_exit;
	}

	lRet = OAL_MapPhysMem(apPhys->mPhysAddr, apPhys->mSize, apVirtAddr,
	                      aVirtType);

map_area_exit:
	return lRet;
}

int32_t OAL_UnmapMemArea(struct PhysicalPageMapping *apPhys,
                         uint8_t *apVirtAddr)
{
	int32_t lRet = 0;

	if ((apPhys == NULL) || (apVirtAddr == OAL_VIRT_NONE)) {
		lRet = -EINVAL;
		goto unmap_phys_exit;
	}

	lRet = munmap_device_memory(apVirtAddr, (size_t)apPhys->mSize);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to unmap memory: %s\n", strerror(errno));
	}

unmap_phys_exit:
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_FlushMemory(uintptr_t aAddr, uint32_t aSize)
{
	int32_t lRet = 0;
	off64_t lPhysAddr;

	lRet = mem_offset64((void *)aAddr, NOFD, aSize, &lPhysAddr, NULL);
	if (lRet != 0) {
		OAL_LOG_WARNING("OAL: Failed to get physical address\n");
	} else {
		CACHE_FLUSH(&gsCacheInfo, (void *)aAddr, (uint64_t)lPhysAddr,
		            aSize);
	}
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_InvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	int32_t lRet = 0;
	off64_t lPhysAddr;
	if (mem_offset64((void *)aAddr, NOFD, aSize, &lPhysAddr, NULL) != 0) {
		OAL_LOG_WARNING("OAL: Failed to get physical address\n");
	} else {
		CACHE_INVAL(&gsCacheInfo, (void *)aAddr, (uint64_t)lPhysAddr,
		            aSize);
	}
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_FlushAndInvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	int32_t lRet = 0;
	off64_t lPhysAddr;
	if (mem_offset64((void *)aAddr, NOFD, aSize, &lPhysAddr, NULL) != 0) {
		OAL_LOG_WARNING("OAL: Failed to get physical address\n");
	} else {
		CACHE_FLUSH(&gsCacheInfo, (void *)aAddr, (uint64_t)lPhysAddr,
		            aSize);
		CACHE_INVAL(&gsCacheInfo, (void *)aAddr, (uint64_t)lPhysAddr,
		            aSize);
	}
	return lRet;
}
