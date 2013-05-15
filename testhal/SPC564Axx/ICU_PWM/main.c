/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
  palClearPad(PORT11, P11_LED1);
}

static void pwmc1cb(PWMDriver *pwmp) {

  (void)pwmp;
  palSetPad(PORT11, P11_LED1);
}

static PWMConfig pwmcfg = {
  80000,                                    /* 80kHz PWM clock frequency.*/
  20000,                                    /* Initial PWM period 0.25s.*/
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb}
  },
  PWM_ALIGN_EDGE
};

icucnt_t last_width, last_period;
icucnt_t last_width2, last_period2;

static void icuwidthcb(ICUDriver *icup) {

  palSetPad(PORT11, P11_LED2);
  last_width = icuGetWidth(icup);
}

static void icuperiodcb(ICUDriver *icup) {

  palClearPad(PORT11, P11_LED2);
  last_period = icuGetPeriod(icup);
}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  80000,                                    /* 80kHz ICU clock frequency.*/
  icuwidthcb,
  icuperiodcb,
  NULL
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
   * Initializes the PWM driver 6 and ICU driver 3.
   * PIN78 is the PWM output.
   * PIN65 is the ICU input.
   * The two pins have to be externally connected together.
   */

  /* Sets PIN65 alternative function.*/
  SIU.PCR[181].R = 0b0000010100001100;

  /* Sets PIN78 alternative function.*/
  SIU.PCR[193].R = 0b0000011000001100;

  icuStart(&ICUD3, &icucfg);
  icuEnable(&ICUD3);
  pwmStart(&PWMD6, &pwmcfg);

  chThdSleepMilliseconds(2000);

  /*
   * Starts the PWM channel 0 using 75% duty cycle.
   */
  pwmEnableChannel(&PWMD6, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD6, 7500));
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 0 to 50% duty cycle.
   */
  pwmEnableChannel(&PWMD6, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD6, 5000));
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 0 to 25% duty cycle.
   */
  pwmEnableChannel(&PWMD6, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD6, 2500));
  chThdSleepMilliseconds(5000);

  /*
   * Changes PWM period and the PWM channel 0 to 50% duty cycle.
   */
  pwmChangePeriod(&PWMD6, 25000);
  pwmEnableChannel(&PWMD6, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD6, 5000));
  chThdSleepMilliseconds(5000);

  /*
   * Disables PWM channel 0 and stops the drivers.
   */
  pwmDisableChannel(&PWMD6, 0);
  pwmStop(&PWMD6);

  /*
   * Disables and stops the ICU drivers.
   */

  icuDisable(&ICUD3);
  icuStop(&ICUD3);

  palClearPad(PORT11, P11_LED3);
  palClearPad(PORT11, P11_LED4);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
