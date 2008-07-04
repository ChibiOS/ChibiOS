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
#include <test.h>

#include "board.h"
#include "stm32_serial.h"

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WorkingArea(waThread1, 128);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    GPIOC->BRR = GPIOC_LED;
    chThdSleep(500);
    GPIOC->BSRR = GPIOC_LED;
    chThdSleep(500);
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
  chThdCreateFast(NORMALPRIO, waThread1, sizeof(waThread1), Thread1);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (TRUE) {
    if (GPIOA->IDR & GPIOA_BUTTON)
      TestThread(&COM2);
    chThdSleep(500);
  }
  return 0;
}
