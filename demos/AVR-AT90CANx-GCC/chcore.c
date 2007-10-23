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

#include <ch.h>

#include <avr/io.h>

void hwinit(void) {

  /*
   * I/O ports setup.
   * Everything configured as input with pull-up initially.
   */
  DDRA  = 0;
  PORTA = 0xFF;
  DDRB  = 0;
  PORTB = 0xFF;
  DDRC  = 0;
  PORTC = 0xFF;
  DDRD  = 0;
  PORTD = 0xFF;
  DDRE  = 0;
  PORTE = 0xFF;
  DDRF  = 0;
  PORTF = 0xFF;
  DDRG  = 0;
  PORTG = 0xFF;

  /*
   * Enables Idle mode for SLEEP instruction.
   */
  SMCR = 1;
}

void chSysPause(void) {

  chThdSetPriority(IDLEPRIO);

  while (TRUE) {
//    asm volatile ("sleep");
  }
}

void chSysHalt(void) {

  chSysLock();

  while (TRUE)
    ;
}
