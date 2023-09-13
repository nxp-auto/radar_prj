/*
** ###################################################################
**     Processor:           S32R45_A53
**     Compiler:            Keil ARM C/C++ Compiler
**     Reference manual:    S32R45 RM Rev.3 Draft D
**     Version:             rev. 2.3, 2021-07-29
**     Build:               b210729
**
**     Abstract:
**         Peripheral Access Layer for S32R45_A53
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2023 NXP
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
 * @file S32R45_MIPICSI2.h
 * @version 2.3
 * @date 2021-07-29
 * @brief Peripheral Access Layer for S32R45_MIPICSI2
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

/* Prevention from multiple including the same memory map */
#if !defined(S32R45_MIPICSI2_H_)  /* Check if memory map has not been already included */
#define S32R45_MIPICSI2_H_

#include "S32R45_COMMON.h"

/* ----------------------------------------------------------------------------
   -- MIPICSI2 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MIPICSI2_Peripheral_Access_Layer MIPICSI2 Peripheral Access Layer
 * @{
 */

/** MIPICSI2 - Size of Registers Arrays */
#define MIPICSI2_RX_LANCS_COUNT                   4u
#define MIPICSI2_GNSPR_COUNT                      4u
#define MIPICSI2_NUMPKTS_COUNT                    4u
#define MIPICSI2_RX_VC_COUNT                      4u
#define MIPICSI2_RX_COUNT                         12u
#define MIPICSI2_DROPDATAR_COUNT                  4u
#define MIPICSI2_CBUF_OUTCFG_COUNT                4u
#define MIPICSI2_CBUF_CHNLENBL_COUNT              4u

/** MIPICSI2 - Register Layout Typedef */
typedef struct {
  __IO uint32_t DPHY_RSTCFG;                       /**< DPHY Reset Configuration, offset: 0x0 */
  uint8_t RESERVED_0[4];
  __IO uint32_t DPHY_CLEAR;                        /**< DPHY Clear, offset: 0x8 */
  __IO uint32_t DPHY_FREQCFG;                      /**< DPHY Frequency Configuration, offset: 0xC */
  uint8_t RESERVED_1[8];
  __IO uint32_t RX_RXNULANE;                       /**< Receive Number of Lanes Configuration, offset: 0x18 */
  __IO uint32_t RX_RXENABLE;                       /**< Receive Enable Configuration, offset: 0x1C */
  __IO uint32_t RX_RXLANESWAP;                     /**< Receive Lane Swap Configuration, offset: 0x20 */
  __I  uint32_t RX_CLKCS;                          /**< Clock Configuration Status, offset: 0x24 */
  __I  uint32_t RX_LANCS[MIPICSI2_RX_LANCS_COUNT]; /**< D-PHY Lane 0 Configuration Status..D-PHY Lane 3 Configuration Status, array offset: 0x28, array step: 0x4 */
  __IO uint32_t RX_SR;                             /**< Soft Reset Config, offset: 0x38 */
  __IO uint32_t RX_VCENABLE;                       /**< Receive Virtual Channel Enable Configuration, offset: 0x3C */
  __I  uint32_t RX_DATAIDR;                        /**< Receive Data ID Report, offset: 0x40 */
  uint8_t RESERVED_2[8];
  __I  uint32_t RX_INVIDR;                         /**< Receive Invalid Data ID Report, offset: 0x4C */
  __I  uint32_t RX_GNSPR_VC[MIPICSI2_GNSPR_COUNT]; /**< Receive Generic Short Packet Report, array offset: 0x50, array step: 0x4 */
  __I  uint32_t RX_NUMPKTS_VC[MIPICSI2_NUMPKTS_COUNT]; /**< Receive Number of Packets for VC, array offset: 0x60, array step: 0x4 */
  __IO uint32_t RX_VCINTRS;                        /**< Receive VC Data Interrupt Status, offset: 0x70 */
  __IO uint32_t RX_VCINTRE;                        /**< Receive Data VC Event Interrupt Enable, offset: 0x74 */
  uint8_t RESERVED_3[24];
  __I  uint32_t CONTROLLER_STATUS_REGISTER;        /**< Controller Status, offset: 0x90 */
  __I  uint32_t CRC_REGISTER;                      /**< CRC, offset: 0x94 */
  __IO uint32_t CONTROLLER_ERR_STATUS_REGISTER;    /**< Controller Error Status, offset: 0x98 */
  __IO uint32_t CONTROLLER_ERR_IE;                 /**< Controller Interrupt Enable, offset: 0x9C */
  struct {                                         /* offset: 0xA0, array step: 0x10 */
    __IO uint32_t PPERRIS;                           /**< Receive Data Protocol and Packet Error Interrupt Status for VC0..Receive Data Protocol and Packet Error Interrupt Status for VC3, array offset: 0xA0, array step: 0x10 */
    __IO uint32_t PPERRIE;                           /**< Receive Data Protocol and Packet Error Interrupt Enable for VC0..Receive Data Protocol and Packet Error Interrupt Enable for VC3, array offset: 0xA4, array step: 0x10 */
    __I  uint32_t ERRPOS;                            /**< Receive ECC 1-Bit Error Position for VC0..Receive ECC 1-Bit Error Position for VC3, array offset: 0xA8, array step: 0x10 */
    __I  uint32_t NUMPPERR;                          /**< Receive Packets Number of Protocol Errors for VC0..Receive Packets Number of Protocol Errors for VC3, array offset: 0xAC, array step: 0x10 */
  } RX_VC[MIPICSI2_RX_VC_COUNT];
  uint8_t RESERVED_4[4];
  __IO uint32_t RX_PHYERRIS;                       /**< Receive Data PHY Level Error Interrupt Status, offset: 0xE4 */
  __IO uint32_t RX_PHYERRIE;                       /**< Receive Data PHY Level Error Interrupt Enable, offset: 0xE8 */
  uint8_t RESERVED_5[16];
  __IO uint32_t RX_STAT_CONFIG;                    /**< Receive Data Statistical Computation Configuration, offset: 0xFC */
  struct {                                         /* offset: 0x100, array step: 0x30 */
    __IO uint32_t CBUF_CONFIG;                       /**< Receive Data Circular Buffer Configuration, array offset: 0x100, array step: 0x30 */
    __IO uint32_t INPLINELEN_CONFIG;                 /**< Receive Data Input Line Length Configuration, array offset: 0x104, array step: 0x30 */
    __IO uint32_t LINELEN_CONFIG;                    /**< Receive Data Line Length Configuration, array offset: 0x108, array step: 0x30 */
    __IO uint32_t NUMLINES_CONFIG;                   /**< Receive Data Expected Number of Lines Configuration, array offset: 0x10C, array step: 0x30 */
    __IO uint32_t CBUF_SRTPTR;                       /**< Receive Data Circular Buffer Start Pointer, array offset: 0x110, array step: 0x30 */
    __IO uint32_t CBUF_BUFLEN;                       /**< Receive Data Circular Buffer Length, array offset: 0x114, array step: 0x30 */
    __IO uint32_t CBUF_NUMLINE;                      /**< Receive Data Circular Buffer Number of Lines, array offset: 0x118, array step: 0x30 */
    __IO uint32_t CBUF_LPDI;                         /**< Receive Data Circular Buffer Lines Done Generation, array offset: 0x11C, array step: 0x30 */
    __I  uint32_t CBUF_NXTLINE;                      /**< Receive Data Circular Buffer Next Row Indication, array offset: 0x120, array step: 0x30 */
    __I  uint32_t CBUF_RXLINE;                       /**< Receive Data Circular Buffer Total Lines Received Status, array offset: 0x124, array step: 0x30 */
    __I  uint32_t CBUF_ERRLEN;                       /**< Receive Data Circular Buffer Error Line Length Status, array offset: 0x128, array step: 0x30 */
    __I  uint32_t CBUF_ERRLINE;                      /**< Receive Data Circular Buffer Line Number for Incorrect Length Status, array offset: 0x12C, array step: 0x30 */
  } RX[MIPICSI2_RX_COUNT];
  uint8_t RESERVED_6[192];
  __IO uint32_t CBUF_INTRS;                        /**< Receive Data Circular Buffer Error Interrupt Status, offset: 0x400 */
  __IO uint32_t CBUF_INTRE;                        /**< Receive Circular Buffer Error Interrupt Enable, offset: 0x404 */
  uint8_t RESERVED_7[8];
  __I  uint32_t RX_DROPDATAR[MIPICSI2_DROPDATAR_COUNT]; /**< Received Drop Data Type and VC Report, array offset: 0x410, array step: 0x4 */
  __IO uint32_t RX_CBUF_OUTCFG[MIPICSI2_CBUF_OUTCFG_COUNT]; /**< Receive Data Channel Output Configuration, array offset: 0x420, array step: 0x4 */
  __IO uint32_t RX_CBUF_CHNLENBL[MIPICSI2_CBUF_CHNLENBL_COUNT]; /**< Receive Data Channel Enable/Disable Configuration, array offset: 0x430, array step: 0x4 */
  __IO uint32_t RX_CBUF0_CHNLOFFSET0_0;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x440 */
  __IO uint32_t RX_CBUF0_CHNLOFFSET1_0;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x444 */
  __IO uint32_t RX_CBUF0_CHNLOFFSET2_0;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x448 */
  __IO uint32_t RX_CBUF0_CHNLOFFSET3_0;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x44C */
  __IO uint32_t RX_CBUF1_CHNLOFFSET0_1;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x450 */
  __IO uint32_t RX_CBUF1_CHNLOFFSET1_1;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x454 */
  __IO uint32_t RX_CBUF1_CHNLOFFSET2_1;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x458 */
  __IO uint32_t RX_CBUF1_CHNLOFFSET3_1;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x45C */
  __IO uint32_t RX_CBUF2_CHNLOFFSET0_2;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x460 */
  __IO uint32_t RX_CBUF2_CHNLOFFSET1_2;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x464 */
  __IO uint32_t RX_CBUF2_CHNLOFFSET2_2;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x468 */
  __IO uint32_t RX_CBUF2_CHNLOFFSET3_2;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x46C */
  __IO uint32_t RX_CBUF3_CHNLOFFSET0_3;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x470 */
  __IO uint32_t RX_CBUF3_CHNLOFFSET1_3;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x474 */
  __IO uint32_t RX_CBUF3_CHNLOFFSET2_3;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x478 */
  __IO uint32_t RX_CBUF3_CHNLOFFSET3_3;            /**< Receive Data Channel Offset Compensation Configuration, offset: 0x47C */
  uint8_t RESERVED_8[16];
  __IO uint32_t RX_CHNL_INTRS;                     /**< Receive Data Channel Status, offset: 0x490 */
  __IO uint32_t RX_CHNL_INTRE;                     /**< Receive Channel Interrupt Enable, offset: 0x494 */
  __IO uint32_t WR_CHNL_INTRS;                     /**< AXI Write Channel Interrupt Status, offset: 0x498 */
  __IO uint32_t WR_CHNL_INTRE;                     /**< AXI Write Channel Interrupt Enable, offset: 0x49C */
  uint8_t RESERVED_9[172];
  __IO uint32_t TX_TURNAROUND_POST_CFG;            /**< Turnaround Post Configuration, offset: 0x54C */
  __IO uint32_t TX_TXNULANE;                       /**< Transmit Number of Lanes Configuration, offset: 0x550 */
  __IO uint32_t TX_TXENABLE;                       /**< Transmit Enable Configuration, offset: 0x554 */
  uint8_t RESERVED_10[4];
  __IO uint32_t TX_DESCSTART;                      /**< Transmit Descriptor Start Configuration, offset: 0x55C */
  __I  uint32_t TX_TXSTAT;                         /**< MIPICSI2 Transmit Status, offset: 0x560 */
  __IO uint32_t TX_DESCRCFGS;                      /**< MIPICSI2 Transmit Descriptor Configuration and Status, offset: 0x564 */
  __IO uint32_t TX_DESCADDR;                       /**< Transmit Descriptor Base Address, offset: 0x568 */
  __I  uint32_t TX_DESCNADDR;                      /**< Next Transmit Descriptor Address Report, offset: 0x56C */
  __I  uint32_t TX_DESCNUM;                        /**< Number of Transmit Descriptor Read Report, offset: 0x570 */
  __I  uint32_t TX_PKTS;                           /**< Transmit Packet Status, offset: 0x574 */
  __IO uint32_t TX_ERRIS;                          /**< Transmit Error Interrupt Status, offset: 0x578 */
  __IO uint32_t TX_IE;                             /**< Transmit Interrupt Enable, offset: 0x57C */
  uint8_t RESERVED_11[4];
  __IO uint32_t TURNCFG;                           /**< Turnaround Request Configuration, offset: 0x584 */
  __IO uint32_t TURNSTAT;                          /**< Turnaround Request Status, offset: 0x588 */
  __IO uint32_t TURNSTATIE;                        /**< Turnaround Error Interrupt Enable, offset: 0x58C */
  __IO uint32_t TURNIS;                            /**< Turnaround Request Interrupt Status, offset: 0x590 */
  __IO uint32_t TURNIE;                            /**< Turnaround Request Interrupt Enable, offset: 0x594 */
  __IO uint32_t TRIGGER_GPIO1;                     /**< GPIO1 Pad Event Trigger Control, offset: 0x598 */
  uint8_t RESERVED_12[4];
  __IO uint32_t TRIGGER_SDMA1;                     /**< SDMA1 Pad Event Trigger Control, offset: 0x5A0 */
  uint8_t RESERVED_13[4];
  __IO uint32_t TRIGGEREN_GPIO;                    /**< GPIO Pad Event Trigger Enable Control, offset: 0x5A8 */
  __IO uint32_t TRIGGEREN_SDMA;                    /**< SDMA Pad Event Trigger Enable Control, offset: 0x5AC */
  uint8_t RESERVED_14[72];
  __IO uint32_t TX_TEST_FIFO_CFG;                  /**< Transmit Test Path FIFO-Based Flow Control Configuration, offset: 0x5F8 */
  uint8_t RESERVED_15[12];
  __IO uint8_t DPHY_CALTYPE_CNTRL;                 /**< System Configuration, offset: 0x608 */
  uint8_t RESERVED_16[3];
  __IO uint8_t DPHY_SKEWCAL_CNTRL;                 /**< System Configuration, offset: 0x60C */
  __IO uint8_t DPHY_RX_SYNALIGN_CFG;               /**< System Configuration, offset: 0x60D */
  __IO uint8_t DPHY_DESKEW_CFG;                    /**< This register is used to program the Deskew accumulator size(FJUMP), offset: 0x60E */
  uint8_t RESERVED_17[213];
  __IO uint8_t DPHY_RX_STARTUP_OVERRIDE;           /**< System Startup Observability, offset: 0x6E4 */
  uint8_t RESERVED_18[1];
  __IO uint8_t DPHY_DDLOSCFREQ_CFG1;               /**< System Startup Override, offset: 0x6E6 */
  __IO uint8_t DPHY_DDLOSCFREQ_CFG2;               /**< System Startup Override, offset: 0x6E7 */
  __IO uint8_t DPHY_DDLOSCFREQ_OVREN;              /**< System Startup Override, offset: 0x6E8 */
  uint8_t RESERVED_19[315];
  __I  uint8_t DPHY_RX_TERM_CAL_0;                 /**< Termination Calibration Observability, offset: 0x824 */
  __I  uint8_t DPHY_RX_TERM_CAL_1;                 /**< Termination Calibration Observability, offset: 0x825 */
  uint8_t RESERVED_20[229];
  __IO uint8_t DPHY_CLOCK_LANE_CNTRL;              /**< Clock Lane Control, offset: 0x90B */
  uint8_t RESERVED_21[115];
  __IO uint8_t DPHY_CLKOFFSETCAL_OVRRIDE;          /**< Clock Lane Offset Cancellation Control, offset: 0x97F */
  __IO uint8_t DPHY_CLKOFFSETCAL_OVRRIDEVAL;       /**< Clock Lane Offset Cancellation Control 2, offset: 0x980 */
  uint8_t RESERVED_22[31];
  __I  uint8_t DPHY_CLKCALVAL_COMPS;               /**< Clock Lane Offset Cancellation Observability 3, offset: 0x9A0 */
  __I  uint8_t DPHY_CLKOFFSETCAL_COMPS;            /**< Clock Lane Offset Cancellation Observability, offset: 0x9A1 */
  uint8_t RESERVED_23[359];
  __IO uint8_t DPHY_RX_LPRXPON_LANE0;              /**< Lane 0 Low Power Receive Control, offset: 0xB09 */
  uint8_t RESERVED_24[41];
  __I  uint8_t DPHY_DATALANE_DESKEW_COMPS0;        /**< Lane 0 Observability, offset: 0xB33 */
  uint8_t RESERVED_25[75];
  __IO uint8_t DPHY_DATAL0OFFSETCAL_OVRCNTRL;      /**< Lane 0 Offset Compensation Control, offset: 0xB7F */
  __IO uint8_t DPHY_DATALOFFSETCAL_OVRVALUE0;      /**< Lane 0 Offset Compensation Control, offset: 0xB80 */
  uint8_t RESERVED_26[34];
  __I  uint8_t DPHY_DATALANE_OFFSETCAL_COMPS0;     /**< Lane Offset Compensation Observability, offset: 0xBA3 */
  uint8_t RESERVED_27[1];
  __I  uint8_t DPHY_DATALOFFSETCAL_VALUE0;         /**< Lane Offset Compensation Observability, offset: 0xBA5 */
  uint8_t RESERVED_28[62];
  __I  uint8_t DPHY_DATALANE_DDLTUNE_COMP0;        /**< Lane DDL Tune Observability, offset: 0xBE4 */
  uint8_t RESERVED_29[4];
  __I  uint8_t DPHY_DATALANE_DDLTUNE_VALUE0;       /**< Lane 0 DDL Tune Observability, offset: 0xBE9 */
  __I  uint8_t DPHY_DATALANE0_DESKEW_VALUE1;       /**< Lane 0 DDL Tune Observability, offset: 0xBEA */
  __I  uint8_t DPHY_DATALANE0_DESKEW_VALUE2;       /**< Lane 0 DDL Tune Observability, offset: 0xBEB */
  uint8_t RESERVED_30[31];
  __IO uint8_t DPHY_DATALANE_DDLTUNE_OVRVALUE0;    /**< Lane 0 DDL Tune Control, offset: 0xC0B */
  __IO uint8_t DPHY_DATALANE0_DESKEW_OVRVALUE1;    /**< Lane 0 DDL Tune Control, offset: 0xC0C */
  __IO uint8_t DPHY_DATALANE0_DESKEW_OVRVALUE2;    /**< Lane 0 DDL Tune Control, offset: 0xC0D */
  uint8_t RESERVED_31[251];
  __IO uint8_t DPHY_RX_LPRXPON_LANE1;              /**< Lane 1 Low Power Receive Control, offset: 0xD09 */
  uint8_t RESERVED_32[5];
  __IO uint8_t DPHY_DATALANE_DESKEW_OVRVALUE11;    /**< Lane 1 Control, offset: 0xD0F */
  __IO uint8_t DPHY_DATALANE_DESKEW_OVRVALUE21;    /**< Lane 1 Control, offset: 0xD10 */
  uint8_t RESERVED_33[34];
  __I  uint8_t DPHY_DATALANE_DESKEW_COMPS1;        /**< Lane 0 Observability, offset: 0xD33 */
  uint8_t RESERVED_34[4];
  __I  uint8_t DPHY_DATALANE_DESKEW_VALUE11;       /**< Lane 1 Observability, offset: 0xD38 */
  __I  uint8_t DPHY_DATALANE_DESKEW_VALUE21;       /**< Lane 1 Observability, offset: 0xD39 */
  uint8_t RESERVED_35[69];
  __IO uint8_t DPHY_DATAL1OFFSETCAL_OVRCNTRL;      /**< Lane 1 Offset Compensation Control, offset: 0xD7F */
  __IO uint8_t DPHY_DATALOFFSETCAL_OVRVALUE1;      /**< Lane 0 Offset Compensation Control, offset: 0xD80 */
  uint8_t RESERVED_36[34];
  __I  uint8_t DPHY_DATALANE_OFFSETCAL_COMPS1;     /**< Lane Offset Compensation Observability, offset: 0xDA3 */
  uint8_t RESERVED_37[1];
  __I  uint8_t DPHY_DATALOFFSETCAL_VALUE1;         /**< Lane Offset Compensation Observability, offset: 0xDA5 */
  uint8_t RESERVED_38[62];
  __I  uint8_t DPHY_DATALANE_DDLTUNE_COMP1;        /**< Lane DDL Tune Observability, offset: 0xDE4 */
  uint8_t RESERVED_39[4];
  __I  uint8_t DPHY_DATALANE_DDLTUNE_VALUE1;       /**< Lane 0 DDL Tune Observability, offset: 0xDE9 */
  uint8_t RESERVED_40[33];
  __IO uint8_t DPHY_DATALANE_DDLTUNE_OVRVALUE1;    /**< Lane 0 DDL Tune Control, offset: 0xE0B */
  uint8_t RESERVED_41[253];
  __IO uint8_t DPHY_RX_LPRXPON_LANE2;              /**< Lane 2 Low Power Receive Control, offset: 0xF09 */
  uint8_t RESERVED_42[5];
  __IO uint8_t DPHY_DATALANE_DESKEW_OVRVALUE12;    /**< Lane 1 Control, offset: 0xF0F */
  __IO uint8_t DPHY_DATALANE_DESKEW_OVRVALUE22;    /**< Lane 1 Control, offset: 0xF10 */
  uint8_t RESERVED_43[34];
  __I  uint8_t DPHY_DATALANE_DESKEW_COMPS2;        /**< Lane 0 Observability, offset: 0xF33 */
  uint8_t RESERVED_44[4];
  __I  uint8_t DPHY_DATALANE_DESKEW_VALUE12;       /**< Lane 1 Observability, offset: 0xF38 */
  __I  uint8_t DPHY_DATALANE_DESKEW_VALUE22;       /**< Lane 1 Observability, offset: 0xF39 */
  uint8_t RESERVED_45[69];
  __IO uint8_t DPHY_DATAL2OFFSETCAL_OVRCNTRL;      /**< Lane 1 Offset Compensation Control, offset: 0xF7F */
  __IO uint8_t DPHY_DATALOFFSETCAL_OVRVALUE2;      /**< Lane 0 Offset Compensation Control, offset: 0xF80 */
  uint8_t RESERVED_46[34];
  __I  uint8_t DPHY_DATALANE_OFFSETCAL_COMPS2;     /**< Lane Offset Compensation Observability, offset: 0xFA3 */
  uint8_t RESERVED_47[1];
  __I  uint8_t DPHY_DATALOFFSETCAL_VALUE2;         /**< Lane Offset Compensation Observability, offset: 0xFA5 */
  uint8_t RESERVED_48[62];
  __I  uint8_t DPHY_DATALANE_DDLTUNE_COMP2;        /**< Lane DDL Tune Observability, offset: 0xFE4 */
  uint8_t RESERVED_49[4];
  __I  uint8_t DPHY_DATALANE_DDLTUNE_VALUE2;       /**< Lane 0 DDL Tune Observability, offset: 0xFE9 */
  uint8_t RESERVED_50[33];
  __IO uint8_t DPHY_DATALANE_DDLTUNE_OVRVALUE2;    /**< Lane 0 DDL Tune Control, offset: 0x100B */
  uint8_t RESERVED_51[253];
  __IO uint8_t DPHY_RX_LPRXPON_LANE3;              /**< Lane 3 Low Power Receive Control, offset: 0x1109 */
  uint8_t RESERVED_52[1];
  __IO uint8_t DPHY_DATALANE_DDLTUNE_OVRVALUE3;    /**< Lane 0 DDL Tune Control, offset: 0x110B */
  uint8_t RESERVED_53[3];
  __IO uint8_t DPHY_DATALANE_DESKEW_OVRVALUE13;    /**< Lane 1 Control, offset: 0x110F */
  __IO uint8_t DPHY_DATALANE_DESKEW_OVRVALUE23;    /**< Lane 1 Control, offset: 0x1110 */
  uint8_t RESERVED_54[34];
  __I  uint8_t DPHY_DATALANE_DESKEW_COMPS3;        /**< Lane 0 Observability, offset: 0x1133 */
  uint8_t RESERVED_55[4];
  __I  uint8_t DPHY_DATALANE_DESKEW_VALUE13;       /**< Lane 1 Observability, offset: 0x1138 */
  __I  uint8_t DPHY_DATALANE_DESKEW_VALUE23;       /**< Lane 1 Observability, offset: 0x1139 */
  uint8_t RESERVED_56[69];
  __IO uint8_t DPHY_DATAL3OFFSETCAL_OVRCNTRL;      /**< Lane 1 Offset Compensation Control, offset: 0x117F */
  __IO uint8_t DPHY_DATALOFFSETCAL_OVRVALUE3;      /**< Lane 0 Offset Compensation Control, offset: 0x1180 */
  uint8_t RESERVED_57[34];
  __I  uint8_t DPHY_DATALANE_OFFSETCAL_COMPS3;     /**< Lane Offset Compensation Observability, offset: 0x11A3 */
  uint8_t RESERVED_58[1];
  __I  uint8_t DPHY_DATALOFFSETCAL_VALUE3;         /**< Lane Offset Compensation Observability, offset: 0x11A5 */
  uint8_t RESERVED_59[62];
  __I  uint8_t DPHY_DATALANE_DDLTUNE_COMP3;        /**< Lane DDL Tune Observability, offset: 0x11E4 */
  uint8_t RESERVED_60[4];
  __I  uint8_t DPHY_DATALANE_DDLTUNE_VALUE3;       /**< Lane 0 DDL Tune Observability, offset: 0x11E9 */
  uint8_t RESERVED_61[360];
  __IO uint8_t DPHY_TURNAROUND_CFG;                /**< System Timers, offset: 0x1352 */
  uint8_t RESERVED_62[9];
  __IO uint8_t DPHY_TX_CLKLANETIMERS_CTRL1;        /**< Clock Lane's HS-TX Tclk-prepare Counter Control, offset: 0x135C */
  __IO uint8_t DPHY_TX_CLKLANETIMERS_CTRL2;        /**< Clock Lane's HS-TX Tclk-request Counter Control, offset: 0x135D */
  uint8_t RESERVED_63[4];
  __IO uint8_t DPHY_TX_DATALANETIMERS_CTRL1;       /**< Data Lane's HS-TX Ths-prepare Counter Control, offset: 0x1362 */
  __IO uint8_t DPHY_TX_DATALANETIMERS_CTRL2;       /**< Data lane's HS-TX Ths-request Counter Control, offset: 0x1363 */
  uint8_t RESERVED_64[262];
  __IO uint8_t DPHY_PLL_VREF_CONFIG;               /**< PLL control, offset: 0x146A */
  uint8_t RESERVED_65[63];
  __IO uint8_t DPHY_CB_VBE_SEL;                    /**< Common Block Control, offset: 0x14AA */
  __IO uint8_t DPHY_ATB_CB_ATB_VBE_SEL;            /**< Common Block Control, offset: 0x14AB */
  uint8_t RESERVED_66[93];
  __IO uint8_t DPHY_TX_RDWR_TERM_CAL_0;            /**< Termination Calibration Control, offset: 0x1509 */
  __IO uint8_t DPHY_TX_TERM_CAL_OVR;               /**< Termination Calibration Control, offset: 0x150A */
  uint8_t RESERVED_67[21];
  __I  uint8_t DPHY_TX_TERM_CAL_0;                 /**< Termination Calibration Observability, offset: 0x1520 */
  __I  uint8_t DPHY_TX_TERM_CAL_1;                 /**< Termination Calibration Observability, offset: 0x1521 */
  __I  uint8_t DPHY_TERMCAL_STAT2;                 /**< Termination Calibration Observability, offset: 0x1522 */
  uint8_t RESERVED_68[228];
  __IO uint8_t DPHY_CLKLANE_POLCFG;                /**< Clock Lane Control, offset: 0x1607 */
  uint8_t RESERVED_69[508];
  __IO uint8_t DPHY_TX_LANE0EN;                    /**< Lane 0 Control, offset: 0x1804 */
  __IO uint8_t DPHY_TX_LANE0LPEN;                  /**< Lane 0 Control, offset: 0x1805 */
  uint8_t RESERVED_70[507];
  __IO uint8_t DPHY_ATB_DATA_LANE1;                /**< Lane 1 Control, offset: 0x1A01 */
  uint8_t RESERVED_71[2];
  __IO uint8_t DPHY_TX_LANE1EN;                    /**< Lane 1 Control, offset: 0x1A04 */
  uint8_t RESERVED_72[508];
  __IO uint8_t DPHY_ATB_DATA_LANE2;                /**< Lane 2 Control, offset: 0x1C01 */
  uint8_t RESERVED_73[2];
  __IO uint8_t DPHY_TX_LANE2EN;                    /**< Lane 2 Control, offset: 0x1C04 */
  uint8_t RESERVED_74[508];
  __IO uint8_t DPHY_ATB_DATA_LANE3;                /**< Lane 3 Control, offset: 0x1E01 */
  uint8_t RESERVED_75[2];
  __IO uint8_t DPHY_TX_LANE3EN;                    /**< Lane 3 Control, offset: 0x1E04 */
} MIPICSI2_Type, *MIPICSI2_MemMapPtr;

/** Number of instances of the MIPICSI2 module. */
#define MIPICSI2_INSTANCE_COUNT                  (4u)

/* MIPICSI2 - Peripheral instance base addresses */
/** Peripheral MIPICSI2_0 base address */
#define IP_MIPICSI2_0_BASE                       (0x44018000u)
/** Peripheral MIPICSI2_0 base pointer */
#define IP_MIPICSI2_0                            ((MIPICSI2_Type *)IP_MIPICSI2_0_BASE)
/** Peripheral MIPICSI2_1 base address */
#define IP_MIPICSI2_1_BASE                       (0x44014000u)
/** Peripheral MIPICSI2_1 base pointer */
#define IP_MIPICSI2_1                            ((MIPICSI2_Type *)IP_MIPICSI2_1_BASE)
/** Peripheral MIPICSI2_2 base address */
#define IP_MIPICSI2_2_BASE                       (0x44020000u)
/** Peripheral MIPICSI2_2 base pointer */
#define IP_MIPICSI2_2                            ((MIPICSI2_Type *)IP_MIPICSI2_2_BASE)
/** Peripheral MIPICSI2_3 base address */
#define IP_MIPICSI2_3_BASE                       (0x4401C000u)
/** Peripheral MIPICSI2_3 base pointer */
#define IP_MIPICSI2_3                            ((MIPICSI2_Type *)IP_MIPICSI2_3_BASE)
/** Array initializer of MIPICSI2 peripheral base addresses */
#define IP_MIPICSI2_BASE_ADDRS                   { IP_MIPICSI2_0_BASE, IP_MIPICSI2_1_BASE, IP_MIPICSI2_2_BASE, IP_MIPICSI2_3_BASE }
/** Array initializer of MIPICSI2 peripheral base pointers */
#define IP_MIPICSI2_BASE_PTRS                    { IP_MIPICSI2_0, IP_MIPICSI2_1, IP_MIPICSI2_2, IP_MIPICSI2_3 }

/* ----------------------------------------------------------------------------
   -- MIPICSI2 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MIPICSI2_Register_Masks MIPICSI2 Register Masks
 * @{
 */

/*! @name DPHY_RSTCFG - DPHY Reset Configuration */
/*! @{ */

#define MIPICSI2_DPHY_RSTCFG_SHUTDWNZ_MASK       (0x1U)
#define MIPICSI2_DPHY_RSTCFG_SHUTDWNZ_SHIFT      (0U)
#define MIPICSI2_DPHY_RSTCFG_SHUTDWNZ_WIDTH      (1U)
#define MIPICSI2_DPHY_RSTCFG_SHUTDWNZ(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_DPHY_RSTCFG_SHUTDWNZ_SHIFT)) & MIPICSI2_DPHY_RSTCFG_SHUTDWNZ_MASK)

#define MIPICSI2_DPHY_RSTCFG_RSTZ_MASK           (0x2U)
#define MIPICSI2_DPHY_RSTCFG_RSTZ_SHIFT          (1U)
#define MIPICSI2_DPHY_RSTCFG_RSTZ_WIDTH          (1U)
#define MIPICSI2_DPHY_RSTCFG_RSTZ(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_DPHY_RSTCFG_RSTZ_SHIFT)) & MIPICSI2_DPHY_RSTCFG_RSTZ_MASK)
/*! @} */

/*! @name DPHY_CLEAR - DPHY Clear */
/*! @{ */

#define MIPICSI2_DPHY_CLEAR_CLRREG_MASK          (0x1U)
#define MIPICSI2_DPHY_CLEAR_CLRREG_SHIFT         (0U)
#define MIPICSI2_DPHY_CLEAR_CLRREG_WIDTH         (1U)
#define MIPICSI2_DPHY_CLEAR_CLRREG(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_DPHY_CLEAR_CLRREG_SHIFT)) & MIPICSI2_DPHY_CLEAR_CLRREG_MASK)
/*! @} */

/*! @name DPHY_FREQCFG - DPHY Frequency Configuration */
/*! @{ */

#define MIPICSI2_DPHY_FREQCFG_HSFREQRNG_MASK     (0x7FU)
#define MIPICSI2_DPHY_FREQCFG_HSFREQRNG_SHIFT    (0U)
#define MIPICSI2_DPHY_FREQCFG_HSFREQRNG_WIDTH    (7U)
#define MIPICSI2_DPHY_FREQCFG_HSFREQRNG(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_DPHY_FREQCFG_HSFREQRNG_SHIFT)) & MIPICSI2_DPHY_FREQCFG_HSFREQRNG_MASK)

#define MIPICSI2_DPHY_FREQCFG_CLKFREQRNG_MASK    (0x7F80U)
#define MIPICSI2_DPHY_FREQCFG_CLKFREQRNG_SHIFT   (7U)
#define MIPICSI2_DPHY_FREQCFG_CLKFREQRNG_WIDTH   (8U)
#define MIPICSI2_DPHY_FREQCFG_CLKFREQRNG(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_DPHY_FREQCFG_CLKFREQRNG_SHIFT)) & MIPICSI2_DPHY_FREQCFG_CLKFREQRNG_MASK)
/*! @} */

/*! @name RX_RXNULANE - Receive Number of Lanes Configuration */
/*! @{ */

#define MIPICSI2_RX_RXNULANE_RXNULANE_MASK       (0xFU)
#define MIPICSI2_RX_RXNULANE_RXNULANE_SHIFT      (0U)
#define MIPICSI2_RX_RXNULANE_RXNULANE_WIDTH      (4U)
#define MIPICSI2_RX_RXNULANE_RXNULANE(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_RXNULANE_RXNULANE_SHIFT)) & MIPICSI2_RX_RXNULANE_RXNULANE_MASK)
/*! @} */

/*! @name RX_RXENABLE - Receive Enable Configuration */
/*! @{ */

#define MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_MASK (0x1U)
#define MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_SHIFT (0U)
#define MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_WIDTH (1U)
#define MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN(x)  (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_SHIFT)) & MIPICSI2_RX_RXENABLE_CFG_CLK_LANE_EN_MASK)

#define MIPICSI2_RX_RXENABLE_CFG_DATA_LANE_EN_MASK (0x1EU)
#define MIPICSI2_RX_RXENABLE_CFG_DATA_LANE_EN_SHIFT (1U)
#define MIPICSI2_RX_RXENABLE_CFG_DATA_LANE_EN_WIDTH (4U)
#define MIPICSI2_RX_RXENABLE_CFG_DATA_LANE_EN(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_RXENABLE_CFG_DATA_LANE_EN_SHIFT)) & MIPICSI2_RX_RXENABLE_CFG_DATA_LANE_EN_MASK)

#define MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT_MASK  (0x1E0U)
#define MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT_SHIFT (5U)
#define MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT_WIDTH (4U)
#define MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT(x)    (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT_SHIFT)) & MIPICSI2_RX_RXENABLE_CFG_FLUSH_CNT_MASK)
/*! @} */

/*! @name RX_RXLANESWAP - Receive Lane Swap Configuration */
/*! @{ */

#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE0_SEL_MASK (0x3U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE0_SEL_SHIFT (0U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE0_SEL_WIDTH (2U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE0_SEL(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_RXLANESWAP_O_CFG_LANE0_SEL_SHIFT)) & MIPICSI2_RX_RXLANESWAP_O_CFG_LANE0_SEL_MASK)

#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE1_SEL_MASK (0xCU)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE1_SEL_SHIFT (2U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE1_SEL_WIDTH (2U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE1_SEL(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_RXLANESWAP_O_CFG_LANE1_SEL_SHIFT)) & MIPICSI2_RX_RXLANESWAP_O_CFG_LANE1_SEL_MASK)

#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE2_SEL_MASK (0x30U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE2_SEL_SHIFT (4U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE2_SEL_WIDTH (2U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE2_SEL(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_RXLANESWAP_O_CFG_LANE2_SEL_SHIFT)) & MIPICSI2_RX_RXLANESWAP_O_CFG_LANE2_SEL_MASK)

#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE3_SEL_MASK (0xC0U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE3_SEL_SHIFT (6U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE3_SEL_WIDTH (2U)
#define MIPICSI2_RX_RXLANESWAP_O_CFG_LANE3_SEL(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_RXLANESWAP_O_CFG_LANE3_SEL_SHIFT)) & MIPICSI2_RX_RXLANESWAP_O_CFG_LANE3_SEL_MASK)
/*! @} */

/*! @name RX_CLKCS - Clock Configuration Status */
/*! @{ */

#define MIPICSI2_RX_CLKCS_HSRA_MASK              (0x1U)
#define MIPICSI2_RX_CLKCS_HSRA_SHIFT             (0U)
#define MIPICSI2_RX_CLKCS_HSRA_WIDTH             (1U)
#define MIPICSI2_RX_CLKCS_HSRA(x)                (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CLKCS_HSRA_SHIFT)) & MIPICSI2_RX_CLKCS_HSRA_MASK)

#define MIPICSI2_RX_CLKCS_ULPSC_MASK             (0x2U)
#define MIPICSI2_RX_CLKCS_ULPSC_SHIFT            (1U)
#define MIPICSI2_RX_CLKCS_ULPSC_WIDTH            (1U)
#define MIPICSI2_RX_CLKCS_ULPSC(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CLKCS_ULPSC_SHIFT)) & MIPICSI2_RX_CLKCS_ULPSC_MASK)

#define MIPICSI2_RX_CLKCS_CSTOP_MASK             (0x4U)
#define MIPICSI2_RX_CLKCS_CSTOP_SHIFT            (2U)
#define MIPICSI2_RX_CLKCS_CSTOP_WIDTH            (1U)
#define MIPICSI2_RX_CLKCS_CSTOP(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CLKCS_CSTOP_SHIFT)) & MIPICSI2_RX_CLKCS_CSTOP_MASK)

#define MIPICSI2_RX_CLKCS_CULPSA_MASK            (0x8U)
#define MIPICSI2_RX_CLKCS_CULPSA_SHIFT           (3U)
#define MIPICSI2_RX_CLKCS_CULPSA_WIDTH           (1U)
#define MIPICSI2_RX_CLKCS_CULPSA(x)              (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CLKCS_CULPSA_SHIFT)) & MIPICSI2_RX_CLKCS_CULPSA_MASK)

#define MIPICSI2_RX_CLKCS_CULPMA_MASK            (0x10U)
#define MIPICSI2_RX_CLKCS_CULPMA_SHIFT           (4U)
#define MIPICSI2_RX_CLKCS_CULPMA_WIDTH           (1U)
#define MIPICSI2_RX_CLKCS_CULPMA(x)              (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CLKCS_CULPMA_SHIFT)) & MIPICSI2_RX_CLKCS_CULPMA_MASK)
/*! @} */

/*! @name RX_LANCS - D-PHY Lane 0 Configuration Status..D-PHY Lane 3 Configuration Status */
/*! @{ */

#define MIPICSI2_RX_LANCS_RXVALH_MASK            (0x1U)
#define MIPICSI2_RX_LANCS_RXVALH_SHIFT           (0U)
#define MIPICSI2_RX_LANCS_RXVALH_WIDTH           (1U)
#define MIPICSI2_RX_LANCS_RXVALH(x)              (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_LANCS_RXVALH_SHIFT)) & MIPICSI2_RX_LANCS_RXVALH_MASK)

#define MIPICSI2_RX_LANCS_RXACTH_MASK            (0x2U)
#define MIPICSI2_RX_LANCS_RXACTH_SHIFT           (1U)
#define MIPICSI2_RX_LANCS_RXACTH_WIDTH           (1U)
#define MIPICSI2_RX_LANCS_RXACTH(x)              (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_LANCS_RXACTH_SHIFT)) & MIPICSI2_RX_LANCS_RXACTH_MASK)

#define MIPICSI2_RX_LANCS_DSTOP_MASK             (0x8U)
#define MIPICSI2_RX_LANCS_DSTOP_SHIFT            (3U)
#define MIPICSI2_RX_LANCS_DSTOP_WIDTH            (1U)
#define MIPICSI2_RX_LANCS_DSTOP(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_LANCS_DSTOP_SHIFT)) & MIPICSI2_RX_LANCS_DSTOP_MASK)

#define MIPICSI2_RX_LANCS_DULPA_MASK             (0x10U)
#define MIPICSI2_RX_LANCS_DULPA_SHIFT            (4U)
#define MIPICSI2_RX_LANCS_DULPA_WIDTH            (1U)
#define MIPICSI2_RX_LANCS_DULPA(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_LANCS_DULPA_SHIFT)) & MIPICSI2_RX_LANCS_DULPA_MASK)

#define MIPICSI2_RX_LANCS_DULMA_MASK             (0x20U)
#define MIPICSI2_RX_LANCS_DULMA_SHIFT            (5U)
#define MIPICSI2_RX_LANCS_DULMA_WIDTH            (1U)
#define MIPICSI2_RX_LANCS_DULMA(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_LANCS_DULMA_SHIFT)) & MIPICSI2_RX_LANCS_DULMA_MASK)
/*! @} */

/*! @name RX_SR - Soft Reset Config */
/*! @{ */

#define MIPICSI2_RX_SR_SOFRST_MASK               (0x80000000U)
#define MIPICSI2_RX_SR_SOFRST_SHIFT              (31U)
#define MIPICSI2_RX_SR_SOFRST_WIDTH              (1U)
#define MIPICSI2_RX_SR_SOFRST(x)                 (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_SR_SOFRST_SHIFT)) & MIPICSI2_RX_SR_SOFRST_MASK)
/*! @} */

/*! @name RX_VCENABLE - Receive Virtual Channel Enable Configuration */
/*! @{ */

#define MIPICSI2_RX_VCENABLE_VC0EN_MASK          (0x1U)
#define MIPICSI2_RX_VCENABLE_VC0EN_SHIFT         (0U)
#define MIPICSI2_RX_VCENABLE_VC0EN_WIDTH         (1U)
#define MIPICSI2_RX_VCENABLE_VC0EN(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCENABLE_VC0EN_SHIFT)) & MIPICSI2_RX_VCENABLE_VC0EN_MASK)

#define MIPICSI2_RX_VCENABLE_VC1EN_MASK          (0x2U)
#define MIPICSI2_RX_VCENABLE_VC1EN_SHIFT         (1U)
#define MIPICSI2_RX_VCENABLE_VC1EN_WIDTH         (1U)
#define MIPICSI2_RX_VCENABLE_VC1EN(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCENABLE_VC1EN_SHIFT)) & MIPICSI2_RX_VCENABLE_VC1EN_MASK)

#define MIPICSI2_RX_VCENABLE_VC2EN_MASK          (0x4U)
#define MIPICSI2_RX_VCENABLE_VC2EN_SHIFT         (2U)
#define MIPICSI2_RX_VCENABLE_VC2EN_WIDTH         (1U)
#define MIPICSI2_RX_VCENABLE_VC2EN(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCENABLE_VC2EN_SHIFT)) & MIPICSI2_RX_VCENABLE_VC2EN_MASK)

#define MIPICSI2_RX_VCENABLE_VC3EN_MASK          (0x8U)
#define MIPICSI2_RX_VCENABLE_VC3EN_SHIFT         (3U)
#define MIPICSI2_RX_VCENABLE_VC3EN_WIDTH         (1U)
#define MIPICSI2_RX_VCENABLE_VC3EN(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCENABLE_VC3EN_SHIFT)) & MIPICSI2_RX_VCENABLE_VC3EN_MASK)
/*! @} */

/*! @name RX_DATAIDR - Receive Data ID Report */
/*! @{ */

#define MIPICSI2_RX_DATAIDR_DATAID_MASK          (0x3FU)
#define MIPICSI2_RX_DATAIDR_DATAID_SHIFT         (0U)
#define MIPICSI2_RX_DATAIDR_DATAID_WIDTH         (6U)
#define MIPICSI2_RX_DATAIDR_DATAID(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DATAIDR_DATAID_SHIFT)) & MIPICSI2_RX_DATAIDR_DATAID_MASK)

#define MIPICSI2_RX_DATAIDR_VCID_MASK            (0xC0U)
#define MIPICSI2_RX_DATAIDR_VCID_SHIFT           (6U)
#define MIPICSI2_RX_DATAIDR_VCID_WIDTH           (2U)
#define MIPICSI2_RX_DATAIDR_VCID(x)              (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DATAIDR_VCID_SHIFT)) & MIPICSI2_RX_DATAIDR_VCID_MASK)
/*! @} */

/*! @name RX_INVIDR - Receive Invalid Data ID Report */
/*! @{ */

#define MIPICSI2_RX_INVIDR_DATAID_MASK           (0x3FU)
#define MIPICSI2_RX_INVIDR_DATAID_SHIFT          (0U)
#define MIPICSI2_RX_INVIDR_DATAID_WIDTH          (6U)
#define MIPICSI2_RX_INVIDR_DATAID(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_INVIDR_DATAID_SHIFT)) & MIPICSI2_RX_INVIDR_DATAID_MASK)

#define MIPICSI2_RX_INVIDR_VCID_MASK             (0xC0U)
#define MIPICSI2_RX_INVIDR_VCID_SHIFT            (6U)
#define MIPICSI2_RX_INVIDR_VCID_WIDTH            (2U)
#define MIPICSI2_RX_INVIDR_VCID(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_INVIDR_VCID_SHIFT)) & MIPICSI2_RX_INVIDR_VCID_MASK)
/*! @} */

/*! @name RX_GNSPR_VC - Receive Generic Short Packet Report */
/*! @{ */

#define MIPICSI2_RX_GNSPR_VC_DATAID_MASK         (0x3FU)
#define MIPICSI2_RX_GNSPR_VC_DATAID_SHIFT        (0U)
#define MIPICSI2_RX_GNSPR_VC_DATAID_WIDTH        (6U)
#define MIPICSI2_RX_GNSPR_VC_DATAID(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_GNSPR_VC_DATAID_SHIFT)) & MIPICSI2_RX_GNSPR_VC_DATAID_MASK)

#define MIPICSI2_RX_GNSPR_VC_DATA_MASK           (0x3FFFC0U)
#define MIPICSI2_RX_GNSPR_VC_DATA_SHIFT          (6U)
#define MIPICSI2_RX_GNSPR_VC_DATA_WIDTH          (16U)
#define MIPICSI2_RX_GNSPR_VC_DATA(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_GNSPR_VC_DATA_SHIFT)) & MIPICSI2_RX_GNSPR_VC_DATA_MASK)
/*! @} */

/*! @name RX_NUMPKTS_VC - Receive Number of Packets for VC */
/*! @{ */

#define MIPICSI2_RX_NUMPKTS_VC_SHORTPKTS_MASK    (0xFFFFU)
#define MIPICSI2_RX_NUMPKTS_VC_SHORTPKTS_SHIFT   (0U)
#define MIPICSI2_RX_NUMPKTS_VC_SHORTPKTS_WIDTH   (16U)
#define MIPICSI2_RX_NUMPKTS_VC_SHORTPKTS(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_NUMPKTS_VC_SHORTPKTS_SHIFT)) & MIPICSI2_RX_NUMPKTS_VC_SHORTPKTS_MASK)

#define MIPICSI2_RX_NUMPKTS_VC_LONGPKTS_MASK     (0xFFFF0000U)
#define MIPICSI2_RX_NUMPKTS_VC_LONGPKTS_SHIFT    (16U)
#define MIPICSI2_RX_NUMPKTS_VC_LONGPKTS_WIDTH    (16U)
#define MIPICSI2_RX_NUMPKTS_VC_LONGPKTS(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_NUMPKTS_VC_LONGPKTS_SHIFT)) & MIPICSI2_RX_NUMPKTS_VC_LONGPKTS_MASK)
/*! @} */

/*! @name RX_VCINTRS - Receive VC Data Interrupt Status */
/*! @{ */

#define MIPICSI2_RX_VCINTRS_FS0_MASK             (0x1U)
#define MIPICSI2_RX_VCINTRS_FS0_SHIFT            (0U)
#define MIPICSI2_RX_VCINTRS_FS0_WIDTH            (1U)
#define MIPICSI2_RX_VCINTRS_FS0(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_FS0_SHIFT)) & MIPICSI2_RX_VCINTRS_FS0_MASK)

#define MIPICSI2_RX_VCINTRS_FE0_MASK             (0x2U)
#define MIPICSI2_RX_VCINTRS_FE0_SHIFT            (1U)
#define MIPICSI2_RX_VCINTRS_FE0_WIDTH            (1U)
#define MIPICSI2_RX_VCINTRS_FE0(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_FE0_SHIFT)) & MIPICSI2_RX_VCINTRS_FE0_MASK)

#define MIPICSI2_RX_VCINTRS_GNSP0_MASK           (0x4U)
#define MIPICSI2_RX_VCINTRS_GNSP0_SHIFT          (2U)
#define MIPICSI2_RX_VCINTRS_GNSP0_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRS_GNSP0(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_GNSP0_SHIFT)) & MIPICSI2_RX_VCINTRS_GNSP0_MASK)

#define MIPICSI2_RX_VCINTRS_FS1_MASK             (0x8U)
#define MIPICSI2_RX_VCINTRS_FS1_SHIFT            (3U)
#define MIPICSI2_RX_VCINTRS_FS1_WIDTH            (1U)
#define MIPICSI2_RX_VCINTRS_FS1(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_FS1_SHIFT)) & MIPICSI2_RX_VCINTRS_FS1_MASK)

#define MIPICSI2_RX_VCINTRS_FE1_MASK             (0x10U)
#define MIPICSI2_RX_VCINTRS_FE1_SHIFT            (4U)
#define MIPICSI2_RX_VCINTRS_FE1_WIDTH            (1U)
#define MIPICSI2_RX_VCINTRS_FE1(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_FE1_SHIFT)) & MIPICSI2_RX_VCINTRS_FE1_MASK)

#define MIPICSI2_RX_VCINTRS_GNSP1_MASK           (0x20U)
#define MIPICSI2_RX_VCINTRS_GNSP1_SHIFT          (5U)
#define MIPICSI2_RX_VCINTRS_GNSP1_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRS_GNSP1(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_GNSP1_SHIFT)) & MIPICSI2_RX_VCINTRS_GNSP1_MASK)

#define MIPICSI2_RX_VCINTRS_FS2_MASK             (0x40U)
#define MIPICSI2_RX_VCINTRS_FS2_SHIFT            (6U)
#define MIPICSI2_RX_VCINTRS_FS2_WIDTH            (1U)
#define MIPICSI2_RX_VCINTRS_FS2(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_FS2_SHIFT)) & MIPICSI2_RX_VCINTRS_FS2_MASK)

#define MIPICSI2_RX_VCINTRS_FE2_MASK             (0x80U)
#define MIPICSI2_RX_VCINTRS_FE2_SHIFT            (7U)
#define MIPICSI2_RX_VCINTRS_FE2_WIDTH            (1U)
#define MIPICSI2_RX_VCINTRS_FE2(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_FE2_SHIFT)) & MIPICSI2_RX_VCINTRS_FE2_MASK)

#define MIPICSI2_RX_VCINTRS_GNSP2_MASK           (0x100U)
#define MIPICSI2_RX_VCINTRS_GNSP2_SHIFT          (8U)
#define MIPICSI2_RX_VCINTRS_GNSP2_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRS_GNSP2(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_GNSP2_SHIFT)) & MIPICSI2_RX_VCINTRS_GNSP2_MASK)

#define MIPICSI2_RX_VCINTRS_FS3_MASK             (0x200U)
#define MIPICSI2_RX_VCINTRS_FS3_SHIFT            (9U)
#define MIPICSI2_RX_VCINTRS_FS3_WIDTH            (1U)
#define MIPICSI2_RX_VCINTRS_FS3(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_FS3_SHIFT)) & MIPICSI2_RX_VCINTRS_FS3_MASK)

#define MIPICSI2_RX_VCINTRS_FE3_MASK             (0x400U)
#define MIPICSI2_RX_VCINTRS_FE3_SHIFT            (10U)
#define MIPICSI2_RX_VCINTRS_FE3_WIDTH            (1U)
#define MIPICSI2_RX_VCINTRS_FE3(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_FE3_SHIFT)) & MIPICSI2_RX_VCINTRS_FE3_MASK)

#define MIPICSI2_RX_VCINTRS_GNSP3_MASK           (0x800U)
#define MIPICSI2_RX_VCINTRS_GNSP3_SHIFT          (11U)
#define MIPICSI2_RX_VCINTRS_GNSP3_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRS_GNSP3(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRS_GNSP3_SHIFT)) & MIPICSI2_RX_VCINTRS_GNSP3_MASK)
/*! @} */

/*! @name RX_VCINTRE - Receive Data VC Event Interrupt Enable */
/*! @{ */

#define MIPICSI2_RX_VCINTRE_FSIE0_MASK           (0x1U)
#define MIPICSI2_RX_VCINTRE_FSIE0_SHIFT          (0U)
#define MIPICSI2_RX_VCINTRE_FSIE0_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRE_FSIE0(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_FSIE0_SHIFT)) & MIPICSI2_RX_VCINTRE_FSIE0_MASK)

#define MIPICSI2_RX_VCINTRE_FEIE0_MASK           (0x2U)
#define MIPICSI2_RX_VCINTRE_FEIE0_SHIFT          (1U)
#define MIPICSI2_RX_VCINTRE_FEIE0_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRE_FEIE0(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_FEIE0_SHIFT)) & MIPICSI2_RX_VCINTRE_FEIE0_MASK)

#define MIPICSI2_RX_VCINTRE_GNSPIE0_MASK         (0x4U)
#define MIPICSI2_RX_VCINTRE_GNSPIE0_SHIFT        (2U)
#define MIPICSI2_RX_VCINTRE_GNSPIE0_WIDTH        (1U)
#define MIPICSI2_RX_VCINTRE_GNSPIE0(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_GNSPIE0_SHIFT)) & MIPICSI2_RX_VCINTRE_GNSPIE0_MASK)

#define MIPICSI2_RX_VCINTRE_FSIE1_MASK           (0x8U)
#define MIPICSI2_RX_VCINTRE_FSIE1_SHIFT          (3U)
#define MIPICSI2_RX_VCINTRE_FSIE1_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRE_FSIE1(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_FSIE1_SHIFT)) & MIPICSI2_RX_VCINTRE_FSIE1_MASK)

#define MIPICSI2_RX_VCINTRE_FEIE1_MASK           (0x10U)
#define MIPICSI2_RX_VCINTRE_FEIE1_SHIFT          (4U)
#define MIPICSI2_RX_VCINTRE_FEIE1_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRE_FEIE1(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_FEIE1_SHIFT)) & MIPICSI2_RX_VCINTRE_FEIE1_MASK)

#define MIPICSI2_RX_VCINTRE_GNSPIE1_MASK         (0x20U)
#define MIPICSI2_RX_VCINTRE_GNSPIE1_SHIFT        (5U)
#define MIPICSI2_RX_VCINTRE_GNSPIE1_WIDTH        (1U)
#define MIPICSI2_RX_VCINTRE_GNSPIE1(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_GNSPIE1_SHIFT)) & MIPICSI2_RX_VCINTRE_GNSPIE1_MASK)

#define MIPICSI2_RX_VCINTRE_FSIE2_MASK           (0x40U)
#define MIPICSI2_RX_VCINTRE_FSIE2_SHIFT          (6U)
#define MIPICSI2_RX_VCINTRE_FSIE2_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRE_FSIE2(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_FSIE2_SHIFT)) & MIPICSI2_RX_VCINTRE_FSIE2_MASK)

#define MIPICSI2_RX_VCINTRE_FEIE2_MASK           (0x80U)
#define MIPICSI2_RX_VCINTRE_FEIE2_SHIFT          (7U)
#define MIPICSI2_RX_VCINTRE_FEIE2_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRE_FEIE2(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_FEIE2_SHIFT)) & MIPICSI2_RX_VCINTRE_FEIE2_MASK)

#define MIPICSI2_RX_VCINTRE_GNSPIE2_MASK         (0x100U)
#define MIPICSI2_RX_VCINTRE_GNSPIE2_SHIFT        (8U)
#define MIPICSI2_RX_VCINTRE_GNSPIE2_WIDTH        (1U)
#define MIPICSI2_RX_VCINTRE_GNSPIE2(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_GNSPIE2_SHIFT)) & MIPICSI2_RX_VCINTRE_GNSPIE2_MASK)

#define MIPICSI2_RX_VCINTRE_FSIE3_MASK           (0x200U)
#define MIPICSI2_RX_VCINTRE_FSIE3_SHIFT          (9U)
#define MIPICSI2_RX_VCINTRE_FSIE3_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRE_FSIE3(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_FSIE3_SHIFT)) & MIPICSI2_RX_VCINTRE_FSIE3_MASK)

#define MIPICSI2_RX_VCINTRE_FEIE3_MASK           (0x400U)
#define MIPICSI2_RX_VCINTRE_FEIE3_SHIFT          (10U)
#define MIPICSI2_RX_VCINTRE_FEIE3_WIDTH          (1U)
#define MIPICSI2_RX_VCINTRE_FEIE3(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_FEIE3_SHIFT)) & MIPICSI2_RX_VCINTRE_FEIE3_MASK)

#define MIPICSI2_RX_VCINTRE_GNSPIE3_MASK         (0x800U)
#define MIPICSI2_RX_VCINTRE_GNSPIE3_SHIFT        (11U)
#define MIPICSI2_RX_VCINTRE_GNSPIE3_WIDTH        (1U)
#define MIPICSI2_RX_VCINTRE_GNSPIE3(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_VCINTRE_GNSPIE3_SHIFT)) & MIPICSI2_RX_VCINTRE_GNSPIE3_MASK)
/*! @} */

/*! @name CONTROLLER_STATUS_REGISTER - Controller Status */
/*! @{ */

#define MIPICSI2_CONTROLLER_STATUS_REGISTER_ECC_RECEIVED_MASK (0x3FU)
#define MIPICSI2_CONTROLLER_STATUS_REGISTER_ECC_RECEIVED_SHIFT (0U)
#define MIPICSI2_CONTROLLER_STATUS_REGISTER_ECC_RECEIVED_WIDTH (6U)
#define MIPICSI2_CONTROLLER_STATUS_REGISTER_ECC_RECEIVED(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CONTROLLER_STATUS_REGISTER_ECC_RECEIVED_SHIFT)) & MIPICSI2_CONTROLLER_STATUS_REGISTER_ECC_RECEIVED_MASK)
/*! @} */

/*! @name CRC_REGISTER - CRC */
/*! @{ */

#define MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_RECEIVED_MASK (0xFFFFU)
#define MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_RECEIVED_SHIFT (0U)
#define MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_RECEIVED_WIDTH (16U)
#define MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_RECEIVED(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_RECEIVED_SHIFT)) & MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_RECEIVED_MASK)

#define MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_CALCULATED_MASK (0xFFFF0000U)
#define MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_CALCULATED_SHIFT (16U)
#define MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_CALCULATED_WIDTH (16U)
#define MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_CALCULATED(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_CALCULATED_SHIFT)) & MIPICSI2_CRC_REGISTER_PAYLOAD_CRC_CALCULATED_MASK)
/*! @} */

/*! @name CONTROLLER_ERR_STATUS_REGISTER - Controller Error Status */
/*! @{ */

#define MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_EXIT_HS_ERROR_MASK (0x1U)
#define MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_EXIT_HS_ERROR_SHIFT (0U)
#define MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_EXIT_HS_ERROR_WIDTH (1U)
#define MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_EXIT_HS_ERROR(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_EXIT_HS_ERROR_SHIFT)) & MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_EXIT_HS_ERROR_MASK)

#define MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_FIFO_OVERFLOW_ERROR_MASK (0x2U)
#define MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_FIFO_OVERFLOW_ERROR_SHIFT (1U)
#define MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_FIFO_OVERFLOW_ERROR_WIDTH (1U)
#define MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_FIFO_OVERFLOW_ERROR(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_FIFO_OVERFLOW_ERROR_SHIFT)) & MIPICSI2_CONTROLLER_ERR_STATUS_REGISTER_FIFO_OVERFLOW_ERROR_MASK)
/*! @} */

/*! @name CONTROLLER_ERR_IE - Controller Interrupt Enable */
/*! @{ */

#define MIPICSI2_CONTROLLER_ERR_IE_FIFO_OVERFLOW_ERRIE_MASK (0x1U)
#define MIPICSI2_CONTROLLER_ERR_IE_FIFO_OVERFLOW_ERRIE_SHIFT (0U)
#define MIPICSI2_CONTROLLER_ERR_IE_FIFO_OVERFLOW_ERRIE_WIDTH (1U)
#define MIPICSI2_CONTROLLER_ERR_IE_FIFO_OVERFLOW_ERRIE(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CONTROLLER_ERR_IE_FIFO_OVERFLOW_ERRIE_SHIFT)) & MIPICSI2_CONTROLLER_ERR_IE_FIFO_OVERFLOW_ERRIE_MASK)

#define MIPICSI2_CONTROLLER_ERR_IE_HS_EXIT_ERRIE_MASK (0x2U)
#define MIPICSI2_CONTROLLER_ERR_IE_HS_EXIT_ERRIE_SHIFT (1U)
#define MIPICSI2_CONTROLLER_ERR_IE_HS_EXIT_ERRIE_WIDTH (1U)
#define MIPICSI2_CONTROLLER_ERR_IE_HS_EXIT_ERRIE(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CONTROLLER_ERR_IE_HS_EXIT_ERRIE_SHIFT)) & MIPICSI2_CONTROLLER_ERR_IE_HS_EXIT_ERRIE_MASK)
/*! @} */

/*! @name PPERRIS - Receive Data Protocol and Packet Error Interrupt Status for VC0..Receive Data Protocol and Packet Error Interrupt Status for VC3 */
/*! @{ */

#define MIPICSI2_PPERRIS_ECCONE_MASK             (0x1U)
#define MIPICSI2_PPERRIS_ECCONE_SHIFT            (0U)
#define MIPICSI2_PPERRIS_ECCONE_WIDTH            (1U)
#define MIPICSI2_PPERRIS_ECCONE(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIS_ECCONE_SHIFT)) & MIPICSI2_PPERRIS_ECCONE_MASK)

#define MIPICSI2_PPERRIS_ECCTWO_MASK             (0x2U)
#define MIPICSI2_PPERRIS_ECCTWO_SHIFT            (1U)
#define MIPICSI2_PPERRIS_ECCTWO_WIDTH            (1U)
#define MIPICSI2_PPERRIS_ECCTWO(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIS_ECCTWO_SHIFT)) & MIPICSI2_PPERRIS_ECCTWO_MASK)

#define MIPICSI2_PPERRIS_ERFSYN_MASK             (0x4U)
#define MIPICSI2_PPERRIS_ERFSYN_SHIFT            (2U)
#define MIPICSI2_PPERRIS_ERFSYN_WIDTH            (1U)
#define MIPICSI2_PPERRIS_ERFSYN(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIS_ERFSYN_SHIFT)) & MIPICSI2_PPERRIS_ERFSYN_MASK)

#define MIPICSI2_PPERRIS_ERFDAT_MASK             (0x8U)
#define MIPICSI2_PPERRIS_ERFDAT_SHIFT            (3U)
#define MIPICSI2_PPERRIS_ERFDAT_WIDTH            (1U)
#define MIPICSI2_PPERRIS_ERFDAT(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIS_ERFDAT_SHIFT)) & MIPICSI2_PPERRIS_ERFDAT_MASK)

#define MIPICSI2_PPERRIS_CRCERR_MASK             (0x10U)
#define MIPICSI2_PPERRIS_CRCERR_SHIFT            (4U)
#define MIPICSI2_PPERRIS_CRCERR_WIDTH            (1U)
#define MIPICSI2_PPERRIS_CRCERR(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIS_CRCERR_SHIFT)) & MIPICSI2_PPERRIS_CRCERR_MASK)

#define MIPICSI2_PPERRIS_INVIDERR_MASK           (0x20U)
#define MIPICSI2_PPERRIS_INVIDERR_SHIFT          (5U)
#define MIPICSI2_PPERRIS_INVIDERR_WIDTH          (1U)
#define MIPICSI2_PPERRIS_INVIDERR(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIS_INVIDERR_SHIFT)) & MIPICSI2_PPERRIS_INVIDERR_MASK)

#define MIPICSI2_PPERRIS_Reserved_MASK           (0x40U)
#define MIPICSI2_PPERRIS_Reserved_SHIFT          (6U)
#define MIPICSI2_PPERRIS_Reserved_WIDTH          (1U)
#define MIPICSI2_PPERRIS_Reserved(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIS_Reserved_SHIFT)) & MIPICSI2_PPERRIS_Reserved_MASK)
/*! @} */

/*! @name PPERRIE - Receive Data Protocol and Packet Error Interrupt Enable for VC0..Receive Data Protocol and Packet Error Interrupt Enable for VC3 */
/*! @{ */

#define MIPICSI2_PPERRIE_ECCONEIE_MASK           (0x1U)
#define MIPICSI2_PPERRIE_ECCONEIE_SHIFT          (0U)
#define MIPICSI2_PPERRIE_ECCONEIE_WIDTH          (1U)
#define MIPICSI2_PPERRIE_ECCONEIE(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIE_ECCONEIE_SHIFT)) & MIPICSI2_PPERRIE_ECCONEIE_MASK)

#define MIPICSI2_PPERRIE_ECCTWOIE_MASK           (0x2U)
#define MIPICSI2_PPERRIE_ECCTWOIE_SHIFT          (1U)
#define MIPICSI2_PPERRIE_ECCTWOIE_WIDTH          (1U)
#define MIPICSI2_PPERRIE_ECCTWOIE(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIE_ECCTWOIE_SHIFT)) & MIPICSI2_PPERRIE_ECCTWOIE_MASK)

#define MIPICSI2_PPERRIE_ERFSYNIE_MASK           (0x4U)
#define MIPICSI2_PPERRIE_ERFSYNIE_SHIFT          (2U)
#define MIPICSI2_PPERRIE_ERFSYNIE_WIDTH          (1U)
#define MIPICSI2_PPERRIE_ERFSYNIE(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIE_ERFSYNIE_SHIFT)) & MIPICSI2_PPERRIE_ERFSYNIE_MASK)

#define MIPICSI2_PPERRIE_ERFDATIE_MASK           (0x8U)
#define MIPICSI2_PPERRIE_ERFDATIE_SHIFT          (3U)
#define MIPICSI2_PPERRIE_ERFDATIE_WIDTH          (1U)
#define MIPICSI2_PPERRIE_ERFDATIE(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIE_ERFDATIE_SHIFT)) & MIPICSI2_PPERRIE_ERFDATIE_MASK)

#define MIPICSI2_PPERRIE_CRCERRIE_MASK           (0x10U)
#define MIPICSI2_PPERRIE_CRCERRIE_SHIFT          (4U)
#define MIPICSI2_PPERRIE_CRCERRIE_WIDTH          (1U)
#define MIPICSI2_PPERRIE_CRCERRIE(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIE_CRCERRIE_SHIFT)) & MIPICSI2_PPERRIE_CRCERRIE_MASK)

#define MIPICSI2_PPERRIE_INVIDERRIE_MASK         (0x20U)
#define MIPICSI2_PPERRIE_INVIDERRIE_SHIFT        (5U)
#define MIPICSI2_PPERRIE_INVIDERRIE_WIDTH        (1U)
#define MIPICSI2_PPERRIE_INVIDERRIE(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIE_INVIDERRIE_SHIFT)) & MIPICSI2_PPERRIE_INVIDERRIE_MASK)

#define MIPICSI2_PPERRIE_Reserved_MASK           (0x40U)
#define MIPICSI2_PPERRIE_Reserved_SHIFT          (6U)
#define MIPICSI2_PPERRIE_Reserved_WIDTH          (1U)
#define MIPICSI2_PPERRIE_Reserved(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_PPERRIE_Reserved_SHIFT)) & MIPICSI2_PPERRIE_Reserved_MASK)
/*! @} */

/*! @name ERRPOS - Receive ECC 1-Bit Error Position for VC0..Receive ECC 1-Bit Error Position for VC3 */
/*! @{ */

#define MIPICSI2_ERRPOS_ERRPOS_MASK              (0x1FU)
#define MIPICSI2_ERRPOS_ERRPOS_SHIFT             (0U)
#define MIPICSI2_ERRPOS_ERRPOS_WIDTH             (5U)
#define MIPICSI2_ERRPOS_ERRPOS(x)                (((uint32_t)(((uint32_t)(x)) << MIPICSI2_ERRPOS_ERRPOS_SHIFT)) & MIPICSI2_ERRPOS_ERRPOS_MASK)
/*! @} */

/*! @name NUMPPERR - Receive Packets Number of Protocol Errors for VC0..Receive Packets Number of Protocol Errors for VC3 */
/*! @{ */

#define MIPICSI2_NUMPPERR_NUMCRCERR_MASK         (0xFFFFU)
#define MIPICSI2_NUMPPERR_NUMCRCERR_SHIFT        (0U)
#define MIPICSI2_NUMPPERR_NUMCRCERR_WIDTH        (16U)
#define MIPICSI2_NUMPPERR_NUMCRCERR(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_NUMPPERR_NUMCRCERR_SHIFT)) & MIPICSI2_NUMPPERR_NUMCRCERR_MASK)

#define MIPICSI2_NUMPPERR_NUMECCERR_MASK         (0xFFFF0000U)
#define MIPICSI2_NUMPPERR_NUMECCERR_SHIFT        (16U)
#define MIPICSI2_NUMPPERR_NUMECCERR_WIDTH        (16U)
#define MIPICSI2_NUMPPERR_NUMECCERR(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_NUMPPERR_NUMECCERR_SHIFT)) & MIPICSI2_NUMPPERR_NUMECCERR_MASK)
/*! @} */

/*! @name RX_PHYERRIS - Receive Data PHY Level Error Interrupt Status */
/*! @{ */

#define MIPICSI2_RX_PHYERRIS_ERRSY0_MASK         (0x1U)
#define MIPICSI2_RX_PHYERRIS_ERRSY0_SHIFT        (0U)
#define MIPICSI2_RX_PHYERRIS_ERRSY0_WIDTH        (1U)
#define MIPICSI2_RX_PHYERRIS_ERRSY0(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERRSY0_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERRSY0_MASK)

#define MIPICSI2_RX_PHYERRIS_NOSYN0_MASK         (0x2U)
#define MIPICSI2_RX_PHYERRIS_NOSYN0_SHIFT        (1U)
#define MIPICSI2_RX_PHYERRIS_NOSYN0_WIDTH        (1U)
#define MIPICSI2_RX_PHYERRIS_NOSYN0(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_NOSYN0_SHIFT)) & MIPICSI2_RX_PHYERRIS_NOSYN0_MASK)

#define MIPICSI2_RX_PHYERRIS_ERRESC0_MASK        (0x4U)
#define MIPICSI2_RX_PHYERRIS_ERRESC0_SHIFT       (2U)
#define MIPICSI2_RX_PHYERRIS_ERRESC0_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERRESC0(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERRESC0_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERRESC0_MASK)

#define MIPICSI2_RX_PHYERRIS_ERSYES0_MASK        (0x8U)
#define MIPICSI2_RX_PHYERRIS_ERSYES0_SHIFT       (3U)
#define MIPICSI2_RX_PHYERRIS_ERSYES0_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERSYES0(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERSYES0_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERSYES0_MASK)

#define MIPICSI2_RX_PHYERRIS_ERCTRL0_MASK        (0x10U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL0_SHIFT       (4U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL0_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL0(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERCTRL0_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERCTRL0_MASK)

#define MIPICSI2_RX_PHYERRIS_ERRSY1_MASK         (0x20U)
#define MIPICSI2_RX_PHYERRIS_ERRSY1_SHIFT        (5U)
#define MIPICSI2_RX_PHYERRIS_ERRSY1_WIDTH        (1U)
#define MIPICSI2_RX_PHYERRIS_ERRSY1(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERRSY1_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERRSY1_MASK)

#define MIPICSI2_RX_PHYERRIS_NOSYN1_MASK         (0x40U)
#define MIPICSI2_RX_PHYERRIS_NOSYN1_SHIFT        (6U)
#define MIPICSI2_RX_PHYERRIS_NOSYN1_WIDTH        (1U)
#define MIPICSI2_RX_PHYERRIS_NOSYN1(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_NOSYN1_SHIFT)) & MIPICSI2_RX_PHYERRIS_NOSYN1_MASK)

#define MIPICSI2_RX_PHYERRIS_ERRESC1_MASK        (0x80U)
#define MIPICSI2_RX_PHYERRIS_ERRESC1_SHIFT       (7U)
#define MIPICSI2_RX_PHYERRIS_ERRESC1_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERRESC1(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERRESC1_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERRESC1_MASK)

#define MIPICSI2_RX_PHYERRIS_ERSYES1_MASK        (0x100U)
#define MIPICSI2_RX_PHYERRIS_ERSYES1_SHIFT       (8U)
#define MIPICSI2_RX_PHYERRIS_ERSYES1_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERSYES1(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERSYES1_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERSYES1_MASK)

#define MIPICSI2_RX_PHYERRIS_ERCTRL1_MASK        (0x200U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL1_SHIFT       (9U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL1_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL1(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERCTRL1_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERCTRL1_MASK)

#define MIPICSI2_RX_PHYERRIS_ERRSY2_MASK         (0x400U)
#define MIPICSI2_RX_PHYERRIS_ERRSY2_SHIFT        (10U)
#define MIPICSI2_RX_PHYERRIS_ERRSY2_WIDTH        (1U)
#define MIPICSI2_RX_PHYERRIS_ERRSY2(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERRSY2_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERRSY2_MASK)

#define MIPICSI2_RX_PHYERRIS_NOSYN2_MASK         (0x800U)
#define MIPICSI2_RX_PHYERRIS_NOSYN2_SHIFT        (11U)
#define MIPICSI2_RX_PHYERRIS_NOSYN2_WIDTH        (1U)
#define MIPICSI2_RX_PHYERRIS_NOSYN2(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_NOSYN2_SHIFT)) & MIPICSI2_RX_PHYERRIS_NOSYN2_MASK)

#define MIPICSI2_RX_PHYERRIS_ERRESC2_MASK        (0x1000U)
#define MIPICSI2_RX_PHYERRIS_ERRESC2_SHIFT       (12U)
#define MIPICSI2_RX_PHYERRIS_ERRESC2_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERRESC2(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERRESC2_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERRESC2_MASK)

#define MIPICSI2_RX_PHYERRIS_ERSYES2_MASK        (0x2000U)
#define MIPICSI2_RX_PHYERRIS_ERSYES2_SHIFT       (13U)
#define MIPICSI2_RX_PHYERRIS_ERSYES2_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERSYES2(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERSYES2_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERSYES2_MASK)

#define MIPICSI2_RX_PHYERRIS_ERCTRL2_MASK        (0x4000U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL2_SHIFT       (14U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL2_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL2(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERCTRL2_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERCTRL2_MASK)

#define MIPICSI2_RX_PHYERRIS_ERRSY3_MASK         (0x8000U)
#define MIPICSI2_RX_PHYERRIS_ERRSY3_SHIFT        (15U)
#define MIPICSI2_RX_PHYERRIS_ERRSY3_WIDTH        (1U)
#define MIPICSI2_RX_PHYERRIS_ERRSY3(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERRSY3_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERRSY3_MASK)

#define MIPICSI2_RX_PHYERRIS_NOSYN3_MASK         (0x10000U)
#define MIPICSI2_RX_PHYERRIS_NOSYN3_SHIFT        (16U)
#define MIPICSI2_RX_PHYERRIS_NOSYN3_WIDTH        (1U)
#define MIPICSI2_RX_PHYERRIS_NOSYN3(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_NOSYN3_SHIFT)) & MIPICSI2_RX_PHYERRIS_NOSYN3_MASK)

#define MIPICSI2_RX_PHYERRIS_ERRESC3_MASK        (0x20000U)
#define MIPICSI2_RX_PHYERRIS_ERRESC3_SHIFT       (17U)
#define MIPICSI2_RX_PHYERRIS_ERRESC3_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERRESC3(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERRESC3_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERRESC3_MASK)

#define MIPICSI2_RX_PHYERRIS_ERSYES3_MASK        (0x40000U)
#define MIPICSI2_RX_PHYERRIS_ERSYES3_SHIFT       (18U)
#define MIPICSI2_RX_PHYERRIS_ERSYES3_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERSYES3(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERSYES3_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERSYES3_MASK)

#define MIPICSI2_RX_PHYERRIS_ERCTRL3_MASK        (0x80000U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL3_SHIFT       (19U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL3_WIDTH       (1U)
#define MIPICSI2_RX_PHYERRIS_ERCTRL3(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIS_ERCTRL3_SHIFT)) & MIPICSI2_RX_PHYERRIS_ERCTRL3_MASK)
/*! @} */

/*! @name RX_PHYERRIE - Receive Data PHY Level Error Interrupt Enable */
/*! @{ */

#define MIPICSI2_RX_PHYERRIE_ERRSYIE0_MASK       (0x1U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE0_SHIFT      (0U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE0_WIDTH      (1U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE0(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERRSYIE0_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERRSYIE0_MASK)

#define MIPICSI2_RX_PHYERRIE_NOSYNIE0_MASK       (0x2U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE0_SHIFT      (1U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE0_WIDTH      (1U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE0(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_NOSYNIE0_SHIFT)) & MIPICSI2_RX_PHYERRIE_NOSYNIE0_MASK)

#define MIPICSI2_RX_PHYERRIE_ERRESCIE0_MASK      (0x4U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE0_SHIFT     (2U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE0_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE0(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERRESCIE0_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERRESCIE0_MASK)

#define MIPICSI2_RX_PHYERRIE_ERSYESIE0_MASK      (0x8U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE0_SHIFT     (3U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE0_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE0(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERSYESIE0_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERSYESIE0_MASK)

#define MIPICSI2_RX_PHYERRIE_ERCTRLIE0_MASK      (0x10U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE0_SHIFT     (4U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE0_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE0(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERCTRLIE0_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERCTRLIE0_MASK)

#define MIPICSI2_RX_PHYERRIE_ERRSYIE1_MASK       (0x20U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE1_SHIFT      (5U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE1_WIDTH      (1U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE1(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERRSYIE1_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERRSYIE1_MASK)

#define MIPICSI2_RX_PHYERRIE_NOSYNIE1_MASK       (0x40U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE1_SHIFT      (6U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE1_WIDTH      (1U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE1(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_NOSYNIE1_SHIFT)) & MIPICSI2_RX_PHYERRIE_NOSYNIE1_MASK)

#define MIPICSI2_RX_PHYERRIE_ERRESCIE1_MASK      (0x80U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE1_SHIFT     (7U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE1_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE1(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERRESCIE1_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERRESCIE1_MASK)

#define MIPICSI2_RX_PHYERRIE_ERSYESIE1_MASK      (0x100U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE1_SHIFT     (8U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE1_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE1(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERSYESIE1_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERSYESIE1_MASK)

#define MIPICSI2_RX_PHYERRIE_ERCTRLIE1_MASK      (0x200U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE1_SHIFT     (9U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE1_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE1(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERCTRLIE1_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERCTRLIE1_MASK)

#define MIPICSI2_RX_PHYERRIE_ERRSYIE2_MASK       (0x400U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE2_SHIFT      (10U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE2_WIDTH      (1U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE2(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERRSYIE2_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERRSYIE2_MASK)

#define MIPICSI2_RX_PHYERRIE_NOSYNIE2_MASK       (0x800U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE2_SHIFT      (11U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE2_WIDTH      (1U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE2(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_NOSYNIE2_SHIFT)) & MIPICSI2_RX_PHYERRIE_NOSYNIE2_MASK)

#define MIPICSI2_RX_PHYERRIE_ERRESCIE2_MASK      (0x1000U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE2_SHIFT     (12U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE2_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE2(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERRESCIE2_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERRESCIE2_MASK)

#define MIPICSI2_RX_PHYERRIE_ERSYESIE2_MASK      (0x2000U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE2_SHIFT     (13U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE2_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE2(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERSYESIE2_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERSYESIE2_MASK)

#define MIPICSI2_RX_PHYERRIE_ERCTRLIE2_MASK      (0x4000U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE2_SHIFT     (14U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE2_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE2(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERCTRLIE2_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERCTRLIE2_MASK)

#define MIPICSI2_RX_PHYERRIE_ERRSYIE3_MASK       (0x8000U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE3_SHIFT      (15U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE3_WIDTH      (1U)
#define MIPICSI2_RX_PHYERRIE_ERRSYIE3(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERRSYIE3_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERRSYIE3_MASK)

#define MIPICSI2_RX_PHYERRIE_NOSYNIE3_MASK       (0x10000U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE3_SHIFT      (16U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE3_WIDTH      (1U)
#define MIPICSI2_RX_PHYERRIE_NOSYNIE3(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_NOSYNIE3_SHIFT)) & MIPICSI2_RX_PHYERRIE_NOSYNIE3_MASK)

#define MIPICSI2_RX_PHYERRIE_ERRESCIE3_MASK      (0x20000U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE3_SHIFT     (17U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE3_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERRESCIE3(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERRESCIE3_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERRESCIE3_MASK)

#define MIPICSI2_RX_PHYERRIE_ERSYESIE3_MASK      (0x40000U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE3_SHIFT     (18U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE3_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERSYESIE3(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERSYESIE3_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERSYESIE3_MASK)

#define MIPICSI2_RX_PHYERRIE_ERCTRLIE3_MASK      (0x80000U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE3_SHIFT     (19U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE3_WIDTH     (1U)
#define MIPICSI2_RX_PHYERRIE_ERCTRLIE3(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_PHYERRIE_ERCTRLIE3_SHIFT)) & MIPICSI2_RX_PHYERRIE_ERCTRLIE3_MASK)
/*! @} */

/*! @name RX_STAT_CONFIG - Receive Data Statistical Computation Configuration */
/*! @{ */

#define MIPICSI2_RX_STAT_CONFIG_STATEN_MASK      (0x1U)
#define MIPICSI2_RX_STAT_CONFIG_STATEN_SHIFT     (0U)
#define MIPICSI2_RX_STAT_CONFIG_STATEN_WIDTH     (1U)
#define MIPICSI2_RX_STAT_CONFIG_STATEN(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_STAT_CONFIG_STATEN_SHIFT)) & MIPICSI2_RX_STAT_CONFIG_STATEN_MASK)
/*! @} */

/*! @name CBUF_CONFIG - Receive Data Circular Buffer Configuration */
/*! @{ */

#define MIPICSI2_CBUF_CONFIG_TRACE_MASK          (0x1U)
#define MIPICSI2_CBUF_CONFIG_TRACE_SHIFT         (0U)
#define MIPICSI2_CBUF_CONFIG_TRACE_WIDTH         (1U)
#define MIPICSI2_CBUF_CONFIG_TRACE(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_CONFIG_TRACE_SHIFT)) & MIPICSI2_CBUF_CONFIG_TRACE_MASK)

#define MIPICSI2_CBUF_CONFIG_DATAID_MASK         (0xFCU)
#define MIPICSI2_CBUF_CONFIG_DATAID_SHIFT        (2U)
#define MIPICSI2_CBUF_CONFIG_DATAID_WIDTH        (6U)
#define MIPICSI2_CBUF_CONFIG_DATAID(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_CONFIG_DATAID_SHIFT)) & MIPICSI2_CBUF_CONFIG_DATAID_MASK)

#define MIPICSI2_CBUF_CONFIG_VCID_MASK           (0x300U)
#define MIPICSI2_CBUF_CONFIG_VCID_SHIFT          (8U)
#define MIPICSI2_CBUF_CONFIG_VCID_WIDTH          (2U)
#define MIPICSI2_CBUF_CONFIG_VCID(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_CONFIG_VCID_SHIFT)) & MIPICSI2_CBUF_CONFIG_VCID_MASK)

#define MIPICSI2_CBUF_CONFIG_FIFTHCH_ENABLE_MASK (0x400U)
#define MIPICSI2_CBUF_CONFIG_FIFTHCH_ENABLE_SHIFT (10U)
#define MIPICSI2_CBUF_CONFIG_FIFTHCH_ENABLE_WIDTH (1U)
#define MIPICSI2_CBUF_CONFIG_FIFTHCH_ENABLE(x)   (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_CONFIG_FIFTHCH_ENABLE_SHIFT)) & MIPICSI2_CBUF_CONFIG_FIFTHCH_ENABLE_MASK)
/*! @} */

/*! @name INPLINELEN_CONFIG - Receive Data Input Line Length Configuration */
/*! @{ */

#define MIPICSI2_INPLINELEN_CONFIG_INPLINELEN_MASK (0xFFFFU)
#define MIPICSI2_INPLINELEN_CONFIG_INPLINELEN_SHIFT (0U)
#define MIPICSI2_INPLINELEN_CONFIG_INPLINELEN_WIDTH (16U)
#define MIPICSI2_INPLINELEN_CONFIG_INPLINELEN(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_INPLINELEN_CONFIG_INPLINELEN_SHIFT)) & MIPICSI2_INPLINELEN_CONFIG_INPLINELEN_MASK)
/*! @} */

/*! @name LINELEN_CONFIG - Receive Data Line Length Configuration */
/*! @{ */

#define MIPICSI2_LINELEN_CONFIG_LINELEN_MASK     (0xFFFFU)
#define MIPICSI2_LINELEN_CONFIG_LINELEN_SHIFT    (0U)
#define MIPICSI2_LINELEN_CONFIG_LINELEN_WIDTH    (16U)
#define MIPICSI2_LINELEN_CONFIG_LINELEN(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_LINELEN_CONFIG_LINELEN_SHIFT)) & MIPICSI2_LINELEN_CONFIG_LINELEN_MASK)
/*! @} */

/*! @name NUMLINES_CONFIG - Receive Data Expected Number of Lines Configuration */
/*! @{ */

#define MIPICSI2_NUMLINES_CONFIG_NUMLINES_MASK   (0xFFFFU)
#define MIPICSI2_NUMLINES_CONFIG_NUMLINES_SHIFT  (0U)
#define MIPICSI2_NUMLINES_CONFIG_NUMLINES_WIDTH  (16U)
#define MIPICSI2_NUMLINES_CONFIG_NUMLINES(x)     (((uint32_t)(((uint32_t)(x)) << MIPICSI2_NUMLINES_CONFIG_NUMLINES_SHIFT)) & MIPICSI2_NUMLINES_CONFIG_NUMLINES_MASK)
/*! @} */

/*! @name CBUF_SRTPTR - Receive Data Circular Buffer Start Pointer */
/*! @{ */

#define MIPICSI2_CBUF_SRTPTR_STRPTR_MASK         (0xFFFFFFF0U)
#define MIPICSI2_CBUF_SRTPTR_STRPTR_SHIFT        (4U)
#define MIPICSI2_CBUF_SRTPTR_STRPTR_WIDTH        (28U)
#define MIPICSI2_CBUF_SRTPTR_STRPTR(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_SRTPTR_STRPTR_SHIFT)) & MIPICSI2_CBUF_SRTPTR_STRPTR_MASK)
/*! @} */

/*! @name CBUF_BUFLEN - Receive Data Circular Buffer Length */
/*! @{ */

#define MIPICSI2_CBUF_BUFLEN_BUFLEN_MASK         (0xFFFFU)
#define MIPICSI2_CBUF_BUFLEN_BUFLEN_SHIFT        (0U)
#define MIPICSI2_CBUF_BUFLEN_BUFLEN_WIDTH        (16U)
#define MIPICSI2_CBUF_BUFLEN_BUFLEN(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_BUFLEN_BUFLEN_SHIFT)) & MIPICSI2_CBUF_BUFLEN_BUFLEN_MASK)
/*! @} */

/*! @name CBUF_NUMLINE - Receive Data Circular Buffer Number of Lines */
/*! @{ */

#define MIPICSI2_CBUF_NUMLINE_NUMLINES_MASK      (0xFFFFU)
#define MIPICSI2_CBUF_NUMLINE_NUMLINES_SHIFT     (0U)
#define MIPICSI2_CBUF_NUMLINE_NUMLINES_WIDTH     (16U)
#define MIPICSI2_CBUF_NUMLINE_NUMLINES(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_NUMLINE_NUMLINES_SHIFT)) & MIPICSI2_CBUF_NUMLINE_NUMLINES_MASK)
/*! @} */

/*! @name CBUF_LPDI - Receive Data Circular Buffer Lines Done Generation */
/*! @{ */

#define MIPICSI2_CBUF_LPDI_NUMLINES_MASK         (0xFFU)
#define MIPICSI2_CBUF_LPDI_NUMLINES_SHIFT        (0U)
#define MIPICSI2_CBUF_LPDI_NUMLINES_WIDTH        (8U)
#define MIPICSI2_CBUF_LPDI_NUMLINES(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_LPDI_NUMLINES_SHIFT)) & MIPICSI2_CBUF_LPDI_NUMLINES_MASK)
/*! @} */

/*! @name CBUF_NXTLINE - Receive Data Circular Buffer Next Row Indication */
/*! @{ */

#define MIPICSI2_CBUF_NXTLINE_NXTLINE_MASK       (0xFFFFU)
#define MIPICSI2_CBUF_NXTLINE_NXTLINE_SHIFT      (0U)
#define MIPICSI2_CBUF_NXTLINE_NXTLINE_WIDTH      (16U)
#define MIPICSI2_CBUF_NXTLINE_NXTLINE(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_NXTLINE_NXTLINE_SHIFT)) & MIPICSI2_CBUF_NXTLINE_NXTLINE_MASK)
/*! @} */

/*! @name CBUF_RXLINE - Receive Data Circular Buffer Total Lines Received Status */
/*! @{ */

#define MIPICSI2_CBUF_RXLINE_TOTLINES_MASK       (0xFFFFU)
#define MIPICSI2_CBUF_RXLINE_TOTLINES_SHIFT      (0U)
#define MIPICSI2_CBUF_RXLINE_TOTLINES_WIDTH      (16U)
#define MIPICSI2_CBUF_RXLINE_TOTLINES(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_RXLINE_TOTLINES_SHIFT)) & MIPICSI2_CBUF_RXLINE_TOTLINES_MASK)
/*! @} */

/*! @name CBUF_ERRLEN - Receive Data Circular Buffer Error Line Length Status */
/*! @{ */

#define MIPICSI2_CBUF_ERRLEN_ERRLEN_MASK         (0xFFFFU)
#define MIPICSI2_CBUF_ERRLEN_ERRLEN_SHIFT        (0U)
#define MIPICSI2_CBUF_ERRLEN_ERRLEN_WIDTH        (16U)
#define MIPICSI2_CBUF_ERRLEN_ERRLEN(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_ERRLEN_ERRLEN_SHIFT)) & MIPICSI2_CBUF_ERRLEN_ERRLEN_MASK)
/*! @} */

/*! @name CBUF_ERRLINE - Receive Data Circular Buffer Line Number for Incorrect Length Status */
/*! @{ */

#define MIPICSI2_CBUF_ERRLINE_ERRLINE_MASK       (0xFFFFU)
#define MIPICSI2_CBUF_ERRLINE_ERRLINE_SHIFT      (0U)
#define MIPICSI2_CBUF_ERRLINE_ERRLINE_WIDTH      (16U)
#define MIPICSI2_CBUF_ERRLINE_ERRLINE(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_ERRLINE_ERRLINE_SHIFT)) & MIPICSI2_CBUF_ERRLINE_ERRLINE_MASK)
/*! @} */

/*! @name CBUF_INTRS - Receive Data Circular Buffer Error Interrupt Status */
/*! @{ */

#define MIPICSI2_CBUF_INTRS_LINLENERR0_MASK      (0x1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR0_SHIFT     (0U)
#define MIPICSI2_CBUF_INTRS_LINLENERR0_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR0(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR0_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR0_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR0_MASK      (0x2U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR0_SHIFT     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR0_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR0(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR0_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR0_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR1_MASK      (0x4U)
#define MIPICSI2_CBUF_INTRS_LINLENERR1_SHIFT     (2U)
#define MIPICSI2_CBUF_INTRS_LINLENERR1_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR1(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR1_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR1_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR1_MASK      (0x8U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR1_SHIFT     (3U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR1_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR1(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR1_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR1_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR2_MASK      (0x10U)
#define MIPICSI2_CBUF_INTRS_LINLENERR2_SHIFT     (4U)
#define MIPICSI2_CBUF_INTRS_LINLENERR2_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR2(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR2_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR2_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR2_MASK      (0x20U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR2_SHIFT     (5U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR2_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR2(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR2_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR2_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR3_MASK      (0x40U)
#define MIPICSI2_CBUF_INTRS_LINLENERR3_SHIFT     (6U)
#define MIPICSI2_CBUF_INTRS_LINLENERR3_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR3(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR3_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR3_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR3_MASK      (0x80U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR3_SHIFT     (7U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR3_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR3(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR3_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR3_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR4_MASK      (0x100U)
#define MIPICSI2_CBUF_INTRS_LINLENERR4_SHIFT     (8U)
#define MIPICSI2_CBUF_INTRS_LINLENERR4_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR4(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR4_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR4_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR4_MASK      (0x200U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR4_SHIFT     (9U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR4_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR4(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR4_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR4_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR5_MASK      (0x400U)
#define MIPICSI2_CBUF_INTRS_LINLENERR5_SHIFT     (10U)
#define MIPICSI2_CBUF_INTRS_LINLENERR5_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR5(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR5_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR5_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR5_MASK      (0x800U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR5_SHIFT     (11U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR5_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR5(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR5_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR5_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR6_MASK      (0x1000U)
#define MIPICSI2_CBUF_INTRS_LINLENERR6_SHIFT     (12U)
#define MIPICSI2_CBUF_INTRS_LINLENERR6_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR6(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR6_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR6_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR6_MASK      (0x2000U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR6_SHIFT     (13U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR6_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR6(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR6_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR6_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR7_MASK      (0x4000U)
#define MIPICSI2_CBUF_INTRS_LINLENERR7_SHIFT     (14U)
#define MIPICSI2_CBUF_INTRS_LINLENERR7_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR7(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR7_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR7_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR7_MASK      (0x8000U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR7_SHIFT     (15U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR7_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR7(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR7_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR7_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR8_MASK      (0x10000U)
#define MIPICSI2_CBUF_INTRS_LINLENERR8_SHIFT     (16U)
#define MIPICSI2_CBUF_INTRS_LINLENERR8_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR8(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR8_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR8_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR8_MASK      (0x20000U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR8_SHIFT     (17U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR8_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR8(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR8_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR8_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR9_MASK      (0x40000U)
#define MIPICSI2_CBUF_INTRS_LINLENERR9_SHIFT     (18U)
#define MIPICSI2_CBUF_INTRS_LINLENERR9_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR9(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR9_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR9_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR9_MASK      (0x80000U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR9_SHIFT     (19U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR9_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR9(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR9_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR9_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR10_MASK     (0x100000U)
#define MIPICSI2_CBUF_INTRS_LINLENERR10_SHIFT    (20U)
#define MIPICSI2_CBUF_INTRS_LINLENERR10_WIDTH    (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR10(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR10_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR10_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR10_MASK     (0x200000U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR10_SHIFT    (21U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR10_WIDTH    (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR10(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR10_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR10_MASK)

#define MIPICSI2_CBUF_INTRS_LINLENERR11_MASK     (0x400000U)
#define MIPICSI2_CBUF_INTRS_LINLENERR11_SHIFT    (22U)
#define MIPICSI2_CBUF_INTRS_LINLENERR11_WIDTH    (1U)
#define MIPICSI2_CBUF_INTRS_LINLENERR11(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINLENERR11_SHIFT)) & MIPICSI2_CBUF_INTRS_LINLENERR11_MASK)

#define MIPICSI2_CBUF_INTRS_LINCNTERR11_MASK     (0x800000U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR11_SHIFT    (23U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR11_WIDTH    (1U)
#define MIPICSI2_CBUF_INTRS_LINCNTERR11(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRS_LINCNTERR11_SHIFT)) & MIPICSI2_CBUF_INTRS_LINCNTERR11_MASK)
/*! @} */

/*! @name CBUF_INTRE - Receive Circular Buffer Error Interrupt Enable */
/*! @{ */

#define MIPICSI2_CBUF_INTRE_LINLENIE0_MASK       (0x1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE0_SHIFT      (0U)
#define MIPICSI2_CBUF_INTRE_LINLENIE0_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE0(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE0_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE0_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE0_MASK       (0x2U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE0_SHIFT      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE0_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE0(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE0_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE0_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE1_MASK       (0x4U)
#define MIPICSI2_CBUF_INTRE_LINLENIE1_SHIFT      (2U)
#define MIPICSI2_CBUF_INTRE_LINLENIE1_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE1(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE1_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE1_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE1_MASK       (0x8U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE1_SHIFT      (3U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE1_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE1(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE1_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE1_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE2_MASK       (0x10U)
#define MIPICSI2_CBUF_INTRE_LINLENIE2_SHIFT      (4U)
#define MIPICSI2_CBUF_INTRE_LINLENIE2_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE2(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE2_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE2_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE2_MASK       (0x20U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE2_SHIFT      (5U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE2_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE2(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE2_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE2_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE3_MASK       (0x40U)
#define MIPICSI2_CBUF_INTRE_LINLENIE3_SHIFT      (6U)
#define MIPICSI2_CBUF_INTRE_LINLENIE3_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE3(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE3_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE3_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE3_MASK       (0x80U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE3_SHIFT      (7U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE3_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE3(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE3_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE3_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE4_MASK       (0x100U)
#define MIPICSI2_CBUF_INTRE_LINLENIE4_SHIFT      (8U)
#define MIPICSI2_CBUF_INTRE_LINLENIE4_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE4(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE4_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE4_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE4_MASK       (0x200U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE4_SHIFT      (9U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE4_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE4(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE4_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE4_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE5_MASK       (0x400U)
#define MIPICSI2_CBUF_INTRE_LINLENIE5_SHIFT      (10U)
#define MIPICSI2_CBUF_INTRE_LINLENIE5_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE5(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE5_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE5_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE5_MASK       (0x800U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE5_SHIFT      (11U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE5_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE5(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE5_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE5_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE6_MASK       (0x1000U)
#define MIPICSI2_CBUF_INTRE_LINLENIE6_SHIFT      (12U)
#define MIPICSI2_CBUF_INTRE_LINLENIE6_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE6(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE6_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE6_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE6_MASK       (0x2000U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE6_SHIFT      (13U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE6_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE6(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE6_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE6_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE7_MASK       (0x4000U)
#define MIPICSI2_CBUF_INTRE_LINLENIE7_SHIFT      (14U)
#define MIPICSI2_CBUF_INTRE_LINLENIE7_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE7(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE7_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE7_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE7_MASK       (0x8000U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE7_SHIFT      (15U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE7_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE7(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE7_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE7_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE8_MASK       (0x10000U)
#define MIPICSI2_CBUF_INTRE_LINLENIE8_SHIFT      (16U)
#define MIPICSI2_CBUF_INTRE_LINLENIE8_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE8(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE8_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE8_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE8_MASK       (0x20000U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE8_SHIFT      (17U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE8_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE8(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE8_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE8_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE9_MASK       (0x40000U)
#define MIPICSI2_CBUF_INTRE_LINLENIE9_SHIFT      (18U)
#define MIPICSI2_CBUF_INTRE_LINLENIE9_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE9(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE9_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE9_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE9_MASK       (0x80000U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE9_SHIFT      (19U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE9_WIDTH      (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE9(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE9_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE9_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE10_MASK      (0x100000U)
#define MIPICSI2_CBUF_INTRE_LINLENIE10_SHIFT     (20U)
#define MIPICSI2_CBUF_INTRE_LINLENIE10_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE10(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE10_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE10_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE10_MASK      (0x200000U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE10_SHIFT     (21U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE10_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE10(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE10_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE10_MASK)

#define MIPICSI2_CBUF_INTRE_LINLENIE11_MASK      (0x400000U)
#define MIPICSI2_CBUF_INTRE_LINLENIE11_SHIFT     (22U)
#define MIPICSI2_CBUF_INTRE_LINLENIE11_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRE_LINLENIE11(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINLENIE11_SHIFT)) & MIPICSI2_CBUF_INTRE_LINLENIE11_MASK)

#define MIPICSI2_CBUF_INTRE_LINCNTIE11_MASK      (0x800000U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE11_SHIFT     (23U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE11_WIDTH     (1U)
#define MIPICSI2_CBUF_INTRE_LINCNTIE11(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_CBUF_INTRE_LINCNTIE11_SHIFT)) & MIPICSI2_CBUF_INTRE_LINCNTIE11_MASK)
/*! @} */

/*! @name RX_DROPDATAR - Received Drop Data Type and VC Report */
/*! @{ */

#define MIPICSI2_RX_DROPDATAR_DROPDATAID0_MASK   (0x3FU)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID0_SHIFT  (0U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID0_WIDTH  (6U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID0(x)     (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DROPDATAR_DROPDATAID0_SHIFT)) & MIPICSI2_RX_DROPDATAR_DROPDATAID0_MASK)

#define MIPICSI2_RX_DROPDATAR_DROPVCID0_MASK     (0xC0U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID0_SHIFT    (6U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID0_WIDTH    (2U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID0(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DROPDATAR_DROPVCID0_SHIFT)) & MIPICSI2_RX_DROPDATAR_DROPVCID0_MASK)

#define MIPICSI2_RX_DROPDATAR_DROPDATAID1_MASK   (0x3F00U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID1_SHIFT  (8U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID1_WIDTH  (6U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID1(x)     (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DROPDATAR_DROPDATAID1_SHIFT)) & MIPICSI2_RX_DROPDATAR_DROPDATAID1_MASK)

#define MIPICSI2_RX_DROPDATAR_DROPVCID1_MASK     (0xC000U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID1_SHIFT    (14U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID1_WIDTH    (2U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID1(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DROPDATAR_DROPVCID1_SHIFT)) & MIPICSI2_RX_DROPDATAR_DROPVCID1_MASK)

#define MIPICSI2_RX_DROPDATAR_DROPDATAID2_MASK   (0x3F0000U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID2_SHIFT  (16U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID2_WIDTH  (6U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID2(x)     (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DROPDATAR_DROPDATAID2_SHIFT)) & MIPICSI2_RX_DROPDATAR_DROPDATAID2_MASK)

#define MIPICSI2_RX_DROPDATAR_DROPVCID2_MASK     (0xC00000U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID2_SHIFT    (22U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID2_WIDTH    (2U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID2(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DROPDATAR_DROPVCID2_SHIFT)) & MIPICSI2_RX_DROPDATAR_DROPVCID2_MASK)

#define MIPICSI2_RX_DROPDATAR_DROPDATAID3_MASK   (0x3F000000U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID3_SHIFT  (24U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID3_WIDTH  (6U)
#define MIPICSI2_RX_DROPDATAR_DROPDATAID3(x)     (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DROPDATAR_DROPDATAID3_SHIFT)) & MIPICSI2_RX_DROPDATAR_DROPDATAID3_MASK)

#define MIPICSI2_RX_DROPDATAR_DROPVCID3_MASK     (0xC0000000U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID3_SHIFT    (30U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID3_WIDTH    (2U)
#define MIPICSI2_RX_DROPDATAR_DROPVCID3(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_DROPDATAR_DROPVCID3_SHIFT)) & MIPICSI2_RX_DROPDATAR_DROPVCID3_MASK)
/*! @} */

/*! @name RX_CBUF_OUTCFG - Receive Data Channel Output Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF_OUTCFG_DATA_MODE_MASK   (0x1U)
#define MIPICSI2_RX_CBUF_OUTCFG_DATA_MODE_SHIFT  (0U)
#define MIPICSI2_RX_CBUF_OUTCFG_DATA_MODE_WIDTH  (1U)
#define MIPICSI2_RX_CBUF_OUTCFG_DATA_MODE(x)     (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_OUTCFG_DATA_MODE_SHIFT)) & MIPICSI2_RX_CBUF_OUTCFG_DATA_MODE_MASK)

#define MIPICSI2_RX_CBUF_OUTCFG_CALIB_ON_MASK    (0x2U)
#define MIPICSI2_RX_CBUF_OUTCFG_CALIB_ON_SHIFT   (1U)
#define MIPICSI2_RX_CBUF_OUTCFG_CALIB_ON_WIDTH   (1U)
#define MIPICSI2_RX_CBUF_OUTCFG_CALIB_ON(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_OUTCFG_CALIB_ON_SHIFT)) & MIPICSI2_RX_CBUF_OUTCFG_CALIB_ON_MASK)

#define MIPICSI2_RX_CBUF_OUTCFG_DROP_RATE_MASK   (0xCU)
#define MIPICSI2_RX_CBUF_OUTCFG_DROP_RATE_SHIFT  (2U)
#define MIPICSI2_RX_CBUF_OUTCFG_DROP_RATE_WIDTH  (2U)
#define MIPICSI2_RX_CBUF_OUTCFG_DROP_RATE(x)     (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_OUTCFG_DROP_RATE_SHIFT)) & MIPICSI2_RX_CBUF_OUTCFG_DROP_RATE_MASK)

#define MIPICSI2_RX_CBUF_OUTCFG_OUTPUT_MODE_MASK (0x30U)
#define MIPICSI2_RX_CBUF_OUTCFG_OUTPUT_MODE_SHIFT (4U)
#define MIPICSI2_RX_CBUF_OUTCFG_OUTPUT_MODE_WIDTH (2U)
#define MIPICSI2_RX_CBUF_OUTCFG_OUTPUT_MODE(x)   (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_OUTCFG_OUTPUT_MODE_SHIFT)) & MIPICSI2_RX_CBUF_OUTCFG_OUTPUT_MODE_MASK)

#define MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_MASK    (0x40U)
#define MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_SHIFT   (6U)
#define MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_WIDTH   (1U)
#define MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_SHIFT)) & MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_MASK)

#define MIPICSI2_RX_CBUF_OUTCFG_SWAPRAWDATA_MASK (0x80U)
#define MIPICSI2_RX_CBUF_OUTCFG_SWAPRAWDATA_SHIFT (7U)
#define MIPICSI2_RX_CBUF_OUTCFG_SWAPRAWDATA_WIDTH (1U)
#define MIPICSI2_RX_CBUF_OUTCFG_SWAPRAWDATA(x)   (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_OUTCFG_SWAPRAWDATA_SHIFT)) & MIPICSI2_RX_CBUF_OUTCFG_SWAPRAWDATA_MASK)

#define MIPICSI2_RX_CBUF_OUTCFG_BYTE_ORDER_LSB_FIRST_MASK (0x100U)
#define MIPICSI2_RX_CBUF_OUTCFG_BYTE_ORDER_LSB_FIRST_SHIFT (8U)
#define MIPICSI2_RX_CBUF_OUTCFG_BYTE_ORDER_LSB_FIRST_WIDTH (1U)
#define MIPICSI2_RX_CBUF_OUTCFG_BYTE_ORDER_LSB_FIRST(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_OUTCFG_BYTE_ORDER_LSB_FIRST_SHIFT)) & MIPICSI2_RX_CBUF_OUTCFG_BYTE_ORDER_LSB_FIRST_MASK)

#define MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_AUX_MASK (0x200U)
#define MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_AUX_SHIFT (9U)
#define MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_AUX_WIDTH (1U)
#define MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_AUX(x)  (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_AUX_SHIFT)) & MIPICSI2_RX_CBUF_OUTCFG_FLIP_BIT_AUX_MASK)
/*! @} */

/*! @name RX_CBUF_CHNLENBL - Receive Data Channel Enable/Disable Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF_CHNLENBL_CHA_ENBL_MASK  (0x1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHA_ENBL_SHIFT (0U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHA_ENBL_WIDTH (1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHA_ENBL(x)    (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_CHNLENBL_CHA_ENBL_SHIFT)) & MIPICSI2_RX_CBUF_CHNLENBL_CHA_ENBL_MASK)

#define MIPICSI2_RX_CBUF_CHNLENBL_CHB_ENBL_MASK  (0x2U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHB_ENBL_SHIFT (1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHB_ENBL_WIDTH (1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHB_ENBL(x)    (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_CHNLENBL_CHB_ENBL_SHIFT)) & MIPICSI2_RX_CBUF_CHNLENBL_CHB_ENBL_MASK)

#define MIPICSI2_RX_CBUF_CHNLENBL_CHC_ENBL_MASK  (0x4U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHC_ENBL_SHIFT (2U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHC_ENBL_WIDTH (1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHC_ENBL(x)    (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_CHNLENBL_CHC_ENBL_SHIFT)) & MIPICSI2_RX_CBUF_CHNLENBL_CHC_ENBL_MASK)

#define MIPICSI2_RX_CBUF_CHNLENBL_CHD_ENBL_MASK  (0x8U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHD_ENBL_SHIFT (3U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHD_ENBL_WIDTH (1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHD_ENBL(x)    (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_CHNLENBL_CHD_ENBL_SHIFT)) & MIPICSI2_RX_CBUF_CHNLENBL_CHD_ENBL_MASK)

#define MIPICSI2_RX_CBUF_CHNLENBL_CHE_ENBL_MASK  (0x10U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHE_ENBL_SHIFT (4U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHE_ENBL_WIDTH (1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHE_ENBL(x)    (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_CHNLENBL_CHE_ENBL_SHIFT)) & MIPICSI2_RX_CBUF_CHNLENBL_CHE_ENBL_MASK)

#define MIPICSI2_RX_CBUF_CHNLENBL_CHF_ENBL_MASK  (0x20U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHF_ENBL_SHIFT (5U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHF_ENBL_WIDTH (1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHF_ENBL(x)    (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_CHNLENBL_CHF_ENBL_SHIFT)) & MIPICSI2_RX_CBUF_CHNLENBL_CHF_ENBL_MASK)

#define MIPICSI2_RX_CBUF_CHNLENBL_CHG_ENBL_MASK  (0x40U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHG_ENBL_SHIFT (6U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHG_ENBL_WIDTH (1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHG_ENBL(x)    (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_CHNLENBL_CHG_ENBL_SHIFT)) & MIPICSI2_RX_CBUF_CHNLENBL_CHG_ENBL_MASK)

#define MIPICSI2_RX_CBUF_CHNLENBL_CHH_ENBL_MASK  (0x80U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHH_ENBL_SHIFT (7U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHH_ENBL_WIDTH (1U)
#define MIPICSI2_RX_CBUF_CHNLENBL_CHH_ENBL(x)    (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF_CHNLENBL_CHH_ENBL_SHIFT)) & MIPICSI2_RX_CBUF_CHNLENBL_CHH_ENBL_MASK)
/*! @} */

/*! @name RX_CBUF0_CHNLOFFSET0_0 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHA_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHA_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHA_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHA_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHA_DC_SHIFT)) & MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHA_DC_MASK)

#define MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHB_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHB_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHB_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHB_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHB_DC_SHIFT)) & MIPICSI2_RX_CBUF0_CHNLOFFSET0_0_CHB_DC_MASK)
/*! @} */

/*! @name RX_CBUF0_CHNLOFFSET1_0 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHC_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHC_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHC_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHC_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHC_DC_SHIFT)) & MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHC_DC_MASK)

#define MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHD_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHD_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHD_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHD_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHD_DC_SHIFT)) & MIPICSI2_RX_CBUF0_CHNLOFFSET1_0_CHD_DC_MASK)
/*! @} */

/*! @name RX_CBUF0_CHNLOFFSET2_0 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHE_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHE_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHE_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHE_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHE_DC_SHIFT)) & MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHE_DC_MASK)

#define MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHF_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHF_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHF_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHF_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHF_DC_SHIFT)) & MIPICSI2_RX_CBUF0_CHNLOFFSET2_0_CHF_DC_MASK)
/*! @} */

/*! @name RX_CBUF0_CHNLOFFSET3_0 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHG_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHG_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHG_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHG_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHG_DC_SHIFT)) & MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHG_DC_MASK)

#define MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHH_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHH_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHH_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHH_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHH_DC_SHIFT)) & MIPICSI2_RX_CBUF0_CHNLOFFSET3_0_CHH_DC_MASK)
/*! @} */

/*! @name RX_CBUF1_CHNLOFFSET0_1 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHA_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHA_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHA_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHA_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHA_DC_SHIFT)) & MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHA_DC_MASK)

#define MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHB_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHB_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHB_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHB_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHB_DC_SHIFT)) & MIPICSI2_RX_CBUF1_CHNLOFFSET0_1_CHB_DC_MASK)
/*! @} */

/*! @name RX_CBUF1_CHNLOFFSET1_1 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHC_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHC_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHC_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHC_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHC_DC_SHIFT)) & MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHC_DC_MASK)

#define MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHD_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHD_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHD_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHD_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHD_DC_SHIFT)) & MIPICSI2_RX_CBUF1_CHNLOFFSET1_1_CHD_DC_MASK)
/*! @} */

/*! @name RX_CBUF1_CHNLOFFSET2_1 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHE_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHE_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHE_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHE_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHE_DC_SHIFT)) & MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHE_DC_MASK)

#define MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHF_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHF_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHF_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHF_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHF_DC_SHIFT)) & MIPICSI2_RX_CBUF1_CHNLOFFSET2_1_CHF_DC_MASK)
/*! @} */

/*! @name RX_CBUF1_CHNLOFFSET3_1 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHG_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHG_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHG_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHG_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHG_DC_SHIFT)) & MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHG_DC_MASK)

#define MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHH_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHH_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHH_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHH_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHH_DC_SHIFT)) & MIPICSI2_RX_CBUF1_CHNLOFFSET3_1_CHH_DC_MASK)
/*! @} */

/*! @name RX_CBUF2_CHNLOFFSET0_2 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHA_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHA_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHA_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHA_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHA_DC_SHIFT)) & MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHA_DC_MASK)

#define MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHB_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHB_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHB_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHB_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHB_DC_SHIFT)) & MIPICSI2_RX_CBUF2_CHNLOFFSET0_2_CHB_DC_MASK)
/*! @} */

/*! @name RX_CBUF2_CHNLOFFSET1_2 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHC_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHC_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHC_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHC_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHC_DC_SHIFT)) & MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHC_DC_MASK)

#define MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHD_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHD_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHD_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHD_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHD_DC_SHIFT)) & MIPICSI2_RX_CBUF2_CHNLOFFSET1_2_CHD_DC_MASK)
/*! @} */

/*! @name RX_CBUF2_CHNLOFFSET2_2 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHE_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHE_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHE_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHE_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHE_DC_SHIFT)) & MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHE_DC_MASK)

#define MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHF_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHF_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHF_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHF_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHF_DC_SHIFT)) & MIPICSI2_RX_CBUF2_CHNLOFFSET2_2_CHF_DC_MASK)
/*! @} */

/*! @name RX_CBUF2_CHNLOFFSET3_2 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHG_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHG_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHG_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHG_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHG_DC_SHIFT)) & MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHG_DC_MASK)

#define MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHH_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHH_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHH_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHH_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHH_DC_SHIFT)) & MIPICSI2_RX_CBUF2_CHNLOFFSET3_2_CHH_DC_MASK)
/*! @} */

/*! @name RX_CBUF3_CHNLOFFSET0_3 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHA_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHA_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHA_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHA_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHA_DC_SHIFT)) & MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHA_DC_MASK)

#define MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHB_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHB_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHB_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHB_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHB_DC_SHIFT)) & MIPICSI2_RX_CBUF3_CHNLOFFSET0_3_CHB_DC_MASK)
/*! @} */

/*! @name RX_CBUF3_CHNLOFFSET1_3 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHC_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHC_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHC_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHC_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHC_DC_SHIFT)) & MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHC_DC_MASK)

#define MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHD_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHD_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHD_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHD_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHD_DC_SHIFT)) & MIPICSI2_RX_CBUF3_CHNLOFFSET1_3_CHD_DC_MASK)
/*! @} */

/*! @name RX_CBUF3_CHNLOFFSET2_3 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHE_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHE_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHE_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHE_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHE_DC_SHIFT)) & MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHE_DC_MASK)

#define MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHF_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHF_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHF_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHF_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHF_DC_SHIFT)) & MIPICSI2_RX_CBUF3_CHNLOFFSET2_3_CHF_DC_MASK)
/*! @} */

/*! @name RX_CBUF3_CHNLOFFSET3_3 - Receive Data Channel Offset Compensation Configuration */
/*! @{ */

#define MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHG_DC_MASK (0xFFFEU)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHG_DC_SHIFT (1U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHG_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHG_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHG_DC_SHIFT)) & MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHG_DC_MASK)

#define MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHH_DC_MASK (0xFFFE0000U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHH_DC_SHIFT (17U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHH_DC_WIDTH (15U)
#define MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHH_DC(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHH_DC_SHIFT)) & MIPICSI2_RX_CBUF3_CHNLOFFSET3_3_CHH_DC_MASK)
/*! @} */

/*! @name RX_CHNL_INTRS - Receive Data Channel Status */
/*! @{ */

#define MIPICSI2_RX_CHNL_INTRS_LINEDONE_MASK     (0x1U)
#define MIPICSI2_RX_CHNL_INTRS_LINEDONE_SHIFT    (0U)
#define MIPICSI2_RX_CHNL_INTRS_LINEDONE_WIDTH    (1U)
#define MIPICSI2_RX_CHNL_INTRS_LINEDONE(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CHNL_INTRS_LINEDONE_SHIFT)) & MIPICSI2_RX_CHNL_INTRS_LINEDONE_MASK)

#define MIPICSI2_RX_CHNL_INTRS_BUFFOVF_MASK      (0x2U)
#define MIPICSI2_RX_CHNL_INTRS_BUFFOVF_SHIFT     (1U)
#define MIPICSI2_RX_CHNL_INTRS_BUFFOVF_WIDTH     (1U)
#define MIPICSI2_RX_CHNL_INTRS_BUFFOVF(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CHNL_INTRS_BUFFOVF_SHIFT)) & MIPICSI2_RX_CHNL_INTRS_BUFFOVF_MASK)
/*! @} */

/*! @name RX_CHNL_INTRE - Receive Channel Interrupt Enable */
/*! @{ */

#define MIPICSI2_RX_CHNL_INTRE_LINEDONEIE_MASK   (0x1U)
#define MIPICSI2_RX_CHNL_INTRE_LINEDONEIE_SHIFT  (0U)
#define MIPICSI2_RX_CHNL_INTRE_LINEDONEIE_WIDTH  (1U)
#define MIPICSI2_RX_CHNL_INTRE_LINEDONEIE(x)     (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CHNL_INTRE_LINEDONEIE_SHIFT)) & MIPICSI2_RX_CHNL_INTRE_LINEDONEIE_MASK)

#define MIPICSI2_RX_CHNL_INTRE_BUFFOVFIE_MASK    (0x2U)
#define MIPICSI2_RX_CHNL_INTRE_BUFFOVFIE_SHIFT   (1U)
#define MIPICSI2_RX_CHNL_INTRE_BUFFOVFIE_WIDTH   (1U)
#define MIPICSI2_RX_CHNL_INTRE_BUFFOVFIE(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_RX_CHNL_INTRE_BUFFOVFIE_SHIFT)) & MIPICSI2_RX_CHNL_INTRE_BUFFOVFIE_MASK)
/*! @} */

/*! @name WR_CHNL_INTRS - AXI Write Channel Interrupt Status */
/*! @{ */

#define MIPICSI2_WR_CHNL_INTRS_ERRRESP_MASK      (0x1U)
#define MIPICSI2_WR_CHNL_INTRS_ERRRESP_SHIFT     (0U)
#define MIPICSI2_WR_CHNL_INTRS_ERRRESP_WIDTH     (1U)
#define MIPICSI2_WR_CHNL_INTRS_ERRRESP(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_WR_CHNL_INTRS_ERRRESP_SHIFT)) & MIPICSI2_WR_CHNL_INTRS_ERRRESP_MASK)

#define MIPICSI2_WR_CHNL_INTRS_BUFFOVFAXI_MASK   (0x2U)
#define MIPICSI2_WR_CHNL_INTRS_BUFFOVFAXI_SHIFT  (1U)
#define MIPICSI2_WR_CHNL_INTRS_BUFFOVFAXI_WIDTH  (1U)
#define MIPICSI2_WR_CHNL_INTRS_BUFFOVFAXI(x)     (((uint32_t)(((uint32_t)(x)) << MIPICSI2_WR_CHNL_INTRS_BUFFOVFAXI_SHIFT)) & MIPICSI2_WR_CHNL_INTRS_BUFFOVFAXI_MASK)
/*! @} */

/*! @name WR_CHNL_INTRE - AXI Write Channel Interrupt Enable */
/*! @{ */

#define MIPICSI2_WR_CHNL_INTRE_ERRRESPIE_MASK    (0x1U)
#define MIPICSI2_WR_CHNL_INTRE_ERRRESPIE_SHIFT   (0U)
#define MIPICSI2_WR_CHNL_INTRE_ERRRESPIE_WIDTH   (1U)
#define MIPICSI2_WR_CHNL_INTRE_ERRRESPIE(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_WR_CHNL_INTRE_ERRRESPIE_SHIFT)) & MIPICSI2_WR_CHNL_INTRE_ERRRESPIE_MASK)

#define MIPICSI2_WR_CHNL_INTRE_BUFFOVFAXIIE_MASK (0x2U)
#define MIPICSI2_WR_CHNL_INTRE_BUFFOVFAXIIE_SHIFT (1U)
#define MIPICSI2_WR_CHNL_INTRE_BUFFOVFAXIIE_WIDTH (1U)
#define MIPICSI2_WR_CHNL_INTRE_BUFFOVFAXIIE(x)   (((uint32_t)(((uint32_t)(x)) << MIPICSI2_WR_CHNL_INTRE_BUFFOVFAXIIE_SHIFT)) & MIPICSI2_WR_CHNL_INTRE_BUFFOVFAXIIE_MASK)
/*! @} */

/*! @name TX_TURNAROUND_POST_CFG - Turnaround Post Configuration */
/*! @{ */

#define MIPICSI2_TX_TURNAROUND_POST_CFG_TURNPOST_MASK (0x7FU)
#define MIPICSI2_TX_TURNAROUND_POST_CFG_TURNPOST_SHIFT (0U)
#define MIPICSI2_TX_TURNAROUND_POST_CFG_TURNPOST_WIDTH (7U)
#define MIPICSI2_TX_TURNAROUND_POST_CFG_TURNPOST(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_TURNAROUND_POST_CFG_TURNPOST_SHIFT)) & MIPICSI2_TX_TURNAROUND_POST_CFG_TURNPOST_MASK)
/*! @} */

/*! @name TX_TXNULANE - Transmit Number of Lanes Configuration */
/*! @{ */

#define MIPICSI2_TX_TXNULANE_TXNULANE_MASK       (0xFU)
#define MIPICSI2_TX_TXNULANE_TXNULANE_SHIFT      (0U)
#define MIPICSI2_TX_TXNULANE_TXNULANE_WIDTH      (4U)
#define MIPICSI2_TX_TXNULANE_TXNULANE(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_TXNULANE_TXNULANE_SHIFT)) & MIPICSI2_TX_TXNULANE_TXNULANE_MASK)
/*! @} */

/*! @name TX_TXENABLE - Transmit Enable Configuration */
/*! @{ */

#define MIPICSI2_TX_TXENABLE_TXEN_MASK           (0x1U)
#define MIPICSI2_TX_TXENABLE_TXEN_SHIFT          (0U)
#define MIPICSI2_TX_TXENABLE_TXEN_WIDTH          (1U)
#define MIPICSI2_TX_TXENABLE_TXEN(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_TXENABLE_TXEN_SHIFT)) & MIPICSI2_TX_TXENABLE_TXEN_MASK)
/*! @} */

/*! @name TX_DESCSTART - Transmit Descriptor Start Configuration */
/*! @{ */

#define MIPICSI2_TX_DESCSTART_STARTONTA_MASK     (0x3U)
#define MIPICSI2_TX_DESCSTART_STARTONTA_SHIFT    (0U)
#define MIPICSI2_TX_DESCSTART_STARTONTA_WIDTH    (2U)
#define MIPICSI2_TX_DESCSTART_STARTONTA(x)       (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_DESCSTART_STARTONTA_SHIFT)) & MIPICSI2_TX_DESCSTART_STARTONTA_MASK)
/*! @} */

/*! @name TX_TXSTAT - MIPICSI2 Transmit Status */
/*! @{ */

#define MIPICSI2_TX_TXSTAT_TXHSACT_MASK          (0x1U)
#define MIPICSI2_TX_TXSTAT_TXHSACT_SHIFT         (0U)
#define MIPICSI2_TX_TXSTAT_TXHSACT_WIDTH         (1U)
#define MIPICSI2_TX_TXSTAT_TXHSACT(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_TXSTAT_TXHSACT_SHIFT)) & MIPICSI2_TX_TXSTAT_TXHSACT_MASK)
/*! @} */

/*! @name TX_DESCRCFGS - MIPICSI2 Transmit Descriptor Configuration and Status */
/*! @{ */

#define MIPICSI2_TX_DESCRCFGS_WRAP_MASK          (0x1U)
#define MIPICSI2_TX_DESCRCFGS_WRAP_SHIFT         (0U)
#define MIPICSI2_TX_DESCRCFGS_WRAP_WIDTH         (1U)
#define MIPICSI2_TX_DESCRCFGS_WRAP(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_DESCRCFGS_WRAP_SHIFT)) & MIPICSI2_TX_DESCRCFGS_WRAP_MASK)

#define MIPICSI2_TX_DESCRCFGS_GO_MASK            (0x2U)
#define MIPICSI2_TX_DESCRCFGS_GO_SHIFT           (1U)
#define MIPICSI2_TX_DESCRCFGS_GO_WIDTH           (1U)
#define MIPICSI2_TX_DESCRCFGS_GO(x)              (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_DESCRCFGS_GO_SHIFT)) & MIPICSI2_TX_DESCRCFGS_GO_MASK)

#define MIPICSI2_TX_DESCRCFGS_LAST_MASK          (0x4U)
#define MIPICSI2_TX_DESCRCFGS_LAST_SHIFT         (2U)
#define MIPICSI2_TX_DESCRCFGS_LAST_WIDTH         (1U)
#define MIPICSI2_TX_DESCRCFGS_LAST(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_DESCRCFGS_LAST_SHIFT)) & MIPICSI2_TX_DESCRCFGS_LAST_MASK)

#define MIPICSI2_TX_DESCRCFGS_IOC_MASK           (0x8U)
#define MIPICSI2_TX_DESCRCFGS_IOC_SHIFT          (3U)
#define MIPICSI2_TX_DESCRCFGS_IOC_WIDTH          (1U)
#define MIPICSI2_TX_DESCRCFGS_IOC(x)             (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_DESCRCFGS_IOC_SHIFT)) & MIPICSI2_TX_DESCRCFGS_IOC_MASK)
/*! @} */

/*! @name TX_DESCADDR - Transmit Descriptor Base Address */
/*! @{ */

#define MIPICSI2_TX_DESCADDR_BASEADDR_MASK       (0xFFFFFFFFU)
#define MIPICSI2_TX_DESCADDR_BASEADDR_SHIFT      (0U)
#define MIPICSI2_TX_DESCADDR_BASEADDR_WIDTH      (32U)
#define MIPICSI2_TX_DESCADDR_BASEADDR(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_DESCADDR_BASEADDR_SHIFT)) & MIPICSI2_TX_DESCADDR_BASEADDR_MASK)
/*! @} */

/*! @name TX_DESCNADDR - Next Transmit Descriptor Address Report */
/*! @{ */

#define MIPICSI2_TX_DESCNADDR_NEXTADDR_MASK      (0xFFFFFFFFU)
#define MIPICSI2_TX_DESCNADDR_NEXTADDR_SHIFT     (0U)
#define MIPICSI2_TX_DESCNADDR_NEXTADDR_WIDTH     (32U)
#define MIPICSI2_TX_DESCNADDR_NEXTADDR(x)        (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_DESCNADDR_NEXTADDR_SHIFT)) & MIPICSI2_TX_DESCNADDR_NEXTADDR_MASK)
/*! @} */

/*! @name TX_DESCNUM - Number of Transmit Descriptor Read Report */
/*! @{ */

#define MIPICSI2_TX_DESCNUM_NUMDESC_MASK         (0xFFFFU)
#define MIPICSI2_TX_DESCNUM_NUMDESC_SHIFT        (0U)
#define MIPICSI2_TX_DESCNUM_NUMDESC_WIDTH        (16U)
#define MIPICSI2_TX_DESCNUM_NUMDESC(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_DESCNUM_NUMDESC_SHIFT)) & MIPICSI2_TX_DESCNUM_NUMDESC_MASK)
/*! @} */

/*! @name TX_PKTS - Transmit Packet Status */
/*! @{ */

#define MIPICSI2_TX_PKTS_PKTTYP_MASK             (0xFFU)
#define MIPICSI2_TX_PKTS_PKTTYP_SHIFT            (0U)
#define MIPICSI2_TX_PKTS_PKTTYP_WIDTH            (8U)
#define MIPICSI2_TX_PKTS_PKTTYP(x)               (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_PKTS_PKTTYP_SHIFT)) & MIPICSI2_TX_PKTS_PKTTYP_MASK)

#define MIPICSI2_TX_PKTS_PKTSIZE_MASK            (0xFFFF00U)
#define MIPICSI2_TX_PKTS_PKTSIZE_SHIFT           (8U)
#define MIPICSI2_TX_PKTS_PKTSIZE_WIDTH           (16U)
#define MIPICSI2_TX_PKTS_PKTSIZE(x)              (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_PKTS_PKTSIZE_SHIFT)) & MIPICSI2_TX_PKTS_PKTSIZE_MASK)
/*! @} */

/*! @name TX_ERRIS - Transmit Error Interrupt Status */
/*! @{ */

#define MIPICSI2_TX_ERRIS_PKTCMDERR_MASK         (0x1U)
#define MIPICSI2_TX_ERRIS_PKTCMDERR_SHIFT        (0U)
#define MIPICSI2_TX_ERRIS_PKTCMDERR_WIDTH        (1U)
#define MIPICSI2_TX_ERRIS_PKTCMDERR(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_ERRIS_PKTCMDERR_SHIFT)) & MIPICSI2_TX_ERRIS_PKTCMDERR_MASK)

#define MIPICSI2_TX_ERRIS_IOCS_MASK              (0x2U)
#define MIPICSI2_TX_ERRIS_IOCS_SHIFT             (1U)
#define MIPICSI2_TX_ERRIS_IOCS_WIDTH             (1U)
#define MIPICSI2_TX_ERRIS_IOCS(x)                (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_ERRIS_IOCS_SHIFT)) & MIPICSI2_TX_ERRIS_IOCS_MASK)

#define MIPICSI2_TX_ERRIS_RDERRRESP_MASK         (0x4U)
#define MIPICSI2_TX_ERRIS_RDERRRESP_SHIFT        (2U)
#define MIPICSI2_TX_ERRIS_RDERRRESP_WIDTH        (1U)
#define MIPICSI2_TX_ERRIS_RDERRRESP(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_ERRIS_RDERRRESP_SHIFT)) & MIPICSI2_TX_ERRIS_RDERRRESP_MASK)
/*! @} */

/*! @name TX_IE - Transmit Interrupt Enable */
/*! @{ */

#define MIPICSI2_TX_IE_PKTCMDERRIE_MASK          (0x1U)
#define MIPICSI2_TX_IE_PKTCMDERRIE_SHIFT         (0U)
#define MIPICSI2_TX_IE_PKTCMDERRIE_WIDTH         (1U)
#define MIPICSI2_TX_IE_PKTCMDERRIE(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_IE_PKTCMDERRIE_SHIFT)) & MIPICSI2_TX_IE_PKTCMDERRIE_MASK)

#define MIPICSI2_TX_IE_IOCIE_MASK                (0x2U)
#define MIPICSI2_TX_IE_IOCIE_SHIFT               (1U)
#define MIPICSI2_TX_IE_IOCIE_WIDTH               (1U)
#define MIPICSI2_TX_IE_IOCIE(x)                  (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_IE_IOCIE_SHIFT)) & MIPICSI2_TX_IE_IOCIE_MASK)

#define MIPICSI2_TX_IE_RDERRRESPIE_MASK          (0x4U)
#define MIPICSI2_TX_IE_RDERRRESPIE_SHIFT         (2U)
#define MIPICSI2_TX_IE_RDERRRESPIE_WIDTH         (1U)
#define MIPICSI2_TX_IE_RDERRRESPIE(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_IE_RDERRRESPIE_SHIFT)) & MIPICSI2_TX_IE_RDERRRESPIE_MASK)
/*! @} */

/*! @name TURNCFG - Turnaround Request Configuration */
/*! @{ */

#define MIPICSI2_TURNCFG_TURNDIS_MASK            (0x1U)
#define MIPICSI2_TURNCFG_TURNDIS_SHIFT           (0U)
#define MIPICSI2_TURNCFG_TURNDIS_WIDTH           (1U)
#define MIPICSI2_TURNCFG_TURNDIS(x)              (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNCFG_TURNDIS_SHIFT)) & MIPICSI2_TURNCFG_TURNDIS_MASK)

#define MIPICSI2_TURNCFG_FORCERXMODE1_MASK       (0x2U)
#define MIPICSI2_TURNCFG_FORCERXMODE1_SHIFT      (1U)
#define MIPICSI2_TURNCFG_FORCERXMODE1_WIDTH      (1U)
#define MIPICSI2_TURNCFG_FORCERXMODE1(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNCFG_FORCERXMODE1_SHIFT)) & MIPICSI2_TURNCFG_FORCERXMODE1_MASK)

#define MIPICSI2_TURNCFG_FORCERXMODE2_MASK       (0x4U)
#define MIPICSI2_TURNCFG_FORCERXMODE2_SHIFT      (2U)
#define MIPICSI2_TURNCFG_FORCERXMODE2_WIDTH      (1U)
#define MIPICSI2_TURNCFG_FORCERXMODE2(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNCFG_FORCERXMODE2_SHIFT)) & MIPICSI2_TURNCFG_FORCERXMODE2_MASK)

#define MIPICSI2_TURNCFG_FORCERXMODE3_MASK       (0x8U)
#define MIPICSI2_TURNCFG_FORCERXMODE3_SHIFT      (3U)
#define MIPICSI2_TURNCFG_FORCERXMODE3_WIDTH      (1U)
#define MIPICSI2_TURNCFG_FORCERXMODE3(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNCFG_FORCERXMODE3_SHIFT)) & MIPICSI2_TURNCFG_FORCERXMODE3_MASK)

#define MIPICSI2_TURNCFG_FORCERXMODE4_MASK       (0x10U)
#define MIPICSI2_TURNCFG_FORCERXMODE4_SHIFT      (4U)
#define MIPICSI2_TURNCFG_FORCERXMODE4_WIDTH      (1U)
#define MIPICSI2_TURNCFG_FORCERXMODE4(x)         (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNCFG_FORCERXMODE4_SHIFT)) & MIPICSI2_TURNCFG_FORCERXMODE4_MASK)
/*! @} */

/*! @name TURNSTAT - Turnaround Request Status */
/*! @{ */

#define MIPICSI2_TURNSTAT_TURNREQ0_MASK          (0x1U)
#define MIPICSI2_TURNSTAT_TURNREQ0_SHIFT         (0U)
#define MIPICSI2_TURNSTAT_TURNREQ0_WIDTH         (1U)
#define MIPICSI2_TURNSTAT_TURNREQ0(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_TURNREQ0_SHIFT)) & MIPICSI2_TURNSTAT_TURNREQ0_MASK)

#define MIPICSI2_TURNSTAT_TURNREQ1_MASK          (0x2U)
#define MIPICSI2_TURNSTAT_TURNREQ1_SHIFT         (1U)
#define MIPICSI2_TURNSTAT_TURNREQ1_WIDTH         (1U)
#define MIPICSI2_TURNSTAT_TURNREQ1(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_TURNREQ1_SHIFT)) & MIPICSI2_TURNSTAT_TURNREQ1_MASK)

#define MIPICSI2_TURNSTAT_TURNREQ2_MASK          (0x4U)
#define MIPICSI2_TURNSTAT_TURNREQ2_SHIFT         (2U)
#define MIPICSI2_TURNSTAT_TURNREQ2_WIDTH         (1U)
#define MIPICSI2_TURNSTAT_TURNREQ2(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_TURNREQ2_SHIFT)) & MIPICSI2_TURNSTAT_TURNREQ2_MASK)

#define MIPICSI2_TURNSTAT_TURNREQ3_MASK          (0x8U)
#define MIPICSI2_TURNSTAT_TURNREQ3_SHIFT         (3U)
#define MIPICSI2_TURNSTAT_TURNREQ3_WIDTH         (1U)
#define MIPICSI2_TURNSTAT_TURNREQ3(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_TURNREQ3_SHIFT)) & MIPICSI2_TURNSTAT_TURNREQ3_MASK)

#define MIPICSI2_TURNSTAT_ONGOINGTA0_MASK        (0x10U)
#define MIPICSI2_TURNSTAT_ONGOINGTA0_SHIFT       (4U)
#define MIPICSI2_TURNSTAT_ONGOINGTA0_WIDTH       (1U)
#define MIPICSI2_TURNSTAT_ONGOINGTA0(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_ONGOINGTA0_SHIFT)) & MIPICSI2_TURNSTAT_ONGOINGTA0_MASK)

#define MIPICSI2_TURNSTAT_ONGOINGTA1_MASK        (0x20U)
#define MIPICSI2_TURNSTAT_ONGOINGTA1_SHIFT       (5U)
#define MIPICSI2_TURNSTAT_ONGOINGTA1_WIDTH       (1U)
#define MIPICSI2_TURNSTAT_ONGOINGTA1(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_ONGOINGTA1_SHIFT)) & MIPICSI2_TURNSTAT_ONGOINGTA1_MASK)

#define MIPICSI2_TURNSTAT_ONGOINGTA2_MASK        (0x40U)
#define MIPICSI2_TURNSTAT_ONGOINGTA2_SHIFT       (6U)
#define MIPICSI2_TURNSTAT_ONGOINGTA2_WIDTH       (1U)
#define MIPICSI2_TURNSTAT_ONGOINGTA2(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_ONGOINGTA2_SHIFT)) & MIPICSI2_TURNSTAT_ONGOINGTA2_MASK)

#define MIPICSI2_TURNSTAT_ONGOINGTA3_MASK        (0x80U)
#define MIPICSI2_TURNSTAT_ONGOINGTA3_SHIFT       (7U)
#define MIPICSI2_TURNSTAT_ONGOINGTA3_WIDTH       (1U)
#define MIPICSI2_TURNSTAT_ONGOINGTA3(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_ONGOINGTA3_SHIFT)) & MIPICSI2_TURNSTAT_ONGOINGTA3_MASK)

#define MIPICSI2_TURNSTAT_DIRECTION0_MASK        (0x100U)
#define MIPICSI2_TURNSTAT_DIRECTION0_SHIFT       (8U)
#define MIPICSI2_TURNSTAT_DIRECTION0_WIDTH       (1U)
#define MIPICSI2_TURNSTAT_DIRECTION0(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_DIRECTION0_SHIFT)) & MIPICSI2_TURNSTAT_DIRECTION0_MASK)

#define MIPICSI2_TURNSTAT_DIRECTION1_MASK        (0x200U)
#define MIPICSI2_TURNSTAT_DIRECTION1_SHIFT       (9U)
#define MIPICSI2_TURNSTAT_DIRECTION1_WIDTH       (1U)
#define MIPICSI2_TURNSTAT_DIRECTION1(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_DIRECTION1_SHIFT)) & MIPICSI2_TURNSTAT_DIRECTION1_MASK)

#define MIPICSI2_TURNSTAT_DIRECTION2_MASK        (0x400U)
#define MIPICSI2_TURNSTAT_DIRECTION2_SHIFT       (10U)
#define MIPICSI2_TURNSTAT_DIRECTION2_WIDTH       (1U)
#define MIPICSI2_TURNSTAT_DIRECTION2(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_DIRECTION2_SHIFT)) & MIPICSI2_TURNSTAT_DIRECTION2_MASK)

#define MIPICSI2_TURNSTAT_DIRECTION3_MASK        (0x800U)
#define MIPICSI2_TURNSTAT_DIRECTION3_SHIFT       (11U)
#define MIPICSI2_TURNSTAT_DIRECTION3_WIDTH       (1U)
#define MIPICSI2_TURNSTAT_DIRECTION3(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTAT_DIRECTION3_SHIFT)) & MIPICSI2_TURNSTAT_DIRECTION3_MASK)
/*! @} */

/*! @name TURNSTATIE - Turnaround Error Interrupt Enable */
/*! @{ */

#define MIPICSI2_TURNSTATIE_ONGOINGTAIE0_MASK    (0x10U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE0_SHIFT   (4U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE0_WIDTH   (1U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE0(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTATIE_ONGOINGTAIE0_SHIFT)) & MIPICSI2_TURNSTATIE_ONGOINGTAIE0_MASK)

#define MIPICSI2_TURNSTATIE_ONGOINGTAIE1_MASK    (0x20U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE1_SHIFT   (5U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE1_WIDTH   (1U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE1(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTATIE_ONGOINGTAIE1_SHIFT)) & MIPICSI2_TURNSTATIE_ONGOINGTAIE1_MASK)

#define MIPICSI2_TURNSTATIE_ONGOINGTAIE2_MASK    (0x40U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE2_SHIFT   (6U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE2_WIDTH   (1U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE2(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTATIE_ONGOINGTAIE2_SHIFT)) & MIPICSI2_TURNSTATIE_ONGOINGTAIE2_MASK)

#define MIPICSI2_TURNSTATIE_ONGOINGTAIE3_MASK    (0x80U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE3_SHIFT   (7U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE3_WIDTH   (1U)
#define MIPICSI2_TURNSTATIE_ONGOINGTAIE3(x)      (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNSTATIE_ONGOINGTAIE3_SHIFT)) & MIPICSI2_TURNSTATIE_ONGOINGTAIE3_MASK)
/*! @} */

/*! @name TURNIS - Turnaround Request Interrupt Status */
/*! @{ */

#define MIPICSI2_TURNIS_TURNCOMPMS_MASK          (0x1U)
#define MIPICSI2_TURNIS_TURNCOMPMS_SHIFT         (0U)
#define MIPICSI2_TURNIS_TURNCOMPMS_WIDTH         (1U)
#define MIPICSI2_TURNIS_TURNCOMPMS(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNIS_TURNCOMPMS_SHIFT)) & MIPICSI2_TURNIS_TURNCOMPMS_MASK)

#define MIPICSI2_TURNIS_TURNCOMPSM_MASK          (0x2U)
#define MIPICSI2_TURNIS_TURNCOMPSM_SHIFT         (1U)
#define MIPICSI2_TURNIS_TURNCOMPSM_WIDTH         (1U)
#define MIPICSI2_TURNIS_TURNCOMPSM(x)            (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNIS_TURNCOMPSM_SHIFT)) & MIPICSI2_TURNIS_TURNCOMPSM_MASK)
/*! @} */

/*! @name TURNIE - Turnaround Request Interrupt Enable */
/*! @{ */

#define MIPICSI2_TURNIE_TURNCOMPMIE_MASK         (0x1U)
#define MIPICSI2_TURNIE_TURNCOMPMIE_SHIFT        (0U)
#define MIPICSI2_TURNIE_TURNCOMPMIE_WIDTH        (1U)
#define MIPICSI2_TURNIE_TURNCOMPMIE(x)           (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNIE_TURNCOMPMIE_SHIFT)) & MIPICSI2_TURNIE_TURNCOMPMIE_MASK)

#define MIPICSI2_TURNIE_TURNCOMPSMIE_MASK        (0x2U)
#define MIPICSI2_TURNIE_TURNCOMPSMIE_SHIFT       (1U)
#define MIPICSI2_TURNIE_TURNCOMPSMIE_WIDTH       (1U)
#define MIPICSI2_TURNIE_TURNCOMPSMIE(x)          (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TURNIE_TURNCOMPSMIE_SHIFT)) & MIPICSI2_TURNIE_TURNCOMPSMIE_MASK)
/*! @} */

/*! @name TRIGGER_GPIO1 - GPIO1 Pad Event Trigger Control */
/*! @{ */

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC0_MASK (0x3U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC0_SHIFT (0U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC0_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC0_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC0_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC0_MASK (0xCU)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC0_SHIFT (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC0_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC0_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC0_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC0_MASK (0x30U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC0_SHIFT (4U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC0_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC0_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC0_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane0_MASK (0x40U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane0_SHIFT (6U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane0_WIDTH (1U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane0_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane0_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane0_MASK (0x80U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane0_SHIFT (7U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane0_WIDTH (1U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane0_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane0_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC1_MASK (0x300U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC1_SHIFT (8U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC1_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC1_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC1_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC1_MASK (0xC00U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC1_SHIFT (10U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC1_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC1_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC1_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC1_MASK (0x3000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC1_SHIFT (12U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC1_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC1_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC1_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane1_MASK (0x4000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane1_SHIFT (14U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane1_WIDTH (1U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane1_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane1_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane1_MASK (0x8000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane1_SHIFT (15U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane1_WIDTH (1U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane1_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane1_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC2_MASK (0x30000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC2_SHIFT (16U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC2_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC2_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC2_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC2_MASK (0xC0000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC2_SHIFT (18U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC2_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC2_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC2_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC2_MASK (0x300000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC2_SHIFT (20U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC2_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC2_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC2_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane2_MASK (0x400000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane2_SHIFT (22U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane2_WIDTH (1U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane2_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane2_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane2_MASK (0x800000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane2_SHIFT (23U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane2_WIDTH (1U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane2_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane2_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC3_MASK (0x3000000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC3_SHIFT (24U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC3_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC3_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONERR_VC3_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC3_MASK (0xC000000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC3_SHIFT (26U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC3_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC3_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKT_VC3_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC3_MASK (0x30000000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC3_SHIFT (28U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC3_WIDTH (2U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC3_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONPKTID_VC3_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane3_MASK (0x40000000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane3_SHIFT (30U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane3_WIDTH (1U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane3_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONTASM_Lane3_MASK)

#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane3_MASK (0x80000000U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane3_SHIFT (31U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane3_WIDTH (1U)
#define MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane3_SHIFT)) & MIPICSI2_TRIGGER_GPIO1_TRIGGERONTAMS_Lane3_MASK)
/*! @} */

/*! @name TRIGGER_SDMA1 - SDMA1 Pad Event Trigger Control */
/*! @{ */

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC0_MASK (0x3U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC0_SHIFT (0U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC0_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC0_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC0_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC0_MASK (0xCU)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC0_SHIFT (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC0_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC0_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC0_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC0_MASK (0x30U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC0_SHIFT (4U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC0_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC0_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC0_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane0_MASK (0x40U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane0_SHIFT (6U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane0_WIDTH (1U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane0_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane0_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane0_MASK (0x80U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane0_SHIFT (7U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane0_WIDTH (1U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane0_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane0_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC1_MASK (0x300U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC1_SHIFT (8U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC1_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC1_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC1_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC1_MASK (0xC00U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC1_SHIFT (10U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC1_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC1_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC1_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC1_MASK (0x3000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC1_SHIFT (12U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC1_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC1_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC1_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane1_MASK (0x4000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane1_SHIFT (14U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane1_WIDTH (1U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane1_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane1_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane1_MASK (0x8000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane1_SHIFT (15U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane1_WIDTH (1U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane1_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane1_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC2_MASK (0x30000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC2_SHIFT (16U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC2_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC2_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC2_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC2_MASK (0xC0000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC2_SHIFT (18U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC2_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC2_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC2_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC2_MASK (0x300000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC2_SHIFT (20U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC2_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC2_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC2_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane2_MASK (0x400000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane2_SHIFT (22U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane2_WIDTH (1U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane2_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane2_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane2_MASK (0x800000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane2_SHIFT (23U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane2_WIDTH (1U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane2_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane2_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC3_MASK (0x3000000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC3_SHIFT (24U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC3_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC3_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONERR_VC3_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC3_MASK (0xC000000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC3_SHIFT (26U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC3_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC3_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKT_VC3_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC3_MASK (0x30000000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC3_SHIFT (28U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC3_WIDTH (2U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC3_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONPKTID_VC3_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane3_MASK (0x40000000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane3_SHIFT (30U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane3_WIDTH (1U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane3_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONTASM_Lane3_MASK)

#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane3_MASK (0x80000000U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane3_SHIFT (31U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane3_WIDTH (1U)
#define MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane3_SHIFT)) & MIPICSI2_TRIGGER_SDMA1_TRIGGERONTAMS_Lane3_MASK)
/*! @} */

/*! @name TRIGGEREN_GPIO - GPIO Pad Event Trigger Enable Control */
/*! @{ */

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC0_MASK (0x1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC0_SHIFT (0U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC0_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC0_MASK (0x2U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC0_SHIFT (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC0_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC0_MASK (0x4U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC0_SHIFT (2U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC0_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC1_MASK (0x8U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC1_SHIFT (3U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC1_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC1_MASK (0x10U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC1_SHIFT (4U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC1_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC1_MASK (0x20U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC1_SHIFT (5U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC1_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC2_MASK (0x40U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC2_SHIFT (6U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC2_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC2_MASK (0x80U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC2_SHIFT (7U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC2_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC2_MASK (0x100U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC2_SHIFT (8U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC2_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC3_MASK (0x200U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC3_SHIFT (9U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONERR_VC3_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC3_MASK (0x400U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC3_SHIFT (10U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTEN_VC3_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC3_MASK (0x800U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC3_SHIFT (11U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO1_TRIGGERONPKTIDEN_VC3_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC0_MASK (0x1000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC0_SHIFT (12U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC0_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC0_MASK (0x2000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC0_SHIFT (13U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC0_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC0_MASK (0x4000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC0_SHIFT (14U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC0_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC1_MASK (0x8000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC1_SHIFT (15U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC1_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC1_MASK (0x10000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC1_SHIFT (16U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC1_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC1_MASK (0x20000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC1_SHIFT (17U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC1_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC2_MASK (0x40000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC2_SHIFT (18U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC2_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC2_MASK (0x80000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC2_SHIFT (19U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC2_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC2_MASK (0x100000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC2_SHIFT (20U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC2_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC3_MASK (0x200000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC3_SHIFT (21U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONERR_VC3_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC3_MASK (0x400000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC3_SHIFT (22U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTEN_VC3_MASK)

#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC3_MASK (0x800000U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC3_SHIFT (23U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_GPIO_GPIO2_TRIGGERONPKTIDEN_VC3_MASK)
/*! @} */

/*! @name TRIGGEREN_SDMA - SDMA Pad Event Trigger Enable Control */
/*! @{ */

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC0_MASK (0x1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC0_SHIFT (0U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC0_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC0_MASK (0x2U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC0_SHIFT (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC0_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC0_MASK (0x4U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC0_SHIFT (2U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC0_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC1_MASK (0x8U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC1_SHIFT (3U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC1_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC1_MASK (0x10U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC1_SHIFT (4U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC1_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC1_MASK (0x20U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC1_SHIFT (5U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC1_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC2_MASK (0x40U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC2_SHIFT (6U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC2_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC2_MASK (0x80U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC2_SHIFT (7U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC2_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC2_MASK (0x100U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC2_SHIFT (8U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC2_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC3_MASK (0x200U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC3_SHIFT (9U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONERR_VC3_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC3_MASK (0x400U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC3_SHIFT (10U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTEN_VC3_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC3_MASK (0x800U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC3_SHIFT (11U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA1_TRIGGERONPKTIDEN_VC3_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC0_MASK (0x1000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC0_SHIFT (12U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC0_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC0_MASK (0x2000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC0_SHIFT (13U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC0_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC0_MASK (0x4000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC0_SHIFT (14U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC0_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC0(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC0_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC0_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC1_MASK (0x8000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC1_SHIFT (15U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC1_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC1_MASK (0x10000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC1_SHIFT (16U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC1_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC1_MASK (0x20000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC1_SHIFT (17U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC1_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC1(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC1_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC1_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC2_MASK (0x40000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC2_SHIFT (18U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC2_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC2_MASK (0x80000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC2_SHIFT (19U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC2_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC2_MASK (0x100000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC2_SHIFT (20U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC2_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC2(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC2_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC2_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC3_MASK (0x200000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC3_SHIFT (21U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONERR_VC3_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC3_MASK (0x400000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC3_SHIFT (22U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTEN_VC3_MASK)

#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC3_MASK (0x800000U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC3_SHIFT (23U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC3_WIDTH (1U)
#define MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC3(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC3_SHIFT)) & MIPICSI2_TRIGGEREN_SDMA_SDMA2_TRIGGERONPKTIDEN_VC3_MASK)
/*! @} */

/*! @name TX_TEST_FIFO_CFG - Transmit Test Path FIFO-Based Flow Control Configuration */
/*! @{ */

#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_START_THRESHOLD_MASK (0x7FU)
#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_START_THRESHOLD_SHIFT (0U)
#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_START_THRESHOLD_WIDTH (7U)
#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_START_THRESHOLD(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_TEST_FIFO_CFG_FIFO_START_THRESHOLD_SHIFT)) & MIPICSI2_TX_TEST_FIFO_CFG_FIFO_START_THRESHOLD_MASK)

#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_STOP_THRESHOLD_MASK (0x3F80U)
#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_STOP_THRESHOLD_SHIFT (7U)
#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_STOP_THRESHOLD_WIDTH (7U)
#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_STOP_THRESHOLD(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_TEST_FIFO_CFG_FIFO_STOP_THRESHOLD_SHIFT)) & MIPICSI2_TX_TEST_FIFO_CFG_FIFO_STOP_THRESHOLD_MASK)

#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_READ_START_THRESHOLD_MASK (0x1FC000U)
#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_READ_START_THRESHOLD_SHIFT (14U)
#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_READ_START_THRESHOLD_WIDTH (7U)
#define MIPICSI2_TX_TEST_FIFO_CFG_FIFO_READ_START_THRESHOLD(x) (((uint32_t)(((uint32_t)(x)) << MIPICSI2_TX_TEST_FIFO_CFG_FIFO_READ_START_THRESHOLD_SHIFT)) & MIPICSI2_TX_TEST_FIFO_CFG_FIFO_READ_START_THRESHOLD_MASK)
/*! @} */

/*! @name DPHY_CALTYPE_CNTRL - System Configuration */
/*! @{ */

#define MIPICSI2_DPHY_CALTYPE_CNTRL_NOEXT_BURNIN_RES_CAL_RW_MASK (0x10U)
#define MIPICSI2_DPHY_CALTYPE_CNTRL_NOEXT_BURNIN_RES_CAL_RW_SHIFT (4U)
#define MIPICSI2_DPHY_CALTYPE_CNTRL_NOEXT_BURNIN_RES_CAL_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CALTYPE_CNTRL_NOEXT_BURNIN_RES_CAL_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CALTYPE_CNTRL_NOEXT_BURNIN_RES_CAL_RW_SHIFT)) & MIPICSI2_DPHY_CALTYPE_CNTRL_NOEXT_BURNIN_RES_CAL_RW_MASK)

#define MIPICSI2_DPHY_CALTYPE_CNTRL_CMP_POLARITY_RW_MASK (0x20U)
#define MIPICSI2_DPHY_CALTYPE_CNTRL_CMP_POLARITY_RW_SHIFT (5U)
#define MIPICSI2_DPHY_CALTYPE_CNTRL_CMP_POLARITY_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CALTYPE_CNTRL_CMP_POLARITY_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CALTYPE_CNTRL_CMP_POLARITY_RW_SHIFT)) & MIPICSI2_DPHY_CALTYPE_CNTRL_CMP_POLARITY_RW_MASK)
/*! @} */

/*! @name DPHY_SKEWCAL_CNTRL - System Configuration */
/*! @{ */

#define MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_RUN_RW_MASK (0x1U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_RUN_RW_SHIFT (0U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_RUN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_RUN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_RUN_RW_SHIFT)) & MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_RUN_RW_MASK)

#define MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_SEL_RW_MASK (0x2U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_SEL_RW_SHIFT (1U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_SEL_RW_WIDTH (1U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_SEL_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_SEL_RW_SHIFT)) & MIPICSI2_DPHY_SKEWCAL_CNTRL_SKEW_MUX_SEL_RW_MASK)

#define MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_LATENCY_RW__1__0___MASK (0x18U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_LATENCY_RW__1__0___SHIFT (3U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_LATENCY_RW__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_LATENCY_RW__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_LATENCY_RW__1__0___SHIFT)) & MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_LATENCY_RW__1__0___MASK)

#define MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_POLARITY_RW_MASK (0x20U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_POLARITY_RW_SHIFT (5U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_POLARITY_RW_WIDTH (1U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_POLARITY_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_POLARITY_RW_SHIFT)) & MIPICSI2_DPHY_SKEWCAL_CNTRL_DESKEW_POLARITY_RW_MASK)

#define MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_RW_MASK (0x40U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_RW_SHIFT (6U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_RW_SHIFT)) & MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_RW_MASK)

#define MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_EN_RW_MASK (0x80U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_EN_RW_SHIFT (7U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_SKEWCAL_CNTRL_TCLK_MISS_DIV2_OVR_EN_RW_MASK)
/*! @} */

/*! @name DPHY_RX_SYNALIGN_CFG - System Configuration */
/*! @{ */

#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_NUMEDGES_UPDATE_RW_MASK (0x1U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_NUMEDGES_UPDATE_RW_SHIFT (0U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_NUMEDGES_UPDATE_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_NUMEDGES_UPDATE_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_NUMEDGES_UPDATE_RW_SHIFT)) & MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_NUMEDGES_UPDATE_RW_MASK)

#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_OVERFLOW_RW_MASK (0x2U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_OVERFLOW_RW_SHIFT (1U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_OVERFLOW_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_OVERFLOW_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_OVERFLOW_RW_SHIFT)) & MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_OVERFLOW_RW_MASK)

#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_JUMP2STEPS_RW__1__0___MASK (0xCU)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_JUMP2STEPS_RW__1__0___SHIFT (2U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_JUMP2STEPS_RW__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_JUMP2STEPS_RW__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_JUMP2STEPS_RW__1__0___SHIFT)) & MIPICSI2_DPHY_RX_SYNALIGN_CFG_DESKEW_JUMP2STEPS_RW__1__0___MASK)

#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_NOALIGN_ERROR_RW_MASK (0x10U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_NOALIGN_ERROR_RW_SHIFT (4U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_NOALIGN_ERROR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_NOALIGN_ERROR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_SYNALIGN_CFG_NOALIGN_ERROR_RW_SHIFT)) & MIPICSI2_DPHY_RX_SYNALIGN_CFG_NOALIGN_ERROR_RW_MASK)

#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_ALIGNER_DK_CNF_RW__1__0___MASK (0x60U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_ALIGNER_DK_CNF_RW__1__0___SHIFT (5U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_ALIGNER_DK_CNF_RW__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_RX_SYNALIGN_CFG_ALIGNER_DK_CNF_RW__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_SYNALIGN_CFG_ALIGNER_DK_CNF_RW__1__0___SHIFT)) & MIPICSI2_DPHY_RX_SYNALIGN_CFG_ALIGNER_DK_CNF_RW__1__0___MASK)
/*! @} */

/*! @name DPHY_DESKEW_CFG - This register is used to program the Deskew accumulator size(FJUMP) */
/*! @{ */

#define MIPICSI2_DPHY_DESKEW_CFG_DESKEW_NUMEDGES_RW_MASK (0xFFU)
#define MIPICSI2_DPHY_DESKEW_CFG_DESKEW_NUMEDGES_RW_SHIFT (0U)
#define MIPICSI2_DPHY_DESKEW_CFG_DESKEW_NUMEDGES_RW_WIDTH (8U)
#define MIPICSI2_DPHY_DESKEW_CFG_DESKEW_NUMEDGES_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DESKEW_CFG_DESKEW_NUMEDGES_RW_SHIFT)) & MIPICSI2_DPHY_DESKEW_CFG_DESKEW_NUMEDGES_RW_MASK)
/*! @} */

/*! @name DPHY_RX_STARTUP_OVERRIDE - System Startup Observability */
/*! @{ */

#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_CLK_EN_LANES_BYPASS_RW_MASK (0x1U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_CLK_EN_LANES_BYPASS_RW_SHIFT (0U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_CLK_EN_LANES_BYPASS_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_CLK_EN_LANES_BYPASS_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_CLK_EN_LANES_BYPASS_RW_SHIFT)) & MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_CLK_EN_LANES_BYPASS_RW_MASK)

#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_OFFSET_MACHINE_RW_MASK (0x4U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_OFFSET_MACHINE_RW_SHIFT (2U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_OFFSET_MACHINE_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_OFFSET_MACHINE_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_OFFSET_MACHINE_RW_SHIFT)) & MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_OFFSET_MACHINE_RW_MASK)

#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_DDLTUNNING_MACHINE_RW_MASK (0x8U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_DDLTUNNING_MACHINE_RW_SHIFT (3U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_DDLTUNNING_MACHINE_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_DDLTUNNING_MACHINE_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_DDLTUNNING_MACHINE_RW_SHIFT)) & MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_DDLTUNNING_MACHINE_RW_MASK)

#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_EN_RW_MASK (0x10U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_EN_RW_SHIFT (4U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_RW_MASK (0x20U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_RW_SHIFT (5U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_RW_SHIFT)) & MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_SKEW_BACK_COMP_EN_OVR_RW_MASK)

#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_SKEW_MACHINE_RW_MASK (0x40U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_SKEW_MACHINE_RW_SHIFT (6U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_SKEW_MACHINE_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_SKEW_MACHINE_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_SKEW_MACHINE_RW_SHIFT)) & MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_BYPASS_SKEW_MACHINE_RW_MASK)

#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_RX_RXHS_COMPATIBILITY_MODE_OVR_EN_RW_MASK (0x80U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_RX_RXHS_COMPATIBILITY_MODE_OVR_EN_RW_SHIFT (7U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_RX_RXHS_COMPATIBILITY_MODE_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_RX_RXHS_COMPATIBILITY_MODE_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_RX_RXHS_COMPATIBILITY_MODE_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_RX_STARTUP_OVERRIDE_RX_RXHS_COMPATIBILITY_MODE_OVR_EN_RW_MASK)
/*! @} */

/*! @name DPHY_DDLOSCFREQ_CFG1 - System Startup Override */
/*! @{ */

#define MIPICSI2_DPHY_DDLOSCFREQ_CFG1_DDL_OSC_FREQ_TARGET_OVR_RW__7__0___MASK (0xFFU)
#define MIPICSI2_DPHY_DDLOSCFREQ_CFG1_DDL_OSC_FREQ_TARGET_OVR_RW__7__0___SHIFT (0U)
#define MIPICSI2_DPHY_DDLOSCFREQ_CFG1_DDL_OSC_FREQ_TARGET_OVR_RW__7__0___WIDTH (8U)
#define MIPICSI2_DPHY_DDLOSCFREQ_CFG1_DDL_OSC_FREQ_TARGET_OVR_RW__7__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DDLOSCFREQ_CFG1_DDL_OSC_FREQ_TARGET_OVR_RW__7__0___SHIFT)) & MIPICSI2_DPHY_DDLOSCFREQ_CFG1_DDL_OSC_FREQ_TARGET_OVR_RW__7__0___MASK)
/*! @} */

/*! @name DPHY_DDLOSCFREQ_CFG2 - System Startup Override */
/*! @{ */

#define MIPICSI2_DPHY_DDLOSCFREQ_CFG2_DDL_OSC_FREQ_TARGET_OVR_RW__11__8___MASK (0xFU)
#define MIPICSI2_DPHY_DDLOSCFREQ_CFG2_DDL_OSC_FREQ_TARGET_OVR_RW__11__8___SHIFT (0U)
#define MIPICSI2_DPHY_DDLOSCFREQ_CFG2_DDL_OSC_FREQ_TARGET_OVR_RW__11__8___WIDTH (4U)
#define MIPICSI2_DPHY_DDLOSCFREQ_CFG2_DDL_OSC_FREQ_TARGET_OVR_RW__11__8__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DDLOSCFREQ_CFG2_DDL_OSC_FREQ_TARGET_OVR_RW__11__8___SHIFT)) & MIPICSI2_DPHY_DDLOSCFREQ_CFG2_DDL_OSC_FREQ_TARGET_OVR_RW__11__8___MASK)
/*! @} */

/*! @name DPHY_DDLOSCFREQ_OVREN - System Startup Override */
/*! @{ */

#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_DDL_OSC_FREQ_TARGET_OVR_EN_RW_MASK (0x1U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_DDL_OSC_FREQ_TARGET_OVR_EN_RW_SHIFT (0U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_DDL_OSC_FREQ_TARGET_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_DDL_OSC_FREQ_TARGET_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DDLOSCFREQ_OVREN_DDL_OSC_FREQ_TARGET_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DDLOSCFREQ_OVREN_DDL_OSC_FREQ_TARGET_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_EN_RW_MASK (0x2U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_EN_RW_SHIFT (1U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_RW__1__0___MASK (0xCU)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_RW__1__0___SHIFT (2U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_RW__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_RW__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_RW__1__0___SHIFT)) & MIPICSI2_DPHY_DDLOSCFREQ_OVREN_RX_RXHS_GMODE_IF_OVR_RW__1__0___MASK)

#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_COUNTER_FOR_DES_EN_CONFIG_IF_RW__3__0___MASK (0xF0U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_COUNTER_FOR_DES_EN_CONFIG_IF_RW__3__0___SHIFT (4U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_COUNTER_FOR_DES_EN_CONFIG_IF_RW__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_DDLOSCFREQ_OVREN_COUNTER_FOR_DES_EN_CONFIG_IF_RW__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DDLOSCFREQ_OVREN_COUNTER_FOR_DES_EN_CONFIG_IF_RW__3__0___SHIFT)) & MIPICSI2_DPHY_DDLOSCFREQ_OVREN_COUNTER_FOR_DES_EN_CONFIG_IF_RW__3__0___MASK)
/*! @} */

/*! @name DPHY_RX_TERM_CAL_0 - Termination Calibration Observability */
/*! @{ */

#define MIPICSI2_DPHY_RX_TERM_CAL_0_CB_CAL_REPL__3__0___MASK (0x3CU)
#define MIPICSI2_DPHY_RX_TERM_CAL_0_CB_CAL_REPL__3__0___SHIFT (2U)
#define MIPICSI2_DPHY_RX_TERM_CAL_0_CB_CAL_REPL__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_RX_TERM_CAL_0_CB_CAL_REPL__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_TERM_CAL_0_CB_CAL_REPL__3__0___SHIFT)) & MIPICSI2_DPHY_RX_TERM_CAL_0_CB_CAL_REPL__3__0___MASK)
/*! @} */

/*! @name DPHY_RX_TERM_CAL_1 - Termination Calibration Observability */
/*! @{ */

#define MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_EN_MASK (0x20U)
#define MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_EN_SHIFT (5U)
#define MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_EN_WIDTH (1U)
#define MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_EN(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_EN_SHIFT)) & MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_EN_MASK)

#define MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_DONE_MASK (0x80U)
#define MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_DONE_SHIFT (7U)
#define MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_DONE_WIDTH (1U)
#define MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_DONE(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_DONE_SHIFT)) & MIPICSI2_DPHY_RX_TERM_CAL_1_RESCAL_DONE_MASK)
/*! @} */

/*! @name DPHY_CLOCK_LANE_CNTRL - Clock Lane Control */
/*! @{ */

#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_CLK_TO_LONG_CHANNEL_IF_RW_MASK (0x4U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_CLK_TO_LONG_CHANNEL_IF_RW_SHIFT (2U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_CLK_TO_LONG_CHANNEL_IF_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_CLK_TO_LONG_CHANNEL_IF_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_CLK_TO_LONG_CHANNEL_IF_RW_SHIFT)) & MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_CLK_TO_LONG_CHANNEL_IF_RW_MASK)

#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_DDR_CLK_EN_IF_RW_MASK (0x8U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_DDR_CLK_EN_IF_RW_SHIFT (3U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_DDR_CLK_EN_IF_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_DDR_CLK_EN_IF_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_DDR_CLK_EN_IF_RW_SHIFT)) & MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_DDR_CLK_EN_IF_RW_MASK)

#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_RW_MASK (0x10U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_RW_SHIFT (4U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_RW_SHIFT)) & MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_RW_MASK)

#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_FEED_INT_CLK_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_INT_CLK_SEL_RW_MASK (0x40U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_INT_CLK_SEL_RW_SHIFT (6U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_INT_CLK_SEL_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_INT_CLK_SEL_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_INT_CLK_SEL_RW_SHIFT)) & MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_INT_CLK_SEL_RW_MASK)

#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_PULL_LONG_CHANNEL_IF_RW_MASK (0x80U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_PULL_LONG_CHANNEL_IF_RW_SHIFT (7U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_PULL_LONG_CHANNEL_IF_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_PULL_LONG_CHANNEL_IF_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_PULL_LONG_CHANNEL_IF_RW_SHIFT)) & MIPICSI2_DPHY_CLOCK_LANE_CNTRL_RXCLK_RXHS_PULL_LONG_CHANNEL_IF_RW_MASK)
/*! @} */

/*! @name DPHY_CLKOFFSETCAL_OVRRIDE - Clock Lane Offset Cancellation Control */
/*! @{ */

#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_EN_RW_MASK (0x1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_EN_RW_SHIFT (0U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_EN_RW_MASK (0x2U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_EN_RW_SHIFT (1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_RW_MASK (0x4U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_RW_SHIFT (2U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_RW_SHIFT)) & MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_RXHS_START_CALIBRATION_OVR_RW_MASK)

#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_EN_RW_MASK (0x8U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_EN_RW_SHIFT (3U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_RW_MASK (0x10U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_RW_SHIFT (4U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_RW_SHIFT)) & MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDE_RXCLK_START_CALIBRATION_OVR_RW_MASK)
/*! @} */

/*! @name DPHY_CLKOFFSETCAL_OVRRIDEVAL - Clock Lane Offset Cancellation Control 2 */
/*! @{ */

#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDEVAL_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_RW__6__0___MASK (0x7FU)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDEVAL_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_RW__6__0___SHIFT (0U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDEVAL_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_RW__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDEVAL_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_RW__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDEVAL_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_RW__6__0___SHIFT)) & MIPICSI2_DPHY_CLKOFFSETCAL_OVRRIDEVAL_RXCLK_RXHS_CLK_OFFSET_CAL_OVR_RW__6__0___MASK)
/*! @} */

/*! @name DPHY_CLKCALVAL_COMPS - Clock Lane Offset Cancellation Observability 3 */
/*! @{ */

#define MIPICSI2_DPHY_CLKCALVAL_COMPS_RXCLK_RXHS_CLK_OFFSET_CAL__6__0___MASK (0x7FU)
#define MIPICSI2_DPHY_CLKCALVAL_COMPS_RXCLK_RXHS_CLK_OFFSET_CAL__6__0___SHIFT (0U)
#define MIPICSI2_DPHY_CLKCALVAL_COMPS_RXCLK_RXHS_CLK_OFFSET_CAL__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_CLKCALVAL_COMPS_RXCLK_RXHS_CLK_OFFSET_CAL__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKCALVAL_COMPS_RXCLK_RXHS_CLK_OFFSET_CAL__6__0___SHIFT)) & MIPICSI2_DPHY_CLKCALVAL_COMPS_RXCLK_RXHS_CLK_OFFSET_CAL__6__0___MASK)
/*! @} */

/*! @name DPHY_CLKOFFSETCAL_COMPS - Clock Lane Offset Cancellation Observability */
/*! @{ */

#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_CALDONE_MASK (0x1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_CALDONE_SHIFT (0U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_CALDONE_WIDTH (1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_CALDONE(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_CALDONE_SHIFT)) & MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_CALDONE_MASK)

#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_ERRCAL__3__0___MASK (0x1EU)
#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_ERRCAL__3__0___SHIFT (1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_ERRCAL__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_ERRCAL__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_ERRCAL__3__0___SHIFT)) & MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_ERRCAL__3__0___MASK)

#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_RXHS_CLK_M_CAL_MASK (0x20U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_RXHS_CLK_M_CAL_SHIFT (5U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_RXHS_CLK_M_CAL_WIDTH (1U)
#define MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_RXHS_CLK_M_CAL(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_RXHS_CLK_M_CAL_SHIFT)) & MIPICSI2_DPHY_CLKOFFSETCAL_COMPS_RXCLK_RXHS_CLK_M_CAL_MASK)
/*! @} */

/*! @name DPHY_RX_LPRXPON_LANE0 - Lane 0 Low Power Receive Control */
/*! @{ */

#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONCD_LANE0_RW_MASK (0x1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONCD_LANE0_RW_SHIFT (0U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONCD_LANE0_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONCD_LANE0_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONCD_LANE0_RW_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONCD_LANE0_RW_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_BYPASS_LANE0_RW_MASK (0x2U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_BYPASS_LANE0_RW_SHIFT (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_BYPASS_LANE0_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_BYPASS_LANE0_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_BYPASS_LANE0_RW_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_BYPASS_LANE0_RW_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_LANE0_RW_MASK (0x4U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_LANE0_RW_SHIFT (2U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_LANE0_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_LANE0_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_LANE0_RW_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONLP_LANE0_RW_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_BYPASS_LANE0_RW_MASK (0x8U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_BYPASS_LANE0_RW_SHIFT (3U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_BYPASS_LANE0_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_BYPASS_LANE0_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_BYPASS_LANE0_RW_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_BYPASS_LANE0_RW_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_LANE0_RW_MASK (0x10U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_LANE0_RW_SHIFT (4U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_LANE0_RW_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_LANE0_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_LANE0_RW_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE0_LPRXPONULP_LANE0_RW_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_COMPS0 - Lane 0 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LPRXDOUTULP_LANE0__1__0___MASK (0x3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LPRXDOUTULP_LANE0__1__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LPRXDOUTULP_LANE0__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LPRXDOUTULP_LANE0__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LPRXDOUTULP_LANE0__1__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LPRXDOUTULP_LANE0__1__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LSRXCLK_LANE0_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LSRXCLK_LANE0_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LSRXCLK_LANE0_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LSRXCLK_LANE0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LSRXCLK_LANE0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_LSRXCLK_LANE0_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALDONE_IF_MASK (0x8U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALDONE_IF_SHIFT (3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALDONE_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALDONE_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALDONE_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALDONE_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALFAILED_IF_MASK (0x10U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALFAILED_IF_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALFAILED_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALFAILED_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALFAILED_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS0_RX0_DESKEWCALFAILED_IF_MASK)
/*! @} */

/*! @name DPHY_DATAL0OFFSETCAL_OVRCNTRL - Lane 0 Offset Compensation Control */
/*! @{ */

#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_MASK (0x4U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_SHIFT (2U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_EN_RW_MASK (0x8U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_EN_RW_SHIFT (3U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_RW_MASK (0x10U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_RW_SHIFT (4U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_START_CALIBRATION_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_RW_MASK (0x40U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_RW_SHIFT (6U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_CAL_DONE_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_MASK (0x80U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_SHIFT (7U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL0OFFSETCAL_OVRCNTRL_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_MASK)
/*! @} */

/*! @name DPHY_DATALOFFSETCAL_OVRVALUE0 - Lane 0 Offset Compensation Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_MASK (0x1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_SHIFT (0U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___MASK (0xFEU)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___SHIFT (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE0_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___MASK)
/*! @} */

/*! @name DPHY_DATALANE_OFFSETCAL_COMPS0 - Lane Offset Compensation Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_START_CALIBRATION_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_START_CALIBRATION_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_START_CALIBRATION_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_START_CALIBRATION(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_START_CALIBRATION_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_START_CALIBRATION_MASK)

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_DONE_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_DONE_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_DONE_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_DONE(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_DONE_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_DONE_MASK)

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_ERROR_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_ERROR_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_ERROR_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_ERROR(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_ERROR_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS0_RX0_CAL_ERROR_MASK)
/*! @} */

/*! @name DPHY_DATALOFFSETCAL_VALUE0 - Lane Offset Compensation Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL__6__0___MASK (0x7FU)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL__6__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL__6__0___SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL__6__0___MASK)

#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_MASK (0x80U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_SHIFT (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_WIDTH (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_VALUE0_RX0_RXHS_DATA_OFFSET_CAL_EN_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_COMP0 - Lane DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_DDL_TUNE_EN_LANE0_IF_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_DDL_TUNE_EN_LANE0_IF_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_DDL_TUNE_EN_LANE0_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_DDL_TUNE_EN_LANE0_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_DDL_TUNE_EN_LANE0_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_DDL_TUNE_EN_LANE0_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_ERROR_FLAG_IF_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_ERROR_FLAG_IF_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_ERROR_FLAG_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_ERROR_FLAG_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_ERROR_FLAG_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_ERROR_FLAG_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_FINISHED_IF_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_FINISHED_IF_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_FINISHED_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_FINISHED_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_FINISHED_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_FINISHED_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_START_CALIBRATION_IF_MASK (0x8U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_START_CALIBRATION_IF_SHIFT (3U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_START_CALIBRATION_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_START_CALIBRATION_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_START_CALIBRATION_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP0_RX0_DDL_START_CALIBRATION_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_VALUE0 - Lane 0 DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_TUNE_IF__4__0___MASK (0x1FU)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_TUNE_IF__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_TUNE_IF__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_TUNE_IF__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_TUNE_IF__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_TUNE_IF__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_MASK (0x20U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE0_DESKEW_VALUE1 - Lane 0 DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RX0_RXHS_DDL_PHASE_CLK_IF__4__0___MASK (0x1FU)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RX0_RXHS_DDL_PHASE_CLK_IF__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RX0_RXHS_DDL_PHASE_CLK_IF__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RX0_RXHS_DDL_PHASE_CLK_IF__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RX0_RXHS_DDL_PHASE_CLK_IF__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RX0_RXHS_DDL_PHASE_CLK_IF__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RESERVED_0_MASK (0xE0U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RESERVED_0_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RESERVED_0_WIDTH (3U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE1_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE0_DESKEW_VALUE2 - Lane 0 DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RX0_RXHS_DDL_PHASE_DATA_IF__3__0___MASK (0xFU)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RX0_RXHS_DDL_PHASE_DATA_IF__3__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RX0_RXHS_DDL_PHASE_DATA_IF__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RX0_RXHS_DDL_PHASE_DATA_IF__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RX0_RXHS_DDL_PHASE_DATA_IF__3__0___SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RX0_RXHS_DDL_PHASE_DATA_IF__3__0___MASK)

#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RESERVED_0_MASK (0xF0U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RESERVED_0_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RESERVED_0_WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_VALUE2_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_OVRVALUE0 - Lane 0 DDL Tune Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___MASK (0x1FU)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_RW__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_TUNE_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_MASK (0x40U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_SHIFT (6U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_MASK (0x80U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_SHIFT (7U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE0_RX0_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_MASK)
/*! @} */

/*! @name DPHY_DATALANE0_DESKEW_OVRVALUE1 - Lane 0 DDL Tune Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___MASK (0x1FU)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_RW__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RX0_RXHS_DDL_PHASE_CLK_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RESERVED_0_MASK (0xC0U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RESERVED_0_SHIFT (6U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RESERVED_0_WIDTH (2U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE1_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE0_DESKEW_OVRVALUE2 - Lane 0 DDL Tune Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___MASK (0xFU)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_RW__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___MASK)

#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_EN_RW_MASK (0x10U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_EN_RW_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RX0_RXHS_DDL_PHASE_DATA_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RESERVED_0_MASK (0xE0U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RESERVED_0_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RESERVED_0_WIDTH (3U)
#define MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE0_DESKEW_OVRVALUE2_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_RX_LPRXPON_LANE1 - Lane 1 Low Power Receive Control */
/*! @{ */

#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponcd_lane1_rw_MASK (0x1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponcd_lane1_rw_SHIFT (0U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponcd_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponcd_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponcd_lane1_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponcd_lane1_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_bypass_lane1_rw_MASK (0x2U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_bypass_lane1_rw_SHIFT (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_bypass_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_bypass_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_bypass_lane1_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_bypass_lane1_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_lane1_rw_MASK (0x4U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_lane1_rw_SHIFT (2U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_lane1_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponlp_lane1_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_bypass_lane1_rw_MASK (0x8U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_bypass_lane1_rw_SHIFT (3U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_bypass_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_bypass_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_bypass_lane1_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_bypass_lane1_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_lane1_rw_MASK (0x10U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_lane1_rw_SHIFT (4U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_lane1_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE1_lprxponulp_lane1_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_reserved_0_MASK (0xE0U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_reserved_0_SHIFT (5U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_reserved_0_WIDTH (3U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE1_reserved_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE1_reserved_0_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE1_reserved_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_OVRVALUE11 - Lane 1 Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___MASK (0x7CU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_MASK (0x80U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_SHIFT (7U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE11_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_OVRVALUE21 - Lane 1 Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___MASK (0xFU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_MASK (0x10U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RESERVED_0_MASK (0xE0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RESERVED_0_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RESERVED_0_WIDTH (3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE21_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_COMPS1 - Lane 0 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LPRXDOUTULP_LANE0__1__0___MASK (0x3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LPRXDOUTULP_LANE0__1__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LPRXDOUTULP_LANE0__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LPRXDOUTULP_LANE0__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LPRXDOUTULP_LANE0__1__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LPRXDOUTULP_LANE0__1__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LSRXCLK_LANE0_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LSRXCLK_LANE0_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LSRXCLK_LANE0_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LSRXCLK_LANE0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LSRXCLK_LANE0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_LSRXCLK_LANE0_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALDONE_IF_MASK (0x8U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALDONE_IF_SHIFT (3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALDONE_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALDONE_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALDONE_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALDONE_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALFAILED_IF_MASK (0x10U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALFAILED_IF_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALFAILED_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALFAILED_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALFAILED_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS1_RX0_DESKEWCALFAILED_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_VALUE11 - Lane 1 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_PM_START_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_PM_START_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_PM_START_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_PM_START(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_PM_START_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_PM_START_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___MASK (0x7CU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CLK_IF__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RESERVED_0_MASK (0x80U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RESERVED_0_SHIFT (7U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RESERVED_0_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE11_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_VALUE21 - Lane 1 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___MASK (0xFU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RX1_RXHS_DDL_PHASE_DATA_IF__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RESERVED_0_MASK (0xF0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RESERVED_0_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RESERVED_0_WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE21_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATAL1OFFSETCAL_OVRCNTRL - Lane 1 Offset Compensation Control */
/*! @{ */

#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_MASK (0x4U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_SHIFT (2U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_EN_RW_MASK (0x8U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_EN_RW_SHIFT (3U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_RW_MASK (0x10U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_RW_SHIFT (4U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_START_CALIBRATION_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_RW_MASK (0x40U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_RW_SHIFT (6U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_CAL_DONE_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_MASK (0x80U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_SHIFT (7U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL1OFFSETCAL_OVRCNTRL_RX1_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_MASK)
/*! @} */

/*! @name DPHY_DATALOFFSETCAL_OVRVALUE1 - Lane 0 Offset Compensation Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_MASK (0x1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_SHIFT (0U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___MASK (0xFEU)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___SHIFT (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE1_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___MASK)
/*! @} */

/*! @name DPHY_DATALANE_OFFSETCAL_COMPS1 - Lane Offset Compensation Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_START_CALIBRATION_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_START_CALIBRATION_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_START_CALIBRATION_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_START_CALIBRATION(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_START_CALIBRATION_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_START_CALIBRATION_MASK)

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_DONE_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_DONE_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_DONE_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_DONE(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_DONE_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_DONE_MASK)

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_ERROR_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_ERROR_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_ERROR_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_ERROR(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_ERROR_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS1_RX0_CAL_ERROR_MASK)
/*! @} */

/*! @name DPHY_DATALOFFSETCAL_VALUE1 - Lane Offset Compensation Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL__6__0___MASK (0x7FU)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL__6__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL__6__0___SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL__6__0___MASK)

#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_MASK (0x80U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_SHIFT (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_WIDTH (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_VALUE1_RX0_RXHS_DATA_OFFSET_CAL_EN_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_COMP1 - Lane DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_DDL_TUNE_EN_LANE0_IF_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_DDL_TUNE_EN_LANE0_IF_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_DDL_TUNE_EN_LANE0_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_DDL_TUNE_EN_LANE0_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_DDL_TUNE_EN_LANE0_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_DDL_TUNE_EN_LANE0_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_ERROR_FLAG_IF_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_ERROR_FLAG_IF_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_ERROR_FLAG_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_ERROR_FLAG_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_ERROR_FLAG_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_ERROR_FLAG_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_FINISHED_IF_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_FINISHED_IF_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_FINISHED_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_FINISHED_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_FINISHED_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_FINISHED_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_START_CALIBRATION_IF_MASK (0x8U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_START_CALIBRATION_IF_SHIFT (3U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_START_CALIBRATION_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_START_CALIBRATION_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_START_CALIBRATION_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP1_RX0_DDL_START_CALIBRATION_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_VALUE1 - Lane 0 DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_TUNE_IF__4__0___MASK (0x1FU)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_TUNE_IF__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_TUNE_IF__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_TUNE_IF__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_TUNE_IF__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_TUNE_IF__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_MASK (0x20U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_PHASE_CHANGE_P_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE1_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_OVRVALUE1 - Lane 0 DDL Tune Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___MASK (0x1FU)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_RW__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_RX0_RXHS_DDL_TUNE_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_reserved_0_MASK (0xC0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_reserved_0_SHIFT (6U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_reserved_0_WIDTH (2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_reserved_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_reserved_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE1_reserved_0_MASK)
/*! @} */

/*! @name DPHY_RX_LPRXPON_LANE2 - Lane 2 Low Power Receive Control */
/*! @{ */

#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponcd_lane2_rw_MASK (0x1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponcd_lane2_rw_SHIFT (0U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponcd_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponcd_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponcd_lane2_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponcd_lane2_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_bypass_lane2_rw_MASK (0x2U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_bypass_lane2_rw_SHIFT (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_bypass_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_bypass_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_bypass_lane2_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_bypass_lane2_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_lane2_rw_MASK (0x4U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_lane2_rw_SHIFT (2U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_lane2_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponlp_lane2_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_bypass_lane2_rw_MASK (0x8U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_bypass_lane2_rw_SHIFT (3U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_bypass_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_bypass_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_bypass_lane2_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_bypass_lane2_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_lane2_rw_MASK (0x10U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_lane2_rw_SHIFT (4U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_lane2_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE2_lprxponulp_lane2_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_reserved_0_MASK (0xE0U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_reserved_0_SHIFT (5U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_reserved_0_WIDTH (3U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE2_reserved_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE2_reserved_0_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE2_reserved_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_OVRVALUE12 - Lane 1 Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___MASK (0x7CU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_MASK (0x80U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_SHIFT (7U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE12_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_OVRVALUE22 - Lane 1 Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___MASK (0xFU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_MASK (0x10U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RESERVED_0_MASK (0xE0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RESERVED_0_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RESERVED_0_WIDTH (3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE22_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_COMPS2 - Lane 0 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LPRXDOUTULP_LANE0__1__0___MASK (0x3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LPRXDOUTULP_LANE0__1__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LPRXDOUTULP_LANE0__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LPRXDOUTULP_LANE0__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LPRXDOUTULP_LANE0__1__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LPRXDOUTULP_LANE0__1__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LSRXCLK_LANE0_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LSRXCLK_LANE0_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LSRXCLK_LANE0_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LSRXCLK_LANE0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LSRXCLK_LANE0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_LSRXCLK_LANE0_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALDONE_IF_MASK (0x8U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALDONE_IF_SHIFT (3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALDONE_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALDONE_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALDONE_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALDONE_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALFAILED_IF_MASK (0x10U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALFAILED_IF_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALFAILED_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALFAILED_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALFAILED_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS2_RX0_DESKEWCALFAILED_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_VALUE12 - Lane 1 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_PM_START_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_PM_START_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_PM_START_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_PM_START(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_PM_START_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_PM_START_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___MASK (0x7CU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CLK_IF__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RESERVED_0_MASK (0x80U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RESERVED_0_SHIFT (7U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RESERVED_0_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE12_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_VALUE22 - Lane 1 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___MASK (0xFU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RX1_RXHS_DDL_PHASE_DATA_IF__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RESERVED_0_MASK (0xF0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RESERVED_0_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RESERVED_0_WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE22_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATAL2OFFSETCAL_OVRCNTRL - Lane 1 Offset Compensation Control */
/*! @{ */

#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_MASK (0x4U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_SHIFT (2U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_EN_RW_MASK (0x8U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_EN_RW_SHIFT (3U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_RW_MASK (0x10U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_RW_SHIFT (4U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_START_CALIBRATION_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_RW_MASK (0x40U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_RW_SHIFT (6U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_CAL_DONE_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_MASK (0x80U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_SHIFT (7U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL2OFFSETCAL_OVRCNTRL_RX2_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_MASK)
/*! @} */

/*! @name DPHY_DATALOFFSETCAL_OVRVALUE2 - Lane 0 Offset Compensation Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_MASK (0x1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_SHIFT (0U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___MASK (0xFEU)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___SHIFT (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE2_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___MASK)
/*! @} */

/*! @name DPHY_DATALANE_OFFSETCAL_COMPS2 - Lane Offset Compensation Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_START_CALIBRATION_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_START_CALIBRATION_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_START_CALIBRATION_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_START_CALIBRATION(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_START_CALIBRATION_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_START_CALIBRATION_MASK)

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_DONE_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_DONE_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_DONE_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_DONE(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_DONE_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_DONE_MASK)

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_ERROR_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_ERROR_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_ERROR_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_ERROR(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_ERROR_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS2_RX0_CAL_ERROR_MASK)
/*! @} */

/*! @name DPHY_DATALOFFSETCAL_VALUE2 - Lane Offset Compensation Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL__6__0___MASK (0x7FU)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL__6__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL__6__0___SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL__6__0___MASK)

#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_MASK (0x80U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_SHIFT (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_WIDTH (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_VALUE2_RX0_RXHS_DATA_OFFSET_CAL_EN_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_COMP2 - Lane DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_DDL_TUNE_EN_LANE0_IF_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_DDL_TUNE_EN_LANE0_IF_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_DDL_TUNE_EN_LANE0_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_DDL_TUNE_EN_LANE0_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_DDL_TUNE_EN_LANE0_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_DDL_TUNE_EN_LANE0_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_ERROR_FLAG_IF_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_ERROR_FLAG_IF_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_ERROR_FLAG_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_ERROR_FLAG_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_ERROR_FLAG_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_ERROR_FLAG_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_FINISHED_IF_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_FINISHED_IF_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_FINISHED_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_FINISHED_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_FINISHED_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_FINISHED_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_START_CALIBRATION_IF_MASK (0x8U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_START_CALIBRATION_IF_SHIFT (3U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_START_CALIBRATION_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_START_CALIBRATION_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_START_CALIBRATION_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP2_RX0_DDL_START_CALIBRATION_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_VALUE2 - Lane 0 DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_TUNE_IF__4__0___MASK (0x1FU)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_TUNE_IF__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_TUNE_IF__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_TUNE_IF__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_TUNE_IF__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_TUNE_IF__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_MASK (0x20U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_PHASE_CHANGE_P_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE2_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_OVRVALUE2 - Lane 0 DDL Tune Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___MASK (0x1FU)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_RW__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_RX0_RXHS_DDL_TUNE_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_reserved_0_MASK (0xC0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_reserved_0_SHIFT (6U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_reserved_0_WIDTH (2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_reserved_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_reserved_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE2_reserved_0_MASK)
/*! @} */

/*! @name DPHY_RX_LPRXPON_LANE3 - Lane 3 Low Power Receive Control */
/*! @{ */

#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponcd_lane3_rw_MASK (0x1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponcd_lane3_rw_SHIFT (0U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponcd_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponcd_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponcd_lane3_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponcd_lane3_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_bypass_lane3_rw_MASK (0x2U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_bypass_lane3_rw_SHIFT (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_bypass_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_bypass_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_bypass_lane3_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_bypass_lane3_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_lane3_rw_MASK (0x4U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_lane3_rw_SHIFT (2U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_lane3_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponlp_lane3_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_bypass_lane3_rw_MASK (0x8U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_bypass_lane3_rw_SHIFT (3U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_bypass_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_bypass_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_bypass_lane3_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_bypass_lane3_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_lane3_rw_MASK (0x10U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_lane3_rw_SHIFT (4U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_lane3_rw_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE3_lprxponulp_lane3_rw_MASK)

#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_reserved_0_MASK (0xE0U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_reserved_0_SHIFT (5U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_reserved_0_WIDTH (3U)
#define MIPICSI2_DPHY_RX_LPRXPON_LANE3_reserved_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_RX_LPRXPON_LANE3_reserved_0_SHIFT)) & MIPICSI2_DPHY_RX_LPRXPON_LANE3_reserved_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_OVRVALUE3 - Lane 0 DDL Tune Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_1_MASK (0xEU)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_1_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_1_WIDTH (3U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_1(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_1_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_1_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4_MASK (0x10U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_RW__4_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RX0_RXHS_DDL_TUNE_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_0_MASK (0xC0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_0_SHIFT (6U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_0_WIDTH (2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_OVRVALUE3_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_OVRVALUE13 - Lane 1 Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___MASK (0x7CU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_RW__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_MASK (0x80U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_SHIFT (7U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE13_RX1_RXHS_DDL_PHASE_CLK_OVR_EN_RW_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_OVRVALUE23 - Lane 1 Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___MASK (0xFU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_RW__3__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_MASK (0x10U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RX1_RXHS_DDL_PHASE_DATA_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RESERVED_0_MASK (0xE0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RESERVED_0_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RESERVED_0_WIDTH (3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_OVRVALUE23_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_COMPS3 - Lane 0 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LPRXDOUTULP_LANE0__1__0___MASK (0x3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LPRXDOUTULP_LANE0__1__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LPRXDOUTULP_LANE0__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LPRXDOUTULP_LANE0__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LPRXDOUTULP_LANE0__1__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LPRXDOUTULP_LANE0__1__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LSRXCLK_LANE0_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LSRXCLK_LANE0_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LSRXCLK_LANE0_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LSRXCLK_LANE0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LSRXCLK_LANE0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_LSRXCLK_LANE0_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALDONE_IF_MASK (0x8U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALDONE_IF_SHIFT (3U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALDONE_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALDONE_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALDONE_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALDONE_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALFAILED_IF_MASK (0x10U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALFAILED_IF_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALFAILED_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALFAILED_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALFAILED_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_COMPS3_RX0_DESKEWCALFAILED_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_VALUE13 - Lane 1 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_PM_START_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_PM_START_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_PM_START_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_PM_START(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_PM_START_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_PM_START_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CHANGE_P_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___MASK (0x7CU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CLK_IF__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RX1_RXHS_DDL_PHASE_CLK_IF__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RESERVED_0_MASK (0x80U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RESERVED_0_SHIFT (7U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RESERVED_0_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE13_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DESKEW_VALUE23 - Lane 1 Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___MASK (0xFU)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RX1_RXHS_DDL_PHASE_DATA_IF__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RX1_RXHS_DDL_PHASE_DATA_IF__3__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RESERVED_0_MASK (0xF0U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RESERVED_0_SHIFT (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RESERVED_0_WIDTH (4U)
#define MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RESERVED_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RESERVED_0_SHIFT)) & MIPICSI2_DPHY_DATALANE_DESKEW_VALUE23_RESERVED_0_MASK)
/*! @} */

/*! @name DPHY_DATAL3OFFSETCAL_OVRCNTRL - Lane 1 Offset Compensation Control */
/*! @{ */

#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_MASK (0x4U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_SHIFT (2U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_EN_RW_MASK (0x8U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_EN_RW_SHIFT (3U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_RW_MASK (0x10U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_RW_SHIFT (4U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_START_CALIBRATION_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_EN_RW_MASK (0x20U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_EN_RW_SHIFT (5U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_RW_MASK (0x40U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_RW_SHIFT (6U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_CAL_DONE_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_MASK (0x80U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_SHIFT (7U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_DATAL3OFFSETCAL_OVRCNTRL_RX3_RXHS_DATA_OFFSET_CAL_EN_OVR_EN_RW_MASK)
/*! @} */

/*! @name DPHY_DATALOFFSETCAL_OVRVALUE3 - Lane 0 Offset Compensation Control */
/*! @{ */

#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_MASK (0x1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_SHIFT (0U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_OVR_RW_MASK)

#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___MASK (0xFEU)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___SHIFT (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_OVRVALUE3_RX0_RXHS_DATA_OFFSET_CAL_OVR_RW__6__0___MASK)
/*! @} */

/*! @name DPHY_DATALANE_OFFSETCAL_COMPS3 - Lane Offset Compensation Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_START_CALIBRATION_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_START_CALIBRATION_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_START_CALIBRATION_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_START_CALIBRATION(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_START_CALIBRATION_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_START_CALIBRATION_MASK)

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_DONE_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_DONE_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_DONE_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_DONE(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_DONE_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_DONE_MASK)

#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_ERROR_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_ERROR_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_ERROR_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_ERROR(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_ERROR_SHIFT)) & MIPICSI2_DPHY_DATALANE_OFFSETCAL_COMPS3_RX0_CAL_ERROR_MASK)
/*! @} */

/*! @name DPHY_DATALOFFSETCAL_VALUE3 - Lane Offset Compensation Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL__6__0___MASK (0x7FU)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL__6__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL__6__0___WIDTH (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL__6__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL__6__0___SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL__6__0___MASK)

#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_MASK (0x80U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_SHIFT (7U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_WIDTH (1U)
#define MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_SHIFT)) & MIPICSI2_DPHY_DATALOFFSETCAL_VALUE3_RX0_RXHS_DATA_OFFSET_CAL_EN_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_COMP3 - Lane DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_DDL_TUNE_EN_LANE0_IF_MASK (0x1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_DDL_TUNE_EN_LANE0_IF_SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_DDL_TUNE_EN_LANE0_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_DDL_TUNE_EN_LANE0_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_DDL_TUNE_EN_LANE0_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_DDL_TUNE_EN_LANE0_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_ERROR_FLAG_IF_MASK (0x2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_ERROR_FLAG_IF_SHIFT (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_ERROR_FLAG_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_ERROR_FLAG_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_ERROR_FLAG_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_ERROR_FLAG_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_FINISHED_IF_MASK (0x4U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_FINISHED_IF_SHIFT (2U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_FINISHED_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_FINISHED_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_FINISHED_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_FINISHED_IF_MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_START_CALIBRATION_IF_MASK (0x8U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_START_CALIBRATION_IF_SHIFT (3U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_START_CALIBRATION_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_START_CALIBRATION_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_START_CALIBRATION_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_COMP3_RX0_DDL_START_CALIBRATION_IF_MASK)
/*! @} */

/*! @name DPHY_DATALANE_DDLTUNE_VALUE3 - Lane 0 DDL Tune Observability */
/*! @{ */

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_TUNE_IF__4__0___MASK (0x1FU)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_TUNE_IF__4__0___SHIFT (0U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_TUNE_IF__4__0___WIDTH (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_TUNE_IF__4__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_TUNE_IF__4__0___SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_TUNE_IF__4__0___MASK)

#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_MASK (0x20U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT (5U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_WIDTH (1U)
#define MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_PHASE_CHANGE_P_IF(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_SHIFT)) & MIPICSI2_DPHY_DATALANE_DDLTUNE_VALUE3_RX0_RXHS_DDL_PHASE_CHANGE_P_IF_MASK)
/*! @} */

/*! @name DPHY_TURNAROUND_CFG - System Timers */
/*! @{ */

#define MIPICSI2_DPHY_TURNAROUND_CFG_TTAWAIT_RW__5__0___MASK (0x3FU)
#define MIPICSI2_DPHY_TURNAROUND_CFG_TTAWAIT_RW__5__0___SHIFT (0U)
#define MIPICSI2_DPHY_TURNAROUND_CFG_TTAWAIT_RW__5__0___WIDTH (6U)
#define MIPICSI2_DPHY_TURNAROUND_CFG_TTAWAIT_RW__5__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TURNAROUND_CFG_TTAWAIT_RW__5__0___SHIFT)) & MIPICSI2_DPHY_TURNAROUND_CFG_TTAWAIT_RW__5__0___MASK)
/*! @} */

/*! @name DPHY_TX_CLKLANETIMERS_CTRL1 - Clock Lane's HS-TX Tclk-prepare Counter Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_rw__5__0___MASK (0x3FU)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_rw__5__0___SHIFT (0U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_rw__5__0___WIDTH (6U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_rw__5__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_rw__5__0___SHIFT)) & MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_rw__5__0___MASK)

#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_en_rw_MASK (0x40U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_en_rw_SHIFT (6U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_en_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_en_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_en_rw_SHIFT)) & MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsprpr_clklane_ovr_en_rw_MASK)

#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsrqst_clklane_ovr_en_rw_MASK (0x80U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsrqst_clklane_ovr_en_rw_SHIFT (7U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsrqst_clklane_ovr_en_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsrqst_clklane_ovr_en_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsrqst_clklane_ovr_en_rw_SHIFT)) & MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL1_hstxthsrqst_clklane_ovr_en_rw_MASK)
/*! @} */

/*! @name DPHY_TX_CLKLANETIMERS_CTRL2 - Clock Lane's HS-TX Tclk-request Counter Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL2_hstxthsrqst_clklane_ovr_rw__7__0___MASK (0xFFU)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL2_hstxthsrqst_clklane_ovr_rw__7__0___SHIFT (0U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL2_hstxthsrqst_clklane_ovr_rw__7__0___WIDTH (8U)
#define MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL2_hstxthsrqst_clklane_ovr_rw__7__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL2_hstxthsrqst_clklane_ovr_rw__7__0___SHIFT)) & MIPICSI2_DPHY_TX_CLKLANETIMERS_CTRL2_hstxthsrqst_clklane_ovr_rw__7__0___MASK)
/*! @} */

/*! @name DPHY_TX_DATALANETIMERS_CTRL1 - Data Lane's HS-TX Ths-prepare Counter Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_rw__5__0___MASK (0x3FU)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_rw__5__0___SHIFT (0U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_rw__5__0___WIDTH (6U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_rw__5__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_rw__5__0___SHIFT)) & MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_rw__5__0___MASK)

#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_en_rw_MASK (0x40U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_en_rw_SHIFT (6U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_en_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_en_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_en_rw_SHIFT)) & MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsprpr_datalanes_ovr_en_rw_MASK)

#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsrqst_datalanes_ovr_en_rw_MASK (0x80U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsrqst_datalanes_ovr_en_rw_SHIFT (7U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsrqst_datalanes_ovr_en_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsrqst_datalanes_ovr_en_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsrqst_datalanes_ovr_en_rw_SHIFT)) & MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL1_hstxthsrqst_datalanes_ovr_en_rw_MASK)
/*! @} */

/*! @name DPHY_TX_DATALANETIMERS_CTRL2 - Data lane's HS-TX Ths-request Counter Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL2_hstxthsrqst_datalanes_ovr_rw__7__0___MASK (0xFFU)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL2_hstxthsrqst_datalanes_ovr_rw__7__0___SHIFT (0U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL2_hstxthsrqst_datalanes_ovr_rw__7__0___WIDTH (8U)
#define MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL2_hstxthsrqst_datalanes_ovr_rw__7__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL2_hstxthsrqst_datalanes_ovr_rw__7__0___SHIFT)) & MIPICSI2_DPHY_TX_DATALANETIMERS_CTRL2_hstxthsrqst_datalanes_ovr_rw__7__0___MASK)
/*! @} */

/*! @name DPHY_PLL_VREF_CONFIG - PLL control */
/*! @{ */

#define MIPICSI2_DPHY_PLL_VREF_CONFIG_PLL_MPLL_PROG_RW__7__0___MASK (0xFFU)
#define MIPICSI2_DPHY_PLL_VREF_CONFIG_PLL_MPLL_PROG_RW__7__0___SHIFT (0U)
#define MIPICSI2_DPHY_PLL_VREF_CONFIG_PLL_MPLL_PROG_RW__7__0___WIDTH (8U)
#define MIPICSI2_DPHY_PLL_VREF_CONFIG_PLL_MPLL_PROG_RW__7__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_PLL_VREF_CONFIG_PLL_MPLL_PROG_RW__7__0___SHIFT)) & MIPICSI2_DPHY_PLL_VREF_CONFIG_PLL_MPLL_PROG_RW__7__0___MASK)
/*! @} */

/*! @name DPHY_CB_VBE_SEL - Common Block Control */
/*! @{ */

#define MIPICSI2_DPHY_CB_VBE_SEL_CB_CHOP_CLK_EN_RW_MASK (0x1U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_CHOP_CLK_EN_RW_SHIFT (0U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_CHOP_CLK_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_CHOP_CLK_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CB_VBE_SEL_CB_CHOP_CLK_EN_RW_SHIFT)) & MIPICSI2_DPHY_CB_VBE_SEL_CB_CHOP_CLK_EN_RW_MASK)

#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_CHOP_CLK_RW_MASK (0x2U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_CHOP_CLK_RW_SHIFT (1U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_CHOP_CLK_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_CHOP_CLK_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_CHOP_CLK_RW_SHIFT)) & MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_CHOP_CLK_RW_MASK)

#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_V400_PROG_RW__2__0___MASK (0x1CU)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_V400_PROG_RW__2__0___SHIFT (2U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_V400_PROG_RW__2__0___WIDTH (3U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_V400_PROG_RW__2__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_V400_PROG_RW__2__0___SHIFT)) & MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_V400_PROG_RW__2__0___MASK)

#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_VREFCD_LPRX_RW__1__0___MASK (0x60U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_VREFCD_LPRX_RW__1__0___SHIFT (5U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_VREFCD_LPRX_RW__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_VREFCD_LPRX_RW__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_VREFCD_LPRX_RW__1__0___SHIFT)) & MIPICSI2_DPHY_CB_VBE_SEL_CB_SEL_VREFCD_LPRX_RW__1__0___MASK)
/*! @} */

/*! @name DPHY_ATB_CB_ATB_VBE_SEL - Common Block Control */
/*! @{ */

#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREF_LPRX_RW_1_0_MASK (0x3U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREF_LPRX_RW_1_0_SHIFT (0U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREF_LPRX_RW_1_0_WIDTH (2U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREF_LPRX_RW_1_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREF_LPRX_RW_1_0_SHIFT)) & MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREF_LPRX_RW_1_0_MASK)

#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREFLPTX_PROG_RW_MASK (0x4U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREFLPTX_PROG_RW_SHIFT (2U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREFLPTX_PROG_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREFLPTX_PROG_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREFLPTX_PROG_RW_SHIFT)) & MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_SEL_VREFLPTX_PROG_RW_MASK)

#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_TXLP_PROG_RW_2_0_MASK (0x38U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_TXLP_PROG_RW_2_0_SHIFT (3U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_TXLP_PROG_RW_2_0_WIDTH (3U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_TXLP_PROG_RW_2_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_TXLP_PROG_RW_2_0_SHIFT)) & MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_TXLP_PROG_RW_2_0_MASK)

#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_ATB_VBE_SEL_RW_MASK (0x40U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_ATB_VBE_SEL_RW_SHIFT (6U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_ATB_VBE_SEL_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_ATB_VBE_SEL_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_ATB_VBE_SEL_RW_SHIFT)) & MIPICSI2_DPHY_ATB_CB_ATB_VBE_SEL_CB_ATB_VBE_SEL_RW_MASK)
/*! @} */

/*! @name DPHY_TX_RDWR_TERM_CAL_0 - Termination Calibration Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_EN_RW_MASK (0x1U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_EN_RW_SHIFT (0U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_RW_MASK (0x2U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_RW_SHIFT (1U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_RW_SHIFT)) & MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_OVR_RW_MASK)

#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_EN_RW_MASK (0x4U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_EN_RW_SHIFT (2U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_RW_MASK (0x8U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_RW_SHIFT (3U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_RW_WIDTH (1U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_RW_SHIFT)) & MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_EN_UP_OVR_RW_MASK)

#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_REPL_OVR_RW__3__0___MASK (0xF0U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_REPL_OVR_RW__3__0___SHIFT (4U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_REPL_OVR_RW__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_REPL_OVR_RW__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_REPL_OVR_RW__3__0___SHIFT)) & MIPICSI2_DPHY_TX_RDWR_TERM_CAL_0_CB_CAL_REPL_OVR_RW__3__0___MASK)
/*! @} */

/*! @name DPHY_TX_TERM_CAL_OVR - Termination Calibration Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_OVR_EN_RW_MASK (0x1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_OVR_EN_RW_SHIFT (0U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_BYPASS_RW_MASK (0x2U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_BYPASS_RW_SHIFT (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_BYPASS_RW_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_BYPASS_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_BYPASS_RW_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_BYPASS_RW_MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_RW__3__0___MASK (0x3CU)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_RW__3__0___SHIFT (2U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_RW__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_RW__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_RW__3__0___SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_RW__3__0___MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_EN_RW_MASK (0x40U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_EN_RW_SHIFT (6U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_EN_RW_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_EN_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_EN_RW_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_CAL_REPL_UP_OVR_EN_RW_MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_EN_45_OHM_RW_MASK (0x80U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_EN_45_OHM_RW_SHIFT (7U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_EN_45_OHM_RW_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_EN_45_OHM_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_EN_45_OHM_RW_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_OVR_CB_EN_45_OHM_RW_MASK)
/*! @} */

/*! @name DPHY_TX_TERM_CAL_0 - Termination Calibration Observability */
/*! @{ */

#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_MASK (0x1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_SHIFT (0U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_UP_MASK (0x2U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_UP_SHIFT (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_UP_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_UP(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_UP_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_EN_UP_MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_REPL__3__0___MASK (0x3CU)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_REPL__3__0___SHIFT (2U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_REPL__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_REPL__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_REPL__3__0___SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_0_CB_CAL_REPL__3__0___MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_SEL_UP_1ST_MASK (0x40U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_SEL_UP_1ST_SHIFT (6U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_SEL_UP_1ST_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_0_CB_SEL_UP_1ST(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_0_CB_SEL_UP_1ST_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_0_CB_SEL_UP_1ST_MASK)
/*! @} */

/*! @name DPHY_TX_TERM_CAL_1 - Termination Calibration Observability */
/*! @{ */

#define MIPICSI2_DPHY_TX_TERM_CAL_1_CB_CAL_REPL_UP__3__0___MASK (0xFU)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_CB_CAL_REPL_UP__3__0___SHIFT (0U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_CB_CAL_REPL_UP__3__0___WIDTH (4U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_CB_CAL_REPL_UP__3__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_1_CB_CAL_REPL_UP__3__0___SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_1_CB_CAL_REPL_UP__3__0___MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_1_CB_COMP_OUT_MASK (0x10U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_CB_COMP_OUT_SHIFT (4U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_CB_COMP_OUT_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_CB_COMP_OUT(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_1_CB_COMP_OUT_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_1_CB_COMP_OUT_MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_EN_MASK (0x20U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_EN_SHIFT (5U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_EN_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_EN(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_EN_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_EN_MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_UP_EN_MASK (0x40U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_UP_EN_SHIFT (6U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_UP_EN_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_UP_EN(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_UP_EN_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_UP_EN_MASK)

#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_DONE_MASK (0x80U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_DONE_SHIFT (7U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_DONE_WIDTH (1U)
#define MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_DONE(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_DONE_SHIFT)) & MIPICSI2_DPHY_TX_TERM_CAL_1_RESCAL_DONE_MASK)
/*! @} */

/*! @name DPHY_TERMCAL_STAT2 - Termination Calibration Observability */
/*! @{ */

#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_ERROR_MASK (0x1U)
#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_ERROR_SHIFT (0U)
#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_ERROR_WIDTH (1U)
#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_ERROR(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_ERROR_SHIFT)) & MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_ERROR_MASK)

#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_DONE_MASK (0x2U)
#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_DONE_SHIFT (1U)
#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_DONE_WIDTH (1U)
#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_DONE(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_DONE_SHIFT)) & MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_DONE_MASK)

#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_ERROR_MASK (0x4U)
#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_ERROR_SHIFT (2U)
#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_ERROR_WIDTH (1U)
#define MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_ERROR(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_ERROR_SHIFT)) & MIPICSI2_DPHY_TERMCAL_STAT2_RESCAL_UP_ERROR_MASK)
/*! @} */

/*! @name DPHY_CLKLANE_POLCFG - Clock Lane Control */
/*! @{ */

#define MIPICSI2_DPHY_CLKLANE_POLCFG_POLARITY_CLKLANE_RW_MASK (0x1U)
#define MIPICSI2_DPHY_CLKLANE_POLCFG_POLARITY_CLKLANE_RW_SHIFT (0U)
#define MIPICSI2_DPHY_CLKLANE_POLCFG_POLARITY_CLKLANE_RW_WIDTH (1U)
#define MIPICSI2_DPHY_CLKLANE_POLCFG_POLARITY_CLKLANE_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_CLKLANE_POLCFG_POLARITY_CLKLANE_RW_SHIFT)) & MIPICSI2_DPHY_CLKLANE_POLCFG_POLARITY_CLKLANE_RW_MASK)
/*! @} */

/*! @name DPHY_TX_LANE0EN - Lane 0 Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_LANE0EN_hstxena_bypass_lane0_rw_MASK (0x1U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxena_bypass_lane0_rw_SHIFT (0U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxena_bypass_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxena_bypass_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0EN_hstxena_bypass_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0EN_hstxena_bypass_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0EN_hstxena_lane0_rw_MASK (0x2U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxena_lane0_rw_SHIFT (1U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxena_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxena_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0EN_hstxena_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0EN_hstxena_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0EN_hstxpon_bypass_lane0_rw_MASK (0x4U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxpon_bypass_lane0_rw_SHIFT (2U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxpon_bypass_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxpon_bypass_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0EN_hstxpon_bypass_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0EN_hstxpon_bypass_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0EN_hstxpon_lane0_rw_MASK (0x8U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxpon_lane0_rw_SHIFT (3U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxpon_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstxpon_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0EN_hstxpon_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0EN_hstxpon_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_bypass_lane0_rw_MASK (0x10U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_bypass_lane0_rw_SHIFT (4U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_bypass_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_bypass_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_bypass_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_bypass_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_lane0_rw_MASK (0x20U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_lane0_rw_SHIFT (5U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0EN_hstx_serial_en_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0EN_hstx_trailer_en_lane0_rw_MASK (0x40U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstx_trailer_en_lane0_rw_SHIFT (6U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstx_trailer_en_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0EN_hstx_trailer_en_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0EN_hstx_trailer_en_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0EN_hstx_trailer_en_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0EN_lprxponcd_bypass_lane0_rw_MASK (0x80U)
#define MIPICSI2_DPHY_TX_LANE0EN_lprxponcd_bypass_lane0_rw_SHIFT (7U)
#define MIPICSI2_DPHY_TX_LANE0EN_lprxponcd_bypass_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0EN_lprxponcd_bypass_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0EN_lprxponcd_bypass_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0EN_lprxponcd_bypass_lane0_rw_MASK)
/*! @} */

/*! @name DPHY_TX_LANE0LPEN - Lane 0 Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponcd_lane0_rw_MASK (0x1U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponcd_lane0_rw_SHIFT (0U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponcd_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponcd_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0LPEN_lprxponcd_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0LPEN_lprxponcd_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_bypass_lane0_rw_MASK (0x2U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_bypass_lane0_rw_SHIFT (1U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_bypass_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_bypass_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_bypass_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_bypass_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_lane0_rw_MASK (0x4U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_lane0_rw_SHIFT (2U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0LPEN_lprxponlp_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_bypass_lane0_rw_MASK (0x8U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_bypass_lane0_rw_SHIFT (3U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_bypass_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_bypass_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_bypass_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_bypass_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_lane0_rw_MASK (0x10U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_lane0_rw_SHIFT (4U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0LPEN_lprxponulp_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_bypass_lane0_rw_MASK (0x20U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_bypass_lane0_rw_SHIFT (5U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_bypass_lane0_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_bypass_lane0_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_bypass_lane0_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_bypass_lane0_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_lane0_rw__1__0___MASK (0xC0U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_lane0_rw__1__0___SHIFT (6U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_lane0_rw__1__0___WIDTH (2U)
#define MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_lane0_rw__1__0__(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_lane0_rw__1__0___SHIFT)) & MIPICSI2_DPHY_TX_LANE0LPEN_lptxdin_lane0_rw__1__0___MASK)
/*! @} */

/*! @name DPHY_ATB_DATA_LANE1 - Lane 1 Control */
/*! @{ */

#define MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_LPTX1200_ON_LANE1_RW_MASK (0x1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_LPTX1200_ON_LANE1_RW_SHIFT (0U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_LPTX1200_ON_LANE1_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_LPTX1200_ON_LANE1_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_LPTX1200_ON_LANE1_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_LPTX1200_ON_LANE1_RW_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_SEL_LANE1_RW_3_0_MASK (0x1EU)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_SEL_LANE1_RW_3_0_SHIFT (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_SEL_LANE1_RW_3_0_WIDTH (4U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_SEL_LANE1_RW_3_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_SEL_LANE1_RW_3_0_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE1_ATB_SEL_LANE1_RW_3_0_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_BYPASS_LANE1_RW_MASK (0x20U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_BYPASS_LANE1_RW_SHIFT (5U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_BYPASS_LANE1_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_BYPASS_LANE1_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_BYPASS_LANE1_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_BYPASS_LANE1_RW_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_LANE1_RW_MASK (0x40U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_LANE1_RW_SHIFT (6U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_LANE1_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_LANE1_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_LANE1_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE1_BINTPON_LANE1_RW_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE1_HSTXBITREV_LANE1_RW_MASK (0x80U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_HSTXBITREV_LANE1_RW_SHIFT (7U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_HSTXBITREV_LANE1_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE1_HSTXBITREV_LANE1_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE1_HSTXBITREV_LANE1_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE1_HSTXBITREV_LANE1_RW_MASK)
/*! @} */

/*! @name DPHY_TX_LANE1EN - Lane 1 Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_LANE1EN_hstxena_bypass_lane1_rw_MASK (0x1U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxena_bypass_lane1_rw_SHIFT (0U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxena_bypass_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxena_bypass_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE1EN_hstxena_bypass_lane1_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE1EN_hstxena_bypass_lane1_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE1EN_hstxena_lane1_rw_MASK (0x2U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxena_lane1_rw_SHIFT (1U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxena_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxena_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE1EN_hstxena_lane1_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE1EN_hstxena_lane1_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE1EN_hstxpon_bypass_lane1_rw_MASK (0x4U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxpon_bypass_lane1_rw_SHIFT (2U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxpon_bypass_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxpon_bypass_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE1EN_hstxpon_bypass_lane1_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE1EN_hstxpon_bypass_lane1_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE1EN_hstxpon_lane1_rw_MASK (0x8U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxpon_lane1_rw_SHIFT (3U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxpon_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstxpon_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE1EN_hstxpon_lane1_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE1EN_hstxpon_lane1_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_bypass_lane1_rw_MASK (0x10U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_bypass_lane1_rw_SHIFT (4U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_bypass_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_bypass_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_bypass_lane1_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_bypass_lane1_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_lane1_rw_MASK (0x20U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_lane1_rw_SHIFT (5U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_lane1_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE1EN_hstx_serial_en_lane1_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE1EN_hstx_trailer_en_lane1_rw_MASK (0x40U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstx_trailer_en_lane1_rw_SHIFT (6U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstx_trailer_en_lane1_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE1EN_hstx_trailer_en_lane1_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE1EN_hstx_trailer_en_lane1_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE1EN_hstx_trailer_en_lane1_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE1EN_reserved_0_MASK (0x80U)
#define MIPICSI2_DPHY_TX_LANE1EN_reserved_0_SHIFT (7U)
#define MIPICSI2_DPHY_TX_LANE1EN_reserved_0_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE1EN_reserved_0(x)   (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE1EN_reserved_0_SHIFT)) & MIPICSI2_DPHY_TX_LANE1EN_reserved_0_MASK)
/*! @} */

/*! @name DPHY_ATB_DATA_LANE2 - Lane 2 Control */
/*! @{ */

#define MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_LPTX1200_ON_LANE2_RW_MASK (0x1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_LPTX1200_ON_LANE2_RW_SHIFT (0U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_LPTX1200_ON_LANE2_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_LPTX1200_ON_LANE2_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_LPTX1200_ON_LANE2_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_LPTX1200_ON_LANE2_RW_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_SEL_LANE2_RW_3_0_MASK (0x1EU)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_SEL_LANE2_RW_3_0_SHIFT (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_SEL_LANE2_RW_3_0_WIDTH (4U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_SEL_LANE2_RW_3_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_SEL_LANE2_RW_3_0_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE2_ATB_SEL_LANE2_RW_3_0_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_BYPASS_LANE2_RW_MASK (0x20U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_BYPASS_LANE2_RW_SHIFT (5U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_BYPASS_LANE2_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_BYPASS_LANE2_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_BYPASS_LANE2_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_BYPASS_LANE2_RW_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_LANE2_RW_MASK (0x40U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_LANE2_RW_SHIFT (6U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_LANE2_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_LANE2_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_LANE2_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE2_BINTPON_LANE2_RW_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE2_HSTXBITREV_LANE2_RW_MASK (0x80U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_HSTXBITREV_LANE2_RW_SHIFT (7U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_HSTXBITREV_LANE2_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE2_HSTXBITREV_LANE2_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE2_HSTXBITREV_LANE2_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE2_HSTXBITREV_LANE2_RW_MASK)
/*! @} */

/*! @name DPHY_TX_LANE2EN - Lane 2 Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_LANE2EN_hstxena_bypass_lane2_rw_MASK (0x1U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxena_bypass_lane2_rw_SHIFT (0U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxena_bypass_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxena_bypass_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE2EN_hstxena_bypass_lane2_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE2EN_hstxena_bypass_lane2_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE2EN_hstxena_lane2_rw_MASK (0x2U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxena_lane2_rw_SHIFT (1U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxena_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxena_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE2EN_hstxena_lane2_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE2EN_hstxena_lane2_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE2EN_hstxpon_bypass_lane2_rw_MASK (0x4U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxpon_bypass_lane2_rw_SHIFT (2U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxpon_bypass_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxpon_bypass_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE2EN_hstxpon_bypass_lane2_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE2EN_hstxpon_bypass_lane2_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE2EN_hstxpon_lane2_rw_MASK (0x8U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxpon_lane2_rw_SHIFT (3U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxpon_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstxpon_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE2EN_hstxpon_lane2_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE2EN_hstxpon_lane2_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_bypass_lane2_rw_MASK (0x10U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_bypass_lane2_rw_SHIFT (4U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_bypass_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_bypass_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_bypass_lane2_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_bypass_lane2_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_lane2_rw_MASK (0x20U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_lane2_rw_SHIFT (5U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_lane2_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE2EN_hstx_serial_en_lane2_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE2EN_hstx_trailer_en_lane2_rw_MASK (0x40U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstx_trailer_en_lane2_rw_SHIFT (6U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstx_trailer_en_lane2_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE2EN_hstx_trailer_en_lane2_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE2EN_hstx_trailer_en_lane2_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE2EN_hstx_trailer_en_lane2_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE2EN_reserved_0_MASK (0x80U)
#define MIPICSI2_DPHY_TX_LANE2EN_reserved_0_SHIFT (7U)
#define MIPICSI2_DPHY_TX_LANE2EN_reserved_0_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE2EN_reserved_0(x)   (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE2EN_reserved_0_SHIFT)) & MIPICSI2_DPHY_TX_LANE2EN_reserved_0_MASK)
/*! @} */

/*! @name DPHY_ATB_DATA_LANE3 - Lane 3 Control */
/*! @{ */

#define MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_LPTX1200_ON_LANE3_RW_MASK (0x1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_LPTX1200_ON_LANE3_RW_SHIFT (0U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_LPTX1200_ON_LANE3_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_LPTX1200_ON_LANE3_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_LPTX1200_ON_LANE3_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_LPTX1200_ON_LANE3_RW_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_SEL_LANE3_RW_3_0_MASK (0x1EU)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_SEL_LANE3_RW_3_0_SHIFT (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_SEL_LANE3_RW_3_0_WIDTH (4U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_SEL_LANE3_RW_3_0(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_SEL_LANE3_RW_3_0_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE3_ATB_SEL_LANE3_RW_3_0_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_BYPASS_LANE3_RW_MASK (0x20U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_BYPASS_LANE3_RW_SHIFT (5U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_BYPASS_LANE3_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_BYPASS_LANE3_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_BYPASS_LANE3_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_BYPASS_LANE3_RW_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_LANE3_RW_MASK (0x40U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_LANE3_RW_SHIFT (6U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_LANE3_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_LANE3_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_LANE3_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE3_BINTPON_LANE3_RW_MASK)

#define MIPICSI2_DPHY_ATB_DATA_LANE3_HSTXBITREV_LANE3_RW_MASK (0x80U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_HSTXBITREV_LANE3_RW_SHIFT (7U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_HSTXBITREV_LANE3_RW_WIDTH (1U)
#define MIPICSI2_DPHY_ATB_DATA_LANE3_HSTXBITREV_LANE3_RW(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_ATB_DATA_LANE3_HSTXBITREV_LANE3_RW_SHIFT)) & MIPICSI2_DPHY_ATB_DATA_LANE3_HSTXBITREV_LANE3_RW_MASK)
/*! @} */

/*! @name DPHY_TX_LANE3EN - Lane 3 Control */
/*! @{ */

#define MIPICSI2_DPHY_TX_LANE3EN_hstxena_bypass_lane3_rw_MASK (0x1U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxena_bypass_lane3_rw_SHIFT (0U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxena_bypass_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxena_bypass_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE3EN_hstxena_bypass_lane3_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE3EN_hstxena_bypass_lane3_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE3EN_hstxena_lane3_rw_MASK (0x2U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxena_lane3_rw_SHIFT (1U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxena_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxena_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE3EN_hstxena_lane3_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE3EN_hstxena_lane3_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE3EN_hstxpon_bypass_lane3_rw_MASK (0x4U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxpon_bypass_lane3_rw_SHIFT (2U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxpon_bypass_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxpon_bypass_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE3EN_hstxpon_bypass_lane3_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE3EN_hstxpon_bypass_lane3_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE3EN_hstxpon_lane3_rw_MASK (0x8U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxpon_lane3_rw_SHIFT (3U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxpon_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstxpon_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE3EN_hstxpon_lane3_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE3EN_hstxpon_lane3_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_bypass_lane3_rw_MASK (0x10U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_bypass_lane3_rw_SHIFT (4U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_bypass_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_bypass_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_bypass_lane3_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_bypass_lane3_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_lane3_rw_MASK (0x20U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_lane3_rw_SHIFT (5U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_lane3_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE3EN_hstx_serial_en_lane3_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE3EN_hstx_trailer_en_lane3_rw_MASK (0x40U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstx_trailer_en_lane3_rw_SHIFT (6U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstx_trailer_en_lane3_rw_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE3EN_hstx_trailer_en_lane3_rw(x) (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE3EN_hstx_trailer_en_lane3_rw_SHIFT)) & MIPICSI2_DPHY_TX_LANE3EN_hstx_trailer_en_lane3_rw_MASK)

#define MIPICSI2_DPHY_TX_LANE3EN_reserved_0_MASK (0x80U)
#define MIPICSI2_DPHY_TX_LANE3EN_reserved_0_SHIFT (7U)
#define MIPICSI2_DPHY_TX_LANE3EN_reserved_0_WIDTH (1U)
#define MIPICSI2_DPHY_TX_LANE3EN_reserved_0(x)   (((uint8_t)(((uint8_t)(x)) << MIPICSI2_DPHY_TX_LANE3EN_reserved_0_SHIFT)) & MIPICSI2_DPHY_TX_LANE3EN_reserved_0_MASK)
/*! @} */

/*!
 * @}
 */ /* end of group MIPICSI2_Register_Masks */

/*!
 * @}
 */ /* end of group MIPICSI2_Peripheral_Access_Layer */

#endif  /* #if !defined(S32R45_MIPICSI2_H_) */
