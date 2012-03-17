/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    STM8L/serial_lld.h
 * @brief   STM8L low level serial driver header.
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

#define SD_MODE_PARITY          0x07        /**< @brief Parity field mask.  */
#define SD_MODE_PARITY_NONE     0x00        /**< @brief No parity.          */
#define SD_MODE_PARITY_EVEN     0x05        /**< @brief Even parity.        */
#define SD_MODE_PARITY_ODD      0x07        /**< @brief Odd parity.         */

#define SD_MODE_STOP            0x30        /**< @brief Stop bits mask.     */
#define SD_MODE_STOP_1          0x00        /**< @brief One stop bit.       */
#define SD_MODE_STOP_2          0x20        /**< @brief Two stop bits.      */
#define SD_MODE_STOP_1P5        0x30        /**< @brief 1.5 stop bits.      */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   USART1 driver enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM8L_SERIAL_USE_USART1) || defined(__DOXYGEN__)
#define STM8L_SERIAL_USE_USART1     TRUE
#endif

/**
 * @brief   USART2 driver enable switch.
 * @details If set to @p TRUE the support for USART3 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM8L_SERIAL_USE_USART2) || defined(__DOXYGEN__)
#define STM8L_SERIAL_USE_USART2     TRUE
#endif

/**
 * @brief   USART3 driver enable switch.
 * @details If set to @p TRUE the support for USART3 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM8L_SERIAL_USE_USART3) || defined(__DOXYGEN__)
#define STM8L_SERIAL_USE_USART3     TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM8L_SERIAL_USE_USART1 && !STM8L_HAS_USART1
#error "USART1 not present in the selected device"
#endif

#if STM8L_SERIAL_USE_USART2 && !STM8L_HAS_USART2
#error "USART2 not present in the selected device"
#endif

#if STM8L_SERIAL_USE_USART3 && !STM8L_HAS_USART3
#error "USART3 not present in the selected device"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Generic Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 * @note    This structure content is architecture dependent, each driver
 *          implementation defines its own version and the custom static
 *          initializers.
 */
typedef struct {
  /**
   * @brief Bit rate register.
   */
  uint16_t                  sc_brr;
  /**
   * @brief Mode flags.
   */
  uint8_t                   sc_mode;
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
  USART_TypeDef             *usart;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Macro for baud rate computation.
 * @note    Make sure the final baud rate is within tolerance.
 */
#define BRR(b) (SYSCLK / (b))

#if STM8L_SERIAL_USE_USART1 || defined(__DOXYGEN__)
/**
 * @brief USART1 RX interrupt handler segment.
 */
#define _USART1_RECEIVE_ISR() {                                             \
  uint8_t sr = USART1->SR;                                                  \
  if (sr & (USART_SR_RXNE | USART_SR_OR | USART_SR_NF |                     \
            USART_SR_FE | USART_SR_PE)) {                                   \
    if (sr & (USART_SR_OR | USART_SR_NF | USART_SR_FE | USART_SR_PE))       \
      sd_lld_set_error(&SD1, sr);                                           \
    chSysLockFromIsr();                                                     \
    sdIncomingDataI(&SD1, USART1->DR);                                      \
    chSysUnlockFromIsr();                                                   \
  }                                                                         \
}

/**
 * @brief USART1 TX interrupt handler segment.
 */
#define _USART1_TRANSMIT_ISR() {                                            \
  if (USART1->SR & USART_SR_TXE) {                                          \
    msg_t b;                                                                \
    chSysLockFromIsr();                                                     \
    b = sdRequestDataI(&SD1);                                               \
    chSysUnlockFromIsr();                                                   \
    if (b < Q_OK)                                                           \
      USART1->CR2 &= (uint8_t)~USART_CR2_TIEN;                              \
    else                                                                    \
      USART1->DR = (uint8_t)b;                                              \
  }                                                                         \
}
#endif /* STM8L_SERIAL_USE_USART1 */

#if STM8L_SERIAL_USE_USART2 || defined(__DOXYGEN__)
/**
 * @brief USART2 RX interrupt handler segment.
 */
#define _USART2_RECEIVE_ISR() {                                             \
  uint8_t sr = USART2->SR;                                                  \
  if (sr & (USART_SR_RXNE | USART_SR_OR | USART_SR_NF |                     \
            USART_SR_FE | USART_SR_PE)) {                                   \
    if (sr & (USART_SR_OR | USART_SR_NF | USART_SR_FE | USART_SR_PE))       \
      sd_lld_set_error(&SD2, sr);                                           \
    chSysLockFromIsr();                                                     \
    sdIncomingDataI(&SD2, USART2->DR);                                      \
    chSysUnlockFromIsr();                                                   \
  }                                                                         \
}

/**
 * @brief USART2 TX interrupt handler segment.
 */
#define _USART2_TRANSMIT_ISR() {                                            \
  if (USART2->SR & USART_SR_TXE) {                                          \
    msg_t b;                                                                \
    chSysLockFromIsr();                                                     \
    b = sdRequestDataI(&SD2);                                               \
    chSysUnlockFromIsr();                                                   \
    if (b < Q_OK)                                                           \
      USART2->CR2 &= (uint8_t)~USART_CR2_TIEN;                              \
    else                                                                    \
      USART2->DR = (uint8_t)b;                                              \
  }                                                                         \
}
#endif /* STM8L_SERIAL_USE_USART2 */

#if STM8L_SERIAL_USE_USART3 || defined(__DOXYGEN__)
/**
 * @brief USART3 RX interrupt handler segment.
 */
#define _USART3_RECEIVE_ISR() {                                             \
  uint8_t sr = USART3->SR;                                                  \
  if (sr & (USART_SR_RXNE | USART_SR_OR | USART_SR_NF |                     \
            USART_SR_FE | USART_SR_PE)) {                                   \
    if (sr & (USART_SR_OR | USART_SR_NF | USART_SR_FE | USART_SR_PE))       \
      sd_lld_set_error(&SD3, sr);                                           \
    chSysLockFromIsr();                                                     \
    sdIncomingDataI(&SD3, USART3->DR);                                      \
    chSysUnlockFromIsr();                                                   \
  }                                                                         \
}

/**
 * @brief USART3 TX interrupt handler segment.
 */
#define _USART3_TRANSMIT_ISR() {                                            \
  if (USART3->SR & USART_SR_TXE) {                                          \
    msg_t b;                                                                \
    chSysLockFromIsr();                                                     \
    b = sdRequestDataI(&SD3);                                               \
    chSysUnlockFromIsr();                                                   \
    if (b < Q_OK)                                                           \
      USART3->CR2 &= (uint8_t)~USART_CR2_TIEN;                              \
    else                                                                    \
      USART3->DR = (uint8_t)b;                                              \
  }                                                                         \
}
#endif /* STM8L_SERIAL_USE_USART3 */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM8L_SERIAL_USE_USART1 && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif
#if STM8L_SERIAL_USE_USART2 && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif
#if STM8L_SERIAL_USE_USART3 && !defined(__DOXYGEN__)
extern SerialDriver SD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
  void sd_lld_set_error(SerialDriver *sdp, uint8_t sr);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */
