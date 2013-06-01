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

static const struct can_instance can1 = {&CAND1, PD_LED1};

/*
 * Internal loopback mode, 1MBaud.
 * See chapter 25 on the SPC5 reference manual.
 */
static const CANConfig cancfg = {
  CAN_MCR_WRN_EN,
  CAN_CTRL_LPB | CAN_CTRL_PROPSEG(2) | CAN_CTRL_PSEG2(7) |
  CAN_CTRL_PSEG1(3) | CAN_CTRL_PRESDIV(3)
#if SPC5_CAN_USE_FILTERS
  ,
  {
   {0, 0x00000001},
   {1, 0x01234567},
   {0, 0x00000000},
   {0, 0x00000003},
   {0, 0x00000004},
   {0, 0x00000005},
   {0, 0x00000006},
   {0, 0x00000007}
  }
#endif
};

#if SPC5_CAN_USE_FILTERS
flagsmask_t rxFlag;
#endif

/*
 * Receiver thread.
 */
static WORKING_AREA(can_rx_wa, 256);
static msg_t can_rx(void *p) {
  struct can_instance *cip = p;
  EventListener el;
  CANRxFrame rxmsg;
  (void)p;
  chRegSetThreadName("receiver");
  chEvtRegister(&cip->canp->rxfull_event, &el, 0);
#if SPC5_CAN_USE_FILTERS
  rxFlag = chEvtGetAndClearFlagsI(&el);
#endif
  while(!chThdShouldTerminate()) {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(100)) == 0)
      continue;
#if !SPC5_CAN_USE_FILTERS
    while (canReceive(cip->canp, CAN_ANY_MAILBOX,
                      &rxmsg, TIME_IMMEDIATE) == RDY_OK) {
      /* Process message.*/
      palTogglePad(PORT_D, cip->led);
    }
#else
    while (canReceive(cip->canp, rxFlag,
                       &rxmsg, TIME_IMMEDIATE) == RDY_OK) {
      /* Process message.*/
      palTogglePad(PORT_D, cip->led);
    }
#endif
  }
  chEvtUnregister(&CAND1.rxfull_event, &el);
  return 0;
}

/*
 * Transmitter thread.
 */
static WORKING_AREA(can_tx_wa, 256);
static msg_t can_tx(void * p) {
  CANTxFrame txmsg;

  (void)p;
  chRegSetThreadName("transmitter");
  txmsg.IDE = CAN_IDE_EXT;
  txmsg.EID = 0x01234567;
  txmsg.RTR = CAN_RTR_DATA;
  txmsg.LENGTH = 8;
  txmsg.data32[0] = 0x55AA55AA;
  txmsg.data32[1] = 0x00FF00FF;

  while (!chThdShouldTerminate()) {
    canTransmit(&CAND1, 1, &txmsg, MS2ST(100));
    palTogglePad(PORT_D, PD_LED2);
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
   * Activates the CAN driver 1.
   */
  canStart(&CAND1, &cancfg);

  /*
   * Starting the transmitter and receiver threads.
   */
  chThdCreateStatic(can_rx_wa, sizeof(can_rx_wa), NORMALPRIO + 7,
                    can_rx, (void *)&can1);
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

