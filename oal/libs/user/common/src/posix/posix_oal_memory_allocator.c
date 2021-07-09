/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_comm.h>
#include <oal_driver_functions.h>
#include <oal_mem_constants.h>
#include <oal_memory_allocator.h>
#include <oal_once.h>
#include <oal_static_pool.h>
#include <oal_virtmem_manager.h>
#include <os_oal_memory_allocator.h>
#include <extra_oal_allocator.h>
#include <posix_oal_memory_allocator.h>

/* Files to be used for cacheable non-cacheable memory mapping */
OAL_DriverHandle_t gOalH = NULL;

struct OSVirtMemAllocation {
	uint8_t *mpVirtPage;
	uint32_t mReferences;
};

static int32_t initializeDriverComm(void)
{
	int32_t lRet = OAL_SUCCESS;

	gOalH = OAL_OpenDriver(OAL_SERVICE_NAME);
	if (gOalH == NULL) {
		lRet = OAL_FAILURE;
	}

	return lRet;
}

static int32_t deinitializeDriverComm(void)
{
	int32_t lRet = 0;

	lRet = OAL_CloseDriver(&gOalH);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to close driver\n");
	} else {
		gOalH = NULL;
	}

	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_AllocPhysicalMemory(struct PhysicalPageMapping *apMapping,
                           uintptr_t *apSize, uint32_t *apAlign,
                           OAL_MEMORY_FLAG aFlags, uint32_t aChunkId)
{
	int32_t lRet = -1;
	uint32_t lStatus;
	CMD_ALLOC_TYPE lDrvCmd;

	lDrvCmd.mSize    = *apSize;
	lDrvCmd.mAlign   = *apAlign;
	lDrvCmd.mChunkId = aChunkId;
	lDrvCmd.mFlags   = aFlags;

	lStatus = OAL_SimpleInOutCall(gOalH, (uint32_t)CMD_ALLOC, lDrvCmd);
	if ((lStatus != 0U) || (lDrvCmd.mRetPhysPointer == 0U)) {
		OAL_LOG_WARNING("Failed to allocate 0x%" PRIx64
		                " bytes from "
		                "chunk %d\n",
		                *apSize, aChunkId);
		if (lDrvCmd.mRetPhysPointer == 0ULL) {
			lRet = -ENOMEM;
		}
	} else {
		*apSize              = lDrvCmd.mSize;
		*apAlign             = lDrvCmd.mAlign;
		apMapping->mPhysAddr = lDrvCmd.mRetPhysPointer;
		apMapping->mSize     = lDrvCmd.mSize;
		lRet                 = 0;
	}

	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_ReleasePhysicalMemory(struct PhysicalPageMapping *apMapping)
{
	int32_t lRet = -1;
	CMD_FREE_TYPE lDrvCmd;

	if (apMapping == NULL) {
		goto release_physical_exit;
	}

	lDrvCmd.mSize           = apMapping->mSize;
	lDrvCmd.mRetPhysPointer = apMapping->mPhysAddr;

	if (OAL_SimpleInCall(gOalH, (uint32_t)CMD_FREE, lDrvCmd) != 0U) {
		lRet = -EIO;
		OAL_LOG_ERROR("Failed to release 0x%" PRIx64 "\n",
		              apMapping->mPhysAddr);
	} else {
		lRet = 0;
	}

	apMapping->mPhysAddr = 0U;

release_physical_exit:
	return lRet;
}

static struct OSVirtMemAllocation gsVirtualMemRegs[OAL_VAS_MAX_ALLOCATION * 2U];
static OAL_DECLARE_STATIC_POOL(gVirtMemPool, (OAL_VAS_MAX_ALLOCATION * 2U));

int32_t __attribute__((weak))
OAL_OS_CopyMappingInfo(struct OAL_MemoryHandle *apDstVirt,
                       struct OAL_MemoryHandle *apSrcVirt)
{
	int32_t lRet = -1;
	struct OSVirtMemAllocation *lpVirtMemAlloc;

	if ((apDstVirt == NULL) || (apSrcVirt == NULL)) {
		goto copy_info_exit;
	}

	if (apSrcVirt->mpOSData == NULL) {
		goto copy_info_exit;
	}

	lpVirtMemAlloc = apSrcVirt->mpOSData;
	lpVirtMemAlloc->mReferences++;
	apDstVirt->mpOSData = lpVirtMemAlloc;

	lRet = 0;

copy_info_exit:
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_MapPhysicalMemory(struct PhysicalPageMapping *apPhys,
                         struct OAL_MemoryHandle *apVirt,
                         enum OAL_MemoryMapping aMapping)
{
	int32_t lRet      = -1;
	uint64_t lFreePos = 0U;
	struct OSVirtMemAllocation *lpVirtMemAlloc;

	if (!LIST_EMPTY(&apPhys->mVirtList)) {
		/* Reuse previous allocation */
		lpVirtMemAlloc = LIST_FIRST(&apPhys->mVirtList)->mpOSData;
		lpVirtMemAlloc->mReferences++;
		apVirt->mpOSData = lpVirtMemAlloc;

		if (aMapping != apVirt->mMapping) {
			OAL_LOG_ERROR(
			    "Associating a page with different "
			    "cache flags\n");
		}

		lRet = 0;
		goto map_phys_mem_exit;
	}

	lRet = OAL_POOL_GET_FIRST_UNUSED(&gVirtMemPool, &lFreePos);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to allocate virtual memory metadata\n");
		goto map_phys_mem_exit;
	}

	lpVirtMemAlloc              = &gsVirtualMemRegs[lFreePos];
	apVirt->mpOSData            = lpVirtMemAlloc;
	lpVirtMemAlloc->mReferences = 1U;

	if (aMapping == OAL_VIRT_CACHEABLE) {
		/* Map cacheable and non-cacheable memory */
		lRet = OAL_MapPhysArea(apPhys, &lpVirtMemAlloc->mpVirtPage,
		                       OAL_VIRT_CACHEABLE);
		if (lRet != 0) {
			lpVirtMemAlloc->mpVirtPage = NULL;
			OAL_LOG_ERROR("Failed to map cacheable memory\n");
		}
	} else {
		lRet = OAL_MapPhysArea(apPhys, &lpVirtMemAlloc->mpVirtPage,
		                       OAL_VIRT_NON_CACHEABLE);
		if (lRet != 0) {
			lpVirtMemAlloc->mpVirtPage = NULL;
			OAL_LOG_ERROR("Failed to map non-cacheable memory\n");
		}
	}

	if (lRet != 0) {
		(void)OAL_POOL_SET_UNUSED(&gVirtMemPool, lFreePos);
	}

map_phys_mem_exit:
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_UnmapPhysicalMemory(struct OAL_MemoryHandle *apVirt)
{
	int32_t lRet = 0;
	uint64_t lFreePos;
	struct OSVirtMemAllocation *lpVirtMemAlloc;
	uintptr_t lVirtMemPoolEnd, lVirtMemPoolBegin;

	lpVirtMemAlloc = apVirt->mpOSData;
	if (lpVirtMemAlloc->mReferences == 0U) {
		lRet = 0;
		goto unmap_phys_memory;
	}

	if (lpVirtMemAlloc->mReferences > 1U) {
		lpVirtMemAlloc->mReferences--;
		lRet = 0;
		goto unmap_phys_memory;
	}

	lVirtMemPoolBegin = (uintptr_t)&gsVirtualMemRegs[0];
	lVirtMemPoolEnd   = (uintptr_t)&gsVirtualMemRegs[0];
	lVirtMemPoolEnd += sizeof(gsVirtualMemRegs);

	if (!((((uintptr_t)apVirt->mpOSData) >= lVirtMemPoolBegin) &&
	      (((uintptr_t)apVirt->mpOSData) < lVirtMemPoolEnd))) {
		lRet = -1;
		OAL_LOG_ERROR("Passed hanlde is out of virtual pool\n");
		goto unmap_phys_memory;
	}

	lpVirtMemAlloc->mReferences--;

	lFreePos = (uintptr_t)apVirt->mpOSData;
	lFreePos -= (uintptr_t)&gsVirtualMemRegs[0];
	lFreePos /= sizeof(gsVirtualMemRegs[0]);

	lRet =
	    OAL_UnmapMemArea(apVirt->mpPhysAlloc, lpVirtMemAlloc->mpVirtPage);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to unmap cacheable memory\n");
		goto unmap_phys_memory;
	}

	lRet = OAL_POOL_SET_UNUSED(&gVirtMemPool, lFreePos);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to mark %" PRIu64 " position as unused\n",
		              lFreePos);
		goto unmap_phys_memory;
	}

unmap_phys_memory:
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_GetVirtAddress(struct OAL_MemoryHandle *apVirt, uint8_t **apAddr,
                      enum OAL_MemoryMapping aMapping)
{
	int32_t lRet = -1;
	struct OSVirtMemAllocation *lpVirtMemAlloc;

	OAL_CHECK_NULL_PARAM(apAddr, get_virt_exit);
	OAL_CHECK_NULL_PARAM(apVirt, get_virt_exit);
	OAL_CHECK_NULL_PARAM(apVirt->mpPhysAlloc, get_virt_exit);

	lpVirtMemAlloc = apVirt->mpOSData;

	OAL_CHECK_NULL_PARAM(lpVirtMemAlloc, get_virt_exit);

	if ((apVirt->mMapping != aMapping) &&
	    (apVirt->mMapping != OAL_VIRT_NONE)) {
		OAL_LOG_ERROR("Trying to get a double mapped address\n");
		goto get_virt_exit;
	}

	*apAddr = lpVirtMemAlloc->mpVirtPage + apVirt->mOffset;
	lRet    = 0;

get_virt_exit:
	return lRet;
}

static void virt_mem_constructor(void)
{
	(void)OAL_INITIALIZE_POOL(&gVirtMemPool);
	(void)OAL_InitializeMemoryPools();

	return;
}

static void __attribute__((constructor)) init_virt_mem(void)
{
	static struct OAL_OnceControl lsVirtmemInit = OAL_ONCE_INIT;
	(void)OAL_ExecuteOnce(&lsVirtmemInit, virt_mem_constructor);
}

int32_t __attribute__((weak)) OAL_OS_Initialize(void)
{
	int32_t lRet = OAL_SUCCESS;

	init_virt_mem();
	if (gOalH == NULL) {
		lRet = initializeDriverComm();
		if (lRet != 0) {
			lRet = OAL_FAILURE;
		} else {
			lRet = OAL_InitializeMemoryFd();
			if (lRet != 0) {
				lRet = OAL_FAILURE;
			}
		}
	}
	if (lRet != 0) {
		(void)OAL_OS_Deinitialize();
	}
	return lRet;
}

int32_t __attribute__((weak)) OAL_OS_Deinitialize(void)
{
	int32_t lRet = 0, lStatus;
	if (gOalH == NULL) {
		goto deinitialize_exit;
	}

	lStatus = OAL_DeinitializeMemoryFd();
	if (lStatus != 0) {
		lRet = lStatus;
	}

	lStatus = deinitializeDriverComm();
	if (lStatus != 0) {
		lRet = lStatus;
	}

deinitialize_exit:
	if (lRet != 0) {
		lRet = OAL_FAILURE;
	} else {
		lRet = OAL_SUCCESS;
	}
	return lRet;
}
