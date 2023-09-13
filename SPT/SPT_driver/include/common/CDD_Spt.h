/*
* Copyright 2022-2023 NXP
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef CDD_SPT_H
#define CDD_SPT_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Spt_Types.h"
#include "Spt_Cfg.h"
#include "rsdk_version.h"

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

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

/** @}*/

/** * @addtogroup spt_driver_api_func
* @{
*/

/**
* @brief        This function initializes the internal state of the SPT Driver and the hardware registers of the SPT
*               in preparation for running SPT kernel functions.
* @details      It checks the validity of the input arguments, then writes them into the SPT registers.
*               It does not modify the input argument.
*               It also enables boot-up of the BBE32 DSP, if Spt_DriverInitType::dspEn is not set to 0.
*
* @param[in]    pSptInitInfo - pointer to the SPT Driver initialization structure.
*
* @return       success or error status information.
*
* @pre          It must be called in the following situations:
*               <ul>
*                   <li>during the first setup of the SPT Driver (e.g. after boot),
*                   <li>whenever the system parameters described in Spt_DriverInitType need to be changed
*                   <li>everytime the Spt_Run() returns an SPT hardware error
*               </ul>
*
*/
Std_ReturnType Spt_Setup(Spt_DriverInitType const *const pSptInitInfo);

/**
* @brief        Configures the SPT kernel parameters, triggers SPT execution, feeds back status information to the caller.
* @details      It checks the validity of the input arguments, then parses the SPT Spt_DriverContextType::kernelParList,
*               writes the parameters into the SPT working registers (see @ref spt_call_conv), starts the SPT,
*               then it either waits for SPT completion (blocking mode) or exits immediately (non-blocking mode).
*               See also the @ref spt_rm_running for details.
*
* @param[in]    sptContext - pointer to the SPT runtime parameters structure
*
* @return       success or error status information.
*
* @pre          It must be called only after Spt_Setup() has been executed successfully at least once.
*               If the SPT signals a hardware error (through the Spt_DriverContextType::ecsIsrCb ),
*               then it is recommended to re-run Spt_Setup() before calling Spt_Run()
*/
Std_ReturnType Spt_Run(Spt_DriverContextType const *const sptContext);

/**
* @brief        Used to handle asynchronous control or status requests, apart from the SPT kernel processing sequence.
* @details      See Spt_DriverCommandIdType for details about the supported commands.
*
* @param[in]    pSptCommand - pointer to a structure containing the ID of the command to be served and additional parameters that may be needed.
*
* @param[out]    pSptCmdResult - container for the returned information.
*
* @return       success or error status information.
*
* @pre          It must be called only after Spt_Setup() has been executed successfully at least once.
*               If the SPT signals a hardware error (through the Spt_DriverContextType::ecsIsrCb ),
*               then it is recommended to re-run Spt_Setup() before calling Spt_Command().
*/
Std_ReturnType Spt_Command(Spt_DriverCommandType const *const pSptCommand, Spt_DriverCmdResType *const pSptCmdResult);

/**
* @brief        This function stops the SPT processing, brings the hardware and the Driver back into reset state,
*               ready for a new initialization.
* @details      It checks the validity of the input arguments, then stops the SPT sequencer, data acquisition
*               and waits until the SPT transitions to 'reset' state.
*
* @return       success or error status information.
*
* @pre          It must be called only after Spt_Setup() has been executed successfully at least once, to prepare
*               for reinitialization or to recover from a SPT hardware error.
*/
Std_ReturnType Spt_Stop(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CDD_SPT_H */
