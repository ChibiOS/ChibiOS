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
 * @file        hal_pwm.c
 * @brief       Generated PWM Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_PWM
 * @{
 */

#include "hal.h"

#if (HAL_USE_PWM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local macros.                                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       PWM Driver initialization.
 *
 * @init
 */
void pwmInit(void) {
  pwm_lld_init();
}

/*===========================================================================*/
/* Module class "hal_pwm_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_pwm_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_pwm_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__pwm_objinit_impl(void *ip, const void *vmt) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->period         = 0U;
  self->enabled        = 0U;
  self->enabled_events = 0U;
  self->events         = 0U;
  self->channels       = 0U;

#if defined(PWM_DRIVER_EXT_INIT_HOOK)
  PWM_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance to be
 *                              disposed.
 */
void __pwm_dispose_impl(void *ip) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __pwm_start_impl(void *ip, const void *config) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  msg_t msg;

  if (config != NULL) {
    self->config = __pwm_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  msg = pwm_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
    self->period = 0U;
    self->enabled = 0U;
    self->enabled_events = 0U;
    self->events = 0U;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 */
void __pwm_stop_impl(void *ip) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  pwm_lld_stop(self);
  self->period = 0U;
  self->enabled = 0U;
  self->enabled_events = 0U;
  self->events = 0U;
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__pwm_setcfg_impl(void *ip, const void *config) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  return (const void *)pwm_lld_setcfg(self,
                                      (const hal_pwm_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__pwm_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  return (const void *)pwm_lld_selcfg(self, cfgnum);
}

/**
 * @brief       Override of method @p __cbdrv_on_cb_set().
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __pwm_oncbset_impl(void *ip, drv_cb_t cb) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  pwm_lld_set_callback(self, cb);
}
/** @} */

/**
 * @brief       VMT structure of PWM driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_pwm_driver_vmt __hal_pwm_driver_vmt = {
  .dispose                  = __pwm_dispose_impl,
  .start                    = __pwm_start_impl,
  .stop                     = __pwm_stop_impl,
  .setcfg                   = __pwm_setcfg_impl,
  .selcfg                   = __pwm_selcfg_impl,
  .oncbset                  = __pwm_oncbset_impl
};

/**
 * @name        Regular methods of hal_pwm_driver_c
 * @{
 */
/**
 * @brief       Changes the period of the PWM peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     period        New cycle time in ticks.
 *
 * @iclass
 */
void pwmChangePeriodI(void *ip, pwmcnt_t period) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->period = period;
  pwm_lld_change_period(self, period);
}

/**
 * @brief       Changes the period of the PWM peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     period        New cycle time in ticks.
 *
 * @api
 */
void pwmChangePeriod(void *ip, pwmcnt_t period) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  pwmChangePeriodI(self, period);
  osalSysUnlock();
}

/**
 * @brief       Enables a PWM channel.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     channel       PWM channel identifier.
 * @param[in]     width         PWM pulse width in timer ticks.
 *
 * @iclass
 */
void pwmEnableChannelI(void *ip, pwmchannel_t channel, pwmcnt_t width) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (channel < self->channels));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->enabled |= ((pwmchnmsk_t)1U << (pwmchnmsk_t)channel);
  pwm_lld_enable_channel(self, channel, width);
}

/**
 * @brief       Enables a PWM channel.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     channel       PWM channel identifier.
 * @param[in]     width         PWM pulse width in timer ticks.
 *
 * @api
 */
void pwmEnableChannel(void *ip, pwmchannel_t channel, pwmcnt_t width) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  pwmEnableChannelI(self, channel, width);
  osalSysUnlock();
}

/**
 * @brief       Disables a PWM channel.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     channel       PWM channel identifier.
 *
 * @iclass
 */
void pwmDisableChannelI(void *ip, pwmchannel_t channel) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (channel < self->channels));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->enabled &= ~((pwmchnmsk_t)1U << (pwmchnmsk_t)channel);
  self->enabled_events &= ~PWM_EVENT_CHANNEL(channel);
  pwm_lld_disable_channel(self, channel);
}

/**
 * @brief       Disables a PWM channel.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     channel       PWM channel identifier.
 *
 * @api
 */
void pwmDisableChannel(void *ip, pwmchannel_t channel) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  pwmDisableChannelI(self, channel);
  osalSysUnlock();
}

/**
 * @brief       Enables PWM event notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     events        Event mask to be enabled.
 *
 * @iclass
 */
void pwmEnableEventsI(void *ip, pwm_events_t events) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");
  osalDbgAssert(drvGetCallbackX(self) != NULL, "undefined callback");

  self->enabled_events |= events;
  pwm_lld_enable_events(self, events);
}

/**
 * @brief       Enables PWM event notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     events        Event mask to be enabled.
 *
 * @api
 */
void pwmEnableEvents(void *ip, pwm_events_t events) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  pwmEnableEventsI(self, events);
  osalSysUnlock();
}

/**
 * @brief       Disables PWM event notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     events        Event mask to be disabled.
 *
 * @iclass
 */
void pwmDisableEventsI(void *ip, pwm_events_t events) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->enabled_events &= ~events;
  pwm_lld_disable_events(self, events);
}

/**
 * @brief       Disables PWM event notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     events        Event mask to be disabled.
 *
 * @api
 */
void pwmDisableEvents(void *ip, pwm_events_t events) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  pwmDisableEventsI(self, events);
  osalSysUnlock();
}

/**
 * @brief       Enables the periodic activation edge notification.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 *
 * @api
 */
void pwmEnablePeriodicNotification(void *ip) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  pwmEnableEvents(self, PWM_EVENT_PERIOD);
}

/**
 * @brief       Disables the periodic activation edge notification.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 *
 * @api
 */
void pwmDisablePeriodicNotification(void *ip) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  pwmDisableEvents(self, PWM_EVENT_PERIOD);
}

/**
 * @brief       Enables a channel de-activation edge notification.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     channel       PWM channel identifier.
 *
 * @api
 */
void pwmEnableChannelNotification(void *ip, pwmchannel_t channel) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgCheck(channel < self->channels);
  osalDbgAssert((self->enabled &
                 ((pwmchnmsk_t)1U << (pwmchnmsk_t)channel)) != 0U,
                "channel not enabled");
  pwmEnableEventsI(self, PWM_EVENT_CHANNEL(channel));
  osalSysUnlock();
}

/**
 * @brief       Disables a channel de-activation edge notification.
 *
 * @param[in,out] ip            Pointer to a @p hal_pwm_driver_c instance.
 * @param[in]     channel       PWM channel identifier.
 *
 * @api
 */
void pwmDisableChannelNotification(void *ip, pwmchannel_t channel) {
  hal_pwm_driver_c *self = (hal_pwm_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgCheck(channel < self->channels);
  pwmDisableEventsI(self, PWM_EVENT_CHANNEL(channel));
  osalSysUnlock();
}
/** @} */

#endif /* HAL_USE_PWM == TRUE */

/** @} */
