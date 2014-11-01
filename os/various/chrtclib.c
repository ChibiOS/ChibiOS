/*
    ChibiOS - Copyright (C) 2014 Uladzimir Pylinsky aka barthess

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

/**
 * @file    chrtclib.c
 * @brief   Time conversion routines.
 *
 * @addtogroup chrtclib
 * @{
 */

#include <time.h>

#include "chrtclib.h"

/**
 * @brief   Convert @p RTCDateTime to broken-down time structure.
 *
 * @param[in]  timespec   pointer to a @p RTCDateTime structure
 * @param[out] timp       pointer to a broken-down time structure
 *
 * @api
 */
void rtcConvertDateTimeToStructTm(const RTCDateTime *timespec,
                                              struct tm *timp) {
  uint32_t tmp;

  timp->tm_year  = timespec->year + (1980 - 1900);
  timp->tm_mon   = timespec->month - 1;
  timp->tm_mday  = timespec->day;
  timp->tm_isdst = timespec->dstflag;

  tmp = timespec->millisecond / 1000;
  timp->tm_sec  = tmp % 60;
  tmp -= timp->tm_sec;
  timp->tm_min  = tmp % 3600;
  tmp -= timp->tm_min * 60;
  timp->tm_hour = tmp / 3600;
}

/**
 * @brief   Convert broken-down time structure to @p RTCDateTime.
 *
 * @param[in] timp        pointer to a broken-down time structure
 * @param[in] tv_msec     milliseconds value
 * @param[out] timespec   pointer to a @p RTCDateTime structure
 *
 * @api
 */
void rtcConvertStructTmToDateTime(const struct tm *timp,
                          uint32_t tv_msec, RTCDateTime *timespec) {

  timespec->year      = timp->tm_year - (1980 - 1900);
  timespec->month     = timp->tm_mon + 1;
  timespec->day       = timp->tm_mday;
  timespec->dayofweek = timp->tm_wday + 1;
  if (-1 == timp->tm_isdst)
    timespec->dstflag = 0;  /* set zero if dst is unknown */
  else
    timespec->dstflag = timp->tm_isdst;
  timespec->millisecond = tv_msec +
      (timp->tm_hour * 3600 + timp->tm_min * 60 + timp->tm_sec) * 1000;
}

/*
 * Lookup table for months' length
 */
static const uint8_t month_len[12] = {
    31, 30, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/**
 * @brief   Get current time in format suitable for usage in FAT file system.
 * @note    The information about day of week and DST is lost in DOS
 *          format, the second field loses its least significant bit.
 *
 * @param[out] timespec pointer to a @p RTCDateTime structure
 * @return              FAT date/time value.
 *
 * @api
 */
uint32_t rtcConvertDateTimeToFAT(const RTCDateTime *timespec) {
  uint32_t fattime;
  uint32_t sec, min, hour, day, month, tmp;

  tmp = timespec->millisecond / 1000;
  sec = tmp % 60;
  min = (tmp - sec) % 3600;
  hour = (tmp - sec - min * 60) / 3600;
  day = timespec->day;
  month = timespec->month;

  /* handle DST flag */
  if (1 == timespec->dstflag) {
    hour += 1;
    if (hour == 24) {
      hour = 0;
      day += 1;
      if (day > month_len[month - 1]) {
        day = 1;
        month += 1;
      }
    }
  }

  fattime  = sec            >> 1;
  fattime |= min            << 5;
  fattime |= hour           << 11;
  fattime |= day            << 16;
  fattime |= month          << 21;
  fattime |= timespec->year << 25;

  return fattime;
}

/** @} */
