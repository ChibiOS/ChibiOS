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
 * @file        hal_adc.c
 * @brief       ADC Driver source.
 *
 * @addtogroup  HAL_ADC
 * @{
 */

#include "hal.h"

#if (HAL_USE_ADC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

const struct hal_adc_driver_vmt __hal_adc_driver_vmt = {
  .dispose                  = __adc_dispose_impl,
  .start                    = __adc_start_impl,
  .stop                     = __adc_stop_impl,
  .setcfg                   = __adc_setcfg_impl,
  .selcfg                   = __adc_selcfg_impl,
  .setcb                    = __adc_setcb_impl
};

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void adcInit(void) {

  adc_lld_init();
}

void *__adc_objinit_impl(void *ip, const void *vmt) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  __cbdrv_objinit_impl(self, vmt);
  self->samples = NULL;
  self->depth   = 0U;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;
#if ADC_USE_WAIT == TRUE
  self->thread  = NULL;
#endif

  return self;
}

void __adc_dispose_impl(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  (void)self;
  __cbdrv_dispose_impl(self);
}

msg_t __adc_start_impl(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  self->samples = NULL;
  self->depth   = 0U;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;

  msg = adc_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
  }

  return msg;
}

void __adc_stop_impl(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  adc_lld_stop(self);
  self->samples = NULL;
  self->depth   = 0U;
  self->grpp    = NULL;
  self->events  = 0U;
  self->errors  = 0U;
}

const void *__adc_setcfg_impl(void *ip, const void *config) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  return (const void *)adc_lld_setcfg(self, (const hal_adc_config_t *)config);
}

const void *__adc_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  return (const void *)adc_lld_selcfg(self, cfgnum);
}

void __adc_setcb_impl(void *ip, drv_cb_t cb) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  __cbdrv_setcb_impl(self, cb);
  adc_lld_set_callback(self, cb);
}

msg_t adcStart(void *ip, const hal_adc_config_t *config) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  if (config != NULL) {
    msg = drvSetCfgX(self, config);
    if (msg != HAL_RET_SUCCESS) {
      return msg;
    }
  }

  return drvStart(self);
}

void adcStop(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  osalDbgCheck(self != NULL);

  drvStop(self);
}

msg_t adcStartConversionI(void *ip, const adc_conversion_group_t *grpp,
                          adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (grpp != NULL) && (samples != NULL) &&
               (depth > 0U) && ((depth == 1U) || ((depth & 1U) == 0U)));
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ERROR),
                "not ready");

  self->samples = samples;
  self->depth   = depth;
  self->grpp    = grpp;
  self->events  = 0U;
  self->errors  = 0U;
  self->state   = HAL_DRV_STATE_ACTIVE;
  msg = adc_lld_start_conversion(self, grpp, samples, depth);
  if (msg != HAL_RET_SUCCESS) {
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
  }

  return msg;
}

msg_t adcStartConversion(void *ip, const adc_conversion_group_t *grpp,
                         adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = adcStartConversionI(self, grpp, samples, depth);
  osalSysUnlock();

  return msg;
}

void adcStopConversionI(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_HALF) ||
                (self->state == HAL_DRV_STATE_FULL) ||
                (self->state == HAL_DRV_STATE_COMPLETE) ||
                (self->state == HAL_DRV_STATE_ERROR),
                "invalid state");

  if ((self->state != HAL_DRV_STATE_READY) &&
      (self->state != HAL_DRV_STATE_STOP)) {
    adc_lld_stop_conversion(self);
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
    _adc_reset_i(self);
  }
}

void adcStopConversion(void *ip) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_HALF) ||
                (self->state == HAL_DRV_STATE_FULL) ||
                (self->state == HAL_DRV_STATE_COMPLETE) ||
                (self->state == HAL_DRV_STATE_ERROR),
                "invalid state");
  if ((self->state != HAL_DRV_STATE_READY) &&
      (self->state != HAL_DRV_STATE_STOP)) {
    adc_lld_stop_conversion(self);
    self->samples = NULL;
    self->depth   = 0U;
    self->grpp    = NULL;
    self->state   = HAL_DRV_STATE_READY;
    _adc_reset_s(self);
  }
  osalSysUnlock();
}

#if (ADC_USE_WAIT == TRUE) || defined(__DOXYGEN__)
msg_t adcConvert(void *ip, const adc_conversion_group_t *grpp,
                 adcsample_t *samples, size_t depth) {
  hal_adc_driver_c *self = (hal_adc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->thread == NULL, "already waiting");
  msg = adcStartConversionI(self, grpp, samples, depth);
  if (msg == HAL_RET_SUCCESS) {
    msg = osalThreadSuspendS(&self->thread);
  }
  osalSysUnlock();

  return msg;
}
#endif

#endif /* HAL_USE_ADC == TRUE */

/** @} */
