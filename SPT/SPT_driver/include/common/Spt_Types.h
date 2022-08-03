/*
* Copyright 2022 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef SPT_TYPES_H
#define SPT_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Spt_Cfg.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/** @addtogroup spt_driver_api_const
* @{
*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/** @addtogroup spt_driver_api_const
* @{
*/

/**
* @brief    Maximum number of input parameters to an SPT kernel.
* @details  It is used to define the size of Spt_DriverContextType::kernelParList array.
* */
#define SPT_MAX_N_PAR               (10u)

/**
* @brief    Kernel code watermarking pattern
* @details  It must be used by all SPT kernels which are launched by RSDK SPT Driver, by placing it
*           in a mandatory first instruction: "set #SPT_KERNEL_WATERMARK, WR_0". This helps
*           protect against passing wrong code memory pointers to the SPT command sequencer.
* */
#define SPT_KERNEL_WATERMARK        (0x00005253444BULL)

/**
* @brief    SPT command maximum result size.
* @details  It is used to define the size of the Spt_DriverCmdResType structure.
* */
#define SPT_CMD_RESULT_SIZE         (8u)

/**
* @brief    Alignment constraint for start address of the SPT code.
* */
#define SPT_CODE_ADDR_ALIGN_BYTES   (16u)

/**
* @brief    Alignment constraint for start address of the SPT data buffers.
* */
#define SPT_DATA_ADDR_ALIGN_BYTES   (8u)

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/** @addtogroup spt_driver_api_data_type
* @{
* 
* @anchor spt_persist_data_section 
* @par Persistent data ELF section
* Internals of SPT driver make use of persistent data. All persistent data of SPT driver is grouped under 
* .RSDK_SPT_DRV_MEM_PER section. It is application-level linker file responsibility to place this section 
* in a proper memory location, with read/write priviledges.
* 
*/

/**
* @brief        Specifies the type of parameter to be passed to the SPT kernel.
* @details      Enum fields are used to initialize Spt_DriverContextType::kernelParList.
*
* User-space address arguments must be pre-processed by the SPT Driver before passing them to the SPT hardware,
* hence the need for differentiation between 'address' and 'data' argument types.
*
* \attention The SPT hardware instructions cannot handle full-width 32bit addresses, instead they work
* with an implicit memory base address (which is #SPT_CUBE_BASE_ADDR or #SPT_OTHER_BASE_ADDR)
* and an address offset on maximum 23 bits.
*
* See the @ref spt_call_conv for details.
*/
typedef enum
{
    SPT_PARAM_TYPE_NOTINIT = 0U, /**< Parameter is not initialized  */
    SPT_PARAM_TYPE_ADDR,         /**< Parameter represents a memory address.
                                          NOTE that all system memory data addresses which are passed to the SPT
                                          must be aligned to an #SPT_DATE_ADDR_ALIGN_BYTES boundary*/
    SPT_PARAM_TYPE_VALUE,        /**< Parameter represents a scalar value. The SPT driver splits this 32-bit value,
                                          then copies the 8 most significant bits into the imaginary part of a Work
                                          Register and the remaining 24 bits into the real part of the same WR.  */
    SPT_PARAM_TYPE_LAST          /**< Signals the end of the SPT parameter list. The last member in
                                          Spt_DriverContextType::kernelParList must have this type. */
} Spt_ParamType;

/**
* @brief        Datatype coding the operating modes for the SPT Driver.
* @details      Spt_Run() can execute either in blocking (polling) or non-blocking (interrupt) modes.
*/
typedef enum
{
    SPT_OP_MODE_BLOCK = 0U, /**< Blocking call (polling on the SPT_CS_STATUS0[PS_STOP] bit) */
    SPT_OP_MODE_NONBLOCK    /**< Non-blocking call (triggering an interrupt on SPT_CS_STATUS0[PS_STOP] bit) */
} Spt_DriverOpModeType;

/**
* @brief        Defines the command codes for Spt_Command().
*
*/
typedef enum
{
    SPT_CMD_NONE = 0U,          /**< dummy value to avoid empty enum declaration */
    SPT_CMD_MEM_ERR_INJECT_EN,  /**< Enable injection of parity errors in the SPT OPRAM and TRAM.
                                          Parity bits will be altered on subsequent write accesses to memory.
                                          Then, parity errors will be generated on memory read-back */
    SPT_CMD_MEM_ERR_INJECT_DIS, /**< Disable injection of parity errors in the SPT OPRAM and TRAM. */
    SPT_CMD_TRIGGER_SW_EVENT,   /**< Trigger and event on the SPT software event line (SW_EVTREG register),
                                          to be used by the SPT WAIT instruction */
#if(SPT_DSP_ENABLE == STD_ON)
    SPT_CMD_GEN_DSP_CMD_CRC,    /**< Compute an 8-bit CRC of the DSP command, accounting for length and endiannes
                                          to match with the verification on BBE32 side. The command parameter must be a
                                          pointer of type Spt_DspCmdType. The result is passed in Spt_DspCmdType::crc field.
                                          See the \ref dsp_call_conv "DSP Calling Convention" */
    SPT_CMD_BBE32_REBOOT
#endif
} Spt_DriverCommandIdType;

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
 *  @brief      Necessary typedef for uintptr, as AUTOSAR doesn't offer one.
 */
typedef uintptr_t   uintptr;

/** @brief  Definition of callback function type to be called by the SPT interrupt handlers.
* @details  This callback is offered to the user for additional processing during the SPT interrupts.
*
* @param[in]    isrStatus - the callback receives a status field from the SPT interrupt handler,
*                           that can signal either successful execution of the SPT or an error.
* @param[in]    errInfo - detailed error information associated to each Std_ReturnType code
*                         (e.g. content of the corresponding error status register).
* @return       nothing
*
* */
typedef void (*Spt_IsrCbType)(Std_ReturnType isrStatus, uint32 errInfo);

#if(SPT_DSP_ENABLE == STD_ON)
/**
* @brief        This structure packs the information needed to call a function on the BBE32 .
*/
typedef struct {
    uint8   id;     /**< Command ID returned from RSDK_DSP_GET_FUNC_ID(function_name)*/
    uint32  arg;    /**< Command argument. Can be either an immediate value or a 32-bit pointer to a command-specific
                         data structure. */
    uint8   crc;    /**< CRC checksum of the id and arg fields, used to verify command integrity on BBE32 side*/
} Spt_DspCmdType;
#endif

/**
* @brief        Datatype describing the parameters to be passed to the SPT kernel. See @ref spt_call_conv for details.
* @details      Used to initialize Spt_DriverContextType::kernelParList with information for the SPT kernels.
*/
typedef struct
{
    Spt_ParamType   paramType;  /**< Indicates the intended usage of paramValue as an immediate value or a memory address*/
    uintptr         paramValue; /**< Can contain either an immediate integer value or a memory address*/
} Spt_DrvParamType;

/**
* @brief        Contains runtime parameters for the SPT Driver.
* @details      This structure is only used by Spt_Run(). Must be allocated and initialized by the caller.
*               Its content is not modified by the SPT Driver.
*               Some fields are used for implementation of the SPT calling convention (see @ref spt_call_conv for details).
*               The intended direction of data flow is specified in the description of each field.
*/
typedef struct
{
    Spt_DriverOpModeType    opMode;   /**< <b>[in]</b> Operating mode for Spt_Run() .*/
    Spt_IsrCbType           ecsIsrCb; /**< <b>[in]</b> Callback function to be called from the SPT ECS interrupt handler.
                                                       If no callback is required then it can be initialized to NULL. */
    Spt_IsrCbType           evtIsrCb; /**< <b>[in]</b> Callback function to be called from the SPT EVT interrupt handler.
                                                       If no callback is required then it can be initialized to NULL. */
    uintptr                 kernelCodeAddr; /**< <b>[in]</b> Starting address of the SPT executable code, must be aligned
                                                             to an #SPT_CODE_ADDR_ALIGN_BYTES boundary*/
    Spt_DrvParamType        kernelParList[SPT_MAX_N_PAR]; /**< <b>[in]</b> List of parameters to be passed to the SPT kernel.*/
    volatile sint32 *       kernelRetPar; /**< <b>[out]</b> Return parameter from the SPT kernel. The SPT driver creates this
                                         32-bit value by concatenating8 bits from WR0_IM with 24 bits from WR0_RE.
                                         This pointer must be initialized with a global address which is visible in the
                                         contexts of Spt_Run() and SPT interrupts. If no return parameter is expected
                                         from the SPT kernel, then it can be initialized to NULL.*/
    uint8     checkKernelWatermark; /**< Check if the watermark instruction exists in the SPT kernel at kernelCodeAddr
                                         address, to guard against passing invalid pointers to the SPT.
                                         Valid values: 0=disable, 1=enable.
                                         If this option is enabled in the SPT Driver, then all SPT kernels must start
                                         with the predefined instruction "set #SPT_KERNEL_WATERMARK, WR_0",
                                         otherwise the driver will not launch them. */
} Spt_DriverContextType;

/**
* @brief          Init parameters which are specific to the hardware platform.
*/
typedef struct
{
#if(SPT_DSP_ENABLE == STD_ON)
    uint8                   dspEn;      /**< Enable the BBE32 DSP to boot-up and start running the "DSP Dispatcher":
                                             0=disable, "non-0"=enable. */
    Spt_IsrCbType           dspIsrCb;   /**< <b>[in]</b> Callback function to be called from the SPT DSP interrupt handler.
                                             Used to pass BBE32 status & error information to the user application.
                                             If no callback is required then it can be initialized to NULL. */
    void (*dspBootloaderCb)(void);  /**< <b>[in]</b> External function called for loading the DSP boot image into memory.
                                         This functions is called by the driver in the middle of the boot procedure,
                                         because the DSP internal memory (IRAM, DRAM) can only be accessed after releasing
                                         the DSP from reset. This callback is required when dspEn != 0. */
#endif
} Spt_DriverInitPlatSpecificType;

/**
* @brief          Contains initialization parameters for the SPT Driver.
* @details        This structure is only used by Spt_Setup(). Must be allocated and initialized by the caller.
*                 Its content is not modified by the SPT Driver.
*/
typedef struct
{
    Spt_DriverInitPlatSpecificType hwPlatSpec; /**< Init parameters which are specific to a particular hardware
                                                    platform. */
} Spt_DriverInitType;

/**
* @brief        Typedef for returning information from Spt_Command(); to be used as argument to the function
* @details      This structure is only used by Spt_Command(). Must be allocated by the caller.
*               Its content is modified by the SPT Driver.
*/
typedef struct
{
    uint8 bytes[SPT_CMD_RESULT_SIZE];
} Spt_DriverCmdResType;

typedef uintptr Spt_DriverCommandParamType;

/**
* @brief        Structure containing the ID of the command to be served and additional parameters
*               that may be needed for Spt_Command().
*/
typedef struct
{
    Spt_DriverCommandIdType     cmdId;    /**< Command ID */
    Spt_DriverCommandParamType  cmdParam; /**< Additional command parameter*/
} Spt_DriverCommandType;

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* SPT_TYPES_H */
