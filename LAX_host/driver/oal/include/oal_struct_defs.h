/*
 * Copyright 2014-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __STRUCT_DEFS_H__
#define __STRUCT_DEFS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#if 0
/**
 * Structure saves info about user space mappings
 */
typedef struct UserAddress
{
  /**
   * Process id
   */
  uint32_t pid;

  /**
   * User virtual memory
   */
  uint64_t user_virtual;

  /**
   * Next in double-linked list
   */
  struct UserAddress *next;

  /**
   * Previous in double-linked list
   */
  struct UserAddress *prev;
} t_UserAddress;
#endif	/* if 0 */

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

#ifdef __cplusplus
}
#endif

#endif
