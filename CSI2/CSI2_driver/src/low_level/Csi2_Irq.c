/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/**
*   @file
*   @implements Csi2_Irq.c_Artifact
*
*   @addtogroup CSI2
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*
* @page misra_violations MISRA-C:2012 violations
*
* @section Csi2_Irq_c_REF_1
* Violates MISRA 2012 Advisory Directive 4.9,
* #A function should be used in preference to a function-like macro where they are interchangeable
* Using a macro produce a smaller code; there is a clear distinction between functions and macros,
* which use only UPPERCASE
*
* @section Csi2_Irq_c_REF_2
* Violates MISRA C-2012 Advisory Rule 8.9,
* #An object should be defined at block scope if its identifier only appears in a single function.
* To not mix code and data storage, for proper memory mapping,
* some of the static variables are not defined at block scope.
*
* @section Csi2_Irq_c_REF_3
* Violates MISRA C-2012 Advisory Rule 11.3,
* #A cast shall not be performed between a pointer to object type and a pointer to a different object type
* This is the only possibility to get the correct pointer to the data to be processed.
*
* @section Csi2_Irq_c_REF_4
* Violates MISRA C-2012 Required Rule 18.4,
* #The +, -, += and -= operators should not be applied to an  expression of pointer type
* Necessary pointer operation, not possible to use normal pointer association.
*
* @section Csi2_Irq_c_REF_5
* Violates MISRA 2012 Advisory Rule 20.1,
* #Include directives should only be preceded by preprocessor directives or comments.
* <MA>_MemMap.h is included after each section define in order to set the current memory section as defined by AUTOSAR.
*/


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Csi2_Cfg.h"
#include "Csi2_Types.h"
#include "CDD_Csi2.h"
#include "Csi2_Irq.h"
#if   !defined(linux)
    #include "Csi2_Defs.h"
#endif
#if defined(linux)
    #include <linux/string.h>
#else
    #include "string.h"
#endif

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

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

#if !defined(RSDK_AUTOSAR) && !defined(linux)
    // Individual irq unit handlers for Rx errors
    static void Csi2_IrqHandlerRxErr0(
    #if defined(__ZEPHYR__)
            const void * paramsPtr
    #else
            void
    #endif
    );
    static void Csi2_IrqHandlerRxErr1(
        #if defined(__ZEPHYR__)
            const void * paramsPtr
        #else
            void
        #endif

    );
        static void Csi2_IrqHandlerRxErr2(
            #if defined(__ZEPHYR__)
                const void * paramsPtr
            #else
                void
            #endif

        );
        static void Csi2_IrqHandlerRxErr3(
            #if defined(__ZEPHYR__)
                const void * paramsPtr
            #else
                void
            #endif

        );
    // Individual irq unit handlers for Path errors
    static void Csi2_IrqHandlerPathErr0(
    #if defined(__ZEPHYR__)
            const void * paramsPtr
    #else
            void
    #endif
    );
    static void Csi2_IrqHandlerPathErr1(
        #if defined(__ZEPHYR__)
            const void * paramsPtr
        #else
            void
        #endif
    );
        static void Csi2_IrqHandlerPathErr2(
            #if defined(__ZEPHYR__)
                const void * paramsPtr
            #else
                void
            #endif
        );
        static void Csi2_IrqHandlerPathErr3(
            #if defined(__ZEPHYR__)
                const void * paramsPtr
            #else
                void
            #endif
        );
    // Individual irq unit handlers for Events (Rx)
    static void Csi2_IrqHandlerEvents0(
    #if defined(__ZEPHYR__)
            const void * paramsPtr
    #else
            void
    #endif
    );
    static void Csi2_IrqHandlerEvents1(
        #if defined(__ZEPHYR__)
            const void * paramsPtr
        #else
            void
        #endif
    );
        static void Csi2_IrqHandlerEvents2(
            #if defined(__ZEPHYR__)
                const void * paramsPtr
            #else
                void
            #endif
        );
        static void Csi2_IrqHandlerEvents3(
            #if defined(__ZEPHYR__)
                const void * paramsPtr
            #else
                void
            #endif
        );
#endif  /* #if !defined(linux)        */

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/
// irq handlers matrix
    #if !defined(linux)

    static  rsdkIrqHandler_t    sgIrqHandlers[RSDK_CSI2_MAX_UNITS][RSDK_CSI2_MAX_IRQ_ID]
            = {
            {// unit 1 handlers
             Csi2_IrqHandlerRxErr0, Csi2_IrqHandlerPathErr0, Csi2_IrqHandlerEvents0},
            {// unit 2 handlers
             Csi2_IrqHandlerRxErr1, Csi2_IrqHandlerPathErr1, Csi2_IrqHandlerEvents1},
            {// unit 3 handlers
             Csi2_IrqHandlerRxErr2, Csi2_IrqHandlerPathErr2, Csi2_IrqHandlerEvents2},
            {// unit 4 handlers
             Csi2_IrqHandlerRxErr3, Csi2_IrqHandlerPathErr3, Csi2_IrqHandlerEvents3}
    };

    static uint32_t gsCsi2IrqUnitRemap[RSDK_CSI2_MAX_UNITS] = {
        (uint32_t)RSDK_CSI2_UNIT_1, (uint32_t)RSDK_CSI2_UNIT_0, (uint32_t)RSDK_CSI2_UNIT_3, (uint32_t)RSDK_CSI2_UNIT_2
    };
    #endif  /* #if !defined(linux) */


/*==================================================================================================
*                                        GLOBAL FUNCTIONS PROTOTYPES
==================================================================================================*/
/* The necessary ISR callbacks                                                                      */


/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*================================================================================================*/
/*
 * @brief       Enable for Rx errors interrupt on the specified VC.
 *
 * @param[in]   vcId    - VC ID, CSI2_VC_0 ... CSI2_MAX_VC
 * @param[in]   regsPtr - pointer to unit registry
 * @param[in]   val     - value to be set, usually 0 or 0xffffffff
 *
 */
static void Csi2_SetVCRxIrq(const uint32 vcId, volatile GENERIC_CSI2_Type *regsPtr, uint32 val)
{
    regsPtr->RX_VC[vcId].PPERRIE = val;
}
/* Csi2_SetVCRxIrq *************************/


/*================================================================================================*/
/*
 * @brief       Init for Rx errors interrupt.
 * @details     This procedure is called after the general unit setup was done.
 *
 * @param[in]   regsPtr - pointer to unit registry
 * @param[in]   paramsPtr - pointer to CSI2 unit setup parameters
 *
 */
static void Csi2_InitRxIrq(volatile GENERIC_CSI2_Type *regsPtr, const Csi2_SetupParamsType *paramsPtr)
{

    uint32                  vcId;                       /* VC ID, from CSI2_VC_0 to MAX                           */

    /* real irq enablement                                                                                        */
    regsPtr->RX_PHYERRIE = 0xffffffffu;                   /* enable all error flags                               */
    for (vcId = (uint32)CSI2_VC_0; vcId < (uint32)CSI2_MAX_VC; vcId++)  /* check all VC                           */
    {
        if (paramsPtr->vcConfigPtr[vcId] != (Csi2_VCParamsType*)NULL_PTR)   /* only for active VC                 */
        {
            Csi2_SetVCRxIrq(vcId, regsPtr, 0xffffffffu);  /* enable the interrupts                                */
        }
        else
        {
            Csi2_SetVCRxIrq(vcId, regsPtr, 0u);           /* disable the interrupts                               */
        }
    }
}
/* Csi2_InitRxIrq *************************/


/*================================================================================================*/
/*
 * @brief       Setup for Path errors interrupt.
 * @details     This procedure is called after the general unit setup was done.
 *
 * @param[in]   regsPtr     - pointer to unit registry
 *
 */
static void Csi2_InitPathIrq(volatile GENERIC_CSI2_Type *regsPtr)
{
    /* real irq enablement                                                                                      */
    regsPtr->CONTROLLER_ERR_IE = 0xffffffffu;
    /* enable all the possible errors on channel data                                                           */
    /*
    * @section Csi2_Irq_c_REF_1
    * Violates MISRA 2012 Advisory Directive 4.9,
    * #A function should be used in preference to a function-like macro where they are interchangeable
    * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
    * which use only UPPERCASE
    */
    CSI2_SET_REGISTRY32(&regsPtr->RX_CHNL_INTRE, MIPICSI2_RX_CHNL_INTRE_BUFFOVFIE_MASK,
                                                                            MIPICSI2_RX_CHNL_INTRE_BUFFOVFIE(1u));
    regsPtr->CBUF_INTRE = 0xffffffu;                                      /* enable all interrupts for line len & cnt */
}
/* Csi2_InitPathIrq *************************/


/*================================================================================================*/
/*
 * @brief       Enable for events interrupt on VC.
 * @details     This procedure is called after the general unit initialization was done.
 *
 * @param[in]   iUnit   - unit ID, RSDK_CSI2_UNIT_0 ... RSDK_CSI2_MAX_UNITS
 * @param[in]   regsPtr - pointer to unit registry
 * @param[in]   val     - single bit value, 0=disable, 1=enable
 *
 */
static void Csi2_SetVCEventIrq(const uint32 vcId, volatile GENERIC_CSI2_Type *regsPtr, uint8 evtMask,
                              uint8 numeLinesTrigger)
{
    uint8   valFS;
    uint8   valFE;

    valFS = ((evtMask & (uint8)CSI2_EVT_FRAME_START) != 0u) ? 1u : 0u;
    valFE = ((evtMask & (uint8)CSI2_EVT_FRAME_END) != 0u) ? 1u : 0u;
    /* set all ie bits                                                                                              */
    switch (vcId)                   /* set the necessary enable bit, to the appropriate registry                    */
    {
        default:                    /* RSDK_CSI2_VC_0                                                               */
            /*
            * @section Csi2_Irq_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(
                &regsPtr->RX_VCINTRE,
                MIPICSI2_RX_VCINTRE_GNSPIE0_MASK | MIPICSI2_RX_VCINTRE_FSIE0_MASK | MIPICSI2_RX_VCINTRE_FEIE0_MASK,
                MIPICSI2_RX_VCINTRE_GNSPIE0(valFE) | MIPICSI2_RX_VCINTRE_FSIE0(valFS) | MIPICSI2_RX_VCINTRE_FEIE0(valFE)
                );
            regsPtr->RX[0].CBUF_LPDI = (uint8)numeLinesTrigger;   /* trigger after each chirp (for stat. process)     */
            break;
        case (uint32)CSI2_VC_1:
            /*
            * @section Csi2_Irq_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(
                &regsPtr->RX_VCINTRE,
                MIPICSI2_RX_VCINTRE_GNSPIE1_MASK | MIPICSI2_RX_VCINTRE_FSIE1_MASK | MIPICSI2_RX_VCINTRE_FEIE1_MASK,
                MIPICSI2_RX_VCINTRE_GNSPIE1(valFE) | MIPICSI2_RX_VCINTRE_FSIE1(valFS) | MIPICSI2_RX_VCINTRE_FEIE1(valFE)
                );
            regsPtr->RX[1u].CBUF_LPDI = (uint8)numeLinesTrigger;
            break;
        case (uint32)CSI2_VC_2:
            /*
            * @section Csi2_Irq_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(
                &regsPtr->RX_VCINTRE,
                MIPICSI2_RX_VCINTRE_GNSPIE2_MASK | MIPICSI2_RX_VCINTRE_FSIE2_MASK | MIPICSI2_RX_VCINTRE_FEIE2_MASK,
                MIPICSI2_RX_VCINTRE_GNSPIE2(valFE) | MIPICSI2_RX_VCINTRE_FSIE2(valFS) | MIPICSI2_RX_VCINTRE_FEIE2(valFE)
                );
            regsPtr->RX[2].CBUF_LPDI = (uint8)numeLinesTrigger;
            break;
        case (uint32)CSI2_VC_3:
            /*
            * @section Csi2_Irq_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(
                &regsPtr->RX_VCINTRE,
                MIPICSI2_RX_VCINTRE_GNSPIE3_MASK | MIPICSI2_RX_VCINTRE_FSIE3_MASK | MIPICSI2_RX_VCINTRE_FEIE3_MASK,
                MIPICSI2_RX_VCINTRE_GNSPIE3(valFE) | MIPICSI2_RX_VCINTRE_FSIE3(valFS) | MIPICSI2_RX_VCINTRE_FEIE3(valFE)
                );
            regsPtr->RX[3].CBUF_LPDI = (uint8)numeLinesTrigger;
            break;
    }  /* switch    */
}
/* Csi2_SetVCEventIrq *************************/


/*================================================================================================*/
/*
 * @brief       Init for events interrupt.
 * @details     Init for events interrupt. All possibilities will be set.
 *
 * @param[in]   iUnit     - unit ID, RSDK_CSI2_UNIT_0 ... MAX
 * @param[in]   regsPtr   - pointer to unit registry
 * @param[in]   paramsPtr - pointer to CSI2 unit initialization parameters
 *
 */
static void Csi2_InitEventIrq(const Csi2_UnitIdType iUnit, volatile GENERIC_CSI2_Type *regsPtr,
                                     const Csi2_SetupParamsType *paramsPtr)
{
    uint32                      vcId;                       /* VC ID, from CSI2_VC_0 to MAX                         */
    uint8                       setLDevt;                   /* set LINEDONE event                                   */
    const Csi2_DriverParamsType *driverStatePtr;            /* pointer to unit driver state                         */

        driverStatePtr = &gCsi2Settings[(uint8)iUnit];

        /* irq enablement for skew calibration                                                                       */
        regsPtr->RX_VCINTRE = 0u;                       /* first disable the interrupts                              */
        setLDevt = 0u;                                  /* no LINEDONE request                                       */
        for (vcId = (uint32)CSI2_VC_0; vcId < (uint32)CSI2_MAX_VC; vcId++)  /* check all VC                          */
        {
            if (paramsPtr->vcConfigPtr[vcId] != (Csi2_VCParamsType*)NULL_PTR)   /* only for active VC                */
            {
                /* enable the interrupts for active VC, FE must be processed anyway                                  */
                Csi2_SetVCEventIrq(vcId, regsPtr,
                                (uint8)paramsPtr->vcConfigPtr[vcId]->vcEventsReq | (uint8)CSI2_EVT_FRAME_END,
                                paramsPtr->vcConfigPtr[vcId]->bufNumLinesTrigger);
            }
            else
            {
                Csi2_SetVCEventIrq(vcId, regsPtr, 0u, 0u);  /* disable the interrupts for inactive VC                */
            }
            setLDevt |= ((uint8)driverStatePtr->workingParamVC[vcId].eventsMask & (uint8)CSI2_EVT_LINE_END);
        }
        /* enable the "line done" interrupt, for internal purposes at least                                          */
        if (
#if (CSI2_AUTO_DC_COMPENSATION == STD_ON)
                (driverStatePtr->statisticsFlag == CSI2_AUTODC_EVERY_LINE) ||
#endif
                (setLDevt != 0u))
        {       /* process all chirps statistics            */
            /*
            * @section Csi2_Irq_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(&regsPtr->RX_CHNL_INTRE, MIPICSI2_RX_CHNL_INTRE_LINEDONEIE_MASK,
                                                        MIPICSI2_RX_CHNL_INTRE_LINEDONEIE(1u));
        }
        else
        {       /* do not process statistics at LINEDONE    */
            /*
            * @section Csi2_Irq_c_REF_1
            * Violates MISRA 2012 Advisory Directive 4.9,
            * #A function should be used in preference to a function-like macro where they are interchangeable
            * Using a macro produce a smaller code; there is a clear distinction between functions and macros,
            * which use only UPPERCASE
            */
            CSI2_SET_REGISTRY32(&regsPtr->RX_CHNL_INTRE, MIPICSI2_RX_CHNL_INTRE_LINEDONEIE_MASK, 0u);
        }
}
/* Csi2_InitEventIrq *************************/


/*==================================================================================================
*                                         GLOBAL FUNCTIONS
==================================================================================================*/
#if !defined(linux)
/*================================================================================================*/
/*
 * @brief       Interrupt handler for unit 0/ PHY error irq.
 *
 */
static
void Csi2_IrqHandlerRxErr0(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
{
#if defined(__ZEPHYR__)
    (void)paramsPtr;
#endif
    Csi2_IrqHandlerRxErr(CSI2_UNIT_0);
}
/* Csi2_IrqHandlerRxErr0 *************************/

/*================================================================================================*/
/*
 * @brief       Interrupt handler for unit 1/ PHY error irq.
 *
 */
    static
    void Csi2_IrqHandlerRxErr1(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
    {
#if defined(__ZEPHYR__)
        (void)paramsPtr;
#endif
        Csi2_IrqHandlerRxErr(CSI2_UNIT_1);
    }
/* Csi2_IrqHandlerRxErr1 *************************/

/*================================================================================================*/
/*
 * @brief       Interrupt handler for unit 2/ PHY error irq.
 *
 */
static void Csi2_IrqHandlerRxErr2(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
{
#if defined(__ZEPHYR__)
    (void)paramsPtr;
#endif
    Csi2_IrqHandlerRxErr(CSI2_UNIT_2);
}
/* Csi2_IrqHandlerRxErr2 *************************/

/*================================================================================================*/
/*
 * @brief       Interrupt handler for unit 3/ PHY error irq.
 *
 */
static void Csi2_IrqHandlerRxErr3(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
{
#if defined(__ZEPHYR__)
    (void)paramsPtr;
#endif
    Csi2_IrqHandlerRxErr(CSI2_UNIT_3);
}
/* Csi2_IrqHandlerRxErr3 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 0/ errors in protocol & packet level irq.
 *
 */
static
void Csi2_IrqHandlerPathErr0(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
{
#if defined(__ZEPHYR__)
    (void)paramsPtr;
#endif
    Csi2_IrqHandlerPathErr(CSI2_UNIT_0);
}
/* Csi2_IrqHandlerPathErr0 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 1/ errors in protocol & packet level irq.
 *
 */
    static
    void Csi2_IrqHandlerPathErr1(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
    {
#if defined(__ZEPHYR__)
        (void)paramsPtr;
#endif
        Csi2_IrqHandlerPathErr(CSI2_UNIT_1);
    }
/* Csi2_IrqHandlerPathErr1 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 2/ errors in protocol & packet level irq.
 *
 */
static void Csi2_IrqHandlerPathErr2(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
{
#if defined(__ZEPHYR__)
    (void)paramsPtr;
#endif
    Csi2_IrqHandlerPathErr(CSI2_UNIT_2);
}
/* Csi2_IrqHandlerPathErr2 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 3/ errors in protocol & packet level irq.
 *
 */
static void Csi2_IrqHandlerPathErr3(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
{
#if defined(__ZEPHYR__)
    (void)paramsPtr;
#endif
    Csi2_IrqHandlerPathErr(CSI2_UNIT_3);
}
/* Csi2_IrqHandlerPathErr3 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 0/ events irq.
 *
 */
static
void Csi2_IrqHandlerEvents0(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
{
#if defined(__ZEPHYR__)
    (void)paramsPtr;
#endif
    Csi2_IrqHandlerEvents(CSI2_UNIT_0);
}
/* Csi2_IrqHandlerEvents0 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 1/ events irq.
 *
 */
    static
    void Csi2_IrqHandlerEvents1(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
    {
#if defined(__ZEPHYR__)
        (void)paramsPtr;
#endif
        Csi2_IrqHandlerEvents(CSI2_UNIT_1);
    }
/* Csi2_IrqHandlerEvents1 *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 2/ events irq.
 *
 */
static void Csi2_IrqHandlerEvents2(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
{
#if defined(__ZEPHYR__)
    (void)paramsPtr;
#endif
    Csi2_IrqHandlerEvents(CSI2_UNIT_2);
}
/* Csi2_IrqHandlerEvents *************************/

/*================================================================================================*/
/*
 * @brief       Irq handler for unit 3/ events irq.
 *
 */
static void Csi2_IrqHandlerEvents3(
#if defined(__ZEPHYR__)
        const void * paramsPtr
#else
        void
#endif
        )
{
#if defined(__ZEPHYR__)
    (void)paramsPtr;
#endif
    Csi2_IrqHandlerEvents(CSI2_UNIT_3);
}
/* Csi2_IrqHandlerEvents3 *************************/
#endif /* !defined(linux)    */

/*================================================================================================*/
/*
 * @brief       Initialize interrupts at Unit level
 *
 * @param[in]   iUnit     - unit ID, RSDK_CSI2_UNIT_0 ... MAX
 * @param[in]   regsPtr   - pointer to unit registry
 * @param[in]   paramsPtr - pointer to CSI2 unit initialization parameters
 *
 */
rsdkStatus_t
Csi2_SetupUIrq(const Csi2_UnitIdType iUnit, volatile GENERIC_CSI2_Type *regsPtr,
                          const Csi2_SetupParamsType *initParamsPtr)
{
    Csi2_IsrCbType              *pDriverStateCallbacks;          /* pointer to callbacks                        */
    rsdkStatus_t                rez = RSDK_SUCCESS;

    pDriverStateCallbacks = (Csi2_IsrCbType*)gCsi2Settings[(uint8)iUnit].pCallback;
#if !defined(linux)
    if (RsdkGlueIrqHandlerRegister(sgIrqHandlers[iUnit][RSDK_CSI2_EVENTS_IRQ_ID],
                                   (uint32_t)CSI2_IRQ_EVENT_BASE_ID_GIC +
                                       ((uint32_t)CSI2_IRQ_MAP_GAP * gsCsi2IrqUnitRemap[iUnit]),
                                       initParamsPtr->irqExecCore, initParamsPtr->irqPriority) != IRQ_REGISTER_SUCCESS)
    {
        rez = RSDK_CSI2_DRV_ERR_IRQ_HANDLER_REG;
    }

    if ((rez == RSDK_SUCCESS) && (RsdkGlueIrqHandlerRegister(sgIrqHandlers[(uint8)iUnit][RSDK_CSI2_RX_ERR_IRQ_ID],
                                   (uint32_t)CSI2_IRQ_RX_BASE_ID_GIC +
                                       ((uint32_t)CSI2_IRQ_MAP_GAP * gsCsi2IrqUnitRemap[(uint8)iUnit]),
                                       initParamsPtr->irqExecCore, initParamsPtr->irqPriority) != IRQ_REGISTER_SUCCESS))
    {
        rez = RSDK_CSI2_DRV_ERR_IRQ_HANDLER_REG;
    }
    if ((rez == RSDK_SUCCESS) && (RsdkGlueIrqHandlerRegister(sgIrqHandlers[(uint8)iUnit][RSDK_CSI2_PATH_ERR_IRQ_ID],
                                   (uint32_t)CSI2_IRQ_PATH_BASE_ID_GIC +
                                       ((uint32_t)CSI2_IRQ_MAP_GAP * gsCsi2IrqUnitRemap[(uint8)iUnit]),
                                       initParamsPtr->irqExecCore, initParamsPtr->irqPriority) != IRQ_REGISTER_SUCCESS))
    {
        rez = RSDK_CSI2_DRV_ERR_IRQ_HANDLER_REG;
    }

    if(rez == RSDK_SUCCESS)
    {
#endif

        /* save the callbacks                                                                                       */
        *pDriverStateCallbacks = initParamsPtr->pCallback[RSDK_CSI2_RX_ERR_IRQ_ID];
        pDriverStateCallbacks++;
        *pDriverStateCallbacks = initParamsPtr->pCallback[RSDK_CSI2_PATH_ERR_IRQ_ID];
        pDriverStateCallbacks++;
        *pDriverStateCallbacks = initParamsPtr->pCallback[RSDK_CSI2_EVENTS_IRQ_ID];

        Csi2_InitEventIrq(iUnit, regsPtr, initParamsPtr);
        Csi2_InitPathIrq(regsPtr);
        Csi2_InitRxIrq(regsPtr, initParamsPtr);

#if !defined(linux)
    }
#endif
    return rez;
}
/* Csi2InitUIrq *************************/


#ifdef __cplusplus
}
#endif

/** @} */




