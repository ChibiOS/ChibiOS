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
 * @file        hal_gpt.c
 * @brief       Generated GPT Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_GPT
 * @{
 */

#include "hal.h"

#if (HAL_USE_GPT == TRUE) || defined(__DOXYGEN__)

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
 * @brief       GPT Driver initialization.
 *
 * @init
 */
void gptInit(void) {
  gpt_lld_init();
}

/*===========================================================================*/
/* Module class "hal_gpt_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_gpt_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_gpt_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__gpt_objinit_impl(void *ip, const void *vmt) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* No initialization code.*/

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance to be
 *                              disposed.
 */
void __gpt_dispose_impl(void *ip) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __gpt_start_impl(void *ip, const void *config) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  msg_t msg;

  if (config != NULL) {
    self->config = __gpt_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  msg = gpt_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 */
void __gpt_stop_impl(void *ip) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  gpt_lld_stop(self);
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__gpt_setcfg_impl(void *ip, const void *config) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  return (const void *)gpt_lld_setcfg(self,
                                      (const hal_gpt_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__gpt_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  return (const void *)gpt_lld_selcfg(self, cfgnum);
}

/**
 * @brief       Override of method @p __cbdrv_on_cb_set().
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __gpt_oncbset_impl(void *ip, drv_cb_t cb) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  gpt_lld_set_callback(self, cb);
}
/** @} */

/**
 * @brief       VMT structure of GPT driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_gpt_driver_vmt __hal_gpt_driver_vmt = {
  .dispose                  = __gpt_dispose_impl,
  .start                    = __gpt_start_impl,
  .stop                     = __gpt_stop_impl,
  .setcfg                   = __gpt_setcfg_impl,
  .selcfg                   = __gpt_selcfg_impl,
  .oncbset                  = __gpt_oncbset_impl
};

/**
 * @name        Regular methods of hal_gpt_driver_c
 * @{
 */
/**
 * @brief       Changes the interval of GPT peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     interval      New cycle time in timer ticks.
 *
 * @iclass
 */
void gptChangeIntervalI(void *ip, gptcnt_t interval) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == GPT_CONTINUOUS, "invalid state");

  gpt_lld_change_interval(self, interval);
}

/**
 * @brief       Changes the interval of GPT peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     interval      New cycle time in timer ticks.
 *
 * @api
 */
void gptChangeInterval(void *ip, gptcnt_t interval) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  gptChangeIntervalI(self, interval);
  osalSysUnlock();
}

/**
 * @brief       Starts the timer in continuous mode.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     interval      Period in ticks.
 *
 * @iclass
 */
void gptStartContinuousI(void *ip, gptcnt_t interval) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = GPT_CONTINUOUS;
  gpt_lld_start_timer(self, interval);
}

/**
 * @brief       Starts the timer in continuous mode.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     interval      Period in ticks.
 *
 * @api
 */
void gptStartContinuous(void *ip, gptcnt_t interval) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  gptStartContinuousI(self, interval);
  osalSysUnlock();
}

/**
 * @brief       Starts the timer in one-shot mode.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     interval      Time interval in ticks.
 *
 * @iclass
 */
void gptStartOneShotI(void *ip, gptcnt_t interval) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(drvGetCallbackX(self) != NULL, "callback not set");
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = GPT_ONESHOT;
  gpt_lld_start_timer(self, interval);
}

/**
 * @brief       Starts the timer in one-shot mode.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     interval      Time interval in ticks.
 *
 * @api
 */
void gptStartOneShot(void *ip, gptcnt_t interval) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  gptStartOneShotI(self, interval);
  osalSysUnlock();
}

/**
 * @brief       Stops the timer.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 *
 * @iclass
 */
void gptStopTimerI(void *ip) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == GPT_CONTINUOUS) ||
                (self->state == GPT_ONESHOT) ||
                (self->state == HAL_DRV_STATE_COMPLETE),
                "invalid state");

  self->state = HAL_DRV_STATE_READY;
  gpt_lld_stop_timer(self);
}

/**
 * @brief       Stops the timer.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 *
 * @api
 */
void gptStopTimer(void *ip) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  gptStopTimerI(self);
  osalSysUnlock();
}

/**
 * @brief       Starts the timer in one-shot mode and waits for completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_gpt_driver_c instance.
 * @param[in]     interval      Time interval in ticks.
 *
 * @api
 */
void gptPolledDelay(void *ip, gptcnt_t interval) {
  hal_gpt_driver_c *self = (hal_gpt_driver_c *)ip;
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  gpt_lld_polled_delay(self, interval);
}
/** @} */

#endif /* HAL_USE_GPT == TRUE */

/** @} */
