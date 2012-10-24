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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for STMicroelectronics STM32F4-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F3_DISCOVERY
#define BOARD_NAME                  "STMicroelectronics STM32F3-Discovery"


/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000
#endif

#define STM32_HSE_BYPASS

/*
 * MCU type as defined in the ST header file stm32f30x.h.
 */
#define STM32F30X

/*
 * IO pins assignments.
 */

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_10M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * GPIOA setup:
 *
 */
#define VAL_GPIOA_MODER             0
#define VAL_GPIOA_OTYPER            0
#define VAL_GPIOA_OSPEEDR           0
#define VAL_GPIOA_PUPDR             0
#define VAL_GPIOA_ODR               0
#define VAL_GPIOA_AFRL              0
#define VAL_GPIOA_AFRH              0

/*
 * GPIOB setup:
 *
 */
#define VAL_GPIOB_MODER             0
#define VAL_GPIOB_OTYPER            0
#define VAL_GPIOB_OSPEEDR           0
#define VAL_GPIOB_PUPDR             0
#define VAL_GPIOB_ODR               0
#define VAL_GPIOB_AFRL              0
#define VAL_GPIOB_AFRH              0

/*
 * GPIOC setup:
 *
 */
#define VAL_GPIOC_MODER             0
#define VAL_GPIOC_OTYPER            0
#define VAL_GPIOC_OSPEEDR           0
#define VAL_GPIOC_PUPDR             0
#define VAL_GPIOC_ODR               0
#define VAL_GPIOC_AFRL              0
#define VAL_GPIOC_AFRH              0

/*
 * GPIOD setup:
 *
0 */
#define VAL_GPIOD_MODER             0
#define VAL_GPIOD_OTYPER            0
#define VAL_GPIOD_OSPEEDR           0
#define VAL_GPIOD_PUPDR             0
#define VAL_GPIOD_ODR               0
#define VAL_GPIOD_AFRL              0
#define VAL_GPIOD_AFRH              0

/*
 * GPIOE setup:
 *
 */
#define VAL_GPIOE_MODER             0
#define VAL_GPIOE_OTYPER            0
#define VAL_GPIOE_OSPEEDR           0
#define VAL_GPIOE_PUPDR             0
#define VAL_GPIOE_ODR               0
#define VAL_GPIOE_AFRL              0
#define VAL_GPIOE_AFRH              0

/*
 * GPIOF setup:
 *
 */
#define VAL_GPIOF_MODER             0
#define VAL_GPIOF_OTYPER            0
#define VAL_GPIOF_OSPEEDR           0
#define VAL_GPIOF_PUPDR             0
#define VAL_GPIOF_ODR               0
#define VAL_GPIOF_AFRL              0
#define VAL_GPIOF_AFRH              0

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
