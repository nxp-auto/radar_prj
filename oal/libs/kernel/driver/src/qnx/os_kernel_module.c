/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <oal_driver_dispatcher.h>
#include <oal_driver_functions.h>
#include <oal_page_manager.h>
#include <os_module_probe.h>

#include <qnx_dev.h>
#include <qnx_dev_resource.h>
#include <qnx_fdt_utils.h>
#include <driver_init.h>

static OAL_RPCService_t gsOALService;

int32_t OAL_InitializeDriver(void)
{
	static const char8_t *lcpsCompatibleS[] = {
	    "fsl,oal-mem-reg",
	    NULL,
	};
	int32_t lRet = 0;

	if (ThreadCtl(_NTO_TCTL_IO_PRIV, NULL) == -1) {
		OAL_LOG_ERROR("Failed to get control\n");
		lRet = errno;
		goto init_exit;
	}

	/* Init memory pools */
	lRet = OAL_InitMemoryPools(1U);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize memory pools\n");
		goto init_exit;
	}

	lRet = OAL_DevFdtMatchCall(lcpsCompatibleS, OAL_OS_ProbeDriver, NULL,
	                           (void **)NULL);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to call driver probe\n");
		goto init_exit;
	}

	gsOALService = OAL_RPCRegister(OAL_SERVICE_NAME, OAL_DriverDispatcher);
	if (gsOALService == NULL) {
		OAL_LOG_ERROR("Failed to start OAL service\n");
		lRet = -ENOMEM;
		goto init_exit;
	}

init_exit:
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to call driver probe\n");
	}

	return lRet;
}

void OAL_ReleaseDriver(void)
{
	(void)OAL_RPCCleanup(gsOALService);
	gsOALService = NULL;
}
