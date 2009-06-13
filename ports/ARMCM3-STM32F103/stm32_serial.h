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

/**
 * @file ports/ARMCM3-STM32F103/stm32_serial.h
 * @brief STM32F103 Serial driver macros and structures.
 * @addtogroup STM32F103_SERIAL
 * @{
 */

#ifndef _STM32_SERIAL_H_
#define _STM32_SERIAL_H_

/**
 * @brief Serial buffers size.
 * @details Configuration parameter, you can change the depth of the queue
 * buffers depending on the requirements of your application.
 * @note The default is 128 bytes for both the transmission and receive buffers.
 */
#if !defined(SERIAL_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_BUFFERS_SIZE 128
#endif

/**
 * @brief Default bit rate.
 * @details Configuration parameter, at startup the USARTs are configured at
 * this speed.
 * @note It is possible to use @p SetUSART() in order to change the working
 *       parameters at runtime.
 */
#if !defined(DEFAULT_USART_BITRATE) || defined(__DOXYGEN__)
#define DEFAULT_USART_BITRATE 38400
#endif

/**
 * @brief USART1 driver enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note The default is @p FALSE.
 */
#if !defined(USE_STM32_USART1) || defined(__DOXYGEN__)
#define USE_STM32_USART1 FALSE
#endif

/**
 * @brief USART2 driver enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_USART2) || defined(__DOXYGEN__)
#define USE_STM32_USART2 TRUE
#endif

/**
 * @brief USART3 driver enable switch.
 * @details If set to @p TRUE the support for USART3 is included.
 * @note The default is @p FALSE.
 */
#if !defined(USE_STM32_USART3) || defined(__DOXYGEN__)
#define USE_STM32_USART3 FALSE
#endif

/*
 * USARTs definitions here.
 */
#define SR_PE                   (1 << 0)    /**< @brief SR PE bit.*/
#define SR_FE                   (1 << 1)    /**< @brief SR FE bit.*/
#define SR_NE                   (1 << 2)    /**< @brief SR NE bit.*/
#define SR_ORE                  (1 << 3)    /**< @brief SR ORE bit.*/
#define SR_IDLE                 (1 << 4)    /**< @brief SR IDLE bit.*/
#define SR_RXNE                 (1 << 5)    /**< @brief SR RXNE bit.*/
#define SR_TC                   (1 << 6)    /**< @brief SR TC bit.*/
#define SR_TXE                  (1 << 7)    /**< @brief SR TXE bit.*/
#define SR_LBD                  (1 << 8)    /**< @brief SR LBD bit.*/
#define SR_CTS                  (1 << 9)    /**< @brief SR CTS bit.*/

#define CR1_SBK                 (1 << 0)    /**< @brief CR1 SBK bit.*/
#define CR1_RWU                 (1 << 1)    /**< @brief CR1 RWU bit.*/
#define CR1_RE                  (1 << 2)    /**< @brief CR1 RE bit.*/
#define CR1_TE                  (1 << 3)    /**< @brief CR1 TE bit.*/
#define CR1_IDLEIE              (1 << 4)    /**< @brief CR1 IDLEIE bit.*/
#define CR1_RXNEIE              (1 << 5)    /**< @brief CR1 RXNEIE bit.*/
#define CR1_TCIE                (1 << 6)    /**< @brief CR1 TCTE bit.*/
#define CR1_TXEIE               (1 << 7)    /**< @brief CR1 TXEIE bit.*/
#define CR1_PEIE                (1 << 8)    /**< @brief CR1 PEIE bit.*/
#define CR1_PS                  (1 << 9)    /**< @brief CR1 PS bit.*/
#define CR1_PCE                 (1 << 10)   /**< @brief CR1 PCE bit.*/
#define CR1_WAKE                (1 << 11)   /**< @brief CR1 WAKE bit.*/
#define CR1_M                   (1 << 12)   /**< @brief CR1 M bit.*/
#define CR1_UE                  (1 << 13)   /**< @brief CR1 UE bit.*/

#define CR2_ADD_MASK            (15 << 0)   /**< @brief CR2 ADD field mask.*/
#define CR2_LBDL                (1 << 5)    /**< @brief CR2 LBDL bit.*/
#define CR2_LBDIE               (1 << 6)    /**< @brief CR2 LBDIE bit.*/
#define CR2_CBCL                (1 << 8)    /**< @brief CR2 CBCL bit.*/
#define CR2_CPHA                (1 << 9)    /**< @brief CR2 CPHA bit.*/
#define CR2_CPOL                (1 << 10)   /**< @brief CR2 CPOL bit.*/
#define CR2_CLKEN               (1 << 11)   /**< @brief CR2 CLKEN bit.*/
#define CR2_STOP_MASK           (3 << 12)   /**< @brief CR2 STOP field mask.*/
#define CR2_STOP1_BITS          (0 << 12)   /**< @brief CR2 1 stop bit value.*/
#define CR2_STOP0P5_BITS        (1 << 12)   /**< @brief CR2 0.5 stop bit value.*/
#define CR2_STOP2_BITS          (2 << 12)   /**< @brief CR2 2 stop bit value.*/
#define CR2_STOP1P5_BITS        (3 << 12)   /**< @brief CR2 1.5 stop bit value.*/
#define CR2_LINEN               (1 << 14)   /**< @brief CR2 LINEN bit.*/

#define CR3_EIE                 (1 << 0)    /**< @brief CR3 EIE bit.*/
#define CR3_IREN                (1 << 1)    /**< @brief CR3 IREN bit.*/
#define CR3_IRLP                (1 << 2)    /**< @brief CR3 IRLP bit.*/
#define CR3_HDSEL               (1 << 3)    /**< @brief CR3 HDSEL bit.*/
#define CR3_NACK                (1 << 4)    /**< @brief CR3 NACK bit.*/
#define CR3_SCEN                (1 << 5)    /**< @brief CR3 SCEN bit.*/
#define CR3_DMAR                (1 << 6)    /**< @brief CR3 DMAR bit.*/
#define CR3_DMAT                (1 << 7)    /**< @brief CR3 DMAT bit.*/
#define CR3_RTSE                (1 << 8)    /**< @brief CR3 RTSE bit.*/
#define CR3_CTSE                (1 << 9)    /**< @brief CR3 CTSE bit.*/
#define CR3_CTSIE               (1 << 10)   /**< @brief CR3 CTSIE bit.*/

/** @cond never*/
#if USE_STM32_USART1
extern FullDuplexDriver COM1;
#endif
#if USE_STM32_USART2
extern FullDuplexDriver COM2;
#endif
#if USE_STM32_USART3
extern FullDuplexDriver COM3;
#endif
/** @endcond*/

#ifdef __cplusplus
extern "C" {
#endif
  void serial_init(uint32_t prio1, uint32_t prio2, uint32_t prio3);
  void usart_setup(USART_TypeDef *u, uint32_t speed, uint16_t cr1,
                   uint16_t cr2, uint16_t cr3);
#ifdef __cplusplus
}
#endif

#endif /* _STM32_SERIAL_H_ */

/** @} */
