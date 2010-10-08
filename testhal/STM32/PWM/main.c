/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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

#include "ch.h"
#include "hal.h"

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  while (TRUE) {
    palClearPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palSetPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
}

static void pwmc1cb(PWMDriver *pwmp) {

  (void)pwmp;
}

static PWMConfig pwmcfg = {
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  (uint16_t)(STM32_TIMCLK2 / 10000 - 1),    /* 100 uS clock.*/
  (uint16_t)(10000 - 1),                    /* Period 1S.*/
  0
};

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {

  (void)argc;
  (void)argv;

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Initializes the PWM driver 1.
   */
  pwmStart(&PWMD1, &pwmcfg);
  palSetPadMode(IOPORT1, 8, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  chThdSleepMilliseconds(2000);

  /*
   * Starts the channel 0 with 50% duty cycle.
   */
  pwmEnableChannel(&PWMD1, 0, 4999); /* 50% */
  chThdSleepMilliseconds(5000);

  /*
   * Changes the channel 0 with 75% duty cycle.
   */
  pwmEnableChannel(&PWMD1, 0, 7499); /* 75% */
  chThdSleepMilliseconds(5000);

  /*
   * Disables channel 0.
   */
  pwmDisableChannel(&PWMD1, 0);


  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
