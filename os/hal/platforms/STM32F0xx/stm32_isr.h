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
 * @file    STM32F0xx/stm32_isr.h
 * @brief   ISR remapper driver header.
 *
 * @addtogroup STM32F0xx_ISR
 * @{
 */

#ifndef _STM32_ISR_H_
#define _STM32_ISR_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    ISR names and numbers remapping
 * @{
 */
/*
 * TIM units.
 */
#define STM32_TIM1_UP_HANDLER       TIM1_BRK_UP_TRG_COM_IRQHandler
#define STM32_TIM1_CC_HANDLER       TIM1_CC_IRQHandler
#define STM32_TIM2_HANDLER          TIM2_IRQHandler
#define STM32_TIM3_HANDLER          TIM3_IRQHandler

#define STM32_TIM1_UP_NUMBER        TIM1_BRK_UP_TRG_COM_IRQn
#define STM32_TIM1_CC_NUMBER        TIM1_CC_IRQn
#define STM32_TIM2_NUMBER           TIM2_IRQn
#define STM32_TIM3_NUMBER           TIM3_IRQn

/*
 * USART units.
 */
#define STM32_USART1_HANDLER        USART1_IRQHandler
#define STM32_USART2_HANDLER        USART2_IRQHandler

#define STM32_USART1_NUMBER         USART1_IRQn
#define STM32_USART2_NUMBER         USART2_IRQn
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#endif /* _STM32_ISR_H_ */

/** @} */
