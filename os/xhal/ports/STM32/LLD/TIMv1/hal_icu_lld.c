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
 * @file    TIMv1/hal_icu_lld.c
 * @brief   STM32 ICU subsystem low level driver source.
 *
 * @addtogroup HAL_ICU
 * @{
 */

#include "hal.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

static const hal_icu_config_t icu_default_config = {
  .mode           = ICU_INPUT_ACTIVE_HIGH,
  .frequency      = 1000000U,
  .enabled_events = 0U,
  .channel        = ICU_CHANNEL_1,
  .dier           = 0U,
  .arr            = 0xFFFFFFFFU
};

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if STM32_ICU_USE_TIM1 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD1;
#endif

#if STM32_ICU_USE_TIM2 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD2;
#endif

#if STM32_ICU_USE_TIM3 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD3;
#endif

#if STM32_ICU_USE_TIM4 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD4;
#endif

#if STM32_ICU_USE_TIM5 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD5;
#endif

#if STM32_ICU_USE_TIM8 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD8;
#endif

#if STM32_ICU_USE_TIM9 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD9;
#endif

#if STM32_ICU_USE_TIM10 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD10;
#endif

#if STM32_ICU_USE_TIM11 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD11;
#endif

#if STM32_ICU_USE_TIM12 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD12;
#endif

#if STM32_ICU_USE_TIM13 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD13;
#endif

#if STM32_ICU_USE_TIM14 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD14;
#endif

#if STM32_ICU_USE_TIM15 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD15;
#endif

#if STM32_ICU_USE_TIM20 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD20;
#endif

#if STM32_ICU_USE_TIM21 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD21;
#endif

#if STM32_ICU_USE_TIM22 || defined(__DOXYGEN__)
hal_icu_driver_c ICUD22;
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static bool icu_lld_validate_config(const hal_icu_config_t *cfg) {

  return (bool)(((cfg->mode == ICU_INPUT_ACTIVE_HIGH) ||
                 (cfg->mode == ICU_INPUT_ACTIVE_LOW)) &&
                ((cfg->channel == ICU_CHANNEL_1) ||
                 (cfg->channel == ICU_CHANNEL_2)) &&
                (cfg->frequency != 0U));
}

static uint32_t icu_lld_events_to_dier(const hal_icu_config_t *cfg,
                                       icu_events_t events) {
  uint32_t dier = 0U;

  if ((events & ICU_EVENT_OVERFLOW) != 0U) {
    dier |= STM32_TIM_DIER_UIE;
  }

  if (cfg->channel == ICU_CHANNEL_1) {
    if ((events & ICU_EVENT_PERIOD) != 0U) {
      dier |= STM32_TIM_DIER_CC1IE;
    }
    if ((events & ICU_EVENT_WIDTH) != 0U) {
      dier |= STM32_TIM_DIER_CC2IE;
    }
  }
  else {
    if ((events & ICU_EVENT_WIDTH) != 0U) {
      dier |= STM32_TIM_DIER_CC1IE;
    }
    if ((events & ICU_EVENT_PERIOD) != 0U) {
      dier |= STM32_TIM_DIER_CC2IE;
    }
  }

  return dier;
}

static bool icu_lld_wait_edge(hal_icu_driver_c *icup) {
  const hal_icu_config_t *cfg = (const hal_icu_config_t *)icup->config;
  uint32_t sr;
  bool result;

  /* Polled mode so re-enabling interrupts while waiting.*/
  osalSysUnlock();

  if (cfg->channel == ICU_CHANNEL_1) {
    while (((sr = icup->tim->SR) &
            (STM32_TIM_SR_CC1IF | STM32_TIM_SR_UIF)) == 0U) {
    }
  }
  else {
    while (((sr = icup->tim->SR) &
            (STM32_TIM_SR_CC2IF | STM32_TIM_SR_UIF)) == 0U) {
    }
  }

  result = (bool)((sr & STM32_TIM_SR_UIF) != 0U);

  osalSysLock();

  icup->tim->SR &= ~(STM32_TIM_SR_CC1IF |
                     STM32_TIM_SR_CC2IF |
                     STM32_TIM_SR_UIF);

  return result;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ICU driver initialization.
 *
 * @notapi
 */
void icu_lld_init(void) {

#if STM32_ICU_USE_TIM1
  icuObjectInit(&ICUD1);
  ICUD1.tim = STM32_TIM1;
#endif

#if STM32_ICU_USE_TIM2
  icuObjectInit(&ICUD2);
  ICUD2.tim = STM32_TIM2;
#endif

#if STM32_ICU_USE_TIM3
  icuObjectInit(&ICUD3);
  ICUD3.tim = STM32_TIM3;
#endif

#if STM32_ICU_USE_TIM4
  icuObjectInit(&ICUD4);
  ICUD4.tim = STM32_TIM4;
#endif

#if STM32_ICU_USE_TIM5
  icuObjectInit(&ICUD5);
  ICUD5.tim = STM32_TIM5;
#endif

#if STM32_ICU_USE_TIM8
  icuObjectInit(&ICUD8);
  ICUD8.tim = STM32_TIM8;
#endif

#if STM32_ICU_USE_TIM9
  icuObjectInit(&ICUD9);
  ICUD9.tim = STM32_TIM9;
#endif

#if STM32_ICU_USE_TIM10
  icuObjectInit(&ICUD10);
  ICUD10.tim = STM32_TIM10;
#endif

#if STM32_ICU_USE_TIM11
  icuObjectInit(&ICUD11);
  ICUD11.tim = STM32_TIM11;
#endif

#if STM32_ICU_USE_TIM12
  icuObjectInit(&ICUD12);
  ICUD12.tim = STM32_TIM12;
#endif

#if STM32_ICU_USE_TIM13
  icuObjectInit(&ICUD13);
  ICUD13.tim = STM32_TIM13;
#endif

#if STM32_ICU_USE_TIM14
  icuObjectInit(&ICUD14);
  ICUD14.tim = STM32_TIM14;
#endif

#if STM32_ICU_USE_TIM15
  icuObjectInit(&ICUD15);
  ICUD15.tim = STM32_TIM15;
#endif

#if STM32_ICU_USE_TIM20
  icuObjectInit(&ICUD20);
  ICUD20.tim = STM32_TIM20;
#endif

#if STM32_ICU_USE_TIM21
  icuObjectInit(&ICUD21);
  ICUD21.tim = STM32_TIM21;
#endif

#if STM32_ICU_USE_TIM22
  icuObjectInit(&ICUD22);
  ICUD22.tim = STM32_TIM22;
#endif
}

/**
 * @brief   Configures and activates the ICU peripheral.
 *
 * @param[in] icup      pointer to a @p hal_icu_driver_c object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t icu_lld_start(hal_icu_driver_c *icup) {
  const hal_icu_config_t *cfg;
  uint32_t psc;

  cfg = (const hal_icu_config_t *)icup->config;
  if (cfg == NULL) {
    cfg = icu_lld_selcfg(icup, 0U);
  }
  if ((cfg == NULL) || !icu_lld_validate_config(cfg)) {
    return HAL_RET_CONFIG_ERROR;
  }

  icup->config         = cfg;
  icup->width          = 0U;
  icup->period         = 0U;
  icup->enabled_events = cfg->enabled_events & ICU_EVENT_ALL;
  icup->events         = 0U;

#if STM32_ICU_USE_TIM1
  if (&ICUD1 == icup) {
    rccEnableTIM1(true);
    rccResetTIM1();
#if defined(STM32_TIM1CLK)
    icup->clock = STM32_TIM1CLK;
#else
    icup->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_ICU_USE_TIM2
  if (&ICUD2 == icup) {
    rccEnableTIM2(true);
    rccResetTIM2();
#if defined(STM32_TIM2CLK)
    icup->clock = STM32_TIM2CLK;
#else
    icup->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_ICU_USE_TIM3
  if (&ICUD3 == icup) {
    rccEnableTIM3(true);
    rccResetTIM3();
#if defined(STM32_TIM3CLK)
    icup->clock = STM32_TIM3CLK;
#else
    icup->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_ICU_USE_TIM4
  if (&ICUD4 == icup) {
    rccEnableTIM4(true);
    rccResetTIM4();
#if defined(STM32_TIM4CLK)
    icup->clock = STM32_TIM4CLK;
#else
    icup->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_ICU_USE_TIM5
  if (&ICUD5 == icup) {
    rccEnableTIM5(true);
    rccResetTIM5();
#if defined(STM32_TIM5CLK)
    icup->clock = STM32_TIM5CLK;
#else
    icup->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_ICU_USE_TIM8
  if (&ICUD8 == icup) {
    rccEnableTIM8(true);
    rccResetTIM8();
#if defined(STM32_TIM8CLK)
    icup->clock = STM32_TIM8CLK;
#else
    icup->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_ICU_USE_TIM9
  if (&ICUD9 == icup) {
    rccEnableTIM9(true);
    rccResetTIM9();
#if defined(STM32_TIM9CLK)
    icup->clock = STM32_TIM9CLK;
#else
    icup->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_ICU_USE_TIM10
  if (&ICUD10 == icup) {
    rccEnableTIM10(true);
    rccResetTIM10();
#if defined(STM32_TIM10CLK)
    icup->clock = STM32_TIM10CLK;
#else
    icup->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_ICU_USE_TIM11
  if (&ICUD11 == icup) {
    rccEnableTIM11(true);
    rccResetTIM11();
#if defined(STM32_TIM11CLK)
    icup->clock = STM32_TIM11CLK;
#else
    icup->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_ICU_USE_TIM12
  if (&ICUD12 == icup) {
    rccEnableTIM12(true);
    rccResetTIM12();
#if defined(STM32_TIM12CLK)
    icup->clock = STM32_TIM12CLK;
#else
    icup->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_ICU_USE_TIM13
  if (&ICUD13 == icup) {
    rccEnableTIM13(true);
    rccResetTIM13();
#if defined(STM32_TIM13CLK)
    icup->clock = STM32_TIM13CLK;
#else
    icup->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_ICU_USE_TIM14
  if (&ICUD14 == icup) {
    rccEnableTIM14(true);
    rccResetTIM14();
#if defined(STM32_TIM14CLK)
    icup->clock = STM32_TIM14CLK;
#else
    icup->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_ICU_USE_TIM15
  if (&ICUD15 == icup) {
    rccEnableTIM15(true);
    rccResetTIM15();
#if defined(STM32_TIM15CLK)
    icup->clock = STM32_TIM15CLK;
#else
    icup->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_ICU_USE_TIM20
  if (&ICUD20 == icup) {
    rccEnableTIM20(true);
    rccResetTIM20();
#if defined(STM32_TIM20CLK)
    icup->clock = STM32_TIM20CLK;
#else
    icup->clock = STM32_TIMCLK2;
#endif
  }
#endif

#if STM32_ICU_USE_TIM21
  if (&ICUD21 == icup) {
    rccEnableTIM21(true);
    rccResetTIM21();
#if defined(STM32_TIM21CLK)
    icup->clock = STM32_TIM21CLK;
#else
    icup->clock = STM32_TIMCLK1;
#endif
  }
#endif

#if STM32_ICU_USE_TIM22
  if (&ICUD22 == icup) {
    rccEnableTIM22(true);
    rccResetTIM22();
#if defined(STM32_TIM22CLK)
    icup->clock = STM32_TIM22CLK;
#else
    icup->clock = STM32_TIMCLK1;
#endif
  }
#endif

  icup->tim->CR1    = 0U;
  icup->tim->CCR[0] = 0U;
  icup->tim->CCR[1] = 0U;
  icup->tim->CNT    = 0U;
  icup->tim->SR     = 0U;
  icup->tim->DIER   = cfg->dier & ~STM32_TIM_DIER_IRQ_MASK;

  psc = (icup->clock / cfg->frequency) - 1U;
  osalDbgAssert((psc <= 0xFFFFU) &&
                (((psc + 1U) * cfg->frequency) == icup->clock),
                "invalid frequency");

  icup->tim->PSC = psc;
  if (cfg->arr == 0U) {
    icup->tim->ARR = 0xFFFFFFFFU;
  }
  else {
    icup->tim->ARR = cfg->arr;
  }

  if (cfg->channel == ICU_CHANNEL_1) {
    icup->tim->CCMR1 = STM32_TIM_CCMR1_CC1S(1) |
                       STM32_TIM_CCMR1_CC2S(2);
    icup->tim->SMCR  = STM32_TIM_SMCR_TS(5) | STM32_TIM_SMCR_SMS(4);

    if (cfg->mode == ICU_INPUT_ACTIVE_HIGH) {
      icup->tim->CCER = STM32_TIM_CCER_CC1E |
                        STM32_TIM_CCER_CC2E | STM32_TIM_CCER_CC2P;
    }
    else {
      icup->tim->CCER = STM32_TIM_CCER_CC1E | STM32_TIM_CCER_CC1P |
                        STM32_TIM_CCER_CC2E;
    }

    icup->wccrp = &icup->tim->CCR[1];
    icup->pccrp = &icup->tim->CCR[0];
  }
  else {
    icup->tim->CCMR1 = STM32_TIM_CCMR1_CC1S(2) |
                       STM32_TIM_CCMR1_CC2S(1);
    icup->tim->SMCR  = STM32_TIM_SMCR_TS(6) | STM32_TIM_SMCR_SMS(4);

    if (cfg->mode == ICU_INPUT_ACTIVE_HIGH) {
      icup->tim->CCER = STM32_TIM_CCER_CC1E | STM32_TIM_CCER_CC1P |
                        STM32_TIM_CCER_CC2E;
    }
    else {
      icup->tim->CCER = STM32_TIM_CCER_CC1E |
                        STM32_TIM_CCER_CC2E | STM32_TIM_CCER_CC2P;
    }

    icup->wccrp = &icup->tim->CCR[0];
    icup->pccrp = &icup->tim->CCR[1];
  }

  icup->tim->DIER |= icu_lld_events_to_dier(cfg, icup->enabled_events);

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the ICU peripheral.
 *
 * @param[in] icup      pointer to a @p hal_icu_driver_c object
 *
 * @notapi
 */
void icu_lld_stop(hal_icu_driver_c *icup) {

  icup->tim->CR1  = 0U;
  icup->tim->DIER = 0U;
  icup->tim->SR   = 0U;

#if STM32_ICU_USE_TIM1
  if (&ICUD1 == icup) {
    rccDisableTIM1();
  }
#endif

#if STM32_ICU_USE_TIM2
  if (&ICUD2 == icup) {
    rccDisableTIM2();
  }
#endif

#if STM32_ICU_USE_TIM3
  if (&ICUD3 == icup) {
    rccDisableTIM3();
  }
#endif

#if STM32_ICU_USE_TIM4
  if (&ICUD4 == icup) {
    rccDisableTIM4();
  }
#endif

#if STM32_ICU_USE_TIM5
  if (&ICUD5 == icup) {
    rccDisableTIM5();
  }
#endif

#if STM32_ICU_USE_TIM8
  if (&ICUD8 == icup) {
    rccDisableTIM8();
  }
#endif

#if STM32_ICU_USE_TIM9
  if (&ICUD9 == icup) {
    rccDisableTIM9();
  }
#endif

#if STM32_ICU_USE_TIM10
  if (&ICUD10 == icup) {
    rccDisableTIM10();
  }
#endif

#if STM32_ICU_USE_TIM11
  if (&ICUD11 == icup) {
    rccDisableTIM11();
  }
#endif

#if STM32_ICU_USE_TIM12
  if (&ICUD12 == icup) {
    rccDisableTIM12();
  }
#endif

#if STM32_ICU_USE_TIM13
  if (&ICUD13 == icup) {
    rccDisableTIM13();
  }
#endif

#if STM32_ICU_USE_TIM14
  if (&ICUD14 == icup) {
    rccDisableTIM14();
  }
#endif

#if STM32_ICU_USE_TIM15
  if (&ICUD15 == icup) {
    rccDisableTIM15();
  }
#endif

#if STM32_ICU_USE_TIM20
  if (&ICUD20 == icup) {
    rccDisableTIM20();
  }
#endif

#if STM32_ICU_USE_TIM21
  if (&ICUD21 == icup) {
    rccDisableTIM21();
  }
#endif

#if STM32_ICU_USE_TIM22
  if (&ICUD22 == icup) {
    rccDisableTIM22();
  }
#endif
}

const hal_icu_config_t *icu_lld_setcfg(hal_icu_driver_c *icup,
                                       const hal_icu_config_t *config) {
  (void)icup;

  if (config == NULL) {
    return icu_lld_selcfg(icup, 0U);
  }

  if (!icu_lld_validate_config(config)) {
    return NULL;
  }

  return config;
}

const hal_icu_config_t *icu_lld_selcfg(hal_icu_driver_c *icup,
                                       unsigned cfgnum) {

#if ICU_USE_CONFIGURATIONS == TRUE
  extern const icu_configurations_t icu_configurations;

  if (cfgnum >= icu_configurations.cfgsnum) {
    return NULL;
  }

  return icu_lld_setcfg(icup, &icu_configurations.cfgs[cfgnum]);
#else
  (void)icup;

  if (cfgnum != 0U) {
    return NULL;
  }

  return &icu_default_config;
#endif
}

void icu_lld_set_callback(hal_icu_driver_c *icup, drv_cb_t cb) {
  (void)icup;
  (void)cb;
}

/**
 * @brief   Starts the input capture.
 *
 * @param[in] icup      pointer to a @p hal_icu_driver_c object
 *
 * @notapi
 */
void icu_lld_start_capture(hal_icu_driver_c *icup) {

  icup->tim->EGR |= STM32_TIM_EGR_UG;
  icup->tim->SR   = 0U;
  icup->tim->CR1  = STM32_TIM_CR1_URS | STM32_TIM_CR1_CEN;
}

/**
 * @brief   Waits for a completed capture.
 *
 * @param[in] icup      pointer to a @p hal_icu_driver_c object
 * @return              The capture status.
 * @retval false        if the capture is successful.
 * @retval true         if a timer overflow occurred.
 *
 * @notapi
 */
bool icu_lld_wait_capture(hal_icu_driver_c *icup) {

  if (icup->state == ICU_WAITING) {
    if (icu_lld_wait_edge(icup)) {
      return true;
    }
  }

  return icu_lld_wait_edge(icup);
}

/**
 * @brief   Stops the input capture.
 *
 * @param[in] icup      pointer to a @p hal_icu_driver_c object
 *
 * @notapi
 */
void icu_lld_stop_capture(hal_icu_driver_c *icup) {

  icup->tim->CR1   = 0U;
  icup->tim->DIER &= ~STM32_TIM_DIER_IRQ_MASK;
  icup->tim->SR    = 0U;
}

/**
 * @brief   Enables ICU event notifications.
 *
 * @param[in] icup      pointer to a @p hal_icu_driver_c object
 * @param[in] events    event flags to be enabled
 *
 * @notapi
 */
void icu_lld_enable_events(hal_icu_driver_c *icup, icu_events_t events) {
  const hal_icu_config_t *cfg = (const hal_icu_config_t *)icup->config;
  uint32_t dier;

  dier = icu_lld_events_to_dier(cfg, events);
  if (dier != 0U) {
    if ((icup->tim->DIER & STM32_TIM_DIER_IRQ_MASK) == 0U) {
      icup->tim->SR = 0U;
    }
    icup->tim->DIER |= dier;
  }
}

/**
 * @brief   Disables ICU event notifications.
 *
 * @param[in] icup      pointer to a @p hal_icu_driver_c object
 * @param[in] events    event flags to be disabled
 *
 * @notapi
 */
void icu_lld_disable_events(hal_icu_driver_c *icup, icu_events_t events) {
  const hal_icu_config_t *cfg = (const hal_icu_config_t *)icup->config;

  icup->tim->DIER &= ~icu_lld_events_to_dier(cfg, events);
}

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] icup      pointer to a @p hal_icu_driver_c object
 *
 * @notapi
 */
void icu_lld_serve_interrupt(hal_icu_driver_c *icup) {
  const hal_icu_config_t *cfg = (const hal_icu_config_t *)icup->config;
  icu_events_t events = 0U;
  uint32_t sr;

  sr  = icup->tim->SR;
  sr &= icup->tim->DIER & STM32_TIM_DIER_IRQ_MASK;
  icup->tim->SR = ~sr;

  if (cfg == NULL) {
    return;
  }

  if (cfg->channel == ICU_CHANNEL_1) {
    if (((sr & STM32_TIM_SR_CC2IF) != 0U) &&
        (icup->state == ICU_ACTIVE)) {
      icup->width = icu_lld_get_width(icup);
      events |= ICU_EVENT_WIDTH;
    }
    if ((sr & STM32_TIM_SR_CC1IF) != 0U) {
      icup->period = icu_lld_get_period(icup);
      if (icup->state == ICU_ACTIVE) {
        events |= ICU_EVENT_PERIOD;
      }
      icup->state = ICU_ACTIVE;
    }
  }
  else {
    if (((sr & STM32_TIM_SR_CC1IF) != 0U) &&
        (icup->state == ICU_ACTIVE)) {
      icup->width = icu_lld_get_width(icup);
      events |= ICU_EVENT_WIDTH;
    }
    if ((sr & STM32_TIM_SR_CC2IF) != 0U) {
      icup->period = icu_lld_get_period(icup);
      if (icup->state == ICU_ACTIVE) {
        events |= ICU_EVENT_PERIOD;
      }
      icup->state = ICU_ACTIVE;
    }
  }

  if ((sr & STM32_TIM_SR_UIF) != 0U) {
    events |= ICU_EVENT_OVERFLOW;
    icup->state = ICU_WAITING;
  }

  if (events != 0U) {
    _icu_isr_invoke_cb(icup, events);
  }
}

#endif /* HAL_USE_ICU */

/** @} */
