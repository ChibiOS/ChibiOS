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

#include <stdio.h>

#include "ch.h"

static WORKING_AREA(waThread1, 2048);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    chThdSleepMilliseconds(1000);
    printf("-\n");
  }
  return 0;
}

/*------------------------------------------------------------------------*
 * Simulator main, start here your threads, examples inside.              *
 *------------------------------------------------------------------------*/
int main(void) {

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();

  /*
   * Starting threads.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  while (TRUE)
    chThdSleepSeconds(1);

  return 0;
}
