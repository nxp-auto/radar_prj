/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CDD_CTE_H
#define CDD_CTE_H


/**
*   @file
*   @implements CDD_CDD_Cte.h_Artifact
*
*   @addtogroup CTE_ASR
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*
* @page misra_violations MISRA-C:2012 violations
*
* @section Cte_h_REF_1
* Violates MISRA 2012 Advisory Rule 20.1, #Include directives should only be preceded by preprocessor directives or comments.
* <MA>_MemMap.h is included after each section define in order to set the current memory section as defined by AUTOSAR.
*/


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Cte_Cfg.h"
#include "Cte_Types.h"
#include "Cte_Irq.h"

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
* @brief Development error codes (passed to DET).
*/

    /**
    * @brief API request called with an invalid parameter (Nullpointer).
    * */
    #define CTE_E_PARAM_POINTER                ((uint8)0x01U)

    /**
    * @brief API request called with invalid parameter (invalid value).
    * */
    #define CTE_E_PARAM_VALUE                  ((uint8)0x02U)

    /**
    * @brief API request called with invalid parameter (out of range).
    * */
    #define CTE_E_PARAM_HANDLE                 ((uint8)0x03U)

    /**
    * @brief Setup of Cte Driver failed.
    * */
    #define CTE_E_SETUP_FAILED                 ((uint8)0x04U)

    /**
    * @brief Incorrect driver status.
    * */
    #define CTE_E_WRONG_STATE                  ((uint8)0x05U)

    /**
    * @brief Hardware error.
    * */
    #define CTE_E_HW_ERROR                     ((uint8)0x06U)


/* update a MIPI-CTE 32 bits registry                              */
#define CTE_SET_REGISTRY32(registryPtr, alignedMask, alignedValue) \
                *(registryPtr) = (((*(registryPtr)) & (~((uint32)alignedMask))) | ((uint32)alignedValue))

/* update a MIPI-CTE 32 bits registry                              */
#define CTE_GET_REGISTRY32(registryPtr, alignedMask, shiftValue) \
                (((*(registryPtr)) & (((uint32)alignedMask))) >> ((uint32)shiftValue))


/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

/**
 * @brief   Initialization procedure for CTE driver
 * @details After initialization the CTE is not started, a specific Cte_Start call must be used for this.
 *          The operation can be done at any moment; if the CTE is working, it will be stopped.
 *
 * @param[in]   pCteInitParams    = pointer to the initialization structure
 * @param[in]   pLutChecksum      = pointer to a uint64 value, which will receive the final LUT checksum;
 *                                  this value can be checked later using Cte_GetLutChecksum
 * @return      E_OK/RSDK_SUCCESS = initialization succeeded
 * @return      other values      = initialization failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_Setup(const Cte_SetupParamsType *pCteInitParams, uint64 *pLutChecksum);

/**
 * @brief   Start procedure for CTE
 * @details After this call the CTE will start to work, if successful.
 *          The procedure could be unsuccessful only if the driver was not initialized before. 
 *          The exit status can be different than E_OK/RSDK_SUCCESS; if the unit is running, the return is not OK
 *          but it is only a warning, signaling the CTE was already working and this was not interrupt.
 *          Start after a Stop will use the initialization done before.
 * 
 * @return  E_OK/RSDK_SUCCESS = start succeeded
 * @return  other values      = start failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_Start(void);

/**
 * @brief   Stop procedure for CTE
 * @details After this call the CTE will stop to work, if successful.
 *          The procedure could be unsuccessful only if the driver is not working at the call time.
 * 
 * @return  E_OK/RSDK_SUCCESS = stop succeeded
 * @return  other values      = stop failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_Stop(void);

/**
 * @brief   Restart CTE.
 * @details This procedure is a single call for Cte_Stop and a Cte_Start.
 *
 * @return  E_OK/RSDK_SUCCESS = restart succeeded
 * @return  other values      = restart failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_Restart(void);

/**
 * @brief   Generate a RFS software signal.
 * @details The procedure can be used only in Slave mode, to reset the time table execution.
 *          The real CTE execution must be triggered by a RCS signal.
 *
 * @return  E_OK/RSDK_SUCCESS = call succeeded
 * @return  other values      = call failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_RfsGenerate(void);

/**
 * @brief   Procedure to update only the existing timing tables.
 * @details The procedure can be used only after a previous successful CTE initialization.
 *          If the CTE is working, it will be stopped and restarted after table changed.
 *          If stopped, it will remains in the same state. It is recommendable to do like this.
 *
 * @param[in]   pTable_         = pointer to the new table(s); first pointer must not be NULL; 
                                if second is NULL, only one table used, else two tables used
 * @param[in]   pLutChecksum    = pointer to a uint64 value, which will receive the final LUT checksum;
 *                                  this value can be checked later using Cte_GetLutChecksum
 * @return  E_OK/RSDK_SUCCESS   = initialization succeeded
 * @return  other values        = initialization failed, use the appropriate tools to detect the issue
 *
 */
Std_ReturnType Cte_UpdateTables(Cte_TimeTableDefType *pTable0, Cte_TimeTableDefType *pTable1,
        uint64 *pLutChecksum);

/**
 * @brief   Get the checksum of the timing LUT.
 * @details This procedure returns the current checksum reported by the hardware, only 40 bits.
 *          The value can be compared to the previous values.
 *
 * @return  LUT checksum
 *
 */
uint64 Cte_GetLutChecksum(void);


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CDD_CTE_H */
