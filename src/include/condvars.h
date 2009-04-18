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
/*
   Concepts and parts of this file are contributed by and Copyright (C) 2008
   of Leon Woestenberg.
 */

/**
 * @file condvars.h
 * @brief Condition Variables macros and structures.
 * @addtogroup CondVars
 * @{
 */

#ifndef _CONDVARS_H_
#define _CONDVARS_H_

#if CH_USE_CONDVARS && CH_USE_MUTEXES

/**
 * @brief CondVar structure.
 */
typedef struct CondVar {
  ThreadsQueue          c_queue;        /**< CondVar threads queue.*/
} CondVar;

#ifdef __cplusplus
extern "C" {
#endif
  void chCondInit(CondVar *cp);
  void chCondSignal(CondVar *cp);
  void chCondSignalI(CondVar *cp);
  void chCondBroadcast(CondVar *cp);
  void chCondBroadcastI(CondVar *cp);
  msg_t chCondWait(CondVar *cp);
  msg_t chCondWaitS(CondVar *cp);
#if CH_USE_CONDVARS_TIMEOUT
  msg_t chCondWaitTimeout(CondVar *cp, systime_t time);
  msg_t chCondWaitTimeoutS(CondVar *cp, systime_t time);
#endif
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_CONDVARS && CH_USE_MUTEXES */

#endif /* _CONDVARS_H_ */

/** @} */
