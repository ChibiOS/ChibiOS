/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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

#include "hal.h"

#ifdef _NIL_
#include "nil.h"
#else
#include "ch.h"
#endif

#define PERIOD_VALUE1 0x7FFF
#define PERIOD_VALUE2 0xFF

#ifdef _NIL_
THD_WORKING_AREA(waThread1, 128);
THD_FUNCTION(Thread1, arg) {
  (void)arg;
  while (true) {
    chThdSleepMilliseconds(1);
  }
}

THD_TABLE_BEGIN
  THD_TABLE_ENTRY(waThread1, "main", Thread1, NULL)
THD_TABLE_END
#endif

int main(void) {

  halInit();

  /*
   * NOTE: when compiling for NIL, after the chSysInit() call, nothing
   * more can be done in this thread so we first initialize PWM subsystem.
   */

#if AVR_PWM_USE_TIM1 && PWM_CHANNELS == 2
  static PWMConfig pwm1cfg = {
    F_CPU,
    PERIOD_VALUE1,
    NULL,
    {
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
      {PWM_OUTPUT_ACTIVE_HIGH, NULL}
    },
  };

  /* PB1-2 are timer 1 pwm channel outputs for ATMega328p */
  palSetGroupMode(IOPORT2, 0x3, 1, PAL_MODE_OUTPUT_PUSHPULL);

  pwmStart(&PWMD1, &pwm1cfg);

  /* channel 0 with 75% duty cycle and 1 with 25% */
  pwmEnableChannel(&PWMD1, 0, (PERIOD_VALUE1 >> 2)*3);
  pwmEnableChannel(&PWMD1, 1, PERIOD_VALUE1 >> 2);
#endif

#if AVR_PWM_USE_TIM2
  static PWMConfig pwm2cfg = {
    F_CPU >> 5,
    PERIOD_VALUE2,
    NULL,
    {
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
      {PWM_OUTPUT_ACTIVE_HIGH, NULL}
    },
  };

  /* PB3 and PD3 are timer 2 pwm channel outputs for ATMega328p */
  palSetPadMode(IOPORT2, 3, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(IOPORT4, 3, PAL_MODE_OUTPUT_PUSHPULL);

  pwmStart(&PWMD2, &pwm2cfg);

  /* channel 0 with 80% duty cycle and 1 with 20% */
  pwmEnableChannel(&PWMD2, 0, PERIOD_VALUE2/5*4);
  pwmEnableChannel(&PWMD2, 1, PERIOD_VALUE2/5);
#endif

#if AVR_PWM_USE_TIM3
  static PWMConfig pwm3cfg = {
    F_CPU,
    PERIOD_VALUE1,
    NULL,
    {
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    },
  };

  /* PE3-5 are timer 3 pwm channel outputs */
  palSetPadMode(IOPORT5, 3, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(IOPORT5, 4, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(IOPORT5, 5, PAL_MODE_OUTPUT_PUSHPULL);

  pwmStart(&PWMD3, &pwm3cfg);
  /* channel 0 with 50% duty cycle, 1 with 25% and 2 with 75% */
  pwmEnableChannel(&PWMD3, 0, PERIOD_VALUE1 >> 1);
  pwmEnableChannel(&PWMD3, 1, PERIOD_VALUE1 >> 2);
  pwmEnableChannel(&PWMD3, 2, (PERIOD_VALUE1 >> 2)*3);
#endif

  chSysInit();

  while (1) {}
}

