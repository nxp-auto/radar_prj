/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_devtree_utils.h>
#include <oal_page_manager.h>
#include <os_module_probe.h>

#ifndef OAL_DONT_USE_FDT
int32_t OAL_OS_InitKernelAllocator(void)
{
	int32_t lRet                            = 0;
	static const char8_t *lcpsCompatibleS[] = {
	    "fsl,oal-mem-reg", NULL,
	};

	/* Init memory pools */
	lRet = OAL_InitMemoryPools(0U);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize memory pools\n");
		goto init_kernel_exit;
	}

	lRet = OAL_FdtMatchCall(lcpsCompatibleS, OAL_OS_ProbeDriver, (void **)NULL);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to call driver probe\n");
		goto init_kernel_exit;
	}

init_kernel_exit:
	return lRet;
}
#else
int32_t OAL_OS_InitKernelAllocator(void)
{
	OAL_NOT_IMPLEMENTED();
	return -1;
}
#endif
