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
 * @file        hal_icu.h
 * @brief       Generated ICU Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_ICU
 * @brief       ICU Driver macros and structures.
 * @{
 */

#ifndef HAL_ICU_H
#define HAL_ICU_H

#include "hal_cb_driver.h"

#if (HAL_USE_ICU == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    ICU event flags
 * @{
 */
/**
 * @brief       Pulse width capture event flag.
 */
#define ICU_EVENT_WIDTH                     (1U << 0)

/**
 * @brief       Cycle period capture event flag.
 */
#define ICU_EVENT_PERIOD                    (1U << 1)

/**
 * @brief       Timer overflow event flag.
 */
#define ICU_EVENT_OVERFLOW                  (1U << 2)

/**
 * @brief       All ICU event flags.
 */
#define ICU_EVENT_ALL                       (ICU_EVENT_WIDTH | ICU_EVENT_PERIOD | ICU_EVENT_OVERFLOW)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Support for ICU user configurations.
 * @note        When enabled the user must provide a variable named @p
 *              icu_configurations of type @p icu_configurations_t.
 */
#if !defined(ICU_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define ICU_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on ICU_USE_CONFIGURATIONS configuration.*/
#if (ICU_USE_CONFIGURATIONS != FALSE) && (ICU_USE_CONFIGURATIONS != TRUE)
#error "invalid ICU_USE_CONFIGURATIONS value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Low level driver helper macros
 * @{
 */
/**
 * @brief       Common ISR code, ICU event.
 *
 * @param[in,out] icup          Pointer to the ICU driver instance.
 * @param[in]     events        ICU event flags.
 *
 * @notapi
 */
#define _icu_isr_invoke_cb(icup, events)                                    \
  do {                                                                      \
    (icup)->events |= (events);                                             \
    __cbdrv_invoke_cb(icup);                                                \
  } while (false)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       ICU frequency type.
 */
typedef uint32_t icufreq_t;

/**
 * @brief       ICU counter type.
 */
typedef uint32_t icucnt_t;

/**
 * @brief       Type of ICU event flags.
 */
typedef uint32_t icu_events_t;

/**
 * @brief       Type of ICU driver state.
 */
typedef driver_state_t icustate_t;

/**
 * @brief       Type of structure representing an ICU driver.
 */
typedef struct hal_icu_driver hal_icu_driver_c;

/**
 * @brief       Type of structure representing an ICU configuration.
 */
typedef struct hal_icu_config hal_icu_config_t;

/**
 * @brief       Type of structure representing an ICU driver (legacy).
 */
typedef struct hal_icu_driver ICUDriver;

/**
 * @brief       Type of structure representing an ICU configuration (legacy).
 */
typedef struct hal_icu_config ICUConfig;

/**
 * @brief       ICU input active level.
 */
typedef enum {
  ICU_INPUT_ACTIVE_HIGH = 0U,       /**< Trigger on rising edge.            */
  ICU_INPUT_ACTIVE_LOW  = 1U        /**< Trigger on falling edge.           */
} icumode_t;

/**
 * @brief       ICU input channel.
 */
typedef enum {
  ICU_CHANNEL_1 = 0U,               /**< Use TIMxCH1.                      */
  ICU_CHANNEL_2 = 1U                /**< Use TIMxCH2.                      */
} icuchannel_t;

#define ICU_WAITING                        (HAL_DRV_STATE_ERROR + 1U)
#define ICU_ACTIVE                         (HAL_DRV_STATE_ERROR + 2U)

/* Inclusion of LLD header.*/
#include "hal_icu_lld.h"

/**
 * @brief       Driver configuration structure.
 */
struct hal_icu_config {
  /**
   * @brief       Input active level.
   */
  icumode_t                 mode;
  /**
   * @brief       Timer clock in Hz.
   */
  icufreq_t                 frequency;
  /**
   * @brief       Initially enabled ICU event flags.
   */
  icu_events_t              enabled_events;
  /* End of the mandatory fields.*/
  icu_lld_config_fields;
#if (defined(ICU_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  ICU_CONFIG_EXT_FIELDS
#endif /* defined(ICU_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Type of user-provided ICU configurations.
 */
typedef struct icu_configurations icu_configurations_t;

/**
 * @brief       Structure representing user-provided ICU configurations.
 */
struct icu_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User ICU configurations.
   */
  hal_icu_config_t          cfgs[];
};

/**
 * @class       hal_icu_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of an ICU driver.
 *
 * @name        Class @p hal_icu_driver_c structures
 * @{
 */

/**
 * @brief       Type of a ICU driver class.
 */
typedef struct hal_icu_driver hal_icu_driver_c;

/**
 * @brief       Class @p hal_icu_driver_c virtual methods table.
 */
struct hal_icu_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*oncbset)(void *ip, drv_cb_t cb);
  /* From hal_icu_driver_c.*/
};

/**
 * @brief       Structure representing a ICU driver class.
 */
struct hal_icu_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_icu_driver_vmt *vmt;
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
   * @brief       Latest captured pulse width in ticks.
   */
  icucnt_t                  width;
  /**
   * @brief       Latest captured cycle period in ticks.
   */
  icucnt_t                  period;
  /**
   * @brief       Mask of the enabled ICU events.
   */
  icu_events_t              enabled_events;
  /**
   * @brief       Cached ICU event flags.
   */
  volatile icu_events_t     events;
#if (defined(ICU_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  ICU_DRIVER_EXT_FIELDS
#endif /* defined(ICU_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  icu_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_icu_driver_c.*/
  void *__icu_objinit_impl(void *ip, const void *vmt);
  void __icu_dispose_impl(void *ip);
  msg_t __icu_start_impl(void *ip, const void *config);
  void __icu_stop_impl(void *ip);
  const void *__icu_setcfg_impl(void *ip, const void *config);
  const void *__icu_selcfg_impl(void *ip, unsigned cfgnum);
  void __icu_oncbset_impl(void *ip, drv_cb_t cb);
  void icuStartCaptureI(void *ip);
  void icuStartCapture(void *ip);
  bool icuWaitCapture(void *ip);
  void icuStopCaptureI(void *ip);
  void icuStopCapture(void *ip);
  void icuEnableEventsI(void *ip, icu_events_t events);
  void icuEnableEvents(void *ip, icu_events_t events);
  void icuDisableEventsI(void *ip, icu_events_t events);
  void icuDisableEvents(void *ip, icu_events_t events);
  void icuEnableNotifications(void *ip);
  void icuDisableNotifications(void *ip);
  /* Regular functions.*/
  void icuInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_icu_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_icu_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_icu_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_icu_driver_c *icuObjectInit(hal_icu_driver_c *self) {
  extern const struct hal_icu_driver_vmt __hal_icu_driver_vmt;

  return __icu_objinit_impl(self, &__hal_icu_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_icu_driver_c
 * @{
 */
/**
 * @brief       Enables ICU notifications for all event kinds.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void icuEnableNotificationsI(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  icuEnableEventsI(self, ICU_EVENT_ALL);
}

/**
 * @brief       Disables ICU notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void icuDisableNotificationsI(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  icuDisableEventsI(self, ICU_EVENT_ALL);
}

/**
 * @brief       Checks if ICU notifications are enabled.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @return                      The notifications status.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline bool icuAreNotificationsEnabledX(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  return icu_lld_are_notifications_enabled(self);
}

/**
 * @brief       Returns the latest captured pulse width.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @return                      The number of ticks.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline icucnt_t icuGetWidthX(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  return self->width;
}

/**
 * @brief       Returns the latest captured cycle period.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @return                      The number of ticks.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline icucnt_t icuGetPeriodX(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  return self->period;
}

/**
 * @brief       Returns the cached ICU event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @return                      The current cached event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline icu_events_t icuGetEventsX(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  return self->events;
}

/**
 * @brief       Gets and clears cached ICU event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @param[in]     mask          Mask of events to be returned and cleared.
 * @return                      The selected pending event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline icu_events_t icuGetAndClearEventsX(void *ip, icu_events_t mask) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  icu_events_t events;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  events = self->events & mask;
  self->events &= ~mask;
  osalSysRestoreStatusX(sts);

  return events;
}
/** @} */

#endif /* HAL_USE_ICU == TRUE */

#endif /* HAL_ICU_H */

/** @} */
