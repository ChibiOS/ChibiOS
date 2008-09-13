/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @addtogroup MemoryPools
 * @{
 */

#include <ch.h>

#ifdef CH_USE_MEMPOOLS

/**
 * Initializes an empty memory pool.
 * @param mp pointer to a \p MemoryPool structure
 * @param size the size of the objects contained in this memory pool
 */
void chPoolInit(MemoryPool *mp, size_t size) {

  chDbgAssert((mp != NULL) && (size >= sizeof(void *)),
              "chpools.c, chPoolFree()");

  mp->mp_next = NULL;
  mp->mp_object_size = size;
}

/**
 * Allocates an object from a memory pool.
 * @param mp pointer to a \p MemoryPool structure
 * @return the pointer to the allocated object or \p NULL if pool is empty
 */
void *chPoolAlloc(MemoryPool *mp) {
  void *objp;

  chDbgAssert(mp != NULL, "chpools.c, chPoolAlloc()");

  chSysLock();

  if ((objp = mp->mp_next) != NULL)
    mp->mp_next = mp->mp_next->ph_next;

  chSysUnlock();
  return objp;
}

/**
 * Releases (or adds) an object into (to) a memory pool.
 * @param mp pointer to a \p MemoryPool structure
 * @param objp the pointer to the object to be released or added
 * @note the object is assumed to be of the right size for the specified
 *       memory pool.
 */
void chPoolFree(MemoryPool *mp, void *objp) {
  struct pool_header *php = objp;

  chDbgAssert((mp != NULL) && (objp != NULL),
              "chpools.c, chPoolFree()");

  chSysLock();

  php->ph_next = mp->mp_next;
  mp->mp_next = php;

  chSysUnlock();
}

#endif /* CH_USE_MEMPOOLS */

/** @} */
