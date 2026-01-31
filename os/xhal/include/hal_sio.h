/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file        hal_sio.h
 * @brief       Generated SIO Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SIO
 * @brief       SIO Driver macros and structures.
 * @{
 */

#ifndef HAL_SIO_H
#define HAL_SIO_H

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    SIO events
 * @{
 */
#define SIO_EV_NONE                         CHN_FL_NONE
#define SIO_EV_PARITY_ERR_POS               CHN_FL_PARITY_ERR_POS
#define SIO_EV_PARITY_ERR                   CHN_FL_PARITY_ERR
#define SIO_EV_FRAMING_ERR_POS              CHN_FL_FRAMING_ERR_POS
#define SIO_EV_FRAMING_ERR                  CHN_FL_FRAMING_ERR
#define SIO_EV_NOISE_ERR_POS                CHN_FL_NOISE_ERR_POS
#define SIO_EV_NOISE_ERR                    CHN_FL_NOISE_ERR
#define SIO_EV_OVERRUN_ERR_POS              CHN_FL_OVERRUN_ERR_POS
#define SIO_EV_OVERRUN_ERR                  CHN_FL_OVERRUN_ERR
#define SIO_EV_ALL_ERRORS_POS               SIO_EV_PARITY_ERR_POS
#define SIO_EV_ALL_ERRORS                   (15U << SIO_EV_ALL_ERRORS_POS)
#define SIO_EV_TX_NOTFULL_POS               CHN_FL_TX_NOTFULL_POS
#define SIO_EV_TX_NOTFULL                   CHN_FL_TX_NOTFULL
#define SIO_EV_RX_NOTEMPTY_POS              CHN_FL_RX_NOTEMPTY_POS
#define SIO_EV_RX_NOTEMPTY                  CHN_FL_RX_NOTEMPTY
#define SIO_EV_ALL_DATA_POS                 SIO_EV_TX_NOTFULL_POS
#define SIO_EV_ALL_DATA                     (3U << SIO_EV_ALL_DATA_POS)
#define SIO_EV_TX_END_POS                   CHN_FL_TX_END_POS
#define SIO_EV_TX_END                       CHN_FL_TX_END
#define SIO_EV_RX_IDLE_POS                  CHN_FL_RX_IDLE_POS
#define SIO_EV_RX_IDLE                      CHN_FL_RX_IDLE
#define SIO_EV_RX_BREAK_POS                 CHN_FL_RX_BREAK_POS
#define SIO_EV_RX_BREAK                     CHN_FL_RX_BREAK
#define SIO_EV_ALL_STATUS_POS               SIO_EV_TX_END_POS
#define SIO_EV_ALL_STATUS                   (7U << SIO_EV_ALL_STATUS_POS)
#define SIO_EV_NONE_MASK                    0U
#define SIO_EV_ALL_EVENTS                   (SIO_EV_ALL_ERRORS | SIO_EV_ALL_DATA | SIO_EV_ALL_STATUS)
/** @} */

/**
 * @name    Additional messages
 * @{
 */
#define SIO_MSG_ERRORS                      1
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Default bit rate.
 * @details     Configuration parameter, this is the baud rate selected for the
 *              default configuration.
 */
#if !defined(SIO_DEFAULT_BITRATE) || defined(__DOXYGEN__)
#define SIO_DEFAULT_BITRATE                 38400
#endif

/**
 * @brief       Support for thread synchronization API.
 */
#if !defined(SIO_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define SIO_USE_SYNCHRONIZATION             TRUE
#endif

/**
 * @brief       Support for streams interface.
 */
#if !defined(SIO_USE_STREAMS_INTERFACE) || defined(__DOXYGEN__)
#define SIO_USE_STREAMS_INTERFACE           SIO_USE_SYNCHRONIZATION
#endif

/**
 * @brief       Support for buffering wrapper class.
 */
#if !defined(SIO_USE_BUFFERING) || defined(__DOXYGEN__)
#define SIO_USE_BUFFERING                   TRUE
#endif

/**
 * @brief       Support for SIO user configurations.
 * @note        When enabled the user must provide a variable named @p
 *              sio_configurations of type @p sio_configurations_t.
 */
#if !defined(SIO_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define SIO_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on SIO_DEFAULT_BITRATE configuration.*/
#if SIO_DEFAULT_BITRATE <= 0
#error "invalid SIO_DEFAULT_BITRATE value"
#endif

/* Checks on SIO_USE_SYNCHRONIZATION configuration.*/
#if (SIO_USE_SYNCHRONIZATION != FALSE) && (SIO_USE_SYNCHRONIZATION != TRUE)
#error "invalid SIO_USE_SYNCHRONIZATION value"
#endif

/* Checks on SIO_USE_STREAMS_INTERFACE configuration.*/
#if (SIO_USE_STREAMS_INTERFACE != FALSE) && (SIO_USE_STREAMS_INTERFACE != TRUE)
#error "invalid SIO_USE_STREAMS_INTERFACE value"
#endif

/* Checks on SIO_USE_BUFFERING configuration.*/
#if (SIO_USE_BUFFERING != FALSE) && (SIO_USE_BUFFERING != TRUE)
#error "invalid SIO_USE_BUFFERING value"
#endif

/* Checks on SIO_USE_CONFIGURATIONS configuration.*/
#if (SIO_USE_CONFIGURATIONS != FALSE) && (SIO_USE_CONFIGURATIONS != TRUE)
#error "invalid SIO_USE_CONFIGURATIONS value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief       Determines the state of the RX FIFO.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The RX FIFO state.
 *
 * @xclass
 */
#define sioIsRXEmptyX(siop)                                                 \
  sio_lld_is_rx_empty(siop)

/**
 * @brief       Determines the activity state of the receiver.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The RX activity state.
 *
 * @xclass
 */
#define sioIsRXIdleX(siop)                                                  \
  sio_lld_is_rx_idle(siop)

/**
 * @brief       Determines if RX has pending errors to be read and cleared.
 * @note        Only error flags are handled, data and idle flags are not
 *              considered.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The RX error events.
 *
 * @xclass
 */
#define sioHasRXErrorsX(siop)                                               \
  sio_lld_has_rx_errors(siop)

/**
 * @brief       Determines the state of the TX FIFO.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The TX FIFO state.
 *
 * @xclass
 */
#define sioIsTXFullX(siop)                                                  \
  sio_lld_is_tx_full(siop)

/**
 * @brief       Determines the transmission state.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The transmission state.
 *
 * @xclass
 */
#define sioIsTXOngoingX(siop)                                               \
  sio_lld_is_tx_ongoing(siop)

/**
 * @brief       Writes the enabled events mask.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @param[in]     mask          Enabled events mask to be written.
 *
 * @xclass
 */
#define sioWriteEnableFlagsX(siop, mask)                                    \
  do {                                                                      \
    (siop)->enabled = (mask);                                               \
    sio_lld_update_enable_flags(siop);                                      \
  } while (false)

/**
 * @brief       Sets flags into the enabled events flags mask.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @param[in]     mask          Enabled events mask to be set.
 *
 * @xclass
 */
#define sioSetEnableFlagsX(siop, mask)                                      \
  do {                                                                      \
    (siop)->enabled |= (mask);                                              \
    sio_lld_update_enable_flags(siop);                                      \
  } while (false)

/**
 * @brief       Clears flags from the enabled events flags mask.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @param[in]     mask          Enabled events mask to be cleared.
 *
 * @xclass
 */
#define sioClearEnableFlagsX(siop, mask)                                    \
  do {                                                                      \
    (siop)->enabled &= ~(mask);                                             \
    sio_lld_update_enable_flags(siop);                                      \
  } while (false)

/**
 * @brief       Gets the enabled condition flags mask.
 *
 * @param[in]     siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The enabled event flags.
 *
 * @xclass
 */
#define sioGetEnableFlagsX(siop)                                            \
  (siop)->enabled

/**
 * @brief       Gets and clears SIO error flags.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The pending error flags.
 *
 * @xclass
 */
#define sioGetAndClearErrorsX(siop)                                         \
  sio_lld_get_and_clear_errors(siop)

/**
 * @brief       Gets and clears SIO event flags.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @param[in]     mask          Mask of events to be returned and cleared.
 * @return                      The pending event flags.
 *
 * @xclass
 */
#define sioGetAndClearEventsX(siop, mask)                                   \
  sio_lld_get_and_clear_events(siop, mask)

/**
 * @brief       Returns all SIO event flags.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The pending event flags.
 *
 * @xclass
 */
#define sioGetEventsX(siop)                                                 \
  sio_lld_get_events(siop)

/**
 * @brief       Returns one frame from the RX FIFO.
 * @note        If the FIFO is empty then the returned value is unpredictable.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The frame from RX FIFO.
 *
 * @xclass
 */
#define sioGetX(siop)                                                       \
  sio_lld_get(siop)

/**
 * @brief       Pushes one frame into the TX FIFO.
 * @note        If the FIFO is full then the behavior is unpredictable.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @param[in]     data          Frame to be written.
 *
 * @xclass
 */
#define sioPutX(siop, data)                                                 \
  sio_lld_put(siop, data)

/**
 * @brief       Reads data from the RX FIFO.
 * @details     This function is non-blocking, data is read if present and the
 *              effective amount is returned.
 * @note        This function can be called from any context but it is meant to
 *              be called from the @p cb callback handler.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @param[out]    buffer        Buffer for the received data.
 * @param[in]     n             Maximum number of frames to read.
 * @return                      The number of received frames.
 * @retval 0                    RX FIFO is empty.
 *
 * @xclass
 */
#define sioAsyncReadX(siop, buffer, n)                                      \
  sio_lld_read(siop, buffer, n)

/**
 * @brief       Writes data into the TX FIFO.
 * @details     This function is non-blocking, data is written if there is
 *              space in the FIFO and the effective amount is returned.
 * @note        This function can be called from any context but it is meant to
 *              be called from the @p cb callback handler.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @param[in]     buffer        Buffer containing the data to be transmitted
 * @param[in]     n             Maximum number of frames to read.
 * @return                      The number of transmitted frames.
 * @retval 0                    TX FIFO is full.
 *
 * @xclass
 */
#define sioAsyncWriteX(siop, buffer, n)                                     \
  sio_lld_write(siop, buffer, n)

/**
 * @brief       Control operation on a serial port.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @param[in]     operation     Control operation code.
 * @param[in,out] arg           Operation argument.
 * @return                      The control operation status.
 *
 * @xclass
 */
#define sioControlX(siop, operation, arg)                                   \
  sio_lld_control(siop, operation, arg)

/**
 * @name    Low level driver helper macros
 * @{
 */
/**
 * @brief       SIO callback.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 * @return                      The control operation status.
 *
 * @notapi
 */
#define __sio_callback(siop)                                                \
  do {                                                                      \
    if ((siop)->cb != NULL) {                                               \
      (siop)->cb(siop);                                                     \
    }                                                                       \
  } while (false)

#if (SIO_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Wakes up because RX errors.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 *
 * @notapi
 */
#define __sio_wakeup_errors(siop)                                           \
  do {                                                                      \
    osalSysLockFromISR();                                                   \
    osalThreadResumeI(&(siop)->sync_rx, SIO_MSG_ERRORS);                    \
    osalThreadResumeI(&(siop)->sync_rxidle, SIO_MSG_ERRORS);                \
    osalSysUnlockFromISR();                                                 \
  } while (false)

/**
 * @brief       Wakes up the RX-waiting thread.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 *
 * @notapi
 */
#define __sio_wakeup_rx(siop)                                               \
  do {                                                                      \
    osalSysLockFromISR();                                                   \
    osalThreadResumeI(&(siop)->sync_rx, MSG_OK);                            \
    osalSysUnlockFromISR();                                                 \
  } while (false)

/**
 * @brief       Wakes up the RX-idle-waiting thread.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 *
 * @notapi
 */
#define __sio_wakeup_rxidle(siop)                                           \
  do {                                                                      \
    osalSysLockFromISR();                                                   \
    osalThreadResumeI(&(siop)->sync_rxidle, MSG_OK);                        \
    osalSysUnlockFromISR();                                                 \
  } while (false)

/**
 * @brief       Wakes up the TX-waiting thread.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 *
 * @notapi
 */
#define __sio_wakeup_tx(siop)                                               \
  do {                                                                      \
    osalSysLockFromISR();                                                   \
    osalThreadResumeI(&(siop)->sync_tx, MSG_OK);                            \
    osalSysUnlockFromISR();                                                 \
  } while (false)

/**
 * @brief       Wakes up the TXend-waiting thread.
 *
 * @param[in,out] siop          Pointer to the @p hal_sio_driver_c object
 *
 * @notapi
 */
#define __sio_wakeup_txend(siop)                                            \
  do {                                                                      \
    osalSysLockFromISR();                                                   \
    osalThreadResumeI(&(siop)->sync_txend, MSG_OK);                         \
    osalSysUnlockFromISR();                                                 \
  } while (false)

#else
#define __sio_wakeup_errors(siop)
#define __sio_wakeup_rx(siop)
#define __sio_wakeup_rxidle(siop)
#define __sio_wakeup_tx(siop)
#define __sio_wakeup_txend(siop)
#endif /* SIO_USE_SYNCHRONIZATION == TRUE */
/** @} */

/**
 * @brief       Relocates a bit field.
 *
 * @param[in]     v             Value.
 * @param[in]     m             Mask of the bit field.
 * @param[in]     s             Source bit offset.
 * @param[in]     d             Destination bit offset.
 */
#define __sio_reloc_field(v, m, s, d)                                       \
  ((((v) & m) >> (s)) << (d))

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of SIO event flags.
 */
typedef chnflags_t sioevents_t;

/**
 * @brief       Type of structure representing a SIO driver.
 */
typedef struct hal_sio_driver hal_sio_driver_c;

/**
 * @brief       Type of structure representing a SIO configuration.
 */
typedef struct hal_sio_config hal_sio_config_t;

/**
 * @brief       Type of structure representing a SIO configuration (legacy).
 */
typedef struct hal_sio_config SIOConfig;

/**
 * @brief       Type of structure representing a SIO driver (legacy).
 */
typedef struct hal_sio_driver SIODriver;

/* Inclusion of LLD header.*/
#include "hal_sio_lld.h"

/**
 * @brief       Driver configuration structure.
 * @note        Implementations may extend this structure to contain more,
 *              architecture dependent, fields.
 */
struct hal_sio_config {
  /* End of the mandatory fields.*/
  sio_lld_config_fields;
#if (defined(SIO_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  SIO_CONFIG_EXT_FIELDS
#endif /* defined(SIO_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Type of user-provided SIO configurations.
 */
typedef struct sio_configurations sio_configurations_t;

/**
 * @brief       Structure representing user-provided SIO configurations.
 */
struct sio_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User SIO configurations.
   */
  hal_sio_config_t          cfgs[];
};

/**
 * @class       hal_sio_driver_c
 * @extends     hal_cb_driver_c
 * @implements  asynchronous_channel_i
 *
 * @brief       Class of a SIO (Serial I/O) driver.
 *
 * @name        Class @p hal_sio_driver_c structures
 * @{
 */

/**
 * @brief       Type of a SIO driver class.
 */
typedef struct hal_sio_driver hal_sio_driver_c;

/**
 * @brief       Class @p hal_sio_driver_c virtual methods table.
 */
struct hal_sio_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*setcb)(void *ip, drv_cb_t cb);
  /* From hal_sio_driver_c.*/
};

/**
 * @brief       Structure representing a SIO driver class.
 */
struct hal_sio_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_sio_driver_vmt *vmt;
  /**
   * @brief       Driver state.
   */
  driver_state_t            state;
  /**
   * @brief       Associated configuration structure.
   */
  const void                *config;
  /**
   * @brief       Driver argument.
   */
  void                      *arg;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver mutex.
   */
  mutex_t                   mutex;
#endif /* HAL_USE_MUTUAL_EXCLUSION == TRUE */
#if (HAL_USE_REGISTRY == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver identifier.
   */
  unsigned int              id;
  /**
   * @brief       Driver name.
   */
  const char                *name;
  /**
   * @brief       Registry link structure.
   */
  hal_regent_t              regent;
#endif /* HAL_USE_REGISTRY == TRUE */
  /**
   * @brief       Driver callback.
   * @note        Can be @p NULL.
   */
  drv_cb_t                  cb;
#if (SIO_USE_STREAMS_INTERFACE == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Implemented interface @p asynchronous_channel_i.
   */
  asynchronous_channel_i    chn;
#endif /* SIO_USE_STREAMS_INTERFACE == TRUE */
  /**
   * @brief       Enabled event flags.
   */
  sioevents_t               enabled;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Synchronization point for RX.
   */
  thread_reference_t        sync_rx;
  /**
   * @brief       Synchronization point for RX idle.
   */
  thread_reference_t        sync_rxidle;
  /**
   * @brief       Synchronization point for TX.
   */
  thread_reference_t        sync_tx;
  /**
   * @brief       Synchronization point for TX-end.
   */
  thread_reference_t        sync_txend;
#endif /* HAL_USE_MUTUAL_EXCLUSION == TRUE */
#if defined(SIO_DRIVER_EXT_FIELDS)
  SIO_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  sio_lld_driver_fields;
};
/** @} */

/**
 * @class       hal_buffered_sio_c
 * @extends     hal_buffered_serial_c
 *
 * @brief       This class implements a buffered channel interface on top of
 *              SIO.
 *
 * @name        Class @p hal_buffered_sio_c structures
 * @{
 */

/**
 * @brief       Type of a buffered SIO wrapper class.
 */
typedef struct hal_buffered_sio hal_buffered_sio_c;

/**
 * @brief       Class @p hal_buffered_sio_c virtual methods table.
 */
struct hal_buffered_sio_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_buffered_serial_c.*/
  /* From hal_buffered_sio_c.*/
};

/**
 * @brief       Structure representing a buffered SIO wrapper class.
 */
struct hal_buffered_sio {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_buffered_sio_vmt *vmt;
  /**
   * @brief       Driver state.
   */
  driver_state_t            state;
  /**
   * @brief       Associated configuration structure.
   */
  const void                *config;
  /**
   * @brief       Driver argument.
   */
  void                      *arg;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver mutex.
   */
  mutex_t                   mutex;
#endif /* HAL_USE_MUTUAL_EXCLUSION == TRUE */
#if (HAL_USE_REGISTRY == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver identifier.
   */
  unsigned int              id;
  /**
   * @brief       Driver name.
   */
  const char                *name;
  /**
   * @brief       Registry link structure.
   */
  hal_regent_t              regent;
#endif /* HAL_USE_REGISTRY == TRUE */
  /**
   * @brief       Implemented interface @p asynchronous_channel_i.
   */
  asynchronous_channel_i    chn;
  /**
   * @brief       Input queue.
   */
  input_queue_t             iqueue;
  /**
   * @brief       Output queue.
   */
  output_queue_t            oqueue;
  /**
   * @brief       I/O condition event source.
   */
  event_source_t            event;
  /**
   * @brief       Pointer to the associated @p hal_sio_driver_c instance.
   */
  hal_sio_driver_c          *siop;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_sio_driver_c.*/
  void *__sio_objinit_impl(void *ip, const void *vmt);
  void __sio_dispose_impl(void *ip);
  msg_t __sio_start_impl(void *ip);
  void __sio_stop_impl(void *ip);
  const void *__sio_setcfg_impl(void *ip, const void *config);
  const void *__sio_selcfg_impl(void *ip, unsigned cfgnum);
  void sioWriteEnableFlags(void *ip, sioevents_t mask);
  void sioSetEnableFlags(void *ip, sioevents_t mask);
  void sioClearEnableFlags(void *ip, sioevents_t mask);
  sioevents_t sioGetAndClearErrors(void *ip);
  sioevents_t sioGetAndClearEvents(void *ip, sioevents_t mask);
  sioevents_t sioGetEvents(void *ip);
#if (SIO_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  msg_t sioSynchronizeRX(void *ip, sysinterval_t timeout);
  msg_t sioSynchronizeRXIdle(void *ip, sysinterval_t timeout);
  msg_t sioSynchronizeTX(void *ip, sysinterval_t timeout);
  msg_t sioSynchronizeTXEnd(void *ip, sysinterval_t timeout);
#endif /* SIO_USE_SYNCHRONIZATION == TRUE */
  /* Methods of hal_buffered_sio_c.*/
  void *__bsio_objinit_impl(void *ip, const void *vmt, hal_sio_driver_c *siop,
                            uint8_t *ib, size_t ibsize, uint8_t *ob,
                            size_t obsize);
  void __bsio_dispose_impl(void *ip);
  msg_t __bsio_start_impl(void *ip);
  void __bsio_stop_impl(void *ip);
  const void *__bsio_setcfg_impl(void *ip, const void *config);
  /* Regular functions.*/
  void sioInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_sio_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_sio_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_sio_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_sio_driver_c *sioObjectInit(hal_sio_driver_c *self) {
  extern const struct hal_sio_driver_vmt __hal_sio_driver_vmt;

  return __sio_objinit_impl(self, &__hal_sio_driver_vmt);
}
/** @} */

/**
 * @name        Default constructor of hal_buffered_sio_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_buffered_sio_c.
 *
 * @param[out]    self          Pointer to a @p hal_buffered_sio_c instance to
 *                              be initialized.
 * @param[in]     siop          Pointer to the @p hal_sio_driver_c object.
 * @param[in]     ib            Pointer to the input buffer.
 * @param[in]     ibsize        Size of the input buffer.
 * @param[in]     ob            Pointer to the output buffer.
 * @param[in]     obsize        Size of the output buffer.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_buffered_sio_c *bsioObjectInit(hal_buffered_sio_c *self,
                                                 hal_sio_driver_c *siop,
                                                 uint8_t *ib, size_t ibsize,
                                                 uint8_t *ob, size_t obsize) {
  extern const struct hal_buffered_sio_vmt __hal_buffered_sio_vmt;

  return __bsio_objinit_impl(self, &__hal_buffered_sio_vmt, siop, ib, ibsize,
                             ob, obsize);
}
/** @} */

#endif /* HAL_USE_SIO == TRUE */

#endif /* HAL_SIO_H */

/** @} */
