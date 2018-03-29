/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "oal_log.h"

#ifndef __OAL_DEBUG_OUT__H__
#define __OAL_DEBUG_OUT__H__

#ifdef DEBUG_OUTPUT
  #define oal_printk(...) VDB_LOG_NOTE(__VA_ARGS__)
#else
  #define oal_printk(...)
#endif


#endif
