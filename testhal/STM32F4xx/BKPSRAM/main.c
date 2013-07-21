/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

uint32_t *bkpsram = (uint32_t *)BKPSRAM_BASE;

/*
 *
 */
static void write_watermark(void){
  size_t i = 0;
  for (i=0; i<10; i++)
    bkpsram[i] = i;

  /* signalize about sram erase */
  palClearPad(GPIOB, GPIOB_LED_R);
}

/*
 *
 */
static bool check_watermark(void){
  size_t i = 0;
  for (i=0; i<10; i++){
    if (bkpsram[i] != i)
      return false;
  }
  return true;
}

/**
 * Main function.
 */
int main(void){

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /* Goal of this testhal */
  if (!check_watermark()){
    write_watermark();
  }

  /* Normal main thread loop. */
  while (TRUE){
    chThdSleepMilliseconds(200);
    palTogglePad(GPIOB, GPIOB_LED_B);
  }

  return 0;
}


