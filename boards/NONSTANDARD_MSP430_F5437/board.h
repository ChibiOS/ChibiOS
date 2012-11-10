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
 * Setup for the NONSTANDARD MSP430-F5437 proto board.
 */

/*
 * Board identifier.
 */
#define BOARD_NONSTANDARD_MSP430_F5437
#define BOARD_NAME "NONSTANDARD MSP430-F5437"

/*
 * Clock constants.
 */
#define LFXT1CLK        (32768) // Hz
#define DCOCLK          (20000000ul) // Hz

/*
 * Pin definitions for the NONSTANDARD MSP430-F5437 board.
 */
#define P3_O_TXD0               (4)
#define P3_O_TXD0_MASK          (1 << P3_O_TXD0)
#define P3_I_RXD0               (5)
#define P3_I_RXD0_MASK          (1 << P3_I_RXD0)

#define P5_O_TXD1               (6)
#define P5_O_TXD1_MASK          (1 << P5_O_TXD1)
#define P5_I_RXD1               (7)
#define P5_I_RXD1_MASK          (1 << P5_I_RXD1)

#define P6_O_GREEN_LED          (1)
#define P6_O_GREEN_LED_MASK     (1 << P6_O_GREEN_LED)

#define P6_O_RED_LED            (2)
#define P6_O_RED_LED_MASK       (1 << P6_O_RED_LED)

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

#define VAL_P5OUT       P5_O_TXD1_MASK
#define VAL_P5DIR       ~P5_I_RXD1_MASK

#define VAL_P6OUT       ~P6_O_GREEN_LED_MASK
#define VAL_P6DIR       (P6_O_GREEN_LED_MASK | P6_O_RED_LED_MASK)

#define VAL_P7OUT       0x00
#define VAL_P7DIR       0xFF

#define VAL_P8OUT       0x00
#define VAL_P8DIR       0xFF

#define VAL_P9OUT       0x00
#define VAL_P9DIR       0xFF

#define VAL_P10OUT      0x00
#define VAL_P10DIR      0xFF

#define VAL_P11OUT      0x00
#define VAL_P11DIR      0xFF

#define VAL_P12OUT      0x00
#define VAL_P12DIR      0xFF

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
