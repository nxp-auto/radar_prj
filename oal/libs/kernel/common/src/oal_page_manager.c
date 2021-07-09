/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_page_manager.h>

static struct OALMemoryChunk gsMemPools[OAL_MAX_RESERVED_REGIONS];
static uint32_t gsNumPools;

struct OALMemoryChunk *OAL_GetMemoryPools(void) { return &gsMemPools[0]; }
uint32_t OAL_GetNumberOfPools(void) { return gsNumPools; }
int32_t OAL_GetChunkBaseAddress(uint64_t aChunkId, uint64_t *apBaseAddr)
{
	int32_t lRet = 0;
	struct OALMemoryChunk *lpMemChunk;
	struct OAL_MemoryAllocatorRegion *lpMemReg;

	if (apBaseAddr == NULL) {
		lRet = -EINVAL;
		goto get_chunk_base_addr;
	}

	if (aChunkId < gsNumPools) {
		lpMemChunk  = &gsMemPools[aChunkId];
		lpMemReg    = &lpMemChunk->mMemoryRegion;
		*apBaseAddr = lpMemReg->mPhysAddr;
		/* Reset search hint for tests */
		lpMemChunk->mSearchHint = 0;
	} else {
		lRet = -EINVAL;
	}

get_chunk_base_addr:
	return lRet;
}

int32_t OAL_GetChunkSize(uint64_t aChunkId, uint64_t *apSize)
{
	int32_t lRet = 0;
	struct OALMemoryChunk *lpMemChunk;
	struct OAL_MemoryAllocatorRegion *lpMemReg;

	if (apSize == NULL) {
		lRet = -EINVAL;
		goto get_chunk_size;
	}

	if (aChunkId < gsNumPools) {
		lpMemChunk = &gsMemPools[aChunkId];
		lpMemReg   = &lpMemChunk->mMemoryRegion;
		*apSize    = lpMemReg->mSize;
	} else {
		lRet = -EINVAL;
	}

get_chunk_size:
	return lRet;
}

int32_t OAL_IsChunkAutobalanced(uint64_t aChunkId, uint32_t *apBalanced)
{
	int32_t lRet = 0;
	struct OALMemoryChunk *lpMemChunk;
	struct OAL_MemoryAllocatorRegion *lpMemReg;

	if (apBalanced == NULL) {
		lRet = -EINVAL;
		goto get_chunk_size;
	}

	if (aChunkId < gsNumPools) {
		lpMemChunk  = &gsMemPools[aChunkId];
		lpMemReg    = &lpMemChunk->mMemoryRegion;
		*apBalanced = ((lpMemReg->mAutobalance != 0U) ? (1U) : (0U));
	} else {
		lRet = -EINVAL;
	}

get_chunk_size:
	return lRet;
}

void OAL_ResetMemoryPools(void) { gsNumPools = 0; }
int32_t OAL_InitMemoryPools(uint8_t aForce)
{
	size_t lIdx;
	int32_t lRet = 0;
	static uint8_t lsInitialized;

	if ((aForce == 0U) && (lsInitialized != 0U)) {
		goto init_mem_pools_exit;
	}

	OAL_ResetMemoryPools();
	for (lIdx = 0; lIdx < OAL_ARRAY_SIZE(gsMemPools); lIdx++) {
		OAL_SET_POOL_SIZE(
		    &gsMemPools[lIdx].mPagesPool,
		    OAL_BITSET_POS_PER_CHUNK *
		        OAL_ARRAY_SIZE(gsMemPools[lIdx].mPagesPool.mBitset));
		lRet = OAL_INITIALIZE_POOL(&gsMemPools[lIdx].mPagesPool);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to initialize memory pools\n");
			break;
		}
		gsMemPools[lIdx].mSearchHint = 0;
		gsMemPools[lIdx].mFreeSpace = gsMemPools[lIdx].mPagesPool.mSize;
	}

	if (lRet == 0) {
		lsInitialized = 1U;
	}

init_mem_pools_exit:
	return lRet;
}

int32_t OAL_AddMemoryPool(struct OAL_MemoryAllocatorRegion *apResMemory,
                          struct OALMemoryChunk **apMemChunk)
{
	int32_t lRet = 0;
	struct OAL_MemoryAllocatorRegion *lpChunk;
	uint64_t lNPages;

	if (apResMemory == NULL) {
		lRet = -1;
		goto add_mem_pool_exit;
	}

	/* Get an unused pool */
	if (gsNumPools >= OAL_ARRAY_SIZE(gsMemPools)) {
		OAL_LOG_ERROR(
		    "The memory pool is full, please revisit "
		    "number of reserved elements for "
		    "gMemPoolsPool\n");
		lRet = -1;
		goto add_mem_pool_exit;
	}

	if (apResMemory->mStartID > OAL_MAX_RESERVED_ID) {
		OAL_LOG_ERROR(
		    "OAL ID isn't in range 0-7."
		    "Please review id attribute.\n");
		lRet = -1;
		goto add_mem_pool_exit;
	}

	*apMemChunk = &gsMemPools[gsNumPools];
	lpChunk     = &(*apMemChunk)->mMemoryRegion;
	(void)memcpy(lpChunk->mName, apResMemory->mName,
	             sizeof(lpChunk->mName));
	lpChunk->mPhysAddr    = apResMemory->mPhysAddr;
	lpChunk->mSize        = apResMemory->mSize;
	lpChunk->mStartID     = apResMemory->mStartID;
	lpChunk->mAlign       = apResMemory->mAlign;
	lpChunk->mInit        = apResMemory->mInit;
	lpChunk->mAutobalance = apResMemory->mAutobalance;

	OAL_PRINT("\tManaging the chunk [0x%" PRIxPTR " - 0x%" PRIx64
	          "]\n"
	          "\t(align = 0x%" PRIx32 ", id = %" PRIu32
	          ", "
	          "autobalance = %d, init = %d)\n",
	          lpChunk->mPhysAddr,
	          (uint64_t)lpChunk->mPhysAddr + lpChunk->mSize,
	          lpChunk->mAlign, lpChunk->mStartID, lpChunk->mAutobalance,
	          lpChunk->mInit);

	lNPages = ((uint64_t)lpChunk->mSize) / OAL_PAGE_SIZE;

	/* Mark as used last part of the pool */
	if (lNPages > (*apMemChunk)->mPagesPool.mSize) {
		lRet = -1;
		OAL_LOG_ERROR(
		    "Please revisit the max size of the reserved chunk."
		    "'%s' node needs at least 0x%" PRIx64 " elements.\n",
		    lpChunk->mName, lNPages);
		goto add_mem_pool_exit;
	} else if (lNPages != (*apMemChunk)->mPagesPool.mSize) {
		lRet = OAL_POOL_SET_USED_RANGE(
		    &(*apMemChunk)->mPagesPool, lNPages,
		    (*apMemChunk)->mPagesPool.mSize - lNPages);
		if (lRet != 0) {
			OAL_LOG_ERROR(
			    "Failed to mark the remaining pages as used.\n");
			goto add_mem_pool_exit;
		}
	} else {
		/* Zero pages to mark as used */
		lRet = 0;
	}

	(*apMemChunk)->mFreeSpace = lNPages;
	gsNumPools++;
	OAL_SET_POOL_SIZE(&(*apMemChunk)->mPagesPool, lNPages);

add_mem_pool_exit:
	return lRet;
}

int32_t OAL_GetMemoryPool(uint32_t aChunkID, struct OALMemoryChunk **apMemChunk)
{
	uint32_t lIt;
	int32_t lRet = -1;
	struct OAL_MemoryAllocatorRegion *lpMemChunk;
	struct OALMemoryChunk *lpAutobalanced0 = NULL;
	struct OALMemoryChunk *lpAutobalanced1 = NULL;

	*apMemChunk = NULL;
	for (lIt = 0; lIt < gsNumPools; lIt++) {
		lpMemChunk = &(gsMemPools[lIt].mMemoryRegion);

		if (lpMemChunk->mAutobalance != 0U) {
			if (lpAutobalanced0 == NULL) {
				lpAutobalanced0 = &gsMemPools[lIt];
			} else {
				lpAutobalanced1 = &gsMemPools[lIt];
			}
		}

		if (lpMemChunk->mStartID == aChunkID) {
			*apMemChunk = &gsMemPools[lIt];
			lRet        = 0;
			break;
		}
	}

	if ((aChunkID == OAL_AUTOBALANCE_CHUNK_ID) &&
	    (lpAutobalanced0 != NULL)) {
		uint64_t lMaxSize0 = 0, lMaxSize1 = 0;
		*apMemChunk = lpAutobalanced0;
		lRet        = 0;

		if (lpAutobalanced1 != NULL) {
			lMaxSize0 = lpAutobalanced0->mFreeSpace;
			lMaxSize1 = lpAutobalanced1->mFreeSpace;

			if (lMaxSize0 > lMaxSize1) {
				*apMemChunk = lpAutobalanced0;
			} else {
				*apMemChunk = lpAutobalanced1;
			}
		}
	}

	return lRet;
}

int32_t OAL_SetFreePages(struct OALMemoryChunk *apChunk, uint64_t aStartIndex,
                         uint64_t aPages)
{
	int32_t lRet = 0;
	if ((apChunk == NULL) || (aPages == 0U)) {
		lRet = -1;
		goto set_free_pages_exit;
	}

	if (aPages == 1U) {
		lRet = OAL_POOL_SET_UNUSED(&apChunk->mPagesPool, aStartIndex);
	} else {
		lRet = OAL_POOL_SET_UNUSED_RANGE(&apChunk->mPagesPool,
		                                 aStartIndex, aPages);
	}

	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to set the allocated pages unused\n");
	} else {
		apChunk->mFreeSpace += aPages;
		/* Reset search hint when the pool is empty */
		if (apChunk->mFreeSpace == apChunk->mPagesPool.mSize) {
			apChunk->mSearchHint = 0U;
			apChunk->mPagesPool.mSearchHint = 0U;
		}
	}

set_free_pages_exit:
	return lRet;
}

int32_t OAL_GetFreePagesFromChunk(uint32_t aChunkId, uint64_t aSize,
                                  uint64_t *apStartIndex, uint64_t *apPages,
                                  struct OALMemoryChunk **apMemChunk)
{
	int32_t lRet = 0;
	struct OALMemoryChunk *lpMemChunk;
	uint64_t lNPages;

	if ((apStartIndex == NULL) || (aSize == 0U) || (apMemChunk == NULL) ||
	    (apPages == NULL)) {
		OAL_LOG_ERROR("Invalid arguments\n");
		lRet = -EINVAL;
		goto get_free_pages_exit;
	}

	*apMemChunk = NULL;
	lRet        = OAL_GetMemoryPool(aChunkId, &lpMemChunk);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to identify chunk %" PRIu32 "\n",
		              aChunkId);
		goto get_free_pages_exit;
	}

	if (lpMemChunk == NULL) {
		OAL_LOG_ERROR("Failed to get chunk %" PRIu32 "\n", aChunkId);
		lRet = -EINVAL;
		goto get_free_pages_exit;
	}

	*apPages = aSize / OAL_PAGE_SIZE;
	if ((aSize % OAL_PAGE_SIZE) != 0U) {
		OAL_LOG_WARNING(
		    "Please revisit the size sent by user space API, "
		    "it's not aligned to OAL_PAGE_SIZE\n");
		(*apPages)++;
	}

	*apStartIndex = lpMemChunk->mSearchHint;
	if (*apStartIndex >= lpMemChunk->mPagesPool.mSize) {
		*apStartIndex = 0;
	}
	lNPages = *apPages;

	if (lNPages == 1U) {
		lRet = OAL_POOL_GET_FIRST_UNUSED(&lpMemChunk->mPagesPool,
		                                 apStartIndex);
		if (lRet != 0) {
			OAL_LOG_ERROR(
			    "There are no more free pages for the "
			    "requested chunk\n");
		}

	} else {
		lRet = OAL_POOL_GET_UNUSED_RANGE(&lpMemChunk->mPagesPool,
		                                 apStartIndex, &lNPages);
		if (lRet != 0) {
			if (lRet == OAL_END_OF_POOL) {
				*apStartIndex           = 0;
				lNPages                 = *apPages;
				lpMemChunk->mSearchHint = 0;
				lRet = OAL_POOL_GET_UNUSED_RANGE(
				    &lpMemChunk->mPagesPool, apStartIndex,
				    &lNPages);
				if (lRet == OAL_END_OF_POOL) {
					OAL_LOG_ERROR(
					    "There are no more free pages for "
					    "the "
					    "requested chunk\n");
					goto get_free_pages_exit;
				}
			} else {
				OAL_LOG_ERROR("Failed to allocate 0x%" PRIx64
				              " pages\n",
				              *apPages);
				goto get_free_pages_exit;
			}
		}

		lRet = OAL_POOL_SET_USED_RANGE(&lpMemChunk->mPagesPool,
		                               *apStartIndex, *apPages);
		if (lRet != 0) {
			OAL_LOG_ERROR(
			    "Failed to set the allocated pages used\n");
			goto get_free_pages_exit;
		}
	}

	if (lRet == 0) {
		lpMemChunk->mSearchHint = *apStartIndex + *apPages;
		lpMemChunk->mFreeSpace -= *apPages;
		*apMemChunk = lpMemChunk;
	}

get_free_pages_exit:
	if ((lRet != 0) && (apPages != NULL)) {
		OAL_LOG_DEBUG("Failed to allocate 0x%" PRIx64
		              " pages from "
		              "chunk 0x%" PRIx32 "\n",
		              *apPages, aChunkId);
	}
	return lRet;
}

int32_t OAL_GetMemoryChunkBasedOnAddr(uint64_t aPhysAddr, uint64_t aSize,
                                      struct OALMemoryChunk **apMemChunk)
{
	int32_t lRet = -1;
	uint32_t lIdx;
	uint32_t lNPools               = OAL_GetNumberOfPools();
	struct OALMemoryChunk *lpPools = OAL_GetMemoryPools();
	struct OALMemoryChunk *lpMemChunk;
	struct OAL_MemoryAllocatorRegion *lpMemReg;
	uint64_t lChunkStart, lChunkEnd;

	OAL_CHECK_NULL_PARAM(apMemChunk, get_memory_chunk_based_on_addr_exit);

	*apMemChunk = NULL;
	for (lIdx = 0; lIdx < lNPools; lIdx++) {
		lpMemChunk  = &lpPools[lIdx];
		lpMemReg    = &lpMemChunk->mMemoryRegion;
		lChunkStart = lpMemReg->mPhysAddr;
		lChunkEnd   = lChunkStart + lpMemReg->mSize;

		if ((lChunkStart <= aPhysAddr) &&
		    (lChunkEnd >= (aPhysAddr + aSize))) {
			*apMemChunk = lpMemChunk;
			lRet        = 0;
			break;
		}
	}

get_memory_chunk_based_on_addr_exit:
	return lRet;
}
