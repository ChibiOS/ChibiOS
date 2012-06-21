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
*/

/**
 * @file    STM3F2xx/stm32_isr.h
 * @brief   ISR remapper driver header.
 *
 * @addtogroup STM32F2xx_ISR
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
#define STM32_TIM1_UP_HANDLER       TIM1_UP_IRQHandler
#define STM32_TIM1_CC_HANDLER       TIM1_CC_IRQHandler
#define STM32_TIM2_HANDLER          TIM2_IRQHandler
#define STM32_TIM3_HANDLER          TIM3_IRQHandler
#define STM32_TIM4_HANDLER          TIM4_IRQHandler
#define STM32_TIM5_HANDLER          TIM5_IRQHandler
#define STM32_TIM8_UP_HANDLER       TIM8_UP_IRQHandler
#define STM32_TIM8_CC_HANDLER       TIM8_CC_IRQHandler

#define STM32_TIM1_UP_NUMBER        TIM1_UP_TIM10_IRQn
#define STM32_TIM1_CC_NUMBER        TIM1_CC_IRQn
#define STM32_TIM2_NUMBER           TIM2_IRQn
#define STM32_TIM3_NUMBER           TIM3_IRQn
#define STM32_TIM4_NUMBER           TIM4_IRQn
#define STM32_TIM5_NUMBER           TIM5_IRQn
#define STM32_TIM8_UP_NUMBER        TIM8_UP_TIM13_IRQn
#define STM32_TIM8_CC_NUMBER        TIM8_CC_IRQn
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
