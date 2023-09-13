/*
* Copyright 2022 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SPT_INTERNALS_H
#define SPT_INTERNALS_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "CDD_Spt.h"
#include "Spt_Internals_Types.h"
#include "Spt_Cfg.h"
#include "rsdk_version.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/*
* @brief   Macro to read a register
* */
#define SPT_HW_READ_REG(reg) (reg)

/*
* @brief   Macro to read a field of a register based on a mask and a shift value for the mask
* */
#define SPT_HW_READ_BITS(reg, mask, shift) (((reg) & ((uint32)mask)) >> ((uint32)shift))

/*
* @brief   Macro to write a register
* */
#define SPT_HW_WRITE_REG(reg, value) ((reg) = ((uint32)value))

/*
* @brief   Macro to write a field of a register using a mask
*  */
#define SPT_HW_WRITE_BITS(reg, mask, value) ((reg) = (((reg) & (~((uint32)mask))) | ((uint32)value)))

#define SPT_PRINT   OAL_SPT_PRINT

#ifndef TRACE_ENABLE
#define RsdkTraceLogEvent(event, extra1, extra2)
#endif

#if !defined(SPT_REPORT_ERROR)
#define SPT_REPORT_ERROR(a, b, c)   RSDK_REPORT_ERROR((rsdkStatus_t)a, (uint16)CDD_SPT_MODULE_ID,   \
                                    (uint8)SPT_INSTANCE_ID, (uint8)b, (uint8)c);                    \
                                    SPT_HALT_ON_ERROR;
#endif

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/
extern Spt_DrvMemPerType gSptMemPer;

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

 static inline void Spt_SetDrvState(Spt_DrvStateType newState)
{
    gSptMemPer.state = newState;
}

volatile SPT_Type *Spt_GetMemMap(void);
Std_ReturnType     Spt_UnmapMem(void);

void               Spt_InitPersistentMem(Spt_DrvMemPerType *pSptMemPer, Spt_DriverInitType const *const pSptInitInfo);

Std_ReturnType     Spt_SetInputParams(Spt_DrvParamType const paramList[]);
Std_ReturnType     Spt_ParamCheckRun(Spt_DriverContextType const *const sptContext, const Spt_DrvStateType state);
Std_ReturnType     Spt_ParamCheckInit(Spt_DriverInitType const *const pSptInitInfo);
void               Spt_GetKernelRetVal(volatile sint32 *kernelRetPar);

#if(SPT_DSP_ENABLE == STD_ON)
uint8              Spt_GenCrc8(const uint8* inData, uint8 numBytes);
#endif

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* SPT_INTERNALS_H */
