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
 * @defgroup STM8L_MDP_HAL STM8L Medium Density Plus sub-family
 *
 * @ingroup HAL
 */

/**
 * @file    STM8L/hal_lld_stm8l_mdp.h
 * @brief   STM8L Medium Density Plus sub-family capabilities descriptor.
 *
 * @addtogroup STM8L_MDP_HAL
 * @{
 */

#ifndef _HAL_LLD_STM8L_MDP_H_
#define _HAL_LLD_STM8L_MDP_H_

/*===========================================================================*/
/* Sub-family capabilities.                                                  */
/*===========================================================================*/

#define STM8L_HAS_ADC1              TRUE

#define STM8L_HAS_BEEP              TRUE

#define STM8L_HAS_COMP1             TRUE
#define STM8L_HAS_COMP2             TRUE

#define STM8L_HAS_DAC1              TRUE

#define STM8L_HAS_DMA1              TRUE

#define STM8L_HAS_GPIOA             TRUE
#define STM8L_HAS_GPIOB             TRUE
#define STM8L_HAS_GPIOC             TRUE
#define STM8L_HAS_GPIOD             TRUE
#define STM8L_HAS_GPIOE             TRUE
#define STM8L_HAS_GPIOF             TRUE
#define STM8L_HAS_GPIOG             TRUE
#define STM8L_HAS_GPIOH             TRUE
#define STM8L_HAS_GPIOI             TRUE

#define STM8L_HAS_I2C1              TRUE

#define STM8L_HAS_LCD               TRUE

#define STM8L_HAS_SPI1              TRUE
#define STM8L_HAS_SPI2              TRUE

#define STM8L_HAS_TIM1              TRUE
#define STM8L_HAS_TIM2              TRUE
#define STM8L_HAS_TIM3              TRUE
#define STM8L_HAS_TIM4              TRUE
#define STM8L_HAS_TIM5              TRUE

#define STM8L_HAS_USART1            TRUE
#define STM8L_HAS_USART2            TRUE
#define STM8L_HAS_USART3            TRUE

#endif /* _HAL_LLD_STM8L_MDP_H_ */

/** @} */
