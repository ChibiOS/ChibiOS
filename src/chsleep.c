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
 * @addtogroup Time
 * @{
 */

#include <ch.h>

#ifdef CH_USE_SLEEP
static void wakeup(void *p) {

  chDbgAssert(((Thread *)p)->p_state == PRSLEEP, "chsleep.c, wakeup()");
  chSchReadyI(p, RDY_OK);
}

/**
 * Suspends the invoking thread for the specified time.
 * @param time the system ticks number
 */
void chThdSleep(t_time time) {
  VirtualTimer vt;

  chSysLock();

  chVTSetI(&vt, time, wakeup, currp);
  chSchGoSleepS(PRSLEEP);

  chSysUnlock();
}

#ifdef CH_USE_SYSTEMTIME
/**
 * Suspends the invoking thread until the system time arrives to the specified
 * value.
 * @param time the system time
 * @note The function is available only if the \p CH_USE_SYSTEMTIME
 *       option is enabled in \p chconf.h.
 */
void chThdSleepUntil(t_time time) {
  VirtualTimer vt;

  chSysLock();

  chVTSetI(&vt, (t_time)(time - rlist.r_stime), wakeup, currp);
  chSchGoSleepS(PRSLEEP);

  chSysUnlock();
}
#endif /* CH_USE_SYSTEMTIME */

#endif /* CH_USE_SLEEP */

/** @} */
