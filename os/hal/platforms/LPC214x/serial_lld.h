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

/**
 * @file LPC214x/serial_lld.h
 * @brief LPC214x low level serial driver header.
 * @addtogroup LPC214x_SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

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
 * @brief FIFO preload parameter.
 * @details Configuration parameter, this values defines how many bytes are
 * preloaded in the HW transmit FIFO for each interrupt, the maximum value is
 * 16 the minimum is 2, the value 0 disables the feature.
 * @note An high value reduces the number of interrupts generated but can
 *       also increase the worst case interrupt response time because the
 *       preload loops.
 * @note The value zero disables the feature and reverts to a simpler code
 *       that will generate an interrupt for each output byte but is much
 *       smaller and simpler.
 */
#if !defined(UART_FIFO_PRELOAD) || defined(__DOXYGEN__)
#define UART_FIFO_PRELOAD 16
#endif

/**
 * @brief UART0 driver enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note The default is @p TRUE .
 */
#if !defined(USE_LPC214x_UART0) || defined(__DOXYGEN__)
#define USE_LPC214x_UART0 TRUE
#endif

/**
 * @brief UART1 driver enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_LPC214x_UART1) || defined(__DOXYGEN__)
#define USE_LPC214x_UART1 TRUE
#endif

/**
 * @brief UART1 interrupt priority level setting.
 */
#if !defined(LPC214x_UART1_PRIORITY) || defined(__DOXYGEN__)
#define LPC214x_UART1_PRIORITY  1
#endif

/**
 * @brief UART2 interrupt priority level setting.
 */
#if !defined(LPC214x_UART2_PRIORITY) || defined(__DOXYGEN__)
#define LPC214x_UART2_PRIORITY   2
#endif

/*===========================================================================*/
/* Unsupported event flags and custom events.                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * Serial Driver condition flags type.
 */
typedef uint32_t sdflags_t;

/**
 * @brief @p SerialDriver specific data.
 */
struct _serial_driver_data {
  /**
   * Input queue, incoming data can be read from this input queue by
   * using the queues APIs.
   */
  InputQueue            iqueue;
  /**
   * Output queue, outgoing data can be written to this output queue by
   * using the queues APIs.
   */
  OutputQueue           oqueue;
  /**
   * Status Change @p EventSource. This event is generated when one or more
   * condition flags change.
   */
  EventSource           sevent;
  /**
   * I/O driver status flags.
   */
  sdflags_t             flags;
  /**
   * Input circular buffer.
   */
  uint8_t               ib[SERIAL_BUFFERS_SIZE];
  /**
   * Output circular buffer.
   */
  uint8_t               ob[SERIAL_BUFFERS_SIZE];
};

/**
 * @brief LPC214x Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 */
typedef struct {
  uint32_t              speed;
  uint32_t              lcr;
  uint32_t              fcr;
} SerialDriverConfig;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/** @cond never*/
#if USE_LPC214x_UART0
extern SerialDriver SD1;
#endif
#if USE_LPC214x_UART1
extern SerialDriver SD2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialDriverConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif /* CH_HAL_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */
