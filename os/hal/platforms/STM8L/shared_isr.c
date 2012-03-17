/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
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
