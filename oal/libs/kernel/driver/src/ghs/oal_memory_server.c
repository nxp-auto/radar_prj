/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <INTEGRITY.h>
#include <stdio.h>
#include <stdlib.h>

#include <oal_driver_dispatcher.h>
#include <oal_driver_functions.h>
#include <oal_kernel_memory_allocator.h>
#include <oal_log.h>
#include <bsp_export.h>

#include <oal_kernel_memory_allocator.h>
#include <oal_page_manager.h>
#include <os_oal_mem_allocator.h>
#include <os_oal_server.h>

#include <ghs_kernel_comm_conn.h>
#include <ghs_mr_utils.h>

static Error zeroizeMemoryRegion(MemoryRegion aPhysMR, const char8_t *acpName)
{
	Error lError = Success;
	Address lStartAddr, lEndAddr;
	Value lSize;
	Address lStartVirt, lEndVirt;
	MemoryRegion lVirtMR;
	uint64_t lOffset;

	OAL_UNUSED_ARG(acpName);
	lError = GetMemoryRegionAddresses(aPhysMR, &lStartAddr, &lEndAddr);
	if (Success != lError) {
		OAL_LOG_ERROR("Failed to get addresses for %s (%s)\n", acpName,
		              ErrorString(lError));
		goto zeroize_exit;
	}

	lSize  = lEndAddr + 1U - lStartAddr;
	lError = OAL_GHS_AllocAndMapMemoryReg(lSize, &lVirtMR, &aPhysMR, 0U);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to map memory (%s)\n",
		              ErrorString(lError));
		goto zeroize_exit;
	}

	lError = GetMemoryRegionAddresses(lVirtMR, &lStartVirt, &lEndVirt);
	if (Success != lError) {
		OAL_LOG_ERROR("Failed to get addresses for %s (%s)\n", acpName,
		              ErrorString(lError));
		goto unmap_vm;
	}

	for (lOffset = 0U; lOffset < lSize; lOffset += sizeof(Value)) {
		*((uint64_t *)(lStartVirt + lOffset)) = 0x0U;
	}

unmap_vm:
	(void)OAL_GHS_UnmapAndReleaseVirtual(lVirtMR);

zeroize_exit:
	return lError;
}

static Error initDeviceData(void)
{
	Error lError = Success;
	int32_t lRet;
	IODevice lIODevice;
	Value lIdx, lPageNumber;
	Address lEndAddr;
	Value lNPages;
	MemoryRegion lChunkMR;
	struct OALMemoryChunk *lpMemChunk;
	struct OAL_MemoryAllocatorRegion lResMemory;
	Value lNumRegions;

	if (OAL_InitMemoryPools(0U) != 0) {
		lError = Failure;
		goto init_dev_exit;
	}

#ifdef OALMemoryAllocatorDev
	/* Without resource manager */
	lIODevice = OALMemoryAllocatorDev;
#else
	/* Connect to the device */
	lError = RequestResource((Object *)&lIODevice, OAL_GHS_DEV_NAME,
	                         "!systempassword");
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to request '%s' resource (%s).\n",
		              OAL_GHS_DEV_NAME, ErrorString(lError));
		goto init_dev_exit;
	}
#endif

	/* Get number of reserved regions */
	lError = ReadIODeviceRegister(lIODevice, OAL_GET_NUMBER_OF_REGIONS,
	                              &lNumRegions);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get number of regions (%s)\n",
		              ErrorString(lError));
		goto init_dev_exit;
	}

	/* Get kernel data */
	for (lIdx = OAL_FIRST_BUFFER_INDEX;
	     lIdx < (lNumRegions + OAL_FIRST_BUFFER_INDEX); lIdx++) {
		lError = ReadIODeviceStatus(lIODevice, lIdx, &lResMemory,
		                            sizeof(lResMemory));
		if (lError != Success) {
			OAL_LOG_ERROR(
			    "Failed to get reserved region: %d (%s)\n",
			    lIdx - OAL_FIRST_BUFFER_INDEX, ErrorString(lError));
			goto init_dev_exit;
		}

		lRet = OAL_AddMemoryPool(&lResMemory, &lpMemChunk);
		if (lRet != 0) {
			OAL_LOG_ERROR("Failed to add '%s' to page allocator\n",
			              lResMemory.mName);
			goto init_dev_exit;
		}

		lError = RequestResource((Object *)&lChunkMR, lResMemory.mName,
		                         "!systempassword");
		if (Success != lError) {
			OAL_LOG_ERROR("Failed to get the resource %s (%s)\n",
			              lResMemory.mName, ErrorString(lError));
			break;
		}

		lError = GetMemoryRegionAddresses(
		    lChunkMR, &lpMemChunk->mOSChunk.mFirstKernelAddress,
		    &lEndAddr);
		if (Success != lError) {
			OAL_LOG_ERROR("Failed to get addresses for %s (%s)\n",
			              lResMemory.mName, ErrorString(lError));
			break;
		}

		if (lResMemory.mInit != 0U) {
			OAL_PRINT("\tInitializing '%s' ...\n", lResMemory.mName);
			lError =
			    zeroizeMemoryRegion(lChunkMR, lResMemory.mName);
			OAL_PRINT("\tInitialized '%s'\n", lResMemory.mName);
		}

		lNPages =
		    (lEndAddr + 1U - lpMemChunk->mOSChunk.mFirstKernelAddress) /
		    (uint64_t)ASP_PAGESIZE;
		for (lPageNumber = 0U; lPageNumber < lNPages; lPageNumber++) {
			lError = SplitMemoryRegion(
			    lChunkMR, ASP_PAGESIZE,
			    &lpMemChunk->mOSChunk.mPages[lPageNumber]);
			if (lError != Success) {
				OAL_LOG_ERROR(
				    "Failed to split the region (%s)\n",
				    ErrorString(lError));
				break;
			}
		}

		if (lError != Success) {
			break;
		}
	}

init_dev_exit:
	return lError;
}

static Error getMemoryChunkBasedOnMR(MemoryRegion aPhysMR,
                                     struct OALMemoryChunk **apMemChunk)
{
	Value lIt;
	Address lPhysStart, lPhysEnd;
	Address lChunkStart, lChunkEnd;
	Error lError;
	Value lNPools;
	struct OALMemoryChunk *lpMemPools, *lpMemChunk;

	lError = GetMemoryRegionAddresses(aPhysMR, &lPhysStart, &lPhysEnd);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get memory region boundaries (%s)\n",
		              ErrorString(lError));
		goto get_mem_chunk_exit;
	}

	lNPools     = OAL_GetNumberOfPools();
	lpMemPools  = OAL_GetMemoryPools();
	*apMemChunk = NULL;
	for (lIt = 0U; lIt < lNPools; lIt++) {
		lpMemChunk  = &lpMemPools[lIt];
		lChunkStart = lpMemChunk->mOSChunk.mFirstKernelAddress;
		lChunkEnd   = lChunkStart + lpMemChunk->mMemoryRegion.mSize;

		if ((lChunkStart <= lPhysStart) && (lChunkEnd >= lPhysEnd)) {
			*apMemChunk = lpMemChunk;
			lError      = Success;
			break;
		}
	}

get_mem_chunk_exit:
	return lError;
}

Error OAL_GHS_AllocateMemory(CMD_ALLOC_TYPE *apAux, MemoryRegion *apMemRegion)
{
	Error lError = Success;
	int32_t lRet;
	struct OALMemoryChunk *lpMemChunk;
	Address lPhysStart = NULLAddress;
	Address lPhysEnd;
	uint64_t lPagesIndex;
	uint64_t lNPages, lIdx;

	lRet = OAL_GetFreePagesFromChunk(apAux->mChunkId, apAux->mSize,
	                             &lPagesIndex, &lNPages, &lpMemChunk);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to allocate 0x%llx bytes from chunk %d\n",
		              apAux->mSize, apAux->mChunkId);
		lError = NotEnoughBuffers;
		goto allocate_mem_exit;
	}

	for (lIdx = (lPagesIndex + 1U); lIdx < (lPagesIndex + lNPages);
	     lIdx++) {
		lError =
		    MergeMemoryRegions(lpMemChunk->mOSChunk.mPages[lIdx],
		                       lpMemChunk->mOSChunk.mPages[lIdx - 1U]);
		if (lError != Success) {
			OAL_LOG_ERROR(
			    "Failed to merge 2 physical memory regions (%s)\n",
			    ErrorString(lError));
			break;
		}
	}

	if (lError != Success) {
		(void)OAL_SetFreePages(lpMemChunk, lPagesIndex, lNPages);
		goto allocate_mem_exit;
	}

	/* The result of merge operation */
	*apMemRegion = lpMemChunk->mOSChunk.mPages[lPagesIndex + lNPages - 1U];
	lError = GetMemoryRegionAddresses(*apMemRegion, &lPhysStart, &lPhysEnd);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get memory region boundaries (%s)\n",
		              ErrorString(lError));
		(void)OAL_GHS_FreeMemory(*apMemRegion);
		goto allocate_mem_exit;
	}

	apAux->mRetPhysPointer =
	    (lPhysStart - lpMemChunk->mOSChunk.mFirstKernelAddress) +
	    lpMemChunk->mMemoryRegion.mPhysAddr;

allocate_mem_exit:
	return lError;
}

Error OAL_GHS_FreeMemory(MemoryRegion aMemRegion)
{
	Error lError       = Success;
	Address lPhysStart = NULLAddress;
	Address lPhysEnd;
	struct OALMemoryChunk *lpMemChunk;
	uint64_t lPagesIndex;
	uint64_t lNPages, lIdx, lLeftSize;
	int32_t lRet;

	lError = GetMemoryRegionAddresses(aMemRegion, &lPhysStart, &lPhysEnd);

	lError = getMemoryChunkBasedOnMR(aMemRegion, &lpMemChunk);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get a valid chunk for a PhysMR (%s)\n",
		              ErrorString(lError));
		goto free_memory_exit;
	}

	/* Last page index */
	lPagesIndex =
	    (lPhysEnd + 1U - lpMemChunk->mOSChunk.mFirstKernelAddress) /
	    (uint64_t)ASP_PAGESIZE;
	lNPages = (lPhysEnd - lPhysStart + 1U) / (uint64_t)ASP_PAGESIZE;

	if ((lPagesIndex - 1U) > 0U) {
		lpMemChunk->mOSChunk.mPages[lPagesIndex - 1U] = aMemRegion;
	}

	lLeftSize = (lNPages - 1U) * (uint64_t)ASP_PAGESIZE;
	for (lIdx = lPagesIndex - 1U; lIdx > 0U; lIdx--) {
		if (lLeftSize <= 0U) {
			break;
		}
		lError = SplitMemoryRegion(
		    lpMemChunk->mOSChunk.mPages[lIdx], lLeftSize,
		    &lpMemChunk->mOSChunk.mPages[lIdx - 1U]);

		if (lError != Success) {
			OAL_LOG_ERROR("Failed to split pages (%s)\n",
			              ErrorString(lError));
			break;
		}

		lLeftSize -= (uint64_t)ASP_PAGESIZE;
	}

	/* Mark the pages as free */
	lRet = OAL_SetFreePages(lpMemChunk, lPagesIndex - lNPages, lNPages);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to mark the pages as free\n");
		lError = Failure;
	}

free_memory_exit:
	return lError;
}

ObjectIndex *__ghs_named_object_table_OALMemoryServerConnPair =
    (ObjectIndex *)(-1);
Address __ghs_named_object_table_OALMemoryServerConnPair_size = (Address)(-1);

int main(void)
{
	Error lError = Success;
	Connection lServerConn;
	OAL_RPCService_t lHandle;

	OAL_PRINT("OAL Memory Server\n");

	if (__ghs_named_object_table_OALMemoryServerConnPair_size !=
	    (Address)(-1)) {
		lServerConn = ConnectionObjectNumber(
		    __ghs_named_object_table_OALMemoryServerConnPair[0]);
	} else {
		OAL_LOG_ERROR(
		    "Failed to get server connection.\n"
		    "Please check the setting from .int file\n");
		goto main_exit;
	}

	if (lServerConn == NULLConnection) {
		lError = Failure;
		OAL_LOG_ERROR(
		    "Failed to initialize OAL server connection, check .int"
		    " file \n");
		goto main_exit;
	}

	lError = initDeviceData();
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to initialize OAL server\n");
		goto main_exit;
	}

	lHandle = OAL_RPCConnectionRegister(lServerConn, OAL_DriverDispatcher);
	if (lHandle == NULL) {
		OAL_LOG_ERROR("Failed to create server connection\n");
	} else {
		while (1U == 1U) {
			(void)getchar();
		}
	}

main_exit:
	return (int)lError;
}
