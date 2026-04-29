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
 * @file    TIMv1/hal_gpt_lld.c
 * @brief   STM32 GPT subsystem low level driver source.
 *
 * @addtogroup HAL_GPT
 * @{
 */

#include "hal.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

static const hal_gpt_config_t gpt_default_config = {
  .frequency = GPT_DEFAULT_FREQUENCY,
  .cr2       = 0U,
  .dier      = 0U
};

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   GPTD1 driver identifier.
 * @note    The driver GPTD1 allocates the complex timer TIM1 when enabled.
 */
#if STM32_GPT_USE_TIM1 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD1;
#endif

/**
 * @brief   GPTD2 driver identifier.
 * @note    The driver GPTD2 allocates the timer TIM2 when enabled.
 */
#if STM32_GPT_USE_TIM2 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD2;
#endif

/**
 * @brief   GPTD3 driver identifier.
 * @note    The driver GPTD3 allocates the timer TIM3 when enabled.
 */
#if STM32_GPT_USE_TIM3 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD3;
#endif

/**
 * @brief   GPTD4 driver identifier.
 * @note    The driver GPTD4 allocates the timer TIM4 when enabled.
 */
#if STM32_GPT_USE_TIM4 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD4;
#endif

/**
 * @brief   GPTD5 driver identifier.
 * @note    The driver GPTD5 allocates the timer TIM5 when enabled.
 */
#if STM32_GPT_USE_TIM5 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD5;
#endif

/**
 * @brief   GPTD6 driver identifier.
 * @note    The driver GPTD6 allocates the timer TIM6 when enabled.
 */
#if STM32_GPT_USE_TIM6 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD6;
#endif

/**
 * @brief   GPTD7 driver identifier.
 * @note    The driver GPTD7 allocates the timer TIM7 when enabled.
 */
#if STM32_GPT_USE_TIM7 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD7;
#endif

/**
 * @brief   GPTD8 driver identifier.
 * @note    The driver GPTD8 allocates the timer TIM8 when enabled.
 */
#if STM32_GPT_USE_TIM8 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD8;
#endif

/**
 * @brief   GPTD9 driver identifier.
 * @note    The driver GPTD9 allocates the timer TIM9 when enabled.
 */
#if STM32_GPT_USE_TIM9 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD9;
#endif

/**
 * @brief   GPTD10 driver identifier.
 * @note    The driver GPTD10 allocates the timer TIM10 when enabled.
 */
#if STM32_GPT_USE_TIM10 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD10;
#endif

/**
 * @brief   GPTD11 driver identifier.
 * @note    The driver GPTD11 allocates the timer TIM11 when enabled.
 */
#if STM32_GPT_USE_TIM11 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD11;
#endif

/**
 * @brief   GPTD12 driver identifier.
 * @note    The driver GPTD12 allocates the timer TIM12 when enabled.
 */
#if STM32_GPT_USE_TIM12 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD12;
#endif

/**
 * @brief   GPTD13 driver identifier.
 * @note    The driver GPTD13 allocates the timer TIM13 when enabled.
 */
#if STM32_GPT_USE_TIM13 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD13;
#endif

/**
 * @brief   GPTD14 driver identifier.
 * @note    The driver GPTD14 allocates the timer TIM14 when enabled.
 */
#if STM32_GPT_USE_TIM14 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD14;
#endif

/**
 * @brief   GPTD15 driver identifier.
 * @note    The driver GPTD15 allocates the timer TIM15 when enabled.
 */
#if STM32_GPT_USE_TIM15 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD15;
#endif

/**
 * @brief   GPTD16 driver identifier.
 * @note    The driver GPTD16 allocates the timer TIM16 when enabled.
 */
#if STM32_GPT_USE_TIM16 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD16;
#endif

/**
 * @brief   GPTD17 driver identifier.
 * @note    The driver GPTD17 allocates the timer TIM17 when enabled.
 */
#if STM32_GPT_USE_TIM17 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD17;
#endif

/**
 * @brief   GPTD20 driver identifier.
 * @note    The driver GPTD20 allocates the timer TIM20 when enabled.
 */
#if STM32_GPT_USE_TIM20 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD20;
#endif

/**
 * @brief   GPTD21 driver identifier.
 * @note    The driver GPTD21 allocates the timer TIM21 when enabled.
 */
#if STM32_GPT_USE_TIM21 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD21;
#endif

/**
 * @brief   GPTD22 driver identifier.
 * @note    The driver GPTD22 allocates the timer TIM22 when enabled.
 */
#if STM32_GPT_USE_TIM22 || defined(__DOXYGEN__)
hal_gpt_driver_c GPTD22;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level GPT driver initialization.
 *
 * @notapi
 */
void gpt_lld_init(void) {

#if STM32_GPT_USE_TIM1
  GPTD1.tim = STM32_TIM1;
  gptObjectInit(&GPTD1);
#endif

#if STM32_GPT_USE_TIM2
  GPTD2.tim = STM32_TIM2;
  gptObjectInit(&GPTD2);
#endif

#if STM32_GPT_USE_TIM3
  GPTD3.tim = STM32_TIM3;
  gptObjectInit(&GPTD3);
#endif

#if STM32_GPT_USE_TIM4
  GPTD4.tim = STM32_TIM4;
  gptObjectInit(&GPTD4);
#endif

#if STM32_GPT_USE_TIM5
  GPTD5.tim = STM32_TIM5;
  gptObjectInit(&GPTD5);
#endif

#if STM32_GPT_USE_TIM6
  GPTD6.tim = STM32_TIM6;
  gptObjectInit(&GPTD6);
#endif

#if STM32_GPT_USE_TIM7
  GPTD7.tim = STM32_TIM7;
  gptObjectInit(&GPTD7);
#endif

#if STM32_GPT_USE_TIM8
  GPTD8.tim = STM32_TIM8;
  gptObjectInit(&GPTD8);
#endif

#if STM32_GPT_USE_TIM9
  GPTD9.tim = STM32_TIM9;
  gptObjectInit(&GPTD9);
#endif

#if STM32_GPT_USE_TIM10
  GPTD10.tim = STM32_TIM10;
  gptObjectInit(&GPTD10);
#endif

#if STM32_GPT_USE_TIM11
  GPTD11.tim = STM32_TIM11;
  gptObjectInit(&GPTD11);
#endif

#if STM32_GPT_USE_TIM12
  GPTD12.tim = STM32_TIM12;
  gptObjectInit(&GPTD12);
#endif

#if STM32_GPT_USE_TIM13
  GPTD13.tim = STM32_TIM13;
  gptObjectInit(&GPTD13);
#endif

#if STM32_GPT_USE_TIM14
  GPTD14.tim = STM32_TIM14;
  gptObjectInit(&GPTD14);
#endif

#if STM32_GPT_USE_TIM15
  GPTD15.tim = STM32_TIM15;
  gptObjectInit(&GPTD15);
#endif

#if STM32_GPT_USE_TIM16
  GPTD16.tim = STM32_TIM16;
  gptObjectInit(&GPTD16);
#endif

#if STM32_GPT_USE_TIM17
  GPTD17.tim = STM32_TIM17;
  gptObjectInit(&GPTD17);
#endif

#if STM32_GPT_USE_TIM20
  GPTD20.tim = STM32_TIM20;
  gptObjectInit(&GPTD20);
#endif

#if STM32_GPT_USE_TIM21
  GPTD21.tim = STM32_TIM21;
  gptObjectInit(&GPTD21);
#endif

#if STM32_GPT_USE_TIM22
  GPTD22.tim = STM32_TIM22;
  gptObjectInit(&GPTD22);
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
  const hal_gpt_config_t *cfg;
  uint16_t psc;

  cfg = (const hal_gpt_config_t *)gptp->config;
  if (cfg == NULL) {
    cfg = gpt_lld_selcfg(gptp, 0U);
  }
  if (cfg == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

  gptp->config = cfg;

  {
#if STM32_GPT_USE_TIM1
    if (&GPTD1 == gptp) {
      rccEnableTIM1(true);
      rccResetTIM1();
#if defined(STM32_TIM1CLK)
      gptp->clock = STM32_TIM1CLK;
#else
      gptp->clock = STM32_TIMCLK2;
#endif
    }
#endif

#if STM32_GPT_USE_TIM2
    if (&GPTD2 == gptp) {
      rccEnableTIM2(true);
      rccResetTIM2();
#if defined(STM32_TIM2CLK)
      gptp->clock = STM32_TIM2CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM3
    if (&GPTD3 == gptp) {
      rccEnableTIM3(true);
      rccResetTIM3();
#if defined(STM32_TIM3CLK)
      gptp->clock = STM32_TIM3CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM4
    if (&GPTD4 == gptp) {
      rccEnableTIM4(true);
      rccResetTIM4();
#if defined(STM32_TIM4CLK)
      gptp->clock = STM32_TIM4CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM5
    if (&GPTD5 == gptp) {
      rccEnableTIM5(true);
      rccResetTIM5();
#if defined(STM32_TIM5CLK)
      gptp->clock = STM32_TIM5CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM6
    if (&GPTD6 == gptp) {
      rccEnableTIM6(true);
      rccResetTIM6();
#if defined(STM32_TIM6CLK)
      gptp->clock = STM32_TIM6CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM7
    if (&GPTD7 == gptp) {
      rccEnableTIM7(true);
      rccResetTIM7();
#if defined(STM32_TIM7CLK)
      gptp->clock = STM32_TIM7CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM8
    if (&GPTD8 == gptp) {
      rccEnableTIM8(true);
      rccResetTIM8();
#if defined(STM32_TIM8CLK)
      gptp->clock = STM32_TIM8CLK;
#else
      gptp->clock = STM32_TIMCLK2;
#endif
    }
#endif

#if STM32_GPT_USE_TIM9
    if (&GPTD9 == gptp) {
      rccEnableTIM9(true);
      rccResetTIM9();
#if defined(STM32_TIM9CLK)
      gptp->clock = STM32_TIM9CLK;
#else
      gptp->clock = STM32_TIMCLK2;
#endif
    }
#endif

#if STM32_GPT_USE_TIM10
    if (&GPTD10 == gptp) {
      rccEnableTIM10(true);
      rccResetTIM10();
#if defined(STM32_TIM10CLK)
      gptp->clock = STM32_TIM10CLK;
#else
      gptp->clock = STM32_TIMCLK2;
#endif
    }
#endif

#if STM32_GPT_USE_TIM11
    if (&GPTD11 == gptp) {
      rccEnableTIM11(true);
      rccResetTIM11();
#if defined(STM32_TIM11CLK)
      gptp->clock = STM32_TIM11CLK;
#else
      gptp->clock = STM32_TIMCLK2;
#endif
    }
#endif

#if STM32_GPT_USE_TIM12
    if (&GPTD12 == gptp) {
      rccEnableTIM12(true);
      rccResetTIM12();
#if defined(STM32_TIM12CLK)
      gptp->clock = STM32_TIM12CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM13
    if (&GPTD13 == gptp) {
      rccEnableTIM13(true);
      rccResetTIM13();
#if defined(STM32_TIM13CLK)
      gptp->clock = STM32_TIM13CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM14
    if (&GPTD14 == gptp) {
      rccEnableTIM14(true);
      rccResetTIM14();
#if defined(STM32_TIM14CLK)
      gptp->clock = STM32_TIM14CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM15
    if (&GPTD15 == gptp) {
      rccEnableTIM15(true);
      rccResetTIM15();
#if defined(STM32_TIM15CLK)
      gptp->clock = STM32_TIM15CLK;
#else
      gptp->clock = STM32_TIMCLK2;
#endif
    }
#endif

#if STM32_GPT_USE_TIM16
    if (&GPTD16 == gptp) {
      rccEnableTIM16(true);
      rccResetTIM16();
#if defined(STM32_TIM16CLK)
      gptp->clock = STM32_TIM16CLK;
#else
      gptp->clock = STM32_TIMCLK2;
#endif
    }
#endif

#if STM32_GPT_USE_TIM17
    if (&GPTD17 == gptp) {
      rccEnableTIM17(true);
      rccResetTIM17();
#if defined(STM32_TIM17CLK)
      gptp->clock = STM32_TIM17CLK;
#else
      gptp->clock = STM32_TIMCLK2;
#endif
    }
#endif

#if STM32_GPT_USE_TIM20
    if (&GPTD20 == gptp) {
      rccEnableTIM20(true);
      rccResetTIM20();
#if defined(STM32_TIM20CLK)
      gptp->clock = STM32_TIM20CLK;
#else
      gptp->clock = STM32_TIMCLK2;
#endif
    }
#endif

#if STM32_GPT_USE_TIM21
    if (&GPTD21 == gptp) {
      rccEnableTIM21(true);
      rccResetTIM21();
#if defined(STM32_TIM21CLK)
      gptp->clock = STM32_TIM21CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif

#if STM32_GPT_USE_TIM22
    if (&GPTD22 == gptp) {
      rccEnableTIM22(true);
      rccResetTIM22();
#if defined(STM32_TIM22CLK)
      gptp->clock = STM32_TIM22CLK;
#else
      gptp->clock = STM32_TIMCLK1;
#endif
    }
#endif
  }

  psc = (uint16_t)((gptp->clock / cfg->frequency) - 1U);
  osalDbgAssert(((uint32_t)(psc + 1U) * cfg->frequency) == gptp->clock,
                "invalid frequency");

  gptp->tim->CR1  = 0U;
  gptp->tim->CR2  = cfg->cr2;
  gptp->tim->PSC  = psc;
  gptp->tim->SR   = 0U;
  gptp->tim->DIER = cfg->dier & ~STM32_TIM_DIER_IRQ_MASK;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 *
 * @notapi
 */
void gpt_lld_stop(hal_gpt_driver_c *gptp) {

  gptp->tim->CR1  = 0U;
  gptp->tim->DIER = 0U;
  gptp->tim->SR   = 0U;

#if STM32_GPT_USE_TIM1
  if (&GPTD1 == gptp) {
    rccDisableTIM1();
  }
#endif

#if STM32_GPT_USE_TIM2
  if (&GPTD2 == gptp) {
    rccDisableTIM2();
  }
#endif

#if STM32_GPT_USE_TIM3
  if (&GPTD3 == gptp) {
    rccDisableTIM3();
  }
#endif

#if STM32_GPT_USE_TIM4
  if (&GPTD4 == gptp) {
    rccDisableTIM4();
  }
#endif

#if STM32_GPT_USE_TIM5
  if (&GPTD5 == gptp) {
    rccDisableTIM5();
  }
#endif

#if STM32_GPT_USE_TIM6
  if (&GPTD6 == gptp) {
    rccDisableTIM6();
  }
#endif

#if STM32_GPT_USE_TIM7
  if (&GPTD7 == gptp) {
    rccDisableTIM7();
  }
#endif

#if STM32_GPT_USE_TIM8
  if (&GPTD8 == gptp) {
    rccDisableTIM8();
  }
#endif

#if STM32_GPT_USE_TIM9
  if (&GPTD9 == gptp) {
    rccDisableTIM9();
  }
#endif

#if STM32_GPT_USE_TIM10
  if (&GPTD10 == gptp) {
    rccDisableTIM10();
  }
#endif

#if STM32_GPT_USE_TIM11
  if (&GPTD11 == gptp) {
    rccDisableTIM11();
  }
#endif

#if STM32_GPT_USE_TIM12
  if (&GPTD12 == gptp) {
    rccDisableTIM12();
  }
#endif

#if STM32_GPT_USE_TIM13
  if (&GPTD13 == gptp) {
    rccDisableTIM13();
  }
#endif

#if STM32_GPT_USE_TIM14
  if (&GPTD14 == gptp) {
    rccDisableTIM14();
  }
#endif

#if STM32_GPT_USE_TIM15
  if (&GPTD15 == gptp) {
    rccDisableTIM15();
  }
#endif

#if STM32_GPT_USE_TIM16
  if (&GPTD16 == gptp) {
    rccDisableTIM16();
  }
#endif

#if STM32_GPT_USE_TIM17
  if (&GPTD17 == gptp) {
    rccDisableTIM17();
  }
#endif

#if STM32_GPT_USE_TIM20
  if (&GPTD20 == gptp) {
    rccDisableTIM20();
  }
#endif

#if STM32_GPT_USE_TIM21
  if (&GPTD21 == gptp) {
    rccDisableTIM21();
  }
#endif

#if STM32_GPT_USE_TIM22
  if (&GPTD22 == gptp) {
    rccDisableTIM22();
  }
#endif
}

const hal_gpt_config_t *gpt_lld_setcfg(hal_gpt_driver_c *gptp,
                                       const hal_gpt_config_t *config) {
  (void)gptp;

  if (config == NULL) {
    return gpt_lld_selcfg(gptp, 0U);
  }

  return config;
}

const hal_gpt_config_t *gpt_lld_selcfg(hal_gpt_driver_c *gptp,
                                       unsigned cfgnum) {
  (void)gptp;

#if GPT_USE_CONFIGURATIONS == TRUE
  extern const gpt_configurations_t gpt_configurations;

  if (cfgnum < gpt_configurations.cfgsnum) {
    return &gpt_configurations.cfgs[cfgnum];
  }
#else
  if (cfgnum != 0U) {
    return NULL;
  }

  return &gpt_default_config;
#endif

  return NULL;
}

void gpt_lld_set_callback(hal_gpt_driver_c *gptp, drv_cb_t cb) {

  if (gptp->state == GPT_CONTINUOUS) {
    if (cb != NULL) {
      gptp->tim->DIER |= STM32_TIM_DIER_UIE;
    }
    else {
      gptp->tim->DIER &= ~STM32_TIM_DIER_UIE;
    }
  }
}

/**
 * @brief   Starts the timer in continuous mode.
 * @note    Interval values 0 and 1 are invalid on this architecture.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @param[in] interval  period in ticks
 *
 * @notapi
 */
void gpt_lld_start_timer(hal_gpt_driver_c *gptp, gptcnt_t interval) {

  osalDbgAssert(interval > (gptcnt_t)0, "invalid interval");

  gptp->tim->ARR = (uint32_t)(interval - 1U);
  gptp->tim->EGR = STM32_TIM_EGR_UG;
  gptp->tim->CNT = 0U;

  /* Giving UIF time to be asserted before clearing it.*/
  gptp->tim->SR  = 0U;
  if (drvGetCallbackX(gptp) != NULL) {
    gptp->tim->DIER |= STM32_TIM_DIER_UIE;
  }
  gptp->tim->CR1 = STM32_TIM_CR1_ARPE | STM32_TIM_CR1_URS |
                   STM32_TIM_CR1_CEN;
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 *
 * @notapi
 */
void gpt_lld_stop_timer(hal_gpt_driver_c *gptp) {

  gptp->tim->CR1 = 0U;
  gptp->tim->SR  = 0U;
  gptp->tim->DIER &= ~STM32_TIM_DIER_IRQ_MASK;
}

/**
 * @brief   Starts the timer in one shot mode and waits for completion.
 * @details This function specifically polls the timer waiting for completion
 *          in order to not have extra delays caused by interrupt servicing,
 *          this function is only recommended for short delays.
 * @note    Interval values 0 and 1 are invalid on this architecture.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @param[in] interval  time interval in ticks
 *
 * @notapi
 */
void gpt_lld_polled_delay(hal_gpt_driver_c *gptp, gptcnt_t interval) {

  osalDbgAssert(interval > (gptcnt_t)0, "invalid interval");

  gptp->tim->CR1 = STM32_TIM_CR1_UDIS;
  gptp->tim->ARR = (uint32_t)(interval - 1U);
  gptp->tim->EGR = STM32_TIM_EGR_UG;
  gptp->tim->SR  = 0U;
  gptp->tim->CR1 = STM32_TIM_CR1_OPM | STM32_TIM_CR1_URS |
                   STM32_TIM_CR1_CEN;
  while ((gptp->tim->SR & STM32_TIM_SR_UIF) == 0U) {
  }
  gptp->tim->SR = 0U;
}

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] gptp      pointer to a @p hal_gpt_driver_c object
 *
 * @notapi
 */
void gpt_lld_serve_interrupt(hal_gpt_driver_c *gptp) {
  uint32_t sr;

  sr  = gptp->tim->SR;
  sr &= gptp->tim->DIER & STM32_TIM_DIER_IRQ_MASK;
  gptp->tim->SR = ~sr;
  if ((sr & STM32_TIM_SR_UIF) != 0U) {
    _gpt_isr_invoke_cb(gptp);
  }
}

#endif /* HAL_USE_GPT */

/** @} */
