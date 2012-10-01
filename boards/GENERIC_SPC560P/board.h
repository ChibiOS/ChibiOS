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
 * Setup for a generic SPC560Pxx proto board.
 */

/*
 * Board identifiers.
 */
#define BOARD_GENERIC_SPC560P
#define BOARD_NAME                  "Generic SPC560P"

/*
 * Board frequencies.
 */
#if !defined(SPC560P_XOSC_CLK)
#define SPC5_XOSC_CLK               40000000
#endif

/*
 * I/O definitions.
 */
#define PB_LIN0_TDX     2
#define PB_LIN0_RDX     3

#define PD_BUTTON1      0
#define PD_BUTTON2      1
#define PD_BUTTON3      2
#define PD_BUTTON4      3

#define PD_LED1         4
#define PD_LED2         5
#define PD_LED3         6
#define PD_LED4         7

/*
 * Support macros.
 */
#define PCR(port, pin)  (((port) * 16) + (pin))

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
