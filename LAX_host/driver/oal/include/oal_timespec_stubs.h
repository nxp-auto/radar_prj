/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * This file contains prototypes for the functions that should be
 * implemented for standalone applications.
 *
 * These functions are platform dependent and should be implemented by the user
 * of oal library. These functions are compiled and linked with the
 * provided oal library into an executable.
 *
 * The provided library needs these functions to be able to work.
 */

#ifndef OAL_TIMESPEC_STUBS_H
#define OAL_TIMESPEC_STUBS_H

#include "os_utils.h"

__BEGIN_DECLS

/**
 * @brief Suspends the execution of the calling thread for <tt>nsecs</tt>
 * nanoseconds
 *
 * @param[in] begin The getTicks() value at the sleep start
 * @param[in] nsecs Sleep duration
 *
 * @return 0 for success or a negative value for an error
 */
int nsleep(unsigned long int begin, unsigned long int nsecs);

/**
 * @brief Get value in miliseconds
 *
 * @param[out] msecs Value in miliseconds
 *
 * @return 0 for success or a negative value for an error
 */
int gettime(unsigned long int *msecs);

/**
 * @brief Get number of ticks
 *
 * @return 0 success or a negative value for an error
 */
unsigned long getTicks(void);

/**
 * @brief Suspends the execution of the calling thread for usecs
 * microseconds.
 *
 * @param[out] usecs Value in microseconds
 *
 * @return 0 success or a negative value for an error
 */
void udelay(unsigned long usecs);

__END_DECLS

#endif
