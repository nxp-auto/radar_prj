/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <linux/sched.h>
#include "oal_allocation_kernel.h"

void *OAL_KernelMemoryAlloc(uint64_t size, uint64_t align, uint8_t chunk_id)
{
  int32_t pid = 0;

  pid = task_pgrp_nr(current);
  return (void *)oal_alloc(size, align, chunk_id, pid);
}

extern uint64_t OAL_KernelMemoryFree;
EXPORT_SYMBOL(OAL_KernelMemoryFree);
EXPORT_SYMBOL(OAL_KernelMemoryAlloc);
