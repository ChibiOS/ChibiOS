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
 * @file    mail.h
 * @brief   Threads mail macros and structures.
 *
 * @addtogroup mail
 * @{
 */

#ifndef _MAIL_H_
#define _MAIL_H_

/*
 * Module dependencies check.
 */
#if !CH_USE_SEMAPHORES || !CH_USE_MEMPOOLS
#error "Mail Pools require CH_USE_SEMAPHORES and CH_USE_MEMPOOLS"
#endif

/**
 * @brief   Mail Pool descriptor.
 */
typedef struct {
  MemoryPool            pool;           /**< @brief Available mail objects. */
  Semaphore             sem;            /**< @brief Semaphore guard.        */
} MailPool;

#ifdef __cplusplus
extern "C" {
#endif
  void mailInit(MailPool *mlp, size_t size, void *p, size_t n);
  void *mailCreate(MailPool *mlp, systime_t time);
  void mailDelete(MailPool *mlp, void *mailp);
#ifdef __cplusplus
}
#endif

#endif /* _MAIL_H_ */

/** @} */
