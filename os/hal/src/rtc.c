/**
 * @file    rtc.c
 * @brief   Real Time Clock Abstraction Layer code.
 *
 * @addtogroup RTC
 * @{
 */

#include "ch.h"
#include "hal.h"

#include "rtc_lld.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/
/**
 * @brief   Enable access to registers and initialize RTC if BKP doamin
 *          was previously reseted.
 */
void rtcInit(void){
  rtc_lld_init();
}

#if RTC_SUPPORTS_CALLBACKS
/**
 * @brief   Configure and start interrupt servicing routines.
 * @param[in] rtcp - pointer to RTC driver structure.
 * @param[in] rtccfgp - pointer to RTC config structure.
 */
void rtcStart(RTCDriver *rtcp, const RTCConfig *rtccfgp){
  chDbgCheck(((rtcp != NULL) && (rtccfgp != NULL)), "rtcStart");
  rtc_lld_start(rtcp, rtccfgp);
}

/**
 * @brief   Stop interrupt servicing routines.
 */
void rtcStop(void){
  rtc_lld_stop();
}
#endif /* RTC_SUPPORTS_CALLBACKS */

/**
 * @brief     Set current time.
 * @param[in] tv_sec - time value in UNIX notation.
 */
void rtcSetTime(uint32_t tv_sec){
  rtc_lld_set_time(tv_sec);
}

/**
 * @brief Return current time in UNIX notation.
 */
uint32_t rtcGetSec(void){
  return rtc_lld_get_sec();
}

/**
 * @brief Return fractional part of current time (milliseconds).
 */
uint16_t rtcGetMsec(void){
  return rtc_lld_get_msec();
}

/**
 * @brief Set alarm date in UNIX notation.
 */
void rtcSetAlarm(uint32_t tv_alarm){
  rtc_lld_set_alarm(tv_alarm);
}

/**
 * @brief Get current alarm date in UNIX notation.
 */
uint32_t rtcGetAlarm(void){
  return rtc_lld_get_alarm();
}






#endif /* HAL_USE_RTC */



/** @} */


