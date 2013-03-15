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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for Embedded Artists LPCXpresso LPC812 board.
 */

/*
 * Board identifiers.
 */
#define BOARD_EA_LPC812
#define BOARD_NAME "Embedded Artists LPCXpresso LPC812"

/*
 * Board frequencies.
 */
#define SYSOSCCLK               12000000


/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the LPC8xx Reference Manual for details.
 */
/* Pull-up/down  */
#define PIN_MODE_NOPULL               (0<<3)
#define PIN_MODE_PULLDOWN             (1<<3)
#define PIN_MODE_PULLUP               (2<<3)
#define PIN_MODE_REPEATER             (3<<3)
/* Hysteresis */
#define PIN_HYS_EN                    (1<<5)
/* Invert Input */
#define PIN_INV_INPUT                 (1<<6)
/* Reserved bits */
#define PIN_RSVD                      (1<<7)
/* I2C Mode */
#define PIN_I2CMODE_STD               (0<<8)
#define PIN_I2CMODE_STDIO             (1<<8)
#define PIN_I2CMODE_FAST              (2<<8)
/* Open Drain */
#define PIN_OPEN_DRAIN                (1<<10)
/* Input Filter Sample Clocks */
#define PIN_SMODE_FILTER(n)           ((n)<<11)
/* Input Filter clock divider */
#define PIN_CLKDIV_FILTER(n)          ((n)<<13)

/*
 * Pin definitions.
 */
#define LED_RED                        7
#define LED_BLUE                      16
#define LED_GREEN                     17


/*
 * GPIO 0 initial setup.
 */
/*#define VAL_PIO0_0              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_1              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_2              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_3              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_4              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_5              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_6              PIN_MODE_PULLUP*/
#define VAL_PIO0_7              PIN_MODE_NOPULL
/*#define VAL_PIO0_8              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_9              PIN_MODE_PULLUP*/
/*#define VAL_PIO0_10             PIN_MODE_PULLUP*/
/*#define VAL_PIO0_11             PIN_MODE_PULLUP*/
/*#define VAL_PIO0_12             PIN_MODE_PULLUP*/
/*#define VAL_PIO0_13             PIN_MODE_PULLUP*/
/*#define VAL_PIO0_14             PIN_MODE_PULLUP*/
/*#define VAL_PIO0_15             PIN_MODE_PULLUP*/
#define VAL_PIO0_16             PIN_MODE_NOPULL
#define VAL_PIO0_17             PIN_MODE_NOPULL

    /* UART0: TXD = P0.4, RXD  = P0.0)*/
#define VAL_PINASSIGN0            ((0xFFFF0000) | (0<<8) | (4))
/*#define VAL_PINASSIGN1            0xFFFFFFFF*/
/*#define VAL_PINASSIGN2            0xFFFFFFFF*/
/*#define VAL_PINASSIGN3            0xFFFFFFFF*/
/*#define VAL_PINASSIGN4            0xFFFFFFFF*/
/*#define VAL_PINASSIGN5            0xFFFFFFFF*/
/*#define VAL_PINASSIGN6            0xFFFFFFFF*/
/*#define VAL_PINASSIGN7            0xFFFFFFFF*/
/*#define VAL_PINASSIGN8            0xFFFFFFFF*/


#define VAL_GPIO0DIR            (PAL_PORT_BIT(LED_RED)   |           \
                                 PAL_PORT_BIT(LED_BLUE)   |          \
                                 PAL_PORT_BIT(LED_GREEN))
                                
#define VAL_GPIO0DATA           (PAL_PORT_BIT(LED_RED)   |           \
                                 PAL_PORT_BIT(LED_BLUE)   |          \
                                 PAL_PORT_BIT(LED_GREEN))


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
