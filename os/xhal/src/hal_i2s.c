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
 * @file        hal_i2s.c
 * @brief       Generated I2S Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_I2S
 * @{
 */

#include "hal.h"

#if (HAL_USE_I2S == TRUE) || defined(__DOXYGEN__)

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
 * @brief       I2S Driver initialization.
 *
 * @init
 */
void i2sInit(void) {
  i2s_lld_init();
}

/*===========================================================================*/
/* Module class "hal_i2s_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_i2s_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_i2s_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__i2s_objinit_impl(void *ip, const void *vmt) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->events = 0U;
  self->errors = I2S_NO_ERROR;
#if I2S_USE_SYNCHRONIZATION == TRUE
  self->thread     = NULL;
  self->sync_state = HAL_DRV_STATE_STOP;
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance to be
 *                              disposed.
 */
void __i2s_dispose_impl(void *ip) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __i2s_start_impl(void *ip, const void *config) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  if (config != NULL) {
    self->config = __i2s_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  if (self->config == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

  self->events = 0U;
  self->errors = I2S_NO_ERROR;

  return i2s_lld_start(self);
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 */
void __i2s_stop_impl(void *ip) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  i2s_lld_stop(self);
  self->events = 0U;
  self->errors = I2S_NO_ERROR;
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__i2s_setcfg_impl(void *ip, const void *config) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  return (const void *)i2s_lld_setcfg(self,
                                      (const hal_i2s_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__i2s_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  return (const void *)i2s_lld_selcfg(self, cfgnum);
}
/** @} */

/**
 * @brief       VMT structure of I2S driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_i2s_driver_vmt __hal_i2s_driver_vmt = {
  .dispose                  = __i2s_dispose_impl,
  .start                    = __i2s_start_impl,
  .stop                     = __i2s_stop_impl,
  .setcfg                   = __i2s_setcfg_impl,
  .selcfg                   = __i2s_selcfg_impl,
  .oncbset                  = __cbdrv_oncbset_impl
};

/**
 * @name        Regular methods of hal_i2s_driver_c
 * @{
 */
/**
 * @brief       Starts an I2S data exchange.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t i2sStartExchangeI(void *ip) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");
  osalDbgAssert(self->config != NULL, "config missing");

  self->events = 0U;
  self->errors = I2S_NO_ERROR;
  self->state  = HAL_DRV_STATE_ACTIVE;
  msg = i2s_lld_start_exchange(self);
  if (msg != HAL_RET_SUCCESS) {
    self->state = HAL_DRV_STATE_READY;
  }

  return msg;
}

/**
 * @brief       Starts an I2S data exchange.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @return                      The operation status.
 *
 * @api
 */
msg_t i2sStartExchange(void *ip) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = i2sStartExchangeI(self);
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Stops the ongoing I2S data exchange.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 *
 * @iclass
 */
void i2sStopExchangeI(void *ip) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY)  ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_HALF)   ||
                (self->state == HAL_DRV_STATE_FULL)   ||
                (self->state == HAL_DRV_STATE_ERROR),
                "invalid state");

  if (self->state != HAL_DRV_STATE_READY) {
    i2s_lld_stop_exchange(self);
    self->state = HAL_DRV_STATE_READY;
    _i2s_reset_i(self);
  }
}

/**
 * @brief       Stops the ongoing I2S data exchange.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 *
 * @api
 */
void i2sStopExchange(void *ip) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert((self->state == HAL_DRV_STATE_READY)  ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_HALF)   ||
                (self->state == HAL_DRV_STATE_FULL)   ||
                (self->state == HAL_DRV_STATE_ERROR),
                "invalid state");
  if (self->state != HAL_DRV_STATE_READY) {
    i2s_lld_stop_exchange(self);
    self->state = HAL_DRV_STATE_READY;
    _i2s_reset_s(self);
  }
  osalSysUnlock();
}

#if (I2S_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Synchronizes with an I2S exchange state.
 * @details     This function synchronizes with a future occurrence of the
 *              specified exchange state. State occurrences are not buffered,
 *              the waiting thread must already be waiting when the state is
 *              reached.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @param[in]     state         State to synchronize with.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 * @retval MSG_OK               If the requested state has been reached.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If exchange stopped or failed while waiting.
 *
 * @sclass
 */
msg_t i2sSynchronizeStateS(void *ip, driver_state_t state,
                           sysinterval_t timeout) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);
  osalDbgCheckClassS();
  osalDbgCheck((state == HAL_DRV_STATE_HALF) ||
               (state == HAL_DRV_STATE_FULL));
  osalDbgAssert(self->state == HAL_DRV_STATE_ACTIVE, "invalid state");
  osalDbgAssert(self->thread == NULL, "already waiting");

  self->sync_state = state;
  msg = osalThreadSuspendTimeoutS(&self->thread, timeout);
  self->sync_state = HAL_DRV_STATE_STOP;

  return msg;
}

/**
 * @brief       Synchronizes with an I2S exchange state.
 * @details     This function synchronizes with a future occurrence of the
 *              specified exchange state. State occurrences are not buffered,
 *              the waiting thread must already be waiting when the state is
 *              reached.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2s_driver_c instance.
 * @param[in]     state         State to synchronize with.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 * @retval MSG_OK               If the requested state has been reached.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If exchange stopped or failed while waiting.
 *
 * @api
 */
msg_t i2sSynchronizeState(void *ip, driver_state_t state,
                          sysinterval_t timeout) {
  hal_i2s_driver_c *self = (hal_i2s_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();
  msg = i2sSynchronizeStateS(self, state, timeout);
  osalSysUnlock();

  return msg;
}
#endif /* I2S_USE_SYNCHRONIZATION == TRUE */
/** @} */

#endif /* HAL_USE_I2S == TRUE */

/** @} */
