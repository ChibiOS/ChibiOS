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
/**
 * Suspends the invoking thread for the specified time.
 * @param time the system ticks number
 */
void chThdSleep(t_time time) {
  VirtualTimer vt;

  chSysLock();

  chVTSetI(&vt, time, (t_vtfunc)chSchReadyI, currp);
  chSchGoSleepI(PRSLEEP);

  chSysUnlock();
}

#ifdef CH_USE_SYSTEMTIME
/**
 * Returns the number of system ticks since the \p chSysInit() invocation.
 * @return the system ticks number
 * @note The counter can reach its maximum and return to zero.
 * @note This function is designed to work with the \p chThdSleepUntil().
 * @note The function is available only if the \p CH_USE_SYSTEMTIME
 *       option is enabled in \p chconf.h.
 */
t_time chSysGetTime(void) {

  return stime;
}

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

  chVTSetI(&vt, (t_time)(time - stime), (t_vtfunc)chSchReadyI, currp);
  chSchGoSleepI(PRSLEEP);

  chSysUnlock();
}
#endif /* CH_USE_SYSTEMTIME */

#endif /* CH_USE_SLEEP */

/** @} */

