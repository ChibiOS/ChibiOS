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
 * @file    sandbox/hal_adc_lld.c
 * @brief   SandBox ADC subsystem low level driver source.
 *
 * @addtogroup HAL_ADC
 * @{
 */

#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

hal_adc_driver_c ADCD1;

static const hal_adc_config_t default_config = {
  .grps = NULL
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

CC_FORCE_INLINE
static inline uint32_t __adc_vadc_init(uint32_t nvadc) {

  __syscall1r(228, VIO_CALL(SB_VADC_INIT, nvadc));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __adc_vadc_deinit(uint32_t nvadc) {

  __syscall1r(228, VIO_CALL(SB_VADC_DEINIT, nvadc));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __adc_vadc_start(uint32_t nvadc, unsigned grpnum,
                                        adcsample_t *samples, size_t depth) {

  __syscall4r(228, VIO_CALL(SB_VADC_START, nvadc),
              grpnum, samples, depth);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __adc_vadc_stop(uint32_t nvadc) {

  __syscall1r(228, VIO_CALL(SB_VADC_STOP, nvadc));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __adc_vadc_iscirc(uint32_t nvadc) {

  __syscall1r(100, VIO_CALL(SB_VADC_ISCIRC, nvadc));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __adc_vadc_gcerr(uint32_t nvadc) {

  __syscall1r(100, VIO_CALL(SB_VADC_GCERR, nvadc));
  return (uint32_t)r0;
}

static void adc_lld_serve_interrupt(hal_adc_driver_c *adcp, uint32_t nvrq) {
  uint32_t sts;

  sts = __sb_vrq_gcsts(nvrq);

  if ((sts & (1U << HAL_DRV_STATE_ERROR)) != 0U) {
    adcerror_t errors = (adcerror_t)__adc_vadc_gcerr(adcp->nvadc);

    if (errors == (adcerror_t)0U) {
      errors = ADC_ERR_DMAFAILURE;
    }

    adcp->errors |= errors;
    __cbdrv_invoke_cb_with_transition(adcp,
                                      HAL_DRV_STATE_ERROR,
                                      HAL_DRV_STATE_READY);
    adcp->grpp = NULL;
    adcp->circular = false;
    _adc_error_wakeup_isr(adcp);
    return;
  }

  if ((sts & (1U << HAL_DRV_STATE_HALF)) != 0U) {
    _adc_isr_half_code(adcp);
  }

  if ((sts & (1U << HAL_DRV_STATE_FULL)) != 0U) {
    adcp->events |= ADC_EVENT_FULL;
    __cbdrv_invoke_full_cb(adcp);
    _adc_wakeup_isr(adcp, HAL_DRV_STATE_FULL);
  }

  if ((sts & (1U << HAL_DRV_STATE_COMPLETE)) != 0U) {
    adcp->events |= ADC_EVENT_COMPLETE;
    __cbdrv_invoke_cb_with_transition(adcp,
                                      HAL_DRV_STATE_COMPLETE,
                                      HAL_DRV_STATE_READY);
    adcp->grpp = NULL;
    adcp->circular = false;
    _adc_wakeup_isr(adcp, HAL_DRV_STATE_COMPLETE);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if VIO_ADC_USE_VADC1 || defined(__DOXYGEN__)
#if !defined(VIO_VADC1_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(VIO_VADC1_IRQ)) {

  OSAL_IRQ_PROLOGUE();

  adc_lld_serve_interrupt(&ADCD1, VIO_VADC1_IRQ);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void adc_lld_init(void) {

  adcObjectInit(&ADCD1);
  ADCD1.nvadc = 0U;
  __sb_vrq_seten(1U << VIO_VADC1_IRQ);
}

msg_t adc_lld_start(hal_adc_driver_c *adcp) {
  msg_t msg = HAL_RET_SUCCESS;

  if (false) {
  }
#if VIO_ADC_USE_VADC1 == TRUE
  else if (&ADCD1 == adcp) {
    msg = (msg_t)__adc_vadc_init(adcp->nvadc);
  }
#endif
  else {
    osalDbgAssert(false, "invalid ADC instance");
  }

  return msg;
}

void adc_lld_stop(hal_adc_driver_c *adcp) {
  msg_t msg = HAL_RET_SUCCESS;

  if (false) {
  }
#if VIO_ADC_USE_VADC1 == TRUE
  else if (&ADCD1 == adcp) {
    msg = (msg_t)__adc_vadc_deinit(adcp->nvadc);
  }
#endif
  else {
    osalDbgAssert(false, "invalid ADC instance");
  }

  osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
}

const hal_adc_config_t *adc_lld_setcfg(hal_adc_driver_c *adcp,
                                       const hal_adc_config_t *config) {

  if (config == NULL) {
    return adc_lld_selcfg(adcp, 0U);
  }

  return NULL;
}

const hal_adc_config_t *adc_lld_selcfg(hal_adc_driver_c *adcp,
                                       unsigned cfgnum) {

  (void)adcp;

  if (cfgnum != 0U) {
    return NULL;
  }

  return &default_config;
}

void adc_lld_set_callback(hal_adc_driver_c *adcp, drv_cb_t cb) {

  (void)adcp;
  (void)cb;
}

msg_t adc_lld_start_conversion(hal_adc_driver_c *adcp, unsigned grpnum,
                               adcsample_t *samples,
                               size_t depth) {
  msg_t msg;

  msg = (msg_t)__adc_vadc_start(adcp->nvadc, grpnum, samples, depth);
  if (msg == HAL_RET_SUCCESS) {
    adcp->circular = __adc_vadc_iscirc(adcp->nvadc) != 0U;
  }

  return msg;
}

void adc_lld_stop_conversion(hal_adc_driver_c *adcp) {
  msg_t msg;

  msg = (msg_t)__adc_vadc_stop(adcp->nvadc);
  osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
}

#endif /* HAL_USE_ADC */

/** @} */
