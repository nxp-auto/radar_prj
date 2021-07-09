/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_log.h>
#include <qnx_posix_typed_mem_utils.h>

static int32_t typed_mem_matcher(struct asinfo_entry *apInfo, char8_t *apName,
                                 void *apData)
{
	struct OAL_MemoryAllocatorRegion *lpMemReg =
	    (struct OAL_MemoryAllocatorRegion *)apData;
	char8_t lPath[OAL_MAX_POSIX_TYPED_MEM_LEN];
	int32_t lRet;
	size_t lPathLen;

	OAL_UNUSED_ARG(apName);
	(void)memset(lPath, 0, sizeof(lPath));
	lRet = OAL_QNX_GetPathFromAsInfo(apInfo, lPath, sizeof(lPath));
	if (lRet != 0) {
		OAL_LOG_ERROR("Failed to get posix path of %s node\n", apName);
		goto matcher_exit;
	}

	lPathLen = strnlen(lPath, sizeof(lpMemReg->mName)) + 1U;
	if (lPathLen >= sizeof(lpMemReg->mName)) {
		OAL_LOG_ERROR(
		    "Posix name (%s) is too long, please adjust "
		    "memory region's name filed max length\n",
		    lPath);
		lRet = -1;
		goto matcher_exit;
	}

	(void)memcpy(lpMemReg->mName, lPath, lPathLen);

matcher_exit:
	return lRet;
}

int32_t OAL_OS_ResolveMemRegName(struct OAL_MemoryAllocatorRegion *apMemReg)
{
	int32_t lRet = 0;

	if (apMemReg == NULL) {
		lRet = -EINVAL;
		goto resolve_name_exit;
	}

	lRet = walk_asinfo(apMemReg->mName, typed_mem_matcher, apMemReg);

resolve_name_exit:
	return lRet;
}
