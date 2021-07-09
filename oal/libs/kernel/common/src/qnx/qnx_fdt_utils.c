/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_log.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <oal_devtree_utils.h>
#include <oal_memmap.h>
#include <qnx_fdt_utils.h>
#include <qnx_posix_typed_mem_utils.h>

int32_t OAL_FdtGetIrq(const struct fdt_node *acpNode, int32_t aIndex,
                    int32_t *apIrqNum)
{
	uint32_t lPropLen = 0U;
	size_t lMinLen    = sizeof(uint32_t) * ((size_t)aIndex + 1ULL) * 3ULL;
	uint8_t *lpFdata  = NULL;
	uintptr_t lFdtAddr;
	int32_t lRet = 0;
	uint32_t lIrq;

	if ((acpNode == NULL) || (apIrqNum == NULL)) {
		lRet = -EINVAL;
		goto fdt_get_irq_exit;
	}

	lRet = OAL_GetFdtAddress(&lFdtAddr);
	if (lRet != 0) {
		lRet = lRet;
		goto fdt_get_irq_exit;
	}

	lRet = OAL_GetProp((const uint32_t *)lFdtAddr, acpNode->mOffset,
	                   OAL_IRQ_PROPERTY, &lPropLen, &lpFdata);
	if ((lpFdata == NULL) || ((size_t)lPropLen < lMinLen)) {
		lRet = -ENODEV;
		goto fdt_get_irq_exit;
	}

	// Skip an interrupt cell from DTS. Each interrupt cell is
	// defined by 3 integers.
	OAL_PROP_SKIP_N_VALUES(lpFdata, ((size_t)aIndex * 3ULL) + 1ULL, uint32_t);
	OAL_PROP_GET_NEXT_UINT32(lIrq, lpFdata);
	lIrq      = OAL_NVIC_TO_GIC_IRQ(lIrq);
	*apIrqNum = (int32_t)lIrq;

fdt_get_irq_exit:
	return lRet;
}

int32_t OAL_FdtMapIomemReg(const struct fdt_node *acpNode, int32_t aIndex,
                          uint8_t **apVirt, uint64_t *apLen)
{
	uint64_t lRegBase;

	int32_t lRet = OAL_FdtGetReg(acpNode, aIndex, &lRegBase, apLen);
	if (lRet == 0) {
		*apVirt = (uint8_t *)OAL_kmemmap((uintptr_t)lRegBase, *apLen);
		if (*apVirt == NULL) {
			lRet = -ENODEV;
		}
	}

	return lRet;
}

#define CLOCK_NAMES_PROPERTY "clock-names"
#define CLOCKS_PROPERTY "clocks"

int32_t OAL_FdtGetClk(const struct fdt_node *acpNode, const char8_t *acpClockName,
                    dev_clock_t *apDclock)
{
	int32_t lRet;
	uintptr_t lFdtAddr;
	uint8_t *lpFdata   = NULL;
	int32_t lPropLen   = 0;
	uint32_t lUPropLen = 0U;
	size_t lClkIndex   = 0;
	size_t lClknameLen;
	size_t lMinLen;

	if ((acpNode == NULL) || (acpClockName == NULL) || (apDclock == NULL)) {
		lRet = -EINVAL;
		goto fdt_get_clk_exit;
	}

	lClknameLen = strlen(acpClockName);
	lRet        = OAL_GetFdtAddress(&lFdtAddr);
	if (lRet != 0) {
		lRet = lRet;
		goto fdt_get_clk_exit;
	}

	// Identify the clock index by its name
	lRet = OAL_GetProp((const uint32_t *)lFdtAddr, acpNode->mOffset,
	                   CLOCK_NAMES_PROPERTY, &lUPropLen, &lpFdata);
	if (lpFdata == NULL) {
		lRet = -ENODEV;
		goto fdt_get_clk_exit;
	}

	lPropLen = (int32_t)lUPropLen;
	while (lPropLen > 0) {
		size_t lClen, lClenExt;
		if ((ssize_t)lClknameLen > (ssize_t)lPropLen) {
			lRet = -EINVAL;
			goto fdt_get_clk_exit;
		}
		lClen = strlen((char8_t *)lpFdata);
		if (strncmp((char8_t *)lpFdata, acpClockName, lClknameLen) ==
		    0) {
			break;
		}

		lClkIndex++;
		lClenExt = lClen + 1ULL;

		lPropLen -= (int32_t)lClenExt;
		lpFdata += lClenExt;
	}

	if (lPropLen <= 0) {
		lRet = -EINVAL;
		goto fdt_get_clk_exit;
	}

	lMinLen = sizeof(uint32_t) * (lClkIndex + 1ULL) * 2ULL;
	lRet    = OAL_GetProp((const uint32_t *)lFdtAddr, acpNode->mOffset,
	                   CLOCKS_PROPERTY, &lUPropLen, &lpFdata);
	if ((lpFdata == NULL) || ((size_t)lUPropLen < lMinLen)) {
		lRet = -ENODEV;
		goto fdt_get_clk_exit;
	}

	OAL_PROP_SKIP_N_VALUES(lpFdata, (lClkIndex * 2ULL) + 1ULL, uint32_t);
	OAL_PROP_GET_NEXT_UINT32(apDclock->mNumber, lpFdata);

fdt_get_clk_exit:
	return lRet;
}
