/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#define OAL_LOG_SUPPRESS_DEBUG
#include "oal_log.h"
#include "oal_driver_dispatcher.h"
#include "oal_driver_functions.h"
#include "oal_allocation_kernel.h"
#include "os_oal_driver_dispatcher.h"
#include "oal_cma_list.h"

uint8_t  gLoadedDevices = 0;
uint8_t  gAutobalancedDevices = 0;
uint32_t gDeviceAlignment[OAL_MAX_ALLOCATOR_NUM] = {0};

uint32_t oalDriverDispatcher(oal_dispatcher_t *d, uint32_t func, char *in, int len)
{
	uint32_t lRetVal = 0;

	switch (func)
	{
			///////////////////////////////////////////////////////////////////////////
			// Allocate a new contiguous region
		case CMD_ALLOC:
			{
				CMD_ALLOC_TYPE *aux;
				pid_t lPid;

				OAL_RPCGetClientPID(d, &lPid);

				OAL_LOG_DEBUG("OAL: ALLOCATE\n");

				aux = (CMD_ALLOC_TYPE *)in;

				aux->ret_phys_pointer = oal_alloc(aux->size, aux->align, aux->chunk_id, lPid);
				if ((aux->flags & OAL_MEMORY_FLAG_ZERO) &&
						(aux->ret_phys_pointer != 0)) {
					lRetVal = oalMemoryZeroisation(d,
							aux->ret_phys_pointer,
							aux->size);
					if (lRetVal != 0) {
						OAL_LOG_ERROR("Failed to zeroise");
					}
				}

				OAL_RPCAppendReply(d, (void *)aux, sizeof(*aux));

				break;
			}

			///////////////////////////////////////////////////////////////////////////
			// Free unused contiguous region
		case CMD_FREE:
			{
				uint64_t *handle_pointer = (uint64_t *)in;

				OAL_LOG_DEBUG("OAL: FREE\n");
				if (oal_free_phys(*handle_pointer) != 0)
				{
					lRetVal = -EINVAL;
				}

				break;
			}
			///////////////////////////////////////////////////////////////////////////
			// Get number of allocations
		case CMD_INFO:
			{
				uint64_t allocs;

				OAL_LOG_DEBUG("OAL: GET NUM ALLOCATIONS\n");

				allocs = oal_get_num_allocations();
				OAL_RPCAppendReply(d, (void *)&allocs, sizeof(allocs));

				break;
			}
		case CMD_MEMORY_GET_SIZE:
			{
				uint64_t *aux = (uint64_t *)in;

				OAL_LOG_DEBUG("OAL: GET SIZE OF DEVICE\n");

				*aux = apex_allocator_get_total_size(*aux);
				OAL_RPCAppendReply(d, (void *)aux, sizeof(*aux));

				break;
			}
			///////////////////////////////////////////////////////////////////////////

			// Get base address
		case CMD_MEMORY_GET_BASE:
			{
				uint64_t *aux = (uint64_t *)in;

				OAL_LOG_DEBUG("OAL: GET BASE ADDRESS\n");

				*aux = apex_allocator_get_physical_base(*aux);
				OAL_RPCAppendReply(d, (void *)aux, sizeof(*aux));

				break;
			}
			///////////////////////////////////////////////////////////////////////////
			// Get available devices
		case CMD_MEMORY_GET_DEVICES:
			{
				uint8_t lSizeInBytes;

				lSizeInBytes = gLoadedDevices;

				OAL_LOG_DEBUG("OAL: GET LOADED DEVICES MASK\n");

				OAL_RPCAppendReply(d, (void *)&lSizeInBytes, sizeof(lSizeInBytes));
				break;
			}
			///////////////////////////////////////////////////////////////////////////
			// Get memory size total
		case CMD_MEMORY_SIZE_TOTAL_GET:
			{
				int64_t lSizeInBytes;

				lSizeInBytes = oal_memorysizetotal();

				OAL_LOG_DEBUG("OAL: GET TOTAL MEMORY SIZE\n");

				OAL_RPCAppendReply(d, (void *)&lSizeInBytes, sizeof(lSizeInBytes));
				break;
			}
			///////////////////////////////////////////////////////////////////////////
			// Get free memory size
		case CMD_MEMORY_SIZE_FREE_GET:
			{
				int64_t lSizeInBytes;

				lSizeInBytes = oal_memorysizefree();

				OAL_LOG_DEBUG("OAL: GET FREE MEMORY\n");

				OAL_RPCAppendReply(d, (void *)&lSizeInBytes, sizeof(lSizeInBytes));

				break;
			}
			///////////////////////////////////////////////////////////////////////////
			// Get autobalanced devices
		case CMD_MEMORY_GET_AUTOBALANCE:
			{
				uint8_t lSizeInBytes;

				lSizeInBytes = gAutobalancedDevices;

				OAL_LOG_DEBUG("OAL: GET AUTOBALANCED DEVICES MASK\n");

				OAL_RPCAppendReply(d, (void *)&lSizeInBytes, sizeof(lSizeInBytes));

				break;
			}
		default:
			{
				// Pass down to OS layer
				lRetVal = osOalDriverDispatcher(d, func, in, len);
			}
	}

	return lRetVal;
}



