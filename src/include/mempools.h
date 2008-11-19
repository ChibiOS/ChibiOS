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

#ifndef _MEMPOOLS_H_
#define _MEMPOOLS_H_

#ifdef CH_USE_MEMPOOLS

struct pool_header {
  struct pool_header *ph_next;
};

typedef struct {
  struct pool_header    *mp_next;
  size_t                mp_object_size;
#ifdef CH_USE_HEAP
  bool_t                mp_grow;
#endif /* CH_USE_HEAP */
} MemoryPool;

#ifdef __cplusplus
extern "C" {
#endif
  void chPoolInit(MemoryPool *mp, size_t size);
  void *chPoolAlloc(MemoryPool *mp);
  void chPoolFree(MemoryPool *mp, void *objp);
#ifdef CH_USE_HEAP
  void chPoolRelease(MemoryPool *mp);
#endif
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_MEMPOOLS */

#endif /* _MEMPOOLS_H_ */

/** @} */
