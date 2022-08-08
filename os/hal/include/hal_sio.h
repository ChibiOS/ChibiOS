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
 * @name    Events enable flags offsets
 * @{
 */
#define SIO_FL_RXNOTEMPY_POS            1
#define SIO_FL_TXNOTFULL_POS            2
#define SIO_FL_RXIDLE_POS               3
#define SIO_FL_TXDONE_POS               4
#define SIO_FL_ALL_ERRORS_POS           5
/** @} */

/**
 * @name    Events enable flags
 * @{
 */
/**
 * @brief No events enabled.
 */
#define SIO_FL_NONE                     0U
/**
 * @brief   RX buffer not empty event.
 */
#define SIO_FL_RXNOTEMPY                (1U << SIO_FL_RXNOTEMPY_POS)
/**
 * @brief   TX buffer not full event.
 */
#define SIO_FL_TXNOTFULL                (1U << SIO_FL_TXNOTFULL_POS)
/**
 * @brief   All data-related events.
 */
#define SIO_FL_ALL_DATA                 (SIO_FL_RXNOTEMPY       |           \
                                         SIO_FL_TXNOTFULL)
/**
 * @brief RX line idling event.
 */
#define SIO_FL_RXIDLE                   (1U << SIO_FL_RXIDLE_POS)
/**
 * @brief   TX complete event.
 */
#define SIO_FL_TXDONE                   (1U << SIO_FL_TXDONE_POS)
/**
 * @brief   All protocol-related events.
 */
#define SIO_FL_ALL_PROTOCOL             (SIO_FL_RXIDLE          |           \
                                         SIO_FL_TXDONE)
/**
 * @brief   All RX error events.
 */
#define SIO_FL_ALL_ERRORS               (1U << SIO_FL_ALL_ERRORS_POS)
/**
 * @brief   All events.
 */
#define SIO_FL_ALL                      (SIO_FL_ALL_DATA        |           \
                                         SIO_FL_ALL_PROTOCOL    |           \
                                         SIO_FL_ALL_ERRORS)
/** @} */

/**
 * @name    Event flags offsets
 * @{
 */
#define SIO_EV_RXNOTEMPY_POS            2   /* CHN_INPUT_AVAILABLE */
#define SIO_EV_TXNOTFULL_POS            3   /* CHN_OUTPUT_EMPTY */
#define SIO_EV_TXDONE_POS               4   /* CHN_TRANSMISSION_END */
#define SIO_EV_PARITY_ERR_POS           5   /* SD_PARITY_ERROR */
#define SIO_EV_FRAMING_ERR_POS          6   /* SD_FRAMING_ERROR */
#define SIO_EV_OVERRUN_ERR_POS          7   /* SD_OVERRUN_ERROR */
#define SIO_EV_NOISE_ERR_POS            8   /* SD_NOISE_ERROR */
#define SIO_EV_BREAK_POS                9
#define SIO_EV_RXIDLE_POS               11
/** @} */

/**
 * @name    Event flags (compatible with channel and serial events)
 * @{
 */
#define SIO_EV_RXNOTEMPY                (1U << SIO_EV_RXNOTEMPY_POS)
#define SIO_EV_TXNOTFULL                (1U << SIO_EV_TXNOTFULL_POS)
#define SIO_EV_TXDONE                   (1U << SIO_EV_TXDONE_POS)
#define SIO_EV_PARITY_ERR               (1U << SIO_EV_PARITY_ERR_POS)
#define SIO_EV_FRAMING_ERR              (1U << SIO_EV_FRAMING_ERR_POS)
#define SIO_EV_OVERRUN_ERR              (1U << SIO_EV_OVERRUN_ERR_POS)
#define SIO_EV_NOISE_ERR                (1U << SIO_EV_NOISE_ERR_POS)
#define SIO_EV_BREAK                    (1U << SIO_EV_BREAK_POS)
#define SIO_EV_RXIDLE                   (1U << SIO_EV_RXIDLE_POS)
/** @} */

/**
 * @name    Additional messages
 * @{
 */
#define SIO_MSG_ERRORS                      1
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    SIO configuration options
 * @{
 */
/**
 * @brief   Default bit rate.
 * @details Configuration parameter, this is the baud rate selected for the
 *          default configuration.
 */
#if !defined(SIO_DEFAULT_BITRATE) || defined(__DOXYGEN__)
#define SIO_DEFAULT_BITRATE                 38400
#endif

/**
 * @brief   Support for thread synchronization API.
 */
#if !defined(SIO_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define SIO_USE_SYNCHRONIZATION             TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of events enable flags.
 */
typedef uint_least8_t sioflags_t;

/**
 * @brief   Type of event flags.
 */
typedef eventflags_t sioevents_t;

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
 * @brief   @p SIODriver specific methods.
 */
#define _sio_driver_methods                                                 \
  _base_channel_methods

/**
 * @extends BaseChannelVMT
 *
 * @brief   @p SIODriver virtual methods table.
 */
struct sio_driver_vmt {
  _sio_driver_methods
};

/**
 * @brief   Structure representing a SIO driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct hal_sio_driver {
#if (SIO_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Virtual Methods Table.
   */
  const struct sio_driver_vmt *vmt;
#endif
  /**
   * @brief   Driver state.
   */
  siostate_t               state;
  /**
   * @brief   Current configuration data.
   */
  const SIOConfig          *config;
  /**
   * @brief   Enabled conditions flags.
   */
  sioflags_t                enabled;
  /**
   * @brief   Current configuration data.
   */
  const SIOOperation       *operation;
#if (SIO_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
  /**
    * @brief   Synchronization point for RX.
    */
   thread_reference_t        sync_rx;
  /**
   * @brief   Synchronization point for RX idle.
   */
  thread_reference_t        sync_rxidle;
  /**
   * @brief   Synchronization point for TX.
   */
  thread_reference_t        sync_tx;
  /**
   * @brief   Synchronization point for TX-end.
   */
  thread_reference_t        sync_txend;
#endif /* SIO_USE_SYNCHRONIZATION == TRUE */
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
   * @brief   Events callback.
   * @note    Can be @p NULL.
   */
  siocb_t                   cb;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Determines the state of the RX FIFO.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX FIFO state.
 * @retval false        if RX FIFO is not empty.
 * @retval true         if RX FIFO is empty.
 *
 * @xclass
 */
#define sioIsRXEmptyX(siop) sio_lld_is_rx_empty(siop)

/**
 * @brief   Determines the activity state of the receiver.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX activity state.
 * @retval false        if RX is in active state.
 * @retval true         if RX is in idle state.
 *
 * @xclass
 */
#define sioIsRXIdleX(siop) sio_lld_is_rx_idle(siop)

/**
 * @brief   Determines if RX has pending errors to be read and cleared.
 * @note    Only errors are handled, data and idle events are not considered.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX error events.
 * @retval false        if RX has no pending events.
 * @retval true         if RX has pending events.
 *
 * @xclass
 */
#define sioHasRXErrorsX(siop)  sio_lld_has_rx_errors(siop)

/**
 * @brief   Determines the state of the TX FIFO.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The TX FIFO state.
 * @retval false        if TX FIFO is not full.
 * @retval true         if TX FIFO is full.
 *
 * @xclass
 */
#define sioIsTXFullX(siop) sio_lld_is_tx_full(siop)

/**
 * @brief   Determines the transmission state.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The TX FIFO state.
 * @retval false        if transmission is over.
 * @retval true         if transmission is ongoing.
 *
 * @xclass
 */
#define sioIsTXOngoingX(siop) sio_lld_is_tx_ongoing(siop)

/**
 * @brief   Writes the condition flags mask.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] flags     flags mask to be written
 *
 * @iclass
 */
#define sioWriteEnableFlagsI(siop, flags) do {                              \
  (siop)->enabled = (flags);                                                \
  sio_lld_update_enable_flags(siop);                                        \
} while (false)

/**
 * @brief   Enables flags to the condition flags mask.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] flags     flags mask to be enabled
 *
 * @iclass
 */
#define sioSetEnableFlagsI(siop, flags) do {                                \
  (siop)->enabled |= (flags);                                               \
  sio_lld_update_enable_flags(siop);                                        \
} while (false)

/**
 * @brief   Disables flags from the condition flags mask.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] flags     flags mask to be disabled
 *
 * @iclass
 */
#define sioClearEnableFlagsI(siop, flags) do {                              \
  (siop)->enabled &= ~(flags);                                              \
  sio_lld_update_enable_flags(siop);                                        \
} while (false)

/**
 * @brief   Return the enabled condition flags mask.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @xclass
 */
#define sioGetEnableFlagsX(siop) (siop)->eflags

/**
 * @brief   Get and clears SIO error event flags.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The pending error event flags.
 *
 * @iclass
 */
#define sioGetAndClearErrorsI(siop) sio_lld_get_and_clear_errors(siop)

/**
 * @brief   Get and clears SIO event flags.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The pending event flags.
 *
 * @iclass
 */
#define sioGetAndClearEventsI(siop) sio_lld_get_and_clear_events(siop)

/**
 * @brief   Returns one frame from the RX FIFO.
 * @note    If the FIFO is empty then the returned value is unpredictable.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The frame from RX FIFO.
 *
 * @xclass
 */
#define sioGetX(siop) sio_lld_get(siop)

/**
 * @brief   Pushes one frame into the TX FIFO.
 * @note    If the FIFO is full then the behavior is unpredictable.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] data      frame to be written
 *
 * @xclass
 */
#define sioPutX(siop, data) sio_lld_put(siop, data)

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

/**
 * @name    Low level driver helper macros
 * @{
 */
/**
 * @brief   SIO callback.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
#define __sio_callback(siop) do {                                           \
  if ((siop)->operation->cb != NULL) {                                      \
    (siop)->operation->cb(siop);                                            \
  }                                                                         \
} while (false)

#if (SIO_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Wakes up because RX errors.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
#define __sio_wakeup_events(siop) do {                                      \
  osalSysLockFromISR();                                                     \
  osalThreadResumeI(&(siop)->sync_rx, SIO_MSG_ERRORS);                      \
  osalThreadResumeI(&(siop)->sync_rxidle, SIO_MSG_ERRORS);                  \
  osalSysUnlockFromISR();                                                   \
} while (false)

/**
 * @brief   Wakes up the RX-waiting thread.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
#define __sio_wakeup_rx(siop) do {                                          \
  osalSysLockFromISR();                                                     \
  osalThreadResumeI(&(siop)->sync_rx, MSG_OK);                              \
  osalSysUnlockFromISR();                                                   \
} while (false)

/**
 * @brief   Wakes up the RX-idle-waiting thread.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
#define __sio_wakeup_rxidle(siop) do {                                      \
  osalSysLockFromISR();                                                     \
  osalThreadResumeI(&(siop)->sync_rxidle, MSG_OK);                          \
  osalSysUnlockFromISR();                                                   \
} while (false)

/**
 * @brief   Wakes up the TX-waiting thread.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] msg       the wake up message
 *
 * @notapi
 */
#define __sio_wakeup_tx(siop) do {                                          \
  osalSysLockFromISR();                                                     \
  osalThreadResumeI(&(siop)->sync_tx, MSG_OK);                              \
  osalSysUnlockFromISR();                                                   \
} while (false)

/**
 * @brief   Wakes up the TXend-waiting thread.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] msg       the wake up message
 *
 * @notapi
 */
#define __sio_wakeup_txend(siop) do {                                       \
  osalSysLockFromISR();                                                     \
  osalThreadResumeI(&(siop)->sync_txend, MSG_OK);                           \
  osalSysUnlockFromISR();                                                   \
} while (false)
#else /* !SIO_USE_SYNCHRONIZATION */
#define __sio_wakeup_rx(siop, msg)
#define __sio_wakeup_tx(siop, msg)
#define __sio_wakeup_txend(siop, msg)
#endif /* !SIO_USE_SYNCHRONIZATION */
/** @} */

/**
 * @brief   Relocates a bit field.
 *
 * @param[in] v         value
 * @param[in] m         mask of the bit field
 * @param[in] s         source bit offset
 * @param[in] d         destination bit offset
 */
#define __sio_reloc_field(v, m, s, d)   ((((v) & m) >> (s)) << (d))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sioInit(void);
  void sioObjectInit(SIODriver *siop);
  msg_t sioStart(SIODriver *siop, const SIOConfig *config);
  void sioStop(SIODriver *siop);
  void sioStartOperation(SIODriver *siop, const SIOOperation *operation);
  void sioStopOperation(SIODriver *siop);
  void sioWriteEnableFlags(SIODriver *siop, sioflags_t flags);
  void sioSetEnableFlags(SIODriver *siop, sioflags_t flags);
  void sioClearEnableFlags(SIODriver *siop, sioflags_t flags);
  sioevents_t sioGetAndClearErrors(SIODriver *siop) ;
  sioevents_t sioGetAndClearEvents(SIODriver *siop);
  size_t sioAsyncRead(SIODriver *siop, uint8_t *buffer, size_t n);
  size_t sioAsyncWrite(SIODriver *siop, const uint8_t *buffer, size_t n);
#if (SIO_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
  msg_t sioSynchronizeRX(SIODriver *siop, sysinterval_t timeout);
  msg_t sioSynchronizeRXIdle(SIODriver *siop, sysinterval_t timeout);
  msg_t sioSynchronizeTX(SIODriver *siop, sysinterval_t timeout);
  msg_t sioSynchronizeTXEnd(SIODriver *siop, sysinterval_t timeout);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SIO == TRUE */

#endif /* HAL_SIO_H */

/** @} */
