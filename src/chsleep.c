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

/**
 * Suspends the invoking thread for the specified time.
 * @param time the system ticks number
 */
void chThdSleep(systime_t time) {

  chSysLock();
  chSchGoSleepTimeoutS(PRSLEEP, time);
  chSysUnlock();
}

#ifdef CH_USE_SYSTEMTIME
/**
 * Checks if the current system time is within the specified time window.
 * @param start the start of the time window (inclusive)
 * @param end the end of the time window (non inclusive)
 */
bool_t chSysInTimeWindow(systime_t start, systime_t end) {

  systime_t time = chSysGetTime();
  return end >= start ? (time >= start) && (time < end) :
                        (time >= start) || (time < end);
}
#endif /* CH_USE_SYSTEMTIME */

/** @} */
