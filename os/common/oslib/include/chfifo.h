/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio.

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
 * @file    chfifo.h
 * @brief   Objects FIFO structures and macros.
 *
 * @addtogroup objects_fifo
 * @{
 */

#ifndef CHFIFO_H
#define CHFIFO_H

#if !defined(CH_CFG_USE_FIFO)
#define CH_CFG_USE_FIFO                     TRUE
#endif

#if (CH_CFG_USE_FIFO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if CH_CFG_USE_MEMPOOLS == FALSE
#error "CH_CFG_USE_FIFO requires CH_CFG_USE_MEMPOOLS"
#endif

#if CH_CFG_USE_SEMAPHORES == FALSE
#error "CH_CFG_USE_FIFO requires CH_CFG_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an objects FIFO.
 */
typedef struct {
  /**
   * @brief   Pool of the free objects.
   */
  guarded_memory_pool_t     free;
  /**
   * @brief   Mailbox of the sent objects.
   */
  mailbox_t                 mbx;
} objects_fifo_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Initializes a mail object.
 *
 * @param[out] ofp      pointer to a @p objects_fifo_t structure
 * @param[in] objsize   size of objects
 * @param[in] objn      number of objects available
 * @param[in] objbuf    pointer to the buffer of objects, it must be able
 *                      to hold @p objn objects of @p objsize size
 * @param[in] msgbuf    pointer to the buffer of messages, it must be able
 *                      to hold @p objn messages
 *
 * @init
 */
static inline void chMailObjectInit(objects_fifo_t *ofp, size_t objsize,
                                    size_t objn, void *objbuf,
                                    msg_t *msgbuf) {

  chGuardedPoolObjectInit(&ofp->free, objsize);
  chGuardedPoolLoadArray(&ofp->free, objbuf, objn);
  chMBObjectInit(&ofp->mbx, msgbuf, (cnt_t)objn); /* TODO: make this a size_t, no more sems there.*/
}

/**
 * @brief   Allocates a free object.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @return              The pointer to the allocated object.
 * @retval NULL         if an object is not immediately available.
 *
 * @iclass
 */
static inline void *chFifoAllocObjectI(objects_fifo_t *ofp) {

  return chGuardedPoolAllocI(&ofp->free);
}

/**
 * @brief   Allocates a free object.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The pointer to the allocated object.
 * @retval NULL         if an object is not available within the specified
 *                      timeout.
 *
 * @iclass
 */
static inline void *chFifoAllocObjectTimeout(objects_fifo_t *ofp,
                                             systime_t timeout) {

  return chGuardedPoolAllocTimeout(&ofp->free, timeout);
}

/**
 * @brief   Releases a fetched object.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @param[in] objp      pointer to the object to be released
 *
 * @iclass
 */
static inline void chFifoReleaseObjectI(objects_fifo_t *ofp,
                                        void *objp) {

  chGuardedPoolFreeI(&ofp->free, objp);
}

/**
 * @brief   Releases a fetched object.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @param[in] objp      pointer to the object to be released
 *
 * @api
 */
static inline void chFifoReleaseObject(objects_fifo_t *ofp,
                                       void *objp) {

  chGuardedPoolFree(&ofp->free, objp);
}

/**
 * @brief   Posts an object.
 * @note    By design the object can be always immediately posted.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @param[in] objp      pointer to the object to be released
 *
 * @iclass
 */
static inline void chFifoPostObjectI(objects_fifo_t *ofp,
                                     void *objp) {
  msg_t msg;

  msg = chMBPostI(&ofp->mbx, (msg_t)objp);
  chDbgAssert(msg == MSG_OK, "post failed");
}

/**
 * @brief   Posts an object.
 * @note    By design the object can be always immediately posted.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @param[in] objp      pointer to the object to be released
 *
 * @sclass
 */
static inline void chFifoPostObjectS(objects_fifo_t *ofp,
                                      void *objp) {
  msg_t msg;

  msg = chMBPostS(&ofp->mbx, (msg_t)objp, TIME_IMMEDIATE);
  chDbgAssert(msg == MSG_OK, "post failed");
}

/**
 * @brief   Posts an object.
 * @note    By design the object can be always immediately posted.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @param[in] objp      pointer to the object to be released
 *
 * @api
 */
static inline void chFifoPostObject(objects_fifo_t *ofp, void *objp) {

  msg_t msg;

  msg = chMBPost(&ofp->mbx, (msg_t)objp, TIME_IMMEDIATE);
  chDbgAssert(msg == MSG_OK, "post failed");
}

/**
 * @brief   Fetches an object.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @param[in] objpp     pointer to the fetched object reference
 * @return              The operation status.
 * @retval MSG_OK       if an object has been correctly fetched.
 * @retval MSG_TIMEOUT  if the FIFO is empty and a message cannot be fetched.
 *
 * @api
 */
static inline msg_t chFifoFetchObjectI(objects_fifo_t *ofp,
                                       void **objpp) {

  return chMBFetchI(&ofp->mbx, (msg_t *)objpp);
}

/**
 * @brief   Fetches an object.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @param[in] objpp     pointer to the fetched object reference
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if an object has been correctly fetched.
 * @retval MSG_TIMEOUT  if the operation has timed out.
 *
 * @sclass
 */
static inline msg_t chFifoFetchObjectTimeoutS(objects_fifo_t *ofp,
                                              void **objpp,
                                              systime_t timeout) {

  return chMBFetchS(&ofp->mbx, (msg_t *)objpp, timeout);
}

/**
 * @brief   Fetches an object.
 *
 * @param[in] ofp       pointer to a @p objects_fifo_t structure
 * @param[in] objpp     pointer to the fetched object reference
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if an object has been correctly fetched.
 * @retval MSG_TIMEOUT  if the operation has timed out.
 *
 * @api
 */
static inline msg_t chFifoFetchObjectTimeout(objects_fifo_t *ofp,
                                             void **objpp,
                                             systime_t timeout) {

  return chMBFetch(&ofp->mbx, (msg_t *)objpp, timeout);
}
#endif /* CH_CFG_USE_FIFO == TRUE */

#endif /* CHFIFO_H */

/** @} */
