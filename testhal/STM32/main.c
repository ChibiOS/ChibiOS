/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/
#include <stdio.h>

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "settings.h"

/*
 * LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(blinker_wa, 128);
static msg_t blinker_thread(void *p) {

  (void)p;
  while (TRUE) {
    palClearPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palSetPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

#if CH_HAL_USE_ADC
static adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
static Thread *adctp;

/*
 * ADC continuous conversion thread.
 */
size_t nx = 0, ny = 0;
static void adccallback(adcsample_t *buffer, size_t n) {

  if (samples == buffer) {
    nx += n;
  }
  else {
    ny += n;
  }
}

static WORKING_AREA(adc_continuous_wa, 256);
static msg_t adc_continuous_thread(void *p){

  (void)p;
  palSetGroupMode(IOPORT3,
                  PAL_PORT_BIT(0) | PAL_PORT_BIT(1),
                  PAL_MODE_INPUT_ANALOG);
  adcStart(&ADCD1, &adccfg);
  adcStartConversion(&ADCD1, &adcgrpcfg, samples,
                     ADC_GRP1_BUF_DEPTH, adccallback);
  adcWaitConversion(&ADCD1, TIME_INFINITE);
  adcStop(&ADCD1);
  return 0;
}
#endif /* CH_HAL_USE_ADC */

#if CH_HAL_USE_CAN
static Thread *canrtp;
static Thread *canttp;

static WORKING_AREA(can_rx_wa, 256);
static msg_t can_rx(void *p) {
  EventListener el;
  CANRxFrame rxmsg;

  (void)p;
  chEvtRegister(&CAND1.cd_rxfull_event, &el, 0);
  while(!chThdShouldTerminate()) {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
      continue;
    while (canReceive(&CAND1, &rxmsg, TIME_IMMEDIATE) == RDY_OK) {
      /* Process message.*/
      palTogglePad(IOPORT3, GPIOC_LED);
    }
  }
  chEvtUnregister(&CAND1.cd_rxfull_event, &el);
  return 0;
}

static WORKING_AREA(can_tx_wa, 256);
static msg_t can_tx(void * p) {
  CANTxFrame txmsg;

  (void)p;
  txmsg.cf_IDE = CAN_IDE_EXT;
  txmsg.cf_EID = 0x01234567;
  txmsg.cf_RTR = CAN_RTR_DATA;
  txmsg.cf_DLC = 8;
  txmsg.cf_data32[0] = 0x55AA55AA;
  txmsg.cf_data32[1] = 0x00FF00FF;

  while (!chThdShouldTerminate()) {
    canTransmit(&CAND1, &txmsg, MS2ST(100));
/*    chThdSleepMilliseconds(5);*/
  }
  return 0;
}
#endif /* CH_HAL_USE_CAN */

#if CH_HAL_USE_SPI
static uint8_t txbuf[512];
static uint8_t rxbuf[512];
static Thread *spitp;

/*
 * Maximum speed SPI continuous loopback thread.
 */
static WORKING_AREA(spi_loopback_wa, 256);
static msg_t spi_loopback_thread(void *p){

  (void)p;
  palSetPadMode(IOPORT1, GPIOA_SPI1NSS, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPad(IOPORT1, GPIOA_SPI1NSS);
  spiStart(&SPID1, &spicfg);
  while (!chThdShouldTerminate()) {
    spiSelect(&SPID1);
    spiExchange(&SPID1, 512, txbuf, rxbuf);
    spiUnselect(&SPID1);
  }
  spiStop(&SPID1);
  return 0;
}
#endif /* CH_HAL_USE_SPI */

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {
   unsigned i;

  (void)argc;
  (void)argv;
  (void)i;

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD2, NULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(blinker_wa, sizeof(blinker_wa),
                    NORMALPRIO + 10, blinker_thread, NULL);

#if CH_HAL_USE_ADC
  /*
   * Creates the ADC continuous conversion test thread.
   */
  adctp = chThdCreateStatic(adc_continuous_wa, sizeof(adc_continuous_wa),
                            NORMALPRIO + 9, adc_continuous_thread, NULL);
#endif

#if CH_HAL_USE_CAN
  canStart(&CAND1, &cancfg);
  canrtp = chThdCreateStatic(can_rx_wa, sizeof(can_rx_wa),
                             NORMALPRIO + 7, can_rx, NULL);
  canttp = chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa),
                             NORMALPRIO + 7, can_tx, NULL);
#endif

#if CH_HAL_USE_SPI
  /*
   * Creates the SPI loopback test thread.
   */
  for (i = 0; i < sizeof(txbuf); i++)
    txbuf[i] = (uint8_t)i;
  spitp = chThdCreateStatic(spi_loopback_wa, sizeof(spi_loopback_wa),
                            NORMALPRIO + 8, spi_loopback_thread, NULL);
#endif

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (TRUE) {
    if (palReadPad(IOPORT1, GPIOA_BUTTON)) {
      TestThread(&SD2);
#if CH_HAL_USE_ADC
      adcStopConversion(&ADCD1);
      chThdWait(adctp);
#endif
#if CH_HAL_USE_CAN
      chThdTerminate(canttp);
      chThdWait(canttp);
      chThdTerminate(canrtp);
      chThdWait(canrtp);
#endif
#if CH_HAL_USE_SPI
      chThdTerminate(spitp);
      chThdWait(spitp);
#endif
      chThdSleepMilliseconds(500);
      TestThread(&SD2);
      chThdSleepMilliseconds(500);
      chSysHalt();
    }
    chThdSleepMilliseconds(500);
  }
  return 0;
}
