/*
 * Copyright 2016-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLATFORM_RRM_H_
#define PLATFORM_RRM_H_
/**
* @file           platform_rrm.h
* @implements     RRM platform definitions 
*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/**
* @brief         LAX Number of AUs = 64
*
*/
#define PS_LAX_AU_COUNT                 LAX_AU_COUNT // CW Build Pre-Processor Macro = 64


/**
* @brief         LAX Supported Word Widths
*
*/
#define PS_LAX_HALF_WORD_NUM_BITS       16U
#define PS_LAX_WORD_NUM_BITS            32U

/**
* @brief         RRM AXI Bus Width Alignment Constants
*
*/

#define PS_AXI_BUS_WIDTH_BITS_RRM       128U
#define PS_AXI_BUS_WIDTH_BITS           (PS_AXI_BUS_WIDTH_BITS_RRM)     //128
#define PS_AXI_BUS_WIDTH_BYTES          ( (PS_AXI_BUS_WIDTH_BITS)/8U )   //16
#define PS_AXI_BUS_WIDTH_WORD16         ( (PS_AXI_BUS_WIDTH_BITS) / (PS_LAX_HALF_WORD_NUM_BITS) )   //8
#define PS_AXI_BUS_WIDTH_WORD32         ( (PS_AXI_BUS_WIDTH_BITS) / (PS_LAX_WORD_NUM_BITS) )        //4
#define PS_AXI_BUS_WIDTH_BYTE_MASK      ( (PS_AXI_BUS_WIDTH_BYTES) - 1U)
#define PS_AXI_BYTE_ADDR_NOT_ALIGNED(x) ( (x) & (PS_AXI_BUS_WIDTH_BYTE_MASK) )

/**
* @brief         AXI Bus Width Defines.
*
*/
#define AXI_BUS_BIT_WIDTH               PS_AXI_BUS_WIDTH_BITS_RRM
#define AXI_BUS_BYTE_WIDTH              ( (AXI_BUS_BIT_WIDTH) >> 3U)
#define AXI_ALIGN_MASK                  (AXI_BUS_BYTE_WIDTH - 1)
#define AXI_BUS_WORD32_WIDTH            ( (AXI_BUS_BIT_WIDTH) >> 5U)
#define AXI_ALIGN_32_MASK               (AXI_BUS_WORD32_WIDTH - 1)

/**
* @brief         CTE/SPT/LAX to LAX connections.
*
*/
#define SPT_LAX_EXT_GO_NUM              4U
#define CTE_LAX_EXT_GO_NUM              4U
#define LAX_LAX_EXT_GO_NUM              1U
#define SPT_LAX_EXT_GO_BIT_OFFSET       0U
#define CTE_LAX_EXT_GO_BIT_OFFSET       7U
#define LAX_LAX_EXT_GO_BIT_OFFSET       5U


/**
* @brief          The number of LAX cores.
* @details        The LAX subsystem may be implemented with one or more LAX cores.
*                 The cores' IDs start from 0.
*/
#define RSDK_LAX_CORES_NUM 2U

#endif /* PLATFORM_RRM_H_ */

