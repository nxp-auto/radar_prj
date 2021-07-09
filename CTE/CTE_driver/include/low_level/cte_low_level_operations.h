/*
* Copyright 2020 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CTE_LOW_LEVEL_OPERATIONS_H
#define CTE_LOW_LEVEL_OPERATIONS_H

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#include "rsdk_cte_driver_api.h"
#ifdef S32R45
#include "rsdk_S32R45.h"
#elif defined(S32R294)
#include "rsdk_S32R294.h"
#else
#error "Working platform not defined !"
#endif
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include "typedefs.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                          CONSTANTS
 ==================================================================================================*/
#define CTE_MAX_LARGE_TIME_TABLE_LEN    64u     // maximum length for single time table usage
#define CTE_MAX_SMALL_TIME_TABLE_LEN    32u     // maximum length for double time table usage (single table)


#ifdef S32R294
#define CTE_IRQ_NUMBER      739u            // hardware interrupt number
#elif defined(S32R45)
#define CTE_IRQ_NUMBER      263u            // hardware interrupt number
#else
#error "Working platform not defined !"
#endif


/*==================================================================================================
 *                                      DEFINES AND MACROS
 ==================================================================================================*/
#define CTE_CLOCK_DIVIDER_LIMIT     0x40u       // the clock divider limit (the value must be less than this value)
#define CTE_TOO_BIG_TIME_DELAY      0xffffffffu // a time delay which exceed the maximum admisible
#define CTE_MAX_TIME_COUNTER        0x10000u    // the internal timecounter limit for CTE events
#define CTE_1G_FREQUENCY            1000000000u // 1GHz frequency

#define CTE_SPT0_SIG_MASK           0x10000000LU    // mask for SPT0 signal
#define CTE_FLEX_SIG_MASK           ((uint64_t)0x1LU << 33u)  // mask for first FLEX signal
#define CTE_OUTPUT_MASK_SHIFT_BASE  32u             // the minimum shift for the output signal mask

#define CTE_INTERNAL_CLOCKS         4u              // CTE has 4 internal clock dividers
#define CTE_MAX_REQ_CLK_DIVIDER     192u            // This assume 128 is an acceptable divider replacer for 192
#define CTE_MAX_CLK_DIVIDERS        8u              // the number of possible dividers for clocks

/*==================================================================================================
 *                                             ENUMS
 ==================================================================================================*/
// enum for toggle signal manipulations
typedef enum {
    CTE_TOGGLE_MASK_TO_LOW = 0u,
    CTE_TOGGLE_MASK_TO_HIGH,
    CTE_TOGGLE_MASK_TOGGLE,
    CTE_TOGGLE_MASK_UNCHANGED,
}cteToggleMask_t;

// enum for clock signal manipulations
typedef enum {
    CTE_CLOCK_MASK_TO_LOW = 0u,
    CTE_CLOCK_MASK_SYNC_RISING,
    CTE_CLOCK_MASK_RUNNING,
    CTE_CLOCK_MASK_TO_HIGH,
}cteClockMask_t;

// enum for logic signal manipulations
typedef enum {
    CTE_LOGIC_MASK_TO_LOW = 0u,
    CTE_LOGIC_MASK_TO_HIGH,
    CTE_LOGIC_MASK_TO_HIZ,
    CTE_LOGIC_MASK_UNCHANGED,
}cteLogicMask_t;

// enum for the driver states
typedef enum {
    CTE_DRIVER_STATE_NOT_INIT = 0u,
    CTE_DRIVER_STATE_INITIALIZED,
    CTE_DRIVER_STATE_RUNNING,
}cteDriverStatus_t;


/*==================================================================================================
 *                                STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
// Structure to keep the necessary data for low-level driver
typedef struct {
    uint8_t     cteDriverStatus;        // the current status of the driver
    uint8_t     cteMainClockDivider;    // the main divider, for main CTE clock divider
    uint8_t     cteUsedClockDividers;   // the number of used clocks
    uint32_t    cteWorkingFreq;         // the CTE working frequency, in Hz
    uint32_t    cteReqEvents;           // the CTE events requested by application to be signaled
    rsdkCteIsrCb_t pCteCallback;        // the application callback to be used for the requested events
    uint32_t    cteClocksPeriods[CTE_INTERNAL_CLOCKS];  // resulting clock period
    rsdkCteSingleOutputDef_t    pSignalDef0[RSDK_CTE_OUTPUT_MAX + 1u];   // copy of the existing signals definitions
    rsdkCteSingleOutputDef_t    pSignalDef1[RSDK_CTE_OUTPUT_MAX + 1u];
} cteDriverState_t;

/*==================================================================================================
 *                                GLOBAL VARIABLE DECLARATIONS
 ==================================================================================================*/

/*==================================================================================================
 *                                    FUNCTION PROTOTYPES
 ==================================================================================================*/
/**
 * @brief   Low level initialization procedure for CTE driver
 * @details If called, the CTE function is stopped.
 *          After initialization the CTE is not started, a specific RsdkCteStart call must be used for this.
 *          The operation can be done at any moment; if the CTE is working, it will be stopped.
 *
 * @param[in]   pCteInitParams  = pointer to the initialization structure
 * @param[in]   pLutChecksum    = pointer to a uint64_t value, which will receive the final LUT checksum;
 *                                  this value can be checked later using RsdkCteGetLutChecksum
 * @return      RSDK_SUCCESS    = initialization succeeded
 *              other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleInit(const rsdkCteInitParams_t *pCteInitParams, uint64_t *pLutChecksum);

/**
 * @brief   Low level start procedure for CTE
 * @details After this call the CTE will start to work, if successful.
 *          The procedure could be unsuccessful only if the driver was not initialized before.
 *          Start after a Stop will use the previous initialization.
 *
 * @return  RSDK_SUCCESS    = start succeeded
 *          other values    = start failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleStart(void);

/**
 * @brief   Low level stop procedure for CTE
 * @details After this call the CTE will stop to work, if successful.
 *          The procedure could be unsuccessful only if the driver is not working at the call time.
 *
 * @return  RSDK_SUCCESS    = stop succeeded
 *          other values    = stop failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleStop(void);

/**
 * @brief   Low level restart CTE.
 * @details This procedure is a single call for RsdkCteStop and a RsdkCteStart.
 *
 * @param[out]  RSDK_SUCCESS    = restart succeeded
 *              other values    = restart failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleRestart(void);

/**
 * @brief   Generate a RFS software signal.
 * @details The procedure can be used only in Slave mode, to reset the time table execution.
 *          The real CTE execution must be triggered by a RCS signal.
 *
 * @param[out]  RSDK_SUCCESS    = call succeeded
 *              other values    = call failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleRfsGenerate(void);

/**
 * @brief   Low level procedure to update only the timing tables.
 * @details The procedure can be used only after a previous successful CTE initialization.
 *          If the CTE is working, it will be stopped and restarted after table changed.
 *          If stopped, it will remains in the same state. It is recommendable to do like this.
 *
 * @param[in]   pTable_         = pointer to the new table(s); first pointer must not be NULL;
                                if second is NULL, only one table used, else two tables used
 * @param[in]   pLutChecksum    = pointer to a uint64_t value, which will receive the final LUT checksum;
 *                                  this value can be checked later using RsdkCteGetLutChecksum
 * @param[out]  RSDK_SUCCESS    = initialization succeeded
 *              other values    = initialization failed; the rsdk_stat.h contains the values and explanations
 *
 */
rsdkStatus_t CtePlatformModuleUpdateTables(rsdkCteTimeTableDef_t *pTable0, rsdkCteTimeTableDef_t *pTable1,
        uint64_t *pLutChecksum);

/**
 * @brief   Get the checksum of the timing LUT.
 * @details This procedure returns the current checksum reported by the hardware, only 40 bits.
 *          The value can be compared to the previous values.
 *
 * @return  LUT checksum
 *
 */
uint64_t CtePlatformModuleGetLutChecksum(void);

/**
 * @brief   Interrupt handler for CTE.
 * @details Low level interrupt handler for CTE driver.
 *
 */
void CteIrqHandler(void);


#ifdef __cplusplus
}
#endif



#endif // CTE_LOW_LEVEL_OPERATIONS_H
