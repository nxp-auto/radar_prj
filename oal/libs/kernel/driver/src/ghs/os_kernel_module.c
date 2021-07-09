/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#define OAL_GHS_KERNEL_SOURCE
#include <oal_kernel_memory_allocator.h>
#include <oal_log.h>
#include <os_oal_mem_allocator.h>
#include <bsp.h>
#include <devtree.h>
#include <asp/common/asp_common_kernel_export.h>
#include <support/buildmemtable.h>
#include <support/devtree/devtree_memorymap.h>
#include <support/devtree_core.h>
#include <support/devtree_kernel_export.h>

#define DEVTREE_OAL_CHUNK_ID "id"
#define DEVTREE_OAL_CHUNK_ALIGNMENT "align"
#define DEVTREE_OAL_CHUNK_INIT "init"
#define DEVTREE_OAL_CHUNK_AUTOBALANCE "autobalance"
#define DEVTREE_OAL_MEM_REGION "memory-region"

#define OAL_REGION_PREFIX "oal_reg"

struct OAL_AllocatorDev {
	struct OAL_MemoryAllocatorRegion
	    mMemoryReservations[OAL_MAX_RESERVED_REGIONS];
	uint32_t mAllocatedRegions;
	struct IODeviceVectorStruct mIODev;
};

static struct OAL_AllocatorDev gsAllocatorDev;
extern char __ghsbegin_ramlowerlimit[];

static struct OAL_MemoryAllocatorRegion *allocateMemoryMeta(void)
{
	struct OAL_MemoryAllocatorRegion *lpRegion = NULL;

	if (gsAllocatorDev.mAllocatedRegions < OAL_MAX_RESERVED_REGIONS) {
		lpRegion =
		    &gsAllocatorDev
		         .mMemoryReservations[gsAllocatorDev.mAllocatedRegions];
		gsAllocatorDev.mAllocatedRegions++;
	}

	return lpRegion;
}

static Error initMemoryRegion(DevTree_Node aNode, uint64_t aPhysAddr,
                              uint64_t aSize,
                              struct OAL_MemoryAllocatorRegion *apMemory)
{
	DevTree_Prop lProp;
	Error lError = Success;

	/* Chunk ID */
	lError = DevTree_Node_GetValueAsU32(aNode, DEVTREE_OAL_CHUNK_ID, 0,
	                                    &lProp, &apMemory->mStartID);

	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get '%s' property (%s)\n",
		              DEVTREE_OAL_CHUNK_ID, ErrorString(lError));
		goto init_mem_region_exit;
	}

	/* Chunk alignment */
	lError = DevTree_Node_GetValueAsU32(aNode, DEVTREE_OAL_CHUNK_ALIGNMENT,
	                                    0, &lProp, &apMemory->mAlign);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get range of : %s (%s)\n",
		              DEVTREE_OAL_CHUNK_ALIGNMENT, ErrorString(lError));
		goto init_mem_region_exit;
	}

	lProp           = DevTree_Node_GetProp(aNode, DEVTREE_OAL_CHUNK_INIT);
	apMemory->mInit = ((lProp != NULL) ? 1U : 0U);

	lProp = DevTree_Node_GetProp(aNode, DEVTREE_OAL_CHUNK_AUTOBALANCE);
	apMemory->mAutobalance = ((lProp != NULL) ? 1U : 0U);
	apMemory->mPhysAddr    = aPhysAddr;
	apMemory->mSize        = aSize;

init_mem_region_exit:
	return lError;
}

static Error getRegionBoundsNode(DevTree_Node aNode, DevTree_Node *apBoundsNode)
{
	uint32_t lPHandle;
	Error lError = Success;
	DevTree_Prop lProp;

	if (apBoundsNode == NULL) {
		OAL_LOG_ERROR("Invalid argument\n");
		lError = ArgumentError;
		goto get_reg_bounds;
	}

	/* Fallback to node "regs" property */
	*apBoundsNode = aNode;

	/* Memory Region property */
	lProp = DevTree_Node_GetProp(aNode, DEVTREE_OAL_MEM_REGION);
	if (lProp != NULL) {
		lError = DevTree_Prop_GetValueAsU32(lProp, 0, &lPHandle);
		*apBoundsNode =
		    DevTree_FindNodeByPHandle(DevTree_GetTree(), lPHandle);
		if (*apBoundsNode == NULL) {
			OAL_LOG_ERROR("Broken phandle %d\n", lPHandle);
			lError = ResourceNotAvailable;
			goto get_reg_bounds;
		}
	}

get_reg_bounds:
	return lError;
}

static Error OAL_MemoryAllocatorTopoInit(DevTree_Node aNode,
                                         const char8_t *acpMatchName)
{
	Value lAttributes = MEMORY_RWE;
	DevTree_Node lBoundsNode;
	Error lError = Success;
	Address lXAddr;
	uint64_t lAddr, lPhysAddr, lSize;
	const char8_t *lcpPath;

	struct OAL_MemoryAllocatorRegion *lpMemory = allocateMemoryMeta();

	if (lpMemory == NULL) {
		OAL_LOG_ERROR(
		    "Failed to allocate a new slot for OAL_ReservedMemory "
		    "structure."
		    "Please review OAL_MAX_RESERVED_REGIONS value.\n");
		lError = MemoryAllocationFailed;
		goto topo_init_exit;
	}

	OAL_UNUSED_ARG(acpMatchName);

	/* Build the name based on device tree path */
	lcpPath = DevTree_Node_GetNodePath(aNode);
	(void)ksnprintf(lpMemory->mName, sizeof(lpMemory->mName), "%s",
	                lcpPath);

	/* memory-reserve phandle or node regs ? */
	lError = getRegionBoundsNode(aNode, &lBoundsNode);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to get '%s' node bounds.\n", lcpPath);
		goto topo_init_exit;
	}

	/* Is part of the RAM ? */
	lError = DevTree_Node_GetRegAsPhysical(lBoundsNode, 0, &lAddr, &lSize);
	if (lError != Success) {
		MemoryType lMemType = ExtendedRAM_MemoryType;
		lError = DevTree_Node_GetReg(lBoundsNode, 0, &lPhysAddr, &lSize,
		                             NULL, NULL);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to get range of : %s\n", lcpPath);
			goto topo_init_exit;
		}

		lAddr = lPhysAddr + ASP_ARM64_GetRamKernelOffset();

#if (__INTEGRITY_MAJOR_VERSION >= 11) && (__INTEGRITY_MINOR_VERSION >= 7)
		/* If Integrity OS has "reserved-memory" support */
		lMemType = Other_MemoryType;
		lError =
		    BMT_RegisterAdditionalPhysicalRange(&(const PhysicalRange){
		        .Attributes          = lAttributes,
		        .PhysicalSpaceNumber = 0,
		        .First               = lAddr,
		        .Last = lAddr + lSize - (ExtendedAddress)1,
		        .Type = lMemType});
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to register '%s' (%s)\n",
			              lpMemory->mName, ErrorString(lError));
			goto topo_init_exit;
		}
#endif

		lError = BMT_AllocateFromAnonymousMemoryReservation(
		    &(const MemoryReservation){
		        .Attributes = lAttributes,
		        .First      = lAddr,
		        .Last       = lAddr + lSize - (ExtendedAddress)1,
		        .Type       = lMemType,
		        .Fixed      = true,
		        .name       = lpMemory->mName,
		    },
		    &lXAddr);
		if (lError != Success) {
			OAL_LOG_ERROR("Failed to reserve '%s' (%s)\n",
			              lpMemory->mName, ErrorString(lError));
			goto topo_init_exit;
		}

		MemoryWindow lMw = MemorySpace_AllocateMemoryWindow();
		if (lMw == NULL) {
			OAL_LOG_ERROR("Failed to allocate a memory window\n");
			lError = MemoryAllocationFailed;
			goto topo_init_exit;
		}

		*lMw = (struct MemoryWindowStruct){
		    .MasterSpace   = &ExtendedKernelMemorySpace,
		    .MasterAddress = lAddr,
		    .TargetSpace   = &PhysicalMemorySpace,
		    .TargetAddress = lPhysAddr,
		    .Length        = lSize,
		    .Flags         = 0,
		};
		MemorySpace_RegisterMemoryWindow(lMw);
	} else {
		lPhysAddr = lAddr;
		lError    = DevTree_AddAdditionalMapping(lAddr, lSize,
		                                      MEMORY_DeviceRegisters,
		                                      lpMemory->mName, &lXAddr);
		if (lError != Success) {
			OAL_LOG_ERROR(
			    "Failed to add a new mapping: "
			    "0x%llx - 0x%llx\n",
			    lAddr, lAddr + lSize);
		}
	}

	if (lError == Success) {
		lError = initMemoryRegion(aNode, lPhysAddr, lSize, lpMemory);
		if (lError != Success) {
			OAL_LOG_ERROR(
			    "Failed to initialize meta data for: %s\n",
			    lcpPath);
		}
	}

topo_init_exit:
	return lError;
}

static Error oalAllocatorReadStatus(IODeviceVector aIoDev, Value aStatusNumber,
                                    void *apBufPtr, Address aLength)
{
	Error lError        = Success;
	Value lStatusNumber = aStatusNumber;
	OAL_UNUSED_ARG(aIoDev);

	if (lStatusNumber < OAL_FIRST_BUFFER_INDEX) {
		lError = IllegalStatusNumber;
		goto alloc_read_status_exit;
	}

	lStatusNumber -= OAL_FIRST_BUFFER_INDEX;
	if (lStatusNumber >= gsAllocatorDev.mAllocatedRegions) {
		lError = IllegalStatusNumber;
		goto alloc_read_status_exit;
	}

	if (aLength != sizeof(gsAllocatorDev.mMemoryReservations[0])) {
		lError = IllegalAlignment;
		goto alloc_read_status_exit;
	}

	{
		void *lpVDst = (void *)apBufPtr;
		void *lpVSrc =
		    (void *)&gsAllocatorDev.mMemoryReservations[lStatusNumber];
		(void)memcpy(lpVDst, lpVSrc, aLength);
	}

alloc_read_status_exit:
	return lError;
}

static Error oalAllocatorReadRegister(IODeviceVector aIoDev, Value aRegNum,
                                      Value *apRegValue)
{
	Error lError = ArgumentError;
	OAL_UNUSED_ARG(aIoDev);

	if (aRegNum == OAL_GET_NUMBER_OF_REGIONS) {
		*apRegValue = gsAllocatorDev.mAllocatedRegions;
		lError      = Success;
	}

	return lError;
}

static Error oalAllocatorCreate(IODeviceVector aIoDev)
{
	OAL_UNUSED_ARG(aIoDev);
	return Success;
}

static void OAL_MemoryAllocatorFinalize(void)
{
	Error lError;

	/* Initialize IODevice */
	(void)memset(&gsAllocatorDev.mIODev, 0, sizeof(gsAllocatorDev.mIODev));
	gsAllocatorDev.mIODev.ReadStatus   = oalAllocatorReadStatus;
	gsAllocatorDev.mIODev.ReadRegister = oalAllocatorReadRegister;
	gsAllocatorDev.mIODev.Create       = oalAllocatorCreate;

	gsAllocatorDev.mIODev.Name = OAL_GHS_DEV_NAME;

	lError =
	    RegisterIODeviceVector(&gsAllocatorDev.mIODev, OAL_GHS_DEV_NAME);
	if (lError != Success) {
		OAL_LOG_ERROR("Failed to register IODevice\n");
	}
}

DevTree_NodeDriver __ghsentry_devtree_node_driver_oal_memory_allocator = {
    .name            = "OAL Memory Reservation",
    .supported_types = (const char *[]){"fsl,oal-mem-reg", NULL},
    .init            = OAL_MemoryAllocatorTopoInit,
};

void (*__ghsentry_bspuserinit_oal_memory_allocator)(void) =
    OAL_MemoryAllocatorFinalize;

/* this line forces the compiler to not discard the above symbol */
char __ghsautoimport_OALDriver;
