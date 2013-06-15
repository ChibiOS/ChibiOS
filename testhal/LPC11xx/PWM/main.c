/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC11xx EXT driver - Copyright (C) 2013 Marcin Jokel
                       - Copyright (C) 2013 mike brown

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

static void pwm2pcb(PWMDriver *pwmp) {

  (void)pwmp;
  palClearPad(GPIO0, GPIO0_LED);
}

static void pwm2c0cb(PWMDriver *pwmp) {

  (void)pwmp;
  palSetPad(GPIO0, GPIO0_LED);
}

static PWMConfig pwmcfg = {
  10000,                                    /* 10kHz PWM clock frequency.   */
  1000,                                     /* Initial PWM period 4,8us     */
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
  pwmEnableChannel(&PWMD2, 1, 250);
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 1 to 50% duty cycle.
   */
  pwmEnableChannel(&PWMD2, 0, 500);
  pwmEnableChannel(&PWMD2, 1, 500);
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 0 to 75% duty cycle.
   */
  pwmEnableChannel(&PWMD2, 0, 250);
  pwmEnableChannel(&PWMD2, 1, 250);
  chThdSleepMilliseconds(5000);

  /*
   * Changes PWM period to half second the duty cycle becomes 50%
   * implicitly.
   */
  pwmChangePeriod(&PWMD2, 500);
  chThdSleepMilliseconds(5000);

  /*
   * Disables channel 1.
   */
  pwmDisableChannel(&PWMD2, 1);
  chThdSleepMilliseconds(5000);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
