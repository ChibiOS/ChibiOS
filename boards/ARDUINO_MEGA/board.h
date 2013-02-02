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
 * Setup for the Arduino Mega board.
 */

/*
 * Board identifier.
 */
#define BOARD_ARDUINO_MEGA
#define BOARD_NAME "Arduino Mega"

/* All inputs with pull-ups */
#define VAL_DDRA  0x00
#define VAL_PORTA 0xFF

/* All inputs except PB7 which has a LED connected */
#define VAL_DDRB  0x80
#define VAL_PORTB 0xFF

/* All inputs with pull-ups */
#define VAL_DDRC  0x00
#define VAL_PORTC 0xFF

/* All inputs with pull-ups */
#define VAL_DDRD  0x00
#define VAL_PORTD 0xFF

/* All inputs except PE1 (Serial TX0) */
#define VAL_DDRE  0x02
#define VAL_PORTE 0xFF

/* All inputs with pull-ups */
#define VAL_DDRF  0x00
#define VAL_PORTF 0xFF

/* All inputs with pull-ups */
#define VAL_DDRG  0x00
#define VAL_PORTG 0xFF

/* All inputs with pull-ups */
#define VAL_DDRH  0x00
#define VAL_PORTH 0xFF

/* All inputs with pull-ups */
#define VAL_DDRJ  0x00
#define VAL_PORTJ 0xFF

/* All inputs with pull-ups */
#define VAL_DDRK  0x00
#define VAL_PORTK 0xFF

/* All inputs with pull-ups */
#define VAL_DDRL  0x00
#define VAL_PORTL 0xFF

#define PORTB_LED1        7

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
