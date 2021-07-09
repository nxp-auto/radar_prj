/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_driver_dispatcher.h>
#include <oal_driver_functions.h>
#include <oal_log.h>
#include <oal_page_manager.h>
#include <posix/posix_oal_driver_dispatcher.h>

uint32_t OAL_OS_DriverDispatcher(oal_dispatcher_t *apDisp, uint32_t aFuncId,
                               uintptr_t aInData, int32_t aDataLen)
{
	uint32_t lRetVal = 0;
	OAL_UNUSED_ARG(aDataLen);
	OAL_UNUSED_ARG(apDisp);
	OAL_UNUSED_ARG(aInData);

	switch (aFuncId) {
		case CMD_GET_CHUNKS: {
			uint32_t lIdx;
			uint32_t lNPools               = OAL_GetNumberOfPools();
			struct OALMemoryChunk *lpPools = OAL_GetMemoryPools();

			OAL_LOG_DEBUG("Get chunks info\n");

			for (lIdx = 0; lIdx < lNPools; lIdx++) {
				(void)OAL_RPCAppendReply(
				    apDisp,
				    (void *)&lpPools[lIdx].mMemoryRegion,
				    sizeof(lpPools[lIdx].mMemoryRegion));
			}

			break;
		}
		default: {
			lRetVal = OAL_PosixOalDriverDispatcher(apDisp, aFuncId,
			                                   aInData, aDataLen);
			break;
		}
	}

	return lRetVal;
}
