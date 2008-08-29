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
 * Initializes a memory pool.
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
 * @param allow_growth if \p TRUE then the object is allocated by using
 *                     \p chHeapAlloc() in case the memory pool is empty
 * @return the pointer to the allocated object or \p NULL if the memory is
 *         exhausted
 */
void *chPoolAlloc(MemoryPool *mp, bool_t allow_growth) {
  void *p;

  chDbgAssert(mp != NULL, "chpools.c, chPoolAlloc()");

  chSysLock();

  if (mp->mp_next == NULL) {
#ifdef CH_USE_HEAP
    if (allow_growth) {

      chSysUnlock();
      return chHeapAlloc(mp->mp_object_size);
    }
#endif /* CH_USE_HEAP */
    return NULL;
  }
  p = mp->mp_next;
  mp->mp_next = mp->mp_next->ph_next;

  chSysUnlock();
  return p;
}

/**
 * Releases (or adds) an object into a memory pool.
 * @param mp pointer to a \p MemoryPool structure
 * @param objp the pointer to the object to be released or added
 * @note the object is assumed to be of the right size for the specified
 *       buffer.
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

#ifdef CH_USE_HEAP
/**
 * Releases all the objects contained into a pool.
 * @param mp pointer to a \p MemoryPool structure
 * @note It is assumed that all the object are allocated using the heap
 *       allocator, do not use this function if the pool contains other kind
 *       of objects.
 */
void chPoolRelease(MemoryPool *mp) {
  void *p;

  while ((p = chPoolAlloc(mp, FALSE)) != NULL)
    chHeapFree(p);
}
#endif

#endif /* CH_USE_MEMPOOLS */

/** @} */
