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
 * @file heap.h
 * @brief Heaps macros and structures.
 * @addtogroup heaps
 * @{
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#if CH_USE_HEAP

/*
 * Module dependencies check.
 */
#if !CH_USE_MEMCORE && !CH_USE_MALLOC_HEAP
#error "CH_USE_HEAP requires CH_USE_MEM"
#endif

#if !CH_USE_MUTEXES && !CH_USE_SEMAPHORES
#error "CH_USE_HEAP requires CH_USE_MUTEXES and/or CH_USE_SEMAPHORES"
#endif

typedef struct memory_heap MemoryHeap;

/**
 * @brief Memory heap block header.
 */
struct heap_header {
  union {
    struct heap_header  *h_next;    /**< @brief Next block in free list.    */
    MemoryHeap          *h_heap;    /**< @brief Block owner heap.           */
  };
  size_t                h_size;     /**< @brief Size of the memory block.   */
};

/**
 * @brief Structure describing a memory heap.
 */
struct memory_heap {
  memgetfunc_t          h_provider; /**< @brief Memory blocks provider for
                                                this heap.                  */
  struct heap_header    h_free;     /**< @brief Free blocks list header.    */
#if CH_USE_MUTEXES
  Mutex                 h_mtx;      /**< @brief Heap access mutex.          */
#else
  Semaphore             h_sem;      /**< @brief Heap access semaphore.      */
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
  void heap_init(void);
  void chHeapInit(MemoryHeap *heapp, void *buf, size_t size);
  void *chHeapAlloc(MemoryHeap *heapp, size_t size);
  void chHeapFree(void *p);
  size_t chHeapStatus(MemoryHeap *heapp, size_t *sizep);
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_HEAP */

#endif /* _HEAP_H_ */

/** @} */
