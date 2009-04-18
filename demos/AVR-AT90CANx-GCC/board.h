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

#define BOARD_OLIMEX_AVR_CAN

/*
 * All inputs with pullups.
 */
#define VAL_DDRA  0x00
#define VAL_PORTA 0xFF

/*
 * All inputs with pullups.
 */
#define VAL_DDRB  0x00
#define VAL_PORTB 0xFF

/*
 * All inputs with pullups.
 */
#define VAL_DDRC  0x00
#define VAL_PORTC 0xFF

/*       PD7 PD6 PD5 PD4 PD3 PD2 PD1 PD0
 *        IN  IN OUT  IN OUT  IN  IN  IN
 * DDRD    0   0   1   0   1   0   0   0
 *        PU HiZ VAL  PU VAL HiZ HiZ HiZ
 * PORTD   1   0  ?1   1   1   0   0   0
 */
#define VAL_DDRD  0x28
#define VAL_PORTD 0xB8

/*       PE7 PE6 BUT LED PE3 PE2 PE1 PE0
 *        IN  IN  IN OUT  IN  IN OUT  IN
 * DDRE    0   0   0   1   0   0   1   0
 *        PU  PU HiZ VAL  PU  PU VAL HiZ
 * PORTE   1   1   0   1   1   1   1   0
 */
#define VAL_DDRE  0x12
#define VAL_PORTE 0xDE

/*       TDI TDO TMS TCK PF3 PF2 PF1 PF0
 *         x   x   x   x  IN  IN  IN  IN
 * DDRF    0   0   0   0   0   0   0   0
 *         x   x   x   x  PU  PU  PU  PU
 * PORTF   0   0   0   0   1   1   1   1
 *
 */
#define VAL_DDRF  0x00
#define VAL_PORTF 0x0F

/*         x   x   x   x   x PG2 PG1 PG0
 *         x   x   x   x   x  IN  IN  IN
 * DDRG    0   0   0   0   0   0   0   0
 *         x   x   x   x   x  PU  PU  PU
 * PORTG   0   0   0   0   0   1   1   1
 *
 */
#define VAL_DDRG  0x00
#define VAL_PORTG 0x07

#define PORTE_LED               (1 << 4)
#define PORTE_BUTTON            (1 << 5)

void hwinit(void);

#endif /* _BOARD_H_ */
