/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_ARCH_TIMESPEC_H
#define OAL_ARCH_TIMESPEC_H

#include <oal_utils.h>

__BEGIN_DECLS

/**
 * @brief Retrieve the size of the cache line
 *
 * @return Cache line size
 */
uint32_t OAL_ARCH_GetCacheLine(void);

__END_DECLS

#endif
