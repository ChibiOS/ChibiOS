/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    hal_sio.h
 * @brief   SIO Driver macros and structures.
 *
 * @addtogroup SIO
 * @{
 */

#ifndef HAL_SIO_H
#define HAL_SIO_H

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    SIO status flags
 * @{
 */
#define SIO_NO_ERROR           0    /**< @brief No pending conditions.      */
#define SIO_PARITY_ERROR       4    /**< @brief Parity error happened.      */
#define SIO_FRAMING_ERROR      8    /**< @brief Framing error happened.     */
#define SIO_OVERRUN_ERROR      16   /**< @brief Overflow happened.          */
#define SIO_NOISE_ERROR        32   /**< @brief Noise on the line.          */
#define SIO_BREAK_DETECTED     64   /**< @brief Break detected.             */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    SIO configuration options
 * @{
 */
#if !defined(HAL_SIO_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define HAL_SIO_USE_SYNCHRONIZATION         TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   SIO driver condition flags type.
 */
typedef uint_fast8_t sioflags_t;

/**
 * @brief   Type of structure representing a SIO driver.
 */
typedef struct hal_sio_driver SIODriver;

/**
 * @brief   Type of structure representing a SIO configuration.
 */
typedef struct hal_sio_config SIOConfig;

/**
 * @brief   Type of structure representing a SIO operation.
 */
typedef struct hal_sio_operation SIOOperation;

/**
 * @brief   Generic SIO notification callback type.
 *
 * @param[in] siop     pointer to the @p SIODriver object
 */
typedef void (*siocb_t)(SIODriver *siop);

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  SIO_UNINIT = 0,                   /**< Not initialized.                   */
  SIO_STOP = 1,                     /**< Stopped.                           */
  SIO_READY = 2,                    /**< Ready.                             */
  SIO_ACTIVE = 3                    /**< Operation ongoing.                 */
} siostate_t;

#include "hal_sio_lld.h"

/**
 * @brief   Driver configuration structure.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct hal_sio_config {
  /* End of the mandatory fields.*/
  sio_lld_config_fields;
};

/**
 * @brief   Structure representing a SIO driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct hal_sio_driver {
  /**
   * @brief   Driver state.
   */
  siostate_t               state;
  /**
   * @brief   Current configuration data.
   */
  const SIOConfig          *config;
  /**
   * @brief   Current configuration data.
   */
  const SIOOperation       *operation;
#if (HAL_SIO_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Synchronization point for RX.
   */
  thread_reference_t        sync_rx;
  /**
   * @brief   Synchronization point for TX.
   */
  thread_reference_t        sync_tx;
  /**
   * @brief   Synchronization point for TX-end.
   */
  thread_reference_t        sync_txend;
#endif /* HAL_SIO_USE_SYNCHRONIZATION == TRUE */
#if defined(SIO_DRIVER_EXT_FIELDS)
  SIO_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  sio_lld_driver_fields;
};

/**
 * @brief   Structure representing a SIO operation.
 */
struct hal_sio_operation {
  /**
   * @brief   Receive buffer filled callback.
   * @note    Can be @p NULL.
   */
  siocb_t                   rx_cb;
  /**
   * @brief   End of transmission buffer callback.
   * @note    Can be @p NULL.
   */
  siocb_t                   tx_cb;
  /**
   * @brief   Physical end of transmission callback.
   * @note    Can be @p NULL.
   */
  siocb_t                   txend_cb;
  /**
   * @brief   Receive event callback.
   * @note    Can be @p NULL.
   */
  siocb_t                   rxevt_cb;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the current set of flags and clears it.
 */
#define sioGetFlagsX(siop) sio_lld_get_flags(siop)

/**
 * @brief   Determines the state of the RX FIFO.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX FIFO state.
 * @retval false        if RX FIFO is not empty
 * @retval true         if RX FIFO is empty
 *
 * @xclass
 */
#define sioIsRXEmptyX(siop) sio_lld_is_rx_empty(siop)

/**
 * @brief   Determines the state of the TX FIFO.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The TX FIFO state.
 * @retval false        if TX FIFO is not full
 * @retval true         if TX FIFO is full
 *
 * @xclass
 */
#define sioIsTXFullX(siop) sio_lld_is_tx_full(siop)

/**
 * @brief   Returns one frame from the RX FIFO.
 * @note    If the FIFO is empty then the returned value is unpredictable.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The frame from RX FIFO.
 *
 * @iclass
 */
#define sioGetI(siop) sio_lld_get(siop)

/**
 * @brief   Pushes one frame into the TX FIFO.
 * @note    If the FIFO is full then the behavior is unpredictable.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] data      frame to be written
 *
 * @iclass
 */
#define sioPutI(siop, data) sio_lld_put(siop, data)

/**
 * @brief   Reads data from the RX FIFO.
 * @details This function is non-blocking, data is read if present and the
 *          effective amount is returned.
 * @note    This function can be called from any context but it is meant to
 *          be called from the @p rxne_cb callback handler.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] size      maximum number of frames to read
 * @param[in] buffer    buffer for the received data
 * @return              The number of received frames.
 *
 * @iclass
 */
#define sioAsyncReadI(siop, size, buffer) sio_lld_read(siop, size, buffer)

/**
 * @brief   Writes data into the TX FIFO.
 * @details This function is non-blocking, data is written if there is space
 *          in the FIFO and the effective amount is returned.
 * @note    This function can be called from any context but it is meant to
 *          be called from the @p txnf_cb callback handler.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] size      maximum number of frames to read
 * @param[out] buffer   buffer containing the data to be transmitted
 * @return              The number of transmitted frames.
 *
 * @iclass
 */
#define sioAsyncWriteI(siop, size, buffer) sio_lld_write(siop, size, buffer)

/**
 * @brief   Control operation on a serial port.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] operation control operation code
 * @param[in,out] arg   operation argument
 *
 * @return              The control operation status.
 * @retval MSG_OK       in case of success.
 * @retval MSG_TIMEOUT  in case of operation timeout.
 * @retval MSG_RESET    in case of operation reset.
 *
 * @xclass
 */
#define sioControlX(siop, operation, arg) sio_lld_control(siop, operation, arg)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sioInit(void);
  void sioObjectInit(SIODriver *siop);
  bool sioStart(SIODriver *siop, const SIOConfig *config);
  void sioStop(SIODriver *siop);
  void sioStartOperation(SIODriver *siop, const SIOOperation *operation);
  void sioStopOperation(SIODriver *siop);
  size_t sioAsyncRead(SIODriver *siop, size_t n, uint8_t *buffer);
  size_t sioAsyncWrite(SIODriver *siop, size_t n, const uint8_t *buffer);
#if (HAL_SIO_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
  msg_t sioSynchronizeRX(SIODriver *siop, sysinterval_t timeout);
  msg_t sioSynchronizeTX(SIODriver *siop, sysinterval_t timeout);
  msg_t sioSynchronizeTXEnd(SIODriver *siop, sysinterval_t timeout);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SIO == TRUE */

#endif /* HAL_SIO_H */

/** @} */
