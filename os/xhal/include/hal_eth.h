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
 * @file        hal_eth.h
 * @brief       Generated ETH Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_ETH
 * @brief       ETH Driver macros and structures.
 * @{
 */

#ifndef HAL_ETH_H
#define HAL_ETH_H

#if (HAL_USE_ETH == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Support for thread synchronization API.
 */
#if !defined(ETH_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define ETH_USE_SYNCHRONIZATION             TRUE
#endif

/**
 * @brief       RTOS events on frame sent/received.
 */
#if !defined(ETH_USE_EVENTS) || defined(__DOXYGEN__)
#define ETH_USE_EVENTS                      FALSE
#endif

/**
 * @brief       Support for ETH user configurations.
 * @note        When enabled the user must provide a variable named @p
 *              sio_configurations of type @p sio_configurations_t.
 */
#if !defined(ETH_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define ETH_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on ETH_USE_SYNCHRONIZATION configuration.*/
#if (ETH_USE_SYNCHRONIZATION != FALSE) && (ETH_USE_SYNCHRONIZATION != TRUE)
#error "invalid ETH_USE_SYNCHRONIZATION value"
#endif

/* Checks on ETH_USE_EVENTS configuration.*/
#if (ETH_USE_EVENTS != FALSE) && (ETH_USE_EVENTS != TRUE)
#error "invalid ETH_USE_EVENTS value"
#endif

/* Checks on ETH_USE_CONFIGURATIONS configuration.*/
#if (ETH_USE_CONFIGURATIONS != FALSE) && (ETH_USE_CONFIGURATIONS != TRUE)
#error "invalid ETH_USE_CONFIGURATIONS value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief       Return a pointer to the configuration structure.
 *
 * @param         ip            Pointer to the @p hal_sio_driver_c object.
 * @return                      A pointer to the configuration structure.
 *
 * @notapi
 */
#define __eth_getconf(ip)                                                   \
  ((const hal_eth_config_t *)((ip)->config))

/**
 * @brief       Retrieves a configuration field.
 *
 * @param         ip            Pointer to the @p hal_sio_driver_c object.
 * @param         field         Configuration field to be retrieved.
 * @return                      The field value.
 *
 * @notapi
 */
#define __eth_getfield(ip, field)                                           \
  (__eth_getconf(ip)->field)

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of a receive handle.
 */
typedef void * eth_receive_handle_t;

/**
 * @brief       Type of a transmit handle.
 */
typedef void * eth_transmit_handle_t;

/**
 * @brief       Type of structure representing a ETH driver.
 */
typedef struct hal_eth_driver hal_eth_driver_c;

/**
 * @brief       Type of structure representing a ETH configuration.
 */
typedef struct hal_eth_config hal_eth_config_t;

/* Inclusion of LLD header.*/
#include "hal_eth_lld.h"

/* Checks on the zero-copy capability of the LLD.*/
#if !defined(ETH_SUPPORTS_ZERO_COPY)
#error "ETH_SUPPORTS_ZERO_COPY not defined in LLD"
#endif
#if (ETH_SUPPORTS_ZERO_COPY != FALSE) && (ETH_SUPPORTS_ZERO_COPY != TRUE)
#error "invalid ETH_SUPPORTS_ZERO_COPY value"
#endif

/**
 * @brief       Driver configuration structure.
 * @note        Implementations may extend this structure to contain more,
 *              architecture dependent, fields.
 */
struct hal_eth_config {
  /**
   * @brief       MAC address to be used or @p NULL for a default.
   */
  const uint8_t             *mac_address;
  /* End of the mandatory fields.*/
  eth_lld_config_fields;
#if (defined(ETH_CONFIG_EXT_FIELS)) || defined (__DOXYGEN__)
  ETH_CONFIG_EXT_FIELDS
#endif /* defined(ETH_CONFIG_EXT_FIELS) */
};

/**
 * @brief       Type of user-provided ETH configurations.
 */
typedef struct eth_configurations eth_configurations_t;

/**
 * @brief       Structure representing user-provided ETH configurations.
 */
struct eth_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User ETH configurations.
   */
  hal_eth_config_t          cfgs[];
};

/**
 * @class       hal_eth_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of a ETH driver.
 *
 * @name        Class @p hal_eth_driver_c structures
 * @{
 */

/**
 * @brief       Type of a ETH driver class.
 */
typedef struct hal_eth_driver hal_eth_driver_c;

/**
 * @brief       Class @p hal_eth_driver_c virtual methods table.
 */
struct hal_eth_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*setcb)(void *ip, drv_cb_t cb);
  /* From hal_eth_driver_c.*/
};

/**
 * @brief       Structure representing a ETH driver class.
 */
struct hal_eth_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_eth_driver_vmt *vmt;
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
#if (ETH_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Transmit threads queue.
   */
  threads_queue_t           txqueue;
  /**
   * @brief       Receive threads queue.
   */
  threads_queue_t           rxqueue;
#endif /* ETH_USE_SYNCHRONIZATION == TRUE */
#if (ETH_USE_EVENTS == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       ETH driver events source.
   */
  event_source_t            es;
#endif /* ETH_USE_EVENTS == TRUE */
#if defined(ETH_DRIVER_EXT_FIELS)
  ETH_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  eth_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_eth_driver_c.*/
  void *__eth_objinit_impl(void *ip, const void *vmt);
  void __eth_dispose_impl(void *ip);
  msg_t __eth_start_impl(void *ip);
  void __eth_stop_impl(void *ip);
  const void *__eth_setcfg_impl(void *ip, const void *config);
  const void *__eth_selcfg_impl(void *ip, unsigned cfgnum);
  bool ethPollLinkStatus(void *ip);
#if (ETH_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  eth_receive_handle_t ethWaitReceiveHandle(void *ip, sysinterval_t timeout);
  eth_transmit_handle_t ethWaitTransmitHandle(void *ip, sysinterval_t timeout);
#endif /* ETH_USE_SYNCHRONIZATION == TRUE */
  /* Regular functions.*/
  void ethInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_eth_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_eth_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_eth_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_eth_driver_c *ethObjectInit(hal_eth_driver_c *self) {
  extern const struct hal_eth_driver_vmt __hal_eth_driver_vmt;

  return __eth_objinit_impl(self, &__hal_eth_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_eth_driver_c
 * @{
 */
/**
 * @brief       Queries for a received frame handle.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The receive handle.
 * @retval NULL                 If a received frame is not available.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline eth_receive_handle_t ethGetReceiveHandleI(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_get_receive_handle(self);
}

/**
 * @brief       Queries for a transmit frame handle.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The transmit handle.
 * @retval NULL                 If an empty transmit frame is not available.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline eth_transmit_handle_t ethGetTransmitHandleI(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_get_transmit_handle(self);
}

/**
 * @brief       Releases a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 *
 * @api
 */
CC_FORCE_INLINE
static inline void ethReleaseReceiveHandle(void *ip, eth_receive_handle_t rxh) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  eth_lld_release_receive_handle(self, rxh);
}

/**
 * @brief       Releases and transmits a frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmi] handle.
 *
 * @api
 */
CC_FORCE_INLINE
static inline void ethReleaseTransmitHandle(void *ip,
                                            eth_transmit_handle_t txh) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  eth_lld_release_transmit_handle(self, txh);
}

/**
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    bp            Received data buffer pointer.
 * @param[in]     n             Number of bytes to read.
 * @return                      The number of bytes read from the handle
 *                              buffer, this value can be less than the amount
 *                              specified in the parameter @p size if there are
 *                              no more bytes to read.
 *
 * @api
 */
CC_FORCE_INLINE
static inline size_t ethReadReceiveHandle(void *ip, eth_receive_handle_t rxh,
                                          uint8_t *bp, size_t n) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_read_receive_handle(self, rxh, bp, n);
}

/**
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmit handle.
 * @param[in]     bp            Transmit data buffer pointer.
 * @param[in]     n             Number of bytes to write.
 * @return                      The number of bytes written into the handle
 *                              buffer this value can be less than the amount
 *                              specified in the parameter @p size if the
 *                              maximum frame size is reached.
 *
 * @api
 */
CC_FORCE_INLINE
static inline size_t ethWriteTransmitHandle(void *ip,
                                            eth_transmit_handle_t txh,
                                            const uint8_t *bp, size_t n) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_write_transmit_handle(self, txh, bp, n);
}

#if (ETH_SUPPORTS_ZERO_COPY == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Direct access to the receive handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    sizep         Size of the received frame.
 * @return                      Pointer to the received frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline const uint8_t *ethGetReceiveBufferX(void *ip,
                                                  eth_receive_handle_t rxh,
                                                  size_t *sizep) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_get_receive_buffer(self, rxh, sizep);
}

/**
 * @brief       Direct access to the transmit handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmit handle.
 * @param[out]    sizep         Maximum size of the transmit buffer.
 * @return                      Pointer to the transmit frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline uint8_t *ethGetTransmitBufferX(void *ip,
                                             eth_transmit_handle_t txh,
                                             size_t *sizep) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_get_transmit_buffer(self, txh, sizep);
}
#endif /* ETH_SUPPORTS_ZERO_COPY == TRUE */
/** @} */

#endif /* HAL_USE_ETH == TRUE */

#endif /* HAL_ETH_H */

/** @} */
