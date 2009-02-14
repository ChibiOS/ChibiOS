/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @addtogroup Heap
 * @{
 */

#include <ch.h>

#ifdef CH_USE_HEAP

#ifndef CH_USE_MALLOC_HEAP

#define MAGIC 0xF5A0
#define ALIGN_TYPE      void *
#define ALIGN_MASK      (sizeof(ALIGN_TYPE) - 1)
#define ALIGN_SIZE(p)   (((size_t)(p) + ALIGN_MASK) & ~ALIGN_MASK)

struct header {
  union {
    struct header       *h_next;
    size_t              h_magic;
  };
  size_t                h_size;
};

static struct {
  struct header         free;   /* Guaranteed to be not adjacent to the heap */
#if defined(CH_USE_MUTEXES)
#define H_LOCK()        chMtxLock(&heap.hmtx)
#define H_UNLOCK()      chMtxUnlock()
  Mutex                 hmtx;
#elif defined(CH_USE_SEMAPHORES)
#define H_LOCK()        chSemWait(&heap.hsem)
#define H_UNLOCK()      chSemSignal(&heap.hsem)
  Semaphore             hsem;
#else
#error "The heap allocator requires mutexes or semaphores to be enabled"
#endif
#if CH_HEAP_SIZE > 0
  union {
    ALIGN_TYPE          alignment;
    char                buffer[ALIGN_SIZE(CH_HEAP_SIZE)];
  };
#endif
} heap;

/**
 * Initializes the allocator subsystem.
 * @note It is internally invoked, this function should not normally be
 *       invoked from the user code.
 */
void chHeapInit(void) {
  struct header *hp;

#if CH_HEAP_SIZE == 0
  extern char __heap_base__;
  extern char __heap_end__;

  hp = (void *)&__heap_base__;
  hp->h_size = &__heap_end__ - &__heap_base__ - sizeof(struct header);
#else
  hp = (void *)&heap.buffer[0];
  hp->h_size = (&heap.buffer[ALIGN_SIZE(CH_HEAP_SIZE)] - &heap.buffer[0]) -
               sizeof(struct header);
#endif
  hp->h_next = NULL;
  heap.free.h_next = hp;
  heap.free.h_size = 0;
#if defined(CH_USE_MUTEXES)
  chMtxInit(&heap.hmtx);
#else
  chSemInit(&heap.hsem, 1);
#endif
}

/**
 * Allocates a block of memory from the heap by using the first-fit algorithm.
 * The allocated block is guaranteed to be properly aligned for a pointer data
 * type.
 * @param size the size of the block to be allocated. Note that the allocated
 *             block may be a bit bigger than the requested size for alignment
 *             and fragmentation reasons.
 * @return A pointer to the allocated block.
 * @retval NULL if the block cannot be allocated.
 */
void *chHeapAlloc(size_t size) {
  struct header *qp, *hp, *fp;

  size = ALIGN_SIZE(size);
  qp = &heap.free;
  H_LOCK();

  while (qp->h_next != NULL) {
    hp = qp->h_next;
    if (hp->h_size >= size) {
      if (hp->h_size < size + sizeof(struct header)) {
        /* Gets the whole block even if it is slightly bigger than the
           requested size because the fragment would be too small to be
           useful */
        qp->h_next = hp->h_next;
      }
      else {
        /* Block bigger enough, must split it */
        fp = (void *)((char *)(hp) + sizeof(struct header) + size);
        fp->h_next = hp->h_next;
        fp->h_size = hp->h_size - sizeof(struct header) - size;
        qp->h_next = fp;
        hp->h_size = size;
      }
      hp->h_magic = MAGIC;

      H_UNLOCK();
      return (void *)(hp + 1);
    }
    qp = hp;
  }

  H_UNLOCK();
  return NULL;
}

#define LIMIT(p) (struct header *)((char *)(p) + \
                                   sizeof(struct header) + \
                                   (p)->h_size)

/**
 * Frees a previously allocated memory block.
 * @param p the memory block pointer
 */
void chHeapFree(void *p) {
  struct header *qp, *hp;

  hp = (struct header *)p - 1;

  chDbgAssert(hp->h_magic == MAGIC, "chheap.c, chHeapFree() #1");

  qp = &heap.free;
  H_LOCK();

  while (TRUE) {

    chDbgAssert((hp < qp) || (hp >= LIMIT(qp)), "chheap.c, chHeapFree() #2");

    if (((qp == &heap.free) || (hp > qp)) &&
        ((qp->h_next == NULL) || (hp < qp->h_next))) {
      /* Insertion after qp */
      hp->h_next = qp->h_next;
      qp->h_next = hp;
      /* Verifies if the newly inserted block should be merged */
      if (LIMIT(hp) == hp->h_next) {
        /* Merge with the next block */
        hp->h_size += hp->h_next->h_size + sizeof(struct header);
        hp->h_next = hp->h_next->h_next;
      }
      if ((LIMIT(qp) == hp)) {  /* Cannot happen when qp == &heap.free */
        /* Merge with the previous block */
        qp->h_size += hp->h_size + sizeof(struct header);
        qp->h_next = hp->h_next;
      }

      H_UNLOCK();
      return;
    }
    qp = qp->h_next;
  }
}

/**
 * Determines the heap status.
 * @param sizep pointer to a variable that will receive the total fragmented
 *              free space
 * @return The number of fragments in the heap.
 * @note This function is meant to be used in the test suite, it should not be
 *       really useful for the application code.
 * @note This function is not implemented when the \p CH_USE_MALLOC_HEAP
 *       configuration option is used (it always returns zero).
 */
size_t chHeapStatus(size_t *sizep) {
  struct header *qp;
  size_t n, sz;

  H_LOCK();

  sz = 0;
  for (n = 0, qp = &heap.free; qp->h_next; n++, qp = qp->h_next)
    sz += qp->h_next->h_size;
  if (sizep)
    *sizep = sz;

  H_UNLOCK();
  return n;
}

#else /* CH_USE_MALLOC_HEAP */

#include <stdlib.h>

#if defined(CH_USE_MUTEXES)
#define H_LOCK()        chMtxLock(&hmtx)
#define H_UNLOCK()      chMtxLock(&hmtx)
static Mutex            hmtx;
#elif defined(CH_USE_SEMAPHORES)
#define H_LOCK()        chSemWait(&hsem)
#define H_UNLOCK()      chSemSignal(&hsem)
static Semaphore        hsem;
#else
#error "The heap allocator requires mutexes or semaphores to be enabled"
#endif

void chHeapInit(void) {

#if defined(CH_USE_MUTEXES)
  chMtxInit(&hmtx);
#else
  chSemInit(&hsem, 1);
#endif
}

void *chHeapAlloc(size_t size) {
  void *p;

  H_LOCK();

  p = malloc(size);

  H_UNLOCK();
  return p;
}

void chHeapFree(void *p) {

  H_LOCK();

  free(p);

  H_UNLOCK();
}

size_t chHeapStatus(size_t *sizep) {

  if (sizep)
    *sizep = 0;
  return 0;
}

#endif /* CH_USE_MALLOC_HEAP */

#endif /* CH_USE_HEAP */

/** @} */
