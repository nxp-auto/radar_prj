/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <ghs_mr_utils.h>

static inline Error copyMemoryPermissions(MemoryRegion aSource,
                                          MemoryRegion aDestination)
{
	Value lPermissions;
	Error lError;

	lError = GetMemoryRegionAttributes(aSource, &lPermissions);
	if (lError != Success) {
		OAL_LOG_ERROR(
		    "Failed to get memory region's permissions (%s)\n",
		    ErrorString(lError));
		goto copy_permissions_exit;
	}

	lError = SetMemoryRegionAttributes(aDestination, lPermissions);
	if (lError != Success) {
		OAL_LOG_ERROR(
		    "Failed to set memory region's permissions (%s)\n",
		    ErrorString(lError));
	}

copy_permissions_exit:
	return lError;
}

Error OAL_GHS_AllocAndMapMemoryReg(uint64_t aSize, MemoryRegion *apVirtMR,
                                   MemoryRegion *apPhysMR, Value aUsage)
{
	Error lError = Success;

	lError = AllocateAnyMemoryRegion(__ghs_VirtualMemoryRegionPool, aSize,
	                                 apVirtMR);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to allocate virtual memory (%s)\n",
		              ErrorString(lError));
		goto alloc_virt_exit;
	}

	/* Set virtual memory permissions */
	lError = copyMemoryPermissions(*apPhysMR, *apVirtMR);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to copy permissions (%s)\n",
		              ErrorString(lError));
		goto deallocate_vm;
	}

	/* Virt - Phys mapping */
	if (aUsage == 0U) {
		lError = MapMemoryRegion(*apVirtMR, *apPhysMR);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to map memory (%s)\n",
			              ErrorString(lError));
		}
	} else {
		lError =
		    MapMemoryRegionAndSetUsage(*apVirtMR, *apPhysMR, aUsage);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to map memory with usage (%s)\n",
			              ErrorString(lError));
		}
	}

deallocate_vm:
	if (lError != Success) {
		(void)DeallocateMemoryRegion(__ghs_VirtualMemoryRegionPool,
		                             *apVirtMR);
	}

alloc_virt_exit:
	return lError;
}

Error OAL_GHS_UnmapAndReleaseVirtual(MemoryRegion aVirtMR)
{
	Error lError = Success;
	lError       = UnmapMemoryRegion(aVirtMR);
	if (lError == Success) {
		lError = DeallocateMemoryRegion(__ghs_VirtualMemoryRegionPool,
		                                aVirtMR);
	}

	return lError;
}
