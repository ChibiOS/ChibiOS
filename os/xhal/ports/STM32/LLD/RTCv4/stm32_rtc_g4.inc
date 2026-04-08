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

/**
 * @file    RTCv4/stm32_rtc_g4.inc
 * @brief   Shared RTCv4 G4 interrupt handlers.
 *
 * @addtogroup STM32_RTC_HANDLER
 * @{
 */

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_RTC)
#error "STM32_HAS_RTC not defined in registry"
#endif

#if STM32_HAS_RTC

/* Checks on vector names and numbers definitions.*/
#if !defined(STM32_RTC_TAMP_STAMP_NUMBER)
#error "STM32_RTC_TAMP_STAMP_NUMBER not defined in stm32_isr.h"
#endif
#if !defined(STM32_RTC_TAMP_STAMP_HANDLER)
#error "STM32_RTC_TAMP_STAMP_HANDLER not defined in stm32_isr.h"
#endif
#if !defined(STM32_RTC_WKUP_NUMBER)
#error "STM32_RTC_WKUP_NUMBER not defined in stm32_isr.h"
#endif
#if !defined(STM32_RTC_WKUP_HANDLER)
#error "STM32_RTC_WKUP_HANDLER not defined in stm32_isr.h"
#endif
#if !defined(STM32_RTC_ALARM_NUMBER)
#error "STM32_RTC_ALARM_NUMBER not defined in stm32_isr.h"
#endif
#if !defined(STM32_RTC_ALARM_HANDLER)
#error "STM32_RTC_ALARM_HANDLER not defined in stm32_isr.h"
#endif

/* Checks for mcuconf.h correctness.*/
#if !defined(STM32_IRQ_RTC_TAMP_STAMP_PRIORITY)
#error "STM32_IRQ_RTC_TAMP_STAMP_PRIORITY not defined in mcuconf.h"
#endif
#if !defined(STM32_IRQ_RTC_WKUP_PRIORITY)
#error "STM32_IRQ_RTC_WKUP_PRIORITY not defined in mcuconf.h"
#endif
#if !defined(STM32_IRQ_RTC_ALARM_PRIORITY)
#error "STM32_IRQ_RTC_ALARM_PRIORITY not defined in mcuconf.h"
#endif

/* Priority settings checks.*/
#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_RTC_TAMP_STAMP_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_RTC_TAMP_STAMP_PRIORITY"
#endif
#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_RTC_WKUP_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_RTC_WKUP_PRIORITY"
#endif
#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_RTC_ALARM_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_RTC_ALARM_PRIORITY"
#endif

#endif /* STM32_HAS_RTC */

static inline void rtc_irq_init(void) {
#if HAL_USE_RTC
  nvicEnableVector(STM32_RTC_TAMP_STAMP_NUMBER,
                   STM32_IRQ_RTC_TAMP_STAMP_PRIORITY);
  nvicEnableVector(STM32_RTC_WKUP_NUMBER,
                   STM32_IRQ_RTC_WKUP_PRIORITY);
  nvicEnableVector(STM32_RTC_ALARM_NUMBER,
                   STM32_IRQ_RTC_ALARM_PRIORITY);
#endif
}

static inline void rtc_irq_deinit(void) {
#if HAL_USE_RTC
  nvicDisableVector(STM32_RTC_TAMP_STAMP_NUMBER);
  nvicDisableVector(STM32_RTC_WKUP_NUMBER);
  nvicDisableVector(STM32_RTC_ALARM_NUMBER);
#endif
}

#if HAL_USE_RTC || defined(__DOXYGEN__)
/**
 * @brief   RTC tamper/time-stamp interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_RTC_TAMP_STAMP_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  rtc_lld_serve_interrupt();
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   RTC wakeup interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_RTC_WKUP_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  rtc_lld_serve_interrupt();
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   RTC alarm interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_RTC_ALARM_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  rtc_lld_serve_interrupt();
  OSAL_IRQ_EPILOGUE();
}
#endif

/** @} */
