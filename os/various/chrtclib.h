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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    chrtclib.h
 * @brief   RTC time conversion utilities header.
 *
 * @addtogroup chrtclib
 * @{
 */

#ifndef CHRTCLIB_H_
#define CHRTCLIB_H_

#include <time.h>

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#if !STM32_RTC_IS_CALENDAR
  uint32_t rtcGetTimeFat(RTCDriver *rtcp);
#endif
  void rtcGetTimeTm(RTCDriver *rtcp, struct tm *timp);
  void rtcSetTimeTm(RTCDriver *rtcp, struct tm *timp);
  time_t rtcGetTimeUnixSec(RTCDriver *rtcp);
  uint64_t rtcGetTimeUnixUsec(RTCDriver *rtcp);
  void rtcSetTimeUnixSec(RTCDriver *rtcp, time_t tv_sec);
#ifdef __cplusplus
}
#endif

#endif /* CHRTCLIB_H_ */

/** @} */
