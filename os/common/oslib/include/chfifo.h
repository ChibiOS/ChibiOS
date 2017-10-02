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
#define CH_CFG_USE_FIFO                     FALSE
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
  void *chFifoAllocObjectI(objects_fifo_t *ofp);
  void *chFifoAllocObjectTimeout(objects_fifo_t *ofp,
                                 systime_t timeout);
  msg_t chFifoReleaseObjectI(objects_fifo_t *ofp,
                             void *objp);
  msg_t chFifoReleaseObject(objects_fifo_t *ofp,
                            void *objp);
  msg_t chFifoPostObjectI(objects_fifo_t *ofp,
                          void *objp);
  msg_t chFifoPostObjectS(objects_fifo_t *ofp,
                          void *objp);
  msg_t chFifoPostObject(objects_fifo_t *ofp, void *obj);
  msg_t chFifoFetchObjectI(objects_fifo_t *ofp,
                           void **objpp);
  msg_t chFifoFetchObjectS(objects_fifo_t *ofp,
                           void **objpp);
  msg_t chFifoFetchObjectTimeout(objects_fifo_t *ofp,
                                 void **objpp,
                                 systime_t timeout);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Initializes a mail object.
 *
 * @param[out] mop      pointer to a @p mail_t structure
 *
 * @init
 */
static inline void chMailObjectInit(mail_t *mop) {

}

#endif /* CH_CFG_USE_FIFO == TRUE */

#endif /* CHFIFO_H */

/** @} */
