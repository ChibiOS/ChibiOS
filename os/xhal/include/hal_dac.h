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
 * @file        hal_dac.h
 * @brief       Generated DAC Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_DAC
 * @brief       DAC Driver macros and structures.
 * @{
 */

#ifndef HAL_DAC_H
#define HAL_DAC_H

#include "hal_cb_driver.h"

#if (HAL_USE_DAC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    DAC event flags
 * @{
 */
/**
 * @brief       Half-buffer completion event flag.
 */
#define DAC_EVENT_HALF                      (1U << 0)

/**
 * @brief       Full-buffer completion event flag.
 */
#define DAC_EVENT_FULL                      (1U << 1)
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
#if !defined(DAC_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define DAC_USE_SYNCHRONIZATION             TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on DAC_USE_SYNCHRONIZATION configuration.*/
#if (DAC_USE_SYNCHRONIZATION != FALSE) && (DAC_USE_SYNCHRONIZATION != TRUE)
#error "invalid DAC_USE_SYNCHRONIZATION value"
#endif

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
 * @param[in]     dacp          Pointer to the DAC driver instance.
 * @return                      The buffer-complete state.
 */
#define dacIsBufferComplete(dacp)                                           \
  ((bool)((dacp)->state == HAL_DRV_STATE_FULL))
/** @} */

/**
 * @name    Low level driver helper macros
 * @{
 */
#if (DAC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Resumes a thread waiting for DAC streaming completion.
 *
 * @param[in]     dacp          Pointer to the DAC driver instance.
 *
 * @notapi
 */
#define _dac_reset_i(dacp)                                                  \
  osalThreadResumeI(&(dacp)->thread, MSG_RESET)

/**
 * @brief       Resumes a thread waiting for DAC streaming completion.
 *
 * @param[in]     dacp          Pointer to the DAC driver instance.
 *
 * @notapi
 */
#define _dac_reset_s(dacp)                                                  \
  osalThreadResumeS(&(dacp)->thread, MSG_RESET)

/**
 * @brief       Wakes up a thread waiting for a DAC streaming state.
 *
 * @param[in]     dacp          Pointer to the DAC driver instance.
 * @param[in]     state         State being synchronized.
 *
 * @notapi
 */
#define _dac_wakeup_isr(dacp, state)                                        \
  do {                                                                      \
    osalSysLockFromISR();                                                   \
    if ((dacp)->sync_state == (state)) {                                    \
      osalThreadResumeI(&(dacp)->thread, MSG_OK);                           \
    }                                                                       \
    osalSysUnlockFromISR();                                                 \
  } while (false)

/**
 * @brief       Wakes up a thread waiting for DAC streaming termination because
 *              of an error.
 *
 * @param[in]     dacp          Pointer to the DAC driver instance.
 *
 * @notapi
 */
#define _dac_error_wakeup_isr(dacp)                                         \
  do {                                                                      \
    osalSysLockFromISR();                                                   \
    osalThreadResumeI(&(dacp)->thread, MSG_RESET);                          \
    osalSysUnlockFromISR();                                                 \
  } while (false)

#else
#define _dac_reset_i(dacp)
#define _dac_reset_s(dacp)
#define _dac_wakeup_isr(dacp, state)
#define _dac_error_wakeup_isr(dacp)
#endif /* DAC_USE_SYNCHRONIZATION == TRUE */

/**
 * @brief       Common ISR code, half buffer event.
 *
 * @param[in,out] dacp          Pointer to the DAC driver instance.
 *
 * @notapi
 */
#define _dac_isr_half_code(dacp)                                            \
  do {                                                                      \
    (dacp)->events |= DAC_EVENT_HALF;                                       \
    __cbdrv_invoke_half_cb(dacp);                                           \
    _dac_wakeup_isr(dacp, HAL_DRV_STATE_HALF);                              \
  } while (false)

/**
 * @brief       Common ISR code, full buffer event.
 *
 * @param[in,out] dacp          Pointer to the DAC driver instance.
 *
 * @notapi
 */
#define _dac_isr_full_code(dacp)                                            \
  do {                                                                      \
    (dacp)->events |= DAC_EVENT_FULL;                                       \
    __cbdrv_invoke_full_cb(dacp);                                           \
    _dac_wakeup_isr(dacp, HAL_DRV_STATE_FULL);                              \
  } while (false)

/**
 * @brief       Common ISR code, error event.
 *
 * @param[in,out] dacp          Pointer to the DAC driver instance.
 * @param[in]     err           Platform dependent error code.
 *
 * @notapi
 */
#define _dac_isr_error_code(dacp, err)                                      \
  do {                                                                      \
    dac_lld_stop_conversion(dacp);                                          \
    (dacp)->errors |= (err);                                                \
    __cbdrv_invoke_cb_with_transition(dacp,                                 \
                                      HAL_DRV_STATE_ERROR,                  \
                                      HAL_DRV_STATE_READY);                 \
    (dacp)->grpp = NULL;                                                    \
    _dac_error_wakeup_isr(dacp);                                            \
  } while (false)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of DAC event flags.
 */
typedef uint32_t daceventflags_t;

/**
 * @brief       Type of structure representing a DAC driver.
 */
typedef struct hal_dac_driver hal_dac_driver_c;

/**
 * @brief       Type of structure representing a DAC configuration.
 */
typedef struct hal_dac_config hal_dac_config_t;

/**
 * @brief       Type of structure representing a DAC conversion group.
 */
typedef struct hal_dac_conversion_group dac_conversion_group_t;

typedef struct hal_dac_driver DACDriver;
typedef struct hal_dac_config DACConfig;
typedef struct hal_dac_conversion_group DACConversionGroup;

/* Inclusion of LLD header.*/
#include "hal_dac_lld.h"

/**
 * @brief       Conversion group configuration structure.
 */
struct hal_dac_conversion_group {
  /**
   * @brief       Number of DAC channels in the conversion group.
   */
  uint32_t                  num_channels;
  /* End of the mandatory fields.*/
  dac_lld_conversion_group_fields;
#if (defined(DAC_CONVERSION_GROUP_EXT_FIELDS)) || defined (__DOXYGEN__)
  DAC_CONVERSION_GROUP_EXT_FIELDS
#endif /* defined(DAC_CONVERSION_GROUP_EXT_FIELDS) */
};

/**
 * @brief       Driver configuration structure.
 */
struct hal_dac_config {
  /* End of the mandatory fields.*/
  dac_lld_config_fields;
#if (defined(DAC_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  DAC_CONFIG_EXT_FIELDS
#endif /* defined(DAC_CONFIG_EXT_FIELDS) */
};

/**
 * @class       hal_dac_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of a DAC driver.
 *
 * @name        Class @p hal_dac_driver_c structures
 * @{
 */

/**
 * @brief       Type of a DAC driver class.
 */
typedef struct hal_dac_driver hal_dac_driver_c;

/**
 * @brief       Class @p hal_dac_driver_c virtual methods table.
 */
struct hal_dac_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*oncbset)(void *ip, drv_cb_t cb);
  /* From hal_dac_driver_c.*/
};

/**
 * @brief       Structure representing a DAC driver class.
 */
struct hal_dac_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_dac_driver_vmt *vmt;
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
   * @brief       Current samples buffer pointer or @p NULL.
   */
  dacsample_t *             samples;
  /**
   * @brief       Current samples buffer depth or zero.
   */
  size_t                    depth;
  /**
   * @brief       Current conversion group pointer or @p NULL.
   */
  const dac_conversion_group_t * grpp;
  /**
   * @brief       Cached DAC event flags.
   */
  volatile daceventflags_t  events;
  /**
   * @brief       Cached DAC error flags.
   */
  volatile dacerror_t       errors;
#if (DAC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Waiting thread reference.
   */
  thread_reference_t        thread;
  /**
   * @brief       State being synchronized.
   */
  driver_state_t            sync_state;
#endif /* DAC_USE_SYNCHRONIZATION == TRUE */
#if (defined(DAC_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  DAC_DRIVER_EXT_FIELDS
#endif /* defined(DAC_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  dac_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_dac_driver_c.*/
  void *__dac_objinit_impl(void *ip, const void *vmt);
  void __dac_dispose_impl(void *ip);
  msg_t __dac_start_impl(void *ip, const void *config);
  void __dac_stop_impl(void *ip);
  const void *__dac_setcfg_impl(void *ip, const void *config);
  const void *__dac_selcfg_impl(void *ip, unsigned cfgnum);
  void __dac_oncbset_impl(void *ip, drv_cb_t cb);
  msg_t dacPutChannelX(void *ip, dacchannel_t channel, dacsample_t sample);
  msg_t dacStartConversionI(void *ip, const dac_conversion_group_t *grpp,
                            dacsample_t *samples, size_t depth);
  msg_t dacStartConversion(void *ip, const dac_conversion_group_t *grpp,
                           dacsample_t *samples, size_t depth);
  void dacStopConversionI(void *ip);
  void dacStopConversion(void *ip);
#if (DAC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  msg_t dacConvert(void *ip, const dac_conversion_group_t *grpp,
                   dacsample_t *samples, size_t depth);
  msg_t dacSynchronizeStateS(void *ip, driver_state_t state,
                             sysinterval_t timeout);
  msg_t dacSynchronizeState(void *ip, driver_state_t state,
                            sysinterval_t timeout);
#endif /* DAC_USE_SYNCHRONIZATION == TRUE */
  /* Regular functions.*/
  void dacInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_dac_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_dac_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_dac_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_dac_driver_c *dacObjectInit(hal_dac_driver_c *self) {
  extern const struct hal_dac_driver_vmt __hal_dac_driver_vmt;

  return __dac_objinit_impl(self, &__hal_dac_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_dac_driver_c
 * @{
 */
/**
 * @brief       Returns the cached DAC event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @return                      The current cached event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline daceventflags_t dacGetEventsX(void *ip) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  return self->events;
}

/**
 * @brief       Gets and clears cached DAC event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     mask          Mask of events to be returned and cleared.
 * @return                      The selected pending event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline daceventflags_t dacGetAndClearEventsX(void *ip,
                                                    daceventflags_t mask) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  daceventflags_t flags;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  flags = self->events & mask;
  self->events &= ~mask;
  osalSysRestoreStatusX(sts);

  return flags;
}

/**
 * @brief       Returns the cached DAC error flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @return                      The current cached error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline dacerror_t dacGetErrorsX(void *ip) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  return self->errors;
}

/**
 * @brief       Gets and clears cached DAC error flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     mask          Mask of errors to be returned and cleared.
 * @return                      The selected pending error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline dacerror_t dacGetAndClearErrorsX(void *ip, dacerror_t mask) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  dacerror_t errors;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  errors = self->errors & mask;
  self->errors &= ~mask;
  osalSysRestoreStatusX(sts);

  return errors;
}
/** @} */

#endif /* HAL_USE_DAC == TRUE */

#endif /* HAL_DAC_H */

/** @} */
