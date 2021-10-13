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
#include "sb.h"
#include "chprintf.h"

#include "rt_test_root.h"
#include "oslib_test_root.h"

/* Static memory areas used by sandboxes.*/
extern uint32_t __flash1_base__, __flash1_end__,
                __flash2_base__, __flash2_end__,
                __ram1_base__,   __ram1_end__,
                __ram2_base__,   __ram2_end__;

/* Sandbox 1 configuration.*/
static const sb_config_t sb_config1 = {
  .code_region    = 0U,
  .data_region    = 1U,
  .regions        = {
    [0] = {
      (uint32_t)&__flash1_base__,   (uint32_t)&__flash1_end__,  false
    },
    [1] = {
       (uint32_t)&__ram1_base__,    (uint32_t)&__ram1_end__,    true
    }
  },
  .stdin_stream   = (SandboxStream *)&LPSD1,
  .stdout_stream  = (SandboxStream *)&LPSD1,
  .stderr_stream  = (SandboxStream *)&LPSD1
};

/* Sandbox 2 configuration.*/
static const sb_config_t sb_config2 = {
  .code_region    = 0U,
  .data_region    = 1U,
  .regions        = {
    [0] = {
      (uint32_t)&__flash2_base__,   (uint32_t)&__flash2_end__,  false
    },
    [1] = {
      (uint32_t)&__ram2_base__,     (uint32_t)&__ram2_end__,    true
    }
  },
  .stdin_stream   = (SandboxStream *)&LPSD1,
  .stdout_stream  = (SandboxStream *)&LPSD1,
  .stderr_stream  = (SandboxStream *)&LPSD1
};

/* Sandbox objects.*/
sb_class_t sbx1, sbx2;

static THD_WORKING_AREA(waUnprivileged1, 256);
static THD_WORKING_AREA(waUnprivileged2, 256);

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;

  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  unsigned i = 1U;
  thread_t *utp1, *utp2;
  event_listener_t el1;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   * - SandBox manager initialization.
   */
  halInit();
  chSysInit();
  sbHostInit();

  /*
   * Listening to sandbox events.
   */
  chEvtRegister(&sb.termination_es, &el1, (eventid_t)0);

  /*
   * Activates the Serial driver using the default configuration.
   */
  sdStart(&LPSD1, NULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+10, Thread1, NULL);

  /*
   * Creating **static** boxes using MPU.
   * Note: The two regions cover both sandbox 1 and 2, there is no
   *       isolation among them.
   */
  mpuConfigureRegion(MPU_REGION_0,
                     0x08070000U,
                     MPU_RASR_ATTR_AP_RO_RO |
                     MPU_RASR_ATTR_CACHEABLE_WT_NWA |
                     MPU_RASR_SIZE_64K |
                     MPU_RASR_ENABLE);
  mpuConfigureRegion(MPU_REGION_1,
                     0x2001E000U,
                     MPU_RASR_ATTR_AP_RW_RW |
                     MPU_RASR_ATTR_CACHEABLE_WB_WA |
                     MPU_RASR_SIZE_8K |
                     MPU_RASR_ENABLE);

  /* Starting sandboxed thread 1.*/
  utp1 = sbStartThread(&sbx1, &sb_config1, "sbx1",
                       waUnprivileged1, sizeof (waUnprivileged1),
                       NORMALPRIO - 1);
  if (utp1 == NULL) {
    chSysHalt("sbx1 failed");
  }

  /* Starting sandboxed thread 2.*/
  utp2 = sbStartThread(&sbx2, &sb_config2, "sbx2",
                       waUnprivileged2, sizeof (waUnprivileged2),
                       NORMALPRIO - 1);
  if (utp1 == NULL) {
    chSysHalt("sbx2 failed");
  }

  /*
   * Normal main() thread activity, in this demo it monitos the user button
   * and checks for sandboxes state.
   */
  while (true) {

    /* Checking for user button, launching test suite if pressed.*/
    if (palReadLine(LINE_BUTTON)) {
      test_execute((BaseSequentialStream *)&LPSD1, &rt_test_suite);
      test_execute((BaseSequentialStream *)&LPSD1, &oslib_test_suite);
    }

    /* Waiting for a sandbox event or timeout.*/
    if (chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)) != (eventmask_t)0) {

      if (chThdTerminatedX(utp1)) {
        chprintf((BaseSequentialStream *)&LPSD1, "SB1 terminated\r\n");
      }

      if (chThdTerminatedX(utp2)) {
        chprintf((BaseSequentialStream *)&LPSD1, "SB2 terminated\r\n");
      }
    }

    if ((i & 1) == 0U) {
      if (!chThdTerminatedX(utp1)) {
        (void) sbSendMessageTimeout(&sbx1, (msg_t)i, TIME_MS2I(10));
      }
    }
    else {
      if (!chThdTerminatedX(utp2)) {
        (void) sbSendMessageTimeout(&sbx2, (msg_t)i, TIME_MS2I(10));
      }
    }
    i++;
  }
}

