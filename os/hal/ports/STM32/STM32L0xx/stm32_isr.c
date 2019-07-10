/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    STM32L0xx/stm32_isr.h
 * @brief   STM32L0xx ISR handler code.
 *
 * @addtogroup SRM32L0xx_ISR
 * @{
 */

#include "hal.h"

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

#define exti_serve_irq(pr, channel) {                                       \
                                                                            \
  if ((pr) & (1U << (channel))) {                                           \
    _pal_isr_code(channel);                                                 \
  }                                                                         \
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (HAL_USE_PAL && (PAL_USE_WAIT || PAL_USE_CALLBACKS)) || defined(__DOXYGEN__)
#if !defined(STM32_DISABLE_EXTI0_1_HANDLER)
/**
 * @brief   EXTI[0]...EXTI[1] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector54) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= ((1U << 0) | (1U << 1));
  EXTI->PR = pr;

  exti_serve_irq(pr, 0);
  exti_serve_irq(pr, 1);

  OSAL_IRQ_EPILOGUE();
}
#endif

/**
 * @brief   EXTI[2]...EXTI[3] interrupt handler.
 *
 * @isr
 */
#if !defined(STM32_DISABLE_EXTI2_3_HANDLER)
OSAL_IRQ_HANDLER(Vector58) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= ((1U << 2) | (1U << 3));
  EXTI->PR = pr;

  exti_serve_irq(pr, 2);
  exti_serve_irq(pr, 3);

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(STM32_DISABLE_EXTI4_15_HANDLER)
/**
 * @brief   EXTI[4]...EXTI[15] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector5C) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= ((1U << 4)  | (1U << 5)  | (1U << 6)  | (1U << 7)  | (1U << 8)  |
         (1U << 9)  | (1U << 10) | (1U << 11) | (1U << 12) | (1U << 13) |
         (1U << 14) | (1U << 15));
  EXTI->PR = pr;

  exti_serve_irq(pr, 4);
  exti_serve_irq(pr, 5);
  exti_serve_irq(pr, 6);
  exti_serve_irq(pr, 7);
  exti_serve_irq(pr, 8);
  exti_serve_irq(pr, 9);
  exti_serve_irq(pr, 10);
  exti_serve_irq(pr, 11);
  exti_serve_irq(pr, 12);
  exti_serve_irq(pr, 13);
  exti_serve_irq(pr, 14);
  exti_serve_irq(pr, 15);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif /* HAL_USE_PAL && (PAL_USE_WAIT || PAL_USE_CALLBACKS) */

#if HAL_USE_SERIAL || HAL_USE_UART || defined(__DOXYGEN__)
#if !defined(STM32_DISABLE_USART1_HANDLER)
#if STM32_SERIAL_USE_USART1 || STM32_UART_USE_USART1
/**
 * @brief   USART1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_USART1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_SERIAL
#if STM32_SERIAL_USE_USART1
  sd_lld_serve_interrupt(&SD1);
#endif
#endif
#if HAL_USE_UART
#if STM32_UART_USE_USART1
  uart_lld_serve_interrupt(&UARTD1);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if !defined(STM32_DISABLE_USART2_HANDLER)
#if STM32_SERIAL_USE_USART2 || STM32_UART_USE_USART2
/**
 * @brief   USART2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_USART2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_SERIAL
#if STM32_SERIAL_USE_USART2
  sd_lld_serve_interrupt(&SD2);
#endif
#endif
#if HAL_USE_UART
#if STM32_UART_USE_USART2
  uart_lld_serve_interrupt(&UARTD2);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if !defined(STM32_DISABLE_UART4_5_HANDLER)
#if STM32_SERIAL_USE_UART4 || STM32_SERIAL_USE_UART5 ||                     \
    STM32_UART_USE_UART4   || STM32_UART_USE_UART5
/**
 * @brief   UART4 and 5 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_UART4_5_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_SERIAL
#if STM32_SERIAL_USE_UART4
  sd_lld_serve_interrupt(&SD4);
#endif
#if STM32_SERIAL_USE_UART5
  sd_lld_serve_interrupt(&SD5);
#endif
#endif
#if HAL_USE_UART
#if STM32_UART_USE_UART4
  uart_lld_serve_interrupt(&UARTD4);
#endif
#if STM32_UART_USE_UART5
  uart_lld_serve_interrupt(&UARTD5);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if !defined(STM32_DISABLE_LPUART1_HANDLER)
#if STM32_SERIAL_USE_LPUART1
/**
 * @brief   LPUART1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_LPUART1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  sd_lld_serve_interrupt(&LPSD1);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif
#endif /* HAL_USE_SERIAL || HAL_USE_UART */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables IRQ sources.
 *
 * @notapi
 */
void irqInit(void) {

#if HAL_USE_PAL
  nvicEnableVector(STM32_EXTI_LINE01_NUMBER, STM32_IRQ_EXTI0_1_PRIORITY);
  nvicEnableVector(STM32_EXTI_LINE23_NUMBER, STM32_IRQ_EXTI2_3_PRIORITY);
  nvicEnableVector(STM32_EXTI_LINE4_15_NUMBER, STM32_IRQ_EXTI4_15_PRIORITY);
  nvicEnableVector(STM32_EXTI_LINE16_NUMBER, STM32_IRQ_EXTI16_PRIORITY);
#endif

#if HAL_USE_SERIAL || HAL_USE_UART
  nvicEnableVector(STM32_USART1_NUMBER, STM32_IRQ_USART1_PRIORITY);
  nvicEnableVector(STM32_USART2_NUMBER, STM32_IRQ_USART2_PRIORITY);
  nvicEnableVector(STM32_UART4_5_NUMBER, STM32_IRQ_UART4_5_PRIORITY);
  nvicEnableVector(STM32_LPUART1_NUMBER, STM32_IRQ_LPUART1_PRIORITY);
#endif
}

/**
 * @brief   Disables IRQ sources.
 *
 * @notapi
 */
void irqDeinit(void) {

#if HAL_USE_PAL
  nvicDisableVector(STM32_EXTI_LINE01_NUMBER);
  nvicDisableVector(STM32_EXTI_LINE23_NUMBER);
  nvicDisableVector(STM32_EXTI_LINE4_15_NUMBER);
  nvicDisableVector(STM32_EXTI_LINE16_NUMBER);
  nvicDisableVector(STM32_EXTI_LINE2122_NUMBER);
#endif

#if HAL_USE_SERIAL || HAL_USE_UART
  nvicDisableVector(STM32_USART1_NUMBER);
  nvicDisableVector(STM32_USART2_NUMBER);
  nvicDisableVector(STM32_UART4_5_NUMBER);
  nvicDisableVector(STM32_LPUART1_NUMBER);
#endif
}

/** @} */
