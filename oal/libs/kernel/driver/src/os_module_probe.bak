/*
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_driver_functions.h>
#include <oal_kernel_memory_allocator.h>
#include <oal_memmap.h>
#include <oal_page_manager.h>

#include <oal_log.h>
#include <oal_add_reserved_mem.h>
#include <oal_comm_kernel.h>
#include <os_module_probe.h>

#ifndef OAL_DONT_USE_FDT
static void copyNodeName(struct fdt_node *apMemNode,
                         struct OAL_MemoryAllocatorRegion *apMemReg)
{
	uint8_t lIdx;

	/* FDT nodes are suffixed with @<address>. */
	for (lIdx = 0; (lIdx < OAL_MAX_LEN_RESERVED_REG) &&
	               (apMemNode->mpName[lIdx] != '@');
	     lIdx++) {
		apMemReg->mName[lIdx] = apMemNode->mpName[lIdx];
	}

	if (lIdx < OAL_MAX_LEN_RESERVED_REG) {
		apMemReg->mName[lIdx] = '\0';
	}
}

static int32_t getReservedRegion(const struct fdt_node *acpNode,
                                 struct OAL_MemoryAllocatorRegion *apMemReg)
{
	int32_t lRet = 0;
	struct fdt_node lMemNode;
	uintptr_t lVirtAddr;
	uintptr_t lStartAddr;

	lRet = OAL_FdtParsePhandle(acpNode, &lMemNode, "memory-region");
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to identify 'memory-region' for %s\n",
		              acpNode->mpName);
		goto get_res_reg_exit;
	}

	lRet = OAL_FdtGetReg(&lMemNode, 0, &lStartAddr, &apMemReg->mSize);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to identify 'reg' property of %s node.\n",
		              lMemNode.mpName);
		goto get_res_reg_exit;
	}

	apMemReg->mPhysAddr = lStartAddr;
	copyNodeName(&lMemNode, apMemReg);

	lRet = OAL_OS_ResolveMemRegName(apMemReg);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to resolve memory region's name\n");
		goto get_res_reg_exit;
	}

	if (apMemReg->mInit != 0U) {
		volatile uint64_t *lpAddr;
		uint64_t lIdx;
		uint64_t lLen;

		lVirtAddr = OAL_kmemmap(apMemReg->mPhysAddr, apMemReg->mSize);
		if (lVirtAddr == 0U) {
			lRet = -ENOMEM;
			OAL_LOG_ERROR("OAL memory init failed!\n");
			goto get_res_reg_exit;
		}

		/* SRAM allows 64 bit access only */
		lpAddr = (volatile uint64_t *)lVirtAddr;
		lLen   = ((uint64_t)apMemReg->mSize) / sizeof(uint64_t);
		for (lIdx = 0U; lIdx < lLen; lIdx++) {
			lpAddr[lIdx] = 0UL;
		}

		lRet = OAL_kmemunmap(lVirtAddr, apMemReg->mSize);
	}

get_res_reg_exit:
	return lRet;
}

int32_t OAL_OS_ProbeDriver(const struct fdt_node *acpNode, const void **acpData)
{
	int32_t lRet = 0;
	uint8_t *lpPropValue;
	struct OAL_MemoryAllocatorRegion lResMem;

	OAL_UNUSED_ARG(acpData);

	/* Get init */
	if (OAL_FdtGetNodeProperty(acpNode, "init", NULL) != 0) {
		lResMem.mInit = 0U;
	} else {
		lResMem.mInit = 1U;
	}

	/* Get ID */
	lRet = OAL_FdtGetNodeProperty(
	    acpNode, "id", (const char8_t **)(uintptr_t)&lpPropValue);
	if (lRet != 0) {
		OAL_LOG_ERROR("OAL ID cannot be read\n");
		goto probe_exit;
	}
	OAL_PROP_GET_NEXT_UINT32(lResMem.mStartID, lpPropValue);

	if (lResMem.mStartID > OAL_MAX_RESERVED_ID) {
		OAL_LOG_ERROR(
		    "OAL ID isn't in range 0-7."
		    "Please review id attribute.\n");
		goto probe_exit;
	}

	lRet = getReservedRegion(acpNode, &lResMem);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "OAL Memory range specification cannot be read."
		    "Please review reg and memory-region attributes.\n");
		goto probe_exit;
	}

	// Get alignment
	lRet = OAL_FdtGetNodeProperty(
	    acpNode, "align", (const char8_t **)(uintptr_t)&lpPropValue);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "OAL alignment cannot be read."
		    "Please review 'align' attribute.\n");
		goto probe_exit;
	}

	OAL_PROP_GET_NEXT_UINT32(lResMem.mAlign, lpPropValue);
	if (lResMem.mAlign == 0U) {
		OAL_LOG_ERROR("Invalid alignment for node: %s\n",
		              acpNode->mpName);
		goto probe_exit;
	}

	// Get if the memory is autobalanced
	if (OAL_FdtGetNodeProperty(acpNode, "autobalance", NULL) != 0) {
		lResMem.mAutobalance = 0U;
	} else {
		lResMem.mAutobalance = 1U;
	}

	lRet = OAL_AddReservedMemoryRegion(
	    lResMem.mName, lResMem.mPhysAddr, lResMem.mSize, lResMem.mStartID,
	    lResMem.mAlign, lResMem.mInit, lResMem.mAutobalance);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to add '%s' region to memory pools\n",
		              lResMem.mName);
	}

probe_exit:
	return lRet;
}
#else
int32_t OAL_OS_ProbeDriver(const struct fdt_node *acpNode, const void **acpData)
{
	OAL_UNUSED_ARG(acpNode);
	OAL_UNUSED_ARG(acpData);
	OAL_NOT_IMPLEMENTED();
	return -1;
}
#endif
