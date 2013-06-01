/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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

static void pwm2pcb(PWMDriver *pwmp) {

  (void)pwmp;
  palSetPad(GPIO1, GPIO1_LED2);
}

static void pwm2c0cb(PWMDriver *pwmp) {

  (void)pwmp;
  palClearPad(GPIO1, GPIO1_LED2);
}

static PWMConfig pwmcfg = {
  10000,                                    /* 100kHz PWM clock frequency.   */
  1000,                                     /* PWM 10 Hz    */
  pwm2pcb,
  {
   {PWM_OUTPUT_ACTIVE_LOW, pwm2c0cb},
   {PWM_OUTPUT_ACTIVE_LOW, NULL}
  }
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
   * Initializes the PWM driver 2.
   */
  pwmStart(&PWMD2, &pwmcfg);
  chThdSleepMilliseconds(2000);

  /*
   * Starts the PWM channel 1 using 75% duty cycle.
   */
  pwmEnableChannel(&PWMD2, 0, 250);
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 1 to 50% duty cycle.
   */
  pwmEnableChannel(&PWMD2, 0, 500);
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 0 to 75% duty cycle.
   */
  pwmEnableChannel(&PWMD2, 0, 250);
  chThdSleepMilliseconds(5000);

  /*
   * Changes PWM period to half second the duty cycle becomes 50%
   * implicitly.
   */
  pwmChangePeriod(&PWMD2, 500);
  chThdSleepMilliseconds(5000);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
