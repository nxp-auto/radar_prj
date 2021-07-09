/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_BYTEORDER_H
#define OAL_BYTEORDER_H

#include <oal_utils.h>

__BEGIN_DECLS

/**
 * @defgroup OAL_Endianness_Conversions  OAL Byteorder
 *
 * @{
 * @brief OAL Endianness Conversions
 * @details
 * Converts 16/32/64-bit data to Little Endiand/Big Endian,
 * depending on the endianness of the platform (detected at runtime)
 */

/**
 * @brief      Return a 16-bit little endian value,
 *             according to the endianness of the CPU
 *
 * @param[in]  aVal Value to be converted
 *
 * @return     Little endian 16-bit value
 */
uint16_t OAL_GetLittleEndian16(uint16_t aVal);

/**
 * @brief      Return a 32-bit little endian value,
 *             according to the endianness of the CPU
 *
 * @param[in]  aVal Value to be converted
 *
 * @return     Little endian 32-bit value
 */
uint32_t OAL_GetLittleEndian32(uint32_t aVal);

/**
 * @brief      Return a 64-bit little endian value,
 *             according to the endianness of the CPU
 *
 * @param[in]  aVal Value to be converted
 *
 * @return     Little endian 64-bit value
 */
uint64_t OAL_GetLittleEndian64(uint64_t aVal);

/**
 * @brief      Return a 16-bit big endian value,
 *             according to the endianness of the CPU
 *
 * @param[in]  aVal Value to be converted
 *
 * @return     Big endian 16-bit value
 */
uint16_t OAL_GetBigEndian16(uint16_t aVal);

/**
 * @brief      Return a 32-bit big endian value,
 *             according to the endianness of the CPU
 *
 * @param[in]  aVal Value to be converted
 *
 * @return     Big endian 32-bit value
 */
uint32_t OAL_GetBigEndian32(uint32_t aVal);

/**
 * @brief      Return a 64-bit big endian value,
 *             according to the endianness of the CPU
 *
 * @param[in]  aVal Value to be converted
 *
 * @return     Big endian 64-bit value
 */
uint64_t OAL_GetBigEndian64(uint64_t aVal);

__END_DECLS

/* @} */

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_byteorder.h>
#endif
#endif /* OAL_BYTEORDER_H */
