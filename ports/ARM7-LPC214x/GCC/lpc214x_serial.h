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

#ifndef _LPC2000_SERIAL_H_
#define _LPC2000_SERIAL_H_

void InitSerial(void);
void SetUARTI(UART *u, int speed, int lcr, int fcr);
void UART0IrqHandler(void);
void UART1IrqHandler(void);

extern FullDuplexDriver COM1, COM2;

#endif /* _LPC2000_SERIAL_H_*/
