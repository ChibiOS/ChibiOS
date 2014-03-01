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
#include "chprintf.h"

#define MEM_SIZE    50

BaseSequentialStream  * chp = (BaseSequentialStream  *)&SD1;

uint32_t mem_src[MEM_SIZE];
uint32_t mem_dst[MEM_SIZE];

static void dma_mem_callback(void * dummy, uint32_t flags) {
  (void)dummy;
  (void)flags;
}

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palTogglePad(GPIO0, GPIO0_LED2_RED);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  uint32_t i;
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
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Activates the SD driver 1.
   */
  sdStart(&SD1, NULL);          /* Default is 38400-8-N-1.*/

  chprintf(chp, "Data before dma transfer.\r\n");
  chprintf(chp, "source \t destination\r\n");
  for (i = 0; i < MEM_SIZE; i++) {
     mem_src[i] = i;
     mem_dst[i] = 0;
     chprintf(chp, "%x \t %x\r\n", mem_src[i], mem_dst[i]);
  }

  dmaChannelAllocate(DMA_CHANNEL0, &dma_mem_callback, NULL);
  dmaChannelSrcAddr(DMA_CHANNEL0, &mem_src[0]);
  dmaChannelDstAddr(DMA_CHANNEL0, &mem_dst[0]);
  dmaChannelControl(DMA_CHANNEL0, DMA_CTRL_TRANSFER_SIZE(MEM_SIZE)  |
                                  DMA_CTRL_SRC_BSIZE_16             |
                                  DMA_CTRL_DST_BSIZE_16             |
                                  DMA_CTRL_SRC_WIDTH_WORD           |
                                  DMA_CTRL_DST_WIDTH_WORD           |
                                  DMA_CTRL_SRC_INC                  |
                                  DMA_CTRL_DST_INC                  |
                                  DMA_CTRL_INT);

  dmaChannelConfig(DMA_CHANNEL0,  DMA_CFG_CH_ENABLE   |
                                  DMA_CFG_TTYPE_M2M   |
                                  DMA_CFG_IE          |
                                  DMA_CFG_ITC);

  chThdSleepMilliseconds(5000);
  chprintf(chp, "Data after dma transfer.\r\n");
  chprintf(chp, "source \t destination\r\n");
  for (i = 0; i < MEM_SIZE; i++) {
    chprintf(chp, "%x \t %x\r\n", mem_src[i], mem_dst[i]);
  }

  for (i = 0; i < MEM_SIZE; i++)
    if (mem_src[i] != mem_dst[i])
      break;

  if (i == MEM_SIZE)
    chprintf(chp, "Data transfer ok.\r\n");
  else
    chprintf(chp, "Error.\r\n");

  while (TRUE) {

  chThdSleepMilliseconds(1000);

  }
}
