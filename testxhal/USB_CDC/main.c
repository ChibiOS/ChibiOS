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

#define SHELL_WA_SIZE              THD_STACK_SIZE(1024)
#define BLINKER_WA_SIZE            THD_STACK_SIZE(256)
#define EP0_WA_SIZE                THD_STACK_SIZE(512)
#define TEST_FAIL_BLINK_PERIOD_MS  100U
#define BLINK_ACTIVE_PERIOD_MS     250U
#define BLINK_IDLE_PERIOD_MS       500U
#define USB_RECONNECT_DELAY_MS     1500U

static void test_fail(void) {

  while (true) {
    palClearLine(PORTAB_BLINK_LED1);
    chThdSleepMilliseconds(TEST_FAIL_BLINK_PERIOD_MS);
    palSetLine(PORTAB_BLINK_LED1);
    chThdSleepMilliseconds(TEST_FAIL_BLINK_PERIOD_MS);
  }
}

static void test_assert(bool condition) {

  if (!condition) {
    test_fail();
  }
}

static bool write_all(asynchronous_channel_i *chp, const uint8_t *bp,
                      size_t n) {

  while (n > 0U) {
    size_t written;

    written = chnWrite(chp, bp, n);
    if (written == 0U) {
      return false;
    }

    bp += written;
    n  -= written;
  }

  return true;
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
    if (!write_all((asynchronous_channel_i *)xshp->stream,
                   buf, sizeof buf - 1U)) {
      break;
    }
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

/*
 * Keep enough margin here, a too-small blinker stack can corrupt adjacent
 * static objects such as the USB binder and make the failure mode misleading.
 */
static THD_WORKING_AREA(waBlinkerThread, BLINKER_WA_SIZE);
static THD_FUNCTION(BlinkerThread, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    systime_t time;

    time = usbGetDriverStateX(&PORTAB_USB1) == USB_ACTIVE ?
           BLINK_ACTIVE_PERIOD_MS : BLINK_IDLE_PERIOD_MS;
    palClearLine(PORTAB_BLINK_LED1);
    chThdSleepMilliseconds(time);
    palSetLine(PORTAB_BLINK_LED1);
    chThdSleepMilliseconds(time);
  }
}

static THD_WORKING_AREA(waEp0Thread, EP0_WA_SIZE);
static THD_FUNCTION(Ep0Thread, arg) {
  hal_usb_binder_c *binderp;

  (void)arg;
  chRegSetThreadName("usb-ep0");

  /*
   * Endpoint zero is handled entirely in thread context in this demo. The
   * LLD only captures SETUP packets and wakes this worker, then control
   * transfers are completed here using the common EP0 helpers:
   *   1. wait for a new SETUP packet,
   *   2. serve standard requests in the shared USB code,
   *   3. forward non-standard requests to the currently bound USB binder,
   *   4. stall EP0 if nobody claims the request.
   */
  while (true) {
    bool handled;
    msg_t msg;

    /* Blocks until a new SETUP packet arrives or the bus is reset. */
    msg = usbEp0WaitSetup(&PORTAB_USB1);
    if (msg != MSG_OK) {
      continue;
    }

    /* Standard device requests are handled by the common USB layer first. */
    handled = false;
    msg = usbEp0HandleStandardRequest(&PORTAB_USB1, &handled);
    if (msg != MSG_OK) {
      continue;
    }

    /* Class/vendor requests are delegated to the active device binder. */
    if (!handled) {
      binderp = usbGetBinderX(&PORTAB_USB1);
      if (binderp != NULL) {
        msg = usbBinderSetup(binderp, &handled);
        if (msg != MSG_OK) {
          continue;
        }
      }
    }

    /* Unclaimed requests must terminate with a stall on endpoint zero. */
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
  test_assert(usbBinderRegisterService(&usbcdc_binder,
                                       sduGetServiceX(&PORTAB_SDU1)) ==
              HAL_RET_SUCCESS);

  usbDisconnectBus(&PORTAB_USB1);
  chThdSleepMilliseconds(USB_RECONNECT_DELAY_MS);
  test_assert(drvStart(&PORTAB_USB1, NULL) == HAL_RET_SUCCESS);
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
