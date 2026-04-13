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
 * @file    ADC/stm32_adc3.inc
 * @brief   Shared ADC3 handler.
 *
 * @addtogroup STM32_ADC3_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_ADC3)
#error "STM32_HAS_ADC3 not defined in registry"
#endif

#if STM32_HAS_ADC3

/* Checks on vector names and numbers definitions.*/
#if !defined(STM32_ADC3_NUMBER)
#error "STM32_ADC3_NUMBER not defined in stm32_isr.h"
#endif
#if !defined(STM32_ADC3_HANDLER)
#error "STM32_ADC3_HANDLER not defined in stm32_isr.h"
#endif

/* Checks for xmcuconf.h correctness.*/
#if !defined(STM32_ADC_USE_ADC3)
#error "STM32_ADC_USE_ADC3 not defined in xmcuconf.h"
#endif

#if !defined(STM32_IRQ_ADC3_PRIORITY)
#error "STM32_IRQ_ADC3_PRIORITY not defined in xmcuconf.h"
#endif

/* Priority settings checks.*/
#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_ADC3_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_ADC3_PRIORITY"
#endif

#endif /* STM32_HAS_ADC3 */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if HAL_USE_ADC && STM32_ADC_USE_ADC3
#define STM32_ADC3_ISR_IS_USED              TRUE
#else
#define STM32_ADC3_ISR_IS_USED              FALSE
#endif

static inline void adc3_irq_init(void) {
#if STM32_ADC3_ISR_IS_USED
  nvicEnableVector(STM32_ADC3_NUMBER, STM32_IRQ_ADC3_PRIORITY);
#endif
}

static inline void adc3_irq_deinit(void) {
#if STM32_ADC3_ISR_IS_USED
  nvicDisableVector(STM32_ADC3_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_ADC3_ISR_IS_USED || defined(__DOXYGEN__)
/**
 * @brief   ADC3 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_ADC3_HANDLER) {
  uint32_t isr;

  OSAL_IRQ_PROLOGUE();

  isr  = ADC3->ISR;
#if defined(STM32_ADC_ADC3_IRQ_HOOK)
  STM32_ADC_ADC3_IRQ_HOOK
#endif
  adc_lld_serve_interrupt(&ADCD3);
  (void)isr;

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */
