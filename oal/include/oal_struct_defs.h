/*
 * Copyright 2014-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __STRUCT_DEFS_H__
#define __STRUCT_DEFS_H__

#include "oal_utils.h"

__BEGIN_DECLS

/**
 * Structure saves info about kernel space contiguous memory allocation
 */
typedef struct
{
  /**
   * Process id
   */
  uint32_t pid;

  uint64_t address[5];

  /**
   * Allocation size
   */
  uint64_t size;

  /**
   * Memory chunk id
   */
  uint8_t chunk_id;

} t_MemArea;

__END_DECLS

#endif
