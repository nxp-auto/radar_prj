/*
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_utils.h>
#include <sys/asinfo.h>
#include <oal_devtree_utils.h>
#include <oal_memmap.h>
#include <qnx_fdt_utils.h>
#include <qnx_posix_typed_mem_utils.h>

#define FDT_NODE "fdt"

struct _fdt_matcher_data {
	void *mpFdtAddr;
	size_t mSize;
};

static int32_t fdt_matcher(struct asinfo_entry *apInfo, char8_t *apName,
                           void *apData)
{
	struct _fdt_matcher_data *lpFdata = (struct _fdt_matcher_data *)apData;
	char8_t lPath[OAL_MAX_POSIX_TYPED_MEM_LEN];
	int32_t lRet;

	void* lpVadr;

	OAL_UNUSED_ARG(apName);

	(void) memset(lPath, 0, sizeof(lPath));
	lRet = OAL_QNX_GetPathFromAsInfo(apInfo, lPath, sizeof(lPath));
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get path from as info\n");
		goto error;
	}

	lpFdata->mSize = apInfo->end - apInfo->start;

	lpVadr = mmap_device_memory(NULL, lpFdata->mSize, PROT_READ,
	    MAP_PHYS | MAP_SHARED, apInfo->start);

	if (((void *)MAP_FAILED) == lpVadr) {
		OAL_LOG_ERROR("Failed to map fdt\n");
		lRet = -EIO;
		goto error;
	}

	lpFdata->mpFdtAddr = lpVadr;

error:
	return lRet;
}



static struct _fdt_matcher_data gsOALFDTData;
int32_t __attribute__((weak)) OAL_GetFdtAddress(uintptr_t *apFdtAddr)
{
	int32_t lRet = 0;
	if (gsOALFDTData.mpFdtAddr == NULL) {
		lRet = walk_asinfo(FDT_NODE, fdt_matcher, &gsOALFDTData);
	}

	// Cached value
	*apFdtAddr = (uintptr_t)gsOALFDTData.mpFdtAddr;
	return lRet;
}

int32_t OAL_ReleaseFdt(void)
{
	int32_t lRet = 0;
	(void)munmap_device_memory(gsOALFDTData.mpFdtAddr, gsOALFDTData.mSize);
	(void)memset(&gsOALFDTData, 0, sizeof(gsOALFDTData));

	return lRet;
}


