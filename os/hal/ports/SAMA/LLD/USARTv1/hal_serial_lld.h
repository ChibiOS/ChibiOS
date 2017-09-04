/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    USARTv1/hal_serial_lld.h
 * @brief   SAMA low level serial driver header.
 *
 * @addtogroup SERIAL
 * @{
 */

#ifndef HAL_SERIAL_LLD_H
#define HAL_SERIAL_LLD_H

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Advanced buffering support switch.
 * @details This constants enables the advanced buffering support in the
 *          low level driver, the queue buffer is no more part of the
 *          @p SerialDriver structure, each driver can have a different
 *          queue size.
 */
#define SERIAL_ADVANCED_BUFFERING_SUPPORT   TRUE

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   UART0 driver enable switch.
 * @details If set to @p TRUE the support for UART0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SAMA_SERIAL_USE_UART0) || defined(__DOXYGEN__)
#define SAMA_SERIAL_USE_UART0               FALSE
#endif

/**
 * @brief   UART1 driver enable switch.
 * @details If set to @p TRUE the support for UART1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SAMA_SERIAL_USE_UART1) || defined(__DOXYGEN__)
#define SAMA_SERIAL_USE_USART1              FALSE
#endif

/**
 * @brief   UART2 driver enable switch.
 * @details If set to @p TRUE the support for UART2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SAMA_SERIAL_USE_UART2) || defined(__DOXYGEN__)
#define SAMA_SERIAL_USE_UART2               FALSE
#endif

/**
 * @brief   UART3 driver enable switch.
 * @details If set to @p TRUE the support for UART3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SAMA_SERIAL_USE_UART3) || defined(__DOXYGEN__)
#define SAMA_SERIAL_USE_UART3               FALSE
#endif

/**
 * @brief   UART4 driver enable switch.
 * @details If set to @p TRUE the support for UART4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(SAMA_SERIAL_USE_UART4) || defined(__DOXYGEN__)
#define SAMA_SERIAL_USE_UART4               FALSE
#endif

/**
 * @brief   UART0 interrupt priority level setting.
 */
#if !defined(SAMA_SERIAL_UART0_PRIORITY) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART0_PRIORITY          4
#endif

/**
 * @brief   UART1 interrupt priority level setting.
 */
#if !defined(SAMA_SERIAL_UART1_PRIORITY) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART1_PRIORITY          4
#endif

/**
 * @brief   UART2 interrupt priority level setting.
 */
#if !defined(SAMA_SERIAL_UART2_PRIORITY) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART2_PRIORITY          4
#endif

/**
 * @brief   UART3 interrupt priority level setting.
 */
#if !defined(SAMA_SERIAL_UART3_PRIORITY) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART3_PRIORITY          4
#endif

/**
 * @brief   UART4 interrupt priority level setting.
 */
#if !defined(SAMA_SERIAL_UART4_PRIORITY) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART4_PRIORITY          4
#endif

/**
 * @brief   Input buffer size for UART0.
 */
#if !defined(SAMA_SERIAL_UART0_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART0_IN_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for UART0.
 */
#if !defined(SAMA_SERIAL_UART0_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART0_OUT_BUF_SIZE      SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for UART1.
 */
#if !defined(SAMA_SERIAL_UART1_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART1_IN_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for UART1.
 */
#if !defined(SAMA_SERIAL_UART1_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART1_OUT_BUF_SIZE      SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for UART2.
 */
#if !defined(SAMA_SERIAL_UART2_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART2_IN_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for UART2.
 */
#if !defined(SAMA_SERIAL_UART2_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART2_OUT_BUF_SIZE      SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for UART3.
 */
#if !defined(SAMA_SERIAL_UART3_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART3_IN_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for UART3.
 */
#if !defined(SAMA_SERIAL_UART3_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART3_OUT_BUF_SIZE      SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for UART4.
 */
#if !defined(SAMA_SERIAL_UART4_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART4_IN_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for UART4.
 */
#if !defined(SAMA_SERIAL_UART4_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define SAMA_SERIAL_UART4_OUT_BUF_SIZE      SERIAL_BUFFERS_SIZE
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !SAMA_SERIAL_USE_UART0 && !SAMA_SERIAL_USE_UART1 &&                     \
    !SAMA_SERIAL_USE_UART2 && !SAMA_SERIAL_USE_UART3 &&                     \
    !SAMA_SERIAL_USE_UART4
#error "SERIAL driver activated but no USART/UART peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   SAMA Serial Driver configuration structure.
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
  uint32_t                  speed;
  /* End of the mandatory fields.*/
  /**
   * @brief Initialization value for the CR register.
   */
  uint32_t                  cr;
  /**
   * @brief Initialization value for the MR register.
   */
  uint32_t                  mr;
} SerialConfig;

/**
 * @brief   @p SerialDriver specific data.
 */
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Input queue.*/                                                         \
  input_queue_t             iqueue;                                         \
  /* Output queue.*/                                                        \
  output_queue_t            oqueue;                                         \
  /* End of the mandatory fields.*/                                         \
  /* Pointer to the UART registers block.*/                                 \
  Uart                      *uart;                                          \
  /* Clock frequency for the associated USART/UART.*/                       \
  uint32_t                  clock;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if SAMA_SERIAL_USE_UART0 && !defined(__DOXYGEN__)
extern SerialDriver SD0;
#endif
#if SAMA_SERIAL_USE_UART1 && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif
#if SAMA_SERIAL_USE_UART2 && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif
#if SAMA_SERIAL_USE_UART3 && !defined(__DOXYGEN__)
extern SerialDriver SD3;
#endif
#if SAMA_SERIAL_USE_UART4 && !defined(__DOXYGEN__)
extern SerialDriver SD4;
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

#endif /* HAL_SERIAL_LLD_H */

/** @} */
