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
 * @file    chpipes.c
 * @brief   Pipes code.
 * @details Byte pipes.
 *          <h2>Operation mode</h2>
 *          A pipe is an asynchronous communication mechanism.<br>
 *          Operations defined for mailboxes:
 *          - <b>Write</b>: Writes a buffer of data in the pipe in FIFO order.
 *          - <b>Read</b>: A buffer of data is read from the read and removed.
 *          - <b>Reset</b>: The pipe is emptied and all the stored data
 *            is lost.
 *          .
 * @pre     In order to use the pipes APIs the @p CH_CFG_USE_PIPES
 *          option must be enabled in @p chconf.h.
 * @note    Compatible with RT and NIL.
 *
 * @addtogroup oslib_pipes
 * @{
 */

#include "ch.h"

#if (CH_CFG_USE_PIPES == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*
 * Defaults on the best synchronization mechanism available.
 */
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
#define P_LOCK(p)       chMtxLock(&(p)->mtx)
#define P_UNLOCK(p)     chMtxUnlock(&(p)->mtx)
#else
#define P_LOCK(p)       (void) chSemWait(&(p)->sem)
#define P_UNLOCK(p)     chSemSignal(&(p)->sem)
#endif

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes a @p mailbox_t object.
 *
 * @param[out] pp       the pointer to the @p pipe_t structure to be
 *                      initialized
 * @param[in] buf       pointer to the pipe buffer as an array of @p uint8_t
 * @param[in] n         number of elements in the buffer array
 *
 * @init
 */
void chPipeObjectInit(pipe_t *pp, uint8_t *buf, size_t n) {

  chDbgCheck((pp != NULL) && (buf != NULL) && (n > (size_t)0));

  pp->buffer = buf;
  pp->rdptr  = buf;
  pp->wrptr  = buf;
  pp->top    = &buf[n];
  pp->cnt    = (size_t)0;
  pp->reset  = false;
  chThdQueueObjectInit(&pp->qw);
  chThdQueueObjectInit(&pp->qr);
}

/**
 * @brief   Resets a @p pipe_t object.
 * @details All the waiting threads are resumed with status @p MSG_RESET and
 *          the queued data is lost.
 * @post    The pipe is in reset state, all operations will fail and
 *          return @p MSG_RESET until the mailbox is enabled again using
 *          @p chPipeResumeX().
 *
 * @param[in] pp        the pointer to an initialized @p pipe_t object
 *
 * @api
 */
void chPipeReset(pipe_t *pp) {

  chDbgCheck(pp != NULL);

  P_LOCK(pp);
  chSysLock();
  pipe_t->wrptr = pipe_t->buffer;
  pipe_t->rdptr = pipe_t->buffer;
  pipe_t->cnt   = (size_t)0;
  pipe_t->reset = true;
  chThdDequeueAllI(&pipe_t->qw, MSG_RESET);
  chThdDequeueAllI(&pipe_t->qr, MSG_RESET);
  chSchRescheduleS();
  chSysUnlock();
  P_UNLOCK();
}

/**
 * @brief   Pipe write with timeout.
 * @details The function writes data from a buffer to a pipe. The
 *          operation completes when the specified amount of data has been
 *          transferred or after the specified timeout or if the pipe has
 *          been reset.
 *
 * @param[in] pp        the pointer to an initialized @p pipe_t object
 * @param[in] bp        pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred, the
 *                      value 0 is reserved
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The number of bytes effectively transferred.
 * @retval MSG_RESET    if the mailbox has been reset.
 * @retval MSG_TIMEOUT  if the operation has timed out.
 *
 * @api
 */
size_t chPipeWriteTimeout(pipe_t *pp, const uint8_t *bp,
                          size_t n, sysinterval_t timeout) {

}

/**
 * @brief   Pipe read with timeout.
 * @details The function reads data from a pipe into a buffer. The
 *          operation completes when the specified amount of data has been
 *          transferred or after the specified timeout or if the pipe has
 *          been reset.
 *
 * @param[in] pp        the pointer to an initialized @p pipe_t object
 * @param[out] bp        pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred, the
 *                      value 0 is reserved
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The number of bytes effectively transferred.
 * @retval MSG_RESET    if the mailbox has been reset.
 * @retval MSG_TIMEOUT  if the operation has timed out.
 *
 * @api
 */
size_t chPipeReadTimeout(pipe_t *pp, uint8_t *bp,
                         size_t n, sysinterval_t timeout) {

}

#endif /* CH_CFG_USE_MAILBOXES == TRUE */

/** @} */
