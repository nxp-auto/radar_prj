/*
 * Copyright 2016-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef HW_PLATFORM_H
#define HW_PLATFORM_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#if defined(S32R294)
#include "rsdk_S32R294.h"
#elif defined(S32R45)
#include "rsdk_S32R45.h"
#elif defined(S32R41)
#include "rsdk_S32R41.h"
#elif defined(STRX)
#include "rsdk_strx.h"
#endif

#endif  //HW_PLATFORM_H
