/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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

#define PC_DEMO

#if defined(LED_DEMO)
 /*
  * Timed Blinking Thread.
  */
static WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  chRegSetThreadName("Blinker");
  while (true) {
    palTogglePad(IOPORT2, PORTB_LED1);
    chThdSleepMilliseconds(1000);
  }
}
#else

BSEMAPHORE_DECL(sem, 0);

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


#if defined(EXT_DEMO)
 /*
  * External Interrupt Callback.
  */
void ext_isr_cb(EXTDriver *extp, expchannel_t channel) {
  chSysLockFromISR();
  if (channel == EXT_INT0_CHANNEL) {
    chBSemSignalI(&sem);
  }
  else if (channel == EXT_INT1_CHANNEL) {
  }
  chSysUnlockFromISR();
}


 /*
  * Configuration for EXT mode
  */
const EXTConfig ext_cfg = {
  .channels = {
    [EXT_INT0_CHANNEL] = {
      .mode = EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_INTERNAL_PULLUP |
              EXT_CH_MODE_AUTOSTART,
      .cb = ext_isr_cb,
    },
    [EXT_INT1_CHANNEL] = {
      .mode = EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_INTERNAL_PULLUP |
              EXT_CH_MODE_AUTOSTART,
      .cb = ext_isr_cb,
    },
  },
};

#elif defined(PC_DEMO)
 /*
  * Pin Change Interrupt Callback.
  */
void pc_isr_cb(PCDriver *pcp, pcchannel_t channel) {
  chSysLockFromISR();
  uint8_t changes =  pcp->current_values[channel] ^ pcp->old_values[channel];
  for (uint8_t i = 0; i < 8; i++)
    if (changes & _BV(i))
      chBSemSignalI(&sem);
  chSysUnlockFromISR();
}




 /*
  * Configuration for PC mode
  */
PCConfig pc_cfg = {
  .enabled = {},
  .cb = {[0] = pc_isr_cb,
         [1] = pc_isr_cb,
         [2] = pc_isr_cb},
};

#endif
#endif
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

  palClearPad(IOPORT2, PORTB_LED1);

  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD1, NULL);
#if defined(LED_DEMO)
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
#else
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);
  #if defined(EXT_DEMO)
    extStart(&EXTD1, &ext_cfg);
  #elif defined(PC_DEMO)
    pcStart(&PCD1, &pc_cfg);
    pcChannelEnable(&PCD1, 8);
    pcChannelEnable(&PCD1, 9);
    pcChannelEnable(&PCD1, 10);
    pcChannelEnable(&PCD1, 11);
  #endif
#endif
  while(TRUE) {
    chnWrite(&SD1, (const uint8_t *)"Hello World!\r\n", 14);
    chThdSleepMilliseconds(1000);
  }
}
