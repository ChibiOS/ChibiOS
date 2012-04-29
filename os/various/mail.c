/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    mail.c
 * @brief   Threads mail code.
 *
 * @addtogroup mail
 * @{
 */

#include "ch.h"
#include "mail.h"

/**
 * @brief   Initializes a Mail Pool.
 * @note    The number of the mail objects in the mail pool should be at
 *          least <b>2+size(mailbox)</b>, this considering one writer and
 *          one reader, add one element for each extra reader or writer in
 *          order to avoid waiting on the mail pool. A smaller number of
 *          elements can be specified if waiting on the pool is acceptable.
 *
 * @param[out] mlp      pointer to a @p MailPool structure
 * @param[in] size      the size of the mail objects to be placed in the pool
 * @param[in] p         pointer to the mail objects array first element
 * @param[in] n         number of elements in the mail objects array
 *
 * @init
 */
void mailInit(MailPool *mlp, size_t size, void *p, size_t n) {

  chPoolInit(&mlp->pool, size, NULL);
  chPoolLoadArray(&mlp->pool, p, n);
  chSemInit(&mlp->sem, (cnt_t)n);
}

/**
 * @brief   Allocates a mail object from a mail pool.
 * @pre     The mail pool must be already been initialized.
 *
 * @param[in] mlp       pointer to a @p MailPool structure
 * @param[in] time      the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The mail object.
 * @retval NULL         timeout expired.
 *
 * @api
 */
void *mailCreate(MailPool *mlp, systime_t time) {
  msg_t msg;
  void *mailp;

  msg = chSemWaitTimeout(&mlp->sem, time);
  if (msg != RDY_OK)
    return NULL;
  mailp = chPoolAlloc(&mlp->pool);
  chDbgAssert(mailp != NULL, "mailCreate(), #1", "empty pool");
  return mailp;
}

/**
 * @brief   Releases a mail object into a mail pool.
 * @pre     The mail pool must be already been initialized.
 *
 * @param[in] mlp       pointer to a @p MailPool structure
 * @param[in] mailp     the pointer to the mail object to be released
 *
 * @api
 */
void mailDelete(MailPool *mlp, void *mailp) {

  chPoolFree(&mlp->pool, mailp);
  chSemSignal(&mlp->sem);
}

/** @} */
