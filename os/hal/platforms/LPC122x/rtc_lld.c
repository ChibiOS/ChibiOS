/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC122x RTC driver - Copyright (C) 2013 Marcin Jokel

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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    LPC122x/rtc_lld.c
 * @brief   LPC122x RTC low level driver.
 *
 * @addtogroup RTC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_RTC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief RTC driver identifier.
 */
RTCDriver RTCD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/


/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   RTC interrupt handler.
 *
 * @isr
 */

#if LPC122x_RTC_USE_ALARM || defined(__DOXYGEN__)
CH_IRQ_HANDLER(VectorB8) {
  uint32_t flag;

  CH_IRQ_PROLOGUE();

  flag = LPC_RTC->RIS;
  LPC_RTC->ICR = flag;    /* Clear interrupt flag */

  if ((flag & RIS_RTCRIS) && (RTCD1.callback != NULL))
    RTCD1.callback(&RTCD1, RTC_EVENT_ALARM);

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enable access to registers.
 *
 * @api
 */
void rtc_lld_init(void) {
  uint32_t temp[2];

  if(LPC_SYSCON->SYSRESSTAT & 0x1) {            /* POR detected */

    LPC_SYSCON->SYSAHBCLKCTRL &= ~(1UL << 19);  /* Disable clock for RTC */
    LPC_PMU->SYSCFG &= ~(0x0FUL << 11);         /* Clear RTC clock source */
    LPC_SYSCON->RTCCLKDIV = LPC122x_RTC_CLKDIV;
    LPC_PMU->SYSCFG |= (LPC122x_RTCCLK << 11);  /* Set RTC clock source */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 19);   /* Enable clock for RTC registers*/
    LPC_RTC->ICR = 0x01;                        /* Clear interrupt flag */
    LPC_RTC->CR= 1;                             /* Enable RTC start */
    LPC_SYSCON->SYSRESSTAT = 0x1;               /* Clear POR flag */

    while(LPC_RTC->DR <3 ) {  /* Wait, data read not valid */
          __NOP();
    }
  }
  else {

    LPC_SYSCON->SYSAHBCLKCTRL |= (1UL << 19);   /* Enable clock for RTC registers*/

    do {
      temp[0] = LPC_RTC->DR;
      temp[1] = LPC_RTC->DR;
    } while(temp[0] == temp[1]);

#if LPC122x_RTC_USE_ALARM
   if (temp[1] >= LPC_RTC->MR) {  /* Check for match event in software, handle if found */
     CH_IRQ_HANDLER(VectorB8); /* Manually invoke ISR */
   }
#endif

  }

#if LPC122x_RTC_USE_ALARM
  nvicEnableVector(RTC_IRQn, CORTEX_PRIORITY_MASK(LPC122x_RTC_IRQ_PRIORITY));
#endif
  return;
}

/**
 * @brief   Set current time.
 * @note    Fractional part will be silently ignored. There is no possibility
 *          to set it on STM32 platform.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] timespec  pointer to a @p RTCTime structure
 *
 * @api
 */
void rtc_lld_set_time(RTCDriver *rtcp, const RTCTime *timespec) {
  (void)rtcp;

  LPC_RTC->LR = timespec->tv_sec;
}

/**
 * @brief   Get current time.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[out] timespec pointer to a @p RTCTime structure
 *
 * @api
 */
void rtc_lld_get_time(RTCDriver *rtcp, RTCTime *timespec) {
  (void)rtcp;

  timespec->tv_sec = LPC_RTC->DR;
}

/**
 * @brief     Set alarm time.
 *
 * @note      Default value after BKP domain reset for both comparators is 0.
 * @note      Function does not performs any checks of alarm time validity.
 *
 * @param[in] rtcp      Pointer to RTC driver structure.
 * @param[in] alarm     Alarm identifier. Can be 1 or 2.
 * @param[in] alarmspec Pointer to a @p RTCAlarm structure.
 *
 * @api
 */
void rtc_lld_set_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       const RTCAlarm *alarmspec) {
  (void)rtcp;
  (void)alarm;
  LPC_RTC->MR = alarmspec->tv_sec;

}

/**
 * @brief   Get alarm time.
 *
 * @param[in] rtcp       pointer to RTC driver structure
 * @param[in] alarm      alarm identifier
 * @param[out] alarmspec pointer to a @p RTCAlarm structure
 *
 * @api
 */
void rtc_lld_get_alarm(RTCDriver *rtcp,
                       rtcalarm_t alarm,
                       RTCAlarm *alarmspec) {
  (void)rtcp;
  (void)alarm;
  alarmspec->tv_sec = LPC_RTC->MR;

}

/**
 * @brief   Enables or disables RTC callbacks.
 * @details This function enables or disables callbacks, use a @p NULL pointer
 *          in order to disable a callback.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @param[in] callback  callback function pointer or @p NULL
 *
 * @notapi
 */
void rtc_lld_set_callback(RTCDriver *rtcp, rtccb_t callback) {

  LPC_RTC->ICR = 0x01;         /* Clear interrupt flag */

  if (callback != NULL) {

    /* IRQ sources enabled only after setting up the callback.*/
    rtcp->callback = callback;
    LPC_RTC->ICSC = ICSC_RTCCIC; /* Enable RTC interrupt */
  }
  else {

    LPC_RTC->ICSC = 0;           /* Disable RTC interrupt */

    /* Callback set to NULL only after disabling the IRQ sources.*/
    rtcp->callback = NULL;
  }
}

#include "chrtclib.h"

/**
 * @brief   Get current time in format suitable for usage in FatFS.
 *
 * @param[in] rtcp      pointer to RTC driver structure
 * @return              FAT time value.
 *
 * @api
 */
uint32_t rtc_lld_get_time_fat(RTCDriver *rtcp) {
  uint32_t fattime;
  struct tm timp;

  rtcGetTimeTm(rtcp, &timp);

  fattime  = (timp.tm_sec)       >> 1;
  fattime |= (timp.tm_min)       << 5;
  fattime |= (timp.tm_hour)      << 11;
  fattime |= (timp.tm_mday)      << 16;
  fattime |= (timp.tm_mon + 1)   << 21;
  fattime |= (timp.tm_year - 80) << 25;

  return fattime;
}

#endif /* HAL_USE_RTC */

/** @} */
