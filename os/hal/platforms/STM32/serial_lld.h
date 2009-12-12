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
 * @file STM32/serial_lld.h
 * @brief STM32 low level serial driver header.
 * @addtogroup STM32_SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief Serial buffers size setting.
 * @details Configuration parameter, you can change the depth of the queue
 * buffers depending on the requirements of your application.
 * @note The default is 128 bytes for both the transmission and receive buffers.
 */
#if !defined(SERIAL_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_BUFFERS_SIZE 128
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

/**
 * @brief USART1 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_USART1_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_USART1_PRIORITY) || defined(__DOXYGEN__)
#define STM32_USART1_PRIORITY   0xC0
#endif

/**
 * @brief USART2 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_USART2_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_USART2_PRIORITY) || defined(__DOXYGEN__)
#define STM32_USART2_PRIORITY   0xC0
#endif

/**
 * @brief USART3 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_USART3_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_USART3_PRIORITY) || defined(__DOXYGEN__)
#define STM32_USART3_PRIORITY   0xC0
#endif

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*
 * Extra USARTs definitions here (missing from the ST header file).
 */
#define USART_CR2_STOP1_BITS    (0 << 12)   /**< @brief CR2 1 stop bit value.*/
#define USART_CR2_STOP0P5_BITS  (1 << 12)   /**< @brief CR2 0.5 stop bit value.*/
#define USART_CR2_STOP2_BITS    (2 << 12)   /**< @brief CR2 2 stop bit value.*/
#define USART_CR2_STOP1P5_BITS  (3 << 12)   /**< @brief CR2 1.5 stop bit value.*/

/*===========================================================================*/
/* Driver data structures.                                                   */
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
 * @brief STM32 Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 *
 * @note This structure content is architecture dependent, each driver
 *       implementation defines its own version and the custom static
 *       initializers.
 */
typedef struct {

  uint32_t              speed;
  uint16_t              cr1;
  uint16_t              cr2;
  uint16_t              cr3;
} SerialDriverConfig;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/** @cond never*/
#if USE_STM32_USART1
extern SerialDriver SD1;
#endif
#if USE_STM32_USART2
extern SerialDriver SD2;
#endif
#if USE_STM32_USART3
extern SerialDriver SD3;
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
