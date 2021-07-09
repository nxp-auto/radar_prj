/*
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_free_mem_tree.h>
#include <oal_memory_allocator.h>
#include <oal_once.h>
#include <oal_virtmem_manager.h>
#include <os_oal_memory_allocator.h>
#include <extra_oal_allocator.h>

#include <oal_mutex.h>

static struct OAL_Mutex gsOalMemAllocMutex;

/////////////////////////////////////////////////////////////////////////
/// Get alignment from flags
/////////////////////////////////////////////////////////////////////////
static int32_t getAlignment(OAL_MEMORY_FLAG aFlags, uint32_t *apAlignment)
{
	int32_t lRet = 0;
	uint64_t lSize;

	if ((aFlags & OAL_MEMORY_FLAG_ALIGN(OAL_ALIGN2_CACHELINE)) ==
	    OAL_MEMORY_FLAG_ALIGN(OAL_ALIGN2_CACHELINE)) {
		lSize = OAL_OS_GetCacheLineSize();
		if (lSize >= OAL_MAX_VAR_VALUE(*apAlignment)) {
			OAL_LOG_ERROR(
			    "Cache line size "
			    "exceeds alignment size\n");
			lRet = -1;
		} else {
			*apAlignment = (uint32_t)lSize;
		}

	} else if ((aFlags & OAL_MEMORY_FLAG_ALIGN(OAL_ALIGN2_PAGE)) ==
	           OAL_MEMORY_FLAG_ALIGN(OAL_ALIGN2_PAGE)) {
		lSize = OAL_OS_GetPageSize();
		if (lSize >= OAL_MAX_VAR_VALUE(*apAlignment)) {
			OAL_LOG_ERROR("Page size exceeds *apAlignment size\n");
			lRet = -1;
		} else {
			*apAlignment = (uint32_t)lSize;
		}
	} else if ((aFlags & OAL_MEMORY_FLAG_ALIGN(OAL_BYTE_N)) ==
	           OAL_MEMORY_FLAG_ALIGN(OAL_BYTE_N)) {
		uint32_t lAlignment = (uint32_t)((aFlags & 0x00FF0000U) >> 16U);
		*apAlignment        = (uint32_t)(1UL << lAlignment);
	} else {
		*apAlignment = (uint32_t)sizeof(struct TreeNode);
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////
/// Get chunk from flags
/////////////////////////////////////////////////////////////////////////
static inline uint8_t getChunk(OAL_MEMORY_FLAG aFlags)
{
	uint8_t lChunkId = (uint8_t)((aFlags & 0xFF000000U) >> 24U);

	if (lChunkId == 0U) {
		lChunkId = 0xFFU;
	} else {
		lChunkId = lChunkId - 1U;
	}
	return lChunkId;
}

static int32_t allocPhysMem(uint32_t *apAlign, struct MemoryChunk *apMemChunk,
                            struct PhysicalPageMapping **apPhysMemRef,
                            struct OAL_MemoryHandle *apVirtMem,
                            OAL_MEMORY_FLAG aFlags)
{
	uintptr_t lPhysSize                   = apVirtMem->mSize;
	int32_t lRet                          = 0;
	struct PhysicalPageMapping *lpPhysMem = NULL;

	/* Metadata for physical allocation */
	lRet = OAL_GetUnusedPhysMemAllocation(apPhysMemRef);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "There are no more free positions in physical "
		    "memory pool\n");
		goto alloc_phys_mem_exit;
	}

	lpPhysMem = *apPhysMemRef;

	/* Request physical memory from OS */
	lRet = OAL_OS_AllocPhysicalMemory(lpPhysMem, &lPhysSize, apAlign,
	                                  aFlags, apMemChunk->mId);
	if (lRet != 0) {
		OAL_LOG_WARNING("Failed to allocate physical memory\n");
		goto release_physical_meta;
	}

	LIST_INIT(&lpPhysMem->mVirtList);

	/* Register physical memory as part of the chunk */
	LIST_INSERT_HEAD(&apMemChunk->mAllocList, lpPhysMem, mChunkEntry);
	lpPhysMem->mpChunk = apMemChunk;

	apVirtMem->mOffset = 0U;

	/* It's not decided yet what mapping shall we have */
	apVirtMem->mpPhysAlloc = lpPhysMem;

release_physical_meta:
	if (lRet != 0) {
		OAL_SetUnusedPhysMemAllocation(lpPhysMem);
	}

alloc_phys_mem_exit:
	return lRet;
}

static int32_t releasePhysMem(struct OAL_MemoryHandle *apVirtMem)
{
	int32_t lRet = -1;
	struct PhysicalPageMapping *lpPhysMem;

	lpPhysMem = apVirtMem->mpPhysAlloc;
	if (lpPhysMem == NULL) {
		goto release_phys_exit;
	}

	lRet = OAL_OS_ReleasePhysicalMemory(lpPhysMem);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to release 0x%" PRIxPTR "\n",
		              lpPhysMem->mPhysAddr);
	} else {
		LIST_REMOVE(lpPhysMem, mChunkEntry);
		(void)OAL_SetUnusedPhysMemAllocation(lpPhysMem);

		apVirtMem->mpPhysAlloc = NULL;
		lRet                   = 0;
	}

release_phys_exit:
	return lRet;
}

static struct TreeNode **getHandleTree(struct OAL_MemoryHandle *apVirtMem)
{
	struct TreeNode **lpTree;
	struct PhysicalPageMapping *lpPhysMem = apVirtMem->mpPhysAlloc;
	struct MemoryChunk *lpMemChunk        = lpPhysMem->mpChunk;

	if (apVirtMem->mMapping == OAL_VIRT_CACHEABLE) {
		lpTree = &lpMemChunk->mpCacheTree;
	} else {
		lpTree = &lpMemChunk->mpNonCacheTree;
	}

	return lpTree;
}

static int32_t addToAvailabilityTree(struct OAL_MemoryHandle *apVirtMem)
{
	int32_t lRet = 0;
	struct TreeNode *lpFreeAreaNode;
	struct PhysicalPageMapping *lpPhysMem = apVirtMem->mpPhysAlloc;
	uintptr_t lFreeNodeOffset;
	struct TreeNode **lpTree;
	uintptr_t lLeftover = lpPhysMem->mSize - apVirtMem->mSize;
	uint8_t *lpVirtAddr = NULL;

	if (lLeftover < sizeof(struct TreeNode)) {
		goto add_to_tree_exit;
	}

	lpTree = getHandleTree(apVirtMem);

	lRet =
	    OAL_OS_GetVirtAddress(apVirtMem, &lpVirtAddr, apVirtMem->mMapping);
	if ((lRet != 0) || (lpVirtAddr == NULL)) {
		OAL_LOG_ERROR("Failed to get mapped address of 0x%" PRIxPTR
		              "\n",
		              lpPhysMem->mPhysAddr);
		if (lRet == 0) {
			lRet = -1;
		}
		goto add_to_tree_exit;
	}

	lFreeNodeOffset = (uintptr_t)lpVirtAddr;
	lFreeNodeOffset += apVirtMem->mSize;

	lpFreeAreaNode = (struct TreeNode *)lFreeNodeOffset;

	/* Add the leftover memory to free list */
	lRet = OAL_InsertTreeNode(lpFreeAreaNode, lpPhysMem->mTag, lLeftover,
	                          lpTree);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to add %p to free list\n",
		              (void *)lpFreeAreaNode);
	}

add_to_tree_exit:
	return lRet;
}

static int32_t mapVirtMem(struct OAL_MemoryHandle *apVirtMem,
                          enum OAL_MemoryMapping aMapping)
{
	int32_t lRet = -1;
	struct PhysicalPageMapping *lpPhysMem;
	struct MemoryChunk *lpMemChunk;

	lpPhysMem = apVirtMem->mpPhysAlloc;
	if (lpPhysMem == NULL) {
		goto map_virt_mem_exit;
	}

	lpMemChunk = lpPhysMem->mpChunk;
	if (lpMemChunk == NULL) {
		goto map_virt_mem_exit;
	}

	lRet = OAL_OS_MapPhysicalMemory(lpPhysMem, apVirtMem, aMapping);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to map 0x%" PRIxPTR "\n",
		              lpPhysMem->mPhysAddr);
		goto map_virt_mem_exit;
	}

	apVirtMem->mMapping = aMapping;
	apVirtMem->mOffset  = 0U;
	LIST_INSERT_HEAD(&lpPhysMem->mVirtList, apVirtMem, mVirtEntry);
map_virt_mem_exit:
	return lRet;
}

static int32_t unmapVirtMem(struct OAL_MemoryHandle *apVirtMem)
{
	int32_t lRet;

	LIST_REMOVE(apVirtMem, mVirtEntry);
	lRet = OAL_OS_UnmapPhysicalMemory(apVirtMem);

	return lRet;
}

static int32_t mapAndAddToTree(struct OAL_MemoryHandle *apVirtMem,
                               enum OAL_MemoryMapping aMapping)
{
	int32_t lRet = -1;

	lRet = mapVirtMem(apVirtMem, aMapping);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to map a memory handle\n");
		goto map_and_add_to_tree_exit;
	}

	/* Add to free list only if it makes sense */
	lRet = addToAvailabilityTree(apVirtMem);
	if (lRet != 0) {
		(void)unmapVirtMem(apVirtMem);
	}

map_and_add_to_tree_exit:
	return lRet;
}

static inline uint8_t isMapped(struct OAL_MemoryHandle *apMemHandle)
{
	uint8_t lRet                            = 0;
	struct PhysicalPageMapping *lpPhysAlloc = NULL;

	lpPhysAlloc = apMemHandle->mpPhysAlloc;
	if (lpPhysAlloc == NULL) {
		goto is_mapped_exit;
	}

	if (!LIST_EMPTY(&lpPhysAlloc->mVirtList)) {
		lRet = 1;
	}

is_mapped_exit:
	return lRet;
}

static int32_t unmapMemory(struct OAL_MemoryHandle *apVirtMem, uint8_t aFree)
{
	int32_t lRet = -1, lStatus;
	struct PhysicalPageMapping *lpPhysMem;
	struct MemoryChunk *lpMemChunk;
	uint8_t *lpVirtAddr = NULL;
	struct TreeNode **lpTree;

	lpPhysMem = apVirtMem->mpPhysAlloc;
	if (lpPhysMem == NULL) {
		goto unmap_exit;
	}

	lpMemChunk = lpPhysMem->mpChunk;
	if (lpMemChunk == NULL) {
		goto unmap_exit;
	}

	/* Add the node to free list */
	lRet =
	    OAL_OS_GetVirtAddress(apVirtMem, &lpVirtAddr, apVirtMem->mMapping);
	if ((lRet != 0) || (lpVirtAddr == NULL)) {
		OAL_LOG_ERROR(
		    "Failed to get virtual address of"
		    " the allocation 0x%" PRIxPTR "\n",
		    lpPhysMem->mPhysAddr);
		if (lRet == 0) {
			lRet = -1;
		}
		goto unmap_exit;
	}

	/* Get a reference to tree before setting mapping to NONE */
	lpTree              = getHandleTree(apVirtMem);
	apVirtMem->mMapping = OAL_VIRT_NONE;

	LIST_REMOVE(apVirtMem, mVirtEntry);

	/*
	 * Add memory back to the tree only if the virtual memory is just a part
	 * of the initial physical allocation.
	 */
	if (apVirtMem->mSize != lpPhysMem->mSize) {
		lStatus = OAL_InsertTreeNode(OAL_GET_TREE_NODE(lpVirtAddr, 0U),
		                             lpPhysMem->mTag, apVirtMem->mSize,
		                             lpTree);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed the insertion of %p\n",
			              lpVirtAddr);
			lRet = lStatus;
			goto unmap_phys_mem;
		}
	}

	/*
	 * Remove node from tree if there are no any other mappings on current
	 * physical allocation.
	 */
	if ((aFree != 0U) && LIST_EMPTY(&lpPhysMem->mVirtList)) {
		/* Delete coalesced node */
		if (apVirtMem->mSize != lpPhysMem->mSize) {
			/* Remove the entire physically allocate chunk */
			uintptr_t lNodeAddr =
			    ((uintptr_t)lpVirtAddr) - apVirtMem->mOffset;
			lStatus = OAL_RemoveIntervalFromTree(
			    lNodeAddr, lpPhysMem->mSize, lpTree);
			if (lStatus != 0) {
				OAL_LOG_ERROR(
				    "Failed to delete (%p + 0x%" PRIxPTR ") \n",
				    lpVirtAddr, lpPhysMem->mSize);
				lRet = lStatus;
			}
		}
	}

unmap_phys_mem:
	lStatus = OAL_OS_UnmapPhysicalMemory(apVirtMem);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to unmap 0x%" PRIxPTR "\n",
		              lpPhysMem->mPhysAddr);
		lRet = lStatus;
		goto unmap_exit;
	}

unmap_exit:
	return lRet;
}

static int32_t initVirtMemAlloc(struct OAL_MemoryHandle **apVirtMem,
                                uintptr_t aSize)
{
	int32_t lRet;
	struct OAL_MemoryHandle *lpVirtMem;
	uint32_t lCacheLineSize;

	/* Virtual memory metadata */
	lRet = OAL_GetUnusedVirtMemAllocation(apVirtMem);
	if (lRet != 0) {
		goto init_virt_mem_exit;
	}

	lCacheLineSize = OAL_OS_GetCacheLineSize();

	lpVirtMem = *apVirtMem;

	(void)memset(lpVirtMem, 0, sizeof(*lpVirtMem));
	lpVirtMem->mMapping = OAL_VIRT_NONE;
	lpVirtMem->mSize    = OAL_ROUNDUP(aSize, lCacheLineSize);

init_virt_mem_exit:
	return lRet;
}

static void deinitVirtMemAlloc(struct OAL_MemoryHandle **apVirtMem)
{
	OAL_SetUnusedVirtMemAllocation(*apVirtMem);
	*apVirtMem = NULL;
}

static int32_t allocPhysAndAddToTree(OAL_MEMORY_FLAG aFlags,
                                     struct MemoryChunk *apMemChunk,
                                     struct OAL_MemoryHandle *apVirtMem,
                                     enum OAL_MemoryMapping aMapping)
{
	int32_t lRet;
	uint32_t lAlign;
	struct PhysicalPageMapping *lpPhysMem;

	/* Allocate new physical memory */
	lRet = allocPhysMem(&lAlign, apMemChunk, &lpPhysMem, apVirtMem, aFlags);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to allocate physical memory\n");
		goto alloc_add_tree;
	}

	/* Map it and add to tree */
	lRet = mapAndAddToTree(apVirtMem, aMapping);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to map\n");
	}

	if (lRet != 0) {
		(void)releasePhysMem(apVirtMem);
	}

alloc_add_tree:
	return lRet;
}

static int32_t getMemoryChunk(uint8_t aChunkId, struct MemoryChunk **apMemChunk)
{
	int32_t lRet = 0;

	if (OAL_IsChunkAvailable(aChunkId, apMemChunk) == 0U) {
		lRet = OAL_GetUnusedMemoryChunk(apMemChunk);
		if (lRet != 0) {
			lRet = -ENOMEM;
			OAL_LOG_ERROR("Please adjust chunks pool size\n");
			goto get_mem_chunk_exit;
		}

		(*apMemChunk)->mId            = aChunkId;
		(*apMemChunk)->mpCacheTree    = NULL;
		(*apMemChunk)->mpNonCacheTree = NULL;
		LIST_INIT(&(*apMemChunk)->mAllocList);
	}

get_mem_chunk_exit:
	return lRet;
}

static void tryToReleaseMemoryChunk(struct MemoryChunk *apMemChunk)
{
	if (LIST_EMPTY(&apMemChunk->mAllocList)) {
		(void)OAL_SetUnusedMemoryChunks(apMemChunk);
	}
}

static struct OAL_MemoryHandle *allocMemoryArea(uintptr_t aSize,
                                                uint32_t aAlign,
                                                uint8_t aChunkId,
                                                OAL_MEMORY_FLAG aFlags)
{
	int32_t lRet;
	struct OAL_MemoryHandle *lpVirtMem    = NULL;
	struct PhysicalPageMapping *lpPhysMem = NULL;
	struct MemoryChunk *lpMemChunk;
	uintptr_t lSize = aSize;
	uint32_t lAlign = aAlign;

	lRet = getMemoryChunk(aChunkId, &lpMemChunk);
	if (lRet != 0) {
		goto alloc_mem_area_exit;
	}

	lRet = initVirtMemAlloc(&lpVirtMem, lSize);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize a handle\n");
		goto free_mem_chunk;
	}

	/* Allocate new physical memory */
	lRet = allocPhysMem(&lAlign, lpMemChunk, &lpPhysMem, lpVirtMem, aFlags);
	if ((lRet != 0) || (lpPhysMem == NULL)) {
		OAL_LOG_WARNING("Failed to allocate physical memory\n");
	} else {
		/*
		 * The virtual range matches the size of the
		 * physical allocation
		 */
		lpVirtMem->mSize = lpPhysMem->mSize;
	}

	if (lRet != 0) {
		deinitVirtMemAlloc(&lpVirtMem);
	}
free_mem_chunk:
	if (lRet != 0) {
		tryToReleaseMemoryChunk(lpMemChunk);
	}
alloc_mem_area_exit:
	return (void *)lpVirtMem;
}

static int32_t gsMemAllocError = 0;
static void mem_alloc_constructor(void)
{
	int32_t lRet;

	lRet = OAL_InitializeMutex(&gsOalMemAllocMutex);
	if (lRet != 0) {
		gsMemAllocError = -EINVAL;
		OAL_LOG_ERROR("Failed to initialize global mutex\n");
		goto mem_alloc_cons_exit;
	}

	lRet = OAL_OS_Initialize();
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize OS part\n");
		gsMemAllocError = -EINVAL;
	}

mem_alloc_cons_exit:
	return;
}

int32_t OAL_Deinitialize(void)
{
	int32_t lRet;
	lRet = OAL_OS_Deinitialize();
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to deinitialize OS part\n");
	}

	return lRet;
}

int32_t OAL_Initialize(void)
{
	static struct OAL_OnceControl lsOALInitialization = OAL_ONCE_INIT;
	int32_t lRet                                      = 0;
	(void)OAL_ExecuteOnce(&lsOALInitialization, mem_alloc_constructor);
	if (gsMemAllocError != 0) {
		OAL_LOG_ERROR("Failed to initialize OAL library\n");
		lRet            = gsMemAllocError;
		gsMemAllocError = 0;
	}

	return lRet;
}

static enum OAL_MemoryMapping getMapping(OAL_MEMORY_ACCESS aAccess)
{
	enum OAL_MemoryMapping lReqMapType;
	if ((aAccess == OAL_ACCESS_NCH_NB) || (aAccess == OAL_ACCESS_NCH_B)) {
		lReqMapType = OAL_VIRT_NON_CACHEABLE;
	} else if ((aAccess == OAL_ACCESS_CH_WB) ||
	           (aAccess == OAL_ACCESS_CH_WT)) {
		lReqMapType = OAL_VIRT_CACHEABLE;
	} else if (aAccess == OAL_ACCESS_PHY) {
		lReqMapType = OAL_VIRT_NONE;
	} else {
		lReqMapType = OAL_VIRT_INVALID;
	}

	return lReqMapType;
}

static int32_t attachVirtMemToPhys(struct MemoryChunk *apMemChunk,
                                   struct OAL_MemoryHandle *apVirtMem,
                                   void *apVirtAddr,
                                   enum OAL_MemoryMapping aMapping)
{
	int32_t lRet;
	struct PhysicalPageMapping *lpPhysMem = NULL;

	apVirtMem->mMapping = aMapping;

	/* Reuse memory */
	lRet = OAL_GetPhysicalPageMapping(apMemChunk, apVirtAddr, aMapping,
	                                  &lpPhysMem, &apVirtMem->mOffset);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "Failed to identify physical memory "
		    "region corresponding to virtual "
		    "address %p\n",
		    apVirtAddr);
		goto attach_exit;
	}

	apVirtMem->mpPhysAlloc = lpPhysMem;
	lRet                   = OAL_OS_CopyMappingInfo(apVirtMem,
                                      LIST_FIRST(&lpPhysMem->mVirtList));
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to duplicate OS metadata\n");
	} else {
		LIST_INSERT_HEAD(&lpPhysMem->mVirtList, apVirtMem, mVirtEntry);
	}
attach_exit:
	return lRet;
}

struct OAL_MemoryHandle *OAL_AllocAndMapMemory(uint32_t aSize,
                                               OAL_MEMORY_FLAG aFlags,
                                               OAL_MEMORY_ACCESS aAccess)
{
	enum OAL_MemoryMapping lMapping = getMapping(aAccess);
	struct OAL_MemoryHandle *lpRet  = NULL;
	struct MemoryChunk *lpMemChunk  = NULL;
	uint8_t lChunkID                = getChunk(aFlags);
	struct TreeNode **lpTree;
	int32_t lRet = 0, lStatus;
	void *lpVirtAddr;
	uintptr_t lPhysTag;

	lStatus = OAL_Initialize();
	if (lStatus != OAL_SUCCESS) {
		OAL_LOG_ERROR("Failed to initialize OAL\n");
		goto alloc_and_map_exit;
	}

	if (aAccess == OAL_ACCESS_PHY) {
		OAL_LOG_ERROR("This function doen't accept " OAL_STR(
		    OAL_ACCESS_PHY) " as access flag\n");
		goto alloc_and_map_exit;
	}

	if ((lMapping != OAL_VIRT_CACHEABLE) &&
	    (lMapping != OAL_VIRT_NON_CACHEABLE)) {
		OAL_LOG_ERROR("Invalid access flags\n");
		goto alloc_and_map_exit;
	}

	lRet = OAL_LockMutex(&gsOalMemAllocMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to take allocator mutex\n");
		goto alloc_and_map_exit;
	}

	lRet = getMemoryChunk(lChunkID, &lpMemChunk);
	if (lRet != 0) {
		goto unlock_mutex;
	}

	if (lMapping == OAL_VIRT_CACHEABLE) {
		lpTree = &lpMemChunk->mpCacheTree;
	} else {
		lpTree = &lpMemChunk->mpNonCacheTree;
	}

	lRet = initVirtMemAlloc(&lpRet, aSize);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize a handle\n");
		goto free_mem_chunk;
	}

	/* If the tree is empty ... */
	lRet =
	    OAL_AllocateFromTree(lpRet->mSize, lpTree, &lpVirtAddr, &lPhysTag);
	if (lRet != 0) {
		lRet =
		    allocPhysAndAddToTree(aFlags, lpMemChunk, lpRet, lMapping);
		if (lRet != 0) {
			deinitVirtMemAlloc(&lpRet);
		}
		goto unlock_mutex;
	}

	lRet = attachVirtMemToPhys(lpMemChunk, lpRet, lpVirtAddr, lMapping);
	if (lRet != 0) {
		uintptr_t lFreeNodeOffset = (uintptr_t)lpVirtAddr;
		struct TreeNode *lpFreeAreaNode =
		    (struct TreeNode *)lFreeNodeOffset;

		OAL_LOG_ERROR("Failed to map a virtual page\n");

		/* Add back the memory allocated above */
		lStatus = OAL_InsertTreeNode(lpFreeAreaNode, lPhysTag,
		                             lpRet->mSize, lpTree);
		if (lStatus != 0) {
			OAL_LOG_ERROR("Failed to add %p to free list\n",
			              (void *)lpFreeAreaNode);
		}
	} else {
		if ((aFlags & OAL_MEMORY_FLAG_ZERO) != 0U) {
			(void)memset((void *)lpVirtAddr, 0, aSize);
			(void)OAL_FlushAndInvalidateMemory(
			    (uintptr_t)lpVirtAddr, aSize);
		}
	}

	if (lRet != 0) {
		deinitVirtMemAlloc(&lpRet);
	}

free_mem_chunk:
	if (lRet != 0) {
		tryToReleaseMemoryChunk(lpMemChunk);
	}

unlock_mutex:
	(void)OAL_UnlockMutex(&gsOalMemAllocMutex);

alloc_and_map_exit:
	if (lRet != 0) {
		lpRet = NULL;
	}
	return lpRet;
}

static uint8_t *getVirtualAddress(struct OAL_MemoryHandle *apMemHandle,
                                  enum OAL_MemoryMapping aMapping)
{
	uint8_t *lpVirtAddr = NULL;
	int32_t lRet;
	uint8_t lUnmap = 0U;

	/* Map it if it's not*/
	if (isMapped(apMemHandle) == 0U) {
		lRet = mapVirtMem(apMemHandle, aMapping);
		if (lRet != 0) {
			goto get_virtual_exit;
		}
		lUnmap = 1U;
	}

	lRet = OAL_OS_GetVirtAddress(apMemHandle, &lpVirtAddr, aMapping);
	if ((lRet != 0) || (lpVirtAddr == NULL)) {
		OAL_LOG_ERROR("Failed to get mapped address\n");
		if (lRet == 0) {
			lpVirtAddr = NULL;
		}
		if (lUnmap != 0U) {
			(void)unmapVirtMem(apMemHandle);
		}
	}

get_virtual_exit:
	return lpVirtAddr;
}

struct OAL_MemoryHandle *OAL_AllocMemory(uint32_t aSize, OAL_MEMORY_FLAG aFlags)
{
	struct OAL_MemoryHandle *lpRet = NULL;
	uint8_t *lpVirtAddr            = NULL;
	uint8_t lChunkID               = getChunk(aFlags);
	uint32_t lAlign;

	int32_t lStatus = OAL_Initialize();
	if (lStatus != OAL_SUCCESS) {
		OAL_LOG_ERROR("Failed to initialize OAL\n");
		goto oal_alloc_mem_exit;
	}

	if (getAlignment(aFlags, &lAlign) != 0) {
		OAL_LOG_ERROR("Failed to get alignment from flags\n");
		goto oal_alloc_mem_exit;
	}

	lStatus = OAL_LockMutex(&gsOalMemAllocMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to take OAL library lock\n");
		goto oal_alloc_mem_exit;
	}

	lpRet = allocMemoryArea(aSize, lAlign, lChunkID, aFlags);

	if (lpRet != NULL) {
		/* Memory zeroisation */
		if ((aFlags & OAL_MEMORY_FLAG_ZERO) != 0U) {
			lpVirtAddr =
			    getVirtualAddress(lpRet, OAL_VIRT_CACHEABLE);
			if (lpVirtAddr != NULL) {
				(void)memset((void *)lpVirtAddr, 0, aSize);
				(void)OAL_FlushMemory((uintptr_t)lpVirtAddr,
				                      aSize);
				(void)unmapMemory(lpRet, 0U);
				lpVirtAddr = NULL;
			} else {
				OAL_LOG_ERROR("Failed to zeroize\n");
				(void)OAL_UnlockMutex(&gsOalMemAllocMutex);
				(void)OAL_FreeMemory(lpRet);
				lpRet = NULL;
				goto oal_alloc_mem_exit;
			}
		}
	}

	(void)OAL_UnlockMutex(&gsOalMemAllocMutex);

oal_alloc_mem_exit:
	return lpRet;
}

int32_t OAL_FreeMemory(struct OAL_MemoryHandle *apMemHandle)
{
	int32_t lRet                            = 0;
	struct PhysicalPageMapping *lpPhysAlloc = NULL;
	struct MemoryChunk *lpMemChunk          = NULL;
	if (apMemHandle == NULL) {
		lRet = -1;
		goto free_memory_exit;
	}

	lpPhysAlloc = apMemHandle->mpPhysAlloc;
	if (lpPhysAlloc == NULL) {
		lRet = -1;
		goto free_memory_exit;
	}

	lpMemChunk = lpPhysAlloc->mpChunk;
	if (lpMemChunk == NULL) {
		lRet = -1;
		goto free_memory_exit;
	}

	lRet = OAL_LockMutex(&gsOalMemAllocMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to take lock\n");
		goto free_memory_exit;
	}

	/* Is it mapped or not */
	if (isMapped(apMemHandle) != 0U) {
		lRet = unmapMemory(apMemHandle, 1U);
	}

	if ((lRet == 0) && (isMapped(apMemHandle) == 0U)) {
		lRet = releasePhysMem(apMemHandle);
	}

	(void)OAL_SetUnusedVirtMemAllocation(apMemHandle);

	tryToReleaseMemoryChunk(lpMemChunk);

	(void)OAL_UnlockMutex(&gsOalMemAllocMutex);

free_memory_exit:
	if (lRet != 0) {
		lRet = OAL_FAILURE;
	}
	return lRet;
}

int32_t OAL_UnmapMemory(struct OAL_MemoryHandle *apMemHandle)
{
	int32_t lRet                            = 0;
	struct PhysicalPageMapping *lpPhysAlloc = NULL;

	if (apMemHandle == NULL) {
		lRet = -1;
		goto unmap_memory_exit;
	}

	lpPhysAlloc = apMemHandle->mpPhysAlloc;
	if (lpPhysAlloc == NULL) {
		/* Corrupted metadata */
		lRet = -1;
		goto unmap_memory_exit;
	}

	(void)OAL_LockMutex(&gsOalMemAllocMutex);

	/* Is it mapped and allocated using OAL_AllocMemory ? */
	if ((isMapped(apMemHandle) != 0U) &&
	    (apMemHandle->mSize == lpPhysAlloc->mSize)) {
		lRet = unmapMemory(apMemHandle, 0U);
	}

	(void)OAL_UnlockMutex(&gsOalMemAllocMutex);

unmap_memory_exit:
	return lRet;
}

uintptr_t OAL_GetReturnAddress(struct OAL_MemoryHandle *apMemHandle,
                               OAL_MEMORY_ACCESS aAccess)
{
	uintptr_t lRet = 0U;
	int32_t lStatus;
	enum OAL_MemoryMapping lReqMapType = getMapping(aAccess);

	if (apMemHandle == NULL) {
		goto get_buffer_exit;
	}

	lStatus = OAL_LockMutex(&gsOalMemAllocMutex);
	if (lStatus != 0) {
		OAL_LOG_ERROR("Failed to take lock\n");
		goto get_buffer_exit;
	}

	if (aAccess == OAL_ACCESS_PHY) {
		struct PhysicalPageMapping *lpPhys;
		lpPhys = apMemHandle->mpPhysAlloc;

		lRet = lpPhys->mPhysAddr;
		lRet += apMemHandle->mOffset;

		goto get_address_unlock_mutex;
	}

	if (lReqMapType == OAL_VIRT_INVALID) {
		OAL_LOG_ERROR("Undefined behaviour for 0x%" PRIx32 " flag\n",
		              aAccess);
		goto get_address_unlock_mutex;
	}

	if ((apMemHandle->mMapping == OAL_VIRT_NONE) ||
	    (lReqMapType == apMemHandle->mMapping)) {
		lRet = (uintptr_t)getVirtualAddress(apMemHandle, lReqMapType);
	} else {
		OAL_LOG_WARNING(
		    "Detected multiple memory mappings with different cache "
		    "policies of the same physical page. Please consider "
		    "revision of the code to prevent different caching "
		    "settings.\n");
	}

get_address_unlock_mutex:
	lStatus = OAL_UnlockMutex(&gsOalMemAllocMutex);
	if (lStatus != 0) {
		lRet = 0U;
		if (isMapped(apMemHandle) != 0U) {
			lStatus = unmapMemory(apMemHandle, 0U);
			if (lStatus != 0) {
				OAL_LOG_ERROR("Failed to unmap\n");
			}
		}

		OAL_LOG_ERROR("Failed to release lock\n");
	}

get_buffer_exit:
	return lRet;
}

int32_t OAL_FlushMemory(uintptr_t aAddr, uint32_t aSize)
{
	return OAL_OS_FlushMemory(aAddr, aSize);
}

int32_t OAL_InvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	return OAL_OS_InvalidateMemory(aAddr, aSize);
}

int32_t OAL_FlushAndInvalidateMemory(uintptr_t aAddr, uint32_t aSize)
{
	return OAL_OS_FlushAndInvalidateMemory(aAddr, aSize);
}

int32_t OAL_FlushAndInvalidateAll(void)
{
	int32_t lRet = 0;
	uintptr_t lVirtAddr;
	struct OAL_MemoryHandle *lpIterator;
	uint32_t lRegSize;
	lpIterator = NULL;

	lRet = OAL_LockMutex(&gsOalMemAllocMutex);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get lock\n");
		goto flush_exit;
	}

	do {
		OAL_GetNextUsedMemAllocation(&lpIterator);
		if (lpIterator == NULL) {
			break;
		}

		if (lpIterator->mMapping == OAL_VIRT_CACHEABLE) {
			lVirtAddr = (uintptr_t)getVirtualAddress(
			    lpIterator, OAL_VIRT_CACHEABLE);
			if (lVirtAddr == 0U) {
				continue;
			}

			if (lpIterator->mSize > OAL_MAX_VAR_VALUE(lRegSize)) {
				OAL_LOG_ERROR("Corrupted memory handle %p",
				              (void *)lpIterator);
				continue;
			}
			lRegSize = (uint32_t)(lpIterator->mSize &
			                      OAL_MAX_VAR_VALUE(lRegSize));
			(void)OAL_FlushAndInvalidateMemory(lVirtAddr, lRegSize);
		}
	} while (1U == 1U);
	(void)OAL_UnlockMutex(&gsOalMemAllocMutex);

flush_exit:
	return lRet;
}

uint8_t OAL_CheckAllocated(const void *acpHandle)
{
	struct OAL_MemoryHandle *lpIterator;
	uint8_t lRet = 0U;

	if (acpHandle == NULL) {
		goto check_allocated_exit;
	}

	lpIterator = NULL;
	(void)OAL_LockMutex(&gsOalMemAllocMutex);
	do {
		OAL_GetNextUsedMemAllocation(&lpIterator);

		if (lpIterator == acpHandle) {
			lRet = 1U;
			break;
		}
	} while (lpIterator != NULL);
	(void)OAL_UnlockMutex(&gsOalMemAllocMutex);

check_allocated_exit:
	return lRet;
}

uintptr_t OAL_GetSizeOfAllocatedMemory(void)
{
	struct OAL_MemoryHandle *lpIterator;
	uintptr_t lSize = 0U;

	lpIterator = NULL;
	(void)OAL_LockMutex(&gsOalMemAllocMutex);
	do {
		OAL_GetNextUsedMemAllocation(&lpIterator);

		if (lpIterator != NULL) {
			lSize += lpIterator->mSize;
		}

	} while (lpIterator != NULL);
	(void)OAL_UnlockMutex(&gsOalMemAllocMutex);

	return lSize;
}
