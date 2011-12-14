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
#include <time.h>

#include "ch.h"
#include "hal.h"

RTCTime  timespec;
RTCAlarm alarmspec;
RTCCallbackConfig cb_cfg;
time_t unix_time;

/**
 * Alarms callback
 */
static inline void exti_rtcalarm_cb(EXTDriver *extp, expchannel_t channel){
  (void)extp;
  (void)channel;
  if (RTCD1.id_rtc->ISR | RTC_ISR_ALRBF){
    RTCD1.id_rtc->ISR &= ~RTC_ISR_ALRBF;
  }
  if (RTCD1.id_rtc->ISR | RTC_ISR_ALRAF){
    RTCD1.id_rtc->ISR &= ~RTC_ISR_ALRAF;
  }
}

/**
 * Wakeup callback
 */
static inline void exti_rtcwakeup_cb(EXTDriver *extp, expchannel_t channel){
  (void)extp;
  (void)channel;
  if (RTCD1.id_rtc->ISR | RTC_ISR_WUTF){
    RTCD1.id_rtc->ISR &= ~RTC_ISR_WUTF;
  }
}


static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_RISING_EDGE, exti_rtcalarm_cb},// RTC alarms
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},// timestamp
    {EXT_CH_MODE_RISING_EDGE, exti_rtcwakeup_cb},// wakeup
  },
  EXT_MODE_EXTI(
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0)//15
};

/**
 * Convert from STM32 BCD to classical format.
 */
void bcd2tm(struct tm *timp, uint32_t tv_time, uint32_t tv_date){
  timp->tm_isdst = -1;

  timp->tm_wday = ((tv_date >> 13) & 0x7);
  if(timp->tm_wday == 7)
    timp->tm_wday = 0;
  timp->tm_mday = (tv_date & 0xF) + ((tv_date >> 4) & 0x3) * 10;
  timp->tm_mon  = (((tv_date >> 8) & 0xF) + ((tv_date >> 12) & 0x1) * 10) - 1;
  timp->tm_year = (((tv_date >> 16)& 0xF) + ((tv_date >> 20) & 0xF) * 10) + 2000 - 1900;

  timp->tm_sec  = (tv_time & 0xF) + ((tv_time >> 4) & 0x7) * 10;
  timp->tm_min  = ((tv_time >> 8)& 0xF) + ((tv_time >> 12) & 0x7) * 10;
  timp->tm_hour = ((tv_time >> 16)& 0xF) + ((tv_time >> 20) & 0x3) * 10;
}

/**
 * Convert from classical format to STM32 BCD
 */
void tm2bcd(struct tm *timp, RTCTime *timespec){
  uint32_t p = 0; // temporal variable

  timespec->tv_date = 0;
  timespec->tv_time = 0;

  p = timp->tm_year - 100;
  timespec->tv_date |= (((p / 10) & 0xF) << 20) | ((p % 10) << 16);
  if (timp->tm_wday == 0)
    p = 7;
  else
    p = timp->tm_wday;
  timespec->tv_date |= (p & 7) << 13;
  p = timp->tm_mon + 1;
  timespec->tv_date |= (((p / 10) & 1) << 12) | ((p % 10) << 8);
  p = timp->tm_mday;
  timespec->tv_date |= (((p / 10) & 3) << 4) | (p % 10);
  p = timp->tm_hour;
  timespec->tv_time |= (((p / 10) & 3) << 20) | ((p % 10) << 16);
  p = timp->tm_min;
  timespec->tv_time |= (((p / 10) & 7) << 12) | ((p % 10) << 8);
  p = timp->tm_sec;
  timespec->tv_time |= (((p / 10) & 7) << 4) | (p % 10);
}



/**
 * Main function.
 */
int main(void){
  struct tm timp;

  halInit();
  chSysInit();

  extStart(&EXTD1, &extcfg);

  /* get current time in unix format */
  rtcGetTime(&RTCD1, &timespec);
  bcd2tm(&timp, timespec.tv_time, timespec.tv_date);
  unix_time = mktime(&timp);



  while (TRUE){
    chThdSleepMilliseconds(500);
  }
  return 0;
}

