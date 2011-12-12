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

RTCTime timespec;
RTCAlarm alarmspec;

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
  rtcGetTime(&RTCD1, &timespec);
  alarmspec.tv_sec = timespec.tv_sec + 60;
  rtcSetAlarm(&RTCD1, 0, &alarmspec);

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

/**
 * Callback function for RTC.
 */
static void my_cb(RTCDriver *rtcp, rtcevent_t event) {
  (void)rtcp;

  switch (event) {
  case RTC_EVENT_OVERFLOW:
    palTogglePad(GPIOC, GPIOC_LED);
    break;
  case RTC_EVENT_SECOND:
    //palTogglePad(GPIOC, GPIOC_LED);
    break;
  case RTC_EVENT_ALARM:
    palTogglePad(GPIOC, GPIOC_LED);
    rtcGetTime(&RTCD1, &timespec);
    alarmspec.tv_sec = timespec.tv_sec + 5;
    rtcSetAlarm(&RTCD1, 0, &alarmspec);
    break;
  }
}

/**
 * Configuration structure with all callbacks supported by platform.
 */
static RTCCallbackConfig rtc_cb_cfg = {
	my_cb
};

/**
 * Main function.
 */
int main(void){
  halInit();
  chSysInit();

  rtcGetTime(&RTCD1, &timespec);
  alarmspec.tv_sec = timespec.tv_sec + 5;
  rtcSetAlarm(&RTCD1, 0, &alarmspec);

  rtcSetCallback(&RTCD1, &rtc_cb_cfg);
  while (TRUE){
    chThdSleepMilliseconds(500);
  }
  return 0;
}
#endif /* TEST_ALARM_WAKEUP */
