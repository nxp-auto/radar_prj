/*
 * Copyright 2017-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "os_utils.h"
#include "arch_oal_timespec.h"

#include <string.h>

/**
 * Define symbols from libc only these are missing,
 * the final solution isn't linked with libc and is compiled with -nostdlib
 */
#ifdef OAL_BUILD_WITH_NOSTDLIB

size_t strnlen(const char *acpStr, size_t aN)
{
	size_t lN          = aN;
	size_t lRet        = 0;
	const char *lcpStr = acpStr;

	while ((lN > 0U) && (*lcpStr != '\0')) {
		lRet++;
		lcpStr++;
		lN--;
	}

	return lRet;
}

int strncmp(const char *acpStr1, const char *acpStr2, size_t aN)
{
	size_t lN           = aN;
	int lRes            = 0;
	const char *lcpStr1 = acpStr1;
	const char *lcpStr2 = acpStr2;

	while (lN > 0U) {
		lRes = *lcpStr1 - *lcpStr2;
		if ((lRes != 0) || (*lcpStr1 == '\0')) {
			break;
		}
		lcpStr1++;
		lcpStr2++;
		lN--;
	}

	return lRes;
}

char *strncpy(char *apDest, const char *acpSrc, size_t aN)
{
	size_t lN          = aN;
	char *lpRet        = apDest;
	char *lpDest       = apDest;
	const char *lcpSrc = acpSrc;

	while (lN != 0U) {
		*lpDest = *lcpSrc;
		if (*lpDest == '\0') {
			break;
		}
		lpDest++;
		lcpSrc++;
		lN--;
	}

	return lpRet;
}
#endif
