/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM32/serial_lld.h
 * @brief   STM32 low level serial driver header.
 *
 * @addtogroup SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   USART1 driver enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SERIAL_USE_USART1) || defined(__DOXYGEN__)
#define STM32_SERIAL_USE_USART1             TRUE
#endif

/**
 * @brief   USART2 driver enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_SERIAL_USE_USART2) || defined(__DOXYGEN__)
#define STM32_SERIAL_USE_USART2             TRUE
#endif

/**
 * @brief   USART3 driver enable switch.
 * @details If set to @p TRUE the support for USART3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SERIAL_USE_USART3) || defined(__DOXYGEN__)
#define STM32_SERIAL_USE_USART3             TRUE
#endif

/**
 * @brief   UART4 driver enable switch.
 * @details If set to @p TRUE the support for UART4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SERIAL_USE_UART4) || defined(__DOXYGEN__)
#define STM32_SERIAL_USE_UART4              TRUE
#endif

/**
 * @brief   UART5 driver enable switch.
 * @details If set to @p TRUE the support for UART5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SERIAL_USE_UART5) || defined(__DOXYGEN__)
#define STM32_SERIAL_USE_UART5              TRUE
#endif

/**
 * @brief   USART1 interrupt priority level setting.
 */
#if !defined(STM32_SERIAL_USART1_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SERIAL_USART1_PRIORITY        12
#endif

/**
 * @brief   USART2 interrupt priority level setting.
 */
#if !defined(STM32_SERIAL_USART2_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SERIAL_USART2_PRIORITY        12)
#endif

/**
 * @brief   USART3 interrupt priority level setting.
 */
#if !defined(STM32_SERIAL_USART3_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SERIAL_USART3_PRIORITY        12
#endif

/**
 * @brief   UART4 interrupt priority level setting.
 */
#if !defined(STM32_SERIAL_UART4_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SERIAL_UART4_PRIORITY         12
#endif

/**
 * @brief   UART5 interrupt priority level setting.
 */
#if !defined(STM32_SERIAL_UART5_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SERIAL_UART5_PRIORITY         12
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_SERIAL_USE_USART1 && !STM32_HAS_USART1
#error "USART1 not present in the selected device"
#endif

#if STM32_SERIAL_USE_USART2 && !STM32_HAS_USART2
#error "USART2 not present in the selected device"
#endif

#if STM32_SERIAL_USE_USART3 && !STM32_HAS_USART3
#error "USART3 not present in the selected device"
#endif

#if STM32_SERIAL_USE_UART4 && !STM32_HAS_UART4
#error "UART4 not present in the selected device"
#endif

#if STM32_SERIAL_USE_UART5 && !STM32_HAS_UART5
#error "UART5 not present in the selected device"
#endif

#if !STM32_SERIAL_USE_USART1 && !STM32_SERIAL_USE_USART2 &&                 \
    !STM32_SERIAL_USE_USART3 && !STM32_SERIAL_USE_UART4  &&                 \
    !STM32_SERIAL_USE_UART5
#error "SERIAL driver activated but no USART/UART peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   STM32 Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 * @note    This structure content is architecture dependent, each driver
 *          implementation defines its own version and the custom static
 *          initializers.
 */
typedef struct {
  /**
   * @brief Bit rate.
   */
  uint32_t                  sc_speed;
  /**
   * @brief Initialization value for the CR1 register.
   */
  uint16_t                  sc_cr1;
  /**
   * @brief Initialization value for the CR2 register.
   */
  uint16_t                  sc_cr2;
  /**
   * @brief Initialization value for the CR3 register.
   */
  uint16_t                  sc_cr3;
} SerialConfig;

/**
 * @brief   @p SerialDriver specific data.
 */
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Input queue.*/                                                         \
  InputQueue                iqueue;                                         \
  /* Output queue.*/                                                        \
  OutputQueue               oqueue;                                         \
  /* Input circular buffer.*/                                               \
  uint8_t                   ib[SERIAL_BUFFERS_SIZE];                        \
  /* Output circular buffer.*/                                              \
  uint8_t                   ob[SERIAL_BUFFERS_SIZE];                        \
  /* End of the mandatory fields.*/                                         \
  /* Pointer to the USART registers block.*/                                \
  USART_TypeDef             *usart;

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
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_SERIAL_USE_USART1 && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif
#if STM32_SERIAL_USE_USART2 && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif
#if STM32_SERIAL_USE_USART3 && !defined(__DOXYGEN__)
extern SerialDriver SD3;
#endif
#if STM32_SERIAL_USE_UART4 && !defined(__DOXYGEN__)
extern SerialDriver SD4;
#endif
#if STM32_SERIAL_USE_UART5 && !defined(__DOXYGEN__)
extern SerialDriver SD5;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */
