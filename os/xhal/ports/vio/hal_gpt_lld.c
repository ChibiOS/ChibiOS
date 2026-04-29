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
 * @file    sandbox/hal_gpt_lld.c
 * @brief   SandBox GPT subsystem low level driver source.
 *
 * @addtogroup HAL_GPT
 * @{
 */

#include "hal.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   GPTD1 driver identifier.
 * @note    The driver GPTD1 is mapped on VGPT1 when enabled.
 */
#if VIO_GPT_USE_VGPT1 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_init(uint32_t nvgpt, size_t n, void *p) {

  __syscall3r(229, VIO_CALL(SB_VGPT_INIT, nvgpt), n, p);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_deinit(uint32_t nvgpt) {

  __syscall1r(229, VIO_CALL(SB_VGPT_DEINIT, nvgpt));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_pdelay(uint32_t nvgpt, gptcnt_t interval) {

  __syscall2r(229, VIO_CALL(SB_VGPT_PDELAY, nvgpt), interval);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_start(uint32_t nvgpt, uint32_t mode,
                                        gptcnt_t interval) {

  __syscall3r(229, VIO_CALL(SB_VGPT_START, nvgpt), mode, interval);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_stop(uint32_t nvgpt) {

  __syscall1r(229, VIO_CALL(SB_VGPT_STOP, nvgpt));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_chgi(uint32_t nvgpt, gptcnt_t interval) {

  __syscall2r(229, VIO_CALL(SB_VGPT_CHGI, nvgpt), interval);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_setcb(uint32_t nvgpt, uint32_t enable) {

  __syscall2r(229, VIO_CALL(SB_VGPT_SETCB, nvgpt), enable);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_selcfg(uint32_t nvgpt, uint32_t ncfg,
                                         size_t n, void *p) {

  __syscall4r(229, VIO_CALL(SB_VGPT_SELCFG, nvgpt), ncfg, n, p);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_getc(uint32_t nvgpt) {

  __syscall1r(101, VIO_CALL(SB_VGPT_GETC, nvgpt));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __gpt_vgpt_getfreq(uint32_t nvgpt) {

  __syscall1r(101, VIO_CALL(SB_VGPT_GETFREQ, nvgpt));
  return (uint32_t)r0;
}

/**
 * @brief   Shared VGPT interrupt service routine.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @param[in] nvrq      VGPT virtual IRQ number
 */
static void gpt_lld_serve_interrupt(hal_gpt_driver_c *gptp, uint32_t nvrq) {
  uint32_t sts;

  sts = __sb_vrq_gcsts(nvrq);

  if ((sts & (1U << HAL_DRV_STATE_COMPLETE)) != 0U) {
    gptp->interval = (gptcnt_t)0;
    __cbdrv_invoke_complete_cb(gptp);
    return;
  }

  if (sts != 0U) {
    __cbdrv_invoke_cb(gptp);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if VIO_GPT_USE_VGPT1 || defined(__DOXYGEN__)
#if !defined(VIO_VGPT1_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(VIO_VGPT1_IRQ)) {

  OSAL_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD1, VIO_VGPT1_IRQ);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level GPT driver initialization.
 *
 * @notapi
 */
void gpt_lld_init(void) {

#if VIO_GPT_USE_VGPT1 == TRUE
  gptObjectInit(&GPTD1);
  GPTD1.nvgpt = 0U;
  GPTD1.cfgbuf.frequency = GPT_DEFAULT_FREQUENCY;
  GPTD1.interval = (gptcnt_t)0;
  __sb_vrq_seten(1U << VIO_VGPT1_IRQ);
#endif
}

/**
 * @brief   Configures and activates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t gpt_lld_start(hal_gpt_driver_c *gptp) {
  msg_t msg = HAL_RET_SUCCESS;

  if (false) {
  }
#if VIO_GPT_USE_VGPT1 == TRUE
  else if (&GPTD1 == gptp) {
    msg = (msg_t)__gpt_vgpt_init(gptp->nvgpt,
                                 sizeof (hal_gpt_config_t),
                                 &gptp->cfgbuf);
  }
#endif
  else {
    osalDbgAssert(false, "invalid GPT instance");
  }

  if (msg == HAL_RET_SUCCESS) {
    gptp->config = &gptp->cfgbuf;
    if (drvGetCallbackX(gptp) != NULL) {
      msg = (msg_t)__gpt_vgpt_setcb(gptp->nvgpt, 1U);
    }
  }

  return msg;
}

/**
 * @brief   Deactivates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 *
 * @notapi
 */
void gpt_lld_stop(hal_gpt_driver_c *gptp) {
  msg_t msg = HAL_RET_SUCCESS;

  if (false) {
  }
#if VIO_GPT_USE_VGPT1 == TRUE
  else if (&GPTD1 == gptp) {
    msg = (msg_t)__gpt_vgpt_deinit(gptp->nvgpt);
  }
#endif
  else {
    osalDbgAssert(false, "invalid GPT instance");
  }

  osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
  gptp->interval = (gptcnt_t)0;
}

/**
 * @brief   Applies an explicit GPT configuration.
 * @note    The VIO port does not accept sandbox-provided hardware
 *          configurations. Only host-owned configurations are used.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @param[in] config    pointer to the @p hal_gpt_config_t structure
 * @return              The configuration pointer or @p NULL on failure.
 *
 * @notapi
 */
const hal_gpt_config_t *gpt_lld_setcfg(hal_gpt_driver_c *gptp,
                                       const hal_gpt_config_t *config) {

  if (config == NULL) {
    return gpt_lld_selcfg(gptp, 0U);
  }

  return NULL;
}

/**
 * @brief   Selects a predefined GPT configuration.
 * @note    In the VIO port configuration zero is the host-owned default
 *          mirrored locally.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @param[in] cfgnum    driver configuration number
 * @return              The configuration pointer or @p NULL on failure.
 *
 * @notapi
 */
const hal_gpt_config_t *gpt_lld_selcfg(hal_gpt_driver_c *gptp,
                                       unsigned cfgnum) {
  msg_t msg;

  msg = __gpt_vgpt_selcfg(gptp->nvgpt, cfgnum,
                          sizeof (hal_gpt_config_t), &gptp->cfgbuf);
  if (msg != HAL_RET_SUCCESS) {
    return NULL;
  }

  return &gptp->cfgbuf;
}

/**
 * @brief   Updates the GPT callback.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @param[in] cb        driver callback
 *
 * @notapi
 */
void gpt_lld_set_callback(hal_gpt_driver_c *gptp, drv_cb_t cb) {

  if (drvGetStateX(gptp) != HAL_DRV_STATE_STOP) {
    msg_t msg;

    msg = (msg_t)__gpt_vgpt_setcb(gptp->nvgpt, cb != NULL ? 1U : 0U);
    osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
  }
}

/**
 * @brief   Changes the interval of GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @param[in] interval  new cycle time in timer ticks
 *
 * @notapi
 */
void gpt_lld_change_interval_impl(hal_gpt_driver_c *gptp,
                                  gptcnt_t interval) {
  msg_t msg;

  msg = (msg_t)__gpt_vgpt_chgi(gptp->nvgpt, interval);
  osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
  gptp->interval = interval;
}

/**
 * @brief   Returns the counter value of GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @return              The current counter value.
 *
 * @notapi
 */
gptcnt_t gpt_lld_get_counter_impl(hal_gpt_driver_c *gptp) {

  return (gptcnt_t)__gpt_vgpt_getc(gptp->nvgpt);
}

/**
 * @brief   Starts the timer.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @param[in] interval  period in ticks
 *
 * @notapi
 */
void gpt_lld_start_timer(hal_gpt_driver_c *gptp, gptcnt_t interval) {
  uint32_t mode;
  msg_t msg;

  if (gptp->state == GPT_CONTINUOUS) {
    mode = SB_VGPT_CONTINUOUS;
  }
  else {
    osalDbgAssert(gptp->state == GPT_ONESHOT, "invalid state");
    mode = SB_VGPT_ONESHOT;
  }

  msg = (msg_t)__gpt_vgpt_start(gptp->nvgpt, mode, interval);
  osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
  gptp->interval = interval;
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 *
 * @notapi
 */
void gpt_lld_stop_timer(hal_gpt_driver_c *gptp) {
  msg_t msg;

  msg = (msg_t)__gpt_vgpt_stop(gptp->nvgpt);
  osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
  gptp->interval = (gptcnt_t)0;
}

/**
 * @brief   Starts the timer in one shot mode and waits for completion.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @param[in] interval  time interval in ticks
 *
 * @notapi
 */
void gpt_lld_polled_delay(hal_gpt_driver_c *gptp, gptcnt_t interval) {
  msg_t msg;

  msg = (msg_t)__gpt_vgpt_pdelay(gptp->nvgpt, interval);
  osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
}

#endif /* HAL_USE_GPT */

/** @} */
