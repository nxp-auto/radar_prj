/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OAL_COMM_H
#define OAL_COMM_H

#include "oal_utils.h"

__BEGIN_DECLS

/**
 * @file oal_comm.h
 * @brief Generic API for application <-> low level driver communication
 */

/**
 * @brief Driver handle
 */
struct OS_OAL_DriverHandle;
typedef struct OS_OAL_DriverHandle* OAL_DriverHandle_t;

/**
 * @brief Describe the argument of a remote function
 */
struct OAL_FuncArgs {
	void *data; ///< The address of the argument
	size_t size; ///< Size of the argument
};

typedef struct OAL_FuncArgs OAL_FuncArgs_t;

/**
 * @brief Open a communication channel to the driver
 *
 * @param[in]  id The identification path of the driver
 *
 * @return A handle of the channel if the operation succeeded, NULL otherwise
 */

OAL_DriverHandle_t OAL_OpenDriver(const char* id);

/**
 * @brief Call a function exported by the driver.
 *
 * @param[in]  h          Driver handle
 * @param[in]  func       The ID of the function to be called
 * @param[in]  in_args    Array with input parameters
 * @param[in]  n_in_args  Number of input parameters
 * @param[out] out_args   Array with outputs
 * @param[in]  n_out_args Number of outputs
 *
 * @return The value set by the driver.
 */
uint32_t OAL_DriverCall(OAL_DriverHandle_t h, uint32_t func,
		OAL_FuncArgs_t *in_args,
		size_t n_in_args,
		OAL_FuncArgs_t *out_args,
		size_t n_out_args);

/**
 * @brief Call a function exported by the driver with no parameters.
 *
 * Equivalent of:
 * @code
 * void driver_function(void)
 * @endcode
 *
 * @param[in]  h          Driver handle
 * @param[in]  func       The ID of the function to be called
 *
 * @return The value set by the driver.
 */

static inline uint32_t OAL_DriverNoArgCall(OAL_DriverHandle_t h, uint32_t func)
{
	return OAL_DriverCall(h, func, NULL, 0, NULL, 0);
}

/**
 * @brief Call a function exported by the driver only with input parameters
 *
 * @param[in]  h          Driver handle
 * @param[in]  func       The ID of the function to be called
 * @param[in]  in_args    Array with input parameters
 * @param[in]  n_in_args  Number of input parameters
 *
 * @return The value set by the driver.
 */
static inline uint32_t OAL_DriverInCall(OAL_DriverHandle_t h, uint32_t func,
					OAL_FuncArgs_t *in_args,
					size_t n_in_args)
{
	return OAL_DriverCall(h, func, in_args, n_in_args, NULL, 0);
}

/**
 * @brief Call a function exported by the driver with no output parameters
 *
 * @param[in]  h          Driver handle
 * @param[in]  func       The ID of the function to be called
 * @param[out] out_args   Array with outputs
 * @param[in]  n_out_args Number of outputs
 *
 * @return The value set by the driver.
 */
static inline uint32_t OAL_DriverOutCall(OAL_DriverHandle_t h, uint32_t func,
					OAL_FuncArgs_t *out_args,
					size_t n_out_args)
{
	return OAL_DriverCall(h, func, NULL, 0, out_args, n_out_args);
}

/**
 * @brief Call a remote function with one single argument. The provided
 * argument is considered an in/out one.
 *
 * @param[in] H           Driver handle
 * @param[in] FUNC        The ID of the function to be called
 * @param[in,out] STRUCT  IN/OUT argument passed by value. The wrapper will
 *                        determine its size and pass down its address to
 *                        <tt>OAL_DriverCall</tt>.
 *
 * @return The value set by the driver.
 */
#define OAL_SimpleInOutCall(H, FUNC, STRUCT)                     \
__extension__ ({                                                 \
	OAL_DriverHandle_t _h = (H);                             \
	uint32_t _func = (FUNC);                                 \
	OAL_FuncArgs_t _in_arg, _out_arg;                        \
                                                                 \
	_in_arg.data = &STRUCT;                                  \
	_in_arg.size = sizeof(STRUCT);                           \
                                                                 \
	_out_arg.data = &STRUCT;                                 \
	_out_arg.size = sizeof(STRUCT);                          \
                                                                 \
	OAL_DriverCall(_h, _func, &_in_arg, 1, &_out_arg, 1);    \
})

/**
 * @brief Call a remote function with one input argument.
 *
 * @param[in] H           Driver handle
 * @param[in] FUNC        The ID of the function to be called
 * @param[in] STRUCT      IN argument passed by value. The wrapper will
 *                        determine its size and pass down its address to
 *                        <tt>OAL_DriverCall</tt>.
 *
 * @return The value set by the driver.
 */
#define OAL_SimpleInCall(H, FUNC, STRUCT)                        \
__extension__ ({                                                 \
	OAL_DriverHandle_t _h = (H);                             \
	uint32_t _func = (FUNC);                                 \
	OAL_FuncArgs_t _in_arg;                                  \
                                                                 \
	_in_arg.data = &STRUCT;                                  \
	_in_arg.size = sizeof(STRUCT);                           \
                                                                 \
	OAL_DriverInCall(_h, _func, &_in_arg, 1);                \
})

/**
 * @brief Call a remote function with one output argument.
 *
 * @param[in] H           Driver handle
 * @param[in] FUNC        The ID of the function to be called
 * @param[out] STRUCT     OUT argument passed by value. The wrapper will
 *                        determine its size and pass down its address to
 *                        <tt>OAL_DriverCall</tt>.
 *
 * @return The value set by the driver.
 */
#define OAL_SimpleOutCall(H, FUNC, STRUCT)                       \
__extension__ ({                                                 \
	OAL_DriverHandle_t _h = (H);                             \
	uint32_t _func = (FUNC);                                 \
	OAL_FuncArgs_t  _out_arg;                                \
                                                                 \
	_out_arg.data = &STRUCT;                                 \
	_out_arg.size = sizeof(STRUCT);                          \
                                                                 \
	OAL_DriverOutCall(_h, _func, &_out_arg, 1);              \
})

/**
 * @brief Close the driver channel
 *
 * @param[in]  h Driver handle
 *
 * @return 0 if the operation succeeded, a negative value otherwise.
 */
int OAL_CloseDriver(OAL_DriverHandle_t *h);

__END_DECLS

#endif /* OAL_COMM_H */
