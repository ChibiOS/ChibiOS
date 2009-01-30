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

#ifndef AT91SAM7X256_H
#include "at91lib/AT91SAM7X256.h"
#endif

#define BOARD_OLIMEX_SAM7_EX256

#define CLK             18432000
#define MCK             48054857

/*
 * I/O definitions.
 */
#define PIOA_LCD_RESET  (1 << 2)
#define PIOA_B1         (1 << 7)
#define PIOA_B2         (1 << 8)
#define PIOA_B3         (1 << 9)
#define PIOA_B4         (1 << 14)
#define PIOA_B5         (1 << 15)
#define PIOA_USB_PUP    (1 << 25)
#define PIOA_USB_PR     (1 << 26)
#define PIOA_PA27       (1 << 27)
#define PIOA_PA28       (1 << 28)
#define PIOA_PA29       (1 << 29)
#define PIOA_PA30       (1 << 30)

#define PIOB_PHY_PD     (1 << 18)
#define PIOB_AUDIO_OUT  (1 << 19)
#define PIOB_LCD_BL     (1 << 20)
#define PIOB_PB21       (1 << 21)
#define PIOB_MMC_WP     (1 << 22)
#define PIOB_MMC_CP     (1 << 23)
#define PIOB_SW1        (1 << 24)
#define PIOB_SW2        (1 << 25)
#define PIOB_PHY_IRQ    (1 << 26)
#define PIOB_PB27_AD0   (1 << 27)
#define PIOB_PB28_AD1   (1 << 28)
#define PIOB_PB29_AD2   (1 << 29)
#define PIOB_PB30_AD3   (1 << 30)

#endif /* _BOARD_H_ */
