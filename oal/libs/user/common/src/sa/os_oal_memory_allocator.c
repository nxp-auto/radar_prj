/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_common.h>
#include <oal_devtree_utils.h>
#include <oal_memory_allocator.h>
#include <oal_page_manager.h>
#include <oal_virtmem_manager.h>
#include <os_module_probe.h>
#include <os_oal_memory_allocator.h>
#include <arch_oal_cache.h>
#include <extra_oal_allocator.h>

uint32_t __attribute__((weak)) OAL_OS_GetCacheLineSize(void)
{
	return OAL_ARCH_GetCacheLine();
}

uint32_t __attribute__((weak)) OAL_OS_GetPageSize(void)
{
	return OAL_PAGE_SIZE;
}
int32_t __attribute__((weak))
OAL_OS_FlushMemory(uintptr_t aAddr, uint32_t aSize)
{
	OAL_UNUSED_ARG(aAddr);
	OAL_UNUSED_ARG(aSize);
	/* Nothing to do, the caches are disabled */
	return 0;
}

int32_t __attribute__((weak))
OAL_OS_InvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	OAL_UNUSED_ARG(aAddr);
	OAL_UNUSED_ARG(aSize);
	/* Nothing to do, the caches are disabled */
	return 0;
}

int32_t __attribute__((weak))
OAL_OS_FlushAndInvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	OAL_UNUSED_ARG(aAddr);
	OAL_UNUSED_ARG(aSize);
	/* Nothing to do, the caches are disabled */
	return 0;
}

int32_t __attribute__((weak))
OAL_OS_AllocPhysicalMemory(struct PhysicalPageMapping *apMapping,
                           uintptr_t *apSize, uint32_t *apAlign,
                           OAL_MEMORY_FLAG aFlags, uint32_t aChunkId)
{
	int32_t lRet;
	uint64_t lNPages;
	uint64_t lPagesIndex;
	struct OALMemoryChunk *lpMemChunk;

	OAL_UNUSED_ARG(aFlags);
	*apSize = OAL_ROUNDUP_PAGE((*apSize));

	lRet = OAL_GetFreePagesFromChunk(aChunkId, *apSize, &lPagesIndex,
	                                 &lNPages, &lpMemChunk);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to allocate 0x%" PRIxPTR
		              " bytes from chunk %" PRIu32 "\n",
		              *apSize, aChunkId);
		lRet = -ENOMEM;
	} else {
		apMapping->mPhysAddr =
		    (((uintptr_t)lPagesIndex) * OAL_PAGE_SIZE) +
		    lpMemChunk->mOSChunk.mStartPhysAddress;
		*apAlign         = lpMemChunk->mMemoryRegion.mAlign;
		*apSize          = ((uintptr_t)lNPages) * OAL_PAGE_SIZE;
		apMapping->mSize = *apSize;
	}

	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_ReleasePhysicalMemory(struct PhysicalPageMapping *apMapping)
{
	int32_t lRet = -1;
	struct OALMemoryChunk *lpMemChunk;
	uint64_t lPagesIndex;

	if (apMapping == NULL) {
		goto release_exit;
	}

	lRet = OAL_GetMemoryChunkBasedOnAddr(apMapping->mPhysAddr,
	                                     apMapping->mSize, &lpMemChunk);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "Failed to identify memory "
		    "chunk for : 0x%" PRIxPTR "\n",
		    apMapping->mPhysAddr);
		goto release_exit;
	}

	lPagesIndex = ((uint64_t)(apMapping->mPhysAddr) -
	               (uint64_t)(lpMemChunk->mMemoryRegion.mPhysAddr)) /
	              ((uint64_t)OAL_PAGE_SIZE);
	lRet = OAL_SetFreePages(lpMemChunk, lPagesIndex,
	                        ((uint64_t)apMapping->mSize) / OAL_PAGE_SIZE);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "Failed to mark unused: [0x%" PRIxPTR " - 0x%" PRIxPTR
		    "]\n",
		    apMapping->mPhysAddr,
		    apMapping->mPhysAddr + (uintptr_t)apMapping->mSize);
		goto release_exit;
	}

	apMapping->mPhysAddr = 0;

release_exit:
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_CopyMappingInfo(struct OAL_MemoryHandle *apDstVirt,
                       struct OAL_MemoryHandle *apSrcVirt)
{
	/* Nothing to copy */
	OAL_UNUSED_ARG(apDstVirt);
	OAL_UNUSED_ARG(apSrcVirt);
	return 0;
}

int32_t __attribute__((weak))
OAL_OS_MapPhysicalMemory(struct PhysicalPageMapping *apPhys,
                         struct OAL_MemoryHandle *apVirt,
                         enum OAL_MemoryMapping aMapping)
{
	OAL_UNUSED_ARG(apPhys);
	OAL_UNUSED_ARG(apVirt);
	OAL_UNUSED_ARG(aMapping);
	/* Nothing to do */
	return 0;
}

int32_t __attribute__((weak))
OAL_OS_UnmapPhysicalMemory(struct OAL_MemoryHandle *apVirt)
{
	int32_t lRet = 0;
	OAL_UNUSED_ARG(apVirt);
	/* Nothing to do */
	return lRet;
}

int32_t __attribute__((weak))
OAL_OS_GetVirtAddress(struct OAL_MemoryHandle *apVirt, uint8_t **apAddr,
                      enum OAL_MemoryMapping aMapping)
{
	int32_t lRet = -1;
	OAL_UNUSED_ARG(aMapping);

	if ((apVirt != NULL) && (apAddr != NULL) &&
	    (apVirt->mpPhysAlloc != NULL)) {
		*apAddr = (uint8_t *)(apVirt->mpPhysAlloc->mPhysAddr +
		                      apVirt->mOffset);
		lRet    = 0;
	}

	return lRet;
}

int32_t __attribute__((weak)) OAL_OS_Initialize(void)
{
	int32_t lRet = OAL_SUCCESS;
	lRet         = OAL_OS_InitKernelAllocator();
	return lRet;
}

int32_t __attribute__((weak)) OAL_OS_Deinitialize(void) { return 0; }

/* Private API */
uint32_t OAL_GetNumberOfReserverdChunks(void)
{
	(void)OAL_Initialize();
	return OAL_GetNumberOfPools();
}

uint64_t OAL_GetResMemBase(uint32_t aMemId)
{
	int32_t lRet;
	uint64_t lBaseAddress = 0ULL;

	(void)OAL_Initialize();

	lRet = OAL_GetChunkBaseAddress(aMemId, &lBaseAddress);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get chunk's (%" PRIu32
		              ") base address\n",
		              aMemId);
	}

	return lBaseAddress;
}

uint64_t OAL_GetResMemSize(uint32_t aMemId)
{
	int32_t lRet;
	uint64_t lSize = 0ULL;

	(void)OAL_Initialize();

	lRet = OAL_GetChunkSize(aMemId, &lSize);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get chunk's (%" PRIu32 ") size\n",
		              aMemId);
	}

	return lSize;
}

uint32_t OAL_IsResMemAutobalanced(uint32_t aMemId)
{
	uint32_t lIsBalanced = 0U;
	int32_t lRet;

	(void)OAL_Initialize();

	lRet = OAL_IsChunkAutobalanced(aMemId, &lIsBalanced);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get chunk's (%" PRIu32
		              ") balance flag\n",
		              aMemId);
	}

	return lIsBalanced;
}
