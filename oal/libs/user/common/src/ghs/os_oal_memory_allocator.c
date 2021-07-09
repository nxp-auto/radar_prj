/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <INTEGRITY.h>

#include <oal_driver_functions.h>
#include <oal_mem_constants.h>
#include <oal_memory_allocator.h>
#include <oal_virtmem_manager.h>
#include <os_oal_memory_allocator.h>
#include <asp_export.h>
#include <ghs_mr_utils.h>
#include <ghs_user_comm.h>
#include <ghs_user_comm_conn.h>
#include <posix_oal_memory_allocator.h>

#define OAL_PAGE_SIZE (ASP_PAGESIZE)

struct OSPhysicalPageMapping {
	MemoryRegion mPhysMR;

	uint8_t mUsed;
};

struct OSVirtMemAllocation {
	MemoryRegion mVirtMR;
	uint32_t mReferences;

	uint8_t mUsed;
};

/* Global data */
OAL_DriverHandle_t gOalH;

static struct OSPhysicalPageMapping gsPhysMemRegs[OAL_VAS_MAX_ALLOCATION];

ObjectIndex *__ghs_named_object_table_OALMemoryClientConn = (ObjectIndex *)-1;
Address __ghs_named_object_table_OALMemoryClientConn_size = (Address)-1;

static Error getUnusedPhysMemReg(struct OSPhysicalPageMapping **apMemReg)
{
	size_t lIdx;
	Error lError = Failure;

	for (lIdx = 0; lIdx < OAL_ARRAY_SIZE(gsPhysMemRegs); lIdx++) {
		if (gsPhysMemRegs[lIdx].mUsed == 0U) {
			lError                    = Success;
			gsPhysMemRegs[lIdx].mUsed = 1U;
			*apMemReg                 = &gsPhysMemRegs[lIdx];
			break;
		}
	}

	return lError;
}

static void setUnusedPhysMemReg(struct OSPhysicalPageMapping *apMemReg)
{
	/* Allocation cleanup */
	(void)memset(apMemReg, 0, sizeof(*apMemReg));
}

static Error getUnusedVirtMemReg(struct OSVirtMemAllocation **apMemReg)
{
	size_t lIdx;
	Error lError = Failure;
	static struct OSVirtMemAllocation
	    lsVirtualMemRegs[OAL_VAS_MAX_ALLOCATION * 2];

	for (lIdx = 0; lIdx < OAL_ARRAY_SIZE(gsPhysMemRegs); lIdx++) {
		if (lsVirtualMemRegs[lIdx].mUsed == 0U) {
			lError                       = Success;
			lsVirtualMemRegs[lIdx].mUsed = 1U;
			*apMemReg                    = &lsVirtualMemRegs[lIdx];
			break;
		}
	}

	return lError;
}

static void setUnusedVirtMemReg(struct OSVirtMemAllocation *apMemReg)
{
	/* Allocation cleanup */
	(void)memset(apMemReg, 0, sizeof(*apMemReg));
}

static int32_t initializeDriverConnection(void)
{
	int32_t lRet     = -1;
	Connection lConn = NULLConnection;

	if (__ghs_named_object_table_OALMemoryClientConn_size == 1U) {
		lConn = ConnectionObjectNumber(
		    __ghs_named_object_table_OALMemoryClientConn[0]);
		if (lConn == NULLConnection) {
			OAL_LOG_ERROR(
			    "Failed to initialize OAL memory "
			    "client connection.\nPlease check .int "
			    "file\n");
		} else {
			gOalH = OAL_OpenConnectionDriver(lConn);
			if (gOalH == NULL) {
				OAL_LOG_ERROR(
				    "Failed to create a communication channel "
				    "using connection from .int file\n");
			} else {
				lRet = 0;
			}
		}
	} else {
		OAL_LOG_ERROR("Failed to get client connection\n");
	}

	return lRet;
}

int32_t OAL_OS_Initialize(void)
{
	int32_t lError                 = OAL_SUCCESS;
	static uint8_t lsIsInitialized = 0U;
	if (lsIsInitialized == 0U) {
		(void)OAL_InitializeMemoryPools();
		lError = initializeDriverConnection();
		if (lError == 0) {
			lsIsInitialized = 1U;
		}
	}
	return lError;
}

int32_t OAL_OS_Deinitialize(void)
{
	OAL_NOT_IMPLEMENTED();
	return 0;
}

static int32_t getPhysMemFromServer(uint64_t *apSize, uint32_t *apAlign,
                                    OAL_MEMORY_FLAG aFlags, uint32_t aChunkId,
                                    struct PhysicalPageMapping *apPhysPages)
{
	CMD_ALLOC_TYPE lDrvCmd;
	int32_t lRet = 0;
	uint32_t lURet;

	*apSize          = OAL_ROUNDUP_PAGE((*apSize));
	lDrvCmd.mSize    = *apSize;
	lDrvCmd.mAlign   = *apAlign;
	lDrvCmd.mChunkId = aChunkId;
	lDrvCmd.mFlags   = aFlags;

	lURet = OAL_SimpleInOutCall(gOalH, (uint32_t)CMD_ALLOC, lDrvCmd);
	if (lURet != 0U) {
		OAL_LOG_ERROR("Failed to send allocation request\n");
		lRet = (int32_t)lURet;
		goto get_phys_mem_exit;
	}

	apPhysPages->mPhysAddr = lDrvCmd.mRetPhysPointer;
	apPhysPages->mpOSData->mPhysMR =
	    (MemoryRegion)OAL_GetServerObject(gOalH);

get_phys_mem_exit:
	return lRet;
}

static int32_t releasePhysicalMemory(MemoryRegion aPhysMR)
{
	int32_t lRet = 0;
	uint32_t lStatus;

	OAL_SetClientObject(gOalH, (Address)aPhysMR);
	lStatus = OAL_DriverNoArgCall(gOalH, (uint32_t)CMD_FREE);
	if (lStatus != 0U) {
		lRet = (int32_t)lStatus;
		OAL_LOG_ERROR("Failed to send free request\n");
	}

	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_AllocPhysicalMemory(struct PhysicalPageMapping *apMapping,
                           uintptr_t *apSize, uint32_t *apAlign,
                           OAL_MEMORY_FLAG aFlags, uint32_t aChunkId)
{
	int32_t lRet = -1;
	Error lError = Success;
	lError       = getUnusedPhysMemReg(&apMapping->mpOSData);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to allocate metadata for allocation\n");
		goto alloc_phys_mem_exit;
	}

	lRet =
	    getPhysMemFromServer(apSize, apAlign, aFlags, aChunkId, apMapping);
	if (lRet == 0) {
		apMapping->mSize = *apSize;
	} else {
		if (apMapping->mpOSData != NULL) {
			setUnusedPhysMemReg(apMapping->mpOSData);
			apMapping->mpOSData = NULL;
		}
	}

alloc_phys_mem_exit:
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_ReleasePhysicalMemory(struct PhysicalPageMapping *apMapping)
{
	int32_t lRet = -1;
	if ((apMapping == NULL) || (apMapping->mpOSData == NULL)) {
		goto release_physical_exit;
	}

	lRet = releasePhysicalMemory(apMapping->mpOSData->mPhysMR);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to release 0x%" PRIx64 "\n",
		              apMapping->mPhysAddr);
	}

	setUnusedPhysMemReg(apMapping->mpOSData);
	apMapping->mPhysAddr = 0;

release_physical_exit:
	return lRet;
}

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
	int32_t lRet = -1;
	Error lError = Success;
	struct OSVirtMemAllocation *lpVirtMemAlloc;

	if (!LIST_EMPTY(&apPhys->mVirtList)) {
		/* Reuse previous allocation */
		struct OAL_MemoryHandle *lpMemAlloc =
		    LIST_FIRST(&apPhys->mVirtList);
		lpVirtMemAlloc = lpMemAlloc->mpOSData;
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

	lError = getUnusedVirtMemReg(&lpVirtMemAlloc);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to allocate metadata for allocation\n");
		goto map_phys_mem_exit;
	}

	apVirt->mpOSData            = lpVirtMemAlloc;
	lpVirtMemAlloc->mReferences = 1;

	if (aMapping == OAL_VIRT_CACHEABLE) {
		lError = OAL_GHS_AllocAndMapMemoryReg(
		    apPhys->mSize, &lpVirtMemAlloc->mVirtMR,
		    &apPhys->mpOSData->mPhysMR, 0);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to map cacheable (%s)\n",
			              ErrorString(lError));
		}
	} else {
		lError = OAL_GHS_AllocAndMapMemoryReg(
		    apPhys->mSize, &lpVirtMemAlloc->mVirtMR,
		    &apPhys->mpOSData->mPhysMR, MEMORY_UNCACHEABLE);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to map uncacheable (%s)\n",
			              ErrorString(lError));
			goto set_unused_virt_mr;
		}
	}

	if (lError == Success) {
		lRet = 0;
	}

set_unused_virt_mr:
	if (lError != Success) {
		setUnusedVirtMemReg(apVirt->mpOSData);
		apVirt->mpOSData = NULL;
	}
map_phys_mem_exit:
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_UnmapPhysicalMemory(struct OAL_MemoryHandle *apVirt)
{
	int32_t lRet = 0;
	Error lError = Success;
	struct OSVirtMemAllocation *lpVirtMemAlloc;

	if ((apVirt == NULL) || (apVirt->mpOSData == NULL)) {
		lRet = -1;
		goto unmap_phys_memory;
	}

	lpVirtMemAlloc = apVirt->mpOSData;
	if (lpVirtMemAlloc->mReferences > 1U) {
		lpVirtMemAlloc->mReferences--;
		lRet = 0;
		goto unmap_phys_memory;
	}

	lError = OAL_GHS_UnmapAndReleaseVirtual(apVirt->mpOSData->mVirtMR);
	if (lError != Success) {
		lRet = -1;
		OAL_LOG_ERROR("Failed to unmap cacheable map\n");
	}

	setUnusedVirtMemReg(apVirt->mpOSData);
	apVirt->mpOSData = NULL;

unmap_phys_memory:
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_GetVirtAddress(struct OAL_MemoryHandle *apVirt, uint8_t **apAddr,
                      enum OAL_MemoryMapping aMapping)
{
	int32_t lRet = -1;
	Address lVirtStart;
	Error lError;
	struct OSVirtMemAllocation *lpVirtMemAlloc;

	OAL_CHECK_NULL_PARAM(apVirt, get_virt_addr_exit);

	if ((apVirt->mMapping != aMapping) &&
	    (apVirt->mMapping != OAL_VIRT_NONE)) {
		OAL_LOG_ERROR("Trying to get a double mapped address\n");
		goto get_virt_addr_exit;
	}

	lpVirtMemAlloc = apVirt->mpOSData;
	lError = GetMemoryRegionAddresses(lpVirtMemAlloc->mVirtMR, &lVirtStart,
	                                  NULL);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get MR boundaries (%s)\n",
		              ErrorString(lError));
	} else {
		*apAddr = (uint8_t *)(lVirtStart + apVirt->mOffset);
		lRet    = 0;
	}

get_virt_addr_exit:
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_FlushMemory(uintptr_t aAddr, uint32_t aSize)
{
	return OAL_OS_FlushAndInvalidateMemory(aAddr, aSize);
}

int32_t __attribute__((weak))
OAL_OS_InvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	return OAL_OS_FlushAndInvalidateMemory(aAddr, aSize);
}

int32_t __attribute__((weak))
OAL_OS_FlushAndInvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	int32_t lRet = 0;
	Error lError;
	/* TODO: It might be an idea to move this functionality into
	   OAL driver using : ASP_InvalidateCaches((Address)pAddr, size); */
	lError = ManageCaches((Address)aAddr, (Length)aSize,
	                      ACCESS_DST_COHERENT | ACCESS_SRC_COHERENT |
	                          ACCESS_SRC_SHARED | ACCESS_DST_SHARED);
	if (lError != Success) {
		OAL_LOG_ERROR("ManageCaches failed with (%s)\n",
		              ErrorString(lError));
		lRet = (int32_t)-lError;
	}
	return lRet;
}

uint32_t __attribute__((weak)) OAL_OS_GetCacheLineSize(void)
{
	static uint32_t lsCacheLineSize = 0x0U;
	if (lsCacheLineSize == 0U) {
		lsCacheLineSize = (uint32_t)CACHE_LINE_SIZE;
	}
	return lsCacheLineSize;
}

uint32_t __attribute__((weak)) OAL_OS_GetPageSize(void)
{
	static uint32_t lsPageSize = 0x0U;
	if (lsPageSize == 0U) {
		lsPageSize = (uint32_t)OAL_PAGE_SIZE;
	}
	return lsPageSize;
}
