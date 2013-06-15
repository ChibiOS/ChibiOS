/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    EXT haltest - Copyright (C) 2013 Marcin Jokel
                - Copyright (C) 2013 mike brown

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

static void ledoff(void *arg) {

  (void)arg;
  palClearPad(GPIO0, GPIO0_LED);
}

/* Triggered when the button is pressed or released. The LED is set to ON.*/
static void ext_cb0(EXTDriver *extp, expchannel_t channel) {
  static VirtualTimer vt4;

  (void)extp;
  (void)channel;

  palSetPad(GPIO0, GPIO0_LED);
  chSysLockFromIsr();
  if (chVTIsArmedI(&vt4))
    chVTResetI(&vt4);

  /* LED set to OFF after 200mS.*/
  chVTSetI(&vt4, MS2ST(200), ledoff, NULL);
  chSysUnlockFromIsr();
}

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, ext_cb0},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

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

  /*
   * Activates the EXT driver 0.
   */
  extStart(&EXTD0, &extcfg);

  /*
   * Normal main() thread activity, in this demo it enables and disables the
   * button EXT channel using 5 seconds intervals.
   */
  while (TRUE) {
    chThdSleepMilliseconds(5000);
    extChannelDisable(&EXTD0, GPIO0_SW_ISP);
    chThdSleepMilliseconds(5000);
    extChannelEnable(&EXTD0, GPIO0_SW_ISP);
  }
}
