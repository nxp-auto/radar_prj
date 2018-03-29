/*
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OALCOMMON_H
#define OALCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////
///
/// Initializes OAL (public call deprecated)
///
/// \return 
///    - On Success: OAL_SUCCESS
///    - On Failure: OAL_FAILURE
/////////////////////////////////////////////////////////////////////////
int32_t OAL_Initialize(void);

/////////////////////////////////////////////////////////////////////////
///
/// Deinitializes OAL (public call deprecated)
///
/// \return 
///    - On Success: OAL_SUCCESS
///    - On Failure: OAL_FAILURE
/////////////////////////////////////////////////////////////////////////
int32_t OAL_Deinitialize(void);

#ifdef __cplusplus
}
#endif

#endif /* OALCOMMON_H */
