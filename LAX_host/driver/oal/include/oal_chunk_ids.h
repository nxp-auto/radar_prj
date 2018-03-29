/*
 * Copyright 2014-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef HEADEROALCHUNKIDS_H
#define HEADEROALCHUNKIDS_H

/*==============================================================================
*                          DEFINES AND MACROS
==============================================================================*/

#define OAL_CHUNK_ID_DDR0          0x01   ///< memory region in DDR0
#define OAL_CHUNK_ID_DDR1          0x02   ///< memory region in DDR1
#define OAL_CHUNK_ID_SRAM_SINGLE   0x03   ///< memory region in SRAM 3MB section
#define OAL_CHUNK_ID_SRAM_MULTI    0x04   ///< memory region in SRAM 1MB section
#define OAL_CHUNK_ID_AUTO          0x00   ///< up to OAL to decide

#define OAL_CHUNK_ID_MAXNR  OAL_CHUNK_ID_SRAM_MULTI /* maximum CHUNK_ID number */

#endif /* HEADEROALCHUNKIDS_H */
