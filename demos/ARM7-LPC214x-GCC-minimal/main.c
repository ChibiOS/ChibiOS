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

#include <ch.h>

#include "lpc214x.h"

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WorkingArea(waThread1, 64);
static t_msg Thread1(void *arg) {

  while (TRUE) {
    IO0CLR = 0x00000800;
    chThdSleep(200);
    IO0SET = 0x00000C00;
    chThdSleep(800);
    IO0CLR = 0x00000400;
    chThdSleep(200);
    IO0SET = 0x00000C00;
    chThdSleep(800);
  }
  return 0;
}

/*
 * Yellow LED blinker thread, times are in milliseconds.
 */
static WorkingArea(waThread2, 64);
static t_msg Thread2(void *arg) {

  while (TRUE) {
    IO0CLR = 0x80000000;
    chThdSleep(200);
    IO0SET = 0x80000000;
    chThdSleep(300);
  }
  return 0;
}

/*
 * Entry point, the interrupts are disabled on entry.
 */
int main(int argc, char **argv) {

  /*
   * The main() function becomes a thread here then the interrupts are
   * enabled and ChibiOS/RT goes live.
   */
  chSysInit();

  /*
   * Creates the blinker threads.
   */
  chThdCreate(NORMALPRIO, 0, waThread1, sizeof(waThread1), Thread1, NULL);
  chThdCreate(NORMALPRIO, 0, waThread2, sizeof(waThread2), Thread2, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop.
   */
  while (TRUE)
    chThdSleep(1000);
  return 0;
}
