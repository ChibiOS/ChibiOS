/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

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
 * @file    chpipes.h
 * @brief   Pipes macros and structures.
 *
 * @addtogroup oslib_pipes
 * @{
 */

#ifndef CHPIPES_H
#define CHPIPES_H

#if (CH_CFG_USE_PIPES == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Structure representing a pipe object.
 */
typedef struct {
  uint8_t               *buffer;        /**< @brief Pointer to the pipe
                                                    buffer.                 */
  uint8_t               *top;           /**< @brief Pointer to the location
                                                    after the buffer.       */
  uint8_t               *wrptr;         /**< @brief Write pointer.          */
  uint8_t               *rdptr;         /**< @brief Read pointer.           */
  size_t                cnt;            /**< @brief Messages in queue.      */
  bool                  reset;          /**< @brief True if in reset state. */
  threads_queue_t       qw;             /**< @brief Queued writers.         */
  threads_queue_t       qr;             /**< @brief Queued readers.         */
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
  mutex_t               mtx;        /**< @brief Heap access mutex.          */
#else
  semaphore_t           sem;        /**< @brief Heap access semaphore.      */
#endif
} pipe_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Data part of a static pipe initializer.
 * @details This macro should be used when statically initializing a
 *          pipe that is part of a bigger structure.
 *
 * @param[in] name      the name of the pipe variable
 * @param[in] buffer    pointer to the pipe buffer array of @p uint8_t
 * @param[in] size      number of @p uint8_t elements in the buffer array
 */
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
#define _PIPE_DATA(name, buffer, size) {                                    \
  (uint8_t *)(buffer),                                                      \
  (uint8_t *)(buffer) + size,                                               \
  (uint8_t *)(buffer),                                                      \
  (uint8_t *)(buffer),                                                      \
  (size_t)0,                                                                \
  false,                                                                    \
  _THREADS_QUEUE_DATA(name.qw),                                             \
  _THREADS_QUEUE_DATA(name.qr),                                             \
  _MUTEX_DATA(name.mtx),                                                    \
}
#else /* CH_CFG_USE_MUTEXES == FALSE */
#define _PIPE_DATA(name, buffer, size) {                                    \
  (uint8_t *)(buffer),                                                      \
  (uint8_t *)(buffer) + size,                                               \
  (uint8_t *)(buffer),                                                      \
  (uint8_t *)(buffer),                                                      \
  (size_t)0,                                                                \
  false,                                                                    \
  _THREADS_QUEUE_DATA(name.qw),                                             \
  _THREADS_QUEUE_DATA(name.qr),                                             \
  _SEMAPHORE_DATA(name.sem, (cnt_t)1),                                      \
}
#endif /* CH_CFG_USE_MUTEXES == FALSE */

/**
 * @brief   Static pipe initializer.
 * @details Statically initialized pipes require no explicit
 *          initialization using @p chPipeObjectInit().
 *
 * @param[in] name      the name of the pipe variable
 * @param[in] buffer    pointer to the pipe buffer array of @p uint8_t
 * @param[in] size      number of @p uint8_t elements in the buffer array
 */
#define PIPE_DECL(name, buffer, size)                                       \
  pipe_t name = _PIPE_DATA(name, buffer, size)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void chPipeObjectInit(pipe_t *pp, uint8_t *buf, size_t n);
  void chPipeReset(pipe_t *pp);
  size_t chPipeWriteTimeout(pipe_t *pp, const uint8_t *bp,
                            size_t n, sysinterval_t timeout);
  size_t chPipeReadTimeout(pipe_t *pp, uint8_t *bp,
                           size_t n, sysinterval_t timeout);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Returns the pipe buffer size as number of bytes.
 *
 * @param[in] pp        the pointer to an initialized @p pipe_t object
 * @return              The size of the pipe.
 *
 * @iclass
 */
static inline size_t chPipeGetSizeI(const pipe_t *pp) {

  /*lint -save -e9033 [10.8] Perfectly safe pointers
    arithmetic.*/
  return (size_t)(pp->top - pp->buffer);
  /*lint -restore*/
}

/**
 * @brief   Returns the number of used byte slots into a pipe.
 *
 * @param[in] pp        the pointer to an initialized @p pipe_t object
 * @return              The number of queued bytes.
 *
 * @iclass
 */
static inline size_t chPipeGetUsedCountI(const pipe_t *pp) {

  chDbgCheckClassI();

  return pp->cnt;
}

/**
 * @brief   Returns the number of free byte slots into a pipe.
 *
 * @param[in] pp        the pointer to an initialized @p pipe_t object
 * @return              The number of empty byte slots.
 *
 * @iclass
 */
static inline size_t chPipeGetFreeCountI(const pipe_t *pp) {

  chDbgCheckClassI();

  return chPipeGetSizeI(pp) - chPipeGetUsedCountI(pp);
}

/**
 * @brief   Returns the next byte in the queue without removing it.
 * @pre     A byte must be present in the queue for this function to work
 *          or it would return garbage. The correct way to use this macro is
 *          to use @p chPipeGetFullCountI() and then use this macro, all within
 *          a lock state.
 *
 * @param[in] pp        the pointer to an initialized @p pipe_t object
 * @return              The next byte in queue.
 *
 * @iclass
 */
static inline uint8_t chPipePeekI(const pipe_t *pp) {

  chDbgCheckClassI();

  return *pp->rdptr;
}

/**
 * @brief   Terminates the reset state.
 *
 * @param[in] pp        the pointer to an initialized @p pipe_t object
 *
 * @xclass
 */
static inline void chPipeResumeX(pipe_t *pp) {

  pp->reset = false;
}

#endif /* CH_CFG_USE_PIPES == TRUE */

#endif /* CHPIPES_H */

/** @} */
