/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_kernel_memory_allocator.h>
#include <oal_memmap.h>
#include <oal_page_manager.h>

#include <oal_log.h>
#include <oal_add_reserved_mem.h>
#include <oal_comm_kernel.h>

int32_t OAL_AddReservedMemoryRegion(char8_t *apName, uintptr_t aPhysAddr,
                                    uintptr_t aSize, uint32_t aStartID,
                                    uint32_t aAlign, uint8_t aInit,
                                    uint8_t aAutobalance)
{
	int32_t lRet                             = 0;
	struct OALMemoryChunk *lpMemChunk        = NULL;
	struct OAL_MemoryAllocatorRegion lResMem = {
	    .mPhysAddr    = aPhysAddr,
	    .mSize        = aSize,
	    .mStartID     = aStartID,
	    .mAlign       = aAlign,
	    .mInit        = aInit,
	    .mAutobalance = aAutobalance,
	};

	OAL_CHECK_NULL_PARAM(apName, add_res_mem_reg_exit);

	lRet = OAL_InitMemoryPools(0U);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize memory pools\n");
		goto add_res_mem_reg_exit;
	}

	(void)memcpy(lResMem.mName, apName,
	             strnlen(apName, sizeof(lResMem.mName)));
	lRet = OAL_AddMemoryPool(&lResMem, &lpMemChunk);
	if ((lRet != 0) || (lpMemChunk == NULL)) {
		OAL_LOG_ERROR("Failed to add '%s' to memory pools\n",
		              lResMem.mName);
		goto add_res_mem_reg_exit;
	}

	lpMemChunk->mOSChunk.mStartPhysAddress = lResMem.mPhysAddr;

	if (lResMem.mInit != 0U) {
		uintptr_t lVirt;
		OAL_PRINT("Initializing '%s'\n", lResMem.mName);
		lVirt = OAL_kmemmap(lResMem.mPhysAddr, lResMem.mSize);
		if (lVirt != 0U) {
			uintptr_t lIdx;
			for (lIdx = 0U; lIdx < lResMem.mSize; lIdx += 8U) {
				*((volatile uint64_t *)(lVirt + lIdx)) = 0x0ULL;
			}
			(void)OAL_kmemunmap(lVirt, (size_t)lResMem.mSize);
		} else {
			OAL_LOG_ERROR("Failed to initialize '%s'\n",
			              lResMem.mName);
			lRet = -1;
		}
	}

add_res_mem_reg_exit:
	return lRet;
}
