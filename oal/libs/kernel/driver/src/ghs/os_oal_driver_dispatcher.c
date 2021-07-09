/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_LOG_SUPPRESS_DEBUG
#define OAL_LOG_SUPPRESS_DEBUG
#endif

#include <oal_driver_dispatcher.h>
#include <oal_driver_functions.h>
#include <oal_log.h>
#include <os_oal_server.h>
#include <ghs_kernel_comm_conn.h>

uint32_t OAL_OS_DriverDispatcher(oal_dispatcher_t *apDisp, uint32_t aFuncId,
                               uintptr_t aInData, int32_t aDataLen)
{
	uint32_t lRetVal = 0;
	int32_t lStatus;
	Error lError;
	OAL_RPCService_t lService;

	OAL_UNUSED_ARG(aDataLen);
	switch (aFuncId) {
		case CMD_ALLOC: {
			CMD_ALLOC_TYPE *lpDrvCmd = (CMD_ALLOC_TYPE *)aInData;
			MemoryRegion lMemRegion  = NULLMemoryRegion;

			lService = OAL_RPCGetService(apDisp);
			if (lService == NULL) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR("Invalid service\n");
				break;
			}

			lError = OAL_GHS_AllocateMemory(lpDrvCmd, &lMemRegion);
			if (lError != Success) {
				lMemRegion = NULLMemoryRegion;
				lRetVal    = EINVAL;
			}

			OAL_SetServiceServerObject(lService,
			                           (Address)lMemRegion);
			lStatus = OAL_RPCAppendReply(apDisp, (void *)lpDrvCmd,
			                             sizeof(*lpDrvCmd));
			if (lStatus != 0) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR(
				    "Failed to send reply to memory "
				    "allocation\n");
				break;
			}

			break;
		}
		case CMD_FREE: {
			MemoryRegion lPhysRegion;

			lService = OAL_RPCGetService(apDisp);
			if (lService == NULL) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR("Invalid service\n");
				break;
			}

			lPhysRegion =
			    (MemoryRegion)OAL_GetServiceClientObject(lService);
			if (lPhysRegion == NULLMemoryRegion) {
				lRetVal = EINVAL;
				OAL_LOG_ERROR(
				    "Received an invalid memory region\n");
				break;
			}

			lError = OAL_GHS_FreeMemory(lPhysRegion);
			if (lError != Success) {
				lRetVal = EINVAL;
			}

			break;
		}
		default: {
			OAL_LOG_ERROR("Invalid function ID: %d\n", aFuncId);
			lRetVal = EINVAL;
			break;
		}
	}

	return lRetVal;
}
