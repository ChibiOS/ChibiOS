/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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

static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
  palSetPad(IOPORT3, GPIOC_LED);
}

static void pwmc1cb(PWMDriver *pwmp) {

  (void)pwmp;
  palClearPad(IOPORT3, GPIOC_LED);
}

static PWMConfig pwmcfg = {
  10000,                                    /* 10KHz PWM clock frequency.   */
  10000,                                    /* Initial PWM period 1S.       */
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0
};

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * LED initially off.
   */
  palSetPad(IOPORT3, GPIOC_LED);

  /*
   * Initializes the PWM driver 1.
   */
  pwmStart(&PWMD1, &pwmcfg);
  palSetPadMode(IOPORT1, 8, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
  chThdSleepMilliseconds(2000);

  /*
   * Starts the channel 0 using 25% duty cycle.
   */
  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 2500));
  chThdSleepMilliseconds(5000);

  /*
   * Changes the channel 0 to 75% duty cycle.
   */
  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 7500));
  chThdSleepMilliseconds(5000);

  /*
   * Changes PWM period to half second and duty cycle to 50%.
   */
  pwmChangePeriod(&PWMD1, 5000);
  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
  chThdSleepMilliseconds(5000);

  /*
   * Disables channel 0.
   */
  pwmDisableChannel(&PWMD1, 0);
  pwmStop(&PWMD1);
  palSetPad(IOPORT3, GPIOC_LED);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
