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
 * @file    TIMv1/hal_pwm_lld.c
 * @brief   STM32 PWM subsystem low level driver source.
 *
 * @addtogroup HAL_PWM
 * @{
 */

#include "hal.h"

#if HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

static const hal_pwm_config_t pwm_default_config = {
  .frequency      = 1000000U,
  .period         = 1000U,
  .enabled_events = 0U,
  .channels       = {
    { .mode = PWM_OUTPUT_DISABLED },
    { .mode = PWM_OUTPUT_DISABLED },
    { .mode = PWM_OUTPUT_DISABLED },
    { .mode = PWM_OUTPUT_DISABLED },
    { .mode = PWM_OUTPUT_DISABLED },
    { .mode = PWM_OUTPUT_DISABLED }
  },
  .cr2            = 0U,
  .bdtr           = 0U,
  .dier           = 0U
};

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if STM32_PWM_USE_TIM1 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD1;
#endif

#if STM32_PWM_USE_TIM2 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD2;
#endif

#if STM32_PWM_USE_TIM3 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD3;
#endif

#if STM32_PWM_USE_TIM4 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD4;
#endif

#if STM32_PWM_USE_TIM5 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD5;
#endif

#if STM32_PWM_USE_TIM8 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD8;
#endif

#if STM32_PWM_USE_TIM9 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD9;
#endif

#if STM32_PWM_USE_TIM10 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD10;
#endif

#if STM32_PWM_USE_TIM11 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD11;
#endif

#if STM32_PWM_USE_TIM12 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD12;
#endif

#if STM32_PWM_USE_TIM13 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD13;
#endif

#if STM32_PWM_USE_TIM14 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD14;
#endif

#if STM32_PWM_USE_TIM15 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD15;
#endif

#if STM32_PWM_USE_TIM16 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD16;
#endif

#if STM32_PWM_USE_TIM17 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD17;
#endif

#if STM32_PWM_USE_TIM20 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD20;
#endif

#if STM32_PWM_USE_TIM21 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD21;
#endif

#if STM32_PWM_USE_TIM22 || defined(__DOXYGEN__)
hal_pwm_driver_c PWMD22;
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static uint32_t pwm_lld_events_to_dier(pwm_events_t events) {
  uint32_t dier = 0U;

  if ((events & PWM_EVENT_PERIOD) != 0U) {
    dier |= STM32_TIM_DIER_UIE;
  }
  if ((events & PWM_EVENT_CHANNEL(0U)) != 0U) {
    dier |= STM32_TIM_DIER_CC1IE;
  }
  if ((events & PWM_EVENT_CHANNEL(1U)) != 0U) {
    dier |= STM32_TIM_DIER_CC2IE;
  }
  if ((events & PWM_EVENT_CHANNEL(2U)) != 0U) {
    dier |= STM32_TIM_DIER_CC3IE;
  }
  if ((events & PWM_EVENT_CHANNEL(3U)) != 0U) {
    dier |= STM32_TIM_DIER_CC4IE;
  }

  return dier;
}

static uint32_t pwm_lld_events_to_sr(pwm_events_t events) {
  uint32_t sr = 0U;

  if ((events & PWM_EVENT_PERIOD) != 0U) {
    sr |= STM32_TIM_SR_UIF;
  }
  if ((events & PWM_EVENT_CHANNEL(0U)) != 0U) {
    sr |= STM32_TIM_SR_CC1IF;
  }
  if ((events & PWM_EVENT_CHANNEL(1U)) != 0U) {
    sr |= STM32_TIM_SR_CC2IF;
  }
  if ((events & PWM_EVENT_CHANNEL(2U)) != 0U) {
    sr |= STM32_TIM_SR_CC3IF;
  }
  if ((events & PWM_EVENT_CHANNEL(3U)) != 0U) {
    sr |= STM32_TIM_SR_CC4IF;
  }

  return sr;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PWM driver initialization.
 *
 * @notapi
 */
void pwm_lld_init(void) {

#if STM32_PWM_USE_TIM1
  pwmObjectInit(&PWMD1);
  PWMD1.channels = STM32_TIM1_CHANNELS;
  PWMD1.tim      = STM32_TIM1;
  PWMD1.has_bdtr = true;
#endif

#if STM32_PWM_USE_TIM2
  pwmObjectInit(&PWMD2);
  PWMD2.channels = STM32_TIM2_CHANNELS;
  PWMD2.tim      = STM32_TIM2;
  PWMD2.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM3
  pwmObjectInit(&PWMD3);
  PWMD3.channels = STM32_TIM3_CHANNELS;
  PWMD3.tim      = STM32_TIM3;
  PWMD3.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM4
  pwmObjectInit(&PWMD4);
  PWMD4.channels = STM32_TIM4_CHANNELS;
  PWMD4.tim      = STM32_TIM4;
  PWMD4.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM5
  pwmObjectInit(&PWMD5);
  PWMD5.channels = STM32_TIM5_CHANNELS;
  PWMD5.tim      = STM32_TIM5;
  PWMD5.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM8
  pwmObjectInit(&PWMD8);
  PWMD8.channels = STM32_TIM8_CHANNELS;
  PWMD8.tim      = STM32_TIM8;
  PWMD8.has_bdtr = true;
#endif

#if STM32_PWM_USE_TIM9
  pwmObjectInit(&PWMD9);
  PWMD9.channels = STM32_TIM9_CHANNELS;
  PWMD9.tim      = STM32_TIM9;
  PWMD9.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM10
  pwmObjectInit(&PWMD10);
  PWMD10.channels = STM32_TIM10_CHANNELS;
  PWMD10.tim      = STM32_TIM10;
  PWMD10.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM11
  pwmObjectInit(&PWMD11);
  PWMD11.channels = STM32_TIM11_CHANNELS;
  PWMD11.tim      = STM32_TIM11;
  PWMD11.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM12
  pwmObjectInit(&PWMD12);
  PWMD12.channels = STM32_TIM12_CHANNELS;
  PWMD12.tim      = STM32_TIM12;
  PWMD12.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM13
  pwmObjectInit(&PWMD13);
  PWMD13.channels = STM32_TIM13_CHANNELS;
  PWMD13.tim      = STM32_TIM13;
  PWMD13.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM14
  pwmObjectInit(&PWMD14);
  PWMD14.channels = STM32_TIM14_CHANNELS;
  PWMD14.tim      = STM32_TIM14;
  PWMD14.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM15
  pwmObjectInit(&PWMD15);
  PWMD15.channels = STM32_TIM15_CHANNELS;
  PWMD15.tim      = STM32_TIM15;
  PWMD15.has_bdtr = true;
#endif

#if STM32_PWM_USE_TIM16
  pwmObjectInit(&PWMD16);
  PWMD16.channels = STM32_TIM16_CHANNELS;
  PWMD16.tim      = STM32_TIM16;
  PWMD16.has_bdtr = true;
#endif

#if STM32_PWM_USE_TIM17
  pwmObjectInit(&PWMD17);
  PWMD17.channels = STM32_TIM17_CHANNELS;
  PWMD17.tim      = STM32_TIM17;
  PWMD17.has_bdtr = true;
#endif

#if STM32_PWM_USE_TIM20
  pwmObjectInit(&PWMD20);
  PWMD20.channels = STM32_TIM20_CHANNELS;
  PWMD20.tim      = STM32_TIM20;
  PWMD20.has_bdtr = true;
#endif

#if STM32_PWM_USE_TIM21
  pwmObjectInit(&PWMD21);
  PWMD21.channels = STM32_TIM21_CHANNELS;
  PWMD21.tim      = STM32_TIM21;
  PWMD21.has_bdtr = false;
#endif

#if STM32_PWM_USE_TIM22
  pwmObjectInit(&PWMD22);
  PWMD22.channels = STM32_TIM22_CHANNELS;
  PWMD22.tim      = STM32_TIM22;
  PWMD22.has_bdtr = false;
#endif
}

/**
 * @brief   Configures and activates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p hal_pwm_driver_c object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t pwm_lld_start(hal_pwm_driver_c *pwmp) {
  const hal_pwm_config_t *cfg;
  uint32_t psc;
  uint32_t ccer;

  cfg = (const hal_pwm_config_t *)pwmp->config;
  if (cfg == NULL) {
    cfg = pwm_lld_selcfg(pwmp, 0U);
  }
  if (cfg == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

  if ((cfg->frequency == 0U) || (cfg->period == 0U)) {
    return HAL_RET_CONFIG_ERROR;
  }

  pwmp->config         = cfg;
  pwmp->period         = cfg->period;
  pwmp->enabled        = 0U;
  pwmp->enabled_events = cfg->enabled_events;
  pwmp->events         = 0U;

#if STM32_PWM_USE_TIM1
  if (&PWMD1 == pwmp) {
    rccEnableTIM1(true);
    rccResetTIM1();
#if defined(STM32_TIM1CLK)
    pwmp->clock = STM32_TIM1CLK;
#else
    pwmp->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_PWM_USE_TIM2
  if (&PWMD2 == pwmp) {
    rccEnableTIM2(true);
    rccResetTIM2();
#if defined(STM32_TIM2CLK)
    pwmp->clock = STM32_TIM2CLK;
#else
    pwmp->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_PWM_USE_TIM3
  if (&PWMD3 == pwmp) {
    rccEnableTIM3(true);
    rccResetTIM3();
#if defined(STM32_TIM3CLK)
    pwmp->clock = STM32_TIM3CLK;
#else
    pwmp->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_PWM_USE_TIM4
  if (&PWMD4 == pwmp) {
    rccEnableTIM4(true);
    rccResetTIM4();
#if defined(STM32_TIM4CLK)
    pwmp->clock = STM32_TIM4CLK;
#else
    pwmp->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_PWM_USE_TIM5
  if (&PWMD5 == pwmp) {
    rccEnableTIM5(true);
    rccResetTIM5();
#if defined(STM32_TIM5CLK)
    pwmp->clock = STM32_TIM5CLK;
#else
    pwmp->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_PWM_USE_TIM8
  if (&PWMD8 == pwmp) {
    rccEnableTIM8(true);
    rccResetTIM8();
#if defined(STM32_TIM8CLK)
    pwmp->clock = STM32_TIM8CLK;
#else
    pwmp->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_PWM_USE_TIM9
  if (&PWMD9 == pwmp) {
    rccEnableTIM9(true);
    rccResetTIM9();
#if defined(STM32_TIM9CLK)
    pwmp->clock = STM32_TIM9CLK;
#else
    pwmp->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_PWM_USE_TIM10
  if (&PWMD10 == pwmp) {
    rccEnableTIM10(true);
    rccResetTIM10();
#if defined(STM32_TIM10CLK)
    pwmp->clock = STM32_TIM10CLK;
#else
    pwmp->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_PWM_USE_TIM11
  if (&PWMD11 == pwmp) {
    rccEnableTIM11(true);
    rccResetTIM11();
#if defined(STM32_TIM11CLK)
    pwmp->clock = STM32_TIM11CLK;
#else
    pwmp->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_PWM_USE_TIM12
  if (&PWMD12 == pwmp) {
    rccEnableTIM12(true);
    rccResetTIM12();
#if defined(STM32_TIM12CLK)
    pwmp->clock = STM32_TIM12CLK;
#else
    pwmp->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_PWM_USE_TIM13
  if (&PWMD13 == pwmp) {
    rccEnableTIM13(true);
    rccResetTIM13();
#if defined(STM32_TIM13CLK)
    pwmp->clock = STM32_TIM13CLK;
#else
    pwmp->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_PWM_USE_TIM14
  if (&PWMD14 == pwmp) {
    rccEnableTIM14(true);
    rccResetTIM14();
#if defined(STM32_TIM14CLK)
    pwmp->clock = STM32_TIM14CLK;
#else
    pwmp->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_PWM_USE_TIM15
  if (&PWMD15 == pwmp) {
    rccEnableTIM15(true);
    rccResetTIM15();
#if defined(STM32_TIM15CLK)
    pwmp->clock = STM32_TIM15CLK;
#else
    pwmp->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_PWM_USE_TIM16
  if (&PWMD16 == pwmp) {
    rccEnableTIM16(true);
    rccResetTIM16();
#if defined(STM32_TIM16CLK)
    pwmp->clock = STM32_TIM16CLK;
#else
    pwmp->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_PWM_USE_TIM17
  if (&PWMD17 == pwmp) {
    rccEnableTIM17(true);
    rccResetTIM17();
#if defined(STM32_TIM17CLK)
    pwmp->clock = STM32_TIM17CLK;
#else
    pwmp->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_PWM_USE_TIM20
  if (&PWMD20 == pwmp) {
    rccEnableTIM20(true);
    rccResetTIM20();
#if defined(STM32_TIM20CLK)
    pwmp->clock = STM32_TIM20CLK;
#else
    pwmp->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_PWM_USE_TIM21
  if (&PWMD21 == pwmp) {
    rccEnableTIM21(true);
    rccResetTIM21();
#if defined(STM32_TIM21CLK)
    pwmp->clock = STM32_TIM21CLK;
#else
    pwmp->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_PWM_USE_TIM22
  if (&PWMD22 == pwmp) {
    rccEnableTIM22(true);
    rccResetTIM22();
#if defined(STM32_TIM22CLK)
    pwmp->clock = STM32_TIM22CLK;
#else
    pwmp->clock = STM32_TIMCLK1;
#endif
  }
#endif

  pwmp->tim->CR1 = 0U;
  pwmp->tim->CCR[0] = 0U;
  pwmp->tim->CCR[1] = 0U;
  pwmp->tim->CCR[2] = 0U;
  pwmp->tim->CCR[3] = 0U;
#if STM32_TIM_MAX_CHANNELS > 4
  if (pwmp->channels > 4U) {
    pwmp->tim->CCXR[0] = 0U;
    pwmp->tim->CCXR[1] = 0U;
  }
#endif
  pwmp->tim->CNT = 0U;

  /* All channels configured in PWM1 mode with preload enabled.*/
  pwmp->tim->CCMR1 = STM32_TIM_CCMR1_OC1M(6) | STM32_TIM_CCMR1_OC1PE |
                     STM32_TIM_CCMR1_OC2M(6) | STM32_TIM_CCMR1_OC2PE;
  pwmp->tim->CCMR2 = STM32_TIM_CCMR2_OC3M(6) | STM32_TIM_CCMR2_OC3PE |
                     STM32_TIM_CCMR2_OC4M(6) | STM32_TIM_CCMR2_OC4PE;
#if STM32_TIM_MAX_CHANNELS > 4
  pwmp->tim->CCMR3 = STM32_TIM_CCMR3_OC5M(6) | STM32_TIM_CCMR3_OC5PE |
                     STM32_TIM_CCMR3_OC6M(6) | STM32_TIM_CCMR3_OC6PE;
#endif

  psc = (pwmp->clock / cfg->frequency) - 1U;
  osalDbgAssert((psc <= 0xFFFFU) &&
                (((psc + 1U) * cfg->frequency) == pwmp->clock),
                "invalid frequency");

  pwmp->tim->PSC = psc;
  pwmp->tim->ARR = cfg->period - 1U;
  pwmp->tim->CR2 = cfg->cr2;

  ccer = 0U;
  switch (cfg->channels[0].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    ccer |= STM32_TIM_CCER_CC1P;
    /* Falls through.*/
  case PWM_OUTPUT_ACTIVE_HIGH:
    ccer |= STM32_TIM_CCER_CC1E;
    /* Falls through.*/
  default:
    ;
  }
  switch (cfg->channels[1].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    ccer |= STM32_TIM_CCER_CC2P;
    /* Falls through.*/
  case PWM_OUTPUT_ACTIVE_HIGH:
    ccer |= STM32_TIM_CCER_CC2E;
    /* Falls through.*/
  default:
    ;
  }
  switch (cfg->channels[2].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    ccer |= STM32_TIM_CCER_CC3P;
    /* Falls through.*/
  case PWM_OUTPUT_ACTIVE_HIGH:
    ccer |= STM32_TIM_CCER_CC3E;
    /* Falls through.*/
  default:
    ;
  }
  switch (cfg->channels[3].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    ccer |= STM32_TIM_CCER_CC4P;
    /* Falls through.*/
  case PWM_OUTPUT_ACTIVE_HIGH:
    ccer |= STM32_TIM_CCER_CC4E;
    /* Falls through.*/
  default:
    ;
  }

  if (pwmp->has_bdtr) {
    switch (cfg->channels[0].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
      ccer |= STM32_TIM_CCER_CC1NP;
      /* Falls through.*/
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
      ccer |= STM32_TIM_CCER_CC1NE;
      /* Falls through.*/
    default:
      ;
    }
    switch (cfg->channels[1].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
      ccer |= STM32_TIM_CCER_CC2NP;
      /* Falls through.*/
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
      ccer |= STM32_TIM_CCER_CC2NE;
      /* Falls through.*/
    default:
      ;
    }
    switch (cfg->channels[2].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
      ccer |= STM32_TIM_CCER_CC3NP;
      /* Falls through.*/
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
      ccer |= STM32_TIM_CCER_CC3NE;
      /* Falls through.*/
    default:
      ;
    }
    switch (cfg->channels[3].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
      ccer |= STM32_TIM_CCER_CC4NP;
      /* Falls through.*/
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
      ccer |= STM32_TIM_CCER_CC4NE;
      /* Falls through.*/
    default:
      ;
    }
  }

  pwmp->tim->CCER = ccer;
  pwmp->tim->EGR  = STM32_TIM_EGR_UG;
  pwmp->tim->SR   = 0U;
  pwmp->tim->DIER = (cfg->dier & ~STM32_TIM_DIER_IRQ_MASK) |
                    pwm_lld_events_to_dier(cfg->enabled_events);
  if (pwmp->has_bdtr) {
    pwmp->tim->BDTR = cfg->bdtr | STM32_TIM_BDTR_MOE;
  }

  pwmp->tim->CR1 = STM32_TIM_CR1_ARPE | STM32_TIM_CR1_URS |
                   STM32_TIM_CR1_CEN;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p hal_pwm_driver_c object
 *
 * @notapi
 */
void pwm_lld_stop(hal_pwm_driver_c *pwmp) {

  pwmp->tim->CR1  = 0U;
  pwmp->tim->DIER = 0U;
  pwmp->tim->SR   = 0U;
  if (pwmp->has_bdtr) {
    pwmp->tim->BDTR = 0U;
  }

#if STM32_PWM_USE_TIM1
  if (&PWMD1 == pwmp) {
    rccDisableTIM1();
  }
#endif

#if STM32_PWM_USE_TIM2
  if (&PWMD2 == pwmp) {
    rccDisableTIM2();
  }
#endif

#if STM32_PWM_USE_TIM3
  if (&PWMD3 == pwmp) {
    rccDisableTIM3();
  }
#endif

#if STM32_PWM_USE_TIM4
  if (&PWMD4 == pwmp) {
    rccDisableTIM4();
  }
#endif

#if STM32_PWM_USE_TIM5
  if (&PWMD5 == pwmp) {
    rccDisableTIM5();
  }
#endif

#if STM32_PWM_USE_TIM8
  if (&PWMD8 == pwmp) {
    rccDisableTIM8();
  }
#endif

#if STM32_PWM_USE_TIM9
  if (&PWMD9 == pwmp) {
    rccDisableTIM9();
  }
#endif

#if STM32_PWM_USE_TIM10
  if (&PWMD10 == pwmp) {
    rccDisableTIM10();
  }
#endif

#if STM32_PWM_USE_TIM11
  if (&PWMD11 == pwmp) {
    rccDisableTIM11();
  }
#endif

#if STM32_PWM_USE_TIM12
  if (&PWMD12 == pwmp) {
    rccDisableTIM12();
  }
#endif

#if STM32_PWM_USE_TIM13
  if (&PWMD13 == pwmp) {
    rccDisableTIM13();
  }
#endif

#if STM32_PWM_USE_TIM14
  if (&PWMD14 == pwmp) {
    rccDisableTIM14();
  }
#endif

#if STM32_PWM_USE_TIM15
  if (&PWMD15 == pwmp) {
    rccDisableTIM15();
  }
#endif

#if STM32_PWM_USE_TIM16
  if (&PWMD16 == pwmp) {
    rccDisableTIM16();
  }
#endif

#if STM32_PWM_USE_TIM17
  if (&PWMD17 == pwmp) {
    rccDisableTIM17();
  }
#endif

#if STM32_PWM_USE_TIM20
  if (&PWMD20 == pwmp) {
    rccDisableTIM20();
  }
#endif

#if STM32_PWM_USE_TIM21
  if (&PWMD21 == pwmp) {
    rccDisableTIM21();
  }
#endif

#if STM32_PWM_USE_TIM22
  if (&PWMD22 == pwmp) {
    rccDisableTIM22();
  }
#endif
}

const hal_pwm_config_t *pwm_lld_setcfg(hal_pwm_driver_c *pwmp,
                                       const hal_pwm_config_t *config) {
  (void)pwmp;

  if (config == NULL) {
    return pwm_lld_selcfg(pwmp, 0U);
  }

  if ((config->frequency == 0U) || (config->period == 0U)) {
    return NULL;
  }

  return config;
}

const hal_pwm_config_t *pwm_lld_selcfg(hal_pwm_driver_c *pwmp,
                                       unsigned cfgnum) {

#if PWM_USE_CONFIGURATIONS == TRUE
  extern const pwm_configurations_t pwm_configurations;

  if (cfgnum >= pwm_configurations.cfgsnum) {
    return NULL;
  }

  return pwm_lld_setcfg(pwmp, &pwm_configurations.cfgs[cfgnum]);
#else
  (void)pwmp;

  if (cfgnum != 0U) {
    return NULL;
  }

  return &pwm_default_config;
#endif
}

void pwm_lld_set_callback(hal_pwm_driver_c *pwmp, drv_cb_t cb) {
  (void)pwmp;
  (void)cb;
}

/**
 * @brief   Enables a PWM channel.
 *
 * @param[in] pwmp      pointer to a @p hal_pwm_driver_c object
 * @param[in] channel   PWM channel identifier
 * @param[in] width     PWM pulse width as clock pulses number
 *
 * @notapi
 */
void pwm_lld_enable_channel(hal_pwm_driver_c *pwmp,
                            pwmchannel_t channel,
                            pwmcnt_t width) {

#if STM32_TIM_MAX_CHANNELS <= 4
  pwmp->tim->CCR[channel] = width;
#else
  if (channel < 4U) {
    pwmp->tim->CCR[channel] = width;
  }
  else {
    pwmp->tim->CCXR[channel - 4U] = width;
  }
#endif
}

/**
 * @brief   Disables a PWM channel and its notification.
 *
 * @param[in] pwmp      pointer to a @p hal_pwm_driver_c object
 * @param[in] channel   PWM channel identifier
 *
 * @notapi
 */
void pwm_lld_disable_channel(hal_pwm_driver_c *pwmp, pwmchannel_t channel) {

#if STM32_TIM_MAX_CHANNELS <= 4
  pwmp->tim->CCR[channel] = 0U;
#else
  if (channel < 4U) {
    pwmp->tim->CCR[channel] = 0U;
  }
  else {
    pwmp->tim->CCXR[channel - 4U] = 0U;
  }
#endif

  pwm_lld_disable_events(pwmp, PWM_EVENT_CHANNEL(channel));
}

/**
 * @brief   Enables PWM event notifications.
 *
 * @param[in] pwmp      pointer to a @p hal_pwm_driver_c object
 * @param[in] events    events mask
 *
 * @notapi
 */
void pwm_lld_enable_events(hal_pwm_driver_c *pwmp, pwm_events_t events) {
  uint32_t sr;
  uint32_t dier;

  osalDbgAssert((events & ~PWM_EVENT_ALL) == 0U, "unsupported events");

  sr   = pwm_lld_events_to_sr(events);
  dier = pwm_lld_events_to_dier(events);

  if (dier != 0U) {
    pwmp->tim->SR = ~sr;
    pwmp->tim->DIER |= dier;
  }
}

/**
 * @brief   Disables PWM event notifications.
 *
 * @param[in] pwmp      pointer to a @p hal_pwm_driver_c object
 * @param[in] events    events mask
 *
 * @notapi
 */
void pwm_lld_disable_events(hal_pwm_driver_c *pwmp, pwm_events_t events) {
  uint32_t dier;

  dier = pwm_lld_events_to_dier(events);
  pwmp->tim->DIER &= ~dier;
}

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] pwmp      pointer to a @p hal_pwm_driver_c object
 *
 * @notapi
 */
void pwm_lld_serve_interrupt(hal_pwm_driver_c *pwmp) {
  pwm_events_t events = 0U;
  uint32_t sr;

  sr  = pwmp->tim->SR;
  sr &= pwmp->tim->DIER & STM32_TIM_DIER_IRQ_MASK;
  pwmp->tim->SR = ~sr;

  if ((sr & STM32_TIM_SR_CC1IF) != 0U) {
    events |= PWM_EVENT_CHANNEL(0U);
  }
  if ((sr & STM32_TIM_SR_CC2IF) != 0U) {
    events |= PWM_EVENT_CHANNEL(1U);
  }
  if ((sr & STM32_TIM_SR_CC3IF) != 0U) {
    events |= PWM_EVENT_CHANNEL(2U);
  }
  if ((sr & STM32_TIM_SR_CC4IF) != 0U) {
    events |= PWM_EVENT_CHANNEL(3U);
  }
  if ((sr & STM32_TIM_SR_UIF) != 0U) {
    events |= PWM_EVENT_PERIOD;
  }

  if (events != 0U) {
    _pwm_isr_invoke_cb(pwmp, events);
  }
}

#endif /* HAL_USE_PWM */

/** @} */
