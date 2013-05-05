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
 * @defgroup STM8L_MD_HAL STM8L Medium Density sub-family
 *
 * @ingroup HAL
 */

/**
 * @file    STM8L/hal_lld_stm8l_md.h
 * @brief   STM8L Medium Density sub-family capabilities descriptor.
 *
 * @addtogroup STM8L_MD_HAL
 * @{
 */

#ifndef _HAL_LLD_STM8L_MD_H_
#define _HAL_LLD_STM8L_MD_H_

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
#define STM8L_HAS_GPIOG             FALSE
#define STM8L_HAS_GPIOH             FALSE
#define STM8L_HAS_GPIOI             FALSE

#define STM8L_HAS_I2C1              TRUE

#define STM8L_HAS_LCD               TRUE

#define STM8L_HAS_SPI1              TRUE
#define STM8L_HAS_SPI2              FALSE

#define STM8L_HAS_TIM1              TRUE
#define STM8L_HAS_TIM2              TRUE
#define STM8L_HAS_TIM3              TRUE
#define STM8L_HAS_TIM4              TRUE
#define STM8L_HAS_TIM5              FALSE

#define STM8L_HAS_USART1            TRUE
#define STM8L_HAS_USART2            FALSE
#define STM8L_HAS_USART3            FALSE

#endif /* _HAL_LLD_STM8L_MD_H_ */

/** @} */
