/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the Olimex MSP430-P1611 proto board.
 */

/*
 * Board identifier.
 */
#define BOARD_OLIMEX_MSP430_P1611

/*
 * Clock constants.
 */
#define LFXT1CLK        32768
#define XT2CLK          8000000
#define DCOCLK          750000

/*
 * Pin definitions for the Olimex MSP430-P1611 board.
 */
#define P3_O_TXD0               4
#define P3_O_TXD0_MASK          (1 << P3_O_TXD0)
#define P3_I_RXD0               5
#define P3_I_RXD0_MASK          (1 << P3_I_RXD0)
#define P6_O_LED                0
#define P6_O_LED_MASK           (1 << P6_O_LED)
#define P6_I_BUTTON             1
#define P6_I_BUTTON_MASK        (1 << P6_I_BUTTON)

/*
 * Initial I/O ports settings.
 */
#define VAL_P1OUT       0x00
#define VAL_P1DIR       0xFF

#define VAL_P2OUT       0x00
#define VAL_P2DIR       0xFF

#define VAL_P3OUT       P3_O_TXD0_MASK
#define VAL_P3DIR       ~P3_I_RXD0_MASK

#define VAL_P4OUT       0x00
#define VAL_P4DIR       0xFF

#define VAL_P5OUT       0x00
#define VAL_P5DIR       0xFF

#define VAL_P6OUT       P6_O_LED_MASK
#define VAL_P6DIR       ~P6_I_BUTTON_MASK

#ifdef __cplusplus
extern "C" {
#endif
  void hwinit(void);
#ifdef __cplusplus
}
#endif

#endif /* _BOARD_H_ */
