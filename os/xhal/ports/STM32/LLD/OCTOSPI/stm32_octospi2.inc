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
 * @file    OCTOSPI/stm32_octospi2.inc
 * @brief   Shared OCTOSPI2 handler.
 *
 * @addtogroup STM32_OCTOSPI2_HANDLER
 * @{
 */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_HAS_OCTOSPI2)
#error "STM32_HAS_OCTOSPI2 not defined in registry"
#endif

#if STM32_HAS_OCTOSPI2

#if !defined(STM32_OCTOSPI2_NUMBER)
#error "STM32_OCTOSPI2_NUMBER not defined in stm32_isr.h"
#endif
#if !defined(STM32_OCTOSPI2_HANDLER)
#error "STM32_OCTOSPI2_HANDLER not defined in stm32_isr.h"
#endif

#if !defined(STM32_WSPI_USE_OCTOSPI2)
#error "STM32_WSPI_USE_OCTOSPI2 not defined in xmcuconf.h"
#endif

#if !defined(STM32_IRQ_OCTOSPI2_PRIORITY)
#error "STM32_IRQ_OCTOSPI2_PRIORITY not defined in xmcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_OCTOSPI2_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_OCTOSPI2_PRIORITY"
#endif

#endif /* STM32_HAS_OCTOSPI2 */

#if (HAL_USE_WSPI && STM32_WSPI_USE_OCTOSPI2)
#define STM32_OCTOSPI2_IS_USED             TRUE
#else
#define STM32_OCTOSPI2_IS_USED             FALSE
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void octospi2_irq_init(void) {
#if STM32_OCTOSPI2_IS_USED
  nvicEnableVector(STM32_OCTOSPI2_NUMBER, STM32_IRQ_OCTOSPI2_PRIORITY);
#endif
}

static inline void octospi2_irq_deinit(void) {
#if STM32_OCTOSPI2_IS_USED
  nvicDisableVector(STM32_OCTOSPI2_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_OCTOSPI2_IS_USED || defined(__DOXYGEN__)
/**
 * @brief   OCTOSPI2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_OCTOSPI2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  wspi_lld_serve_interrupt(&WSPID2);

  OSAL_IRQ_EPILOGUE();
}
#endif

/** @} */
