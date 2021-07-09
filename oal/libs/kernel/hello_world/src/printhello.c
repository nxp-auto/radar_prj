/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "printhello.h"
#include "oal_log.h"

void OAL_Printhello(void)
{
	OAL_LOG_NOTE("%s: %d: Hello world!\n", __FILE__, __LINE__);
}
