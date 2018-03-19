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
 *
 * @addtogroup pipes
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
 * @{
 */

#include "ch.h"

#if (CH_CFG_USE_PIPES == TRUE) || defined(__DOXYGEN__)

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
void chPipeReset(mailbox_t *mbp) {

  chSysLock();
  chMBResetI(mbp);
  chSchRescheduleS();
  chSysUnlock();
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
void chPipeResetI(pipe_t *pp) {

  chDbgCheckClassI();
  chDbgCheck(pipe_t != NULL);

  pipe_t->wrptr = pipe_t->buffer;
  pipe_t->rdptr = pipe_t->buffer;
  pipe_t->cnt   = (size_t)0;
  pipe_t->reset = true;
  chThdDequeueAllI(&pipe_t->qw, MSG_RESET);
  chThdDequeueAllI(&pipe_t->qr, MSG_RESET);
}

size_t chPipeReadI(pipe_t *pp, uint8_t *bp, size_t n) {

}

size_t chPipeReadTimeoutS(pipe_t *pp, uint8_t *bp,
                          size_t n, sysinterval_t timeout) {

}

size_t chPipeReadTimeout(pipe_t *pp, uint8_t *bp,
                         size_t n, sysinterval_t timeout) {

}

size_t chPipeWriteI(pipe_t *pp, const uint8_t *bp, size_t n) {

}

size_t chPipeWriteTimeoutS(pipe_t *pp, const uint8_t *bp,
                           size_t n, sysinterval_t timeout) {

}

size_t chPipeWriteTimeout(pipe_t *pp, const uint8_t *bp,
                          size_t n, sysinterval_t timeout) {

}

#endif /* CH_CFG_USE_MAILBOXES == TRUE */

/** @} */
