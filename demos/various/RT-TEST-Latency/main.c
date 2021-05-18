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
#include "chprintf.h"

#include "portab.h"

#define TEST_CYCLES 1000U

static time_measurement_t tm1, tm2;
static thread_reference_t tr;

/*
 * Flyback thread.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;

  chRegSetThreadName("flyback");

  while (true) {
    /* Waiting for wakeup from PENDSV then stopping measurement.*/
    chSysLock();
    (void) chThdSuspendS(&tr);
    chTMStopMeasurementX(&tm2);
    chSysUnlock();
  }
}

/*
 * IRQ 0 handler.
 */
CH_IRQ_HANDLER(Vector40) {

  CH_IRQ_PROLOGUE();

  chTMChainMeasurementToX(&tm1, &tm2);

  chSysLockFromISR();
  chThdResumeI(&tr, MSG_OK);
  chSysUnlockFromISR();

  CH_IRQ_EPILOGUE();
}

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

  /* Starting a serial port for test report output.*/
  sdStart(&PORTAB_SD1, NULL);

  /* Starting the flyback thread.*/
  tr = NULL;
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1U, Thread1, NULL);

  /* Initializing a TM objects for measurement of latency.*/
  chTMObjectInit(&tm1);
  chTMObjectInit(&tm2);

  /* Setting up an IRQ for the latency test. Highest available priority
     is used.*/
  nvicEnableVector(0, CORTEX_MAX_KERNEL_PRIORITY);

  /* Printing banner.*/
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "*** Compiled:      %s\r\n", __DATE__ " - " __TIME__);
#if defined(PLATFORM_NAME)
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "*** Platform:      %s\r\n", PLATFORM_NAME);
#endif
#if defined(BOARD_NAME)
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "*** Test Board:    %s\r\n", BOARD_NAME);
#endif
#if defined(PORT_ARCHITECTURE_NAME)
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "*** Architecture:  %s\r\n", PORT_ARCHITECTURE_NAME);
#endif
#if defined(PORT_CORE_VARIANT_NAME)
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "*** Core Variant:  %s @ %uMHz\r\n", PORT_CORE_VARIANT_NAME, SystemCoreClock / 1000000U);
#endif
#if defined(PORT_COMPILER_NAME)
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "*** Compiler:      %s\r\n\r\n", PORT_COMPILER_NAME);
#endif
  chThdSleepMilliseconds(500U);

  /* Test loop.*/
  for (unsigned i = 0U; i < TEST_CYCLES; i++) {
    /* Triggering PENDSV, it will happen on the unlock.*/
    chSysLock();
//    SCB->ICSR =SCB_ICSR_PENDSVSET_Msk;
    nvicSetPending(0);
    chTMStartMeasurementX(&tm1);
    chSysUnlock();
    chThdSleepMilliseconds(1);
  }

  /* Printing results.*/
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "ISR activation time latency\r\n\r\n");
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Iterations:        %u\r\n", tm1.n);
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Last measurement:  %u\r\n", tm1.last);
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Best measurement:  %u\r\n", tm1.best);
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Worst measurement: %u\r\n", tm1.worst);
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Cumulative time:   %u\r\n\r\n", (uint32_t)tm1.cumulative);
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Thread fly-back latency\r\n\r\n");
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Iterations:        %u\r\n", tm2.n);
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Last measurement:  %u\r\n", tm2.last);
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Best measurement:  %u\r\n", tm2.best);
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Worst measurement: %u\r\n", tm2.worst);
  chprintf((BaseSequentialStream *)&PORTAB_SD1, "Cumulative time:   %u\r\n\r\n", (uint32_t)tm2.cumulative);

  /*
   * Normal main() thread activity, if the button is pressed then the DAC
   * transfer is stopped.
   */
  while (true) {
    if (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
    }
    chThdSleepMilliseconds(500);
  }
  return 0;
}
