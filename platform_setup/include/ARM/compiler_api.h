/*
 * Copyright 2017-2023 NXP
 * NXP Confidential and Proprietary. This software is owned or controlled by NXP and
 * may only be used strictly in accordance with the applicable license terms.  By
 * expressly accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms.  If you do not agree to
 * be bound by the applicable license terms, then you may not retain, install, activate or
 * otherwise use the software.
 */

#ifndef COMPILER_API_H
#define COMPILER_API_H




#include "typedefs.h"

#include "rsdk_osenv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
#if !defined(__GNUC__) && !defined(__ghs__) && !defined(__DCC__)
#error "Compiler is not defined"
#endif









#if   defined(__linux__)
#define CompilerGetCoreId() (0)
#else
#define CompilerGetCoreId() (0)
#endif

#define stringify(s) tostring(s)
#define tostring(s) #s

#if defined(__GNUC__)
#define INLINE_ASM __asm
#define ASMNOP(label) INLINE_ASM(stringify(label) ": nop")
#elif defined(__ghs__)
#define INLINE_ASM __asm
#define ASMNOP(label) INLINE_ASM(stringify(label) ":: nop")
#endif
/* Put name between "" */
#define STR(s) #s

/* Put expanded macro/define between "" */
#define XSTR(s) STR(s)


/* definitions for assembler functions */

#ifdef __cplusplus
}
#endif

#endif  /* COMPILER_API_H */
