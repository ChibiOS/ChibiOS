/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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

/*
 * If defined CANDx drivers work as CAN, else they work as FDCAN
 */
#define USE_CAN_PROTOCOL

struct can_instance {
  CANDriver     *canp;
  uint32_t      led;
};

static const struct can_instance can1 = {&CAND1, LINE_LED_GREEN};
static const struct can_instance can2 = {&CAND2, LINE_LED_RED};

/*
 * Baud 125kbit/s.
 */
static const CANConfig cancfg = {
#if defined USE_CAN_PROTOCOL
  OPMODE_CAN,
#else
  OPMODE_FDCAN,                    /* OP MODE */
#endif
  FDCAN_CONFIG_NBTP_NTSEG2(51U) |
  FDCAN_CONFIG_NBTP_NTSEG1(10U) |
  FDCAN_CONFIG_NBTP_NBRP(7U),      /* NBTP */
  FDCAN_CONFIG_DBTP_DSJW(3U) |
  FDCAN_CONFIG_DBTP_DTSEG2(3U) |
  FDCAN_CONFIG_DBTP_DTSEG1(10U) |
  FDCAN_CONFIG_DBTP_DBRP(7U),      /* DBTP */
  0,                               /* TDCR */
  0,                               /* CCCR */
  0,                               /* TEST */
  0                                /* GFC */
};

/*
 * Receiver thread.
 */
static THD_WORKING_AREA(can_rx1_wa, 256);
static THD_WORKING_AREA(can_rx2_wa, 256);
static THD_FUNCTION(can_rx, p) {
  struct can_instance *cip = p;
  event_listener_t el;
  CANRxFrame rxmsg;

  (void)p;
  chRegSetThreadName("receiver");
  chEvtRegister(&cip->canp->rxfull_event, &el, 0);
  while (true) {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, TIME_MS2I(100)) == 0)
      continue;
    while (canReceive(cip->canp, CAN_ANY_MAILBOX,
                      &rxmsg, TIME_I2MS(100)) == MSG_OK) {
      /* Process message.*/
      palToggleLine(cip->led);
      chThdSleepMilliseconds(10);
    }
  }
  chEvtUnregister(&CAND1.rxfull_event, &el);
}

/*
 * Transmitter thread.
 */
static THD_WORKING_AREA(can_tx_wa, 512);
static THD_FUNCTION(can_tx, p) {

  CANTxFrame txmsg1;

  (void)p;
  chRegSetThreadName("transmitter");
  txmsg1.ext.EID = 0x01234567;
  txmsg1.common.XTD = 1;  /* Extended ID. */
  txmsg1.DLC = FDCAN_8BYTES_TO_DLC;
#if defined USE_CAN_PROTOCOL
  txmsg1.FDF = 0;         /* CAN frame format. */
#else
  txmsg1.FDF = 1;         /* FDCAN frame format. */
#endif
  txmsg1.data32[0] = 0x55AA55AA;
  txmsg1.data32[1] = 0x00FF00FF;

  while (true) {
    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg1, TIME_IMMEDIATE);
    canTransmit(&CAND2, CAN_ANY_MAILBOX, &txmsg1, TIME_IMMEDIATE);
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

  /*
   * Activates the CAN drivers 1 and 2.
   */
  canStart(&CAND1, &cancfg);
  canStart(&CAND2, &cancfg);

  /*
   * Starting the transmitter and receiver threads.
   */
  chThdCreateStatic(can_rx1_wa, sizeof(can_rx1_wa), NORMALPRIO + 7,
                    can_rx, (void *)&can1);
  chThdCreateStatic(can_rx2_wa, sizeof(can_rx2_wa), NORMALPRIO + 7,
                    can_rx, (void *)&can2);
  chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa), NORMALPRIO + 7,
                    can_tx, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
