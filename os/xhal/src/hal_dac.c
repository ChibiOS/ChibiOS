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
 * @file        hal_dac.c
 * @brief       Generated DAC Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_DAC
 * @{
 */

#include "hal.h"

#if (HAL_USE_DAC == TRUE) || defined(__DOXYGEN__)

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
 * @brief       DAC Driver initialization.
 *
 * @init
 */
void dacInit(void) {
  dac_lld_init();
}

/*===========================================================================*/
/* Module class "hal_dac_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_dac_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_dac_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__dac_objinit_impl(void *ip, const void *vmt) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->samples = NULL;
  self->depth   = 0U;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;
#if DAC_USE_SYNCHRONIZATION == TRUE
  self->thread     = NULL;
  self->sync_state = HAL_DRV_STATE_STOP;
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance to be
 *                              disposed.
 */
void __dac_dispose_impl(void *ip) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __dac_start_impl(void *ip, const void *config) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  msg_t msg;

  if (config != NULL) {
    self->config = __dac_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  self->samples = NULL;
  self->depth   = 0U;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;

  msg = dac_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 */
void __dac_stop_impl(void *ip) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  dac_lld_stop(self);
  self->samples = NULL;
  self->depth   = 0U;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__dac_setcfg_impl(void *ip, const void *config) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  return (const void *)dac_lld_setcfg(self,
                                      (const hal_dac_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__dac_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  return (const void *)dac_lld_selcfg(self, cfgnum);
}

/**
 * @brief       Override of method @p __cbdrv_on_cb_set().
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __dac_oncbset_impl(void *ip, drv_cb_t cb) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  dac_lld_set_callback(self, cb);
}
/** @} */

/**
 * @brief       VMT structure of DAC driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_dac_driver_vmt __hal_dac_driver_vmt = {
  .dispose                  = __dac_dispose_impl,
  .start                    = __dac_start_impl,
  .stop                     = __dac_stop_impl,
  .setcfg                   = __dac_setcfg_impl,
  .selcfg                   = __dac_selcfg_impl,
  .oncbset                  = __dac_oncbset_impl
};

/**
 * @name        Regular methods of hal_dac_driver_c
 * @{
 */
/**
 * @brief       Outputs a value directly on a DAC channel.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     channel       DAC channel number.
 * @param[in]     sample        Value to be output.
 *
 * @xclass
 */
msg_t dacPutChannelX(void *ip, dacchannel_t channel, dacsample_t sample) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  osalDbgCheck(channel < (dacchannel_t)DAC_MAX_CHANNELS);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY)  ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_HALF)   ||
                (self->state == HAL_DRV_STATE_FULL),
                "invalid state");

  return dac_lld_put_channel(self, channel, sample);
}

/**
 * @brief       Starts a DAC circular streaming conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     grpp          Conversion group.
 * @param[in]     samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t dacStartConversionI(void *ip, const dac_conversion_group_t *grpp,
                          dacsample_t *samples, size_t depth) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (grpp != NULL) && (samples != NULL) &&
               (depth > 0U) && ((depth == 1U) || ((depth & 1U) == 0U)));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->samples = samples;
  self->depth   = depth;
  self->grpp    = grpp;
  self->events  = 0U;
  self->errors  = 0U;
  self->state   = HAL_DRV_STATE_ACTIVE;
  msg = dac_lld_start_conversion(self);
  if (msg != HAL_RET_SUCCESS) {
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
  }

  return msg;
}

/**
 * @brief       Starts a DAC circular streaming conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     grpp          Conversion group.
 * @param[in]     samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The operation status.
 *
 * @api
 */
msg_t dacStartConversion(void *ip, const dac_conversion_group_t *grpp,
                         dacsample_t *samples, size_t depth) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = dacStartConversionI(self, grpp, samples, depth);
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Stops an ongoing DAC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 *
 * @iclass
 */
void dacStopConversionI(void *ip) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY)  ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_HALF)   ||
                (self->state == HAL_DRV_STATE_FULL)   ||
                (self->state == HAL_DRV_STATE_ERROR),
                "invalid state");

  if (self->state != HAL_DRV_STATE_READY) {
    dac_lld_stop_conversion(self);
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
    _dac_reset_i(self);
  }
}

/**
 * @brief       Stops an ongoing DAC conversion.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 *
 * @api
 */
void dacStopConversion(void *ip) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert((self->state == HAL_DRV_STATE_READY)  ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_HALF)   ||
                (self->state == HAL_DRV_STATE_FULL)   ||
                (self->state == HAL_DRV_STATE_ERROR),
                "invalid state");
  if (self->state != HAL_DRV_STATE_READY) {
    dac_lld_stop_conversion(self);
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
    _dac_reset_s(self);
  }
  osalSysUnlock();
}

#if (DAC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Starts a DAC circular conversion and waits for one full cycle.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     grpp          Conversion group.
 * @param[in]     samples       Samples buffer.
 * @param[in]     depth         Buffer depth.
 * @return                      The operation result.
 *
 * @api
 */
msg_t dacConvert(void *ip, const dac_conversion_group_t *grpp,
                 dacsample_t *samples, size_t depth) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->thread == NULL, "already waiting");
  msg = dacStartConversionI(self, grpp, samples, depth);
  if (msg == HAL_RET_SUCCESS) {
    msg = dacSynchronizeStateS(self, HAL_DRV_STATE_FULL, TIME_INFINITE);
  }
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Synchronizes with a DAC streaming state.
 * @details     This function synchronizes with a future occurrence of the
 *              specified streaming state. State occurrences are not buffered,
 *              the waiting thread must already be waiting when the state is
 *              reached.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     state         State to synchronize with.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 * @retval MSG_OK               If the requested state has been reached.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If conversion stopped or failed while waiting.
 *
 * @sclass
 */
msg_t dacSynchronizeStateS(void *ip, driver_state_t state,
                           sysinterval_t timeout) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
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
 * @brief       Synchronizes with a DAC streaming state.
 * @details     This function synchronizes with a future occurrence of the
 *              specified streaming state. State occurrences are not buffered,
 *              the waiting thread must already be waiting when the state is
 *              reached.
 *
 * @param[in,out] ip            Pointer to a @p hal_dac_driver_c instance.
 * @param[in]     state         State to synchronize with.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 * @retval MSG_OK               If the requested state has been reached.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If conversion stopped or failed while waiting.
 *
 * @api
 */
msg_t dacSynchronizeState(void *ip, driver_state_t state,
                          sysinterval_t timeout) {
  hal_dac_driver_c *self = (hal_dac_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();
  msg = dacSynchronizeStateS(self, state, timeout);
  osalSysUnlock();

  return msg;
}
#endif /* DAC_USE_SYNCHRONIZATION == TRUE */
/** @} */

#endif /* HAL_USE_DAC == TRUE */

/** @} */
