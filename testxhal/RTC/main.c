/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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

#include "portab.h"

#define RTC_TEST_ALARM_A_SECONDS           4U
#define RTC_TEST_ALARM_B_SECONDS           7U

static volatile unsigned rtc_cb_count;

static void test_fail(void) {

  while (true) {
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_ON);
    chThdSleepMilliseconds(100);
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_OFF);
    chThdSleepMilliseconds(100);
  }
}

static void test_assert(bool condition) {

  if (!condition) {
    test_fail();
  }
}

static void rtc_test_cb(void *ip) {
  hal_rtc_driver_c *rtcp = (hal_rtc_driver_c *)ip;

  (void)rtcp;
  rtc_cb_count++;
}

static void datetime_add_seconds(rtc_datetime_t *timespec, unsigned seconds) {
  rtc_time64_t tv;
  msg_t msg;

  msg = rtcConvertDateTimeToTime64(timespec, &tv);
  test_assert(msg == HAL_RET_SUCCESS);
  tv.tv_sec += (int64_t)seconds;
  tv.tv_nsec = 0U;
  msg = rtcConvertTime64ToDateTime(&tv, timespec);
  test_assert(msg == HAL_RET_SUCCESS);
}

static uint32_t encode_alarm(const rtc_datetime_t *timespec) {
  uint32_t seconds, minutes, hours;

  seconds = (timespec->millisecond / 1000U) % 60U;
  minutes = (timespec->millisecond / 60000U) % 60U;
  hours = timespec->millisecond / 3600000U;

  return RTC_ALRM_MSK4 |
         RTC_ALRM_HT(hours / 10U) |
         RTC_ALRM_HU(hours % 10U) |
         RTC_ALRM_MNT(minutes / 10U) |
         RTC_ALRM_MNU(minutes % 10U) |
         RTC_ALRM_ST(seconds / 10U) |
         RTC_ALRM_SU(seconds % 10U);
}

static void wait_button_release(void) {

  while (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
    chThdSleepMilliseconds(50);
  }
}

static void rtc_basic_test(void) {
  rtc_datetime_t setdt, getdt;
  rtc_time64_t settv, gettv;
  msg_t msg;

  setdt.year = 46U;
  setdt.month = 4U;
  setdt.day = 7U;
  setdt.dayofweek = RTC_DAY_MONDAY;
  setdt.dstflag = 0U;
  setdt.millisecond = 12U * 3600000U + 34U * 60000U + 56U * 1000U;

  msg = rtcSetDateTime(&RTCD1, &setdt);
  test_assert(msg == HAL_RET_SUCCESS);

  msg = rtcGetDateTime(&RTCD1, &getdt);
  test_assert(msg == HAL_RET_SUCCESS);
  test_assert(getdt.year == setdt.year);
  test_assert(getdt.month == setdt.month);
  test_assert(getdt.day == setdt.day);
  test_assert((getdt.millisecond / 1000U) == (setdt.millisecond / 1000U));

  msg = rtcConvertDateTimeToTime64(&setdt, &settv);
  test_assert(msg == HAL_RET_SUCCESS);
  msg = rtcGetTime64(&RTCD1, &gettv);
  test_assert(msg == HAL_RET_SUCCESS);
  test_assert(gettv.tv_sec == settv.tv_sec);
}

static void rtc_alarm_test(void) {
  rtc_datetime_t basetime, alarmtime;
  rtc_alarm_t alarmspec;
  rtceventflags_t flags;
  msg_t msg;
  unsigned retries;

  msg = rtcGetDateTime(&RTCD1, &basetime);
  test_assert(msg == HAL_RET_SUCCESS);

  alarmtime = basetime;
  datetime_add_seconds(&alarmtime, RTC_TEST_ALARM_A_SECONDS);
  alarmspec.alrmr = encode_alarm(&alarmtime);
  msg = rtcSetAlarm(&RTCD1, 0U, &alarmspec);
  test_assert(msg == HAL_RET_SUCCESS);

#if RTC_ALARMS > 1
  alarmtime = basetime;
  datetime_add_seconds(&alarmtime, RTC_TEST_ALARM_B_SECONDS);
  alarmspec.alrmr = encode_alarm(&alarmtime);
  msg = rtcSetAlarm(&RTCD1, 1U, &alarmspec);
  test_assert(msg == HAL_RET_SUCCESS);
#endif

  retries = 0U;
  flags = 0U;
  while ((flags & RTC_FLAGS_ALARM_A) == 0U) {
    chThdSleepMilliseconds(200);
    flags |= rtcGetAndClearEventsX(&RTCD1, (rtceventflags_t)-1);
    retries++;
    test_assert(retries < 50U);
  }

#if RTC_ALARMS > 1
  while ((flags & RTC_FLAGS_ALARM_B) == 0U) {
    chThdSleepMilliseconds(200);
    flags |= rtcGetAndClearEventsX(&RTCD1, (rtceventflags_t)-1);
    retries++;
    test_assert(retries < 80U);
  }
#endif

  test_assert(rtc_cb_count > 0U);
  test_assert((rtcGetEventsX(&RTCD1) & (RTC_FLAGS_ALARM_A | RTC_FLAGS_ALARM_B)) == 0U);

  msg = rtcSetAlarm(&RTCD1, 0U, NULL);
  test_assert(msg == HAL_RET_SUCCESS);
#if RTC_ALARMS > 1
  msg = rtcSetAlarm(&RTCD1, 1U, NULL);
  test_assert(msg == HAL_RET_SUCCESS);
#endif
}

static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;

  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
  }
}

int main(void) {

  halInit();
  chSysInit();

  portab_setup();
  wait_button_release();

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  test_assert(drvStart(&RTCD1, NULL) == HAL_RET_SUCCESS);
  drvSetCallbackX(&RTCD1, rtc_test_cb);

  rtc_basic_test();
  rtc_alarm_test();

  while (true) {
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_ON);
    chThdSleepMilliseconds(50);
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_OFF);
    chThdSleepMilliseconds(950);
  }
}
