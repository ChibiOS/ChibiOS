/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file    chregistry.c
 * @brief   Threads registry code.
 *
 * @addtogroup registry
 * @{
 */
#include "ch.h"

#if CH_USE_REGISTRY

/**
 * @brief   Returns the first thread in the system.
 * @details Returns the most ancient thread in the system, usually this is
 *          the main thread unless it terminated.
 * @note    A reference is added to the returned thread in order to make sure
 *          it status is not lost.
 * @note    This function cannot return @p NULL because there is always at
 *          least one thread in the system.
 *
 * @return              A reference to the first thread.
 */
Thread *chRegFirstThread(void) {
  Thread *tp;

  chSysLock();
  tp = rlist.p_newer;
#if CH_USE_DYNAMIC
  tp->p_refs++;
#endif
  chSysUnlock();
  return tp;
}

/**
 * @brief   Returns the thread next to the specified one.
 * @details The reference counter of the specified thread is decremented and
 *          the reference counter of the returned thread is incremented.
 *
 * @param[in] tp        pointer to the thread
 * @return              A reference to the next thread.
 * @retval NULL         if there is no next thread.
 */
Thread *chRegNextThread(Thread *tp) {

  chSysLock();
#if CH_USE_DYNAMIC
  chDbgAssert(tp->p_refs > 0, "chRegNextThread(), #1",
              "not referenced");
  tp->p_refs--;
#endif
  if (tp->p_newer != (Thread *)&rlist) {
    tp = tp->p_newer;
#if CH_USE_DYNAMIC
    chDbgAssert(tp->p_refs < 255, "chRegNextThread(), #2",
                "too many references");
    tp->p_refs++;
#endif
  }
  else
    tp = NULL;
  chSysUnlock();
  return tp;
}

#endif /* CH_USE_REGISTRY */

/** @} */
