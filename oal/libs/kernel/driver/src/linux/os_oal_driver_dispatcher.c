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
#include <linux/mman.h>
#include <linux/module.h>

#define OAL_LOG_SUPPRESS_DEBUG
#include "oal_log.h"
#include "oal_driver_dispatcher.h"
#include "oal_driver_functions.h"
#include "os_oal_comm_kernel.h"

// Cache control functions defined in ASM
extern void flush_dcache_range(void *pMemory, uint32_t size);
extern void invalidate_dcache_range(void *pMemory, uint32_t size);
extern void flush_and_invalidate_dcache_range(void *pMemory, uint32_t size);

uint32_t osOalDriverDispatcher(oal_dispatcher_t *d, uint32_t func, char *in, int len)
{
	uint32_t lRetVal = 0;

	switch (func)
	{
		///////////////////////////////////////////////////////////////////////////
		// Flush & Invalidate
		case CMD_FLUSHINVALIDATE_SPECIFIC:
			{
				CMD_FLUSH_SPECIFIC_TYPE *aux = (CMD_FLUSH_SPECIFIC_TYPE *)in;

				OAL_LOG_DEBUG("OAL: FLUSH & INVALIDATE ADDRESS\n");

				flush_and_invalidate_dcache_range((void *)aux->virtual_pointer, aux->size);
				break;
			}
		case CMD_INVALIDATE_SPECIFIC:
			{
				CMD_FLUSH_SPECIFIC_TYPE *aux = (CMD_FLUSH_SPECIFIC_TYPE *) in;

				OAL_LOG_DEBUG("OAL: INVALIDATE ADDRESS\n");

				invalidate_dcache_range((void *)aux->virtual_pointer, aux->size);

				break;
			}

			///////////////////////////////////////////////////////////////////////////
			// Flush
		case CMD_FLUSH_SPECIFIC:
			{
				CMD_FLUSH_SPECIFIC_TYPE *aux = (CMD_FLUSH_SPECIFIC_TYPE *)in;

				OAL_LOG_DEBUG("OAL: FLUSH ADDRESS\n");

				flush_dcache_range((void *)aux->virtual_pointer, aux->size);

				break;
			}
		default:
			{
				OAL_LOG_ERROR("Invalid function ID: %d\n", func);
				lRetVal = -EINVAL;
			}
	}

	return lRetVal;
}


int oalMemoryZeroisation(oal_dispatcher_t *apDispatcher, uint64_t aPhysAddr,
		uint64_t aSize)
{
	int lRetVal = 0;
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	unsigned long lVirtAddr;

	lVirtAddr = vm_mmap(OAL_WriteGetFile(apDispatcher),
			0,
			aSize,
			PROT_READ | PROT_WRITE | PROT_EXEC,
			MAP_SHARED,
			aPhysAddr);
	if (lVirtAddr == 0) {
		lRetVal = -EIO;
	} else {
		memset((void *)lVirtAddr, 0, aSize);

		vma = find_vma(mm, lVirtAddr);
		if (vma == NULL) {
			lRetVal = -EIO;
		} else {
			flush_and_invalidate_dcache_range((void *)lVirtAddr, aSize);
			vm_munmap(lVirtAddr, aSize);
		}
	}

	return lRetVal;
}
