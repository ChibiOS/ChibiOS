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

int main(void) {

  halInit();
  chSysInit();

  static PWMConfig pwm1cfg = {
    1023,   /* Not real clock */
    1023,   /* Maximum PWM count */
    NULL,
    {
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    },
  };

  /* PB5-7 are timer 1 pwm channel outputs */
  palSetPadMode(IOPORT2, 7, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(IOPORT2, 6, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(IOPORT2, 5, PAL_MODE_OUTPUT_PUSHPULL);

  pwmStart(&PWMD1, &pwm1cfg);

  /* channel 0 with 50% duty cycle, 1 with 25% and 2 with 75% */
  pwmEnableChannel(&PWMD1, 0, 511);
  pwmEnableChannel(&PWMD1, 1, 255);
  pwmEnableChannel(&PWMD1, 2, 767);

  while (1) {
    chThdSleepMilliseconds(500);
  }
}
