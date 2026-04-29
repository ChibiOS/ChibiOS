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
 * @file        hal_pwm.h
 * @brief       Generated PWM Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_PWM
 * @brief       PWM Driver macros and structures.
 * @{
 */

#ifndef HAL_PWM_H
#define HAL_PWM_H

#include "hal_cb_driver.h"

#if (HAL_USE_PWM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    PWM output mode macros
 * @{
 */
/**
 * @brief       Standard output modes mask.
 */
#define PWM_OUTPUT_MASK                     0x0FU

/**
 * @brief       Output not driven.
 */
#define PWM_OUTPUT_DISABLED                 0x00U

/**
 * @brief       Positive PWM logic, active level is logic one.
 */
#define PWM_OUTPUT_ACTIVE_HIGH              0x01U

/**
 * @brief       Inverse PWM logic, active level is logic zero.
 */
#define PWM_OUTPUT_ACTIVE_LOW               0x02U
/** @} */

/**
 * @name    PWM event flags
 * @{
 */
/**
 * @brief       PWM period event flag.
 */
#define PWM_EVENT_PERIOD                    (1U << 0)

/**
 * @brief       All PWM channel event flags supported by notifications.
 */
#define PWM_EVENT_ALL_CHANNELS              ((pwm_events_t)0x0000001EU)

/**
 * @brief       All PWM event flags.
 */
#define PWM_EVENT_ALL                       (PWM_EVENT_PERIOD | PWM_EVENT_ALL_CHANNELS)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Support for PWM user configurations.
 * @note        When enabled the user must provide a variable named @p
 *              pwm_configurations of type @p pwm_configurations_t.
 */
#if !defined(PWM_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define PWM_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on PWM_USE_CONFIGURATIONS configuration.*/
#if (PWM_USE_CONFIGURATIONS != FALSE) && (PWM_USE_CONFIGURATIONS != TRUE)
#error "invalid PWM_USE_CONFIGURATIONS value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    PWM duty cycle conversion
 * @{
 */
/**
 * @brief       Returns the event flag associated to a PWM channel.
 *
 * @param[in]     n             PWM channel index.
 * @return                      The event flag associated to the specified
 *                              channel.
 *
 * @api
 */
#define PWM_EVENT_CHANNEL(n)                                                \
  (pwm_events_t)(1U << ((unsigned)(n) + 1U))

/**
 * @brief       Converts from fraction to pulse width.
 *
 * @param[in]     pwmp          Pointer to the PWM driver instance.
 * @param[in]     denominator   Denominator of the fraction.
 * @param[in]     numerator     Numerator of the fraction.
 * @return                      The pulse width to be passed to @p
 *                              pwmEnableChannel().
 *
 * @api
 */
#define PWM_FRACTION_TO_WIDTH(pwmp, denominator, numerator)                 \
  ((pwmcnt_t)((((pwmcnt_t)(pwmp)->period) *                                 \
               (pwmcnt_t)(numerator)) / (pwmcnt_t)(denominator)))

/**
 * @brief       Converts from degrees to pulse width.
 *
 * @param[in]     pwmp          Pointer to the PWM driver instance.
 * @param[in]     degrees       Degrees as an integer between 0 and 36000.
 * @return                      The pulse width to be passed to @p
 *                              pwmEnableChannel().
 *
 * @api
 */
#define PWM_DEGREES_TO_WIDTH(pwmp, degrees)                                 \
  PWM_FRACTION_TO_WIDTH(pwmp, 36000, degrees)

/**
 * @brief       Converts from percentage to pulse width.
 *
 * @param[in]     pwmp          Pointer to the PWM driver instance.
 * @param[in]     percentage    Percentage as an integer between 0 and 10000.
 * @return                      The pulse width to be passed to @p
 *                              pwmEnableChannel().
 *
 * @api
 */
#define PWM_PERCENTAGE_TO_WIDTH(pwmp, percentage)                           \
  PWM_FRACTION_TO_WIDTH(pwmp, 10000, percentage)
/** @} */

/**
 * @name    Low level driver helper macros
 * @{
 */
/**
 * @brief       Common ISR code, PWM event.
 *
 * @param[in,out] pwmp          Pointer to the PWM driver instance.
 * @param[in]     events        PWM event flags.
 *
 * @notapi
 */
#define _pwm_isr_invoke_cb(pwmp, events)                                    \
  do {                                                                      \
    (pwmp)->events |= (events);                                             \
    __cbdrv_invoke_cb(pwmp);                                                \
  } while (false)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of a PWM mode.
 */
typedef uint32_t pwmmode_t;

/**
 * @brief       Type of a PWM channel.
 */
typedef uint8_t pwmchannel_t;

/**
 * @brief       Type of a PWM channels mask.
 */
typedef uint32_t pwmchnmsk_t;

/**
 * @brief       Type of a PWM counter.
 */
typedef uint32_t pwmcnt_t;

/**
 * @brief       Type of PWM event flags.
 */
typedef uint32_t pwm_events_t;

/**
 * @brief       Type of PWM driver state.
 */
typedef driver_state_t pwmstate_t;

/**
 * @brief       Type of structure representing a PWM driver.
 */
typedef struct hal_pwm_driver hal_pwm_driver_c;

/**
 * @brief       Type of structure representing a PWM configuration.
 */
typedef struct hal_pwm_config hal_pwm_config_t;

/**
 * @brief       Type of structure representing a PWM channel configuration.
 */
typedef struct hal_pwm_channel_config hal_pwm_channel_config_t;

/**
 * @brief       Type of structure representing a PWM driver (legacy).
 */
typedef struct hal_pwm_driver PWMDriver;

/**
 * @brief       Type of structure representing a PWM configuration (legacy).
 */
typedef struct hal_pwm_config PWMConfig;

/**
 * @brief       Type of structure representing a PWM channel configuration
 *              (legacy).
 */
typedef struct hal_pwm_channel_config PWMChannelConfig;

/* Inclusion of LLD header.*/
#include "hal_pwm_lld.h"

/**
 * @brief       PWM driver channel configuration structure.
 */
struct hal_pwm_channel_config {
  /**
   * @brief       Channel active logic level.
   */
  pwmmode_t                 mode;
#if (defined(PWM_CHANNEL_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  PWM_CHANNEL_CONFIG_EXT_FIELDS
#endif /* defined(PWM_CHANNEL_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Driver configuration structure.
 */
struct hal_pwm_config {
  /**
   * @brief       Timer clock in Hz.
   */
  uint32_t                  frequency;
  /**
   * @brief       PWM period in ticks.
   */
  pwmcnt_t                  period;
  /**
   * @brief       Initially enabled PWM event flags.
   */
  pwm_events_t              enabled_events;
  /**
   * @brief       Channels configurations.
   */
  hal_pwm_channel_config_t  channels[PWM_CHANNELS];
  /* End of the mandatory fields.*/
  pwm_lld_config_fields;
#if (defined(PWM_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  PWM_CONFIG_EXT_FIELDS
#endif /* defined(PWM_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Type of user-provided PWM configurations.
 */
typedef struct pwm_configurations pwm_configurations_t;

/**
 * @brief       Structure representing user-provided PWM configurations.
 */
struct pwm_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User PWM configurations.
   */
  hal_pwm_config_t          cfgs[];
};

/**
 * @class       hal_pwm_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of a PWM driver.
 *
 * @name        Class @p hal_pwm_driver_c structures
 * @{
 */

/**
 * @brief       Type of a PWM driver class.
 */
typedef struct hal_pwm_driver hal_pwm_driver_c;

/**
 * @brief       Class @p hal_pwm_driver_c virtual methods table.
 */
struct hal_pwm_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*oncbset)(void *ip, drv_cb_t cb);
  /* From hal_pwm_driver_c.*/
};

/**
 * @brief       Structure representing a PWM driver class.
 */
struct hal_pwm_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_pwm_driver_vmt *vmt;
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
   * @brief       Current PWM period in ticks.
   */
  pwmcnt_t                  period;
  /**
   * @brief       Mask of the enabled channels.
   */
  pwmchnmsk_t               enabled;
  /**
   * @brief       Mask of the enabled PWM events.
   */
  pwm_events_t              enabled_events;
  /**
   * @brief       Cached PWM event flags.
   */
  volatile pwm_events_t     events;
  /**
   * @brief       Number of channels in this instance.
   */
  pwmchannel_t              channels;
#if (defined(PWM_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  PWM_DRIVER_EXT_FIELDS
#endif /* defined(PWM_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  pwm_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_pwm_driver_c.*/
  void *__pwm_objinit_impl(void *ip, const void *vmt);
  void __pwm_dispose_impl(void *ip);
  msg_t __pwm_start_impl(void *ip, const void *config);
  void __pwm_stop_impl(void *ip);
  const void *__pwm_setcfg_impl(void *ip, const void *config);
  const void *__pwm_selcfg_impl(void *ip, unsigned cfgnum);
  void __pwm_oncbset_impl(void *ip, drv_cb_t cb);
  void pwmChangePeriodI(void *ip, pwmcnt_t period);
  void pwmChangePeriod(void *ip, pwmcnt_t period);
  void pwmEnableChannelI(void *ip, pwmchannel_t channel, pwmcnt_t width);
  void pwmEnableChannel(void *ip, pwmchannel_t channel, pwmcnt_t width);
  void pwmDisableChannelI(void *ip, pwmchannel_t channel);
  void pwmDisableChannel(void *ip, pwmchannel_t channel);
  void pwmEnableEventsI(void *ip, pwm_events_t events);
  void pwmEnableEvents(void *ip, pwm_events_t events);
  void pwmDisableEventsI(void *ip, pwm_events_t events);
  void pwmDisableEvents(void *ip, pwm_events_t events);
  void pwmEnablePeriodicNotification(void *ip);
  void pwmDisablePeriodicNotification(void *ip);
  void pwmEnableChannelNotification(void *ip, pwmchannel_t channel);
  void pwmDisableChannelNotification(void *ip, pwmchannel_t channel);
  /* Regular functions.*/
  void pwmInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_pwm_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_pwm_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_pwm_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_pwm_driver_c *pwmObjectInit(hal_pwm_driver_c *self) {
  extern const struct hal_pwm_driver_vmt __hal_pwm_driver_vmt;

  return __pwm_objinit_impl(self, &__hal_pwm_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_pwm_driver_c
 * @{
 */
/**
 * @brief       Returns a PWM channel status.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     channel       PWM channel identifier.
 * @return                      The channel status.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline bool pwmIsChannelEnabledI(void *ip, pwmchannel_t channel) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  return (bool)((self->enabled &
                 ((pwmchnmsk_t)1U << (pwmchnmsk_t)channel)) != 0U);
}

/**
 * @brief       Enables the periodic activation edge notification.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void pwmEnablePeriodicNotificationI(void *ip) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  pwmEnableEventsI(self, PWM_EVENT_PERIOD);
}

/**
 * @brief       Disables the periodic activation edge notification.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void pwmDisablePeriodicNotificationI(void *ip) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  pwmDisableEventsI(self, PWM_EVENT_PERIOD);
}

/**
 * @brief       Enables a channel de-activation edge notification.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     channel       PWM channel identifier.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void pwmEnableChannelNotificationI(void *ip,
                                                 pwmchannel_t channel) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheck(channel < self->channels);
  osalDbgAssert((self->enabled &
                 ((pwmchnmsk_t)1U << (pwmchnmsk_t)channel)) != 0U,
                "channel not enabled");
  pwmEnableEventsI(self, PWM_EVENT_CHANNEL(channel));
}

/**
 * @brief       Disables a channel de-activation edge notification.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     channel       PWM channel identifier.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void pwmDisableChannelNotificationI(void *ip,
                                                  pwmchannel_t channel) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheck(channel < self->channels);
  pwmDisableEventsI(self, PWM_EVENT_CHANNEL(channel));
}

/**
 * @brief       Returns the cached PWM event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @return                      The current cached event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline pwm_events_t pwmGetEventsX(void *ip) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  return self->events;
}

/**
 * @brief       Gets and clears cached PWM event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     mask          Mask of events to be returned and cleared.
 * @return                      The selected pending event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline pwm_events_t pwmGetAndClearEventsX(void *ip, pwm_events_t mask) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  pwm_events_t events;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  events = self->events & mask;
  self->events &= ~mask;
  osalSysRestoreStatusX(sts);

  return events;
}
/** @} */

#endif /* HAL_USE_PWM == TRUE */

#endif /* HAL_PWM_H */

/** @} */
