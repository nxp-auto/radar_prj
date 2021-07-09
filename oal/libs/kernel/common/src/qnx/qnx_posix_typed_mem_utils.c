/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <qnx_posix_typed_mem_utils.h>
#include <oal_log.h>

#define AS_OFF2INFO(OFF, AS_ROOT)                                              \
	((struct asinfo_entry const *)(((const char8_t *)(AS_ROOT)) + (OFF)))
#define PATH_SEPARATOR '/'

static size_t get_path_length(struct asinfo_entry const *acpAs,
                              const char8_t *acpStrings,
                              const struct asinfo_entry *acpAsRoot)
{
	const char8_t *lcpAsName;
	size_t lLen = 0;

	while (acpAs->owner != AS_NULL_OFF) {
		lcpAsName = acpStrings + acpAs->name;
		lLen += strlen(lcpAsName) + 1ULL;
		acpAs = AS_OFF2INFO(acpAs->owner, acpAsRoot);
	}

	lcpAsName = acpStrings + acpAs->name;
	lLen += strlen(lcpAsName) + 1ULL;

	// + End of string
	return lLen + 1ULL;
}

static int32_t get_path(struct asinfo_entry const *acpAs, char8_t *apPath,
                        size_t aPathLen, const char8_t *acpStrings,
                        const struct asinfo_entry *acpAsRoot)
{
	int32_t lRet = 0;

	size_t lNameLen;
	struct asinfo_entry const *lcpAs = acpAs;
	size_t lLen    = get_path_length(lcpAs, acpStrings, acpAsRoot);
	size_t lOffset = lLen - 1ULL;
	const char8_t *lcpAsName;
	char8_t *lpPtr;
	uintptr_t lAsInfoAddr;

	if ((apPath == NULL) || (aPathLen < lLen)) {
		lRet = -ENOMEM;
		goto get_path_exit;
	}

	apPath[lLen] = (char8_t)0;

	while (lcpAs->owner != AS_NULL_OFF) {
		lcpAsName = acpStrings + lcpAs->name;
		lNameLen  = strlen(lcpAsName);
		lOffset -= (lNameLen + 1ULL);
		lpPtr = apPath + lOffset;

		*lpPtr = PATH_SEPARATOR;
		(void)memcpy(lpPtr + 1, lcpAsName, lNameLen);

		lAsInfoAddr = (uintptr_t)AS_OFF2INFO(lcpAs->owner, acpAsRoot);
		lcpAs       = (struct asinfo_entry const *)lAsInfoAddr;
	}

	lcpAsName = acpStrings + lcpAs->name;
	lNameLen  = strlen(lcpAsName);
	*apPath  = PATH_SEPARATOR;
	(void)memcpy(apPath + 1ULL, lcpAsName, lNameLen);

get_path_exit:
	return lRet;
}

int32_t OAL_QNX_GetPathFromAsInfo(struct asinfo_entry const *acpAs,
                                  char8_t *apPath, size_t aPathLen)
{
	int32_t lRet;
	char8_t *lpStrings            = SYSPAGE_ENTRY(strings)->data;
	struct asinfo_entry *lpAsRoot = SYSPAGE_ENTRY(asinfo);

	lRet = get_path(acpAs, apPath, aPathLen, lpStrings, lpAsRoot);

	return lRet;
}
