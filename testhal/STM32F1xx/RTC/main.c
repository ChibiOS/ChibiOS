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

//#define TEST_DEEPSLEEP_ENABLE

#ifdef TEST_DEEPSLEEP_ENABLE

static WORKING_AREA(blinkWA, 128);
static msg_t blink_thd(void *arg){
  (void)arg;
  while (TRUE) {
    chThdSleepMilliseconds(500);
    palTogglePad(IOPORT3, GPIOC_LED);
  }
  return 0;
}




int main(void) {
  halInit();
  chSysInit();

  chThdCreateStatic(blinkWA, sizeof(blinkWA), NORMALPRIO, blink_thd, NULL);
  /* set alarm in near future */
  rtcSetAlarm(rtcGetSec() + 60);

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



#else /* TEST_DEEPSLEEP_ENABLE */

static void my_overflowcb(RTCDriver *rtcp){
  (void)rtcp;
  palTogglePad(IOPORT3, GPIOC_LED);
  rtcSetAlarm(rtcGetSec() + 10);
}

static void my_secondcb(RTCDriver *rtcp){
  (void)rtcp;
  //palTogglePad(IOPORT3, GPIOC_LED);
}

static void my_alarmcb(RTCDriver *rtcp){
  (void)rtcp;
  palTogglePad(IOPORT3, GPIOC_LED);
  rtcSetAlarm(rtcGetSec() + 10);
}


int main(void) {
  halInit();
  chSysInit();

  rtcSetAlarm(rtcGetSec() + 10);
  rtcSetCallback(&RTCD, NULL, my_secondcb, my_alarmcb);
  while (TRUE){
    chThdSleepMilliseconds(500);
  }
  return 0;
}
#endif /* TEST_DEEPSLEEP_ENABLE */
