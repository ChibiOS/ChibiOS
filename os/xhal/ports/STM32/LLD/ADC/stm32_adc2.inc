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
 * @file    ADC/stm32_adc2.inc
 * @brief   Shared ADC2 handler.
 *
 * @addtogroup STM32_ADC2_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_ADC2)
#error "STM32_HAS_ADC2 not defined in registry"
#endif

#if STM32_HAS_ADC2

/* Checks on vector names and numbers definitions.*/
#if !defined(STM32_ADC2_NUMBER)
#error "STM32_ADC2_NUMBER not defined in stm32_isr.h"
#endif
#if !defined(STM32_ADC2_HANDLER)
#error "STM32_ADC2_HANDLER not defined in stm32_isr.h"
#endif

/* Checks for xmcuconf.h correctness.*/
#if !defined(STM32_ADC_USE_ADC2)
#error "STM32_ADC_USE_ADC2 not defined in xmcuconf.h"
#endif

/* Priority definitions checks.*/
#if !defined(STM32_IRQ_ADC2_PRIORITY)
#error "STM32_IRQ_ADC2_PRIORITY not defined in xmcuconf.h"
#endif

/* Priority settings checks.*/
#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_ADC2_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_ADC2_PRIORITY"
#endif

#endif /* STM32_HAS_ADC2 */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if HAL_USE_ADC && STM32_ADC_USE_ADC2 &&                                    \
    (!defined(STM32_ADC1_NUMBER) || (STM32_ADC1_NUMBER != STM32_ADC2_NUMBER))
#define STM32_ADC2_ISR_IS_USED              TRUE
#else
#define STM32_ADC2_ISR_IS_USED              FALSE
#endif

static inline void adc2_irq_init(void) {
#if STM32_ADC2_ISR_IS_USED
  nvicEnableVector(STM32_ADC2_NUMBER, STM32_IRQ_ADC2_PRIORITY);
#endif
}

static inline void adc2_irq_deinit(void) {
#if STM32_ADC2_ISR_IS_USED
  nvicDisableVector(STM32_ADC2_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_ADC2_ISR_IS_USED || defined(__DOXYGEN__)
/**
 * @brief   ADC2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_ADC2_HANDLER) {
  uint32_t isr;

  OSAL_IRQ_PROLOGUE();

  isr  = ADC2->ISR;
#if defined(STM32_ADC_ADC2_IRQ_HOOK)
  STM32_ADC_ADC2_IRQ_HOOK
#endif
  adc_lld_serve_interrupt(&ADCD2);
  (void)isr;

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */
