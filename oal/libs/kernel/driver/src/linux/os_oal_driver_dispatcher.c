/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <linux/cache.h>
#include <linux/mman.h>
#include <linux/module.h>

#ifndef OAL_LOG_SUPPRESS_DEBUG
#define OAL_LOG_SUPPRESS_DEBUG
#endif

#include "oal_driver_dispatcher.h"
#include "oal_driver_functions.h"
#include "oal_log.h"
#include "os_oal_comm_kernel.h"
#include <posix/posix_oal_driver_dispatcher.h>

// Cache control functions defined in ASM
extern void flush_dcache_range(uintptr_t aMemory, uint32_t aSize);
extern void invalidate_dcache_range(uintptr_t aMemory, uint32_t aSize);
extern void flush_and_invalidate_dcache_range(uintptr_t aMemory,
                                              uint32_t aSize);

uint32_t OAL_OS_DriverDispatcher(oal_dispatcher_t *apDisp, uint32_t aFuncId,
                               uintptr_t aInData, int32_t aDataLen)
{
	uint32_t lRetVal = 0;

	switch (aFuncId) {
		///////////////////////////////////////////////////////////////////////////
		// Flush & Invalidate
		case CMD_FLUSHINVALIDATE_SPECIFIC: {
			CMD_FLUSH_SPECIFIC_TYPE *lpDrvCmd =
			    (CMD_FLUSH_SPECIFIC_TYPE *)aInData;

			OAL_LOG_DEBUG("OAL: FLUSH & INVALIDATE ADDRESS\n");

			flush_and_invalidate_dcache_range(
			    lpDrvCmd->mVirtualPointer,
			    (uint32_t)lpDrvCmd->mSize);
			break;
		}

		case CMD_INVALIDATE_SPECIFIC: {
			CMD_FLUSH_SPECIFIC_TYPE *lpDrvCmd =
			    (CMD_FLUSH_SPECIFIC_TYPE *)aInData;

			OAL_LOG_DEBUG("OAL: INVALIDATE ADDRESS\n");

			invalidate_dcache_range(lpDrvCmd->mVirtualPointer,
			                        (uint32_t)lpDrvCmd->mSize);

			break;
		}

		///////////////////////////////////////////////////////////////////////////
		// Flush
		case CMD_FLUSH_SPECIFIC: {
			CMD_FLUSH_SPECIFIC_TYPE *lpDrvCmd =
			    (CMD_FLUSH_SPECIFIC_TYPE *)aInData;

			OAL_LOG_DEBUG("OAL: FLUSH ADDRESS\n");

			flush_dcache_range(lpDrvCmd->mVirtualPointer,
			                   (uint32_t)lpDrvCmd->mSize);

			break;
		}

		///////////////////////////////////////////////////////////////////////////
		// Cache line
		case CMD_GET_CACHE_LINE: {
			uint32_t lCacheLine;
			OAL_LOG_DEBUG("OAL: Get Cache Line\n");
			lCacheLine = (uint32_t)cache_line_size();

			(void)OAL_RPCAppendReply(apDisp, (void *)&lCacheLine,
			                         sizeof(lCacheLine));
			break;
		}

		///////////////////////////////////////////////////////////////////////////
		// Page size
		case CMD_GET_PAGE_SIZE: {
			uint32_t lPageSize;
			OAL_LOG_DEBUG("OAL: Get Cache Line\n");

			lPageSize = OAL_PAGE_SIZE;

			(void)OAL_RPCAppendReply(apDisp, (void *)&lPageSize,
			                         sizeof(lPageSize));
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
