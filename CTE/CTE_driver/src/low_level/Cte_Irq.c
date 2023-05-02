/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**
*   @file
*   @implements Cte.c_Artifact
*
*   @addtogroup CTE
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*
* @page misra_violations MISRA-C:2012 violations
*
* @section Cte_c_REF_1
* Violates MISRA 2012 Advisory Directive 4.9,
*   #A function should be used in preference to a function-like macro where they are interchangeable
* Using a macro produce a smaller code; there is a clear distinction between functions and macros,
* which use only UPPERCASE
*
* @section Cte_c_REF_2
* Violates MISRA 2012 Required Directive 4.10,
*   #Precautions shall be taken in order to prevent the contents of a header file being included more than once.
* This violation is not fixed since the inclusion of <MA>_MemMap.h is as per AUTOSAR requirement [SWS_MemMap_00003].
*
* @section Cte_c_REF_3
* Violates MISRA C-2012 Advisory Rule 8.9,
*   #An object should be defined at block scope if its identifier only appears in a single function.
* To not mix code and data storage, for proper memory mapping,
* some of the static variables are not defined at block scope.
*
* @section Cte_c_REF_4
* Violates MISRA C-2012 Advisory Rule 11.4,
*   #A conversion should not be performed between a pointer to object and an integer type
* Some initialization need to be done for pointers.
*
* @section Cte_c_REF_5
* Violates MISRA C-2012 Required Rule 11.6,
*   #A cast shall not be performed between pointer to void and an arithmetic type
* In some contexts, is necessary to process pointers using arithmetic operations.
*
* @section Cte_c_REF_6
* Violates MISRA C-2012 Required Rule 18.1,
*   #A pointer resulting from arithmetic on a pointer operand shall address an element of the same array
*   as that pointer operand
* Some initialization need to be done for pointers.
*
* @section Cte_c_REF_7
* Violates MISRA C-2012 Required Rule 18.4,
*   #The +, -, += and -= operators should not be applied to an  expression of pointer type
* Necessary pointer operation, not possible to use normal pointer association.
*
* @section Cte_c_REF_8
* Violates MISRA 2012 Advisory Rule 20.1,
*   #Include directives should only be preceded by preprocessor directives or comments.
* <MA>_MemMap.h is included after each section define in order to set the current memory section as defined by AUTOSAR.
*
*/

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Cte_Irq.h"
#include "Cte_Specific.h"
    #ifndef linux
        #include <string.h>
        #include <stdint.h>
    #else
        #include "rsdk_cte_driver_module.h"
    #endif
    #include "rsdk_status.h"





    #include "S32R45_CTE.h"







/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/
/* error report management      */
#if !defined(CTE_REPORT_ERROR)
#endif

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
 *                                      GLOBAL CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      GLOBAL VARIABLES
 ==================================================================================================*/

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/
/**
 * @brief   Interrupt handler for CTE.
 * @details Low level interrupt handler for CTE driver.
 *
 */
#if   !defined(linux)
static void Cte_IrqHandler(
#ifdef __ZEPHYR__
    const void *pParams
#else
    void
#endif
);
#endif


/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 ==================================================================================================*/
/**
 * @brief   Interrupt handler for CTE.
 * @details Low level interrupt handler for CTE driver.
 *
 */
#if !defined(RSDK_AUTOSAR) && !defined(linux)
static
#endif
void Cte_IrqHandler(
#ifdef __ZEPHYR__
    const void *pParams
#else
    void
#endif
)
{
#ifdef __ZEPHYR__
    (void)(pParams);
#endif
    uint32 cteEvents;

    cteEvents = gspCTE->INTSTAT;
    gspCTE->INTSTAT = cteEvents;          /* clear all bits       */
    cteEvents &= gsDriverData.cteReqEvents;
    gsDriverData.pCteCallback(cteEvents);
}
/*=== Cte_IrqHandler ===========================*/


/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 ==================================================================================================*/
/*==================================================================================================*/
/**
 * @brief   Procedure to define the necessary clock periods.
 * @details The hardware can use up to 4 clock dividers, so up to 4 periods available.
 *          If more clocks required, intermediate periods will be used, to reduce the deviation below 40%.
 *          If it is not possible to have up to 4 clocks and all the required periods to enter 40% deviation,
 *          an error will be returned.
 *
 * @param[in]   pointers to the initialization params
 * @return      E_OK/RSDK_SUCCESS = success; other = error
 *
 */
Std_ReturnType Cte_IrqInit(const Cte_SetupParamsType *pCteInitParams)
{
    Std_ReturnType rez = (Std_ReturnType)E_OK;

    gspCTE->INTEN = 0u;             /* mask all irq sources         */
#if !defined(linux) && !defined(RSDK_AUTOSAR)       /* for Linux/ASR, the irq are registered in other place     */
    if (RsdkGlueIrqHandlerRegister(Cte_IrqHandler, CTE_IRQ_NUMBER,
            (rsdkCoreId_t)pCteInitParams->irqExecCore, pCteInitParams->irqPriority) != IRQ_REGISTER_SUCCESS)
    {
        rez = RSDK_CTE_DRV_IRQ_REG_FAILED;
    }
#endif
    if(((uint32)pCteInitParams->cteIrqEvents != 0u)
#if !defined(linux) && !defined(RSDK_AUTOSAR)
        && (rez == (Std_ReturnType)E_OK)
#endif
        )
    {                       /* events callback requested        */
        gspCTE->INTEN = (uint32) pCteInitParams->cteIrqEvents;
    }
    return rez;
}
/*=== Cte_IrqInit ===========================*/


