/*
    Nil RTOS - Copyright (C) 2012 Giovanni Di Sirio.

    This file is part of Nil RTOS.

    Nil RTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Nil RTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nil.h"
#include "hwinit.h"

/*
 * Thread 1.
 */
NIL_WORKING_AREA(waThread1, 128);
NIL_THREAD(Thread1, arg) {

  (void)arg;

  while (true) {
    gpioSetPad(GPIOC, GPIOC_LED4);
    nilThdSleepMilliseconds(500);
    gpioClearPad(GPIOC, GPIOC_LED4);
    nilThdSleepMilliseconds(500);
  }
}

/*
 * Thread 2.
 */
NIL_WORKING_AREA(waThread2, 128);
NIL_THREAD(Thread2, arg) {

  (void)arg;

  while (true) {
    gpioSetPad(GPIOC, GPIOC_LED3);
    nilThdSleepMilliseconds(250);
    gpioClearPad(GPIOC, GPIOC_LED3);
    nilThdSleepMilliseconds(250);
  }
}

/*
 * Threads static table, one entry per thread. The number of entries must
 * match NIL_CFG_NUM_THREADS.
 */
NIL_THREADS_TABLE_BEGIN()
  NIL_THREADS_TABLE_ENTRY("thread1", Thread1, NULL, waThread1, sizeof(waThread1))
  NIL_THREADS_TABLE_ENTRY("thread2", Thread2, NULL, waThread2, sizeof(waThread2))
NIL_THREADS_TABLE_END()

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations:
   * - HW specific initialization.
   * - Nil RTOS initialization.
   */
  hwInit();
  nilSysInit();

  /* This is now the idle thread loop, you may perform here a low priority
     task but you must never try to sleep or wait in this loop.*/
  while (true) {
  }
}
