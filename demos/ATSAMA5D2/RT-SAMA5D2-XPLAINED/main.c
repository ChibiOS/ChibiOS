/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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

#define AICREDIR_KEY 0x5B6C0E26u

/*
 * Seconds counter thread.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  bool ld = true;
  chRegSetThreadName("counter");

  while (true) {
  /* TODO: Replace with toggle function of GPIO */
    if(ld){
      PIOA->PIO_PIO_[1].S_PIO_CODR = S_PIO_CODR_P5;
      ld = false;
    }
    else{
      PIOA->PIO_PIO_[1].S_PIO_SODR = S_PIO_SODR_P5;
      ld = true;
    }
    chThdSleepMilliseconds(500);
  }
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

  /* Redirect interrupts */
  uint32_t aicredir = SFR_AICREDIR_AICREDIRKEY((uint32_t)(AICREDIR_KEY));
  SFR->SFR_AICREDIR = (aicredir ^ SFR->SFR_SN1);

  /*
   *  TODO: Replace with PAL functions
   *  Led green
   */
  PIOA->PIO_PIO_[1].S_PIO_SIOSR |= S_PIO_SIOSR_P5;
  PIOA->PIO_PIO_[1].S_PIO_MSKR = S_PIO_MSKR_MSK5_ENABLED;
  PIOA->PIO_PIO_[1].S_PIO_CFGR = S_PIO_CFGR_DIR_OUTPUT;
  PIOA->PIO_PIO_[1].S_PIO_SODR = S_PIO_SODR_P5 ;

  /*
   * TODO: Replace with PAL functions
   * Uart0 pins PB26 rx and PB27 tx (J18)
   */
  PIOA->PIO_PIO_[1].S_PIO_SIOSR |= S_PIO_SIOSR_P26 | S_PIO_SIOSR_P27;
  /* select pins */
  PIOA->PIO_PIO_[1].S_PIO_MSKR = S_PIO_MSKR_MSK26 | S_PIO_MSKR_MSK27;
  /* pins are driven by func_periph_c (uart0 mode) */
  PIOA->PIO_PIO_[1].S_PIO_CFGR = S_PIO_CFGR_FUNC_PERIPH_C;

  sdStart(&SD0, NULL);
  chprintf((BaseSequentialStream *)&SD0, "ChibiOS is running!\r\n\n");
  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * increasing the minutes counter.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
}
