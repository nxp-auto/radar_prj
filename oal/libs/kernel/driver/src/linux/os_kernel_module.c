/*
 * Copyright 2017-2019 NXP
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

#ifndef OAL_LOG_SUPPRESS_DEBUG
#define OAL_LOG_SUPPRESS_DEBUG
#endif
#include <oal_comm_kernel.h>
#include <oal_driver_dispatcher.h>
#include <oal_kernel_memory_allocator.h>
#include <oal_log.h>
#include <oal_page_manager.h>
#include <oal_timespec.h>
#include <os_oal_comm_kernel.h>
#include <driver_init.h>
#include <linux/linux_device.h>

#include <oal_mem_constants.h>

#define OAL_CACHED_DEV "oal_cached"
#define OAL_NON_CACHED_DEV "oal_noncached"

static OAL_DevFile_t gsNonCachedDevFile;
static OAL_RPCService_t gsOALService;
static int32_t gsOalInitErrror;

int32_t static isPartOfResMem(struct vm_area_struct *apVma)
{
	int32_t lRet;
	uint64_t lPhysAddr = apVma->vm_pgoff * OAL_PAGE_SIZE;
	uint64_t lSize     = apVma->vm_end - apVma->vm_start;
	struct OALMemoryChunk *lpMemChunk;

	lRet = OAL_GetMemoryChunkBasedOnAddr(lPhysAddr, lSize, &lpMemChunk);
	if (lRet != 0) {
		OAL_LOG_ERROR("Cannot get the memory chunk of 0x%" PRIx64 "\n",
		              lPhysAddr);
	}

	return lRet;
}

static int32_t oal_mem_mmap_cached(struct file *apFilp,
                                   struct vm_area_struct *apVma)
{
	int32_t lRet = 0;

	OAL_UNUSED_ARG(apFilp);

	if (isPartOfResMem(apVma) != 0) {
		lRet = -EINVAL;
		goto oal_map_cahced;
	}

	apVma->vm_flags |= (VM_IO | VM_DONTCOPY | VM_DONTEXPAND | VM_DONTDUMP);
	if (remap_pfn_range(apVma, apVma->vm_start, apVma->vm_pgoff,
	                    apVma->vm_end - apVma->vm_start,
	                    apVma->vm_page_prot) != 0) {
		lRet = -EAGAIN;
	}

oal_map_cahced:
	return lRet;
}

static int32_t oal_mem_mmap_noncached(struct file *apFilp,
                                      struct vm_area_struct *apVma)
{
	int32_t lRet = 0;

	OAL_UNUSED_ARG(apFilp);

	if (isPartOfResMem(apVma) != 0) {
		lRet = -EINVAL;
		goto oal_map_noncahced;
	}

	/* Non-cached mapping */
	apVma->vm_flags |= (VM_IO | VM_DONTCOPY | VM_DONTEXPAND | VM_DONTDUMP);
	apVma->vm_page_prot = pgprot_writecombine(apVma->vm_page_prot);

	if (remap_pfn_range(apVma, apVma->vm_start, apVma->vm_pgoff,
	                    apVma->vm_end - apVma->vm_start,
	                    apVma->vm_page_prot) != 0) {
		lRet = -EAGAIN;
	}

oal_map_noncahced:
	return lRet;
}

static int32_t getReservedRegion(struct platform_device *apDev,
                                 uintptr_t *apStart, uint64_t *apLen,
                                 uint8_t aInit)
{
	int32_t lRet = 0;
	struct device_node *lpMemNode;
	void __iomem *lpVirtAddr;
	uint64_t lRes[2];

	lpMemNode = of_parse_phandle(apDev->dev.of_node, "memory-region", 0);
	if (lpMemNode == NULL) {
		lRet = -EINVAL;
		goto get_res_reg_exit;
	}

	lRet = of_property_read_u64_array(lpMemNode, "reg", lRes,
	                                  OAL_ARRAY_SIZE(lRes));
	if (lRet != 0) {
		goto release_node;
	}

	*apStart = lRes[0];
	*apLen   = lRes[1];

	if ((*apStart == 0U) || (*apLen == 0U)) {
		OAL_LOG_ERROR("Invalid region: 0x%" PRIxPTR " - 0x%"
			      PRIx64 "\n", *apStart, *apLen);
		lRet = -EINVAL;
		goto release_node;
	}

	if (aInit != 0U) {
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

static int32_t OAL_OS_ProbeDriver(struct platform_device *apDev)
{
	int32_t lRet = 0;

	struct OAL_MemoryAllocatorRegion lResMem;
	struct OALMemoryChunk *lpMemChunk = NULL;
	uint64_t lSize = 0ULL;

	(void)strncpy(lResMem.mName, apDev->name,
	              OAL_ARRAY_SIZE(lResMem.mName) - 1U);
	/* String termination */
	lResMem.mName[OAL_ARRAY_SIZE(lResMem.mName) -1U] = '\0';

	/* Get init */
	if (of_get_property(apDev->dev.of_node, "init", NULL) == NULL) {
		lResMem.mInit = 0U;
	} else {
		lResMem.mInit = 1U;
	}

	lResMem.mPhysAddr = 0ULL;

	lRet =
	    getReservedRegion(apDev, &lResMem.mPhysAddr, &lSize, lResMem.mInit);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "OAL Memory range specification cannot be read."
		    "Please review reg and memory-region "
		    "attributes.\n");
		goto probe_exit;
	}
	lResMem.mSize = (uintptr_t)lSize;

	/* Get ID */
	lRet =
	    of_property_read_u32(apDev->dev.of_node, "id", &lResMem.mStartID);
	if ((lRet != 0) || (lResMem.mStartID > OAL_MAX_RESERVED_ID)) {
		OAL_LOG_ERROR(
		    "OAL ID cannot be read or isn't in range 0-7."
		    "Please review id attribute.\n");
		goto probe_exit;
	}

	/* Get alignment */
	lRet =
	    of_property_read_u32(apDev->dev.of_node, "align", &lResMem.mAlign);
	if (lRet != 0) {
		OAL_LOG_ERROR(
		    "OAL ID cannot be read or isn't in range 0-7."
		    "Please review id attribute.\n");
		goto probe_exit;
	}

	/* Get if the memory is autobalanced */
	if (of_get_property(apDev->dev.of_node, "autobalance", NULL) == NULL) {
		lResMem.mAutobalance = 0U;
	} else {
		lResMem.mAutobalance = 1U;
	}

	lRet = OAL_AddMemoryPool(&lResMem, &lpMemChunk);
	if ((lRet != 0) || (lpMemChunk == NULL)) {
		lRet = -1;
		OAL_LOG_ERROR("Failed to add '%s' to memory pools\n",
		              lResMem.mName);
		goto probe_exit;
	}

	lpMemChunk->mOSChunk.mStartPhysAddress = lResMem.mPhysAddr;

	if (lResMem.mInit != 0U) {
		uint8_t *lpVirt;
		OAL_PRINT("Initializing '%s'\n", lResMem.mName);
		lpVirt = (uint8_t *)ioremap(lResMem.mPhysAddr, lResMem.mSize);
		if (lpVirt != NULL) {
			uint64_t lIdx;
			for (lIdx = 0; lIdx < lResMem.mSize; lIdx += 8U) {
				writeq(0x0ULL, lpVirt + lIdx);
			}
			iounmap(lpVirt);
		} else {
			OAL_LOG_ERROR("Failed to initialize '%s'\n",
			              lResMem.mName);
			lRet = -EIO;
		}
	}

probe_exit:
	if (lRet != 0) {
		gsOalInitErrror = lRet;
	}
	return lRet;
}

static const struct of_device_id gcsOALDtIds[] = {
    {
        .compatible = "fsl,oal-mem-reg",
    },
    {}};

static struct platform_driver gsOALDriver = {
    .driver = {.name           = "OS Abstraction Layer",
               .owner          = THIS_MODULE,
               .of_match_table = gcsOALDtIds},
    .probe = OAL_OS_ProbeDriver,
};

int32_t OAL_InitializeDriver(void)
{
	int32_t lRet = 0;
	struct file_operations lFops;
	static struct file_operations lsOalFopsNc = {
	    .owner = THIS_MODULE, .mmap = oal_mem_mmap_noncached,
	};

	gsOALService = OAL_RPCRegister(OAL_CACHED_DEV, OAL_DriverDispatcher);
	if (gsOALService == NULL) {
		OAL_LOG_ERROR("Failed to start RPC service\n");
		lRet = -EIO;
		goto init_exit;
	}

	/* Overwrite mmap and release operations */
	(void)memset(&lFops, 0, sizeof(lFops));
	lFops.mmap = oal_mem_mmap_cached;

	lRet = OAL_SetCustomFileOperations(gsOALService, &lFops);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to register custom file operations\n");
		goto stop_service;
	}

	lRet = OAL_InitDevFile(&gsNonCachedDevFile, &lsOalFopsNc,
	                       OAL_NON_CACHED_DEV);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to init " OAL_NON_CACHED_DEV " device\n");
		goto destroy_non_cached;
	}

	/* Init memory pools */
	lRet = OAL_InitMemoryPools(1U);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize memory pools\n");
		goto destroy_non_cached;
	}

	lRet = platform_driver_register(&gsOALDriver);
	if (OAL_GetNumberOfPools() == 0U) {
		OAL_LOG_ERROR(
		    "Failed to detect OAL nodes from device tree."
		    "Please review the setup.\n");
		lRet = -EINVAL;
		goto unregister_driver;
	}

	if (gsOalInitErrror != 0) {
		OAL_LOG_ERROR("Failed to probe one of the nodes\n");
		lRet = gsOalInitErrror;
		goto unregister_driver;
	}

	goto init_exit;

unregister_driver:
	(void)platform_driver_unregister(&gsOALDriver);
destroy_non_cached:
	(void)OAL_DestroyDevFile(&gsNonCachedDevFile);
stop_service:
	(void)OAL_RPCCleanup(gsOALService);
init_exit:
	return lRet;
}

void OAL_ReleaseDriver(void)
{
	(void)OAL_InitMemoryPools(1U);
	(void)OAL_DestroyDevFile(&gsNonCachedDevFile);
	(void)OAL_RPCCleanup(gsOALService);

	platform_driver_unregister(&gsOALDriver);
}

