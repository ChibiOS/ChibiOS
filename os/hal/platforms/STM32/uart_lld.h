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
 * @file    STM32/uart_lld.h
 * @brief   STM32 low level UART driver header.
 *
 * @addtogroup UART
 * @{
 */

#ifndef _UART_LLD_H_
#define _UART_LLD_H_

#if HAL_USE_UART || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   UART driver on USART1 enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_UART_USE_USART1) || defined(__DOXYGEN__)
#define STM32_UART_USE_USART1               TRUE
#endif

/**
 * @brief   UART driver on USART2 enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_UART_USE_USART2) || defined(__DOXYGEN__)
#define STM32_UART_USE_USART2               TRUE
#endif

/**
 * @brief   UART driver on USART3 enable switch.
 * @details If set to @p TRUE the support for USART3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_UART_USE_USART3) || defined(__DOXYGEN__)
#define STM32_UART_USE_USART3               TRUE
#endif

/**
 * @brief   USART1 interrupt priority level setting.
 */
#if !defined(STM32_UART_USART1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_UART_USART1_IRQ_PRIORITY      12
#endif

/**
 * @brief   USART2 interrupt priority level setting.
 */
#if !defined(STM32_UART_USART2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_UART_USART2_IRQ_PRIORITY      12
#endif

/**
 * @brief   USART3 interrupt priority level setting.
 */
#if !defined(STM32_UART_USART3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_UART_USART3_IRQ_PRIORITY      12
#endif

/**
 * @brief   USART1 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(STM32_UART_USART1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_UART_USART1_DMA_PRIORITY      0
#endif

/**
 * @brief   USART2 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(STM32_UART_USART2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_UART_USART2_DMA_PRIORITY      0
#endif
/**
 * @brief   USART3 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(STM32_UART_USART3_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_UART_USART3_DMA_PRIORITY      0
#endif

/**
 * @brief   USART1 DMA error hook.
 * @note    The default action for DMA errors is a system halt because DMA error
 *          can only happen because programming errors.
 */
#if !defined(STM32_UART_USART1_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_UART_USART1_DMA_ERROR_HOOK()  chSysHalt()
#endif

/**
 * @brief   USART2 DMA error hook.
 * @note    The default action for DMA errors is a system halt because DMA error
 *          can only happen because programming errors.
 */
#if !defined(STM32_UART_USART2_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_UART_USART2_DMA_ERROR_HOOK()  chSysHalt()
#endif

/**
 * @brief   USART3 DMA error hook.
 * @note    The default action for DMA errors is a system halt because DMA error
 *          can only happen because programming errors.
 */
#if !defined(STM32_UART_USART3_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_UART_USART3_DMA_ERROR_HOOK()  chSysHalt()
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_UART_USE_USART1 && !STM32_HAS_USART1
#error "USART1 not present in the selected device"
#endif

#if STM32_UART_USE_USART2 && !STM32_HAS_USART2
#error "USART2 not present in the selected device"
#endif

#if STM32_UART_USE_USART3 && !STM32_HAS_USART3
#error "USART3 not present in the selected device"
#endif

#if STM32_UART_USE_UART4 && !STM32_HAS_UART4
#error "UART4 not present in the selected device"
#endif

#if !STM32_UART_USE_USART1 && !STM32_UART_USE_USART2 &&                     \
    !STM32_UART_USE_USART3 && !STM32_UART_USE_UART4
#error "UART driver activated but no USART/UART peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   UART driver condition flags type.
 */
typedef uint32_t uartflags_t;

/**
 * @brief   Structure representing an UART driver.
 */
typedef struct UARTDriver UARTDriver;

/**
 * @brief   Generic UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
typedef void (*uartcb_t)(UARTDriver *uartp);

/**
 * @brief   Character received UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] c         received character
 */
typedef void (*uartccb_t)(UARTDriver *uartp, uint16_t c);

/**
 * @brief   Receive error UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] e         receive error mask
 */
typedef void (*uartecb_t)(UARTDriver *uartp, uartflags_t e);

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief End of transmission buffer callback.
   */
  uartcb_t                  uc_txend1;
  /**
   * @brief Physical end of transmission callback.
   */
  uartcb_t                  uc_txend2;
  /**
   * @brief Receive buffer filled callback.
   */
  uartcb_t                  uc_rxend;
  /**
   * @brief Character received while out if the @p UART_RECEIVE state.
   */
  uartccb_t                 uc_rxchar;
  /**
   * @brief Receive error callback.
   */
  uartecb_t                 uc_rxerr;
  /* End of the mandatory fields.*/
  /**
   * @brief Bit rate.
   */
  uint32_t                  uc_speed;
  /**
   * @brief Initialization value for the CR1 register.
   */
  uint16_t                  uc_cr1;
  /**
   * @brief Initialization value for the CR2 register.
   */
  uint16_t                  uc_cr2;
  /**
   * @brief Initialization value for the CR3 register.
   */
  uint16_t                  uc_cr3;
} UARTConfig;

/**
 * @brief   Structure representing an UART driver.
 */
struct UARTDriver {
  /**
   * @brief Driver state.
   */
  uartstate_t               ud_state;
  /**
   * @brief Transmitter state.
   */
  uarttxstate_t             ud_txstate;
  /**
   * @brief Receiver state.
   */
  uartrxstate_t             ud_rxstate;
  /**
   * @brief Current configuration data.
   */
  const UARTConfig          *ud_config;
#if defined(UART_DRIVER_EXT_FIELDS)
  UART_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the USART registers block.
   */
  USART_TypeDef             *ud_usart;
  /**
   * @brief Pointer to the DMA registers block.
   */
  stm32_dma_t               *ud_dmap;
  /**
   * @brief DMA priority bit mask.
   */
  uint32_t                  ud_dmaccr;
  /**
   * @brief Receive DMA channel.
   */
  uint8_t                   ud_dmarx;
  /**
   * @brief Transmit DMA channel.
   */
  uint8_t                   ud_dmatx;
  /**
   * @brief Default receive buffer while into @p UART_RX_IDLE state.
   */
  volatile uint16_t         ud_rxbuf;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_UART_USE_USART1 && !defined(__DOXYGEN__)
extern UARTDriver UARTD1;
#endif

#if STM32_UART_USE_USART2 && !defined(__DOXYGEN__)
extern UARTDriver UARTD2;
#endif

#if STM32_UART_USE_USART3 && !defined(__DOXYGEN__)
extern UARTDriver UARTD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void uart_lld_init(void);
  void uart_lld_start(UARTDriver *uartp);
  void uart_lld_stop(UARTDriver *uartp);
  void uart_lld_start_send(UARTDriver *uartp, size_t n, const void *txbuf);
  size_t uart_lld_stop_send(UARTDriver *uartp);
  void uart_lld_start_receive(UARTDriver *uartp, size_t n, void *rxbuf);
  size_t uart_lld_stop_receive(UARTDriver *uartp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_UART */

#endif /* _UART_LLD_H_ */

/** @} */
