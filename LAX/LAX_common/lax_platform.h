/*
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef LAX_PLATFORM_H_
#define LAX_PLATFORM_H_

/**
* @file           lax_platform.h
* @implements     Top header file for platform definitions 
*/


/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#ifdef LAX_PLATFORM_RRM
#include "platform_rrm.h"
//RRM does not have address translation unit (ATU)
#define LAX_PLATFORM_ATU 0
#else
#include "platform_wifi.h"
//UAWIFI features ATU
#define LAX_PLATFORM_ATU 1
#endif



#endif /* LAX_PLATFORM_H_ */
