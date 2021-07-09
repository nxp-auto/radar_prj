/*
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef OAL_ONCE_H
#define OAL_ONCE_H

#include <oal_utils.h>
#include <os_oal_once.h>

/**
 * @defgroup OAL_Once OAL Once
 *
 * @{
 * @brief Module initialization mechanism
 * @details
 * This functionality is similar to <tt>pthread_once</tt>. It offers the
 * possibility to initialize a library / module in a thread safe manner
 * regardless of the number of calls.
 *
 * Example:
 * @code
 * static void constructor()
 * {
 * 	#if 0
 * 	  Constructor code
 * 	#endif
 * }
 *
 * static struct OAL_OnceControl gInitialized = OAL_ONCE_INIT;
 *
 * static void moduleFunction()
 * {
 * 	OAL_ExecuteOnce(&gInitialized, constructor);
 *
 * 	#if 0
 * 	  After initialization code
 * 	#endif
 * }
 * @endcode
 *
 * @see man pthread_once
 * @see https:/linux.die.net/man/3/pthread_once
 */

/**
 * @brief Initialization for OAL_OnceControl.
 *
 * Example:
 * @code
 * static struct OAL_OnceControl gMyInitBlock = OAL_ONCE_INIT;
 * @endcode
 */
#ifndef OAL_ONCE_INIT
#define OAL_ONCE_INIT
#endif

__BEGIN_DECLS

/**
 * @brief OAL Once control block
 */
struct OAL_OnceControl;

/**
 * @brief Initialization function to be passed along to \ref OAL_ExecuteOnce
 */
typedef void (*OAL_OnceFunction_t)(void);

/**
 * @brief Execute <tt>aFunc</tt> if not executed yet.
 * OAL Once guarantees one single execution of <tt>aFunc</tt>, regardless of the
 * of \ref OAL_ExecuteOnce calls.
 *
 * @param[in] apOnce Initialized control block
 * @param[in] aFunc  The initialization function of the module to be executed
 *                   once.
 *
 * @return 0 if the function succeeds, a negative value otherwise.
 */
int32_t OAL_ExecuteOnce(struct OAL_OnceControl *apOnce,
                        OAL_OnceFunction_t aFunc);

__END_DECLS

/** @} */

#ifdef OAL_TRACE_API_FUNCTIONS
#include <trace/oal_once.h>
#endif
#endif
