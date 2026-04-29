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
 * @file        hal_icu.c
 * @brief       Generated ICU Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_ICU
 * @{
 */

#include "hal.h"

#if (HAL_USE_ICU == TRUE) || defined(__DOXYGEN__)

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
 * @brief       ICU Driver initialization.
 *
 * @init
 */
void icuInit(void) {
  icu_lld_init();
}

/*===========================================================================*/
/* Module class "hal_icu_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_icu_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_icu_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__icu_objinit_impl(void *ip, const void *vmt) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->width          = 0U;
  self->period         = 0U;
  self->enabled_events = 0U;
  self->events         = 0U;

#if defined(ICU_DRIVER_EXT_INIT_HOOK)
  ICU_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance to be
 *                              disposed.
 */
void __icu_dispose_impl(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __icu_start_impl(void *ip, const void *config) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  msg_t msg;

  if (config != NULL) {
    self->config = __icu_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  msg = icu_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
    self->width = 0U;
    self->period = 0U;
    self->enabled_events = 0U;
    self->events = 0U;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 */
void __icu_stop_impl(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  icu_lld_stop(self);
  self->width = 0U;
  self->period = 0U;
  self->enabled_events = 0U;
  self->events = 0U;
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__icu_setcfg_impl(void *ip, const void *config) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  return (const void *)icu_lld_setcfg(self,
                                      (const hal_icu_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__icu_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  return (const void *)icu_lld_selcfg(self, cfgnum);
}

/**
 * @brief       Override of method @p __cbdrv_on_cb_set().
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __icu_oncbset_impl(void *ip, drv_cb_t cb) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  icu_lld_set_callback(self, cb);
}
/** @} */

/**
 * @brief       VMT structure of ICU driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_icu_driver_vmt __hal_icu_driver_vmt = {
  .dispose                  = __icu_dispose_impl,
  .start                    = __icu_start_impl,
  .stop                     = __icu_stop_impl,
  .setcfg                   = __icu_setcfg_impl,
  .selcfg                   = __icu_selcfg_impl,
  .oncbset                  = __icu_oncbset_impl
};

/**
 * @name        Regular methods of hal_icu_driver_c
 * @{
 */
/**
 * @brief       Starts the input capture.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 *
 * @iclass
 */
void icuStartCaptureI(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->width  = 0U;
  self->period = 0U;
  self->events = 0U;
  self->state  = ICU_WAITING;
  icu_lld_start_capture(self);
}

/**
 * @brief       Starts the input capture.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 *
 * @api
 */
void icuStartCapture(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  icuStartCaptureI(self);
  osalSysUnlock();
}

/**
 * @brief       Waits for a completed capture.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @return                      The capture status.
 * @retval false                If the capture is successful.
 * @retval true                 If a timer overflow occurred.
 *
 * @api
 */
bool icuWaitCapture(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  bool status;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert((self->state == ICU_WAITING) ||
                (self->state == ICU_ACTIVE),
                "invalid state");
  osalDbgAssert(icuAreNotificationsEnabledX(self) == false,
                "notifications enabled");

  status = icu_lld_wait_capture(self);
  if (status) {
    self->state = ICU_WAITING;
  }
  else {
    self->width  = icu_lld_get_width(self);
    self->period = icu_lld_get_period(self);
    self->state  = ICU_ACTIVE;
  }
  osalSysUnlock();

  return status;
}

/**
 * @brief       Stops the input capture.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 *
 * @iclass
 */
void icuStopCaptureI(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == ICU_WAITING) ||
                (self->state == ICU_ACTIVE),
                "invalid state");

  icu_lld_stop_capture(self);
  self->enabled_events = 0U;
  self->state = HAL_DRV_STATE_READY;
}

/**
 * @brief       Stops the input capture.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 *
 * @api
 */
void icuStopCapture(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  icuStopCaptureI(self);
  osalSysUnlock();
}

/**
 * @brief       Enables ICU event notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @param[in]     events        Event mask to be enabled.
 *
 * @iclass
 */
void icuEnableEventsI(void *ip, icu_events_t events) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == ICU_WAITING) ||
                (self->state == ICU_ACTIVE),
                "invalid state");
  osalDbgAssert(drvGetCallbackX(self) != NULL, "undefined callback");

  events &= ICU_EVENT_ALL;
  self->enabled_events |= events;
  icu_lld_enable_events(self, events);
}

/**
 * @brief       Enables ICU event notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @param[in]     events        Event mask to be enabled.
 *
 * @api
 */
void icuEnableEvents(void *ip, icu_events_t events) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  icuEnableEventsI(self, events);
  osalSysUnlock();
}

/**
 * @brief       Disables ICU event notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @param[in]     events        Event mask to be disabled.
 *
 * @iclass
 */
void icuDisableEventsI(void *ip, icu_events_t events) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == ICU_WAITING) ||
                (self->state == ICU_ACTIVE),
                "invalid state");

  events &= ICU_EVENT_ALL;
  self->enabled_events &= ~events;
  icu_lld_disable_events(self, events);
}

/**
 * @brief       Disables ICU event notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 * @param[in]     events        Event mask to be disabled.
 *
 * @api
 */
void icuDisableEvents(void *ip, icu_events_t events) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  icuDisableEventsI(self, events);
  osalSysUnlock();
}

/**
 * @brief       Enables ICU notifications for all event kinds.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 *
 * @api
 */
void icuEnableNotifications(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  icuEnableEvents(self, ICU_EVENT_ALL);
}

/**
 * @brief       Disables ICU notifications.
 *
 * @param[in,out] ip            Pointer to a @p hal_icu_driver_c instance.
 *
 * @api
 */
void icuDisableNotifications(void *ip) {
  hal_icu_driver_c *self = (hal_icu_driver_c *)ip;
  icuDisableEvents(self, ICU_EVENT_ALL);
}
/** @} */

#endif /* HAL_USE_ICU == TRUE */

/** @} */
