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
 * @file    nil/include/chevt.h
 * @brief   Nil RTOS events header file.
 *
 * @addtogroup NIL_EVENTS
 * @{
 */

#ifndef CHEVT_H
#define CHEVT_H

#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Events related macros
 * @{
 */
/**
 * @brief   All events allowed mask.
 */
#define ALL_EVENTS              ((eventmask_t)-1)

/**
 * @brief   Returns an event mask from an event identifier.
 */
#define EVENT_MASK(eid)         ((eventmask_t)(1 << (eid)))
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void chEvtSignal(thread_t *tp, eventmask_t mask);
  void chEvtSignalI(thread_t *tp, eventmask_t mask);
  eventmask_t chEvtWaitAnyTimeout(eventmask_t mask, sysinterval_t timeout);
  eventmask_t chEvtWaitAllTimeout(eventmask_t mask, sysinterval_t timeout);
#ifdef __cplusplus
}
#endif

#endif /* CH_CFG_USE_EVENTS == TRUE */

#endif /* CHEVT_H */

/** @} */
