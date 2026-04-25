/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file        hal_i2s.h
 * @brief       Generated I2S Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_I2S
 * @brief       I2S Driver macros and structures.
 * @{
 */

#ifndef HAL_I2S_H
#define HAL_I2S_H

#include "hal_cb_driver.h"

#if (HAL_USE_I2S == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    I2S event flags
 * @{
 */
/**
 * @brief       Half-buffer completion event flag.
 */
#define I2S_EVENT_HALF                      (1U << 0)

/**
 * @brief       Full-buffer completion event flag.
 */
#define I2S_EVENT_FULL                      (1U << 1)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro functions
 * @{
 */
/**
 * @brief       Checks if the current callback refers to a full buffer.
 *
 * @param[in]     i2sp          Pointer to the I2S driver instance.
 * @return                      The buffer-complete state.
 */
#define i2sIsBufferComplete(i2sp)                                           \
  ((bool)((i2sp)->state == HAL_DRV_STATE_FULL))
/** @} */

/**
 * @name    Low level driver helper macros
 * @{
 */
/**
 * @brief       Common ISR code, half buffer event.
 *
 * @param[in,out] i2sp          Pointer to the I2S driver instance.
 *
 * @notapi
 */
#define _i2s_isr_half_code(i2sp)                                            \
  do {                                                                      \
    (i2sp)->events |= I2S_EVENT_HALF;                                       \
    __cbdrv_invoke_half_cb(i2sp);                                           \
  } while (false)

/**
 * @brief       Common ISR code, full buffer event.
 *
 * @param[in,out] i2sp          Pointer to the I2S driver instance.
 *
 * @notapi
 */
#define _i2s_isr_full_code(i2sp)                                            \
  do {                                                                      \
    (i2sp)->events |= I2S_EVENT_FULL;                                       \
    __cbdrv_invoke_full_cb(i2sp);                                           \
  } while (false)

/**
 * @brief       Common ISR code, error event.
 *
 * @param[in,out] i2sp          Pointer to the I2S driver instance.
 * @param[in]     err           Platform dependent error code.
 *
 * @notapi
 */
#define _i2s_isr_error_code(i2sp, err)                                      \
  do {                                                                      \
    i2s_lld_stop_exchange(i2sp);                                            \
    (i2sp)->errors |= (err);                                                \
    __cbdrv_invoke_cb_with_transition(i2sp,                                 \
                                      HAL_DRV_STATE_ERROR,                  \
                                      HAL_DRV_STATE_READY);                 \
  } while (false)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of I2S event flags.
 */
typedef uint32_t i2seventflags_t;

/**
 * @brief       Type of structure representing an I2S driver.
 */
typedef struct hal_i2s_driver hal_i2s_driver_c;

/**
 * @brief       Type of structure representing an I2S configuration.
 */
typedef struct hal_i2s_config hal_i2s_config_t;

typedef struct hal_i2s_driver I2SDriver;
typedef struct hal_i2s_config I2SConfig;

/* Inclusion of LLD header.*/
#include "hal_i2s_lld.h"

/**
 * @brief       Driver configuration structure.
 */
struct hal_i2s_config {
  /**
   * @brief       Transmission buffer pointer or @p NULL.
   */
  const void *              tx_buffer;
  /**
   * @brief       Receive buffer pointer or @p NULL.
   */
  void *                    rx_buffer;
  /**
   * @brief       TX and RX buffer size as number of samples.
   */
  size_t                    size;
  /* End of the mandatory fields.*/
  i2s_lld_config_fields;
#if (defined(I2S_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  I2S_CONFIG_EXT_FIELDS
#endif /* defined(I2S_CONFIG_EXT_FIELDS) */
};

/**
 * @class       hal_i2s_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of an I2S driver.
 *
 * @name        Class @p hal_i2s_driver_c structures
 * @{
 */

/**
 * @brief       Type of a I2S driver class.
 */
typedef struct hal_i2s_driver hal_i2s_driver_c;

/**
 * @brief       Class @p hal_i2s_driver_c virtual methods table.
 */
struct hal_i2s_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  msg_t (*synchronize)(void *ip, sysinterval_t timeout);
  /* From hal_cb_driver_c.*/
  void (*setcb)(void *ip, drv_cb_t cb);
  /* From hal_i2s_driver_c.*/
};

/**
 * @brief       Structure representing a I2S driver class.
 */
struct hal_i2s_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_i2s_driver_vmt *vmt;
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
  /**
   * @brief       Cached I2S event flags.
   */
  volatile i2seventflags_t  events;
  /**
   * @brief       Cached I2S error flags.
   */
  volatile i2serror_t       errors;
#if (defined(I2S_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  I2S_DRIVER_EXT_FIELDS
#endif /* defined(I2S_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  i2s_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_i2s_driver_c.*/
  void *__i2s_objinit_impl(void *ip, const void *vmt);
  void __i2s_dispose_impl(void *ip);
  msg_t __i2s_start_impl(void *ip, const void *config);
  void __i2s_stop_impl(void *ip);
  const void *__i2s_setcfg_impl(void *ip, const void *config);
  const void *__i2s_selcfg_impl(void *ip, unsigned cfgnum);
  msg_t i2sStartExchangeI(void *ip);
  msg_t i2sStartExchange(void *ip);
  void i2sStopExchangeI(void *ip);
  void i2sStopExchange(void *ip);
  /* Regular functions.*/
  void i2sInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_i2s_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_i2s_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_i2s_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_i2s_driver_c *i2sObjectInit(hal_i2s_driver_c *self) {
  extern const struct hal_i2s_driver_vmt __hal_i2s_driver_vmt;

  return __i2s_objinit_impl(self, &__hal_i2s_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_i2s_driver_c
 * @{
 */
/**
 * @brief       Returns the cached I2S event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @return                      The current cached event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline i2seventflags_t i2sGetEventsX(void *ip) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  return self->events;
}

/**
 * @brief       Gets and clears cached I2S event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @param[in]     mask          Mask of events to be returned and cleared.
 * @return                      The selected pending event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline i2seventflags_t i2sGetAndClearEventsX(void *ip,
                                                    i2seventflags_t mask) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  i2seventflags_t flags;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  flags = self->events & mask;
  self->events &= ~mask;
  osalSysRestoreStatusX(sts);

  return flags;
}

/**
 * @brief       Returns the cached I2S error flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @return                      The current cached error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline i2serror_t i2sGetErrorsX(void *ip) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  return self->errors;
}

/**
 * @brief       Gets and clears cached I2S error flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @param[in]     mask          Mask of errors to be returned and cleared.
 * @return                      The selected pending error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline i2serror_t i2sGetAndClearErrorsX(void *ip, i2serror_t mask) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  i2serror_t errors;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  errors = self->errors & mask;
  self->errors &= ~mask;
  osalSysRestoreStatusX(sts);

  return errors;
}
/** @} */

#endif /* HAL_USE_I2S == TRUE */

#endif /* HAL_I2S_H */

/** @} */
