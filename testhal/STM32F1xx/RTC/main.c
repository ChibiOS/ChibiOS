/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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


RTCDateTime timespec;
RTCDateTime alarmspec;

#define TEST_ALARM_WAKEUP FALSE



#if TEST_ALARM_WAKEUP

/* sleep indicator thread */
static WORKING_AREA(blinkWA, 128);
static msg_t blink_thd(void *arg){
  (void)arg;
  while (TRUE) {
    chThdSleepMilliseconds(100);
    palTogglePad(IOPORT3, GPIOC_LED);
  }
  return 0;
}

int main(void) {
  halInit();
  chSysInit();

  chThdCreateStatic(blinkWA, sizeof(blinkWA), NORMALPRIO, blink_thd, NULL);
  /* set alarm in near future */
  rtcGetTime(&timespec);
  alarmspec.tv_sec = timespec.tv_sec + 60;
  rtcSetAlarm(&alarmspec);

  while (TRUE){
      chThdSleepSeconds(10);
      chSysLock();

      /* going to anabiosis*/
      PWR->CR |= (PWR_CR_PDDS | PWR_CR_LPDS | PWR_CR_CSBF | PWR_CR_CWUF);
      SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
      __WFI();
  }
  return 0;
}



#else /* TEST_ALARM_WAKEUP */

static void my_overflowcb(RTCDriver *rtcp){
  (void)rtcp;
  palTogglePad(IOPORT3, GPIOC_LED);
}

static void my_secondcb(RTCDriver *rtcp){
  (void)rtcp;
  //palTogglePad(IOPORT3, GPIOC_LED);
}

static void my_alarmcb(RTCDriver *rtcp){
  (void)rtcp;
  palTogglePad(IOPORT3, GPIOC_LED);
  rtcGetTime(&timespec);
  alarmspec.tv_sec = timespec.tv_sec + 10;
  rtcSetAlarm(&alarmspec);
}


int main(void) {
  halInit();
  chSysInit();

  rtcGetTime(&timespec);
  alarmspec.tv_sec = timespec.tv_sec + 10;
  rtcSetAlarm(&alarmspec);

  rtcSetCallback(&RTCD, my_overflowcb, my_secondcb, my_alarmcb);
  while (TRUE){
    chThdSleepMilliseconds(500);
  }
  return 0;
}
#endif /* TEST_ALARM_WAKEUP */
