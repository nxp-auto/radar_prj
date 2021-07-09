/*
 * Copyright 2016-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RSDK_CSI2_IRQ_MANAGEMENT_H
#define RSDK_CSI2_IRQ_MANAGEMENT_H

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "csi2_driver_platform_specific.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                          CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 *                                      DEFINES AND MACROS
 ==================================================================================================*/
// clang-format off
#ifdef PLATFORM_VDK
#define CSI2_IRQ_RX_BASE_ID_GIC         205         // irq number for CSI2 Rx error interrupt, unit 0
#define CSI2_IRQ_PATH_BASE_ID_GIC       206         // irq number for CSI2 Path error interrupt, unit 0
#define CSI2_IRQ_EVENT_BASE_ID_GIC      204         // irq number for CSI2 events interrupt, unit 0
#define CSI2_IRQ_TX_BASE_ID_GIC         207         // irq number for CSI2 Tx error interrupt, unit 0
#elif defined(S32R45)
#define CSI2_IRQ_RX_BASE_ID_GIC         204         // irq number for CSI2 Rx error interrupt, unit 0
#define CSI2_IRQ_PATH_BASE_ID_GIC       205         // irq number for CSI2 Path error interrupt, unit 0
#define CSI2_IRQ_EVENT_BASE_ID_GIC      206         // irq number for CSI2 events interrupt, unit 0
#define CSI2_IRQ_TX_BASE_ID_GIC         207         // irq number for CSI2 Tx error interrupt, unit 0
#elif defined(S32R294)
#define CSI2_IRQ_RX_BASE_ID_GIC         758         // irq number for CSI2 Rx error interrupt, unit 0
#define CSI2_IRQ_PATH_BASE_ID_GIC       759         // irq number for CSI2 Path error interrupt, unit 0
#define CSI2_IRQ_EVENT_BASE_ID_GIC      760         // irq number for CSI2 events interrupt, unit 0
#define CSI2_IRQ_TX_BASE_ID_GIC         761         // irq number for CSI2 Tx error interrupt, unit 0
#endif // #ifdef PLATFORM_VDK

#define CSI2_IRQ_RX_BASE_ID             0           // irq number for CSI2 Rx error interrupt, in unit irq array
#define CSI2_IRQ_PATH_BASE_ID           1           // irq number for CSI2 Path error interrupt, in unit irq array
#define CSI2_IRQ_EVENT_BASE_ID          2           // irq number for CSI2 events interrupt, in unit irq array
#define CSI2_IRQ_TX_BASE_ID             3           // irq number for CSI2 Tx error interrupt, in unit irq array

// masks for unit DPHY errors
#define CSI2_IRQ_GENERIC_PHY_ERR_CTRL  0x00084210u  // all ctrl errors
#define CSI2_IRQ_GENERIC_PHY_ERR_ESYN  0x00042108u  // all esc sync errors
#define CSI2_IRQ_GENERIC_PHY_ERR_ESC   0x00021084u  // all esc errors
#define CSI2_IRQ_GENERIC_PHY_ERR_NSYN  0x00010842u  // all multibit sync errors
#define CSI2_IRQ_GENERIC_PHY_ERR_SYN   0x00008421u  // all sync pattern errors

// masks for VC PnP errors
#define CSI2_IRQ_VC_ECC1_MASK           0x01u       // ECC one bit error
#define CSI2_IRQ_VC_ECC2_MASK           0x02u       // ECC two bits error
#define CSI2_IRQ_VC_FSYN_MASK           0x04u       // frame sync error
#define CSI2_IRQ_VC_FDAT_MASK           0x08u       // frame data error (==CRC)
#define CSI2_IRQ_VC_CRC_MASK            0x10u       // CRC error
#define CSI2_IRQ_VC_IVID_MASK           0x20u       // invalid frame ID

// masks for VC data path errors
#define CSI2_IRQ_VC_LIN0LENERR_MASK     1           // line length errors for VC 0 only
#define CSI2_IRQ_VC_LIN0CNTERR_MASK     2           // line count errors for VC 0 only

// masks for VC events
#define CSI2_IRQ_VC_GNSP_MASK           0x0924u     // generic mask for "generic short packet"
#define CSI2_IRQ_VC_FEND_MASK           0x0492u     // generic mask for "frame end"
#define CSI2_IRQ_VC_FSTT_MASK           0x0249u     // generic mask for "frame start"

#define CSI2_IRQ_EVT_NEXT_START_NOT_0   0x80000000u // internal mask for not aligned frame start

// elements for statistics
#ifdef PLATFORM_VDK
#define CSI2_STAT_CHANNEL_SUM_ADJUST    0x1         // divider for the channel samples sum
#define CSI2_STAT_DC_OFFSET_ADJUST      0x1         // multiplier for the channel statistic value
#else
#define CSI2_STAT_CHANNEL_SUM_ADJUST    0x1         // divider for the channel samples sum
#define CSI2_STAT_DC_OFFSET_ADJUST      0x4         // multiplier for the channel statistic value
#endif

#ifdef PLATFORM_VDK
#define CSI2_TOGGLE_BITS_MASK           0xfffu       // mask for toggle bits
#else
#define CSI2_TOGGLE_BITS_MASK           0xfff0u      // mask for toggle bits
#endif
#define CSI2_FE_MASK_VC_0               2u          // mask for FE flag on VC 0
#define CSI2_VC_NUM_TOTALBITS           3u          // bits length for VC number
#define CSI2_VC_NUM_BITSMASK            0x7u        // mask for VC number (only the lowest part)

// clang-format on

/*==================================================================================================
*                                         EXTERN DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/
/**
 * @brief       Initialize interrupts at Unit level
 *
 * @param[in]   iUnit   - unit ID, RSDK_CSI2_UNIT_1 ... MAX
 * @param[in]   pRegs   - pointer to unit registry
 * @param[in]   pParams - pointer to CSI2 unit initialization parameters
 *
 */
rsdkStatus_t Csi2InitUIrq(const rsdkCsi2UnitId_t iUnit, volatile struct MIPICSI2_REG_STRUCT *pRegs,
                          const rsdkCsi2InitParams_t *pInitParams);

#ifdef linux
/*================================================================================================*/
/**
 * @brief       Generic interrupt procedure for events irq.
 *
 * @param[in]   iUnit   - unit id, 0...3
 *
 */
extern void Csi2IrqHandlerEvents(const rsdkCsi2UnitId_t iUnit);

/*================================================================================================*/
/**
 * @brief       Generic interrupt procedure for errors in protocol & packet level irq.
 *
 * @param[in]   iUnit   - unit id, RSDK_CSI2_UNIT_1 ... MAX
 *
 */
extern void Csi2IrqHandlerPathErr(const rsdkCsi2UnitId_t iUnit);

/*================================================================================================*/
/**
 * @brief       Generic interrupt procedure for PHY error irq.
 *
 * @param[in]   iUnit   - unit id, RSDK_CSI2_UNIT_1 ... MAX
 *
 */
extern void Csi2IrqHandlerRxErr(const rsdkCsi2UnitId_t iUnit);

#endif  // linux

#endif /* RSDK_CSI2M_IRQ_H_ */

#ifdef __cplusplus
}
#endif
