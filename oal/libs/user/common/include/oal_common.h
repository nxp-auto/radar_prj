/*
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_OALCOMMON_H
#define OAL_OALCOMMON_H

#include <oal_utils.h>

__BEGIN_DECLS

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

__END_DECLS

#endif /* OAL_OALCOMMON_H */
