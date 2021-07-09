/*
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <driver_init.h>
#include <qnx_dev.h>
#include <fcntl.h>

int32_t main(const int32_t acArgc, const char *const *acpArgv)
{
	int32_t lRet;

	(void)set_lowest_fd(3);

	lRet = OAL_InitializeDriver();
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to initialize driver\n");
		goto main_exit;
	}

	OAL_UNUSED_ARG(acArgc);
	OAL_UNUSED_ARG(acpArgv);

	lRet = OAL_QnxWaitDevRelease();
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to call driver probe\n");
	}

	(void)OAL_ReleaseDriver();
main_exit:
	return lRet;
}
