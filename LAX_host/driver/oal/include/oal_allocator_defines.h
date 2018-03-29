/*
 * Copyright 2014-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OALALLOCATORDEFINES_H
#define OALALLOCATORDEFINES_H

#ifdef __cplusplus
extern "C"
{
#endif

enum
{
  ACCESS_PHY    = 0,    ///< Physical access
  ACCESS_CH_WB  = 1,  ///< Cached access with Writeback policy
  ACCESS_CH_WT  = 2,  ///< Cached access with Writethrough policy
  ACCESS_NCH_B  = 3,  ///< Non-Cached Bufferable access
  ACCESS_NCH_NB = 4,  ///< Non-Cached Non-Bufferable Access
  ACCESS_NUM    = 5
};

#ifdef __cplusplus
}
#endif

#endif /* OALALLOCATORDEFINES_H */
