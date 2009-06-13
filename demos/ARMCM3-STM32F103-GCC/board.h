/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Tricks required to make the TRUE/FALSE declaration inside the library
 * compatible.
 */
#undef FALSE
#undef TRUE
#ifndef __STM32F10x_MAP_H
#include "stm32f10x_map.h"
#endif
#define FALSE 0
#define TRUE (!FALSE)

#define BOARD_OLIMEX_STM32_P103

/*
 * Uncomment this if you want a 48MHz system clock, else it will be 72MHz.
 */
//#define SYSCLK_48

/*
 * NOTES: PLLPRE can be 1 or 2, PLLMUL can be 2..16.
 */
#define LSECLK          32768
#define HSECLK          8000000
#define HSICLK          8000000
#define PLLPRE          1
#ifdef SYSCLK_48
  #define PLLMUL        6
#else
  #define PLLMUL        9
#endif
#define PLLCLK          ((HSECLK / PLLPRE) * PLLMUL)
#define SYSCLK          PLLCLK
#define APB1CLK         (SYSCLK / 2)
#define APB2CLK         (SYSCLK / 2)
#define AHB1CLK         (SYSCLK / 1)

/*
 * Values derived from the clock settings.
 */
#define PLLPREBITS      ((PLLPRE - 1) << 17)
#define PLLMULBITS      ((PLLMUL - 2) << 18)
#ifdef SYSCLK_48
  #define USBPREBITS    USBPRE_DIV1_BITS
  #define FLASHBITS     0x00000011
#else
  #define USBPREBITS    USBPRE_DIV1P5_BITS
  #define FLASHBITS     0x00000012
#endif

/*
 * Definitions for RCC_CR register.
 */
#define CR_HSION_MASK           (0x1 << 0)
#define CR_HSIRDY_MASK          (0x1 << 1)
#define CR_HSITRIM_MASK         (0x1F << 3)
#define   HSITRIM_RESET_BITS    (0x10 << 3)
#define CR_HSICAL_MASK          (0xFF << 8)
#define CR_HSEON_MASK           (0x1 << 16)
#define CR_HSERDY_MASK          (0x1 << 17)
#define CR_HSEBYP_MASK          (0x1 << 18)
#define CR_CSSON_MASK           (0x1 << 19)
#define CR_PLLON_MASK           (0x1 << 24)
#define CR_PLLRDY_MASK          (0x1 << 25)

/*
 * Definitions for RCC_CFGR register.
 */
#define CFGR_SW_MASK            (0x3 << 0)
#define   SW_HSI_BITS           (0 << 0)
#define   SW_HSE_BITS           (1 << 0)
#define   SW_PLL_BITS           (2 << 0)
#define CFGR_SWS_MASK           (0x3 << 2)
#define   SWS_HSI_BITS          (0 << 2)
#define   SWS_HSE_BITS          (1 << 2)
#define   SWS_PLL_BITS          (2 << 2)
#define CFGR_HPRE_MASK          (0xF << 4)
#define    HPRE_DIV1_BITS       (0 << 4)
#define CFGR_PPRE1_MASK         (0x7 << 8)
#define   PPRE1_DIV1_BITS       (0 << 8)
#define   PPRE1_DIV2_BITS       (4 << 8)
#define CFGR_PPRE2_MASK         (0x7 << 11)
#define   PPRE2_DIV1_BITS       (0 << 11)
#define   PPRE2_DIV2_BITS       (4 << 11)
#define CFGR_ADCPRE_MASK        (0x3 << 14)
#define   ADCPRE_DIV2_BITS      (0 << 14)
#define   ADCPRE_DIV4_BITS      (1 << 14)
#define   ADCPRE_DIV6_BITS      (2 << 14)
#define   ADCPRE_DIV8_BITS      (3 << 14)
#define CFGR_PLLSRC_MASK        (0x1 << 16)
#define   PLLSRC_HSI_BITS       (0 << 16)
#define   PLLSRC_HSE_BITS       (1 << 16)
#define CFGR_PLLXTPRE_MASK      (0x1 << 17)
#define CFGR_PLLMUL_MASK        (0xF << 18)
#define CFGR_USBPRE_MASK        (0x1 << 22)
#define   USBPRE_DIV1P5_BITS    (0 << 22)
#define   USBPRE_DIV1_BITS      (1 << 22)
#define CFGR_MCO_MASK           (0x7 << 24)
#define   MCO_DISABLED_BITS     (0 << 24)

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON    (1 << 0)

#define GPIOC_MMCWP     (1 << 6)
#define GPIOC_MMCCP     (1 << 7)
#define GPIOC_CANCNTL   (1 << 10)
#define GPIOC_DISC      (1 << 11)
#define GPIOC_LED       (1 << 12)

/*
 * All inputs with pullups unless otherwise specified.
 */
#define VAL_GPIOACRL    0x88888884      // PA0:FI
#define VAL_GPIOACRH    0x88888888
#define VAL_GPIOAODR    0xFFFFFFFF

#define VAL_GPIOBCRL    0x88883888      // PB3:PP
#define VAL_GPIOBCRH    0x88888888
#define VAL_GPIOBODR    0xFFFFFFFF

#define VAL_GPIOCCRL    0x44888888      // PC6,PC7:FI
#define VAL_GPIOCCRH    0x88833888      // PC11,PC12:PP
#define VAL_GPIOCODR    0xFFFFFFFF

#define VAL_GPIODCRL    0x88888844      // PD0,PD1:FI
#define VAL_GPIODCRH    0x88888888
#define VAL_GPIODODR    0xFFFFFFFF

#endif /* _BOARD_H_ */
