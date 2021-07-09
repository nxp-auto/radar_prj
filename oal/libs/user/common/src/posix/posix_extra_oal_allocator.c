/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_comm.h>
#include <oal_common.h>
#include <oal_driver_functions.h>
#include <oal_log.h>
#include <oal_memory_allocator.h>
#include <extra_oal_allocator.h>
#include <posix_oal_memory_allocator.h>

uint32_t OAL_GetNumberOfReserverdChunks(void)
{
	uint32_t lNPools = 0;

	(void)OAL_Initialize();
	if (gOalH != NULL) {
		if (OAL_SimpleOutCall(gOalH, (uint32_t)CMD_MEMORY_GET_DEVICES,
		                      lNPools) != 0U) {
			lNPools = 0;
		}
	} else {
		OAL_LOG_ERROR(
		    "OAL: Initialization failed. Please check the OAL kernel "
		    "module.\n");
	}

	return lNPools;
}

uint64_t OAL_GetResMemBase(uint32_t aMemId)
{
	uint64_t lBaseAddr = aMemId;
	(void)OAL_Initialize();
	if (gOalH != NULL) {
		if (OAL_SimpleInOutCall(gOalH, (uint32_t)CMD_MEMORY_GET_BASE,
		                        lBaseAddr) != 0U) {
			lBaseAddr = 0;
		}
	} else {
		OAL_LOG_ERROR(
		    "OAL: Initialization failed. Please check the OAL kernel "
		    "module.\n");
	}

	return lBaseAddr;
}

uint64_t OAL_GetResMemSize(uint32_t aMemId)
{
	uint64_t lSize = aMemId;
	(void)OAL_Initialize();
	if (gOalH != NULL) {
		if (OAL_SimpleInOutCall(gOalH, (uint32_t)CMD_MEMORY_GET_SIZE,
		                        lSize) != 0U) {
			lSize = 0;
		}
	} else {
		OAL_LOG_ERROR(
		    "OAL: Initialization failed. Please check the OAL kernel "
		    "module.\n");
	}

	return lSize;
}

uint32_t OAL_IsResMemAutobalanced(uint32_t aMemId)
{
	uint32_t lIsBalanced = aMemId;
	(void)OAL_Initialize();
	if (gOalH != NULL) {
		if (OAL_SimpleInOutCall(gOalH,
		                        (uint32_t)CMD_MEMORY_GET_AUTOBALANCE,
		                        lIsBalanced) != 0U) {
			lIsBalanced = 0;
		}
	} else {
		OAL_LOG_ERROR(
		    "OAL: Initialization failed. Please check the OAL kernel "
		    "module.\n");
	}

	return lIsBalanced;
}
