/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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
 * @file    chmempools.c
 * @brief   Memory Pools code.
 *
 * @addtogroup pools
 * @details Memory Pools related APIs and services.
 *          <h2>Operation mode</h2>
 *          The Memory Pools APIs allow to allocate/free fixed size objects in
 *          <b>constant time</b> and reliably without memory fragmentation
 *          problems.
 * @pre     In order to use the memory pools APIs the @p CH_USE_MEMPOOLS option
 *          must be enabled in @p chconf.h.
 * @{
 */

#include "ch.h"

#if CH_USE_MEMPOOLS || defined(__DOXYGEN__)
/**
 * @brief   Initializes an empty memory pool.
 * @note    The size is internally aligned to be a multiple of the
 *          @p stkalign_t type size.
 *
 * @param[out] mp       pointer to a @p MemoryPool structure
 * @param[in] size      the size of the objects contained in this memory pool,
 *                      the minimum accepted size is the size of a pointer to
 *                      void.
 * @param[in] provider  memory provider function for the memory pool or
 *                      @p NULL if the pool is not allowed to grow
 *                      automatically
 *
 * @init
 */
void chPoolInit(MemoryPool *mp, size_t size, memgetfunc_t provider) {

  chDbgCheck((mp != NULL) && (size >= sizeof(void *)), "chPoolInit");

  mp->mp_next = NULL;
  mp->mp_object_size = MEM_ALIGN_SIZE(size);
  mp->mp_provider = provider;
}

/**
 * @brief   Allocates an object from a memory pool.
 *
 * @param[in] mp        pointer to a @p MemoryPool structure
 * @return              The pointer to the allocated object.
 * @retval NULL         if pool is empty.
 *
 * @iclass
 */
void *chPoolAllocI(MemoryPool *mp) {
  void *objp;

  chDbgCheck(mp != NULL, "chPoolAllocI");

  if ((objp = mp->mp_next) != NULL)
    mp->mp_next = mp->mp_next->ph_next;
  else if (mp->mp_provider != NULL)
    objp = mp->mp_provider(mp->mp_object_size);
  return objp;
}

/**
 * @brief   Allocates an object from a memory pool.
 *
 * @param[in] mp        pointer to a @p MemoryPool structure
 * @return              The pointer to the allocated object.
 * @retval NULL         if pool is empty.
 *
 * @api
 */
void *chPoolAlloc(MemoryPool *mp) {
  void *objp;

  chSysLock();
  objp = chPoolAllocI(mp);
  chSysUnlock();
  return objp;
}

/**
 * @brief   Releases (or adds) an object into (to) a memory pool.
 * @pre     The freed object must be of the right size for the specified
 *          memory pool.
 * @pre     The freed object must be memory aligned to the size of
 *          @p stkalign_t type.
 *
 * @param[in] mp        pointer to a @p MemoryPool structure
 * @param[in] objp      the pointer to the object to be released or added
 *
 * @iclass
 */
void chPoolFreeI(MemoryPool *mp, void *objp) {
  struct pool_header *php = objp;

  chDbgCheck((mp != NULL) && (objp != NULL) && MEM_IS_ALIGNED(objp),
             "chPoolFreeI");

  php->ph_next = mp->mp_next;
  mp->mp_next = php;
}

/**
 * @brief   Releases (or adds) an object into (to) a memory pool.
 * @pre     The freed object must be of the right size for the specified
 *          memory pool.
 * @pre     The freed object must be memory aligned to the size of
 *          @p stkalign_t type.
 *
 * @param[in] mp        pointer to a @p MemoryPool structure
 * @param[in] objp      the pointer to the object to be released or added
 *
 * @api
 */
void chPoolFree(MemoryPool *mp, void *objp) {

  chSysLock();
  chPoolFreeI(mp, objp);
  chSysUnlock();
}
#endif /* CH_USE_MEMPOOLS */

/** @} */
