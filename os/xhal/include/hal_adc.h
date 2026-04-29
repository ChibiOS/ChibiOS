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
 * @file        hal_adc.h
 * @brief       Generated ADC Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_ADC
 * @brief       ADC Driver macros and structures.
 * @{
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include "hal_cb_driver.h"

#if (HAL_USE_ADC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    ADC event flags
 * @{
 */
/**
 * @brief       Half-buffer completion event flag.
 */
#define ADC_EVENT_HALF                      (1U << 0)

/**
 * @brief       Full-buffer completion event flag.
 */
#define ADC_EVENT_FULL                      (1U << 1)

/**
 * @brief       Linear conversion completion event flag.
 */
#define ADC_EVENT_COMPLETE                  (1U << 2)
/** @} */

/**
 * @name    ADC driver specific states
 * @{
 */
/**
 * @brief       Linear conversion active state.
 */
#define ADC_ACTIVE_LINEAR                   HAL_DRV_STATE_ACTIVE

/**
 * @brief       Circular conversion active state.
 */
#define ADC_ACTIVE_CIRCULAR                 (HAL_DRV_STATE_ERROR + 1U)
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
#if !defined(ADC_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define ADC_USE_SYNCHRONIZATION             TRUE
#endif

/**
 * @brief       Support for ADC user configurations.
 * @note        When enabled the user must provide a variable named @p
 *              adc_configurations of type @p adc_configurations_t.
 */
#if !defined(ADC_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define ADC_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on ADC_USE_SYNCHRONIZATION configuration.*/
#if (ADC_USE_SYNCHRONIZATION != FALSE) && (ADC_USE_SYNCHRONIZATION != TRUE)
#error "invalid ADC_USE_SYNCHRONIZATION value"
#endif

/* Checks on ADC_USE_CONFIGURATIONS configuration.*/
#if (ADC_USE_CONFIGURATIONS != FALSE) && (ADC_USE_CONFIGURATIONS != TRUE)
#error "invalid ADC_USE_CONFIGURATIONS value"
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
 * @param[in]     adcp          Pointer to the ADC driver instance.
 * @return                      The buffer-complete state.
 */
#define adcIsBufferComplete(adcp)                                           \
  ((bool)((adcp)->state == HAL_DRV_STATE_FULL))
/** @} */

/**
 * @name    Low level driver helper macros
 * @{
 */
#if (ADC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Resumes a thread waiting for conversion completion.
 *
 * @param[in]     adcp          Pointer to the ADC driver instance.
 *
 * @notapi
 */
#define _adc_reset_i(adcp)                                                  \
  osalThreadResumeI(&(adcp)->thread, MSG_RESET)

/**
 * @brief       Resumes a thread waiting for conversion completion.
 *
 * @param[in]     adcp          Pointer to the ADC driver instance.
 *
 * @notapi
 */
#define _adc_reset_s(adcp)                                                  \
  osalThreadResumeS(&(adcp)->thread, MSG_RESET)

/**
 * @brief       Wakes up a thread waiting for a conversion state.
 *
 * @param[in]     adcp          Pointer to the ADC driver instance.
 * @param[in]     state         State being synchronized.
 *
 * @notapi
 */
#define _adc_wakeup_isr(adcp, state)                                        \
  do {                                                                      \
    osalSysLockFromISR();                                                   \
    if ((adcp)->sync_state == (state)) {                                    \
      osalThreadResumeI(&(adcp)->thread, MSG_OK);                           \
    }                                                                       \
    osalSysUnlockFromISR();                                                 \
  } while (false)

/**
 * @brief       Wakes up a thread waiting for conversion termination because of
 *              an error.
 *
 * @param[in]     adcp          Pointer to the ADC driver instance.
 *
 * @notapi
 */
#define _adc_error_wakeup_isr(adcp)                                         \
  do {                                                                      \
    osalSysLockFromISR();                                                   \
    osalThreadResumeI(&(adcp)->thread, MSG_RESET);                          \
    osalSysUnlockFromISR();                                                 \
  } while (false)

#else
#define _adc_reset_i(adcp)
#define _adc_reset_s(adcp)
#define _adc_wakeup_isr(adcp, state)
#define _adc_error_wakeup_isr(adcp)
#endif /* ADC_USE_SYNCHRONIZATION == TRUE */

/**
 * @brief       Common ISR code, half buffer event.
 *
 * @param[in,out] adcp          Pointer to the ADC driver instance.
 *
 * @notapi
 */
#define _adc_isr_half_code(adcp)                                            \
  do {                                                                      \
    (adcp)->events |= ADC_EVENT_HALF;                                       \
    __cbdrv_invoke_cb_with_transition(adcp,                                 \
                                      HAL_DRV_STATE_HALF,                   \
                                      ADC_ACTIVE_CIRCULAR);                 \
    _adc_wakeup_isr(adcp, HAL_DRV_STATE_HALF);                              \
  } while (false)

/**
 * @brief       Common ISR code, full buffer event.
 *
 * @param[in,out] adcp          Pointer to the ADC driver instance.
 *
 * @notapi
 */
#define _adc_isr_full_code(adcp)                                            \
  do {                                                                      \
    if ((adcp)->state == ADC_ACTIVE_CIRCULAR) {                             \
      (adcp)->events |= ADC_EVENT_FULL;                                     \
      __cbdrv_invoke_cb_with_transition(adcp,                               \
                                        HAL_DRV_STATE_FULL,                 \
                                        ADC_ACTIVE_CIRCULAR);               \
      _adc_wakeup_isr(adcp, HAL_DRV_STATE_FULL);                            \
    }                                                                       \
    else {                                                                  \
      adc_lld_stop_conversion(adcp);                                        \
      (adcp)->events |= ADC_EVENT_COMPLETE;                                 \
      __cbdrv_invoke_cb_with_transition(adcp,                               \
                                        HAL_DRV_STATE_COMPLETE,             \
                                        HAL_DRV_STATE_READY);               \
      (adcp)->grpp = NULL;                                                  \
      _adc_wakeup_isr(adcp, HAL_DRV_STATE_COMPLETE);                        \
    }                                                                       \
  } while (false)

/**
 * @brief       Common ISR code, error event.
 *
 * @param[in,out] adcp          Pointer to the ADC driver instance.
 * @param[in]     err           Platform dependent error code.
 *
 * @notapi
 */
#define _adc_isr_error_code(adcp, err)                                      \
  do {                                                                      \
    adc_lld_stop_conversion(adcp);                                          \
    (adcp)->errors |= (err);                                                \
    __cbdrv_invoke_cb_with_transition(adcp,                                 \
                                      HAL_DRV_STATE_ERROR,                  \
                                      HAL_DRV_STATE_READY);                 \
    (adcp)->grpp = NULL;                                                    \
    _adc_error_wakeup_isr(adcp);                                            \
  } while (false)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of ADC event flags.
 */
typedef uint32_t adceventflags_t;

/**
 * @brief       Type of structure representing an ADC driver.
 */
typedef struct hal_adc_driver hal_adc_driver_c;

/**
 * @brief       Type of structure representing an ADC configuration.
 */
typedef struct hal_adc_config hal_adc_config_t;

/**
 * @brief       Type of structure representing an ADC conversion group.
 */
typedef struct hal_adc_conversion_group adc_conversion_group_t;

/**
 * @brief       Type of structure representing ADC conversion groups.
 */
typedef struct hal_adc_conversion_groups adc_conversion_groups_t;

/**
 * @brief       Type of user-provided ADC configurations.
 */
typedef struct adc_configurations adc_configurations_t;

/* Including the low level driver header, it exports information required
   for completing types.*/
#include "hal_adc_lld.h"

/**
 * @brief       Conversion group configuration structure.
 */
struct hal_adc_conversion_group {
  /**
   * @brief       Number of analog channels in the conversion group.
   */
  adc_channels_num_t        num_channels;
  /* End of the mandatory fields.*/
  adc_lld_configuration_group_fields;
#if (defined(ADC_CONVERSION_GROUP_EXT_FIELDS)) || defined (__DOXYGEN__)
  ADC_CONVERSION_GROUP_EXT_FIELDS
#endif /* defined(ADC_CONVERSION_GROUP_EXT_FIELDS) */
};

/**
 * @brief       Conversion groups structure.
 */
struct hal_adc_conversion_groups {
  /**
   * @brief       Number of conversion groups in the open array.
   */
  unsigned                  grpsnum;
  /**
   * @brief       ADC conversion groups.
   */
  adc_conversion_group_t    grps[];
};

/**
 * @brief       Driver configuration structure.
 */
struct hal_adc_config {
  /**
   * @brief       ADC conversion groups or @p NULL.
   */
  const adc_conversion_groups_t * grps;
  /* End of the mandatory fields.*/
  adc_lld_config_fields;
#if (defined(ADC_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  ADC_CONFIG_EXT_FIELDS
#endif /* defined(ADC_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Structure representing user-provided ADC configurations.
 */
struct adc_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User ADC configurations.
   */
  hal_adc_config_t          cfgs[];
};

/**
 * @class       hal_adc_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of an ADC driver.
 *
 * @name        Class @p hal_adc_driver_c structures
 * @{
 */

/**
 * @brief       Type of a ADC driver class.
 */
typedef struct hal_adc_driver hal_adc_driver_c;

/**
 * @brief       Class @p hal_adc_driver_c virtual methods table.
 */
struct hal_adc_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*oncbset)(void *ip, drv_cb_t cb);
  /* From hal_adc_driver_c.*/
};

/**
 * @brief       Structure representing a ADC driver class.
 */
struct hal_adc_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_adc_driver_vmt *vmt;
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
  adcsample_t *             samples;
  /**
   * @brief       Current samples buffer depth or zero.
   */
  size_t                    depth;
  /**
   * @brief       Current conversion group pointer or @p NULL.
   */
  const adc_conversion_group_t * grpp;
  /**
   * @brief       Cached ADC event flags.
   */
  volatile adceventflags_t  events;
  /**
   * @brief       Cached ADC error flags.
   */
  volatile adcerror_t       errors;
#if (ADC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Waiting thread reference.
   */
  thread_reference_t        thread;
  /**
   * @brief       State being synchronized.
   */
  driver_state_t            sync_state;
#endif /* ADC_USE_SYNCHRONIZATION == TRUE */
#if (defined(ADC_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  ADC_DRIVER_EXT_FIELDS
#endif /* defined(ADC_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  adc_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_adc_driver_c.*/
  void *__adc_objinit_impl(void *ip, const void *vmt);
  void __adc_dispose_impl(void *ip);
  msg_t __adc_start_impl(void *ip, const void *config);
  void __adc_stop_impl(void *ip);
  const void *__adc_setcfg_impl(void *ip, const void *config);
  const void *__adc_selcfg_impl(void *ip, unsigned cfgnum);
  void __adc_oncbset_impl(void *ip, drv_cb_t cb);
  msg_t adcStartConversionLinearI(void *ip, unsigned grpnum,
                                  adcsample_t *samples, size_t depth);
  msg_t adcStartConversionLinear(void *ip, unsigned grpnum,
                                 adcsample_t *samples, size_t depth);
  msg_t adcStartConversionCircularI(void *ip, unsigned grpnum,
                                    adcsample_t *samples, size_t depth);
  msg_t adcStartConversionCircular(void *ip, unsigned grpnum,
                                   adcsample_t *samples, size_t depth);
  void adcStopConversionI(void *ip);
  void adcStopConversion(void *ip);
#if (ADC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  msg_t adcConvert(void *ip, unsigned grpnum, adcsample_t *samples,
                   size_t depth);
  msg_t adcSynchronizeStateS(void *ip, driver_state_t state,
                             sysinterval_t timeout);
  msg_t adcSynchronizeState(void *ip, driver_state_t state,
                            sysinterval_t timeout);
#endif /* ADC_USE_SYNCHRONIZATION == TRUE */
  /* Regular functions.*/
  void adcInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_adc_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_adc_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_adc_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_adc_driver_c *adcObjectInit(hal_adc_driver_c *self) {
  extern const struct hal_adc_driver_vmt __hal_adc_driver_vmt;

  return __adc_objinit_impl(self, &__hal_adc_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_adc_driver_c
 * @{
 */
/**
 * @brief       Returns the cached ADC event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @return                      The current cached event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline adceventflags_t adcGetEventsX(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  return self->events;
}

/**
 * @brief       Gets and clears cached ADC event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     mask          Mask of events to be returned and cleared.
 * @return                      The selected pending event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline adceventflags_t adcGetAndClearEventsX(void *ip,
                                                    adceventflags_t mask) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  adceventflags_t flags;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  flags = self->events & mask;
  self->events &= ~mask;
  osalSysRestoreStatusX(sts);

  return flags;
}

/**
 * @brief       Returns the cached ADC error flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @return                      The current cached error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline adcerror_t adcGetErrorsX(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  return self->errors;
}

/**
 * @brief       Gets and clears cached ADC error flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_adc_driver_c instance.
 * @param[in]     mask          Mask of errors to be returned and cleared.
 * @return                      The selected pending error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline adcerror_t adcGetAndClearErrorsX(void *ip, adcerror_t mask) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  adcerror_t errors;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  errors = self->errors & mask;
  self->errors &= ~mask;
  osalSysRestoreStatusX(sts);

  return errors;
}
/** @} */

#endif /* HAL_USE_ADC == TRUE */

#endif /* HAL_ADC_H */

/** @} */
