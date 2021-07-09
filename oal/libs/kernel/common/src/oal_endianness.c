/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <oal_endianness.h>

uint8_t OAL_IsLittleEndian(void)
{
	static uint32_t lsVal = 0xdeadbeefU;
	uint8_t *lpByte;
	uint8_t lRet;

	lpByte = (uint8_t *)&lsVal;
	if (*lpByte == 0xefU) {
		lRet = 1U;
	} else {
		lRet = 0U;
	}

	return lRet;
}


