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

/**
 * @file    rtc.h
 * @brief   RTC Driver macros and structures.
 *
 * @addtogroup RTC
 * @{
 */


#ifndef _RTC_H_
#define _RTC_H_



#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef struct RTCDriver RTCDriver;

typedef void (*rtccb_t)(RTCDriver *rtcp);

#include "rtc_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void rtcInit(void);

  #if RTC_SUPPORTS_CALLBACKS
    void rtcStart(RTCDriver *rtcp, const RTCConfig *rtccfgp);
    void rtcStop(void);
  #else /* RTC_SUPPORTS_CALLBACKS */
    #define rtcStart(rtcp, rtccfgp)
    #define rtcStop()
  #endif /* RTC_SUPPORTS_CALLBACKS */

  void rtcSetTime(uint32_t tv_sec);
  uint32_t rtcGetSec(void);
  uint16_t rtcGetMsec(void);
  void rtcSetAlarm(uint32_t tv_alarm);
  uint32_t rtcGetAlarm(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_RTC */
#endif /* _RTC_H_ */

/** @} */
