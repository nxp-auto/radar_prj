/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef OAL_TYPES_H_
#define OAL_TYPES_H_

#ifdef __cplusplus
extern "C"
{
#else
#include <stdbool.h>
#endif

typedef struct
{
    char     name[50];
    uint64_t start_address;
    uint32_t size;
    uint8_t  id;
    uint32_t align;
    uint8_t  init;
    uint8_t  autobalance;
} oal_mem;

static inline int is_memory_zone_empty(const oal_mem zone)
{
    bool a = ((bool)zone.start_address || (bool)zone.size || (bool)zone.id ||
              (bool)zone.align || (bool)zone.init || (bool)zone.autobalance);
    return !a;
}

#ifdef __cplusplus
}
#endif

#endif /* OAL_TYPES_H_ */
