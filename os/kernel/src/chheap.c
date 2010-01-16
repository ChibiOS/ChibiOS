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
 * @file chheap.c
 * @brief Heaps code.
 * @addtogroup heaps
 * @{
 */

#include "ch.h"

#if CH_USE_HEAP

/*
 * Defaults on the best synchronization mechanism available.
 */
#if CH_USE_MUTEXES
#define H_LOCK(h)       chMtxLock(&(h)->h_mtx)
#define H_UNLOCK(h)     chMtxUnlock()
#else
#define H_LOCK(h)       chSemWait(&(h)->h_sem)
#define H_UNLOCK(h)     chSemSignal(&(h)->h_sem)
#endif

#if !CH_USE_MALLOC_HEAP

/**
 * @brief Default heap descriptor.
 */
static MemoryHeap default_heap;

/**
 * @brief Initializes the default heap.
 *
 * @note Internal use only.
 */
void heap_init(void) {
  default_heap.h_provider = chCoreAlloc;
  default_heap.h_free.h_next = NULL;
  default_heap.h_free.h_size = 0;
#if CH_USE_MUTEXES
  chMtxInit(&default_heap.h_mtx);
#else
  chSemInit(&default_heap.h_sem, 1);
#endif
}

/**
 * @brief Initializes a memory heap from a static memory area.
 *
 * @param[out] heapp pointer to a memory heap descriptor to be initialized
 * @param[in] buf heap buffer base
 * @param[in] size heap size
 *
 * @note Both the heap buffer base and the heap size must be aligned to
 *       the @p align_t type size.
 */
void chHeapInit(MemoryHeap *heapp, void *buf, size_t size) {
  struct heap_header *hp;

  chDbgCheck(MEM_IS_ALIGNED(buf) && MEM_IS_ALIGNED(size), "chHeapInit");

  heapp->h_provider = NULL;
  heapp->h_free.h_next = hp = buf;
  heapp->h_free.h_size = 0;
  hp->h_next = NULL;
  hp->h_size = size - sizeof(struct heap_header);
#if CH_USE_MUTEXES
  chMtxInit(&heapp->h_mtx);
#else
  chSemInit(&heapp->h_sem, 1);
#endif
}

/**
 * @brief Allocates a block of memory from the heap by using the first-fit
 *        algorithm.
 * @details The allocated block is guaranteed to be properly aligned for a
 *          pointer data type (@p align_t).
 *
 * @param[in] heapp pointer to a heap descriptor or @p NULL in order to access
 *                  the default heap.
 * @param[in] size the size of the block to be allocated. Note that the
 *                 allocated block may be a bit bigger than the requested
 *                 size for alignment and fragmentation reasons.
 * @return A pointer to the allocated block.
 * @retval NULL if the block cannot be allocated.
 */
void *chHeapAlloc(MemoryHeap *heapp, size_t size) {
  struct heap_header *qp, *hp, *fp;

  if (heapp == NULL)
    heapp = &default_heap;

  size = MEM_ALIGN_SIZE(size);
  qp = &heapp->h_free;
  H_LOCK(heapp);

  while (qp->h_next != NULL) {
    hp = qp->h_next;
    if (hp->h_size >= size) {
      if (hp->h_size < size + sizeof(struct heap_header)) {
        /*
         * Gets the whole block even if it is slightly bigger than the
         * requested size because the fragment would be too small to be
         * useful.
         */
        qp->h_next = hp->h_next;
      }
      else {
        /*
         * Block bigger enough, must split it.
         */
        fp = (void *)((uint8_t *)(hp) + sizeof(struct heap_header) + size);
        fp->h_next = hp->h_next;
        fp->h_size = hp->h_size - sizeof(struct heap_header) - size;
        qp->h_next = fp;
        hp->h_size = size;
      }
      hp->h_heap = heapp;

      H_UNLOCK(heapp);
      return (void *)(hp + 1);
    }
    qp = hp;
  }

  H_UNLOCK(heapp);

  /*
   * More memory is required, tries to get it from the associated provider.
   */
  if (heapp->h_provider) {
    hp = heapp->h_provider(size + sizeof(struct heap_header));
    if (hp != NULL) {
      hp->h_heap = heapp;
      hp->h_size = size;
      hp++;
      return (void *)hp;
    }
  }
  return NULL;
}

#define LIMIT(p) (struct heap_header *)((uint8_t *)(p) + \
                                        sizeof(struct heap_header) + \
                                        (p)->h_size)

/**
 * @brief Frees a previously allocated memory block.
 *
 * @param[in] p the memory block pointer
 */
void chHeapFree(void *p) {
  struct heap_header *qp, *hp;
  MemoryHeap *heapp;

  chDbgCheck(p != NULL, "chHeapFree");

  hp = (struct heap_header *)p - 1;
  heapp = hp->h_heap;
  qp = &heapp->h_free;
  H_LOCK(heapp);

  while (TRUE) {
    chDbgAssert((hp < qp) || (hp >= LIMIT(qp)),
                "chHeapFree(), #1",
                "within free block");

    if (((qp == &heapp->h_free) || (hp > qp)) &&
        ((qp->h_next == NULL) || (hp < qp->h_next))) {
      /*
       * Insertion after qp.
       */
      hp->h_next = qp->h_next;
      qp->h_next = hp;
      /*
       * Verifies if the newly inserted block should be merged.
       */
      if (LIMIT(hp) == hp->h_next) {
        /*
         * Merge with the next block.
         */
        hp->h_size += hp->h_next->h_size + sizeof(struct heap_header);
        hp->h_next = hp->h_next->h_next;
      }
      if ((LIMIT(qp) == hp)) {
        /*
         * Merge with the previous block.
         */
        qp->h_size += hp->h_size + sizeof(struct heap_header);
        qp->h_next = hp->h_next;
      }
      break;
    }
    qp = qp->h_next;
  }

  H_UNLOCK(heapp);
  return;
}

/**
 * @brief Reports the heap status.
 *
 * @param[in] heapp pointer to a heap descriptor or @p NULL in order to access
 *                  the default heap.
 * @param[in] sizep pointer to a variable that will receive the total
 *                  fragmented free space
 * @return The number of fragments in the heap.
 * @note This function is meant to be used in the test suite, it should not be
 *       really useful for the application code.
 * @note This function is not implemented when the @p CH_USE_MALLOC_HEAP
 *       configuration option is used (it always returns zero).
 */
size_t chHeapStatus(MemoryHeap *heapp, size_t *sizep) {
  struct heap_header *qp;
  size_t n, sz;

  if (heapp == NULL)
    heapp = &default_heap;

  H_LOCK(heapp);

  sz = 0;
  for (n = 0, qp = &heapp->h_free; qp->h_next; n++, qp = qp->h_next)
    sz += qp->h_next->h_size;
  if (sizep)
    *sizep = sz;

  H_UNLOCK(heapp);
  return n;
}

#else /* CH_USE_MALLOC_HEAP */

#include <stdlib.h>

#if CH_USE_MUTEXES
#define H_LOCK()        chMtxLock(&hmtx)
#define H_UNLOCK()      chMtxUnock()
static Mutex            hmtx;
#elif CH_USE_SEMAPHORES
#define H_LOCK()        chSemWait(&hsem)
#define H_UNLOCK()      chSemSignal(&hsem)
static Semaphore        hsem;
#endif

void heap_init(void) {

#if CH_USE_MUTEXES
  chMtxInit(&hmtx);
#else
  chSemInit(&hsem, 1);
#endif
}

void *chHeapAlloc(MemoryHeap *heapp, size_t size) {
  void *p;

  chDbgCheck(heapp == NULL, "chHeapAlloc");

  H_LOCK();
  p = malloc(size);
  H_UNLOCK();
  return p;
}

void chHeapFree(void *p) {

  chDbgCheck(p != NULL, "chHeapFree");

  H_LOCK();
  free(p);
  H_UNLOCK();
}

size_t chHeapStatus(MemoryHeap *heapp, size_t *sizep) {

  chDbgCheck(heapp == NULL, "chHeapStatus");

  if (sizep)
    *sizep = 0;
  return 0;
}

#endif /* CH_USE_MALLOC_HEAP */

#endif /* CH_USE_HEAP */

/** @} */
