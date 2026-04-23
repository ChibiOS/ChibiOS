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
#include "portab.h"

#include "hal_serial_usb.h"
#include "oop_chprintf.h"
#include "xshell.h"

#include "usbcfg.h"

#define SHELL_WA_SIZE       THD_STACK_SIZE(1024)

static void test_fail(void) {

  while (true) {
    palClearLine(PORTAB_BLINK_LED1);
    chThdSleepMilliseconds(100);
    palSetLine(PORTAB_BLINK_LED1);
    chThdSleepMilliseconds(100);
  }
}

static void test_assert(bool condition) {

  if (!condition) {
    test_fail();
  }
}

static void cmd_write(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  static const uint8_t buf[] =
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";
  (void)argv;
  (void)envp;

  if (argc != 1) {
    xshellUsage(xshp, "write");
    return;
  }

  while (chnGetTimeout((asynchronous_channel_i *)xshp->stream,
                       TIME_IMMEDIATE) == Q_TIMEOUT) {
    chnWrite(xshp->stream, buf, sizeof buf - 1U);
  }
  chprintf(xshp->stream, XSHELL_NEWLINE_STR "stopped" XSHELL_NEWLINE_STR);
}

static const xshell_command_t commands[] = {
  {"write", cmd_write},
  {NULL, NULL}
};

static const xshell_manager_config_t shell_cfg = {
  .thread_name      = "shell",
  .banner           = "\r\nXHAL USB CDC test\r\n",
  .prompt           = XSHELL_DEFAULT_PROMPT_STR,
  .commands         = commands,
  .use_heap         = true,
  .stack.size       = SHELL_WA_SIZE
};

static THD_WORKING_AREA(waBlinkerThread, 128);
static THD_FUNCTION(BlinkerThread, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    systime_t time;

    time = usbGetDriverStateX(&PORTAB_USB1) == USB_ACTIVE ? 250 : 500;
    palClearLine(PORTAB_BLINK_LED1);
    chThdSleepMilliseconds(time);
    palSetLine(PORTAB_BLINK_LED1);
    chThdSleepMilliseconds(time);
  }
}

static THD_WORKING_AREA(waEp0Thread, 512);
static THD_FUNCTION(Ep0Thread, arg) {
  hal_usb_binder_c *binderp;

  (void)arg;
  chRegSetThreadName("usb-ep0");
  while (true) {
    bool handled;
    msg_t msg;

    msg = usbEp0WaitSetup(&PORTAB_USB1);
    if (msg != MSG_OK) {
      continue;
    }

    handled = false;
    msg = usbEp0HandleStandardRequest(&PORTAB_USB1, &handled);
    if (msg != MSG_OK) {
      continue;
    }

    if (!handled) {
      binderp = usbGetBinderX(&PORTAB_USB1);
      if (binderp != NULL) {
        msg = usbBinderSetup(binderp, &handled);
        if (msg != MSG_OK) {
          continue;
        }
      }
    }

    if (!handled) {
      usbEp0Stall(&PORTAB_USB1);
    }
  }
}

int main(void) {
  xshell_manager_t sm1;

  halInit();
  chSysInit();

  portab_setup();
  usbcdcObjectInit();
  sduObjectInit(&PORTAB_SDU1);

  test_assert(sduStart(&PORTAB_SDU1, &serusbcfg) == HAL_RET_SUCCESS);

  usbDisconnectBus(&PORTAB_USB1);
  chThdSleepMilliseconds(1500);
  test_assert(usbStart(&PORTAB_USB1, NULL) == HAL_RET_SUCCESS);
  test_assert(usbBind(&PORTAB_USB1, &usbcdc_binder) == HAL_RET_SUCCESS);
  usbConnectBus(&PORTAB_USB1);

  chThdCreateStatic(waBlinkerThread, sizeof(waBlinkerThread),
                    NORMALPRIO, BlinkerThread, NULL);
  chThdCreateStatic(waEp0Thread, sizeof(waEp0Thread),
                    NORMALPRIO + 2, Ep0Thread, NULL);

  xshellObjectInit(&sm1, &shell_cfg);

  while (true) {
    if (usbGetDriverStateX(&PORTAB_USB1) == USB_ACTIVE) {
      xshell_t *xshp;

      xshp = xshellSpawn(&sm1, (BaseSequentialStream *)&PORTAB_SDU1.chn,
                         NORMALPRIO + 1, NULL);
      xshellWait(xshp);
    }
    chThdSleepMilliseconds(100);
  }
}
