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
  palClearPad(PORT_E, PE_LED1);

}

static void pwmc1cb(PWMDriver *pwmp) {

  (void)pwmp;
  palSetPad(PORT_E, PE_LED1);
}

static PWMConfig pwmcfg = {
  40000,                                    /* 40kHz PWM clock frequency.*/
  20000,                                    /* Initial PWM period 0.5s.*/
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_LOW, pwmc1cb},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  PWM_ALIGN_EDGE
};

icucnt_t last_width, last_period;

static void icuwidthcb(ICUDriver *icup) {

  palSetPad(PORT_E, PE_LED2);
  last_width = icuGetWidth(icup);
}

static void icuperiodcb(ICUDriver *icup) {

  palClearPad(PORT_E, PE_LED2);
  last_period = icuGetPeriod(icup);
}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_LOW,
  40000,                                    /* 40kHz ICU clock frequency.*/
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

  palClearPad(PORT_E, PE_LED4);

  /*
   * Initializes the PWM driver 1 and ICU driver 1.
   * GPIOA9 is the PWM channel 0 output.
   * GPIOA0 is the ICU input.
   * The two pins have to be externally connected together.
   */
  icuStart(&ICUD1, &icucfg);
  icuEnable(&ICUD1);

  /* Sets A0 alternative function.*/
  SIU.PCR[0].R = 0b0000010100000100;

  /* Sets A9 alternative function.*/
  SIU.PCR[9U].R = 0b0000011000000100;

  pwmStart(&PWMD1, &pwmcfg);

  chThdSleepMilliseconds(2000);

  /*
   * Starts the PWM channel 0 using 75% duty cycle.
   */
  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 7500));
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 0 to 50% duty cycle.
   */
  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 0 to 25% duty cycle.
   */
  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 2500));
  chThdSleepMilliseconds(5000);

  /*
   * Changes PWM period and the PWM channel 0 to 50% duty cycle.
   */
  pwmChangePeriod(&PWMD1, 30000);
  pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
  chThdSleepMilliseconds(5000);

  /*
   * Disables channel 0 and stops the drivers.
   */
  pwmDisableChannel(&PWMD1, 0);
  pwmStop(&PWMD1);

  icuDisable(&ICUD1);
  icuStop(&ICUD1);

  palClearPad(PORT_E, PE_LED3);
  palClearPad(PORT_E, PE_LED4);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
