/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file chmemcore.c
 * @brief Core memory manager code.
 * @addtogroup memcore
 * @{
 */

#include "ch.h"

#if CH_USE_MEMCORE

static uint8_t *nextmem;
static uint8_t *endmem;

/**
 * @brief Low level memory manager initialization.
 *
 * @note Internal use only.
 */
void core_init(void) {
#if CH_MEMCORE_SIZE == 0
  extern uint8_t __heap_base__;
  extern uint8_t __heap_end__;
  nextmem = &__heap_base__;
  endmem = &__heap_end__;
#else
  static align_t buffer[MEM_ALIGN_SIZE(CH_MEMCORE_SIZE) / sizeof(align_t)];
  nextmem = (uint8_t *)&buffer[0];
  endmem = (uint8_t *)&buffer[MEM_ALIGN_SIZE(CH_MEMCORE_SIZE) / sizeof(align_t)];
#endif
}

/**
 * @brief Allocates a memory block.
 * @details The size of the returned block is aligned to the alignment
 *          type @p align_t so it is not possible to allocate less than
 *          <code>sizeof(align_t)</code>.
 *
 *
 * @param[in] size the size of the block to be allocated
 * @return A pointer to the allocated memory block.
 * @retval NULL allocation failed, core memory exhausted.
 */
void *chCoreAlloc(size_t size) {
  void *p;

  chSysLock();
  p = chCoreAllocI(size);
  chSysUnlock();
  return p;
}

/**
 * @brief Allocates a memory block.
 * @details The size of the returned block is aligned to the alignment
 *          type @p align_t so it is not possible to allocate less than
 *          <code>sizeof(align_t)</code>.
 *
 * @param[in] size the size of the block to be allocated.
 * @return A pointer to the allocated memory block.
 * @retval NULL allocation failed, core memory exhausted.
 */
void *chCoreAllocI(size_t size) {
  void *p;

  size = MEM_ALIGN_SIZE(size);
  if ((size_t)(endmem - nextmem) < size)
    return NULL;
  p = nextmem;
  nextmem += size;
  return p;
}

#endif /* CH_USE_MEMCORE */

/** @} */
