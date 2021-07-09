/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <oal_byteorder.h>
#include <oal_endianness.h>

static inline uint16_t swapBytes16(uint16_t aByteVal)
{
	return ((aByteVal >> 8U) & 0x00ffU) | ((aByteVal << 8U) & 0xff00U);
}

static inline uint32_t swapBytes32(uint32_t aByteVal)
{
	return ((aByteVal >> 24U) & 0x000000ffU) |
	       ((aByteVal >> 8U) & 0x0000ff00U) |
	       ((aByteVal << 8U) & 0x00ff0000U) |
	       ((aByteVal << 24U) & 0xff000000U);
}

static inline uint64_t swapBytes64(uint64_t aByteVal)
{
	return ((aByteVal >> 56U) & 0x00000000000000ffULL) |
	       ((aByteVal >> 40U) & 0x000000000000ff00ULL) |
	       ((aByteVal >> 24U) & 0x0000000000ff0000ULL) |
	       ((aByteVal >> 8U) & 0x00000000ff000000ULL) |
	       ((aByteVal << 8U) & 0x000000ff00000000ULL) |
	       ((aByteVal << 24U) & 0x0000ff0000000000ULL) |
	       ((aByteVal << 40U) & 0x00ff000000000000ULL) |
	       ((aByteVal << 56U) & 0xff00000000000000ULL);
}

uint16_t OAL_GetLittleEndian16(uint16_t aVal)
{
	uint16_t lRet;

	if (OAL_IsLittleEndian() == 1U) {
		lRet = aVal;
	} else {
		lRet = swapBytes16(aVal);
	}
	return lRet;
}

uint32_t OAL_GetLittleEndian32(uint32_t aVal)
{
	uint32_t lRet;

	if (OAL_IsLittleEndian() == 1U) {
		lRet = aVal;
	} else {
		lRet = swapBytes32(aVal);
	}

	return lRet;
}

uint64_t OAL_GetLittleEndian64(uint64_t aVal)
{
	uint64_t lRet;

	if (OAL_IsLittleEndian() == 1U) {
		lRet = aVal;
	} else {
		lRet = swapBytes64(aVal);
	}

	return lRet;
}

uint16_t OAL_GetBigEndian16(uint16_t aVal)
{
	uint16_t lRet;

	if (OAL_IsLittleEndian() == 1U) {
		lRet = swapBytes16(aVal);
	} else {
		lRet = aVal;
	}

	return lRet;
}

uint32_t OAL_GetBigEndian32(uint32_t aVal)
{
	uint32_t lRet;

	if (OAL_IsLittleEndian() == 1U) {
		lRet = swapBytes32(aVal);
	} else {
		lRet = aVal;
	}

	return lRet;
}

uint64_t OAL_GetBigEndian64(uint64_t aVal)
{
	uint64_t lRet;

	if (OAL_IsLittleEndian() == 1U) {
		lRet = swapBytes64(aVal);
	} else {
		lRet = aVal;
	}

	return lRet;
}
