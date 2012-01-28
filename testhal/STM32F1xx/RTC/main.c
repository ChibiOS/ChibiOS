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
    palTogglePad(GPIOC, GPIOC_LED);
  }
  return 0;
}

int main(void) {
  halInit();
  chSysInit();

  chThdCreateStatic(blinkWA, sizeof(blinkWA), NORMALPRIO, blink_thd, NULL);
  /* set alarm in near future */
  rtcGetTime(&RTCD1, &timespec);
  alarmspec.tv_sec = timespec.tv_sec + 30;
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

/* Manually reloaded test alarm period.*/
#define RTC_ALARMPERIOD   10

BinarySemaphore alarm_sem;

static void my_cb(RTCDriver *rtcp, rtcevent_t event) {

  (void)rtcp;

  switch (event) {
  case RTC_EVENT_OVERFLOW:
    palTogglePad(GPIOC, GPIOC_LED);
    break;
  case RTC_EVENT_SECOND:
    /* palTogglePad(GPIOC, GPIOC_LED); */
    break;
  case RTC_EVENT_ALARM:
    palTogglePad(GPIOC, GPIOC_LED);
    chSysLockFromIsr();
    chBSemSignalI(&alarm_sem);
    chSysUnlockFromIsr();
    break;
  }
}

int main(void) {
  msg_t status = RDY_TIMEOUT;

  halInit();
  chSysInit();
  chBSemInit(&alarm_sem, TRUE);

  rtcGetTime(&RTCD1, &timespec);
  alarmspec.tv_sec = timespec.tv_sec + RTC_ALARMPERIOD;
  rtcSetAlarm(&RTCD1, 0, &alarmspec);

  rtcSetCallback(&RTCD1, my_cb);
  while (TRUE){

    /* Wait until alarm callback signaled semaphore.*/
    status = chBSemWaitTimeout(&alarm_sem, S2ST(RTC_ALARMPERIOD + 5));

    if (status == RDY_TIMEOUT){
      chSysHalt();
    }
    else{
      rtcGetTime(&RTCD1, &timespec);
      alarmspec.tv_sec = timespec.tv_sec + RTC_ALARMPERIOD;
      rtcSetAlarm(&RTCD1, 0, &alarmspec);
    }
  }
  return 0;
}
#endif /* TEST_ALARM_WAKEUP */
