/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include <string.h>

#include "ch.h"
#include "hal.h"

#define ADC_GRP2_NUM_CHANNELS   8
#define ADC_GRP2_BUF_DEPTH      16

static adcsample_t samples2[ADC_GRP2_NUM_CHANNELS * ADC_GRP2_BUF_DEPTH];

static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

  (void)adcp;
  (void)buffer;
  (void)n;
}

static void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;
  chSysHalt();
}

/*
 * ADC conversion group.
 * Mode:        Continuous, 16 samples of 8 channels, SW triggered.
 * Channels:    IN11, IN12, IN11, IN12, IN11, IN12, Sensor, VRef.
 */
static const ADCConversionGroup adcgrpcfg2 = {
  TRUE,
  ADC_GRP2_NUM_CHANNELS,
  adccallback,
  adcerrorcallback,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  ADC_SMPR1_SMP_AN12(ADC_SAMPLE_56) | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_56) |
  ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144) | ADC_SMPR1_SMP_VREF(ADC_SAMPLE_144),
  0,                        /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_GRP2_NUM_CHANNELS),
  ADC_SQR2_SQ8_N(ADC_CHANNEL_SENSOR) | ADC_SQR2_SQ7_N(ADC_CHANNEL_VREFINT),
  ADC_SQR3_SQ6_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ5_N(ADC_CHANNEL_IN11) |
  ADC_SQR3_SQ4_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN11) |
  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11)
};

/*
 * Maximum speed SPI configuration (21MHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig hs_spicfg = {
  NULL,
  GPIOB,
  12,
  0
};

static void tmo(void *p) {

  (void)p;
  chSysHalt();
}

/*
 * SPI thread.
 */
static WORKING_AREA(waSPI1, 1024);
static WORKING_AREA(waSPI2, 1024);
static WORKING_AREA(waSPI3, 1024);
static msg_t spi_thread(void *p) {
  unsigned i;
  SPIDriver *spip = (SPIDriver *)p;
  VirtualTimer vt;
  uint8_t txbuf[256];
  uint8_t rxbuf[256];

  /* Prepare transmit pattern.*/
  for (i = 0; i < sizeof(txbuf); i++)
    txbuf[i] = (uint8_t)i;

  /* Continuous transmission.*/
  while (TRUE) {
    /* Starts a VT working as watchdog to catch a malfunction in the SPI
       driver.*/
    chSysLock();
    chVTSetI(&vt, MS2ST(10), tmo, NULL);
    chSysUnlock();

    spiExchange(spip, sizeof(txbuf), txbuf, rxbuf);

    /* Stops the watchdog.*/
    chSysLock();
    if (chVTIsArmedI(&vt))
      chVTResetI(&vt);
    chSysUnlock();
  }
}

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palSetPad(GPIOD, GPIOD_LED3);       /* Orange.  */
    chThdSleepMilliseconds(500);
    palClearPad(GPIOD, GPIOD_LED3);     /* Orange.  */
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  unsigned i;
  static uint8_t patterns1[4096], patterns2[4096], buf1[4096], buf2[4096];

  /* System initializations.
     - HAL initialization, this also initializes the configured device drivers
       and performs the board-specific initializations.
     - Kernel initialization, the main() function becomes a thread and the
       RTOS is active.*/
  halInit();
  chSysInit();

  /* Creates the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 10,
                    Thread1, NULL);

  /* Activates the ADC1 driver and the thermal sensor.*/
  adcStart(&ADCD1, NULL);
  adcSTM32EnableTSVREFE();

  /* Starts an ADC continuous conversion.*/
  adcStartConversion(&ADCD1, &adcgrpcfg2, samples2, ADC_GRP2_BUF_DEPTH);

  /* Activating SPI drivers.*/
  spiStart(&SPID1, &hs_spicfg);
  spiStart(&SPID2, &hs_spicfg);
  spiStart(&SPID3, &hs_spicfg);

  /* Starting SPI threads instances.*/
  chThdCreateStatic(waSPI1, sizeof(waSPI1), NORMALPRIO + 1, spi_thread, &SPID1);
  chThdCreateStatic(waSPI2, sizeof(waSPI2), NORMALPRIO + 1, spi_thread, &SPID2);
  chThdCreateStatic(waSPI3, sizeof(waSPI3), NORMALPRIO + 1, spi_thread, &SPID3);

  /* Allocating two DMA2 streams for memory copy operations.*/
  if (dmaStreamAllocate(STM32_DMA2_STREAM6, 0, NULL, NULL))
    chSysHalt();
  if (dmaStreamAllocate(STM32_DMA2_STREAM7, 0, NULL, NULL))
    chSysHalt();
  for (i = 0; i < sizeof (patterns1); i++)
    patterns1[i] = (uint8_t)i;
  for (i = 0; i < sizeof (patterns2); i++)
    patterns2[i] = (uint8_t)(i ^ 0xAA);

  /* Normal main() thread activity, it does continues memory copy operations
     using 2 DMA streams at the lowest priority.*/
  while (TRUE) {
    VirtualTimer vt;

    /* Starts a VT working as watchdog to catch a malfunction in the DMA
       driver.*/
    chSysLock();
    chVTSetI(&vt, MS2ST(10), tmo, NULL);
    chSysUnlock();

    /* Copy pattern 1.*/
    dmaStartMemCopy(STM32_DMA2_STREAM6,
                    STM32_DMA_CR_PL(0) | STM32_DMA_CR_PSIZE_BYTE |
                                         STM32_DMA_CR_MSIZE_BYTE,
                    patterns1, buf1, sizeof (patterns1));
    dmaStartMemCopy(STM32_DMA2_STREAM7,
                    STM32_DMA_CR_PL(0) | STM32_DMA_CR_PSIZE_BYTE |
                                         STM32_DMA_CR_MSIZE_BYTE,
                    patterns1, buf2, sizeof (patterns1));
    dmaWaitCompletion(STM32_DMA2_STREAM6);
    dmaWaitCompletion(STM32_DMA2_STREAM7);
    if (memcmp(patterns1, buf1, sizeof (patterns1)))
      chSysHalt();
    if (memcmp(patterns1, buf2, sizeof (patterns1)))
      chSysHalt();

    /* Copy pattern 2.*/
    dmaStartMemCopy(STM32_DMA2_STREAM6,
                    STM32_DMA_CR_PL(0) | STM32_DMA_CR_PSIZE_BYTE |
                                         STM32_DMA_CR_MSIZE_BYTE,
                    patterns2, buf1, sizeof (patterns2));
    dmaStartMemCopy(STM32_DMA2_STREAM7,
                    STM32_DMA_CR_PL(0) | STM32_DMA_CR_PSIZE_BYTE |
                                         STM32_DMA_CR_MSIZE_BYTE,
                    patterns2, buf2, sizeof (patterns2));
    dmaWaitCompletion(STM32_DMA2_STREAM6);
    dmaWaitCompletion(STM32_DMA2_STREAM7);
    if (memcmp(patterns2, buf1, sizeof (patterns2)))
      chSysHalt();
    if (memcmp(patterns2, buf2, sizeof (patterns2)))
      chSysHalt();

    /* Stops the watchdog.*/
    chSysLock();
    if (chVTIsArmedI(&vt))
      chVTResetI(&vt);
    chSysUnlock();

    chThdSleepMilliseconds(2);
  }
  return 0;
}
