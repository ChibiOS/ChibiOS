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
 * @brief       ADC Driver header.
 *
 * @addtogroup  HAL_ADC
 * @brief       ADC Driver macros and structures.
 * @{
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#if (HAL_USE_ADC == TRUE) || defined(__DOXYGEN__)

#include "hal_cb_driver.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name        ADC event flags
 * @{
 */
#define ADC_EVENT_HALF                  (1U << 0)
#define ADC_EVENT_FULL                  (1U << 1)
#define ADC_EVENT_COMPLETE              (1U << 2)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name        ADC configuration options
 * @{
 */
#if !defined(ADC_USE_WAIT) || defined(__DOXYGEN__)
#define ADC_USE_WAIT                    TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (ADC_USE_WAIT != FALSE) && (ADC_USE_WAIT != TRUE)
#error "invalid ADC_USE_WAIT value"
#endif

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
 * @brief       Type of structure representing an ADC driver configuration.
 */
typedef struct hal_adc_config hal_adc_config_t;

/**
 * @brief       Type of structure representing an ADC conversion group.
 */
typedef struct hal_adc_conversion_group adc_conversion_group_t;

/**
 * @brief       HAL-compatible type aliases.
 * @{
 */
typedef hal_adc_driver_c ADCDriver;
typedef hal_adc_config_t ADCConfig;
typedef adc_conversion_group_t ADCConversionGroup;
/** @} */

/* Including the low level driver header, it exports information required
   for completing types.*/
#include "hal_adc_lld.h"

/**
 * @brief       Type of an ADC notification callback.
 * @details     The callback is invoked from ISR context.
 *
 * @param[in]   adcp            Pointer to the @p ADCDriver object triggering
 *                              the callback.
 */
typedef void (*adccallback_t)(ADCDriver *adcp);

/**
 * @brief       Type of an ADC error callback.
 *
 * @param[in]   adcp            Pointer to the @p ADCDriver object triggering
 *                              the callback.
 * @param[in]   err             ADC error code.
 */
typedef void (*adcerrorcallback_t)(ADCDriver *adcp, adcerror_t err);

/**
 * @brief       Conversion group configuration structure.
 */
struct hal_adc_conversion_group {
  /**
   * @brief       Enables the circular buffer mode for the group.
   */
  bool                      circular;
  /**
   * @brief       Number of the analog channels belonging to the conversion
   *              group.
   */
  adc_channels_num_t        num_channels;
  /**
   * @brief       End callback function associated to the group or @p NULL.
   */
  adccallback_t             end_cb;
  /**
   * @brief       Error callback associated to the group or @p NULL.
   */
  adcerrorcallback_t        error_cb;
  /* End of the mandatory fields.*/
  adc_lld_configuration_group_fields;
#if defined(ADC_CONVERSION_GROUP_EXT_FIELDS) || defined(__DOXYGEN__)
  ADC_CONVERSION_GROUP_EXT_FIELDS
#endif
};

/**
 * @brief       Driver configuration structure.
 */
struct hal_adc_config {
  /* End of the mandatory fields.*/
  adc_lld_config_fields;
#if defined(ADC_CONFIG_EXT_FIELDS) || defined(__DOXYGEN__)
  ADC_CONFIG_EXT_FIELDS
#endif
};

/**
 * @class       hal_adc_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of an ADC driver.
 * @{
 */

/**
 * @brief       Class @p hal_adc_driver_c virtual methods table.
 */
struct hal_adc_driver_vmt {
  void (*dispose)(void *ip);
  msg_t (*start)(void *ip);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  void (*setcb)(void *ip, drv_cb_t cb);
};

/**
 * @brief       Structure representing an ADC driver class.
 */
struct hal_adc_driver {
  const struct hal_adc_driver_vmt *vmt;
  driver_state_t            state;
  const void                *config;
  void                      *arg;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  mutex_t                   mutex;
#endif
#if (HAL_USE_REGISTRY == TRUE) || defined(__DOXYGEN__)
  unsigned int              id;
  const char                *name;
  hal_regent_t              regent;
#endif
  drv_cb_t                  cb;
  adcsample_t               *samples;
  size_t                    depth;
  const adc_conversion_group_t *grpp;
  volatile adceventflags_t  events;
  volatile adcerror_t       errors;
#if (ADC_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  thread_reference_t        thread;
#endif
#if defined(ADC_DRIVER_EXT_FIELDS) || defined(__DOXYGEN__)
  ADC_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  adc_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name        Macro functions
 * @{
 */
#define adcIsBufferComplete(adcp) ((bool)((adcp)->state == HAL_DRV_STATE_FULL))
/** @} */

/**
 * @name        Low level driver helper macros
 * @{
 */
#if (ADC_USE_WAIT == TRUE) || defined(__DOXYGEN__)
#define _adc_reset_i(adcp)                                                  \
  osalThreadResumeI(&(adcp)->thread, MSG_RESET)

#define _adc_reset_s(adcp)                                                  \
  osalThreadResumeS(&(adcp)->thread, MSG_RESET)

#define _adc_wakeup_isr(adcp) {                                             \
  osalSysLockFromISR();                                                     \
  osalThreadResumeI(&(adcp)->thread, MSG_OK);                               \
  osalSysUnlockFromISR();                                                   \
}

#define _adc_timeout_isr(adcp) {                                            \
  osalSysLockFromISR();                                                     \
  osalThreadResumeI(&(adcp)->thread, MSG_TIMEOUT);                          \
  osalSysUnlockFromISR();                                                   \
}
#else
#define _adc_reset_i(adcp)
#define _adc_reset_s(adcp)
#define _adc_wakeup_isr(adcp)
#define _adc_timeout_isr(adcp)
#endif

#define _adc_isr_half_code(adcp) {                                          \
  (adcp)->events |= ADC_EVENT_HALF;                                         \
  if ((adcp)->grpp->end_cb != NULL) {                                       \
    (adcp)->state = HAL_DRV_STATE_HALF;                                     \
    (adcp)->grpp->end_cb(adcp);                                             \
    if ((adcp)->state == HAL_DRV_STATE_HALF) {                              \
      (adcp)->state = HAL_DRV_STATE_ACTIVE;                                 \
    }                                                                       \
  }                                                                         \
  __cbdrv_invoke_cb(adcp);                                                  \
}

#define _adc_isr_full_code(adcp) {                                          \
  if ((adcp)->grpp->circular) {                                             \
    (adcp)->events |= ADC_EVENT_FULL;                                       \
    if ((adcp)->grpp->end_cb != NULL) {                                     \
      (adcp)->state = HAL_DRV_STATE_FULL;                                   \
      (adcp)->grpp->end_cb(adcp);                                           \
      if ((adcp)->state == HAL_DRV_STATE_FULL) {                            \
        (adcp)->state = HAL_DRV_STATE_ACTIVE;                               \
      }                                                                     \
    }                                                                       \
    __cbdrv_invoke_cb(adcp);                                                \
  }                                                                         \
  else {                                                                    \
    const adc_conversion_group_t *grpp = (adcp)->grpp;                      \
    adc_lld_stop_conversion(adcp);                                          \
    (adcp)->events |= ADC_EVENT_COMPLETE;                                   \
    if ((adcp)->grpp->end_cb != NULL) {                                     \
      (adcp)->state = HAL_DRV_STATE_COMPLETE;                               \
      (adcp)->grpp->end_cb(adcp);                                           \
      if (((adcp)->state == HAL_DRV_STATE_COMPLETE) &&                      \
          ((adcp)->grpp == grpp)) {                                         \
        (adcp)->state = HAL_DRV_STATE_READY;                                \
        (adcp)->grpp = NULL;                                                \
      }                                                                     \
    }                                                                       \
    else {                                                                  \
      (adcp)->state = HAL_DRV_STATE_READY;                                  \
      (adcp)->grpp = NULL;                                                  \
    }                                                                       \
    __cbdrv_invoke_cb(adcp);                                                \
    _adc_wakeup_isr(adcp);                                                  \
  }                                                                         \
}

#define _adc_isr_error_code(adcp, err) {                                    \
  adc_lld_stop_conversion(adcp);                                            \
  (adcp)->errors |= (err);                                                  \
  if ((adcp)->grpp->error_cb != NULL) {                                     \
    (adcp)->state = HAL_DRV_STATE_ERROR;                                    \
    (adcp)->grpp->error_cb(adcp, err);                                      \
    if ((adcp)->state == HAL_DRV_STATE_ERROR) {                             \
      (adcp)->state = HAL_DRV_STATE_READY;                                  \
      (adcp)->grpp = NULL;                                                  \
    }                                                                       \
  }                                                                         \
  else {                                                                    \
    (adcp)->state = HAL_DRV_STATE_READY;                                    \
    (adcp)->grpp = NULL;                                                    \
  }                                                                         \
  __cbdrv_invoke_cb(adcp);                                                  \
  _adc_timeout_isr(adcp);                                                   \
}
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void adcInit(void);
  void *__adc_objinit_impl(void *ip, const void *vmt);
  void __adc_dispose_impl(void *ip);
  msg_t __adc_start_impl(void *ip);
  void __adc_stop_impl(void *ip);
  const void *__adc_setcfg_impl(void *ip, const void *config);
  const void *__adc_selcfg_impl(void *ip, unsigned cfgnum);
  void __adc_setcb_impl(void *ip, drv_cb_t cb);
  msg_t adcStart(void *ip, const hal_adc_config_t *config);
  void adcStop(void *ip);
  msg_t adcStartConversionI(void *ip, const adc_conversion_group_t *grpp,
                            adcsample_t *samples, size_t depth);
  msg_t adcStartConversion(void *ip, const adc_conversion_group_t *grpp,
                           adcsample_t *samples, size_t depth);
  void adcStopConversionI(void *ip);
  void adcStopConversion(void *ip);
#if (ADC_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  msg_t adcConvert(void *ip, const adc_conversion_group_t *grpp,
                   adcsample_t *samples, size_t depth);
#endif
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
CC_FORCE_INLINE
static inline adceventflags_t adcGetEventsX(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  return self->events;
}

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

CC_FORCE_INLINE
static inline adcerror_t adcGetErrorsX(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  return self->errors;
}

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
