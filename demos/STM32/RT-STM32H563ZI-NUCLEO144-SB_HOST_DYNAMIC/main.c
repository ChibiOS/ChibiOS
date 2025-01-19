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

#include "oop_chprintf.h"
#include "oop_nullstreams.h"

#include "startup_defs.h"

/* Sandbox objects.*/
sb_class_t sbx1, sbx2;

/*===========================================================================*/
/* VIO-related.                                                              */
/*===========================================================================*/

static vio_gpio_units_t gpio_units1 = {
  .n = 1U,
  .units = {
    [0] = {
      .permissions  = VIO_GPIO_PERM_WRITE,
      .port         = GPIOB,
      .mask         = 1U,
      .offset       = GPIOB_LED1
    }
  }
};

static vio_uart_units_t uart_units1 = {
  .n = 1U,
  .units = {
    [0] = {
      .siop         = &SIOD3,
      .vrqsb        = &sbx1,
      .vrqn         = 8
    }
  }
};

static sio_configurations_t uart_configs1 = {
  .cfgsnum = 1U,
  .cfgs = {
    [0]             = SIO_DEFAULT_CONFIGURATION
  }
};

static vio_conf_t vio_config1 = {
  .gpios            = &gpio_units1,
  .uarts            = &uart_units1,
  .uartconfs        = &uart_configs1
};

static vio_gpio_units_t gpio_units2 = {
  .n        = 1U,
  .units = {
    [0] = {
      .permissions  = VIO_GPIO_PERM_WRITE,
      .port         = GPIOF,
      .mask         = 1U,
      .offset       = GPIOF_LED2
    }
  }
};

static vio_uart_units_t uart_units2 = {
  .n                = 0U
};

static sio_configurations_t uart_configs2 = {
  .cfgsnum          = 0U
};

static vio_conf_t vio_config2 = {
  .gpios            = &gpio_units2,
  .uarts            = &uart_units2,
  .uartconfs        = &uart_configs2
};

/*===========================================================================*/
/* VFS-related.                                                              */
/*===========================================================================*/

#if VFS_CFG_ENABLE_DRV_FATFS == TRUE
/* VFS FatFS driver object representing the root directory.*/
static vfs_fatfs_driver_c root_driver;
#endif

/* VFS overlay driver object representing the root directory.*/
static vfs_overlay_driver_c root_overlay_driver;

/* VFS streams driver object representing the /dev directory.*/
static vfs_streams_driver_c dev_driver;

/* VFS API will use this object as implicit root, defining this
   symbol is expected.*/
vfs_driver_c *vfs_root = (vfs_driver_c *)&root_overlay_driver;

static null_stream_c nullstream;

/* Stream to be exposed under /dev as files.*/
static const drv_streams_element_t streams[] = {
//  {"VSIO1", (BaseSequentialStream *)oopGetIf(&SIOD3, chn), VFS_MODE_S_IFCHR},
  {"null", (BaseSequentialStream *)oopGetIf(&nullstream, stm), VFS_MODE_S_IFCHR},
  {NULL, NULL, 0}
};

/*===========================================================================*/
/* SB-related.                                                               */
/*===========================================================================*/

/* Privileged stacks for sandboxes.*/
static SB_STACK(sbx1stk);
static SB_STACK(sbx2stk);

/* Arguments and environments for SB1.*/
static const char *sbx1_argv[] = {
  "sbx1",
  NULL
};

static const char *sbx1_envp[] = {
  NULL
};

/* Arguments and environments for SB2.*/
static const char *sbx2_argv[] = {
  "sbx2",
  NULL
};

static const char *sbx2_envp[] = {
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

static void start_sb2(void) {
  thread_t *utp;
  msg_t ret;
  vfs_node_c *np;

  /*
   * Associating standard input, output and error to sandbox 2.*/
  ret = vfsOpen("/dev/null", 0, &np);
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }
  sbRegisterDescriptor(&sbx2, STDIN_FILENO, (vfs_node_c *)roAddRef(np));
  sbRegisterDescriptor(&sbx2, STDOUT_FILENO, (vfs_node_c *)roAddRef(np));
  sbRegisterDescriptor(&sbx2, STDERR_FILENO, (vfs_node_c *)roAddRef(np));
  vfsClose(np);

  /* Starting sandboxed thread 2.*/
  utp = sbStart(&sbx2, NORMALPRIO-20, sbx2stk, sbx2_argv, sbx2_envp);
  if (utp == NULL) {
    chSysHalt("sbx2 failed");
  }
}

/*
 * Messenger thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {
  unsigned i = 1U;

  (void)arg;

  chRegSetThreadName("messenger");
  while (true) {
    chThdSleepMilliseconds(500);
    sbSendMessage(&sbx2, (msg_t)i);
    i++;
  }
}

/*
 * Application entry point.
 */
int main(void) {
  event_listener_t el1;
  msg_t ret;

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
  vfsInit();
  sbHostInit();

  /*
   * Starting a serial port for I/O, initializing other streams too.
   */
  drvStart(&SIOD3);
  nullstmObjectInit(&nullstream);

  /*
   * Initializing an overlay VFS object as a root, no overlaid driver,
   * registering a streams VFS driver on the VFS overlay root as "/dev".
   */
  ovldrvObjectInit(&root_overlay_driver, NULL, NULL);
  ret = ovldrvRegisterDriver(&root_overlay_driver,
                             (vfs_driver_c *)stmdrvObjectInit(&dev_driver,
                                                              &streams[0]),
                             "dev");
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }

  /*
   * Sandbox objects initialization.
   */
  sbObjectInit(&sbx1);
  sbSetRegion(&sbx1, 0, STARTUP_FLASH1_BASE, STARTUP_FLASH1_SIZE, SB_REG_IS_CODE);
  sbSetRegion(&sbx1, 1, STARTUP_RAM1_BASE,   STARTUP_RAM1_SIZE, SB_REG_IS_DATA);
  sbSetVirtualIO(&sbx1, &vio_config1);

  sbObjectInit(&sbx2);
  sbSetRegion(&sbx2, 0, STARTUP_FLASH2_BASE, STARTUP_FLASH2_SIZE, SB_REG_IS_CODE);
  sbSetRegion(&sbx2, 1, STARTUP_RAM2_BASE,   STARTUP_RAM2_SIZE, SB_REG_IS_DATA);
  sbSetVirtualIO(&sbx2, &vio_config2);
  sbSetFileSystem(&sbx2, (vfs_driver_c *)&root_overlay_driver);

  /* Starting sandboxed threads.*/
  start_sb1();
  start_sb2();

  /*
   * Creating a messenger thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+10, Thread1, NULL);

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
        chprintf(oopGetIf(&SIOD3, chn), "SB1 terminated: 0x%08x\r\n", msg);
        chThdSleepMilliseconds(100);
        start_sb1();
      }

      if (!sbIsThreadRunningX(&sbx2)) {
        msg_t msg = sbWait(&sbx2);
        chprintf(oopGetIf(&SIOD3, chn), "SB2 terminated: 0x%08x\r\n", msg);
        chThdSleepMilliseconds(100);
        start_sb2();
      }
    }
  }
}
