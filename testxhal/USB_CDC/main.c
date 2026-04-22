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

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "portab.h"

#include "hal_serial_usb.h"

#include "usbcfg.h"

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

static void send_string(const char *s) {

  (void)chnWriteTimeout(&PORTAB_SDU1.chn, (const uint8_t *)s, strlen(s),
                        TIME_INFINITE);
}

static void execute_write_test(void) {
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
  asynchronous_channel_i *chnp = &PORTAB_SDU1.chn;

  while (chnGetTimeout(chnp, TIME_IMMEDIATE) == STM_TIMEOUT) {
    (void)chnWriteTimeout(&PORTAB_SDU1.chn, buf, sizeof buf - 1U,
                          TIME_INFINITE);
  }
  send_string("\r\nstopped\r\n> ");
}

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
      msg = sduRequestsHook(&PORTAB_SDU1, &handled);
      if (msg != MSG_OK) {
        continue;
      }
    }

    if (!handled) {
      usbEp0Stall(&PORTAB_USB1);
    }
  }
}

int main(void) {
  bool banner_shown;
  char line[16];
  unsigned linelen;

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

  banner_shown = false;
  linelen = 0U;
  while (true) {
    if (usbGetDriverStateX(&PORTAB_USB1) == USB_ACTIVE) {
      msg_t c;

      if (!banner_shown) {
        send_string("\r\nXHAL USB CDC test\r\n");
        send_string("Commands: help, write\r\n> ");
        banner_shown = true;
      }

      c = chnGetTimeout(&PORTAB_SDU1.chn, TIME_MS2I(100));
      if (c >= 0) {
        if ((c == '\r') || (c == '\n')) {
          send_string("\r\n");
          line[linelen] = '\0';
          if (strcmp(line, "write") == 0) {
            execute_write_test();
          }
          else if ((strcmp(line, "help") == 0) || (linelen == 0U)) {
            send_string("Commands: help, write\r\n> ");
          }
          else {
            send_string("Unknown command\r\n> ");
          }
          linelen = 0U;
        }
        else if ((c == 0x08) || (c == 0x7FU)) {
          if (linelen > 0U) {
            linelen--;
            send_string("\b \b");
          }
        }
        else if ((c >= 0x20) && (c < (msg_t)0x7FU)) {
          if (linelen < (sizeof line - 1U)) {
            line[linelen++] = (char)c;
            (void)chnPutTimeout(&PORTAB_SDU1.chn, (uint8_t)c, TIME_INFINITE);
          }
        }
      }
    }
    else {
      banner_shown = false;
      linelen = 0U;
      chThdSleepMilliseconds(100);
    }
  }
}
