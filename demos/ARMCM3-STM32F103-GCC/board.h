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

#undef FALSE
#undef TRUE
#ifndef __STM32F10x_MAP_H
#include "stm32lib/stm32f10x_map.h"
#endif

#define BOARD_OLIMEX_STM32_P103

#define LSECLK          32768
#define HSECLK          8000000
#define PLLDIV          1
#define PLLMUL          9
#define PLLCLK          ((HSECLK / PLLDIV) * PLLMUL)

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
