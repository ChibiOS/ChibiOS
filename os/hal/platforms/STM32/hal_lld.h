/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM32/hal_lld.h
 * @brief   STM32 HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_HSECLK.
 *          .
 *          One of the following macros must also be defined:
 *          - STM32F10X_LD_VL for Value Line Low Density devices.
 *          - STM32F10X_MD_VL for Value Line Medium Density devices.
 *          - STM32F10X_LD for Performance Low Density devices.
 *          - STM32F10X_MD for Performance Medium Density devices.
 *          - STM32F10X_HD for Performance High Density devices.
 *          - STM32F10X_XL for Performance eXtra Density devices.
 *          - STM32F10X_CL for Connectivity Line devices.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @brief   Platform name.
 */
#if defined(__DOXYGEN__)
#define PLATFORM_NAME           "STM32"

#elif defined(STM32F10X_LD_VL)
/*
 * Capability flags for Value Line Low Density devices.
 */
#define PLATFORM_NAME           "STM32 Value Line Low Density"
#include "hal_lld_f100.h"

#define STM32_HAS_ADC1          TRUE
#define STM32_HAS_ADC2          FALSE
#define STM32_HAS_ADC3          FALSE

#define STM32_HAS_CAN1          FALSE
#define STM32_HAS_CAN2          FALSE

#define STM32_HAS_DAC           TRUE

#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          FALSE

#define STM32_HAS_ETH           FALSE

#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         TRUE
#define STM32_HAS_GPIOE         TRUE
#define STM32_HAS_GPIOF         FALSE
#define STM32_HAS_GPIOG         FALSE

#define STM32_HAS_I2C1          TRUE
#define STM32_HAS_I2C2          FALSE

#define STM32_HAS_RTC           TRUE

#define STM32_HAS_SDIO          FALSE

#define STM32_HAS_SPI1          TRUE
#define STM32_HAS_SPI2          FALSE
#define STM32_HAS_SPI3          FALSE

#define STM32_HAS_TIM1          TRUE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          FALSE
#define STM32_HAS_TIM5          FALSE
#define STM32_HAS_TIM6          TRUE
#define STM32_HAS_TIM7          TRUE
#define STM32_HAS_TIM8          FALSE
#define STM32_HAS_TIM9          FALSE
#define STM32_HAS_TIM10         FALSE
#define STM32_HAS_TIM11         FALSE
#define STM32_HAS_TIM12         FALSE
#define STM32_HAS_TIM13         FALSE
#define STM32_HAS_TIM14         FALSE
#define STM32_HAS_TIM15         TRUE
#define STM32_HAS_TIM16         TRUE
#define STM32_HAS_TIM17         TRUE

#define STM32_HAS_USART1        TRUE
#define STM32_HAS_USART2        TRUE
#define STM32_HAS_USART3        FALSE
#define STM32_HAS_UART3         FALSE
#define STM32_HAS_UART4         FALSE

#define STM32_HAS_USB           FALSE
#define STM32_HAS_OTG1          FALSE

#elif defined(STM32F10X_MD_VL)
/*
 * Capability flags for Value Line Medium Density devices.
 */
#define PLATFORM_NAME           "STM32 Value Line Medium Density"
#include "hal_lld_f100.h"

#define STM32_HAS_ADC1          TRUE
#define STM32_HAS_ADC2          FALSE
#define STM32_HAS_ADC3          FALSE

#define STM32_HAS_CAN1          FALSE
#define STM32_HAS_CAN2          FALSE

#define STM32_HAS_DAC           TRUE

#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          FALSE

#define STM32_HAS_ETH           FALSE

#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         TRUE
#define STM32_HAS_GPIOE         TRUE
#define STM32_HAS_GPIOF         FALSE
#define STM32_HAS_GPIOG         FALSE

#define STM32_HAS_I2C1          TRUE
#define STM32_HAS_I2C2          TRUE

#define STM32_HAS_RTC           TRUE

#define STM32_HAS_SDIO          FALSE

#define STM32_HAS_SPI1          TRUE
#define STM32_HAS_SPI2          TRUE
#define STM32_HAS_SPI3          FALSE

#define STM32_HAS_TIM1          TRUE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          TRUE
#define STM32_HAS_TIM5          FALSE
#define STM32_HAS_TIM6          TRUE
#define STM32_HAS_TIM7          TRUE
#define STM32_HAS_TIM8          FALSE
#define STM32_HAS_TIM9          FALSE
#define STM32_HAS_TIM10         FALSE
#define STM32_HAS_TIM11         FALSE
#define STM32_HAS_TIM12         FALSE
#define STM32_HAS_TIM13         FALSE
#define STM32_HAS_TIM14         FALSE
#define STM32_HAS_TIM15         TRUE
#define STM32_HAS_TIM16         TRUE
#define STM32_HAS_TIM17         TRUE

#define STM32_HAS_USART1        TRUE
#define STM32_HAS_USART2        TRUE
#define STM32_HAS_USART3        TRUE
#define STM32_HAS_UART3         FALSE
#define STM32_HAS_UART4         FALSE

#define STM32_HAS_USB           FALSE
#define STM32_HAS_OTG1          FALSE

#elif defined(STM32F10X_LD)
/*
 * Capability flags for Performance Line Low Density devices.
 */
#define PLATFORM_NAME           "STM32 Performance Line Low Density"
#include "hal_lld_f103.h"

#define STM32_HAS_ADC1          TRUE
#define STM32_HAS_ADC2          TRUE
#define STM32_HAS_ADC3          FALSE

#define STM32_HAS_CAN1          TRUE
#define STM32_HAS_CAN2          FALSE

#define STM32_HAS_DAC           FALSE

#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          FALSE

#define STM32_HAS_ETH           FALSE

#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         TRUE
#define STM32_HAS_GPIOE         FALSE
#define STM32_HAS_GPIOF         FALSE
#define STM32_HAS_GPIOG         FALSE

#define STM32_HAS_I2C1          TRUE
#define STM32_HAS_I2C2          FALSE

#define STM32_HAS_RTC           TRUE

#define STM32_HAS_SDIO          FALSE

#define STM32_HAS_SPI1          TRUE
#define STM32_HAS_SPI2          FALSE
#define STM32_HAS_SPI3          FALSE

#define STM32_HAS_TIM1          TRUE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          FALSE
#define STM32_HAS_TIM5          FALSE
#define STM32_HAS_TIM6          FALSE
#define STM32_HAS_TIM7          FALSE
#define STM32_HAS_TIM8          FALSE
#define STM32_HAS_TIM9          FALSE
#define STM32_HAS_TIM10         FALSE
#define STM32_HAS_TIM11         FALSE
#define STM32_HAS_TIM12         FALSE
#define STM32_HAS_TIM13         FALSE
#define STM32_HAS_TIM14         FALSE
#define STM32_HAS_TIM15         FALSE
#define STM32_HAS_TIM16         FALSE
#define STM32_HAS_TIM17         FALSE

#define STM32_HAS_USART1        TRUE
#define STM32_HAS_USART2        TRUE
#define STM32_HAS_USART3        FALSE
#define STM32_HAS_UART3         FALSE
#define STM32_HAS_UART4         FALSE

#define STM32_HAS_USB           FALSE
#define STM32_HAS_OTG1          FALSE

#elif defined(STM32F10X_MD)
/*
 * Capability flags for Performance Line Medium Density devices.
 */
#define PLATFORM_NAME           "STM32 Performance Line Medium Density"
#include "hal_lld_f103.h"

#define STM32_HAS_ADC1          TRUE
#define STM32_HAS_ADC2          TRUE
#define STM32_HAS_ADC3          FALSE

#define STM32_HAS_CAN1          TRUE
#define STM32_HAS_CAN2          FALSE

#define STM32_HAS_DAC           FALSE

#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          FALSE

#define STM32_HAS_ETH           FALSE

#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         TRUE
#define STM32_HAS_GPIOE         TRUE
#define STM32_HAS_GPIOF         FALSE
#define STM32_HAS_GPIOG         FALSE

#define STM32_HAS_I2C1          TRUE
#define STM32_HAS_I2C2          TRUE

#define STM32_HAS_RTC           TRUE

#define STM32_HAS_SDIO          FALSE

#define STM32_HAS_SPI1          TRUE
#define STM32_HAS_SPI2          TRUE
#define STM32_HAS_SPI3          FALSE

#define STM32_HAS_TIM1          TRUE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          TRUE
#define STM32_HAS_TIM5          FALSE
#define STM32_HAS_TIM6          FALSE
#define STM32_HAS_TIM7          FALSE
#define STM32_HAS_TIM8          FALSE
#define STM32_HAS_TIM9          FALSE
#define STM32_HAS_TIM10         FALSE
#define STM32_HAS_TIM11         FALSE
#define STM32_HAS_TIM12         FALSE
#define STM32_HAS_TIM13         FALSE
#define STM32_HAS_TIM14         FALSE
#define STM32_HAS_TIM15         FALSE
#define STM32_HAS_TIM16         FALSE
#define STM32_HAS_TIM17         FALSE

#define STM32_HAS_USART1        TRUE
#define STM32_HAS_USART2        TRUE
#define STM32_HAS_USART3        TRUE
#define STM32_HAS_UART3         FALSE
#define STM32_HAS_UART4         FALSE

#define STM32_HAS_USB           TRUE
#define STM32_HAS_OTG1          FALSE

#elif defined(STM32F10X_HD)
/*
 * Capability flags for Performance Line High Density devices.
 */
#define PLATFORM_NAME           "STM32 Performance Line High Density"
#include "hal_lld_f103.h"

#define STM32_HAS_ADC1          TRUE
#define STM32_HAS_ADC2          TRUE
#define STM32_HAS_ADC3          TRUE

#define STM32_HAS_CAN1          TRUE
#define STM32_HAS_CAN2          FALSE

#define STM32_HAS_DAC           TRUE

#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          TRUE

#define STM32_HAS_ETH           FALSE

#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         TRUE
#define STM32_HAS_GPIOE         TRUE
#define STM32_HAS_GPIOF         TRUE
#define STM32_HAS_GPIOG         TRUE

#define STM32_HAS_I2C1          TRUE
#define STM32_HAS_I2C2          TRUE

#define STM32_HAS_RTC           TRUE

#define STM32_HAS_SDIO          TRUE

#define STM32_HAS_SPI1          TRUE
#define STM32_HAS_SPI2          TRUE
#define STM32_HAS_SPI3          TRUE

#define STM32_HAS_TIM1          TRUE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          TRUE
#define STM32_HAS_TIM5          TRUE
#define STM32_HAS_TIM6          TRUE
#define STM32_HAS_TIM7          TRUE
#define STM32_HAS_TIM8          TRUE
#define STM32_HAS_TIM9          FALSE
#define STM32_HAS_TIM10         FALSE
#define STM32_HAS_TIM11         FALSE
#define STM32_HAS_TIM12         FALSE
#define STM32_HAS_TIM13         FALSE
#define STM32_HAS_TIM14         FALSE
#define STM32_HAS_TIM15         FALSE
#define STM32_HAS_TIM16         FALSE
#define STM32_HAS_TIM17         FALSE

#define STM32_HAS_USART1        TRUE
#define STM32_HAS_USART2        TRUE
#define STM32_HAS_USART3        TRUE
#define STM32_HAS_UART3         TRUE
#define STM32_HAS_UART4         TRUE

#define STM32_HAS_USB           TRUE
#define STM32_HAS_OTG1          FALSE

#elif defined(STM32F10X_XL)
/*
 * Capability flags for Performance Line eXtra Density devices.
 */
#define PLATFORM_NAME           "STM32 Performance Line eXtra Density"
#include "hal_lld_f103.h"

#define STM32_HAS_ADC1          TRUE
#define STM32_HAS_ADC2          TRUE
#define STM32_HAS_ADC3          TRUE

#define STM32_HAS_CAN1          TRUE
#define STM32_HAS_CAN2          FALSE

#define STM32_HAS_DAC           TRUE

#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          TRUE

#define STM32_HAS_ETH           FALSE

#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         TRUE
#define STM32_HAS_GPIOE         TRUE
#define STM32_HAS_GPIOF         TRUE
#define STM32_HAS_GPIOG         TRUE

#define STM32_HAS_I2C1          TRUE
#define STM32_HAS_I2C2          TRUE

#define STM32_HAS_RTC           TRUE

#define STM32_HAS_SDIO          TRUE

#define STM32_HAS_SPI1          TRUE
#define STM32_HAS_SPI2          TRUE
#define STM32_HAS_SPI3          TRUE

#define STM32_HAS_TIM1          TRUE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          TRUE
#define STM32_HAS_TIM5          TRUE
#define STM32_HAS_TIM6          TRUE
#define STM32_HAS_TIM7          TRUE
#define STM32_HAS_TIM8          TRUE
#define STM32_HAS_TIM9          FALSE
#define STM32_HAS_TIM10         FALSE
#define STM32_HAS_TIM11         FALSE
#define STM32_HAS_TIM12         FALSE
#define STM32_HAS_TIM13         FALSE
#define STM32_HAS_TIM14         FALSE
#define STM32_HAS_TIM15         FALSE
#define STM32_HAS_TIM16         FALSE
#define STM32_HAS_TIM17         FALSE

#define STM32_HAS_USART1        TRUE
#define STM32_HAS_USART2        TRUE
#define STM32_HAS_USART3        TRUE
#define STM32_HAS_UART3         TRUE
#define STM32_HAS_UART4         TRUE

#define STM32_HAS_USB           TRUE
#define STM32_HAS_OTG1          FALSE

#elif defined(STM32F10X_CL)
/*
 * Capability flags for Connectivity Line devices.
 */
#define PLATFORM_NAME           "STM32 Connectivity Line"
#include "hal_lld_f105_f107.h"

#define STM32_HAS_ADC1          TRUE
#define STM32_HAS_ADC2          TRUE
#define STM32_HAS_ADC3          FALSE

#define STM32_HAS_CAN1          TRUE
#define STM32_HAS_CAN2          TRUE

#define STM32_HAS_DAC           TRUE

#define STM32_HAS_DMA1          TRUE
#define STM32_HAS_DMA2          TRUE

#define STM32_HAS_ETH           TRUE

#define STM32_HAS_GPIOA         TRUE
#define STM32_HAS_GPIOB         TRUE
#define STM32_HAS_GPIOC         TRUE
#define STM32_HAS_GPIOD         TRUE
#define STM32_HAS_GPIOE         TRUE
#define STM32_HAS_GPIOF         FALSE
#define STM32_HAS_GPIOG         FALSE

#define STM32_HAS_I2C1          TRUE
#define STM32_HAS_I2C2          TRUE

#define STM32_HAS_RTC           TRUE

#define STM32_HAS_SDIO          FALSE

#define STM32_HAS_SPI1          TRUE
#define STM32_HAS_SPI2          TRUE
#define STM32_HAS_SPI3          TRUE

#define STM32_HAS_TIM1          TRUE
#define STM32_HAS_TIM2          TRUE
#define STM32_HAS_TIM3          TRUE
#define STM32_HAS_TIM4          TRUE
#define STM32_HAS_TIM5          TRUE
#define STM32_HAS_TIM6          TRUE
#define STM32_HAS_TIM7          TRUE
#define STM32_HAS_TIM8          FALSE
#define STM32_HAS_TIM9          FALSE
#define STM32_HAS_TIM10         FALSE
#define STM32_HAS_TIM11         FALSE
#define STM32_HAS_TIM12         FALSE
#define STM32_HAS_TIM13         FALSE
#define STM32_HAS_TIM14         FALSE
#define STM32_HAS_TIM15         FALSE
#define STM32_HAS_TIM16         FALSE
#define STM32_HAS_TIM17         FALSE

#define STM32_HAS_USART1        TRUE
#define STM32_HAS_USART2        TRUE
#define STM32_HAS_USART3        TRUE
#define STM32_HAS_UART3         TRUE
#define STM32_HAS_UART4         TRUE

#define STM32_HAS_USB           FALSE
#define STM32_HAS_OTG1          TRUE

#else
#error "unspecified, unsupported or invalid STM32 platform"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Tricks required to make the TRUE/FALSE declaration inside the library
   compatible.*/
#undef FALSE
#undef TRUE
#include "stm32f10x.h"
#define FALSE 0
#define TRUE (!FALSE)

/* STM32 DMA support code.*/
#include "stm32_dma.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void stm32_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
