/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    STM8L/shared_isr.c
 * @brief   STM8L shared interrupt code source.
 * @details The STM8L shares some interrupt handlers among several sources.
 *          This module includes all the interrupt handlers that are
 *          used by more than one peripheral.
 * @note    Only the interrupt handlers that are used by the HAL are defined
 *          in this module.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/* This inclusion allows user ISR to be added to the HAL.*/
#if defined(_USER_ISR_)
#include "user_isr.h"
#endif

#if defined(_TIM2_UPDATE_ISR) || defined(_USART2_TRANSMIT_ISR) ||         \
    defined(__DOXYGEN__)
/**
 * @brief   IRQ 19 service routine.
 * @details This handler is shared between the following sources:
 *          - TIM2 update/overflow/trigger/break.
 *          - USART2 transmit.
 *          .
 *
 * @isr
 */
CH_IRQ_HANDLER(19) {
  CH_IRQ_PROLOGUE();

#if defined(_TIM2_UPDATE_ISR)
  _TIM2_UPDATE_ISR();
#endif
#if defined(_USART2_TRANSMIT_ISR)
  _USART2_TRANSMIT_ISR();
#endif

  CH_IRQ_EPILOGUE();
}
#endif /* defined(_TIM2_UPDATE_ISR) || defined(_USART2_TRANSMIT_ISR) */

#if defined(_TIM2_COMPARE_ISR) || defined(_USART2_RECEIVE_ISR) ||           \
    defined(__DOXYGEN__)
/**
 * @brief   IRQ 20 service routine.
 * @details This handler is shared between the following sources:
 *          - TIM2 compare/capture
 *          - USART2 receive.
 *          .
 *
 * @isr
 */
CH_IRQ_HANDLER(20) {
  CH_IRQ_PROLOGUE();

#if defined(_TIM2_COMPARE_ISR)
  _TIM2_COMPARE_ISR();
#endif
#if defined(_USART2_RECEIVE_ISR)
  _USART2_RECEIVE_ISR();
#endif

  CH_IRQ_EPILOGUE();
}
#endif /* defined(_TIM2_COMPARE_ISR) || defined(_USART2_RECEIVE_ISR) */

#if defined(_TIM3_UPDATE_ISR) || defined(_USART3_TRANSMIT_ISR) ||         \
    defined(__DOXYGEN__)
/**
 * @brief   IRQ 21 service routine.
 * @details This handler is shared between the following sources:
 *          - TIM3 update/overflow/trigger/break.
 *          - USART3 transmit.
 *          .
 *
 * @isr
 */
CH_IRQ_HANDLER(21) {
  CH_IRQ_PROLOGUE();

#if defined(_TIM3_UPDATE_ISR)
  _TIM3_UPDATE_ISR();
#endif
#if defined(_USART3_TRANSMIT_ISR)
  _USART3_TRANSMIT_ISR();
#endif

  CH_IRQ_EPILOGUE();
}
#endif /* defined(_TIM3_UPDATE_ISR) || defined(_USART3_TRANSMIT_ISR) */

#if defined(_TIM3_COMPARE_ISR) || defined(_USART3_RECEIVE_ISR) ||           \
    defined(__DOXYGEN__)
/**
 * @brief   IRQ 22 service routine.
 * @details This handler is shared between the following sources:
 *          - TIM3 compare/capture
 *          - USART3 receive.
 *          .
 *
 * @isr
 */
CH_IRQ_HANDLER(22) {
  CH_IRQ_PROLOGUE();

#if defined(_TIM3_COMPARE_ISR)
  _TIM3_COMPARE_ISR();
#endif
#if defined(_USART3_RECEIVE_ISR)
  _USART3_RECEIVE_ISR();
#endif

  CH_IRQ_EPILOGUE();
}
#endif /* defined(_TIM3_COMPARE_ISR) || defined(_USART3_RECEIVE_ISR) */

#if defined(_TIM5_UPDATE_ISR) || defined(_USART1_TRANSMIT_ISR) ||         \
    defined(__DOXYGEN__)
/**
 * @brief   IRQ 27 service routine.
 * @details This handler is shared between the following sources:
 *          - TIM5 update/overflow/trigger/break.
 *          - USART1 transmit.
 *          .
 *
 * @isr
 */
CH_IRQ_HANDLER(27) {
  CH_IRQ_PROLOGUE();

#if defined(_TIM5_UPDATE_ISR)
  _TIM5_UPDATE_ISR();
#endif
#if defined(_USART1_TRANSMIT_ISR)
  _USART1_TRANSMIT_ISR();
#endif

  CH_IRQ_EPILOGUE();
}
#endif /* defined(_TIM5_UPDATE_ISR) || defined(_USART1_TRANSMIT_ISR) */

#if defined(_TIM5_COMPARE_ISR) || defined(_USART1_RECEIVE_ISR) ||           \
    defined(__DOXYGEN__)
/**
 * @brief   IRQ 28 service routine.
 * @details This handler is shared between the following sources:
 *          - TIM5 compare/capture
 *          - USART1 receive.
 *          .
 *
 * @isr
 */
CH_IRQ_HANDLER(28) {
  CH_IRQ_PROLOGUE();

#if defined(_TIM5_COMPARE_ISR)
  _TIM5_COMPARE_ISR();
#endif
#if defined(_USART1_RECEIVE_ISR)
  _USART1_RECEIVE_ISR();
#endif

  CH_IRQ_EPILOGUE();
}
#endif /* defined(_TIM5_COMPARE_ISR) || defined(_USART1_RECEIVE_ISR) */

/** @} */
