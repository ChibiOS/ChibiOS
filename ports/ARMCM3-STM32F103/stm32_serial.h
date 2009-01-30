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

#ifndef _STM32_SERIAL_H_
#define _STM32_SERIAL_H_

//#define USE_USART1
#define USE_USART2
//#define USE_USART3

/*
 * Configuration parameter, you can change the depth of the queue buffers
 * depending on the requirements of your application.
 */
#define SERIAL_BUFFERS_SIZE 128

/*
 * USARTs definitions here, ST headers are not good enough IMHO.
 */
#define SR_PE                   (1 << 0)
#define SR_FE                   (1 << 1)
#define SR_NE                   (1 << 2)
#define SR_ORE                  (1 << 3)
#define SR_IDLE                 (1 << 4)
#define SR_RXNE                 (1 << 5)
#define SR_TC                   (1 << 6)
#define SR_TXE                  (1 << 7)
#define SR_LBD                  (1 << 8)
#define SR_CTS                  (1 << 9)

#define CR1_SBK                 (1 << 0)
#define CR1_RWU                 (1 << 1)
#define CR1_RE                  (1 << 2)
#define CR1_TE                  (1 << 3)
#define CR1_IDLEIE              (1 << 4)
#define CR1_RXNEIE              (1 << 5)
#define CR1_TCIE                (1 << 6)
#define CR1_TXEIE               (1 << 7)
#define CR1_PEIE                (1 << 8)
#define CR1_PS                  (1 << 9)
#define CR1_PCE                 (1 << 10)
#define CR1_WAKE                (1 << 11)
#define CR1_M                   (1 << 12)
#define CR1_UE                  (1 << 13)

#define CR2_ADD_MASK            (15 << 0)
#define CR2_LBDL                (1 << 5)
#define CR2_LBDIE               (1 << 6)
#define CR2_CBCL                (1 << 8)
#define CR2_CPHA                (1 << 9)
#define CR2_CPOL                (1 << 10)
#define CR2_CLKEN               (1 << 11)
#define CR2_STOP_MASK           (3 << 12)
#define CR2_STOP1_BITS          (0 << 12)
#define CR2_STOP0P5_BITS        (1 << 12)
#define CR2_STOP2_BITS          (2 << 12)
#define CR2_STOP1P5_BITS        (3 << 12)
#define CR2_LINEN               (1 << 14)

#define CR3_EIE                 (1 << 0)
#define CR3_IREN                (1 << 1)
#define CR3_IRLP                (1 << 2)
#define CR3_HDSEL               (1 << 3)
#define CR3_NACK                (1 << 4)
#define CR3_SCEN                (1 << 5)
#define CR3_DMAR                (1 << 6)
#define CR3_DMAT                (1 << 7)
#define CR3_RTSE                (1 << 8)
#define CR3_CTSE                (1 << 9)
#define CR3_CTSIE               (1 << 10)

#ifdef USE_USART1
extern FullDuplexDriver COM1;
#endif
#ifdef USE_USART2
extern FullDuplexDriver COM2;
#endif
#ifdef USE_USART3
extern FullDuplexDriver COM3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void InitSerial(uint32_t prio1, uint32_t prio2, uint32_t prio3);
  void SetUSARTI(USART_TypeDef *u, uint32_t speed, uint16_t cr1,
                 uint16_t cr2, uint16_t cr3);
#ifdef __cplusplus
}
#endif

#endif /* _STM32_SERIAL_H_ */
