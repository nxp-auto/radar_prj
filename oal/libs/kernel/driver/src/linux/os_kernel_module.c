/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/tlbflush.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/mman.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/slab.h>

#define OAL_LOG_SUPPRESS_DEBUG
#include "oal_allocation_kernel.h"
#include "oal_cma_list.h"
#include "oal_comm_kernel.h"
#include "oal_debug_out.h"
#include "oal_driver_dispatcher.h"
#include "oal_log.h"
#include "oal_timespec.h"
#include "linux/linux_device.h"
#include "os_oal.h"
#include "os_oal_comm_kernel.h"

#define OAL_CACHED_DEV     "oal_cached"
#define OAL_NON_CACHED_DEV "oal_noncached"

///////////////////////////////////////////////////////////////////////////////////
// Module devices bookkeeping
///////////////////////////////////////////////////////////////////////////////////

static OAL_DevFile_t gNonCachedDevFile;
static OAL_RPCService_t gOALService;

////////////////////////////////////////////////////////////////////////////////
// Memmap function
////////////////////////////////////////////////////////////////////////////////
static int32_t oal_mem_mmap_cached(struct file *filp, struct vm_area_struct *vma)
{
	int32_t retval = 0;

	vma->vm_page_prot = set_pte_bit(vma->vm_page_prot, __pgprot(PTE_DIRTY));

	if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot))
	{
		retval = -EAGAIN;
	}

	return retval;
}

static int32_t oal_mem_mmap_noncached(struct file *filp, struct vm_area_struct *vma)
{
	int32_t retval = 0;

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	vma->vm_page_prot = set_pte_bit(vma->vm_page_prot, __pgprot(PTE_DIRTY));

	if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start, vma->vm_page_prot))
	{
		retval = -EAGAIN;
	}

	return retval;
}

static int nxp_oal_release(struct inode *inodep, struct file *filep)
{
	cma_list_free_pid((uint32_t)task_pgrp_nr(current));

	return 0;
}

static int getReservedRegion(struct platform_device *apDev,
		uint64_t *apStart, uint64_t *apLen, bool aInit)
{
	int lRet = 0;
	struct device_node *lpMemNode;
	void __iomem *lpVirtAddr;
	uint64_t lRes[2];

	if ((apDev == NULL) || (apStart == NULL) || (apLen == NULL)) {
		lRet = -EINVAL;
		goto get_res_reg_exit;
	}

	lpMemNode = of_parse_phandle(apDev->dev.of_node, "memory-region", 0);
	if (lpMemNode == NULL) {
		lRet = -EINVAL;
		goto get_res_reg_exit;
	}

	lRet = of_property_read_u64_array(lpMemNode, "reg", lRes,
			ARRAY_SIZE(lRes));
	if (lRet != 0) {
		goto release_node;
	}

	*apStart = lRes[0];
	*apLen = lRes[1];

	if (aInit) {
		lpVirtAddr = of_iomap(lpMemNode, 0);
		if (lpVirtAddr == NULL) {
			lRet = -ENOMEM;
			OAL_LOG_ERROR("OAL memory init failed!\n");
			goto release_node;
		}

		memset_io(lpVirtAddr, 0x0, *apLen);
		iounmap(lpVirtAddr);
		OAL_LOG_DEBUG("  Initialized to 0x0ULL\n");
	}

release_node:
	of_node_put(lpMemNode);

get_res_reg_exit:
	return lRet;
}

static int probeOAL(struct platform_device *apDev)
{
	int lRet = 0;

	uint64_t lResMemStart;
	uint64_t lResMemLen;
	uint32_t lStartID;
	int32_t lAlignment;
	bool lInitReg;

	// Get init
	if (of_get_property(apDev->dev.of_node, "init", NULL) == NULL) {
		lInitReg = false;
	} else {
		lInitReg = true;
	}

	lRet = getReservedRegion(apDev, &lResMemStart, &lResMemLen, lInitReg);
	if (lRet) {
		OAL_LOG_ERROR("OAL Memory range specification cannot be read."
				"Please review reg and memory-region attributes.\n");
		goto probe_exit;
	}

	// Get ID
	lRet = of_property_read_u32(apDev->dev.of_node, "id", &lStartID);
	if ((lRet != 0) || (lStartID < OAL_MIN_ID) || (lStartID > OAL_MAX_ID)) {
		OAL_LOG_ERROR("OAL ID cannot be read or isn't in range 0-7."
				"Please review id attribute.\n");
		goto probe_exit;
	}

	gLoadedDevices |= (1 << lStartID);

	// Get alignment
	lRet = of_property_read_u32(apDev->dev.of_node, "align", &lAlignment);
	if ((lRet != 0) || (lAlignment <= 0)) {
		OAL_LOG_ERROR("OAL ID cannot be read or isn't in range 0-7."
				"Please review id attribute.\n");
		goto probe_exit;
	}

	gDeviceAlignment[lStartID] = lAlignment;

	// Get if the memory is autobalanced
	if (of_get_property(apDev->dev.of_node, "autobalance", NULL) == NULL) {
		gAutobalancedDevices &= ~(1 << lStartID);
	} else {
		gAutobalancedDevices |= (1 << lStartID);
	}

	apex_allocator_mmap_init(lStartID, lResMemStart, lResMemLen);

	printk("OAL region successfully mapped %llX@%llX, Alignment: 0x%X\n",
			lResMemLen, lResMemStart,
			gDeviceAlignment[lStartID]);

	cma_list_init(lStartID, lResMemStart, lResMemStart + lResMemLen,
			gDeviceAlignment[lStartID]);

	printk("  CMA: ID %02X. Start %02llX. "
			"End %02llX. Length %02llX. Num %02llX.\n",
			lStartID, lResMemStart, (lResMemStart + lResMemLen),
			lResMemLen, (lResMemLen / gDeviceAlignment[lStartID]));
probe_exit:
	return lRet;
}

static struct file_operations oal_fops_nc = {
	.owner =    THIS_MODULE,
	.mmap=      oal_mem_mmap_noncached,
};


static const struct of_device_id gOALDtIds[] = {
	{
		.compatible = "fsl,oal-mem-reg",
	},
	{}
};


static struct platform_driver gOALDriver = {
	.driver = {
		.name = "OS Abstraction Layer",
		.owner = THIS_MODULE,
		.of_match_table = gOALDtIds
	},
	.probe = probeOAL,
};

static int initOALDriver(void)
{
	int lRet = 0;
	struct file_operations lFops;

	gOALService = OAL_RPCRegister(OAL_CACHED_DEV, oalDriverDispatcher);
	if (gOALService == NULL) {
		OAL_LOG_ERROR("Faield to start RPC service\n");
		goto init_exit;
	}

	// Overwrite mmap and release operations
	memset(&lFops, 0, sizeof(lFops));
	lFops.mmap = oal_mem_mmap_cached;
	lFops.release = nxp_oal_release;

	lRet = OAL_SetCustomFileOperations(gOALService, &lFops);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to register custom file operations\n");
		goto stop_service;
	}

	lRet = OAL_initDevFile(&gNonCachedDevFile, &oal_fops_nc, OAL_NON_CACHED_DEV);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to init " OAL_NON_CACHED_DEV
				" device\n");
		goto stop_service;
	}

	lRet = platform_driver_register(&gOALDriver);
	if (gLoadedDevices == 0) {
		OAL_LOG_ERROR("Failed to detect OAL nodes from device tree."
				"Please review the setup.");
	}
	goto init_exit;

stop_service:
	(void) OAL_RPCCleanup(gOALService);
init_exit:
	return lRet;
}
module_init(initOALDriver);

static void releaseOALDriver(void)
{
	int i = 0;

	cma_list_free_all();
	cma_list_deinit();

	for (i = 0; i < OAL_MAX_ALLOCATOR_NUM; ++i)
	{
		apex_allocator_mmap_deinit(i);
	}


	(void) OAL_RPCCleanup(gOALService);
	(void) OAL_destroyDevFile(&gNonCachedDevFile);

	platform_driver_unregister(&gOALDriver);
}
module_exit(releaseOALDriver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OAL kernel module for contiguous "
		"memory allocations and its management.");
MODULE_ALIAS("OAL");
