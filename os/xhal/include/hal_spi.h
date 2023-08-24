/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

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
 * @file        hal_spi.h
 * @brief       Generated SPI Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SPI
 * @brief       SPI Driver macros and structures.
 * @{
 */

#ifndef HAL_SPI_H
#define HAL_SPI_H

#if (HAL_USE_SPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    SPI operation modes
 * @{
 */
/**
 * @brief       Memory buffers frame size bit position.
 */
#define SPI_MODE_FSIZE_POS                  0U

/**
 * @brief       Memory buffers frame size mask.
 */
#define SPI_MODE_FSIZE_MASK                 (3U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Memory frame size is 8 bits.
 */
#define SPI_MODE_FSIZE_8                    (0U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Memory frame size is 16 bits.
 */
#define SPI_MODE_FSIZE_16                   (1U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Memory frame size is 32 bits.
 */
#define SPI_MODE_FSIZE_32                   (2U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Memory frame size is 64 bits.
 */
#define SPI_MODE_FSIZE_64                   (3U << SPI_MODE_FSIZE_POS)

/**
 * @brief       Enables the circular buffer mode.
 */
#define SPI_MODE_CIRCULAR                   (1U << 2)

/**
 * @brief       Enables the slave mode.
 */
#define SPI_MODE_SLAVE                      (1U << 3)
/** @} */

/**
 * @name    SPI CS modes
 * @{
 */
/**
 * @brief       @p spiSelect() and @p spiUnselect() do nothing.
 */
#define SPI_SELECT_MODE_NONE                0

/**
 * @brief       Selection by PAL port and pad number.
 */
#define SPI_SELECT_MODE_PAD                 1

/**
 * @brief       Selection by port and port mask.
 */
#define SPI_SELECT_MODE_PORT                2

/**
 * @brief       Selection by PAL line identifier.
 */
#define SPI_SELECT_MODE_LINE                3

/**
 * @brief       Selection by LLD-defined mode.
 */
#define SPI_SELECT_MODE_LLD                 4
/** @} */

/**
 * @name    SPI status flags
 * @{
 */
/**
 * @brief       Last transfer failed because HW error.
 */
#define SPI_STS_FAILED                      1U
/** @} */

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
#if !defined(SPI_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define SPI_USE_SYNCHRONIZATION             TRUE
#endif

/**
 * @brief       Handling method for SPI CS line.
 */
#if !defined(SPI_SELECT_MODE) || defined(__DOXYGEN__)
#define SPI_SELECT_MODE                     SPI_SELECT_MODE_PAD
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on SPI_USE_SYNCHRONIZATION configuration.*/
#if (SPI_USE_SYNCHRONIZATION != FALSE) && (SPI_USE_SYNCHRONIZATION != TRUE)
#error "invalid SPI_USE_SYNCHRONIZATION value"
#endif

/* Checks on SPI_SELECT_MODE configuration.*/
#if (SPI_SELECT_MODE < SPI_SELECT_MODE_NONE) || (SPI_SELECT_MODE > SPI_SELECT_MODE_LLD)
#error "invalid SPI_SELECT_MODE value"
#endif

/* Some modes have a dependency on the PAL driver, making the required
   checks here.*/
#if ((SPI_SELECT_MODE != SPI_SELECT_MODE_PAD)  ||                           \
     (SPI_SELECT_MODE != SPI_SELECT_MODE_PORT) ||                           \
     (SPI_SELECT_MODE != SPI_SELECT_MODE_LINE)) &&                          \
    (HAL_USE_PAL != TRUE)
#error "current SPI_SELECT_MODE requires HAL_USE_PAL"
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
#define __spi_getconf(ip)                                                   \
  ((const hal_spi_config_t *)((ip)->config))

/**
 * @brief       Retrieves a configuration field.
 *
 * @param         ip            Pointer to the @p hal_sio_driver_c object.
 * @param         field         Configuration field to be retrieved.
 * @return                      The field value.
 *
 * @notapi
 */
#define __spi_getfield(ip, field)                                           \
  (__spi_getconf(ip)->field)

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of SPI transfer mode options.
 */
typedef uint_fast16_t spi_mode_t;

/**
 * @brief       Type of structure representing a SPI driver.
 */
typedef struct hal_spi_driver hal_spi_driver_c;

/**
 * @brief       Type of structure representing a SPI configuration.
 */
typedef struct hal_spi_config hal_spi_config_t;

/**
 * @brief       Type of structure representing a SPI configuration (legacy).
 */
typedef struct hal_spi_config SPIConfig;

/**
 * @brief       Type of structure representing a SPI driver (legacy).
 */
typedef struct hal_spi_driver SPIDriver;

/* Inclusion of LLD header.*/
#include "hal_spi_lld.h"

#if !defined(SPI_SUPPORTS_CIRCULAR)
#error "SPI_SUPPORTS_CIRCULAR not defined in SPI LLD driver"
#endif

#if !defined(SPI_SUPPORTS_SLAVE_MODE)
#error "SPI_SUPPORTS_SLAVE_MODE not defined in SPI LLD driver"
#endif

/**
 * @brief       Driver configuration structure.
 * @note        Implementations may extend this structure to contain more,
 *              architecture dependent, fields.
 */
struct hal_spi_config {
  /**
   * @brief       SPI transfer mode options.
   */
  spi_mode_t                mode;
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_LINE) || defined (__DOXYGEN__)
  /**
   * @brief       The chip select line.
   * @note        Only used in master mode.
   */
  ioline_t                  ssline;
#endif /* SPI_SELECT_MODE == SPI_SELECT_MODE_LINE */
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_PORT) || defined (__DOXYGEN__)
  /**
   * @brief       The chip select port.
   * @note        Only used in master mode.
   */
  ioportid_t                ssport;
  /**
   * @brief       The chip select port mask.
   * @note        Only used in master mode.
   */
  ioportmask_t              ssmask;
#endif /* SPI_SELECT_MODE == SPI_SELECT_MODE_PORT */
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_PAD) || defined (__DOXYGEN__)
  /**
   * @brief       The chip select port.
   * @note        Only used in master mode.
   */
  ioportid_t                ssport;
  /**
   * @brief       The chip select pad number.
   * @note        Only used in master mode.
   */
  ioportmask_t              sspad;
#endif /* SPI_SELECT_MODE == SPI_SELECT_MODE_PAD */
  /* End of the mandatory fields.*/
  spi_lld_config_fields;
#if (defined(SPI_CONFIG_EXT_FIELS)) || defined (__DOXYGEN__)
  SPI_CONFIG_EXT_FIELDS
#endif /* defined(SPI_CONFIG_EXT_FIELS) */
};

/**
 * @class       hal_spi_driver_c
 * @extends     base_object_c, hal_base_driver_c, hal_cb_driver_c.
 *
 * @brief       Class of a SPI driver.
 *
 * @name        Class @p hal_spi_driver_c structures
 * @{
 */

/**
 * @brief       Type of a SPI driver class.
 */
typedef struct hal_spi_driver hal_spi_driver_c;

/**
 * @brief       Class @p hal_spi_driver_c virtual methods table.
 */
struct hal_spi_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  const void * (*doconf)(void *ip, const void *config);
  /* From hal_cb_driver_c.*/
  void (*setcb)(void *ip, drv_cb_t cb);
  drv_status_t (*gsts)(void *ip);
  drv_status_t (*gcsts)(void *ip, drv_status_t mask);
  /* From hal_spi_driver_c.*/
};

/**
 * @brief       Structure representing a SPI driver class.
 */
struct hal_spi_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_spi_driver_vmt *vmt;
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
#if (SPI_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Synchronization point for transfer.
   */
  thread_reference_t        sync_transfer;
#endif /* SPI_USE_SYNCHRONIZATION == TRUE */
#if defined(SPI_DRIVER_EXT_FIELS)
  SPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  spi_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_spi_driver_c.*/
  void *__spi_objinit_impl(void *ip, const void *vmt);
  void __spi_dispose_impl(void *ip);
  msg_t __spi_start_impl(void *ip);
  void __spi_stop_impl(void *ip);
  const void *__spi_doconf_impl(void *ip, const void *config);
  drv_status_t __spi_gsts_impl(void *ip);
  drv_status_t __spi_gcsts_impl(void *ip, drv_status_t mask);
  msg_t spiStartIgnoreI(void *ip, size_t n);
  msg_t spiStartIgnore(void *ip, size_t n);
  msg_t spiStartExchangeI(void *ip, size_t n, const void *txbuf, void *rxbuf);
  msg_t spiStartExchange(void *ip, size_t n, const void *txbuf, void *rxbuf);
  msg_t spiStartSendI(void *ip, size_t n, const void *txbuf);
  msg_t spiStartSend(void *ip, size_t n, const void *txbuf);
  msg_t spiStartReceiveI(void *ip, size_t n, void *rxbuf);
  msg_t spiStartReceive(void *ip, size_t n, void *rxbuf);
  msg_t spiStopTransferI(void *ip, size_t *np);
  msg_t spiStopTransfer(void *ip, size_t *np);
#if (SPI_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  msg_t spiSynchronizeS(void *ip, sysinterval_t timeout);
  msg_t spiSynchronize(void *ip, sysinterval_t timeout);
  msg_t spiIgnore(void *ip, size_t n);
  msg_t spiExchange(void *ip, size_t n, const void *txbuf, void *rxbuf);
  msg_t spiSend(void *ip, size_t n, const void *txbuf);
  msg_t spiReceive(void *ip, size_t n, void *rxbuf);
#endif /* SPI_USE_SYNCHRONIZATION == TRUE */
  /* Regular functions.*/
  void spiInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_spi_driver_c
 * @{
 */
/**
 * @memberof    hal_spi_driver_c
 *
 * @brief       Default initialization function of @p hal_spi_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_spi_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_spi_driver_c *spiObjectInit(hal_spi_driver_c *self) {
  extern const struct hal_spi_driver_vmt __hal_spi_driver_vmt;

  return __spi_objinit_impl(self, &__hal_spi_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_spi_driver_c
 * @{
 */
/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Returns the configured size, in bytes, of data frames.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 */
CC_FORCE_INLINE
static inline size_t spiGetFrameSizeX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  return (size_t)(1U << ((__spi_getfield(self, mode) & SPI_MODE_FSIZE_MASK) >> SPI_MODE_FSIZE_POS));
}

#if (SPI_SELECT_MODE == SPI_SELECT_MODE_LLD) || defined (__DOXYGEN__)
/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Asserts the slave select signal and prepares for transfers.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void spiSelectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  spi_lld_select(self);
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Deasserts the slave select signal.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void spiUnselectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  spi_lld_unselect(self);
}

#elif SPI_SELECT_MODE == SPI_SELECT_MODE_LINE
CC_FORCE_INLINE
static inline void spiSelectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  palClearLine(__spi_getfield(self, ssline));
}

CC_FORCE_INLINE
static inline void spiUnselectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  palSetLine(__spi_getfield(self, ssline));
}

#elif SPI_SELECT_MODE == SPI_SELECT_MODE_PORT
CC_FORCE_INLINE
static inline void spiSelectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  palClearPort(__spi_getfield(self, ssport), __spi_getfield(self, ssmask));
}

CC_FORCE_INLINE
static inline void spiUnselectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  palSetPort(__spi_getfield(self, ssport), __spi_getfield(self, ssmask));
}

#elif SPI_SELECT_MODE == SPI_SELECT_MODE_PAD
CC_FORCE_INLINE
static inline void spiSelectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  palClearPad(__spi_getfield(self, ssport), __spi_getfield(self, sspad));
}

CC_FORCE_INLINE
static inline void spiUnselectX(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  palSetPad(__spi_getfield(self, ssport), __spi_getfield(self, sspad));
}
#endif /* SPI_SELECT_MODE == SPI_SELECT_MODE_LLD */

#if (SPI_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Wakes up the waiting thread.
 * @note        This function is meant to be used in the low level drivers
 *              implementations only.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     msg           The wakeup message.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __spi_wakeup_isr(void *ip, msg_t msg) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  osalSysLockFromISR();
  osalThreadResumeI(&self->sync_transfer, msg);
  osalSysUnlockFromISR();
}

#else

CC_FORCE_INLINE
static inline void __spi_wakeup_isr(void *ip, msg_t msg) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  (void)self;
  (void)msg;
}
#endif /* SPI_USE_SYNCHRONIZATION == TRUE */

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Common ISR code in linear mode.
 *              This code handles the portable part of the ISR code:
 *                - Callback invocation.
 *                - Waiting thread wakeup, if any.
 *                - Driver state transitions.
 *                .
 * @note        This function is meant to be used in the low level drivers
 *              implementations only.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __spi_isr_complete_code(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  __cbdrv_invoke_cb_with_transition(self,
                                    HAL_DRV_STATE_COMPLETE,
                                    HAL_DRV_STATE_READY);
  __spi_wakeup_isr(self, MSG_OK);
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Half buffer filled ISR code in circular mode.
 *              The callback is invoked with driver
 *                             state set to @p HAL_DRV_STATE_ACTIVE.
 * @note        This function is meant to be used in the low level drivers
 *              implementations only.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __spi_isr_half_code(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  __cbdrv_invoke_cb(self);
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Full buffer filled ISR code in circular mode.
 *              The callback is invoked with driver
 *                             state set to @p HAL_DRV_STATE_COMPLETE.
 * @note        This function is meant to be used in the low level drivers
 *              implementations only.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __spi_isr_full_code(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  __cbdrv_invoke_cb_with_transition(self,
                                    HAL_DRV_STATE_COMPLETE,
                                    HAL_DRV_STATE_ACTIVE);
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       ISR error reporting code..
 *              The callback is invoked with driver
 *                             state set to @p HAL_DRV_STATE_ERROR.
 * @note        This function is meant to be used in the low level drivers
 *              implementations only.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     msg           The error code.
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __spi_isr_error_code(void *ip, msg_t msg) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  __cbdrv_invoke_cb_with_transition(self,
                                    HAL_DRV_STATE_ERROR,
                                    HAL_DRV_STATE_READY);
  __spi_wakeup_isr(self, msg);
}
/** @} */

#endif /* HAL_USE_SPI == TRUE */

#endif /* HAL_SPI_H */

/** @} */
