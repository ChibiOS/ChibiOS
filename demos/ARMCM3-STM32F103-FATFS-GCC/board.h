/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * Tricks required to make the TRUE/FALSE declaration inside the library
 * compatible.
 */
#undef FALSE
#undef TRUE
#include <stm32f10x.h>
#define FALSE 0
#define TRUE (!FALSE)

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
  #define USBPREBITS    RCC_CFGR_USBPRE_DIV1_BITS
  #define FLASHBITS     0x00000011
#else
  #define USBPREBITS    RCC_CFGR_USBPRE_DIV1P5_BITS
  #define FLASHBITS     0x00000012
#endif

/*
 * Extra definitions for RCC_CR register (missing from the ST header file).
 */
#define RCC_CR_HSITRIM_RESET_BITS       (0x10 << 3)

/*
 * Extra definitions for RCC_CFGR register (missing from the ST header file).
 */
#define RCC_CFGR_PLLSRC_HSI_BITS        (0 << 16)
#define RCC_CFGR_PLLSRC_HSE_BITS        (1 << 16)
#define RCC_CFGR_USBPRE_DIV1P5_BITS     (0 << 22)
#define RCC_CFGR_USBPRE_DIV1_BITS       (1 << 22)

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON            0
#define GPIOA_SPI1NSS           4

#define GPIOB_SPI2NSS           12

#define GPIOC_MMCWP             6
#define GPIOC_MMCCP             7
#define GPIOC_CANCNTL           10
#define GPIOC_DISC              11
#define GPIOC_LED               12

/*
 * All inputs with pullups unless otherwise specified.
 */
#define VAL_GPIOACRL            0x88888884      // PA0:FI
#define VAL_GPIOACRH            0x88888888
#define VAL_GPIOAODR            0xFFFFFFFF

#define VAL_GPIOBCRL            0x88883888      // PB3:PP
#define VAL_GPIOBCRH            0x88888888
#define VAL_GPIOBODR            0xFFFFFFFF

#define VAL_GPIOCCRL            0x44888888      // PC6,PC7:FI
#define VAL_GPIOCCRH            0x88833888      // PC11,PC12:PP
#define VAL_GPIOCODR            0xFFFFFFFF

#define VAL_GPIODCRL            0x88888844      // PD0,PD1:FI
#define VAL_GPIODCRH            0x88888888
#define VAL_GPIODODR            0xFFFFFFFF

#define VAL_GPIOECRL            0x88888888
#define VAL_GPIOECRH            0x88888888
#define VAL_GPIOEODR            0xFFFFFFFF

#endif /* _BOARD_H_ */
