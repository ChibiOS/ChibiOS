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
 * @file    PWMv1/hal_pwm_lld.c
 * @brief   RP PWM subsystem low level driver source.
 *
 * @addtogroup PWM
 * @{
 */

#include "hal.h"

#if (HAL_USE_PWM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PWMD0 driver identifier.
 * @note    The driver PWMD0 allocates the PWM slice 0 when enabled.
 */
#if (RP_PWM_USE_PWM0 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD0;
#endif

/**
 * @brief   PWMD1 driver identifier.
 * @note    The driver PWMD1 allocates the PWM slice 1 when enabled.
 */
#if (RP_PWM_USE_PWM1 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD1;
#endif

/**
 * @brief   PWMD2 driver identifier.
 * @note    The driver PWMD2 allocates the PWM slice 2 when enabled.
 */
#if (RP_PWM_USE_PWM2 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD2;
#endif

/**
 * @brief   PWMD3 driver identifier.
 * @note    The driver PWMD3 allocates the PWM slice 3 when enabled.
 */
#if (RP_PWM_USE_PWM3 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD3;
#endif

/**
 * @brief   PWMD4 driver identifier.
 * @note    The driver PWMD4 allocates the PWM slice 4 when enabled.
 */
#if (RP_PWM_USE_PWM4 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD4;
#endif

/**
 * @brief   PWMD5 driver identifier.
 * @note    The driver PWMD5 allocates the PWM slice 5 when enabled.
 */
#if (RP_PWM_USE_PWM5 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD5;
#endif

/**
 * @brief   PWMD6 driver identifier.
 * @note    The driver PWMD6 allocates the PWM slice 6 when enabled.
 */
#if (RP_PWM_USE_PWM6 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD6;
#endif

/**
 * @brief   PWMD7 driver identifier.
 * @note    The driver PWMD7 allocates the PWM slice 7 when enabled.
 */
#if (RP_PWM_USE_PWM7 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD7;
#endif

/**
 * @brief   PWMD8 driver identifier.
 * @note    The driver PWMD8 allocates the PWM slice 8 when enabled.
 */
#if (RP_PWM_USE_PWM8 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD8;
#endif

/**
 * @brief   PWMD9 driver identifier.
 * @note    The driver PWMD9 allocates the PWM slice 9 when enabled.
 */
#if (RP_PWM_USE_PWM9 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD9;
#endif

/**
 * @brief   PWMD10 driver identifier.
 * @note    The driver PWMD10 allocates the PWM slice 10 when enabled.
 */
#if (RP_PWM_USE_PWM10 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD10;
#endif

/**
 * @brief   PWMD11 driver identifier.
 * @note    The driver PWMD11 allocates the PWM slice 11 when enabled.
 */
#if (RP_PWM_USE_PWM11 == TRUE) || defined(__DOXYGEN__)
PWMDriver PWMD11;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (RP_PWM_USE_PWM0 == TRUE) || (RP_PWM_USE_PWM1 == TRUE) || \
    (RP_PWM_USE_PWM2 == TRUE) || (RP_PWM_USE_PWM3 == TRUE) || \
    (RP_PWM_USE_PWM4 == TRUE) || (RP_PWM_USE_PWM5 == TRUE) || \
    (RP_PWM_USE_PWM6 == TRUE) || (RP_PWM_USE_PWM7 == TRUE) || \
    defined(__DOXYGEN__)
#define RP_PWM_USE_ANY_PWM07 TRUE
#else
#define RP_PWM_USE_ANY_PWM07 FALSE
#endif

#if (RP_HAS_PWM8 == TRUE)
#if (RP_PWM_USE_PWM8 == TRUE)  || (RP_PWM_USE_PWM9 == TRUE) || \
    (RP_PWM_USE_PWM10 == TRUE) || (RP_PWM_USE_PWM11 == TRUE)
#define RP_PWM_USE_ANY_PWM811 TRUE
#else
#define RP_PWM_USE_ANY_PWM811 FALSE
#endif
#else
#define RP_PWM_USE_ANY_PWM811 FALSE
#endif

#if (RP_PWM_USE_ANY_PWM07 == TRUE) || (RP_PWM_USE_ANY_PWM811 == TRUE) || \
    defined(__DOXYGEN__)

OSAL_IRQ_HANDLER(RP_PWM_IRQ_WRAP_0_HANDLER) {
  OSAL_IRQ_PROLOGUE();

  uint32_t ints = PWM->IRQ0_INTS;
  PWM->INTR = ints;

#if RP_PWM_USE_PWM0 == TRUE
  if (((ints & PWM_INTS_CH(0)) != 0) && (PWMD0.config->callback != NULL)) {
    PWMD0.config->callback(&PWMD0);
  }
#endif

#if RP_PWM_USE_PWM1 == TRUE
  if (((ints & PWM_INTS_CH(1)) != 0) && (PWMD1.config->callback != NULL)) {
    PWMD1.config->callback(&PWMD1);
  }
#endif

#if RP_PWM_USE_PWM2 == TRUE
  if (((ints & PWM_INTS_CH(2)) != 0) && (PWMD2.config->callback != NULL)) {
    PWMD2.config->callback(&PWMD2);
  }
#endif

#if RP_PWM_USE_PWM3 == TRUE
  if (((ints & PWM_INTS_CH(3)) != 0) && (PWMD3.config->callback != NULL)) {
    PWMD3.config->callback(&PWMD3);
  }
#endif

#if RP_PWM_USE_PWM4 == TRUE
  if (((ints & PWM_INTS_CH(4)) != 0) && (PWMD4.config->callback != NULL)) {
    PWMD4.config->callback(&PWMD4);
  }
#endif

#if RP_PWM_USE_PWM5 == TRUE
  if (((ints & PWM_INTS_CH(5)) != 0) && (PWMD5.config->callback != NULL)) {
    PWMD5.config->callback(&PWMD5);
  }
#endif

#if RP_PWM_USE_PWM6 == TRUE
  if (((ints & PWM_INTS_CH(6)) != 0) && (PWMD6.config->callback != NULL)) {
    PWMD6.config->callback(&PWMD6);
  }
#endif

#if RP_PWM_USE_PWM7 == TRUE
  if (((ints & PWM_INTS_CH(7)) != 0) && (PWMD7.config->callback != NULL)) {
    PWMD7.config->callback(&PWMD7);
  }
#endif

#if RP_PWM_USE_PWM8 == TRUE
  if (((ints & PWM_INTS_CH(8)) != 0) && (PWMD8.config->callback != NULL)) {
    PWMD8.config->callback(&PWMD8);
  }
#endif

#if RP_PWM_USE_PWM9 == TRUE
  if (((ints & PWM_INTS_CH(9)) != 0) && (PWMD9.config->callback != NULL)) {
    PWMD9.config->callback(&PWMD9);
  }
#endif

#if RP_PWM_USE_PWM10 == TRUE
  if (((ints & PWM_INTS_CH(10)) != 0) && (PWMD10.config->callback != NULL)) {
    PWMD10.config->callback(&PWMD10);
  }
#endif

#if RP_PWM_USE_PWM11 == TRUE
  if (((ints & PWM_INTS_CH(11)) != 0) && (PWMD11.config->callback != NULL)) {
    PWMD11.config->callback(&PWMD11);
  }
#endif

  OSAL_IRQ_EPILOGUE();
}

#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PWM driver initialization.
 *
 * @notapi
 */
void pwm_lld_init(void) {

  /* Reset PWM peripheral once for all slices. */
  rp_peripheral_reset(RESETS_ALLREG_PWM);

#if RP_PWM_USE_PWM0 == TRUE
  pwmObjectInit(&PWMD0);
  PWMD0.pwm = PWM;
  PWMD0.timer_id = 0;
  PWMD0.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM1 == TRUE
  pwmObjectInit(&PWMD1);
  PWMD1.pwm = PWM;
  PWMD1.timer_id = 1;
  PWMD1.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM2 == TRUE
  pwmObjectInit(&PWMD2);
  PWMD2.pwm = PWM;
  PWMD2.timer_id = 2;
  PWMD2.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM3 == TRUE
  pwmObjectInit(&PWMD3);
  PWMD3.pwm = PWM;
  PWMD3.timer_id = 3;
  PWMD3.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM4 == TRUE
  pwmObjectInit(&PWMD4);
  PWMD4.pwm = PWM;
  PWMD4.timer_id = 4;
  PWMD4.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM5 == TRUE
  pwmObjectInit(&PWMD5);
  PWMD5.pwm = PWM;
  PWMD5.timer_id = 5;
  PWMD5.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM6 == TRUE
  pwmObjectInit(&PWMD6);
  PWMD6.pwm = PWM;
  PWMD6.timer_id = 6;
  PWMD6.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM7 == TRUE
  pwmObjectInit(&PWMD7);
  PWMD7.pwm = PWM;
  PWMD7.timer_id = 7;
  PWMD7.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM8 == TRUE
  pwmObjectInit(&PWMD8);
  PWMD8.pwm = PWM;
  PWMD8.timer_id = 8;
  PWMD8.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM9 == TRUE
  pwmObjectInit(&PWMD9);
  PWMD9.pwm = PWM;
  PWMD9.timer_id = 9;
  PWMD9.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM10 == TRUE
  pwmObjectInit(&PWMD10);
  PWMD10.pwm = PWM;
  PWMD10.timer_id = 10;
  PWMD10.channels = PWM_CHANNELS;
#endif

#if RP_PWM_USE_PWM11 == TRUE
  pwmObjectInit(&PWMD11);
  PWMD11.pwm = PWM;
  PWMD11.timer_id = 11;
  PWMD11.channels = PWM_CHANNELS;
#endif
}

/**
 * @brief   Configures and activates the PWM peripheral.
 * @note    Starting a driver that is already in the @p PWM_READY state
 *          disables all the active channels.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_start(PWMDriver *pwmp) {
  PWM_TypeDef *p = pwmp->pwm;

  if (pwmp->state == PWM_STOP) {
    /* Clock activation and timer reset. */
    rp_peripheral_unreset(RESETS_ALLREG_PWM);
    nvicEnableVector(RP_PWM_IRQ_WRAP_0_NUMBER, RP_PWM_IRQ_WRAP_NUMBER_PRIORITY);
  } else {
    /* Driver re-configuration scenario, it must be stopped first. */
    p->CH[pwmp->timer_id].CSR = 0;
    p->CH[pwmp->timer_id].CTR = 0;
    p->CH[pwmp->timer_id].CC  = 0;
  }

  /* Counter clock divider. */
  halfreq_t sys_clk = halClockGetPointX(RP_CLK_SYS);
  halfreq_t pwm_freq_min = sys_clk / 256;

  osalDbgAssert(pwmp->config->frequency >= pwm_freq_min,
                "PWM counter frequency too low");

  /* Integer part must not be zero. */
  halfreq_t integer = sys_clk / pwmp->config->frequency;
  integer = integer == 0 ? 1 : integer;

  halfreq_t fraction = (sys_clk << 4) / pwmp->config->frequency;
  p->CH[pwmp->timer_id].DIV = (integer << 4 | (fraction & 0xF));
  p->CH[pwmp->timer_id].TOP = pwmp->period;

  uint32_t csr = PWM_CSR_EN | PWM_CSR_DIVMODE_FREE;
  csr &= ~PWM_CSR_PH_CORRECT;

  switch (pwmp->config->channels[0].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    csr |= PWM_CSR_A_INV;
    break;
  case PWM_OUTPUT_ACTIVE_HIGH:
    csr &= ~PWM_CSR_A_INV;
    break;
  default:
    ;
  }

  switch (pwmp->config->channels[1].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    csr |= PWM_CSR_B_INV;
    break;
  case PWM_OUTPUT_ACTIVE_HIGH:
    csr &= ~PWM_CSR_B_INV;
    break;
  default:
    ;
  }

  p->CH[pwmp->timer_id].CSR = csr;
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_stop(PWMDriver *pwmp) {
  PWM_TypeDef *p = pwmp->pwm;

  /* If in ready state then disables the PWM clock. */
  if (pwmp->state == PWM_READY) {
    p->CH[pwmp->timer_id].CSR = 0U;
    p->CH[pwmp->timer_id].CTR = 0U;
    p->CH[pwmp->timer_id].CC  = 0U;
    p->CH[pwmp->timer_id].DIV = 1U;
    p->CH[pwmp->timer_id].TOP = 0xFFFF;
  }

  /* If all timers are disabled, disable the interrupt and reset
     PWM peripheral. */
  if (!p->EN) {
    nvicDisableVector(RP_PWM_IRQ_WRAP_0_NUMBER);
    rp_peripheral_reset(RESETS_ALLREG_PWM);
  }
}

/**
 * @brief   Enables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is active using the specified configuration.
 * @note    The function has effect at the next cycle start.
 * @note    Channel notification is not enabled.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 * @param[in] width     PWM pulse width as clock pulses number
 *
 * @notapi
 */
void pwm_lld_enable_channel(PWMDriver *pwmp,
                            pwmchannel_t channel,
                            pwmcnt_t width) {
  uint32_t current_cc = pwmp->pwm->CH[pwmp->timer_id].CC;

  if (channel == 0) {
    pwmp->pwm->CH[pwmp->timer_id].CC = (width & PWM_CC_A) |
                                        (current_cc & PWM_CC_B);
  } else {
    pwmp->pwm->CH[pwmp->timer_id].CC = (current_cc & PWM_CC_A) |
                                        ((width << PWM_CC_B_Pos) & PWM_CC_B);
  }
}

/**
 * @brief   Disables a PWM channel and its notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is disabled and its output line returned to the
 *          idle state.
 * @note    The function has effect at the next cycle start.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 *
 * @notapi
 */
void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {
  if (channel == 0) {
    pwmp->pwm->CH[pwmp->timer_id].CC &= ~PWM_CC_A | PWM_CC_B;
  } else {
    pwmp->pwm->CH[pwmp->timer_id].CC &= PWM_CC_A | ~PWM_CC_B;
  }
}

/**
 * @brief   Enables the periodic activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @note    If the notification is already enabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_enable_periodic_notification(PWMDriver *pwmp) {
  pwmp->pwm->IRQ0_INTE |= PWM_INTE_CH(pwmp->timer_id);
}

/**
 * @brief   Disables the periodic activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @note    If the notification is already disabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_disable_periodic_notification(PWMDriver *pwmp) {
  pwmp->pwm->IRQ0_INTE &= ~PWM_INTE_CH(pwmp->timer_id);
}

/**
 * @brief   Enables a channel de-activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @pre     The channel must have been activated using @p pwmEnableChannel().
 * @note    If the notification is already enabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 *
 * @notapi
 */
void pwm_lld_enable_channel_notification(PWMDriver *pwmp,
                                         pwmchannel_t channel) {
  (void)pwmp;
  (void)channel;
  osalDbgAssert(false, "Individual channel notifications are not available.");
}

/**
 * @brief   Disables a channel de-activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @pre     The channel must have been activated using @p pwmEnableChannel().
 * @note    If the notification is already disabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 *
 * @notapi
 */
void pwm_lld_disable_channel_notification(PWMDriver *pwmp,
                                          pwmchannel_t channel) {
  (void)pwmp;
  (void)channel;
  osalDbgAssert(false, "Individual channel notifications are not available.");
}

#endif /* HAL_USE_PWM == TRUE */

/** @} */
