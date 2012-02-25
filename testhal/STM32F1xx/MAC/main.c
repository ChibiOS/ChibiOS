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
*/

#include "ch.h"
#include "hal.h"

#include "evtimer.h"

#define PERIODIC_TIMER_ID       1
#define FRAME_RECEIVED_ID       2

static const MACConfig mac_config = {NULL};

static uint8_t frame[STM32_MAC_BUFFERS_SIZE];

/*
 * Application entry point.
 */
int main(void) {
  EvTimer evt;
  EventListener el0, el1;

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
   * Activates the MAC driver 1.
   */
  macStart(&ETHD1, &mac_config);

  /* Setup event sources.*/
  evtInit(&evt, S2ST(5));
  evtStart(&evt);
  chEvtRegisterMask(&evt.et_es, &el0, PERIODIC_TIMER_ID);
  chEvtRegisterMask(macGetReceiveEventSource(&ETHD1), &el1, FRAME_RECEIVED_ID);
  chEvtAddFlags(PERIODIC_TIMER_ID | FRAME_RECEIVED_ID);

  /*
   * Normal main() thread activity, in this demo it enables and disables the
   * button EXT channel using 5 seconds intervals.
   */
  while (TRUE) {
    eventmask_t mask = chEvtWaitAny(ALL_EVENTS);
    if (mask & PERIODIC_TIMER_ID)
      (void)macPollLinkStatus(&ETHD1);
    if (mask & FRAME_RECEIVED_ID) {
      MACReceiveDescriptor rd;

      if (macWaitReceiveDescriptor(&ETHD1, &rd, TIME_IMMEDIATE) == RDY_OK) {
        macReadReceiveDescriptor(&rd, frame, STM32_MAC_BUFFERS_SIZE);
        macReleaseReceiveDescriptor(&rd);
      }
    }
  }
}
