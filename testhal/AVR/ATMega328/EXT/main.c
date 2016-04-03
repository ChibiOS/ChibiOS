/*
    ChibiOS - HAL - EXT driver example.
    Copyright (C) 2015 Igor Stoppa <igor.stoppa@gmail.com>

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

BSEMAPHORE_DECL(sem, 0);

 /*
  * External Interrupt Callback for direct irqs.
  */
void ext_int_isr_cb(EXTDriver *extp, expchannel_t channel) {
  chSysLockFromISR();
  /* This check would be unnecessary here, it's only showing how to tell
   * apart different sources. */
  if (channel == EXT_INT0_CHANNEL) {
    chBSemSignalI(&sem);
  }
  else if (channel == EXT_INT1_CHANNEL) {
    chBSemSignalI(&sem);
  }
  chSysUnlockFromISR();
}

 /*
  * External Interrupt Callback for PC irqs.
  */
void ext_pc_isr_cb(EXTDriver *extp, expchannel_t channel) {
  chSysLockFromISR();
  chBSemSignalI(&sem);
  chSysUnlockFromISR();
}



 /*
  * Configuration for EXT/PC interrupts.
  */
const EXTConfig ext_cfg = {
  .channels = {
    [EXT_INT0_CHANNEL] = {
      .mode = EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_INTERNAL_PULLUP,
      .cb = ext_int_isr_cb,
    },
    [EXT_INT1_CHANNEL] = {
      .mode = EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_INTERNAL_PULLUP,
      .cb = ext_int_isr_cb,
    },
    [PORTC_INDEX] = {
      .cb = ext_pc_isr_cb,
    },
  },
};

 /*
  * Semaphore Driven Thread.
  */
static WORKING_AREA(waThread2, 32);
static THD_FUNCTION(Thread2, arg) {
  (void)arg;
  while (true) {
    chBSemWait(&sem);
    palTogglePad(IOPORT2, PORTB_LED1);
  }
}

/*
 * Application entry point.
 */

int main(void) {
  halInit();
  chSysInit();
  extStart(&EXTD1, &ext_cfg);

  /* Configure the output appriately. */
  palClearPad(IOPORT2, PORTB_LED1);

  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  /* Enable different types of interrupt sources. */
  extChannelEnable(&EXTD1, EXT_INT0_CHANNEL);
  extChannelEnable(&EXTD1, EXT_INT1_CHANNEL);
  extChannelEnable(&EXTD1, ext_port_to_channel(C, 0));
  extChannelEnable(&EXTD1, ext_port_to_channel(C, 1));
  extChannelEnable(&EXTD1, ext_port_to_channel(C, 2));
  extChannelEnable(&EXTD1, ext_port_to_channel(C, 3));

  while(TRUE) {
    chThdSleepMilliseconds(1000);
  }
}
