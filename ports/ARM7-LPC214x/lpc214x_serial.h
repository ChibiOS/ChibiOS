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

#ifndef _LPC214x_SERIAL_H_
#define _LPC214x_SERIAL_H_

/*
 * Configuration parameter, this values defines how many bytes are preloaded
 * in the HW transmit FIFO for each interrupt, the maximum value is 16 the
 * minimum is 2.
 * NOTE: A greater value reduces the number of interrupts generated but can
 *       also increase the worst case interrupt response time.
 * NOTE: You can undefine the following macro and revert to a simpler code
 *       that will generate an interrupt for each output byte,
 */
#define FIFO_PRELOAD 16

/*
 * Configuration parameter, you can change the depth of the queue buffers
 * depending on the requirements of your application.
 */
#define SERIAL_BUFFERS_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif
  void InitSerial(int vector1, int vector2);
  void UART0IrqHandler(void);
  void UART1IrqHandler(void);
  void SetUARTI(UART *u, int speed, int lcr, int fcr);
#ifdef __cplusplus
}
#endif

extern FullDuplexDriver COM1, COM2;

#endif /* _LPC214x_SERIAL_H_*/
