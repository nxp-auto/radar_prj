/*
* Copyright 2017 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

#if defined(__GNUC__) /* GCC compiler*/
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif
/* Short names for volatiles used by header files, based on ISO C standard */
typedef volatile int8_t  vint8_t;
typedef volatile uint8_t vuint8_t;

typedef volatile int16_t  vint16_t;
typedef volatile uint16_t vuint16_t;

typedef volatile int32_t  vint32_t;
typedef volatile uint32_t vuint32_t;

typedef volatile int64_t  vint64_t;
typedef volatile uint64_t vuint64_t;

/*float types*/
typedef float  float32_t;
typedef double float64_t;

#elif defined(__MWERKS__) /* Metrowerk CodeWarrior */
#include <stdint.h>

/*  Standard typedefs used by header files, based on ISO C standard */
typedef volatile int8_t  vint8_t;
typedef volatile uint8_t vuint8_t;

typedef volatile int16_t  vint16_t;
typedef volatile uint16_t vuint16_t;

typedef volatile int32_t  vint32_t;
typedef volatile uint32_t vuint32_t;

#elif defined(__ghs__) /* GreenHills */
#include <stdint.h>

/* Standard typedefs used by header files, based on ISO C standard */
typedef volatile int8_t  vint8_t;
typedef volatile uint8_t vuint8_t;

typedef volatile int16_t  vint16_t;
typedef volatile uint16_t vuint16_t;

typedef volatile int32_t  vint32_t;
typedef volatile uint32_t vuint32_t;

#elif defined(__DCC__) /* WindRiver's diab*/
#include <stdint.h>

/* Standard typedefs used by header files, based on ISO C standard */
typedef volatile int8_t  vint8_t;
typedef volatile uint8_t vuint8_t;

typedef volatile int16_t  vint16_t;
typedef volatile uint16_t vuint16_t;

typedef volatile int32_t  vint32_t;
typedef volatile uint32_t vuint32_t;

/*float types*/
typedef float  float32_t;
typedef double float64_t;

#else
/* This is needed for compilers that don't have a stdint.h file i.e. DIAB */

typedef signed char            int8_t;
typedef unsigned char          uint8_t;
typedef volatile signed char   vint8_t;
typedef volatile unsigned char vuint8_t;

typedef signed short            int16_t;
typedef unsigned short          uint16_t;
typedef volatile signed short   vint16_t;
typedef volatile unsigned short vuint16_t;

typedef signed long            int32_t;
typedef unsigned long          uint32_t;
typedef volatile signed long   vint32_t;
typedef volatile unsigned long vuint32_t;

/* 8-byte Extended type, supported by DIAB */
typedef long long          int64_t;
typedef unsigned long long uint64_t;

/*float types*/
typedef float  float32_t;
typedef double float64_t;

#endif

#ifdef __cplusplus
}
#endif

#endif  //TYPEDEFS_H
