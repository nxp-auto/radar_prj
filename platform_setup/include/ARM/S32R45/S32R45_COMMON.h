/*
** ###################################################################
**     Processor:           S32R45_A53
**     Compiler:            Keil ARM C/C++ Compiler
**     Reference manual:    S32R45 RM Rev.3
**     Version:             rev. 2.4, 2022-02-14
**     Build:               b220214
**
**     Abstract:
**         Peripheral Access Layer for S32R45_A53
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2022 NXP
**     SPDX-License-Identifier: BSD-3-Clause                                  *
**
**     NXP Confidential. This software is owned or controlled by NXP and may only be
**     used strictly in accordance with the applicable license terms. By expressly
**     accepting such terms or by downloading, installing, activating and/or otherwise
**     using the software, you are agreeing that you have read, and that you agree to
**     comply with and are bound by, such license terms. If you do not agree to be
**     bound by the applicable license terms, then you may not retain, install,
**     activate or otherwise use the software. The production use license in
**     Section 2.3 is expressly granted for this software.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
** ###################################################################
*/

/*!
 * @file S32R45_A53.h
 * @version 2.4
 * @date 2022-02-14
 * @brief Peripheral Access Layer for S32R45_A53
 *
 * This file contains register definitions and macros for easy access to their
 * bit fields.
 *
 * This file assumes LITTLE endian system.
 */

/**
* @page misra_violations MISRA-C:2012 violations
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 2.3, local typedef not referenced
* The SoC header defines typedef for all modules.
*
* @section [global]
* Violates MISRA 2012 Advisory Rule 2.5, local macro not referenced
* The SoC header defines macros for all modules and registers.
*
* @section [global]
* Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
* These are generated macros used for accessing the bit-fields from registers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.1, identifier clash
* The supported compilers use more than 31 significant characters for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.2, identifier clash
* The supported compilers use more than 31 significant characters for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.4, identifier clash
* The supported compilers use more than 31 significant characters for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.5, identifier clash
* The supported compilers use more than 31 significant characters for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 21.1, defined macro '__I' is reserved to the compiler
* This type qualifier is needed to ensure correct I/O access and addressing.
*/


/* ----------------------------------------------------------------------------
   -- MCU activation
   ---------------------------------------------------------------------------- */

/* Prevention from multiple including the same memory map */
#if !defined(S32R45_COMMON_H_)  /* Check if memory map has not been already included */
#define S32R45_COMMON_H_
#define MCU_S32R45

/* Check if another memory map has not been also included */
#if (defined(MCU_ACTIVE))
  #error S32R45_A53 memory map: There is already included another memory map. Only one memory map can be included.
#endif /* (defined(MCU_ACTIVE)) */
#define MCU_ACTIVE

#include "typedefs.h"

/** Memory map major version (memory maps with equal major version number are
 * compatible) */
#define MCU_MEM_MAP_VERSION 0x0200U
/** Memory map minor version */
#define MCU_MEM_MAP_VERSION_MINOR 0x0004U

/* ----------------------------------------------------------------------------
   -- Generic macros
   ---------------------------------------------------------------------------- */

/* IO definitions (access restrictions to peripheral registers) */
/**
*   IO Type Qualifiers are used
*   \li to specify the access to peripheral variables.
*   \li for automatic generation of peripheral register debug information.
*/
#ifndef __IO
#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions                 */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions                */
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */
#endif


/**
* @brief 32 bits memory read macro.
*/
#if !defined(REG_READ32)
  #define REG_READ32(address)               (*(volatile uint32_t*)(address))
#endif

/**
* @brief 32 bits memory write macro.
*/
#if !defined(REG_WRITE32)
  #define REG_WRITE32(address, value)       ((*(volatile uint32_t*)(address))= (uint32_t)(value))
#endif

/**
* @brief 32 bits bits setting macro.
*/
#if !defined(REG_BIT_SET32)
  #define REG_BIT_SET32(address, mask)      ((*(volatile uint32_t*)(address))|= (uint32_t)(mask))
#endif

/**
* @brief 32 bits bits clearing macro.
*/
#if !defined(REG_BIT_CLEAR32)
  #define REG_BIT_CLEAR32(address, mask)    ((*(volatile uint32_t*)(address))&= ((uint32_t)~((uint32_t)(mask))))
#endif

/**
* @brief 32 bit clear bits and set with new value
* @note It is user's responsability to make sure that value has only "mask" bits set - (value&~mask)==0
*/
#if !defined(REG_RMW32)
  #define REG_RMW32(address, mask, value)   (REG_WRITE32((address), ((REG_READ32(address)& ((uint32_t)~((uint32_t)(mask))))| ((uint32_t)(value)))))
#endif


/* ----------------------------------------------------------------------------
   -- Interrupt vector numbers
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Interrupt_vector_numbers Interrupt vector numbers
 * @{
 */

/** Interrupt Number Definitions */
#define NUMBER_OF_INT_VECTORS 272                /**< Number of interrupts in the Vector table */

typedef enum {
  /* Auxiliary constants */
  NotAvail_IRQn                = -128,             /**< Not available device specific interrupt */

  /* Core interrupts */
  Software0_IRQn               = 0,                /**< Cortex-A53 Software Generated Interrupt 0 */
  Software1_IRQn               = 1,                /**< Cortex-A53 Software Generated Interrupt 1 */
  Software2_IRQn               = 2,                /**< Cortex-A53 Software Generated Interrupt 2 */
  Software3_IRQn               = 3,                /**< Cortex-A53 Software Generated Interrupt 3 */
  Software4_IRQn               = 4,                /**< Cortex-A53 Software Generated Interrupt 4 */
  Software5_IRQn               = 5,                /**< Cortex-A53 Software Generated Interrupt 5 */
  Software6_IRQn               = 6,                /**< Cortex-A53 Software Generated Interrupt 6 */
  Software7_IRQn               = 7,                /**< Cortex-A53 Software Generated Interrupt 7 */
  Software8_IRQn               = 8,                /**< Cortex-A53 Software Generated Interrupt 8 */
  Software9_IRQn               = 9,                /**< Cortex-A53 Software Generated Interrupt 9 */
  Software10_IRQn              = 10,               /**< Cortex-A53 Software Generated Interrupt 10 */
  Software11_IRQn              = 11,               /**< Cortex-A53 Software Generated Interrupt 11 */
  Software12_IRQn              = 12,               /**< Cortex-A53 Software Generated Interrupt 12 */
  Software13_IRQn              = 13,               /**< Cortex-A53 Software Generated Interrupt 13 */
  Software14_IRQn              = 14,               /**< Cortex-A53 Software Generated Interrupt 14 */
  Software15_IRQn              = 15,               /**< Cortex-A53 Software Generated Interrupt 15 */
  VirtualMaintenance_IRQn      = 25,               /**< Cortex-A53 Virtual Maintenance Interrupt */
  HypervisorTimer_IRQn         = 26,               /**< Cortex-A53 Hypervisor Timer Interrupt */
  VirtualTimer_IRQn            = 27,               /**< Cortex-A53 Virtual Timer Interrupt */
  LegacyFastInt_IRQn           = 28,               /**< Cortex-A53 Legacy nFIQ signal Interrupt */
  SecurePhyTimer_IRQn          = 29,               /**< Cortex-A53 Secure Physical Timer Interrupt */
  NonSecurePhyTimer_IRQn       = 30,               /**< Cortex-A53 Non-secure Physical Timer Interrupt */
  LegacyIRQ_IRQn               = 31,               /**< Cortex-A53 Legacy nIRQ Interrupt */

  /* Device specific interrupts */
  Pcie_1_MSI_IRQn              = 32,               /**< PCIe_1 MSI interrupt to M7/A53 */
  INT0_IRQn                    = 33,               /**< Software generated CPU interrupt to A53 core - interrupt#0 */
  INT1_IRQn                    = 34,               /**< Software generated CPU interrupt to A53 core - interrupt#1 */
  INT2_IRQn                    = 35,               /**< Software generated CPU interrupt to A53 core - interrupt#2 */
  Pcie_0_MSI_IRQn              = 36,               /**< PCIe_0 MSI interrupt to M7/A53 */
  CTI_0_IRQn                   = 37,               /**< CTI interrupt[0] */
  CTI_1_IRQn                   = 38,               /**< CTI interrupt[1] */
  MCM_IRQn                     = 39,               /**< Cortex M7 interrupt for FPU events and TCM errors */
  DMA0_0_15_IRQn               = 40,               /**< eDMA0 - DMA interrupt CH0-15 */
  DMA0_16_31_IRQn              = 41,               /**< eDMA0 - DMA interrupt CH16-31 */
  DMA0_ERR0_IRQn               = 42,               /**< eDMA0 - DMA error interrupt */
  DMA1_0_15_IRQn               = 43,               /**< eDMA1 - DMA interrupt  CH0-15 */
  DMA1_16_31_IRQn              = 44,               /**< eDMA1 - DMA interrupt CH16-31 */
  DMA1_ERR0_IRQn               = 45,               /**< eDMA1 - DMA error interrupt */
  SWT0_IRQn                    = 46,               /**< Watchdog timer interrupt */
  SWT1_IRQn                    = 47,               /**< Watchdog timer interrupt */
  SWT2_IRQn                    = 48,               /**< Watchdog timer interrupt */
  SWT3_IRQn                    = 49,               /**< Watchdog timer interrupt */
  SWT4_IRQn                    = 50,               /**< Watchdog timer interrupt */
  SWT5_IRQn                    = 51,               /**< Watchdog timer interrupt */
  SWT6_IRQn                    = 52,               /**< Watchdog timer interrupt */
  SWT7_IRQn                    = 53,               /**< Watchdog timer interrupt */
  STM0_IRQn                    = 56,               /**< STM0 interrupt channel request 0, 1, 2, 3 */
  STM1_IRQn                    = 57,               /**< STM1 interrupt channel request 0, 1, 2, 3 */
  STM2_IRQn                    = 58,               /**< STM2 interrupt channel request 0, 1, 2, 3 */
  STM3_IRQn                    = 59,               /**< STM3 interrupt channel request 0, 1, 2, 3 */
  STM4_IRQn                    = 60,               /**< STM4 interrupt channel request 0, 1, 2, 3 */
  STM5_IRQn                    = 61,               /**< STM5 interrupt channel request 0, 1, 2, 3 */
  STM6_IRQn                    = 62,               /**< STM6 interrupt channel request 0, 1, 2, 3 */
  STM7_IRQn                    = 63,               /**< STM7 interrupt channel request 0, 1, 2, 3 */
  QSPI0_IRQn                   = 64,               /**< QSPI Ored interrupt via OTC */
  QSPI1_IRQn                   = 65,               /**< Flash A Error interrupt via OTC */
  QSPI2_IRQn                   = 66,               /**< Flash B Error interrupt via OTC */
  STCU2_LBIST_MBIST_IRQn       = 67,               /**< LBIST and MBIST */
  USDHC_IRQn                   = 68,               /**< uSDHC Interrupt */
  CAN0_ORED_IRQn               = 69,               /**< CAN0 OR'ed Bus in Off State. */
  CAN0_ERR_IRQn                = 70,               /**< CAN0 Interrupt indicating that errors were detected on the CAN bus */
  CAN0_ORED_0_7_MB_IRQn        = 71,               /**< CAN0 OR'ed Message buffer (0-7),Rx FIFO Watermark, Rx FIFO Data Available, Rx FIFO Underflow, Rx FIFO Overflow */
  CAN0_ORED_8_127_MB_IRQn      = 72,               /**< CAN0 OR'ed Message buffer (8-127) */
  CAN1_ORED_IRQn               = 73,               /**< CAN1 OR'ed Bus in Off State */
  CAN1_ERR_IRQn                = 74,               /**< CAN1 Interrupt indicating that errors were detected on the CAN bus */
  CAN1_ORED_0_7_MB_IRQn        = 75,               /**< CAN1 OR'ed Message buffer (0-7),Rx FIFO Watermark, Rx FIFO Data Available, Rx FIFO Underflow, Rx FIFO Overflow */
  CAN1_ORED_8_127_MB_IRQn      = 76,               /**< CAN1 OR'ed Message buffer (8-127) */
  CAN2_ORED_IRQn               = 77,               /**< CAN2 OR'ed Bus in Off State */
  CAN2_ERR_IRQn                = 78,               /**< CAN2 Interrupt indicating that errors were detected on the CAN bus */
  CAN2_ORED_0_7_MB_IRQn        = 79,               /**< CAN2 OR'ed Message buffer (0-7),Rx FIFO Watermark, Rx FIFO Data Available, Rx FIFO Underflow, Rx FIFO Overflow */
  CAN2_ORED_8_127_MB_IRQn      = 80,               /**< CAN2 OR'ed Message buffer (8-127) */
  CAN3_ORED_IRQn               = 81,               /**< CAN3 OR'ed Bus in Off State */
  CAN3_ERR_IRQn                = 82,               /**< CAN3 Interrupt indicating that errors were detected on the CAN bus */
  CAN3_ORED_0_7_MB_IRQn        = 83,               /**< CAN3 OR'ed Message buffer (0-7),Rx FIFO Watermark, Rx FIFO Data Available, Rx FIFO Underflow, Rx FIFO Overflow */
  CAN3_ORED_8_127_MB_IRQn      = 84,               /**< CAN3 OR'ed Message buffer (8-127) */
  PIT0_IRQn                    = 85,               /**< OR'ed Interrupt for Channel 0, 1, 2, 3, 4, 5, 6 */
  PIT1_IRQn                    = 86,               /**< OR'ed Interrupt for Channel 0, 1, 2, 3, 4, 5 */
  FTM0_IRQn                    = 87,               /**< OR'ed Interrupt for Channel 0, 1, 2, 3, 4, 5, Overflow, Reload */
  FTM1_IRQn                    = 88,               /**< OR'ed Interrupt for Channel 0, 1, 2, 3, 4, 5, Overflow, Reload */
  GMAC0_Common_IRQn            = 89,               /**< Common Interrupt */
  GMAC0_CH0_TX_IRQn            = 90,               /**< Channel0 TX Interrupt */
  GMAC0_CH0_RX_IRQn            = 91,               /**< Channel0 RX Interrupt */
  GMAC0_CH1_TX_IRQn            = 92,               /**< Channel1 TX Interrupt */
  GMAC0_CH1_RX_IRQn            = 93,               /**< Channel1 RX Interrupt */
  GMAC0_CH2_TX_IRQn            = 94,               /**< Channel2 TX Interrupt */
  GMAC0_CH2_RX_IRQn            = 95,               /**< Channel2 RX Interrupt */
  GMAC0_CH3_TX_IRQn            = 96,               /**< Channel3 TX Interrupt */
  GMAC0_CH3_RX_IRQn            = 97,               /**< Channel3 RX Interrupt */
  GMAC0_CH4_TX_IRQn            = 98,               /**< Channel4 TX Interrupt */
  GMAC0_CH4_RX_IRQn            = 99,               /**< Channel4 RX Interrupt */
  SAR_ADC0_INT_IRQn            = 102,              /**< End of conv., ERROR and Analog Watchdog Interrupt */
  SAR_ADC1_INT_IRQn            = 103,              /**< End of conv., ERROR and Analog Watchdog Interrupt */
  FLEXRAY_NCERR_IRQn           = 104,              /**< LRAM and DRAM Non-Corrected Error interrupt */
  FLEXRAY_CERR_IRQn            = 105,              /**< LRAM and DRAM Corrected Error interrupt */
  FLEXRAY_CH0_RX_FIFO_IRQn     = 106,              /**< Receive FIFO channel A not empty interrupt */
  FLEXRAY_CH1_RX_FIFO_IRQn     = 107,              /**< Receive FIFO channel B not empty interrupt */
  FLEXRAY_WKUP_IRQn            = 108,              /**< Wakeup interrupt */
  FLEXRAY_STATUS_IRQn          = 109,              /**< Combined protocol status and error interrupt */
  FLEXRAY_CMBERR_IRQn          = 110,              /**< Combined CHI error interrupt */
  FLEXRAY_TX_BUFF_IRQn         = 111,              /**< Combined transmit message buffer interrupt */
  FLEXRAY_RX_BUFF_IRQn         = 112,              /**< Combined receive message buffer interrupt */
  FLEXRAY_MODULE_IRQn          = 113,              /**< Combined module interrupt */
  LINFLEXD0_IRQn               = 114,              /**< Internal all interrupt request */
  LINFLEXD1_IRQn               = 115,              /**< Internal all interrupt request */
  SPI0_IRQn                    = 117,              /**< OR'ed DSPI Global Interrupt */
  SPI1_IRQn                    = 118,              /**< OR'ed DSPI Global Interrupt */
  SPI2_IRQn                    = 119,              /**< OR'ed DSPI Global Interrupt */
  SPI3_IRQn                    = 120,              /**< OR'ed DSPI Global Interrupt */
  SPI4_IRQn                    = 121,              /**< OR'ed DSPI Global Interrupt */
  SPI5_IRQn                    = 122,              /**< OR'ed DSPI Global Interrupt */
  I2C0_IRQn                    = 124,              /**< Interrupt Request */
  I2C1_IRQn                    = 125,              /**< Interrupt Request */
  MC_RGM_IRQn                  = 130,              /**< Interrupt Request to System */
  FCCU_ALARM_IRQn              = 132,              /**< Interrupt request (ALARM state) */
  FCCU_MISC_IRQn               = 133,              /**< Interrupt request (miscellaneous conditions) */
  SBSW_IRQn                    = 134,              /**< Interrupt triggered by writing to the TMWDP CONFG_ADDR Register,TMWDPI timer interrupt */
  HSE_MU0_TX_IRQn              = 135,              /**< Ored tx interrupt to MU-0 */
  HSE_MU0_RX_IRQn              = 136,              /**< Ored rx interrupt to MU-0 */
  HSE_MU0_ORED_IRQn            = 137,              /**< ORed general purpose interrupt request to MU-0 */
  HSE_MU1_TX_IRQn              = 138,              /**< Ored tx interrupt to MU-1 */
  HSE_MU1_RX_IRQn              = 139,              /**< Ored rx interrupt to MU-1 */
  HSE_MU1_ORED_IRQn            = 140,              /**< ORed general purpose interrupt request to MU-1 */
  HSE_MU2_TX_IRQn              = 141,              /**< Ored tx interrupt to MU-2 */
  HSE_MU2_RX_IRQn              = 142,              /**< Ored rx interrupt to MU-2 */
  HSE_MU2_ORED_IRQn            = 143,              /**< ORed general purpose interrupt request to MU-2 */
  HSE_MU3_TX_IRQn              = 144,              /**< Ored tx interrupt to MU-3 */
  HSE_MU3_RX_IRQn              = 145,              /**< Ored rx interrupt to MU-3 */
  HSE_MU3_ORED_IRQn            = 146,              /**< ORed general purpose interrupt request to MU-3 */
  DDR0_SCRUB_IRQn              = 147,              /**< Scrubber interrupt indicating one full address range sweep */
  DDR0_PHY_IRQn                = 148,              /**< PHY address decoding error inside DDR SS, PHY interrupt */
  CTU_FIFO_FULL_EMPTY_IRQn     = 149,              /**< FIFO 0,1,2,3 full  or empty or overflow or overrun interrupt */
  CTU_M_RELOAD_IRQn            = 150,              /**< Master reload interrupt,Trigger0 interrupt,Trigger1 interrupt,Trigger2 interrupt,Trigger3 interrupt,Trigger4 interrupt,Trigger5 interrupt,Trigger6 interrupt,Trigger7 interrupt,ADC command interrupt */
  CTU_ERR_IRQn                 = 151,              /**< Error interrupt */
  TMU_ALARM_IRQn               = 152,              /**< Level sensitive temperature alarm interrupt,Level sensitive citical temperature alarm interrupt */
  PCIE0_ORED_DMA_IRQn          = 155,              /**< Logical OR of PCIe DMA interrupts */
  PCIE0_LINK_IRQn              = 156,              /**< Link request status interrupt */
  PCIE0_AXI_MSI_IRQn           = 157,              /**< DSP AXI MSI Interrupt Detected */
  PCIE0_PHY_LDOWN_IRQn         = 158,              /**< PHY link down interrupt */
  PCIE0_PHY_LUP_IRQn           = 159,              /**< PHY link up interrupt */
  PCIE0_INTA_IRQn              = 160,              /**< Interrupt indicating INTA message received */
  PCIE0_INTB_IRQn              = 161,              /**< Interrupt indicating INTB message received */
  PCIE0_INTC_IRQn              = 162,              /**< Interrupt indicating INTC message received */
  PCIE0_INTD_IRQn              = 163,              /**< Interrupt indicating INTD message received */
  PCIE0_MISC_IRQn              = 164,              /**< Miscellaneous interrupt generated by SerDes Subsystem */
  PCIE0_PCS_IRQn               = 165,              /**< PCS interrupt */
  PCIE0_TLP_IRQn               = 166,              /**< TLP request has not completed within the expected time window */
  CORTEX_A53_ERR_L2RAM_CLUSTER0_IRQn = 183,        /**< Error indicator for L2 RAM double-bit ECC error for cluster0 */
  CORTEX_A53_ERR_AXI_CLUSTER0_IRQn = 184,          /**< A53:  Cluster0 Error indicator for AXI or CH bus error */
  CORTEX_A53_ERR_L2RAM_CLUSTER1_IRQn = 185,        /**< Error indicator for L2 RAM double-bit ECC error for cluster1 */
  CORTEX_A53_ERR_AXI_CLUSTER1_IRQn = 186,          /**< A53:  Cluster1 Error indicator for AXI bus error */
  JDC_IRQn                     = 187,              /**< Indicates data ready to be read from JIN_IPS register or new data can be written to JOUT_IPS register when asserted */
  FASTDMA_TX_IRQn              = 202,              /**< Transfer Done interrupt */
  FASTDMA_ERR_IRQn             = 203,              /**< DMA Configuration or Transfer error, 'CRC error encountered during data transfer */
  MIPICSI2_0_INT0_IRQn         = 204,              /**< MIPI-CSI2_1 - Reports errors in the receive path */
  MIPICSI2_0_INT1_IRQn         = 205,              /**< MIPI-CSI2_1 - Protocol and Packet Level Error Reporting */
  MIPICSI2_0_INT2_IRQn         = 206,              /**< MIPI-CSI2_1 - Receive path errors like line length error/ line count error */
  MIPICSI2_0_INT3_IRQn         = 207,              /**< MIPI-CSI2_1 - Turnaround and Transmit related Errors and Events */
  MIPICSI2_1_INT0_IRQn         = 208,              /**< MIPI-CSI2_2 - Reports errors in the receive path */
  MIPICSI2_1_INT1_IRQn         = 209,              /**< MIPI-CSI2_2 - Protocol and Packet Level Error Reporting */
  MIPICSI2_1_INT2_IRQn         = 210,              /**< MIPI-CSI2_2 - Receive path errors like line length error/ line count error */
  MIPICSI2_1_INT3_IRQn         = 211,              /**< MIPI-CSI2_2 - Turnaround and Transmit related Errors and Events */
  MIPICSI2_2_INT0_IRQn         = 212,              /**< MIPI-CSI2_1 - Reports errors in the receive path */
  MIPICSI2_2_INT1_IRQn         = 213,              /**< MIPI-CSI2_1 - Protocol and Packet Level Error Reporting */
  MIPICSI2_2_INT2_IRQn         = 214,              /**< MIPI-CSI2_1 - Receive path errors like line length error/ line count error */
  MIPICSI2_2_INT3_IRQn         = 215,              /**< MIPI-CSI2_1 - Turnaround and Transmit related Errors and Events */
  MIPICSI2_3_INT0_IRQn         = 216,              /**< MIPI-CSI2_2 - Reports errors in the receive path */
  MIPICSI2_3_INT1_IRQn         = 217,              /**< MIPI-CSI2_2 - Protocol and Packet Level Error Reporting */
  MIPICSI2_3_INT2_IRQn         = 218,              /**< MIPI-CSI2_2 - Receive path errors like line length error/ line count error */
  MIPICSI2_3_INT3_IRQn         = 219,              /**< MIPI-CSI2_2 - Turnaround and Transmit related Errors and Events */
  SPT_DSP_ERR_IRQn             = 220,              /**< DSP error interrupt */
  SPT_EVENT_IRQn               = 221,              /**< SPT Event IRQ */
  SPT_ECS_IRQn                 = 222,              /**< ECS IRQ */
  SPT_DMA_COMPL_IRQn           = 223,              /**< ECS IRQ */
  CAN4_ORED_IRQn               = 224,              /**< CAN4 OR'ed Bus in Off State. */
  CAN4_ERR_IRQn                = 225,              /**< CAN4 Interrupt indicating that errors were detected on the CAN bus */
  CAN4_ORED_0_7_MB_IRQn        = 226,              /**< CAN4 OR'ed Message buffer (0-7),Rx FIFO Watermark, Rx FIFO Data Available, Rx FIFO Underflow, Rx FIFO Overflow */
  CAN4_ORED_8_127_MB_IRQn      = 227,              /**< CAN4 OR'ed Message buffer (8-127) */
  CAN5_ORED_IRQn               = 228,              /**< CAN5 OR'ed Bus in Off State. */
  CAN5_ERR_IRQn                = 229,              /**< CAN5 Interrupt indicating that errors were detected on the CAN bus */
  CAN5_ORED_0_7_MB_IRQn        = 230,              /**< CAN5 OR'ed Message buffer (0-7),Rx FIFO Watermark, Rx FIFO Data Available, Rx FIFO Underflow, Rx FIFO Overflow */
  CAN5_ORED_8_127_MB_IRQn      = 231,              /**< CAN5 OR'ed Message buffer (8-127) */
  CAN6_ORED_IRQn               = 232,              /**< CAN6 OR'ed Bus in Off State. */
  CAN6_ERR_IRQn                = 233,              /**< CAN6 Interrupt indicating that errors were detected on the CAN bus */
  CAN6_ORED_0_7_MB_IRQn        = 234,              /**< CAN6 OR'ed Message buffer (0-7),Rx FIFO Watermark, Rx FIFO Data Available, Rx FIFO Underflow, Rx FIFO Overflow */
  CAN6_ORED_8_127_MB_IRQn      = 235,              /**< CAN6 OR'ed Message buffer (8-127) */
  CAN7_ORED_IRQn               = 236,              /**< CAN7 OR'ed Bus in Off State. */
  CAN7_ERR_IRQn                = 237,              /**< CAN7 Interrupt indicating that errors were detected on the CAN bus */
  CAN7_ORED_0_7_MB_IRQn        = 238,              /**< CAN7 OR'ed Message buffer (0-7),Rx FIFO Watermark, Rx FIFO Data Available, Rx FIFO Underflow, Rx FIFO Overflow */
  CAN7_ORED_8_127_MB_IRQn      = 239,              /**< CAN7 OR'ed Message buffer (8-127) */
  PCIE1_ORED_DMA_IRQn          = 240,              /**< Logical OR of PCIe DMA interrupts */
  PCIE1_LINK_IRQn              = 241,              /**< Link request status interrupt */
  PCIE1_AXI_MSI_IRQn           = 242,              /**< DSP AXI MSI Interrupt Detected */
  PCIE1_PHY_LDOWN_IRQn         = 243,              /**< PHY link down interrupt */
  PCIE1_PHY_LUP_IRQn           = 244,              /**< PHY link up interrupt */
  PCIE1_INTA_IRQn              = 245,              /**< Interrupt indicating INTA message received */
  PCIE1_INTB_IRQn              = 246,              /**< Interrupt indicating INTB message received */
  PCIE1_INTC_IRQn              = 247,              /**< Interrupt indicating INTC message received */
  PCIE1_INTD_IRQn              = 248,              /**< Interrupt indicating INTD message received */
  PCIE1_MISC_IRQn              = 249,              /**< Miscellaneous interrupt generated by PCIe Subsystem */
  PCIE1_PCS_IRQn               = 250,              /**< PCS interrupt */
  PCIE1_TLP_IRQn               = 251,              /**< TLP request has not completed within the expected time window */
  GMAC1_Common_IRQn            = 252,              /**< Common Interrupt */
  GMAC1_CH0_TX_IRQn            = 253,              /**< Channel0 TX Interrupt */
  GMAC1_CH0_RX_IRQn            = 254,              /**< Channel0 RX Interrupt */
  GMAC1_CH1_TX_IRQn            = 255,              /**< Channel1 TX Interrupt */
  GMAC1_CH1_RX_IRQn            = 256,              /**< Channel1 RX Interrupt */
  GMAC1_CH2_TX_IRQn            = 257,              /**< Channel2 TX Interrupt */
  GMAC1_CH2_RX_IRQn            = 258,              /**< Channel2 RX Interrupt */
  GMAC1_CH3_TX_IRQn            = 259,              /**< Channel3 TX Interrupt */
  GMAC1_CH3_RX_IRQn            = 260,              /**< Channel3 RX Interrupt */
  GMAC1_CH4_TX_IRQn            = 261,              /**< Channel4 TX Interrupt */
  GMAC1_CH4_RX_IRQn            = 262,              /**< Channel4 RX Interrupt */
  CTE_INT_IRQn                 = 263,              /**< Interrupt signal becomes high on the rising edge of the event defined in the interrupt status register at 0x0224 */
  LAX1_ERR_IRQn                = 264,              /**< LAX_DMA_ERR or VCPU_IIT interrupt */
  LAX0_ERR_IRQn                = 265,              /**< LAX_DMA_ERR or VCPU_IIT interrupt */
  LAX0_ORED_FUNC_IRQn          = 266,              /**< Ored Functional Interrupt */
  LAX0_SOFT_INT_IRQn           = 267,              /**< VSPA Software Interrupt 0, VSPA Software Interrupt 1 */
  SIUL2_1_INT_IRQn             = 269,              /**< External Interrupt Vector 0, External Interrupt Vector 1, External Interrupt Vector 2, External Interrupt Vector 3 */
  LAX1_ORED_FUNC_IRQn          = 270,              /**< Ored Functional Interrupt */
  LAX1_SOFT_INT_IRQn           = 271               /**< VSPA Software Interrupt 0, VSPA Software Interrupt 1 */
} IRQn_Type;

/*!
 * @}
 */ /* end of group Interrupt_vector_numbers */


/* ----------------------------------------------------------------------------
   -- Cortex A53 Core Configuration
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Cortex_Core_Configuration Cortex A53 Core Configuration
 * @{
 */

#define __CA53_REV                     0x0004    /**< Core revision r0p4 */
#define __MPU_PRESENT                  1         /**< Defines if an MPU is present or not */
#define __NVIC_PRIO_BITS               4         /**< Number of priority bits implemented in the NVIC */
#define __Vendor_SysTickConfig         0         /**< Vendor specific implementation of SysTickConfig is defined */
#define __FPU_PRESENT                  1         /**< Defines if an FPU is present or not */


/*!
 * @}
 */ /* end of group Cortex_Core_Configuration */


/* ----------------------------------------------------------------------------
   -- SDK Compatibility
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SDK_Compatibility_Symbols SDK Compatibility
 * @{
 */

/* No SDK compatibility issues. */

/*!
 * @}
 */ /* end of group SDK_Compatibility_Symbols */


#endif  /* #if !defined(S32R45_COMMON_H_) */
