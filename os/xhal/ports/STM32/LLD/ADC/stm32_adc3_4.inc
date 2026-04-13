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
 * @file    ADC/stm32_adc3_4.inc
 * @brief   Shared ADC3, ADC4 handler.
 *
 * @addtogroup STM32_ADC3_4_HANDLER
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
#if !defined(STM32_HAS_ADC4)
#error "STM32_HAS_ADC4 not defined in registry"
#endif

#if STM32_HAS_ADC3 || STM32_HAS_ADC4

/* Checks on vector names and numbers definitions.*/
#if !defined(STM32_ADC3_NUMBER)
#error "STM32_ADC3_NUMBER not defined in stm32_isr.h"
#endif
#if !defined(STM32_ADC4_NUMBER)
#error "STM32_ADC4_NUMBER not defined in stm32_isr.h"
#endif
#if !defined(STM32_ADC4_HANDLER)
#error "STM32_ADC4_HANDLER not defined in stm32_isr.h"
#endif

/* Checks for xmcuconf.h correctness.*/
#if !defined(STM32_ADC_USE_ADC3)
#error "STM32_ADC_USE_ADC3 not defined in xmcuconf.h"
#endif
#if !defined(STM32_ADC_DUAL_MODE)
#error "STM32_ADC_DUAL_MODE not defined in xmcuconf.h"
#endif

/* Priority definitions checks.*/
#if !defined(STM32_IRQ_ADC4_PRIORITY)
#error "STM32_IRQ_ADC4_PRIORITY not defined in xmcuconf.h"
#endif

/* Priority settings checks.*/
#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_ADC4_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_ADC4_PRIORITY"
#endif

#endif /* STM32_HAS_ADC3 || STM32_HAS_ADC4 */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if HAL_USE_ADC && STM32_ADC_DUAL_MODE && STM32_ADC_USE_ADC3
#define STM32_ADC3_ADC4_ISR_IS_USED         TRUE
#else
#define STM32_ADC3_ADC4_ISR_IS_USED         FALSE
#endif

static inline void adc3_adc4_irq_init(void) {
#if STM32_ADC3_ADC4_ISR_IS_USED
  nvicEnableVector(STM32_ADC4_NUMBER, STM32_IRQ_ADC4_PRIORITY);
#endif
}

static inline void adc3_adc4_irq_deinit(void) {
#if STM32_ADC3_ADC4_ISR_IS_USED
  nvicDisableVector(STM32_ADC4_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_ADC3_ADC4_ISR_IS_USED || defined(__DOXYGEN__)
/**
 * @brief   ADC4 interrupt handler (as ADC3 slave).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_ADC4_HANDLER) {
  uint32_t isr;

  OSAL_IRQ_PROLOGUE();

  isr  = ADC4->ISR;
#if defined(STM32_ADC_ADC4_IRQ_HOOK)
  STM32_ADC_ADC4_IRQ_HOOK
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
