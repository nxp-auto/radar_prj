/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_devtree_utils.h>
#include <oal_log.h>

static int32_t priv_fdt_node_by_compatible(uintptr_t aFdtAddr,
                                           const char8_t *acpCompatible,
                                           struct fdt_node *apNode)
{
	int32_t lRet = 0;

	lRet = OAL_NodeOffsetByCompatible(
	    (const uint32_t *)aFdtAddr, apNode->mOffset, acpCompatible,
	    &apNode->mOffset);

	if (apNode->mOffset == ((uint32_t)OAL_FDT_OFFSET_ERR)) {
		lRet = -ENODEV;
	} else {
		lRet =
		    OAL_GetName((const uint32_t *)aFdtAddr,
				apNode->mOffset, NULL, &apNode->mpName);
	}

	return lRet;
}

int32_t OAL_FdtNodeByCompatible(const char8_t *acpCompatible,
                                struct fdt_node *apNode)
{
	uintptr_t lFdtAddr;
	int32_t lRet = OAL_GetFdtAddress(&lFdtAddr);
	if (lRet == 0) {
		if ((acpCompatible == NULL) || (apNode == NULL)) {
			lRet = -EINVAL;
		} else {
			apNode->mOffset = (uint32_t)OAL_FDT_OFFSET_ERR;
			lRet            = priv_fdt_node_by_compatible(
			    lFdtAddr, acpCompatible, apNode);
		}
	} else {
		OAL_LOG_ERROR("Failed to get the address of the device tree\n");
	}

	return lRet;
}

int32_t OAL_FdtGetNmatchesByCompatible(const char8_t *const *acpCompatible)
{
	uint32_t lOffset = 0U;
	int32_t lCount   = -ENODEV;
	uintptr_t lFdtAddr;
	int32_t lRet                            = 0;
	const char8_t *const *lcpCompatibleIter = acpCompatible;

	OAL_CHECK_NULL_PARAM(acpCompatible,
	                     fdt_get_nmatches_by_compatible_exit);
	lRet = OAL_GetFdtAddress(&lFdtAddr);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get the address of the device tree\n");
		goto fdt_get_nmatches_by_compatible_exit;
	}
	lCount = 0;
	while (*lcpCompatibleIter != NULL) {
		lOffset = (uint32_t)(OAL_FDT_OFFSET_ERR);
		do {
			lRet = OAL_NodeOffsetByCompatible(
			    (const uint32_t *)lFdtAddr, lOffset,
			    *lcpCompatibleIter, &lOffset);
			if (lRet == 0) {
				lCount++;
			} else {
				if (lOffset != ((uint32_t)OAL_FDT_OFFSET_ERR)) {
					lCount = -1;
					break;
				}
			}
		} while (lRet == 0);
		lcpCompatibleIter++;
	}
fdt_get_nmatches_by_compatible_exit:
	return lCount;
}

int32_t OAL_FdtMatchCall(const char8_t *const *acpCompatible,
                         fdt_callback_t aCallback, void **apFdata)
{
	uintptr_t lFdtAddr;
	struct fdt_node lNode;
	int32_t lRet                            = 0;
	int32_t lI                              = 0;
	const char8_t *const *lcpCompatibleIter = acpCompatible;

	if (aCallback == NULL) {
		lRet = -EINVAL;
		goto fdt_match_call_exit;
	}

	lRet = OAL_GetFdtAddress(&lFdtAddr);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get the address of the device tree\n");
		lRet = -ENODEV;
		goto fdt_match_call_exit;
	}

	while (*lcpCompatibleIter != NULL) {
		lNode.mOffset = (uint32_t)(OAL_FDT_OFFSET_ERR);
		do {
			lRet = priv_fdt_node_by_compatible(
			    lFdtAddr, *lcpCompatibleIter, &lNode);
			if (lRet == 0) {
				if (apFdata != NULL) {
					lRet = aCallback(
					    &lNode,
					    (const void **)&apFdata[lI]);
				} else {
					lRet = aCallback(&lNode, NULL);
				}
				lI++;
				if (lRet != 0) {
					goto fdt_match_call_exit;
				}
			}
		} while (lRet == 0);
		lRet = 0;
		lcpCompatibleIter++;
	}

fdt_match_call_exit:
	return lRet;
}

int32_t OAL_FdtParsePhandle(const struct fdt_node *acpSnode,
                            struct fdt_node *apDestNode,
                            const char8_t *acpPropName)
{
	int32_t lRet         = 0;
	uint8_t *lpPropValue = NULL;
	uintptr_t lFdtAddr;
	uint32_t lPropLen = 0U;
	uint32_t lPhandle;

	lRet = OAL_GetFdtAddress(&lFdtAddr);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get the address of the device tree\n");
		goto parse_handle_exit;
	}

	lRet =
	    OAL_GetProp((const uint32_t *)lFdtAddr, (uint32_t)acpSnode->mOffset,
	                acpPropName, &lPropLen, &lpPropValue);
	if ((lpPropValue == NULL) ||  ((size_t)lPropLen < sizeof(lPhandle))) {
		lRet = -ENODEV;
		goto parse_handle_exit;
	}

	OAL_PROP_GET_NEXT_UINT32(lPhandle, lpPropValue);

	lRet = OAL_NodeOffsetByPhandle((const uint32_t *)lFdtAddr, lPhandle,
	                               (uint32_t *)&apDestNode->mOffset);

	if (lRet != 0) {
		lRet = -ENODEV;
		goto parse_handle_exit;
	}

	lRet = OAL_GetName((const uint32_t *)lFdtAddr, apDestNode->mOffset,
	                   NULL, &apDestNode->mpName);
parse_handle_exit:
	return lRet;
}

int32_t OAL_FdtGetReg(const struct fdt_node *acpNode, int32_t aIndex,
                      uintptr_t *apRegBase, uintptr_t *apLen)
{
	uint32_t lPropLen = 0U;
	size_t lMinLen =
	    sizeof(uint64_t) * ((size_t)aIndex + ((size_t)(1U))) * ((size_t)2U);
	uint8_t *lpFdata = NULL;
	uintptr_t lFdtAddr;
	int32_t lRet = 0;

	if ((acpNode == NULL) || (apRegBase == NULL) || (apLen == NULL)) {
		lRet = -EINVAL;
		goto fdt_get_reg_exit;
	}

	lRet = OAL_GetFdtAddress(&lFdtAddr);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get the address of the device tree\n");
		goto fdt_get_reg_exit;
	}

	lRet =
	    OAL_GetProp((const uint32_t *)lFdtAddr, (uint32_t)acpNode->mOffset,
	                OAL_REG_PROPERTY, &lPropLen, &lpFdata);

	if ((lpFdata == NULL) || ((size_t)lPropLen < lMinLen)) {
		lRet = -ENODEV;
	} else {
		OAL_PROP_SKIP_N_VALUES(lpFdata, (size_t)aIndex * 2ULL,
		                       uint64_t);
		OAL_PROP_GET_NEXT_UINT64(*apRegBase, lpFdata);
		OAL_PROP_GET_NEXT_UINT64(*apLen, lpFdata);
	}

fdt_get_reg_exit:
	return lRet;
}

int32_t OAL_FdtGetNodeProperty(const struct fdt_node *acpNode,
                               const char8_t *acpPropName,
                               const char8_t **acpPropValue)
{
	int32_t lRet = 0;
	uint32_t lPropLen;
	uintptr_t lFdtAddr;
	uint8_t *lpValue;

	if ((acpNode == NULL) || (acpPropName == NULL)) {
		lRet = -EINVAL;
		goto fdt_get_node_prop_exit;
	}

	lRet = OAL_GetFdtAddress(&lFdtAddr);
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get the address of the device tree\n");
		goto fdt_get_node_prop_exit;
	}

	lRet =
	    OAL_GetProp((const uint32_t *)lFdtAddr, (uint32_t)acpNode->mOffset,
	                acpPropName, &lPropLen, &lpValue);

	if ((lRet == 0) && (acpPropValue != NULL)) {
		*acpPropValue = (char8_t *)lpValue;
	}

fdt_get_node_prop_exit:
	return lRet;
}
