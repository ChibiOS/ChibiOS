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

struct can_instance {
  CANDriver     *canp;
  uint32_t      led;
};

static const struct can_instance can1 = {&CAND1, GPIO0_LED2_RED};
static const struct can_instance can2 = {&CAND2, GPIO0_LED3_EXT}; /* Connect LED to board. */

#if LPC17xx_CAN_USE_FILTER
static const CANFilterExt cfe_id_table[2] = {
  CANFilterExtEntry(0, 0x0ABCDEF0),
  CANFilterExtEntry(1, 0x01234567)

};

static const CANFilterConfig canfcfg = {
 0,
 NULL,
 0,
 NULL,
 0,
 NULL,
 0,
 cfe_id_table,
 2,
 NULL,
 0
};
#endif

/*
 * Operating mode
 */
static const CANConfig cancfg = {
  0,
  CANBTR_SJW(0) | CANBTR_TESG2(1) |
  CANBTR_TESG1(8) | CANBTR_BRP(20)
};

/*
 * Receiver thread.
 */
static WORKING_AREA(can_rx1_wa, 256);
static WORKING_AREA(can_rx2_wa, 256);
static msg_t can_rx(void *p) {
  struct can_instance *cip = p;
  EventListener el;
  CANRxFrame rxmsg;

  (void)p;
  chRegSetThreadName("receiver");
  chEvtRegister(&cip->canp->rxfull_event, &el, 0);
  while(!chThdShouldTerminate()) {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
      continue;
    while (canReceive(cip->canp, CAN_ANY_MAILBOX,
                      &rxmsg, TIME_IMMEDIATE) == RDY_OK) {
      /* Process message.*/
      palTogglePad(GPIO0, cip->led);
    }
  }
  chEvtUnregister(&CAND1.rxfull_event, &el);
  return 0;
}

/*
 * Transmitter thread.
 */
static WORKING_AREA(can_tx_wa, 256);
static msg_t can_tx(void * p) {
  CANTxFrame txmsg_can1;
  CANTxFrame txmsg_can2;

  (void)p;
  chRegSetThreadName("transmitter");
  txmsg_can1.IDE = CAN_IDE_EXT;
  txmsg_can1.EID = 0x01234567;
  txmsg_can1.RTR = CAN_RTR_DATA;
  txmsg_can1.DLC = 8;
  txmsg_can1.data32[0] = 0x55AA55AA;
  txmsg_can1.data32[1] = 0x00FF00FF;

  txmsg_can2.IDE = CAN_IDE_EXT;
  txmsg_can2.EID = 0x0ABCDEF0;
  txmsg_can2.RTR = CAN_RTR_DATA;
  txmsg_can2.DLC = 8;
  txmsg_can2.data32[0] = 0x66AA66AA;
  txmsg_can2.data32[1] = 0x44FF44FF;

  while (!chThdShouldTerminate()) {
    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg_can1, MS2ST(100));
    canTransmit(&CAND2, CAN_ANY_MAILBOX, &txmsg_can2, MS2ST(100));
    chThdSleepMilliseconds(500);
  }
  return 0;
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

#if LPC17xx_CAN_USE_FILTER
  canSetFilter(&canfcfg);
#endif
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
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
