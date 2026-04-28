/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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

#include "oop_chprintf.h"
#include "oop_nullstreams.h"

#include "startup_defs.h"

/* Sandbox objects.*/
sb_class_t sbx1, sbx2;

/*===========================================================================*/
/* VIO-related.                                                              */
/*===========================================================================*/

static vio_gpio_units_t gpio_units1 = {
  .n        = 1U,
  .units = {
    [0] = {
      .permissions  = VIO_GPIO_PERM_WRITE,
      .port         = GPIOA,
      .mask         = 1U,
      .offset       = GPIOA_LED_GREEN
    }
  }
};

static vio_uart_units_t uart_units1 = {
  .n        = 1U,
  .units = {
    [0] = {
      .siop  = &LPSIOD1,
      .vrqsb = &sbx1,
      .vrqn  = 8
    }
  }
};

static sio_configurations_t uart_configs1 = {
  .cfgsnum      = 1U,
  .cfgs = {
    [0]         = SIO_DEFAULT_CONFIGURATION
  }
};

static vio_conf_t vio_config1 = {
  .gpios        = &gpio_units1,
  .uarts        = &uart_units1,
  .uartconfs    = &uart_configs1
};

/*===========================================================================*/
/* SB-related.                                                               */
/*===========================================================================*/

/* Privileged stacks for sandboxes.*/
static SB_STACK(sbx1stk);

/* Arguments and environments for SB1.*/
static const char *sbx1_argv[] = {
  "sbx1",
  NULL
};

static const char *sbx1_envp[] = {
  NULL
};

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

static void start_sb1(void) {
  thread_t *utp;

  /* Starting sandboxed thread 1.*/
  utp = sbStart(&sbx1, NORMALPRIO-10, sbx1stk, sbx1_argv, sbx1_envp);
  if (utp == NULL) {
    chSysHalt("sbx1 failed");
  }
}

/*
 * Application entry point.
 */
int main(void) {
  event_listener_t el1;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   * - Virtual File System initialization.
   * - SandBox manager initialization.
   */
  halInit();
  chSysInit();
  sbHostInit();

  /*
   * Starting a serial port for I/O, initializing other streams too.
   */
  if (drvStart(&SIOD1, NULL) != MSG_OK) {
    chSysHalt("SIOD1 failed");
  }


  /*
   * Sandbox objects initialization, regions are assigned explicitly.
   */
  sbObjectInit(&sbx1);
  sbSetRegion(&sbx1, 0, STARTUP_FLASH1_BASE, STARTUP_FLASH1_SIZE, SB_REG_IS_CODE);
  sbSetRegion(&sbx1, 1, STARTUP_RAM1_BASE,   STARTUP_RAM1_SIZE, SB_REG_IS_DATA);
  sbSetVirtualIO(&sbx1, &vio_config1);

  /* Starting sandboxed threads.*/
  start_sb1();

  /*
   * Listening to sandbox events.
   */
  chEvtRegister(&sb.termination_es, &el1, (eventid_t)0);

  /*
   * Normal main() thread activity, in this demo it checks for sandboxes state.
   */
  while (true) {

    /* Waiting for a sandbox event or timeout.*/
    if (chEvtWaitAnyTimeout(ALL_EVENTS, TIME_MS2I(500)) != (eventmask_t)0) {

      if (!sbIsThreadRunningX(&sbx1)) {
        msg_t msg = sbWait(&sbx1);
        chprintf(oopGetIf(&SIOD1, chn), "SB1 terminated: 0x%08x\r\n", msg);
        chThdSleepMilliseconds(100);
        start_sb1();
      }
    }
  }
}
