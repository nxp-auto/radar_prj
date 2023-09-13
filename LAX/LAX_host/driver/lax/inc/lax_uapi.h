/*
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef LAX_UAPI_H
#define LAX_UAPI_H

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/
#include "rsdk_lax_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/
#define RSDK_LAX_OAL_COMM_SERV   "laxoalcomm"
#define LAX_MAGIC_NUM 'V'

#define LAX_FLAG_REPORT_DMA_COMPLETE    (1U<<3U)

#define LAX_MAX_ELD_FILENAME    (256)

/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/

/*=================================================================================================
*                                             ENUMS
=================================================================================================*/
enum rsdkLaxUapiFunctions{
    RSDK_LAX_UAPI_DMA_LAX0,
    RSDK_LAX_UAPI_DMA_LAX1,
    RSDK_LAX_UAPI_REGISTER_EVENTS,
    RSDK_LAX_UAPI_DEREGISTER_EVENTS,
    RSDK_LAX_UAPI_TRIGGER_EVENT,
};

enum laxDmaReqType
{
    LAX_DMA_REQ_ELD,
    LAX_DMA_REQ_CMD
};

/*=================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
=================================================================================================*/
struct laxDmaReq
{
    union
    {
        uint32_t    control;
        struct
        {
            uint8_t    id;
            uint8_t    flags;
            uint8_t    rsvd;
            uint8_t    type;
        };
    };
    uint32_t    dmemAddr;
    uint64_t    axiAddr;
    uint32_t    byteCnt;
    uint32_t    xfrCtrl;
};

struct laxVersions
{
    uint32_t    laxHwVersion;
    uint32_t    ippuHwVersion;
    uint32_t    laxSwVersion;
    uint32_t    ippuSwVersion;
};

struct laxHardware
{
    uint32_t    param0;
    uint32_t    param1;
    uint32_t    param2;
    uint32_t    dmaChannels;
    uint32_t    gpOutRegs;
    uint32_t    gpInRegs;
    uint32_t    dmemBytes;
    uint32_t    ippuBytes;
    uint32_t    arithmeticUnits;
};

struct laxReg
{
    uint32_t    reg;
    uint32_t    val;
};

struct laxMb32
{
    union
    {
        uint32_t      control;
        struct
        {
            uint8_t     id;
            uint8_t     flags;
            uint8_t     rsvd0;
            uint8_t     rsvd1;
        };
    };
    uint32_t        data;
};

struct laxMb64
{
    union
    {
        uint32_t      control;
        struct
        {
            uint8_t     id;
            uint8_t     flags;
            uint8_t     rsvd0;
            uint8_t     rsvd1;
        };
    };
    uint32_t        dataMsb;
    uint32_t        dataLsb;
};

/*=================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
=================================================================================================*/

/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/

#ifdef __cplusplus
}
#endif


#endif /* LAX_UAPI_H */
