/*
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef VSPA_UAPI_H
#define VSPA_UAPI_H

#include "rsdk_lax_common.h"


#define RSDK_LAX_OAL_COMM_SERV   "laxoalcomm"
#define VSPA_MAGIC_NUM 'V'

/* Maximum allowed size for any individual command */
#define CMD_MAX_SZ_BYTES	(128)
#define RSDK_LAX_UAPI_VALID 0x600DC0DE
enum rsdkLaxUapiFunctions{
    RSDK_LAX_UAPI_GET_HW_CFG,
    RSDK_LAX_UAPI_REQ_POWER,
    RSDK_LAX_UAPI_STARTUP,
    RSDK_LAX_UAPI_CMD_WRITE,
    RSDK_LAX_UAPI_EVENT_READ,
    RSDK_LAX_UAPI_DMA,
    RSDK_LAX_UAPI_SET_RECOVERY,

};

enum vspa_state 
{
    VSPA_STATE_UNKNOWN = 0,
    VSPA_STATE_POWER_DOWN,
    VSPA_STATE_STARTUP_ERR,
    VSPA_STATE_UNPROGRAMMED_IDLE,
    VSPA_STATE_UNPROGRAMMED_BUSY,
    VSPA_STATE_LOADING,
    VSPA_STATE_RUNNING_IDLE,
    VSPA_STATE_RUNNING_BUSY,
    VSPA_STATE_RECOVERY
};

enum vspa_power 
{
    VSPA_POWER_DOWN = 0,
    VSPA_POWER_UP,
    VSPA_POWER_CYCLE
};

enum vspa_event_type 
{
    VSPA_EVENT_NONE = 0,
    VSPA_EVENT_DMA,
    VSPA_EVENT_CMD,
    VSPA_EVENT_REPLY,
    VSPA_EVENT_SPM,
    VSPA_EVENT_MB64_IN,
    VSPA_EVENT_MB32_IN,
    VSPA_EVENT_MB64_OUT,
    VSPA_EVENT_MB32_OUT,
    VSPA_EVENT_ERROR
};

#define VSPA_FLAG_EXPECT_CMD_REPLY	(1<<0)
#define VSPA_FLAG_REPORT_CMD_REPLY	(1<<1)
#define VSPA_FLAG_REPORT_CMD_CONSUMED	(1<<2)
#define VSPA_FLAG_REPORT_DMA_COMPLETE	(1<<3)
#define VSPA_FLAG_REPORT_MB_COMPLETE	(1<<4)


#define VSPA_MSG(x)			(0x10 << x)

#define VSPA_MSG_PEEK       VSPA_MSG(VSPA_EVENT_NONE)
#define VSPA_MSG_DMA        VSPA_MSG(VSPA_EVENT_DMA)
#define VSPA_MSG_CMD        VSPA_MSG(VSPA_EVENT_CMD)
#define VSPA_MSG_REPLY      VSPA_MSG(VSPA_EVENT_REPLY)
#define VSPA_MSG_SPM        VSPA_MSG(VSPA_EVENT_SPM)
#define VSPA_MSG_MB64_IN    VSPA_MSG(VSPA_EVENT_MB64_IN)
#define VSPA_MSG_MB32_IN    VSPA_MSG(VSPA_EVENT_MB32_IN)
#define VSPA_MSG_MB64_OUT   VSPA_MSG(VSPA_EVENT_MB64_OUT)
#define VSPA_MSG_MB32_OUT   VSPA_MSG(VSPA_EVENT_MB32_OUT)

#define VSPA_MSG_ERROR			VSPA_MSG(VSPA_EVENT_ERROR)
#define VSPA_MSG_ALL			(0xFFF0)
#define VSPA_MSG_ALL_EVENTS		(0xFFE0)

#define VSPA_ERR_DMA_CFGERR		(0x10)
#define VSPA_ERR_DMA_XFRERR		(0x11)
#define VSPA_ERR_WATCHDOG		(0x12)

struct vspa_event 
{
    union 
    {
        uint32_t	control;
        struct 
        {
            uint8_t	id;
            uint8_t	err;
            uint16_t	type;
        };
    };
    uint16_t	pkt_size;
    uint16_t	buf_size;
    uint32_t	data[0];
};

struct vspa_dma_req 
{
    union 
    {
        uint32_t	control;
        struct 
        {
            uint8_t	id;
            uint8_t	flags;
            uint8_t	rsvd;
            uint8_t	type;
        };
    };
    uint32_t	dmem_addr;
    dma_addr_t	axi_addr;
    uint32_t	byte_cnt;
    uint32_t	xfr_ctrl;
};

#define VSPA_MAX_ELD_FILENAME (256)
struct vspa_startup 
{
    uint32_t	cmd_buf_size;
    uint32_t	cmd_buf_addr;
    uint32_t	spm_buf_size;
    uint32_t	spm_buf_addr;
    uint8_t		cmd_dma_chan;
    char		filename[VSPA_MAX_ELD_FILENAME];
        ///< The reply buffers physical address
    dma_addr_t      replyPhys[RSDK_LAX_MAX_CMDS_NUM + RSDK_LAX_CTE_CMD_MAX_NUM + RSDK_LAX_SPT_CMD_MAX_NUM];
    unsigned int flags;
};

struct vspa_versions 
{
    uint32_t	vspa_hw_version;
    uint32_t	ippu_hw_version;
    uint32_t	vspa_sw_version;
    uint32_t	ippu_sw_version;
};

struct vspa_hardware 
{
    uint32_t	param0;
    uint32_t	param1;
    uint32_t	param2;
    uint32_t	axi_data_width;
    uint32_t	dma_channels;
    uint32_t	gp_out_regs;
    uint32_t	gp_in_regs;
    uint32_t	dmem_bytes;
    uint32_t	ippu_bytes;
    uint32_t	arithmetic_units;
};

struct vspa_reg 
{
    uint32_t	reg;
    uint32_t	val;
};

struct vspa_mb32 
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

struct vspa_mb64 
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
    uint32_t        data_msb;
    uint32_t        data_lsb;
};

struct vspa_cmd 
{
    char      *buf;
    size_t          len;
    dma_addr_t      cmd_buf_phy;
    char* 	cmd_buf_wrp;
    dma_addr_t 	cmd_buf_wrp_phys;
};

struct vspa_event_read 
{
    uint32_t	event_mask;
    int		timeout;
    size_t		buf_len;
    struct vspa_event *buf_ptr;
};


/* get VSPA Hardware configuration */
#define VSPA_IOC_GET_HW_CFG	_IOR(VSPA_MAGIC_NUM, 0, struct vspa_hardware)

/* VSPA operational state */
#define VSPA_IOC_GET_STATE	_IOR(VSPA_MAGIC_NUM, 1, int)

/* Read register */
#define VSPA_IOC_REG_READ	_IOR(VSPA_MAGIC_NUM, 2, struct vspa_reg)

/* Write register */
#define VSPA_IOC_REG_WRITE	_IOW(VSPA_MAGIC_NUM, 3, struct vspa_reg)

/* VSPA HW and SW versions */
#define VSPA_IOC_GET_VERSIONS	_IOR(VSPA_MAGIC_NUM, 4, struct vspa_versions)

/* Power Management request for vspa */
#define VSPA_IOC_REQ_POWER	_IO(VSPA_MAGIC_NUM, 5)
/* DMA transaction */
#define VSPA_IOC_DMA		_IOW(VSPA_MAGIC_NUM, 6, struct vspa_dma_req)

/* Startup VSPA core */
#define VSPA_IOC_STARTUP	_IOW(VSPA_MAGIC_NUM, 7, struct vspa_startup)

/* Write Command buffer transactions */
#define VSPA_IOC_CMD_WRITE	_IOW(VSPA_MAGIC_NUM, 8, struct vspa_cmd)

/* Write Mailbox transactions */
//#define VSPA_IOC_MB32_WRITE	_IOW(VSPA_MAGIC_NUM, 8, struct vspa_mb32)
#define VSPA_IOC_MB64_WRITE	_IOW(VSPA_MAGIC_NUM, 9, struct vspa_mb64)

/* Set Watchdog interval */
#define VSPA_IOC_WATCHDOG_INT	_IO(VSPA_MAGIC_NUM, 10)
#define VSPA_WATCHDOG_INTERVAL_DEFAULT	 (10000000)
#define VSPA_WATCHDOG_INTERVAL_MIN	  (1000000)
#define VSPA_WATCHDOG_INTERVAL_MAX	(600000000)

/* Set the event mask used for poll checks */
#define VSPA_IOC_SET_POLL_MASK	_IO(VSPA_MAGIC_NUM, 11)

/* Retrieve the next matching event */
#define VSPA_IOC_EVENT_READ	_IOW(VSPA_MAGIC_NUM, 12, struct vspa_event_read)

/* Driver debug value */
#define VSPA_IOC_GET_DEBUG	_IOR(VSPA_MAGIC_NUM, 13, int)
#define VSPA_IOC_SET_DEBUG	_IO(VSPA_MAGIC_NUM, 14)

/* Set driver to recovery state */
#define VSPA_IOC_SET_RECOVERY	_IO(VSPA_MAGIC_NUM, 15)

#define VSPA_IOC_MAX (15)

#endif /* VSPA_UAPI_H */
