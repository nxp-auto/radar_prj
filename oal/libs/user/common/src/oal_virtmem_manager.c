/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_mem_constants.h>
#include <oal_static_pool.h>
#include <oal_virtmem_manager.h>
#include <os_oal_memory_allocator.h>

/* Pools */
static struct MemoryChunk gsMemChunks[OAL_MAX_RESERVED_REGIONS];
static OAL_DECLARE_BITSET(gMemChuksBitset, OAL_MAX_RESERVED_REGIONS);

static struct OAL_MemoryHandle gsVirtMemAlloc[OAL_VAS_MAX_ALLOCATION];
static OAL_DECLARE_STATIC_POOL(gVirtMemAllocPool, OAL_VAS_MAX_ALLOCATION);

static struct PhysicalPageMapping gsPhysMemAlloc[OAL_VAS_MAX_ALLOCATION];
static OAL_DECLARE_STATIC_POOL(gPhysMemPool, OAL_VAS_MAX_ALLOCATION);

int32_t OAL_InitializeMemoryPools(void)
{
	int32_t lRet      = 0;
	size_t lArraySize = OAL_ARRAY_SIZE(gsMemChunks);
	lRet              = OAL_INIT_BITSET(&gMemChuksBitset[0], lArraySize);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize the bitset\n");
		goto init_pools_exit;
	}

	lRet = OAL_INITIALIZE_POOL(&gVirtMemAllocPool);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize virtual memory pool\n");
		goto init_pools_exit;
	}

	lRet = OAL_INITIALIZE_POOL(&gPhysMemPool);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize physical memory pool\n");
		goto init_pools_exit;
	}

init_pools_exit:
	return lRet;
}

uint8_t OAL_IsChunkAvailable(uint8_t aChunkId, struct MemoryChunk **apChunk)
{
	uint8_t lRet = 0U;
	size_t lIdx;

	for (lIdx = 0; lIdx < OAL_ARRAY_SIZE(gsMemChunks); lIdx++) {
		if (OAL_IS_BIT_SET(&gMemChuksBitset, lIdx) &&
		    (gsMemChunks[lIdx].mId == aChunkId)) {
			*apChunk = &gsMemChunks[lIdx];
			lRet     = 1U;
			break;
		}
	}

	return lRet;
}

void OAL_GetNextUsedMemAllocation(struct OAL_MemoryHandle **apAllocation)
{
	struct OAL_MemoryHandle *lpStart;
	uint64_t lIdx;
	size_t lNumVirtAlloc = OAL_ARRAY_SIZE(gsVirtMemAlloc);
	intptr_t lAllocIndex;

	if (apAllocation == NULL) {
		OAL_LOG_ERROR("Invalid arguments\n");
		goto get_next_alloc_exit;
	}

	lpStart = *apAllocation;

	if (lpStart == NULL) {
		lpStart = &gsVirtMemAlloc[0];
	} else {
		lpStart++;
	}

	*apAllocation = NULL;
	lAllocIndex   = lpStart - &gsVirtMemAlloc[0];

	if ((uint64_t)lAllocIndex < lNumVirtAlloc) {
		for (lIdx = (uint64_t)lAllocIndex; lIdx < lNumVirtAlloc;
		     lIdx++) {
			if (OAL_IS_BIT_SET(&gVirtMemAllocPool.mBitset, lIdx)) {
				*apAllocation = &gsVirtMemAlloc[lIdx];
				break;
			}
		}
	}

get_next_alloc_exit:
	return;
}

int32_t OAL_GetUnusedVirtMemAllocation(struct OAL_MemoryHandle **apAllocation)
{
	int32_t lError     = -1;
	uint64_t lAvailPos = 0U;

	if (apAllocation != NULL) {
		lError =
		    OAL_POOL_GET_FIRST_UNUSED(&gVirtMemAllocPool, &lAvailPos);
		if (lError != 0) {
			OAL_LOG_ERROR(
			    "The buffer allocated for virtual memory "
			    "allocations is full. Please increase "
			    "its size. Current size : %zu\n",
			    OAL_ARRAY_SIZE(gsVirtMemAlloc));
		} else {
			*apAllocation = &gsVirtMemAlloc[lAvailPos];
			OAL_SET_BIT(&gVirtMemAllocPool.mBitset, lAvailPos);
			lError = 0;
		}
	}

	return lError;
}

void OAL_SetUnusedVirtMemAllocation(struct OAL_MemoryHandle *apAllocation)
{
	uintptr_t lDiff;

	/* Allocation cleanup */
	if (apAllocation == NULL) {
		goto set_unused_virt_exit;
	}

	if ((apAllocation >= &gsVirtMemAlloc[0]) &&
	    (apAllocation <
	     (&gsVirtMemAlloc[0] + OAL_ARRAY_SIZE(gsVirtMemAlloc)))) {
		(void)memset(apAllocation, 0, sizeof(*apAllocation));

		lDiff = (uintptr_t)apAllocation;
		lDiff -= (uintptr_t)&gsVirtMemAlloc[0];
		lDiff /= sizeof(gsVirtMemAlloc[0]);

		(void)OAL_POOL_SET_UNUSED(&gVirtMemAllocPool, lDiff);
	} else {
		OAL_LOG_ERROR("Out of bounds access %p\n",
		              (void *)apAllocation);
	}
set_unused_virt_exit:
	return;
}

int32_t OAL_GetUnusedPhysMemAllocation(
    struct PhysicalPageMapping **apPhysAllocation)
{
	int32_t lError     = -1;
	uint64_t lAvailPos = 0U;

	if (apPhysAllocation != NULL) {
		lError = OAL_POOL_GET_FIRST_UNUSED(&gPhysMemPool, &lAvailPos);
		if (lError != 0) {
			OAL_LOG_ERROR(
			    "The buffer allocated for physical memory"
			    " allocations is full. Please increase "
			    "its size. Current size : %zu\n",
			    OAL_ARRAY_SIZE(gsPhysMemAlloc));
		} else {
			lError = 0;
			OAL_SET_BIT(&gPhysMemPool.mBitset, lAvailPos);
			gsPhysMemAlloc[lAvailPos].mTag = (uintptr_t)lAvailPos;
			*apPhysAllocation = &gsPhysMemAlloc[lAvailPos];
		}
	}

	return lError;
}

void OAL_SetUnusedPhysMemAllocation(
    struct PhysicalPageMapping *apPhysAllocation)
{
	int32_t lRet = 0;
	uintptr_t lDiff;
	struct PhysicalPageMapping *lpArrayBegin = &gsPhysMemAlloc[0];

	/* Allocation cleanup */
	if (apPhysAllocation == NULL) {
		goto set_unused_phys;
	}

	/* Only if it's part of the array */
	if ((apPhysAllocation >= lpArrayBegin) &&
	    (apPhysAllocation <
	     (lpArrayBegin + OAL_ARRAY_SIZE(gsPhysMemAlloc)))) {
		(void)memset(apPhysAllocation, 0, sizeof(*apPhysAllocation));

		lDiff = (uintptr_t)apPhysAllocation;
		lDiff -= (uintptr_t)lpArrayBegin;
		lDiff /= sizeof(gsPhysMemAlloc[0]);

		lRet = OAL_POOL_SET_UNUSED(&gPhysMemPool, lDiff);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to set unused range\n");
		}
	} else {
		OAL_LOG_ERROR(
		    "Passed allocatioon isn't part of "
		    "gsPhysMemAlloc\n");
	}

set_unused_phys:
	return;
}

int32_t OAL_GetUnusedMemoryChunk(struct MemoryChunk **apMemChunk)
{
	size_t lIdx;
	int32_t lError = -1;

	if (apMemChunk != NULL) {
		for (lIdx = 0; lIdx < OAL_ARRAY_SIZE(gsMemChunks); lIdx++) {
			if (!OAL_IS_BIT_SET(&gMemChuksBitset, lIdx)) {
				OAL_SET_BIT(&gMemChuksBitset, lIdx);
				LIST_INIT(&gsMemChunks[lIdx].mAllocList);
				*apMemChunk = &gsMemChunks[lIdx];
				lError      = 0;
				break;
			}
		}

		if (lError != 0) {
			OAL_LOG_ERROR(
			    "The buffer allocated for memory chunks"
			    " is full. Please increase its size."
			    "Current size : %zu\n",
			    OAL_ARRAY_SIZE(gsMemChunks));
		}
	}

	return lError;
}

void OAL_SetUnusedMemoryChunks(struct MemoryChunk *apMemChunk)
{
	uintptr_t lDiff;

	/* Memory chunk cleanup */
	if (apMemChunk == NULL) {
		goto set_unused_chunk;
	}

	if ((apMemChunk >= &gsMemChunks[0]) &&
	    (apMemChunk < (&gsMemChunks[0] + OAL_ARRAY_SIZE(gsMemChunks)))) {
		(void)memset(apMemChunk, 0, sizeof(*apMemChunk));

		lDiff = (uintptr_t)apMemChunk;
		lDiff -= (uintptr_t)&gsMemChunks[0];
		lDiff /= sizeof(gsMemChunks[0]);

		(void)OAL_CLEAR_BIT(&gMemChuksBitset, lDiff);
	} else {
		OAL_LOG_ERROR("Out of bounds access %p\n", (void *)apMemChunk);
	}

set_unused_chunk:
	return;
}

void OAL_DumpPhysicalPageMapping(struct PhysicalPageMapping *apPhys)
{
	struct OAL_MemoryHandle *lpVirt;
	(void)OAL_PRINT("Phys : [0x%" PRIxPTR " - 0x%" PRIxPTR "] -> ",
	                apPhys->mPhysAddr,
	                apPhys->mPhysAddr + (uintptr_t)apPhys->mSize);

	LIST_FOREACH (lpVirt, &apPhys->mVirtList, mVirtEntry) {
		(void)OAL_PRINT("(0x%" PRIxPTR " - 0x%" PRIxPTR ") ",
		                lpVirt->mOffset,
		                lpVirt->mOffset + lpVirt->mSize);
	}
	(void)OAL_PRINT("%s", "\n");
}

void OAL_DumpMemChunk(struct MemoryChunk *apMemChunk)
{
	struct PhysicalPageMapping *lpPhys;

	(void)OAL_PRINT("CHUNK : %" PRIu32 "\n", apMemChunk->mId);
	LIST_FOREACH (lpPhys, &apMemChunk->mAllocList, mChunkEntry) {
		(void)OAL_PRINT("%s", "\t");
		OAL_DumpPhysicalPageMapping(lpPhys);
	}
}

static uint8_t isPartOfVirtPage(void *apVirtAddr,
                                struct PhysicalPageMapping *apPhysMem,
                                uintptr_t *apOffset,
                                enum OAL_MemoryMapping aMapping)
{
	uint8_t lRet = 0U;
	int32_t lStatus;
	struct OAL_MemoryHandle *lpVirt;
	uint8_t *lpAddr = NULL;
	intptr_t lDiff;

	if (apOffset == NULL) {
		goto is_part_of_virt_page_exit;
	}

	LIST_FOREACH (lpVirt, &apPhysMem->mVirtList, mVirtEntry) {
		if ((lpVirt->mMapping != aMapping) &&
		    (lpVirt->mMapping != OAL_VIRT_NONE)) {
			continue;
		}

		lStatus = OAL_OS_GetVirtAddress(lpVirt, &lpAddr, aMapping);
		if ((lStatus != 0) || (lpAddr == NULL)) {
			continue;
		}

		lpAddr -= lpVirt->mOffset;
		if (((uint8_t *)apVirtAddr >= lpAddr) &&
		    ((uint8_t *)apVirtAddr < (lpAddr + apPhysMem->mSize))) {
			lDiff     = ((uint8_t *)apVirtAddr) - lpAddr;
			*apOffset = (uintptr_t)lDiff;
			lRet      = 1U;
			break;
		}
	}

is_part_of_virt_page_exit:
	return lRet;
}

int32_t OAL_GetPhysicalPageMapping(struct MemoryChunk *apMemChunk,
                                   void *apVirtAddr,
                                   enum OAL_MemoryMapping aMapping,
                                   struct PhysicalPageMapping **apPhys,
                                   uintptr_t *apOffset)
{
	int32_t lRet = -1;
	struct PhysicalPageMapping *lpPhys;

	LIST_FOREACH (lpPhys, &apMemChunk->mAllocList, mChunkEntry) {
		if (isPartOfVirtPage(apVirtAddr, lpPhys, apOffset, aMapping) ==
		    1U) {
			lRet    = 0;
			*apPhys = lpPhys;
			break;
		}
	}

	return lRet;
}
