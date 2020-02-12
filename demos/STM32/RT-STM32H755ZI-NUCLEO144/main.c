/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
#include "rt_test_root.h"
#include "oslib_test_root.h"

#include "chprintf.h"

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetLine(LINE_LED1);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED2);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED3);
    chThdSleepMilliseconds(200);
    palClearLine(LINE_LED1);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED2);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED3);
    chThdSleepMilliseconds(200);
  }
}

#include "i_ctrl.h"

/*
 * Application entry point.
 */
__attribute__((section(".ram6_init.main")))
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
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD3, NULL);

  time_measurement_t tm1;
  chTMObjectInit(&tm1);

  float32_t vbridge[N_PHASES];

  for (int i = 0; i < 10; i++) {
    chTMStartMeasurementX(&tm1);
    {
      static float32_t duty[N_PHASES] = {0.0f};

      float32_t il[N_PHASES] = {1.0f, 5.0f, 3.0f};
      float32_t vc[N_PHASES] = {300.0f, 400.0f, 100.0f};
      float32_t vDC[DC_RAILS] = {450.0f, 400.0f};
      float32_t iL_ref[N_PHASES] = {7.0f, 10.0f, 5.0f};
      bool enable = 1;

       ictrl_3L_step(il, vc, vDC, iL_ref, enable, vbridge);

      calculate_duty_cycle(vbridge, vDC, duty);
    }
    chTMStopMeasurementX(&tm1);
  }

  chprintf((BaseSequentialStream *)&SD3, "Measurements:      %d\r\n", tm1.n);
  chprintf((BaseSequentialStream *)&SD3, "Best measurement:  %d\r\n", tm1.best);
  chprintf((BaseSequentialStream *)&SD3, "Worst measurement: %d\r\n", tm1.worst);
  chprintf((BaseSequentialStream *)&SD3, "Last measurement:  %d\r\n", tm1.last);
  chprintf((BaseSequentialStream *)&SD3, "vbridge[0]:        %f\r\n", vbridge[0]);
  chprintf((BaseSequentialStream *)&SD3, "vbridge[1]:        %f\r\n", vbridge[1]);
  chprintf((BaseSequentialStream *)&SD3, "vbridge[2]:        %f\r\n", vbridge[2]);

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (1) {
    if (palReadLine(LINE_BUTTON)) {
      test_execute((BaseSequentialStream *)&SD3, &rt_test_suite);
      test_execute((BaseSequentialStream *)&SD3, &oslib_test_suite);
    }
    chThdSleepMilliseconds(500);
  }
}
