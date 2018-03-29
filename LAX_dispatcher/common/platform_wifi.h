/*
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
//! @file           platform_wifi.h
//! @brief          WiFi Platform Specific Parameters
//!
//! WiFi VSPA/Platform Specific data widths, hard-wired connectivity, static
//! DMA channel mapping, etc.

#ifndef PLATFORM_WIFI_H
#define PLATFORM_WIFI_H

// -----------------------------------------------------------------------------
//! defgroup       GROUP_PLATFORM_WIFI
//!
//! WiFi VSPA/Platform Specific data widths, hard-wired connectivity, static
//! DMA channel mapping, etc.
//!
//! @{
// -----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//! @brief VSPA Number of AUs = 16
// ---------------------------------------------------------------------------
#define PS_VSPA_AU_COUNT                VSPA_AU_COUNT // CW Build Pre-Processor Macro = 16

// ----------------------------------------------------------------------------
//! @brief WiFi AXI Bus Width Alignment Constants
// ----------------------------------------------------------------------------
#define PS_AXI_BUS_WIDTH_BITS_WIFI      256U
#define PS_AXI_BUS_WIDTH_BITS           (PS_AXI_BUS_WIDTH_BITS_WIFI) //256
#define PS_AXI_BUS_WIDTH_BYTES          ( (PS_AXI_BUS_WIDTH_BITS)/8 ) //32
#define PS_AXI_BUS_WIDTH_WORD16         ( (PS_AXI_BUS_WIDTH_BITS) / (PS_VSPA_HALF_WORD_NUM_BITS) ) //16
#define PS_AXI_BUS_WIDTH_WORD32         ( (PS_AXI_BUS_WIDTH_BITS) / (PS_VSPA_WORD_NUM_BITS) ) //8
#define PS_AXI_BUS_WIDTH_BYTE_MASK      ( (PS_AXI_BUS_WIDTH_BYTES) - 1)
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


// -----------------------------------------------------------------------------
//! @} GROUP_PLATFORM_WIFI
// -----------------------------------------------------------------------------


/**
* @brief         CTE/SPT to LAX connections. (forced addition for platform wifi)
*
*/
#define SPT_LAX_EXT_GO_NUM           4U
#define CTE_LAX_EXT_GO_NUM           4U
#define SPT_LAX_EXT_GO_BIT_OFFSET           0U
#define CTE_LAX_EXT_GO_BIT_OFFSET           7U

#endif /* PLATFORM_WIFI_H */
//! @}

