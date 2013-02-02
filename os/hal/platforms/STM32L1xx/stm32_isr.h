/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
*/

/**
 * @file    STM32L1xx/stm32_isr.h
 * @brief   ISR remapper driver header.
 *
 * @addtogroup STM32L1xx_ISR
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
#define STM32_TIM2_HANDLER          TIM2_IRQHandler
#define STM32_TIM3_HANDLER          TIM3_IRQHandler
#define STM32_TIM4_HANDLER          TIM4_IRQHandler

#define STM32_TIM2_NUMBER           TIM2_IRQn
#define STM32_TIM3_NUMBER           TIM3_IRQn
#define STM32_TIM4_NUMBER           TIM4_IRQn

/*
 * USART units.
 */
#define STM32_USART1_HANDLER        USART1_IRQHandler
#define STM32_USART2_HANDLER        USART2_IRQHandler
#define STM32_USART3_HANDLER        USART3_IRQHandler

#define STM32_USART1_NUMBER         USART1_IRQn
#define STM32_USART2_NUMBER         USART2_IRQn
#define STM32_USART3_NUMBER         USART3_IRQn

/*
 * USB units.
 */
#define STM32_USB1_HP_HANDLER       Vector8C
#define STM32_USB1_LP_HANDLER       Vector90

#define STM32_USB1_HP_NUMBER        USB_HP_IRQn
#define STM32_USB1_LP_NUMBER        USB_LP_IRQn
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
