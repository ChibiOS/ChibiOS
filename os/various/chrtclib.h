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

#ifndef _CHRTCLIB_H_
#define _CHRTCLIB_H_

#include <time.h>
#include <stdint.h>

/**
 * @file    chrtclib.h
 * @brief   Time conversion routines.
 *
 * @addtogroup chrtclib
 * @{
 */

/**
 * @brief   Type of a structure representing an RTC date/time stamp.
 */
typedef struct {
  uint32_t      year: 8;            /**< @brief Years since 1980.           */
  uint32_t      month: 4;           /**< @brief Months 1..12.               */
  uint32_t      dstflag: 1;         /**< @brief DST correction flag.        */
  uint32_t      dayofweek: 3;       /**< @brief Day of week 1..7.           */
  uint32_t      day: 5;             /**< @brief Day of the month 1..31.     */
  uint32_t      millisecond: 27;    /**< @brief Milliseconds since midnight.*/
} RTCDateTime;

#ifdef __cplusplus
extern "C" {
#endif
  uint32_t rtcConvertDateTimeToFAT(const RTCDateTime *timespec);
  void rtcConvertDateTimeToStructTm(const RTCDateTime *timespec,
                                    struct tm *timp);
  void rtcConvertStructTmToDateTime(const struct tm *timp,
                                    uint32_t tv_msec,
                                    RTCDateTime *timespec);
#ifdef __cplusplus
}
#endif


#endif /* _CHRTCLIB_H_ */

/** @} */
