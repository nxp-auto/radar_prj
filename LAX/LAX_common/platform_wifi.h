/*
 * Copyright 2016-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
//! @file           platform_wifi.h
//! @brief          WiFi Platform Specific Parameters
//!
//! WiFi LAX/Platform Specific data widths, hard-wired connectivity, static
//! DMA channel mapping, etc.

#ifndef PLATFORM_WIFI_H
#define PLATFORM_WIFI_H

// -----------------------------------------------------------------------------
//! defgroup       GROUP_PLATFORM_WIFI
//!
//! WiFi LAX/Platform Specific data widths, hard-wired connectivity, static
//! DMA channel mapping, etc.
//!
//! @{
// -----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//! @brief LAX Number of AUs = 16
// ---------------------------------------------------------------------------
#define PS_LAX_AU_COUNT                 VSPA_AU_COUNT // CW Build Pre-Processor Macro = 16

// ---------------------------------------------------------------------------
//! @brief LAX Memory Sizes (make sure linker script sizes match these!)
// ---------------------------------------------------------------------------
#define PS_PMEM_VCPU_BYTES              65536
#define PS_PMEM_IPPU_BYTES              2048
#define PS_DMEM_VCPU_BYTES              131072
#define PS_DMEM_IPPU_BYTES              8192

// ---------------------------------------------------------------------------
//! @brief LAX Supported Word Widths
// ---------------------------------------------------------------------------
#define PS_LAX_HALF_WORD_NUM_BITS       16
#define PS_LAX_WORD_NUM_BITS            32

// ----------------------------------------------------------------------------
//! @brief WiFi AXI Bus Width Alignment Constants
// ----------------------------------------------------------------------------
#define PS_AXI_BUS_WIDTH_BITS_WIFI      256U
#define PS_AXI_BUS_WIDTH_BITS           (PS_AXI_BUS_WIDTH_BITS_WIFI)    //256
#define PS_AXI_BUS_WIDTH_BYTES          ( (PS_AXI_BUS_WIDTH_BITS)/8u )   //32
#define PS_AXI_BUS_WIDTH_WORD16         ( (PS_AXI_BUS_WIDTH_BITS) / (PS_LAX_HALF_WORD_NUM_BITS) )   //16
#define PS_AXI_BUS_WIDTH_WORD32         ( (PS_AXI_BUS_WIDTH_BITS) / (PS_LAX_WORD_NUM_BITS) )        //8
#define PS_AXI_BUS_WIDTH_BYTE_MASK      ( (PS_AXI_BUS_WIDTH_BYTES) - 1u)
#define PS_AXI_BYTE_ADDR_NOT_ALIGNED(x) ( (x) & (PS_AXI_BUS_WIDTH_BYTE_MASK) )

/**
* @brief         AXI Bus Width Defines.
* 
*/
#define AXI_BUS_BIT_WIDTH               PS_AXI_BUS_WIDTH_BITS_WIFI
#define AXI_BUS_BYTE_WIDTH              ( (AXI_BUS_BIT_WIDTH) >> 3U)
#define AXI_ALIGN_MASK                  (AXI_BUS_BYTE_WIDTH - 1U)
#define AXI_BUS_WORD32_WIDTH            ( (AXI_BUS_BIT_WIDTH) >> 5U)
#define AXI_ALIGN_32_MASK               (AXI_BUS_WORD32_WIDTH - 1U)

// ---------------------------------------------------------------------------
//! @brief Baseband Digital IQ Interface Data
//!
//! (for both Rx and Tx unless otherwise specified)
// ---------------------------------------------------------------------------
#define PS_IQ_PAIR_SZ_BITS              32                              // HP fixed: 16bit I | 16bit Q
#define PS_IQ_PAIR_SZ_BYTES             ( (PS_IQ_PAIR_SZ_BITS) / 8 )    // 32/8 = 4
#define PS_IQ_PAIR_SZ_WORD16            ( (PS_IQ_PAIR_SZ_BITS) / (PS_LAX_HALF_WORD_NUM_BITS) )  // 32/16 = 2
#define PS_IQ_PAIR_SZ_WORD32            ( (PS_IQ_PAIR_SZ_BITS) / (PS_LAX_WORD_NUM_BITS) )       // 32/32 = 1

// ---------------------------------------------------------------------------
//! @brief LAX DMEM Vector/Line Width Constants
// ---------------------------------------------------------------------------
#define PS_DMEM_LINE_SZ_WORD32          ( 2*(PS_LAX_AU_COUNT) )             // 32
#define PS_DMEM_LINE_SZ_WORD16          ( 2*(PS_DMEM_LINE_SZ_WORD32) )      // 64
#define PS_DMEM_LINE_SZ_BYTES           ( 4*(PS_DMEM_LINE_SZ_WORD32) )      // 128
#define PS_DMEM_LINE_SZ_BITS            ( 32*(PS_DMEM_LINE_SZ_WORD32) )     // 1024
#define PS_DMEM_LINE_SZ_IQ_PAIR         ( (PS_DMEM_LINE_SZ_WORD32) / (PS_IQ_PAIR_SZ_WORD32 )    // 32

// ----------------------------------------------------------------------------
//! @brief WiFi LAX DMEM AXI Slave Addresses (for DMA access)
// ----------------------------------------------------------------------------
#define PS_LAX0_DMEM_VCPU_BASE_ADDR     0x01000000
#define PS_LAX0_DMEM_IPPU_BASE_ADDR     0x01100000

#define PS_LAX1_DMEM_VCPU_BASE_ADDR     0x01400000
#define PS_LAX1_DMEM_IPPU_BASE_ADDR     0x01500000

#define PS_LAX2_DMEM_VCPU_BASE_ADDR     0x01800000
#define PS_LAX2_DMEM_IPPU_BASE_ADDR     0x01900000

#define PS_LAX3_DMEM_VCPU_BASE_ADDR     0x01C00000
#define PS_LAX3_DMEM_IPPU_BASE_ADDR     0x01D00000

// ---------------------------------------------------------------------------
//! @brief WiFi FECA RAM (FRAM)
// ---------------------------------------------------------------------------
#define PS_FRAM_BASE_ADDR               0x00300000
#define PS_FRAM_SZ_BYTES                0x00080000  // 512KB - TBD WIFI

// ---------------------------------------------------------------------------
//! @brief WiFi RX/TX DTE FIFO Addresses (Data Octet FIFOs)
//! (same address from LAX side, but reads/writes will translate to different external addresses)
// ---------------------------------------------------------------------------
#define PS_TX_DTE_FIFO_ADDR             0x00020000
#define PS_RX_DTE_FIFO_ADDR             0x00020000

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ Base Address Space Mapping
// ----------------------------------------------------------------------------
#define PS_RF_AXIQ0_BASE_ADDR           0x00100000
#define PS_RF_AXIQ1_BASE_ADDR           0x00110000
#define PS_RF_AXIQ2_BASE_ADDR           0x00120000
#define PS_RF_AXIQ3_BASE_ADDR           0x00130000
#define PS_RF_AXIQ4_BASE_ADDR           0x00140000
#define PS_RF_AXIQ5_BASE_ADDR           0x00150000

// ----------------------------------------------------------------------------
//! @brief Macro to calculate base address of AXIQ, axiq = [0,...,5]
// ----------------------------------------------------------------------------
#define GET_RF_AXIQ_BASE_ADDR(axiq)     (PS_RF_AXIQ0_BASE_ADDR + (axiq)*(0x10000) )

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ FIFO Address Offsets (from Base Space above)
// ----------------------------------------------------------------------------
#define PS_RF_AXIQ_FIFO_OFST_RX0        0x1000
#define PS_RF_AXIQ_FIFO_OFST_RX1        0x2000
#define PS_RF_AXIQ_FIFO_OFST_RX2        0x3000
#define PS_RF_AXIQ_FIFO_OFST_TX0        0x4000
#define PS_RF_AXIQ_FIFO_OFST_TX1        0x5000

// ----------------------------------------------------------------------------
//! @brief Macro to calculate offset to AXIQ FIFO address, axiq = [0,...,5]
// ----------------------------------------------------------------------------
#define GET_RF_AXIQ_FIFO_OFST_RX(axiq)  (PS_RF_AXIQ_FIFO_OFST_RX0 + (axiq)*(0x1000) )
#define GET_RF_AXIQ_FIFO_OFST_TX(axiq)  (PS_RF_AXIQ_FIFO_OFST_TX0 + (axiq)*(0x1000) )

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ0 FIFO Base Addresses
// ----------------------------------------------------------------------------
#define PS_RF_AXIQ0_FIFO_ADDR_RX0       (PS_RF_AXIQ0_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX0) // 0x00101000
#define PS_RF_AXIQ0_FIFO_ADDR_RX1       (PS_RF_AXIQ0_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX1) // 0x00102000
#define PS_RF_AXIQ0_FIFO_ADDR_RX2       (PS_RF_AXIQ0_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX2) // 0x00103000
#define PS_RF_AXIQ0_FIFO_ADDR_TX0       (PS_RF_AXIQ0_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX0) // 0x00104000
#define PS_RF_AXIQ0_FIFO_ADDR_TX1       (PS_RF_AXIQ0_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX1) // 0x00105000

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ1 FIFO Base Addresses
// ----------------------------------------------------------------------------
#define PS_RF_AXIQ1_FIFO_ADDR_RX0       (PS_RF_AXIQ1_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX0) // 0x00111000
#define PS_RF_AXIQ1_FIFO_ADDR_RX1       (PS_RF_AXIQ1_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX1) // 0x00112000
#define PS_RF_AXIQ1_FIFO_ADDR_RX2       (PS_RF_AXIQ1_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX2) // 0x00113000
#define PS_RF_AXIQ1_FIFO_ADDR_TX0       (PS_RF_AXIQ1_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX0) // 0x00114000
#define PS_RF_AXIQ1_FIFO_ADDR_TX1       (PS_RF_AXIQ1_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX1) // 0x00115000

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ2 FIFO Base Addresses
// ----------------------------------------------------------------------------
#define PS_RF_AXIQ2_FIFO_ADDR_RX0       (PS_RF_AXIQ2_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX0) // 0x00121000
#define PS_RF_AXIQ2_FIFO_ADDR_RX1       (PS_RF_AXIQ2_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX1) // 0x00122000
#define PS_RF_AXIQ2_FIFO_ADDR_RX2       (PS_RF_AXIQ2_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX2) // 0x00123000
#define PS_RF_AXIQ2_FIFO_ADDR_TX0       (PS_RF_AXIQ2_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX0) // 0x00124000
#define PS_RF_AXIQ2_FIFO_ADDR_TX1       (PS_RF_AXIQ2_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX1) // 0x00125000

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ3 FIFO Base Addresses
// ----------------------------------------------------------------------------
#define PS_RF_AXIQ3_FIFO_ADDR_RX0       (PS_RF_AXIQ3_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX0) // 0x00131000
#define PS_RF_AXIQ3_FIFO_ADDR_RX1       (PS_RF_AXIQ3_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX1) // 0x00132000
#define PS_RF_AXIQ3_FIFO_ADDR_RX2       (PS_RF_AXIQ3_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX2) // 0x00133000
#define PS_RF_AXIQ3_FIFO_ADDR_TX0       (PS_RF_AXIQ3_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX0) // 0x00134000
#define PS_RF_AXIQ3_FIFO_ADDR_TX1       (PS_RF_AXIQ3_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX1) // 0x00135000

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ4 FIFO Base Addresses
// ----------------------------------------------------------------------------
#define PS_RF_AXIQ4_FIFO_ADDR_RX0       (PS_RF_AXIQ4_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX0) // 0x00141000
#define PS_RF_AXIQ4_FIFO_ADDR_RX1       (PS_RF_AXIQ4_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX1) // 0x00142000
#define PS_RF_AXIQ4_FIFO_ADDR_RX2       (PS_RF_AXIQ4_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX2) // 0x00143000
#define PS_RF_AXIQ4_FIFO_ADDR_TX0       (PS_RF_AXIQ4_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX0) // 0x00144000
#define PS_RF_AXIQ4_FIFO_ADDR_TX1       (PS_RF_AXIQ3_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX1) // 0x00145000

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ5 FIFO Base Addresses
// ----------------------------------------------------------------------------
#define PS_RF_AXIQ5_FIFO_ADDR_RX0       (PS_RF_AXIQ5_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX0) // 0x00151000
#define PS_RF_AXIQ5_FIFO_ADDR_RX1       (PS_RF_AXIQ5_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX1) // 0x00152000
#define PS_RF_AXIQ5_FIFO_ADDR_RX2       (PS_RF_AXIQ5_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_RX2) // 0x00153000
#define PS_RF_AXIQ5_FIFO_ADDR_TX0       (PS_RF_AXIQ5_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX0) // 0x00154000
#define PS_RF_AXIQ5_FIFO_ADDR_TX1       (PS_RF_AXIQ5_BASE_ADDR + PS_RF_AXIQ_FIFO_OFST_TX1) // 0x00155000

// ----------------------------------------------------------------------------
//! @brief Macro to calculate offset to AXIQ FIFO address
//! axiq = [0,...,5]
//! rx   = [0,1,2]
//! tx   = [0,1]
// ----------------------------------------------------------------------------
#define GET_RF_AXIQ_RX_FIFO_ADDR(axiq,rx)   ( GET_RF_AXIQ_BASE_ADDR(axiq) + GET_RF_AXIQ_FIFO_OFST_TX(tx) )
#define GET_RF_AXIQ_TX_FIFO_ADDR(axiq,tx)   ( GET_RF_AXIQ_BASE_ADDR(axiq) + GET_RF_AXIQ_FIFO_OFST_TX(tx) )

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ0 DMA Channels
// ----------------------------------------------------------------------------
#define PS_DMA_CH_AXIQ0_RX0             1
#define PS_DMA_CH_AXIQ0_RX1             2
#define PS_DMA_CH_AXIQ0_RX2             3
#define PS_DMA_CH_AXIQ0_TX0             4
#define PS_DMA_CH_AXIQ0_TX1             5

#define PS_DMA_MASK_AXIQ0_RX0           ( 0x1U << (PS_DMA_CH_AXIQ0_RX0) )
#define PS_DMA_MASK_AXIQ0_RX1           ( 0x1U << (PS_DMA_CH_AXIQ0_RX1) )
#define PS_DMA_MASK_AXIQ0_RX2           ( 0x1U << (PS_DMA_CH_AXIQ0_RX2) )
#define PS_DMA_MASK_AXIQ0_TX0           ( 0x1U << (PS_DMA_CH_AXIQ0_TX0) )
#define PS_DMA_MASK_AXIQ0_TX1           ( 0x1U << (PS_DMA_CH_AXIQ0_TX1) )

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ1 DMA Channels
// ----------------------------------------------------------------------------
#define PS_DMA_CH_AXIQ1_RX0             6
#define PS_DMA_CH_AXIQ1_RX1             7
#define PS_DMA_CH_AXIQ1_RX2             8
#define PS_DMA_CH_AXIQ1_TX0             9
#define PS_DMA_CH_AXIQ1_TX1             10

#define PS_DMA_MASK_AXIQ1_RX0           ( 0x1U << (PS_DMA_CH_AXIQ1_RX0) )
#define PS_DMA_MASK_AXIQ1_RX1           ( 0x1U << (PS_DMA_CH_AXIQ1_RX1) )
#define PS_DMA_MASK_AXIQ1_RX2           ( 0x1U << (PS_DMA_CH_AXIQ1_RX2) )
#define PS_DMA_MASK_AXIQ1_TX0           ( 0x1U << (PS_DMA_CH_AXIQ1_TX0) )
#define PS_DMA_MASK_AXIQ1_TX1           ( 0x1U << (PS_DMA_CH_AXIQ1_TX1) )

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ2 DMA Channels
// ----------------------------------------------------------------------------
#define PS_DMA_CH_AXIQ2_RX0             11
#define PS_DMA_CH_AXIQ2_RX1             12
#define PS_DMA_CH_AXIQ2_RX2             13
#define PS_DMA_CH_AXIQ2_TX0             14
#define PS_DMA_CH_AXIQ2_TX1             15

#define PS_DMA_MASK_AXIQ2_RX0           ( 0x1U << (PS_DMA_CH_AXIQ2_RX0) )
#define PS_DMA_MASK_AXIQ2_RX1           ( 0x1U << (PS_DMA_CH_AXIQ2_RX1) )
#define PS_DMA_MASK_AXIQ2_RX2           ( 0x1U << (PS_DMA_CH_AXIQ2_RX2) )
#define PS_DMA_MASK_AXIQ2_TX0           ( 0x1U << (PS_DMA_CH_AXIQ2_TX0) )
#define PS_DMA_MASK_AXIQ2_TX1           ( 0x1U << (PS_DMA_CH_AXIQ2_TX1) )

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ3 DMA Channels
// ----------------------------------------------------------------------------
#define PS_DMA_CH_AXIQ3_RX0             16
#define PS_DMA_CH_AXIQ3_RX1             17
#define PS_DMA_CH_AXIQ3_RX2             18
#define PS_DMA_CH_AXIQ3_TX0             19
#define PS_DMA_CH_AXIQ3_TX1             20

#define PS_DMA_MASK_AXIQ3_RX0           ( 0x1U << (PS_DMA_CH_AXIQ3_RX0) )
#define PS_DMA_MASK_AXIQ3_RX1           ( 0x1U << (PS_DMA_CH_AXIQ3_RX1) )
#define PS_DMA_MASK_AXIQ3_RX2           ( 0x1U << (PS_DMA_CH_AXIQ3_RX2) )
#define PS_DMA_MASK_AXIQ3_TX0           ( 0x1U << (PS_DMA_CH_AXIQ3_TX0) )
#define PS_DMA_MASK_AXIQ3_TX1           ( 0x1U << (PS_DMA_CH_AXIQ3_TX1) )

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ4 DMA Channels
// ----------------------------------------------------------------------------
#define PS_DMA_CH_AXIQ4_RX0             21
#define PS_DMA_CH_AXIQ4_RX1             22
#define PS_DMA_CH_AXIQ4_RX2             23
#define PS_DMA_CH_AXIQ4_TX0             24
#define PS_DMA_CH_AXIQ4_TX1             25

#define PS_DMA_MASK_AXIQ4_RX0           ( 0x1U << (PS_DMA_CH_AXIQ4_RX0) )
#define PS_DMA_MASK_AXIQ4_RX1           ( 0x1U << (PS_DMA_CH_AXIQ4_RX1) )
#define PS_DMA_MASK_AXIQ4_RX2           ( 0x1U << (PS_DMA_CH_AXIQ4_RX2) )
#define PS_DMA_MASK_AXIQ4_TX0           ( 0x1U << (PS_DMA_CH_AXIQ4_TX0) )
#define PS_DMA_MASK_AXIQ4_TX1           ( 0x1U << (PS_DMA_CH_AXIQ4_TX1) )

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ5 DMA Channels
// ----------------------------------------------------------------------------
#define PS_DMA_CH_AXIQ5_RX0             26
#define PS_DMA_CH_AXIQ5_RX1             27
#define PS_DMA_CH_AXIQ5_RX2             28
#define PS_DMA_CH_AXIQ5_TX0             29
#define PS_DMA_CH_AXIQ5_TX1             30

#define PS_DMA_MASK_AXIQ5_RX0           ( 0x1U << (PS_DMA_CH_AXIQ5_RX0) )
#define PS_DMA_MASK_AXIQ5_RX1           ( 0x1U << (PS_DMA_CH_AXIQ5_RX1) )
#define PS_DMA_MASK_AXIQ5_RX2           ( 0x1U << (PS_DMA_CH_AXIQ5_RX2) )
#define PS_DMA_MASK_AXIQ5_TX0           ( 0x1U << (PS_DMA_CH_AXIQ5_TX0) )
#define PS_DMA_MASK_AXIQ5_TX1           ( 0x1U << (PS_DMA_CH_AXIQ5_TX1) )

// ----------------------------------------------------------------------------
//! @brief Macro to get the correct RX DMA Channel for given AXIQ and RX Ch
//! axiq = [0,...,5]
//! rx   = [0,1,2]
// ----------------------------------------------------------------------------
#define GET_RF_AXIQ_RX_DMA_CH(axiq,rx)  (PS_DMA_CH_AXIQ0_RX0 + (axiq)*5 + rx)

// ----------------------------------------------------------------------------
//! @brief Macro to get the correct TX DMA Channel for given AXIQ and TX Ch
//! axiq = [0,...,5]
//! tx   = [0,1]
// ----------------------------------------------------------------------------
#define GET_RF_AXIQ_TX_DMA_CH(axiq,rx)  (PS_DMA_CH_AXIQ0_TX0 + (axiq)*5 + tx)

// ----------------------------------------------------------------------------
//! @brief WiFi Inter-LAX-Communication GPout
// ----------------------------------------------------------------------------
#define PS_GPOUT_IPC_TO_LAX0            0
#define PS_GPOUT_IPC_TO_LAX1            1
#define PS_GPOUT_IPC_TO_LAX2            2
#define PS_GPOUT_IPC_TO_LAX3            3

// ----------------------------------------------------------------------------
//! @brief WiFi Inter-LAX-Communication GPin
// ----------------------------------------------------------------------------
#define PS_GPIN_IPC_FROM_LAX0           0
#define PS_GPIN_IPC_FROM_LAX1           1
#define PS_GPIN_IPC_FROM_LAX2           2
#define PS_GPIN_IPC_FROM_LAX3           3

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ Config GPout
// ----------------------------------------------------------------------------
#define PS_GPOUT_RF_AXIQ0_CTL           4
#define PS_GPOUT_RF_AXIQ1_CTL           5
#define PS_GPOUT_RF_AXIQ2_CTL           6
#define PS_GPOUT_RF_AXIQ3_CTL           7
#define PS_GPOUT_RF_AXIQ4_CTL           8
#define PS_GPOUT_RF_AXIQ5_CTL           9

// ----------------------------------------------------------------------------
//! @brief WiFi RF AXIQ Status GPin
// ----------------------------------------------------------------------------
#define PS_GPIN_RF_AXIQ0_STS            4
#define PS_GPIN_RF_AXIQ1_STS            5
#define PS_GPIN_RF_AXIQ2_STS            6
#define PS_GPIN_RF_AXIQ3_STS            7
#define PS_GPIN_RF_AXIQ4_STS            8
#define PS_GPIN_RF_AXIQ5_STS            9

// ----------------------------------------------------------------------------
//! @brief Macro to get the GPIO # connecting to given AXIQ module
//! axiq = [0,...,5]
// ----------------------------------------------------------------------------
#define GET_RF_AXIQ_GPIO_NUM(axiq)      (axiq+4)

// ----------------------------------------------------------------------------
//! @brief WiFi PHY-to-AIOP Host Communication GPout
// ----------------------------------------------------------------------------
#define PS_GPOUT_AIOP_HOST0             10
#define PS_GPOUT_AIOP_HOST1             11
#define PS_GPOUT_AIOP_HOST2             12
#define PS_GPOUT_AIOP_HOST3             13
#define PS_GPOUT_AIOP_HOST4             14
#define PS_GPOUT_AIOP_HOST5             15
#define PS_GPOUT_AIOP_HOST6             16
#define PS_GPOUT_AIOP_HOST7             17

// ----------------------------------------------------------------------------
//! @brief WiFi Host AIOP-to-PHY Communication LAX GPin
// ----------------------------------------------------------------------------
#define PS_GPIN_AIOP_HOST0              10
#define PS_GPIN_AIOP_HOST1              11
#define PS_GPIN_AIOP_HOST2              12
#define PS_GPIN_AIOP_HOST3              13
#define PS_GPIN_AIOP_HOST4              14
#define PS_GPIN_AIOP_HOST5              15
#define PS_GPIN_AIOP_HOST6              16
#define PS_GPIN_AIOP_HOST7              17
#define PS_GPIN_AIOP_HOST8              18 // only bits[7:0] corresponding to GPO[17:10]

// ----------------------------------------------------------------------------
//! @brief WiFi GPOUT for AVI WatchDog Counter (bits 31:16)
// ----------------------------------------------------------------------------
#define PS_GPOUT_AVI_WDOG_CNT           (18U)

// ----------------------------------------------------------------------------
//! @brief WiFi LAX Debug GPout (bits 3:0 are for debug, the rest are AXI sideband)
// ----------------------------------------------------------------------------
#define PS_GPOUT_LAX_DEBUG              18
#define PS_GPOUT_LAX_DEBUG_MASK         0x00000003
#define PS_GPOUT_LAX_DEBUG_BIT_0        0
#define PS_GPOUT_LAX_DEBUG_BIT_1        1

#define PS_GPOUT_LAX_SHARED_DEBUG_MASK  0x000003FC
#define PS_GPOUT_LAX_SHARED_DEBUG_BIT_0 0
#define PS_GPOUT_LAX_DEBUG_BIT_1        1


#define PS_GPOUT_LAX_DEBUG_BIT_2        2
#define PS_GPOUT_LAX_DEBUG_BIT_3        3

// ---------------------------------------------------------------------------
//! @brief WiFi LAX External GO Bits
// ---------------------------------------------------------------------------
#define PS_LAX_EXT_GO_MASK              0x0003FC3F

#define PS_EXT_GO_B_IPC_FROM_LAX0       0
#define PS_EXT_GO_B_IPC_FROM_LAX1       1
#define PS_EXT_GO_B_IPC_FROM_LAX2       2
#define PS_EXT_GO_B_IPC_FROM_LAX3       3

#define PS_EXT_GO_B_PHY_TIMER0          4
#define PS_EXT_GO_B_PHY_TIMER1          5

#define PS_EXT_GO_B_AIOP_HOST0          10
#define PS_EXT_GO_B_AIOP_HOST1          11
#define PS_EXT_GO_B_AIOP_HOST2          12
#define PS_EXT_GO_B_AIOP_HOST3          13
#define PS_EXT_GO_B_AIOP_HOST4          14
#define PS_EXT_GO_B_AIOP_HOST5          15
#define PS_EXT_GO_B_AIOP_HOST6          16
#define PS_EXT_GO_B_AIOP_HOST7          17

#define PS_EXT_GO_MASK_IPC_FROM_LAX0    ( 0x1U << (PS_EXT_GO_B_IPC_FROM_LAX0) )
#define PS_EXT_GO_MASK_IPC_FROM_LAX1    ( 0x1U << (PS_EXT_GO_B_IPC_FROM_LAX1) )
#define PS_EXT_GO_MASK_IPC_FROM_LAX2    ( 0x1U << (PS_EXT_GO_B_IPC_FROM_LAX2) )
#define PS_EXT_GO_MASK_IPC_FROM_LAX3    ( 0x1U << (PS_EXT_GO_B_IPC_FROM_LAX3) )

#define PS_EXT_GO_MASK_PHY_TIMER0       ( 0x1U << (PS_EXT_GO_B_PHY_TIMER0) )
#define PS_EXT_GO_MASK_PHY_TIMER1       ( 0x1U << (PS_EXT_GO_B_PHY_TIMER1) )

#define PS_EXT_GO_MASK_AIOP_HOST0       ( 0x1U << (PS_EXT_GO_B_AIOP_HOST0) )
#define PS_EXT_GO_MASK_AIOP_HOST1       ( 0x1U << (PS_EXT_GO_B_AIOP_HOST1) )
#define PS_EXT_GO_MASK_AIOP_HOST2       ( 0x1U << (PS_EXT_GO_B_AIOP_HOST2) )
#define PS_EXT_GO_MASK_AIOP_HOST3       ( 0x1U << (PS_EXT_GO_B_AIOP_HOST3) )
#define PS_EXT_GO_MASK_AIOP_HOST4       ( 0x1U << (PS_EXT_GO_B_AIOP_HOST4) )
#define PS_EXT_GO_MASK_AIOP_HOST5       ( 0x1U << (PS_EXT_GO_B_AIOP_HOST5) )
#define PS_EXT_GO_MASK_AIOP_HOST6       ( 0x1U << (PS_EXT_GO_B_AIOP_HOST6) )
#define PS_EXT_GO_MASK_AIOP_HOST7       ( 0x1U << (PS_EXT_GO_B_AIOP_HOST7) )

// -----------------------------------------------------------------------------
//! @} GROUP_PLATFORM_WIFI
// -----------------------------------------------------------------------------


/**
* @brief         CTE/SPT to LAX connections. (forced addition for platform wifi)
*
*/
#define SPT_LAX_EXT_GO_NUM              4U
#define CTE_LAX_EXT_GO_NUM              4U
#define SPT_LAX_EXT_GO_BIT_OFFSET       0U
#define CTE_LAX_EXT_GO_BIT_OFFSET       7U

/**
* @brief          The number of LAX cores.
* @details        The LAX subsystem may be implemented with one or more LAX cores.
*                 The cores' IDs start from 0.
*/
#define RSDK_LAX_CORES_NUM 2U

#endif /* PLATFORM_WIFI_H */
//! @}

