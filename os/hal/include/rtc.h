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
/* TODO: move this to hal_lld_f103.h & mcuconf.h */
#define STM32_LSECLK             32768       /**< Low speed external clock.  */


/* RCC_CFGR register bits definitions.*/
#define STM32_RTC_NONE           (0 << 8)    /**< */
#define STM32_RTC_LSE            (1 << 8)    /**< LSE oscillator clock used as RTC clock */
#define STM32_RTC_LSI            (2 << 8)    /**< LSI oscillator clock used as RTC clock */
#define STM32_RTC_HSE            (3 << 8)    /**< HSE oscillator clock divided by 128 used as RTC clock */


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
  void rtcStart(RTCDriver *rtcp, RTCConfig *rtccfgp);
  void rtcStop(void);
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
