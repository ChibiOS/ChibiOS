/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    chregistry.h
 * @brief   Threads registry macros and structures.
 *
 * @addtogroup registry
 * @{
 */

#ifndef _CHREGISTRY_H_
#define _CHREGISTRY_H_

#if CH_USE_REGISTRY

/**
 * @brief   Removes a thread from the registry list.
 * @note    This macro is not meant for use in application code.
 *
 * @param[in] tp        thread to remove from the registry
 */
#define REG_REMOVE(tp) {                                                    \
  (tp)->p_older->p_newer = (tp)->p_newer;                                   \
  (tp)->p_newer->p_older = (tp)->p_older;                                   \
}

/**
 * @brief   Adds a thread to the registry list.
 * @note    This macro is not meant for use in application code.
 *
 * @param[in] tp        thread to add to the registry
 */
#define REG_INSERT(tp) {                                                    \
  (tp)->p_newer = (Thread *)&rlist;                                         \
  (tp)->p_older = rlist.r_older;                                            \
  (tp)->p_older->p_newer = rlist.r_older = (tp);                            \
}

#ifdef __cplusplus
extern "C" {
#endif
  Thread *chRegFirstThread(void);
  Thread *chRegNextThread(Thread *tp);
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_REGISTRY */

#endif /* _CHREGISTRY_H_ */

/** @} */
