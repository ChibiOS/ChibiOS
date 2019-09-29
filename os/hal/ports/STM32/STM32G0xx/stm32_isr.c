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
 * @file    STM32G0xx/stm32_isr.h
 * @brief   STM32G0xx ISR handler code.
 *
 * @addtogroup STM32G0xx_ISR
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define exti_serve_irq(pr, channel) {                                       \
                                                                            \
  if ((pr) & (1U << (channel))) {                                           \
    _pal_isr_code(channel);                                                 \
  }                                                                         \
}

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
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#include "stm32_exti0_1.inc"
#include "stm32_exti2_3.inc"
#include "stm32_exti4_15.inc"

#include "stm32_usart1.inc"
#include "stm32_usart2.inc"
#include "stm32_usart3_usart4_lpuart1.inc"

#if STM32_HAS_TIM1
#include "stm32_tim1.inc"
#endif
#if STM32_HAS_TIM2
#include "stm32_tim2.inc"
#endif
#include "stm32_tim3.inc"
#include "stm32_tim6.inc"
#include "stm32_tim7.inc"
#include "stm32_tim14.inc"
#include "stm32_tim15.inc"
#include "stm32_tim16.inc"
#include "stm32_tim17.inc"

#if defined(STM32_DMA_REQUIRED) || defined(__DOXYGEN__)
/**
 * @brief   DMA1 streams 2 and 3 shared ISR.
 * @note    It is declared here because this device has a non-standard
 *          DMA shared IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH23_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  /* Check on channel 2.*/
  dmaServeInterrupt(STM32_DMA1_STREAM2);

  /* Check on channel 3.*/
  dmaServeInterrupt(STM32_DMA1_STREAM3);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 streams 4, 5, 6 and 7 shared ISR.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH4567_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  /* Check on channel 4.*/
  dmaServeInterrupt(STM32_DMA1_STREAM4);

  /* Check on channel 5.*/
  dmaServeInterrupt(STM32_DMA1_STREAM5);

  /* Check on channel 6.*/
  dmaServeInterrupt(STM32_DMA1_STREAM6);

  /* Check on channel 7.*/
  dmaServeInterrupt(STM32_DMA1_STREAM7);

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables IRQ sources.
 *
 * @notapi
 */
void irqInit(void) {

  exti0_1_irq_init();
  exti2_3_irq_init();
  exti4_15_irq_init();

#if STM32_HAS_TIM1
  tim1_irq_init();
#endif
#if STM32_HAS_TIM2
  tim2_irq_init();
#endif
  tim3_irq_init();
  tim6_irq_init();
  tim7_irq_init();
  tim14_irq_init();
  tim15_irq_init();
  tim16_irq_init();
  tim17_irq_init();

  usart1_irq_init();
  usart2_irq_init();
  usart3_usart4_lpuart1_irq_init();
}

/**
 * @brief   Disables IRQ sources.
 *
 * @notapi
 */
void irqDeinit(void) {

  exti0_1_irq_deinit();
  exti2_3_irq_deinit();
  exti4_15_irq_deinit();

#if STM32_HAS_TIM1
  tim1_irq_deinit();
#endif
#if STM32_HAS_TIM2
  tim2_irq_deinit();
#endif
  tim3_irq_deinit();
  tim6_irq_deinit();
  tim7_irq_deinit();
  tim14_irq_deinit();
  tim15_irq_deinit();
  tim16_irq_deinit();
  tim17_irq_deinit();

  usart1_irq_deinit();
  usart2_irq_deinit();
  usart3_usart4_lpuart1_irq_deinit();
}

/** @} */
