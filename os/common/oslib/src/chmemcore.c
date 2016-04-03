/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    chmemcore.c
 * @brief   Core memory manager code.
 *
 * @addtogroup memcore
 * @details Core Memory Manager related APIs and services.
 *          <h2>Operation mode</h2>
 *          The core memory manager is a simplified allocator that only
 *          allows to allocate memory blocks without the possibility to
 *          free them.<br>
 *          This allocator is meant as a memory blocks provider for the
 *          other allocators such as:
 *          - C-Runtime allocator (through a compiler specific adapter module).
 *          - Heap allocator (see @ref heaps).
 *          - Memory pools allocator (see @ref pools).
 *          .
 *          By having a centralized memory provider the various allocators
 *          can coexist and share the main memory.<br>
 *          This allocator, alone, is also useful for very simple
 *          applications that just require a simple way to get memory
 *          blocks.
 * @pre     In order to use the core memory manager APIs the @p CH_CFG_USE_MEMCORE
 *          option must be enabled in @p chconf.h.
 * @note    Compatible with RT and NIL.
 * @{
 */

#include "ch.h"

#if (CH_CFG_USE_MEMCORE == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static uint8_t *nextmem;
static uint8_t *endmem;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level memory manager initialization.
 *
 * @notapi
 */
void _core_init(void) {
#if CH_CFG_MEMCORE_SIZE == 0
  extern uint8_t __heap_base__[];
  extern uint8_t __heap_end__[];

  /*lint -save -e9033 [10.8] Required cast operations.*/
  nextmem = __heap_base__;
  endmem  = __heap_end__;
  /*lint restore*/
#else
  static uint8_t static_heap[CH_CFG_MEMCORE_SIZE];

  nextmem = &static_heap[0];
  endmem  = &static_heap[CH_CFG_MEMCORE_SIZE];
#endif
}

/**
 * @brief   Allocates a memory block.
 * @details The allocated block is guaranteed to be properly aligned to the
 *          specified alignment.
 *
 * @param[in] size      the size of the block to be allocated.
 * @param[in] align     desired memory alignment
 * @return              A pointer to the allocated memory block.
 * @retval NULL         allocation failed, core memory exhausted.
 *
 * @iclass
 */
void *chCoreAllocAlignedI(size_t size, unsigned align) {
  uint8_t *p;

  chDbgCheckClassI();
  chDbgCheck(MEM_IS_VALID_ALIGNMENT(align));

  size = MEM_ALIGN_NEXT(size, align);
  p = (uint8_t *)MEM_ALIGN_NEXT(nextmem, align);

  if (((size_t)endmem - (size_t)p) < size) {
    return NULL;
  }
  nextmem = p + size;

  return p;
}

/**
 * @brief   Allocates a memory block.
 * @details The allocated block is guaranteed to be properly aligned to the
 *          specified alignment.
 *
 * @param[in] size      the size of the block to be allocated
 * @param[in] align     desired memory alignment
 * @return              A pointer to the allocated memory block.
 * @retval NULL         allocation failed, core memory exhausted.
 *
 * @api
 */
void *chCoreAllocAligned(size_t size, unsigned align) {
  void *p;

  chSysLock();
  p = chCoreAllocAlignedI(size, align);
  chSysUnlock();

  return p;
}

/**
 * @brief   Core memory status.
 *
 * @return              The size, in bytes, of the free core memory.
 *
 * @xclass
 */
size_t chCoreGetStatusX(void) {

  /*lint -save -e9033 [10.8] The cast is safe.*/
  return (size_t)(endmem - nextmem);
  /*lint -restore*/
}
#endif /* CH_CFG_USE_MEMCORE == TRUE */

/** @} */
