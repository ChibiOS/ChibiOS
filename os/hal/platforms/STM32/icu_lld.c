/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM32/icu_lld.c
 * @brief   STM32 ICU subsystem low level driver header.
 *
 * @addtogroup ICU
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   ICUD1 driver identifier.
 * @note    The driver ICUD1 allocates the complex timer TIM1 when enabled.
 */
#if STM32_ICU_USE_TIM1 || defined(__DOXYGEN__)
ICUDriver ICUD1;
#endif

/**
 * @brief   ICUD2 driver identifier.
 * @note    The driver ICUD1 allocates the timer TIM2 when enabled.
 */
#if STM32_ICU_USE_TIM2 || defined(__DOXYGEN__)
ICUDriver ICUD2;
#endif

/**
 * @brief   ICUD3 driver identifier.
 * @note    The driver ICUD1 allocates the timer TIM3 when enabled.
 */
#if STM32_ICU_USE_TIM3 || defined(__DOXYGEN__)
ICUDriver ICUD3;
#endif

/**
 * @brief   ICUD4 driver identifier.
 * @note    The driver ICUD4 allocates the timer TIM4 when enabled.
 */
#if STM32_ICU_USE_TIM4 || defined(__DOXYGEN__)
ICUDriver ICUD4;
#endif

/**
 * @brief   ICUD5 driver identifier.
 * @note    The driver ICUD5 allocates the timer TIM5 when enabled.
 */
#if STM32_ICU_USE_TIM5 || defined(__DOXYGEN__)
ICUDriver ICUD5;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 */
static void icu_lld_serve_interrupt(ICUDriver *icup) {
  uint16_t sr;

  sr = TIM1->SR & TIM1->DIER;
  icup->tim->SR = 0;
  if ((sr & TIM_SR_CC1IF) != 0)
    icup->config->period_cb(icup);
  if ((sr & TIM_SR_CC2IF) != 0)
    icup->config->width_cb(icup);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_ICU_USE_TIM1
/**
 * @brief   TIM1 compare interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM1_CC_IRQHandler) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD1);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_ICU_USE_TIM1 */

#if STM32_ICU_USE_TIM2
/**
 * @brief   TIM2 compare interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM2_IRQHandler) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD2);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_ICU_USE_TIM2 */

#if STM32_ICU_USE_TIM3
/**
 * @brief   TIM3 compare interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM3_IRQHandler) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD3);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_ICU_USE_TIM3 */

#if STM32_ICU_USE_TIM4
/**
 * @brief   TIM4 compare interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM4_IRQHandler) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD4);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_ICU_USE_TIM4 */

#if STM32_ICU_USE_TIM5
/**
 * @brief   TIM5 compare interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM5_IRQHandler) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD5);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_ICU_USE_TIM5 */

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
  /* Driver initialization.*/
  icuObjectInit(&ICUD1);
  ICUD1.tim = TIM1;
#endif

#if STM32_ICU_USE_TIM2
  /* Driver initialization.*/
  icuObjectInit(&ICUD2);
  ICUD2.tim = TIM2;
#endif

#if STM32_ICU_USE_TIM3
  /* Driver initialization.*/
  icuObjectInit(&ICUD3);
  ICUD3.tim = TIM3;
#endif

#if STM32_ICU_USE_TIM4
  /* Driver initialization.*/
  icuObjectInit(&ICUD4);
  ICUD4.tim = TIM4;
#endif

#if STM32_ICU_USE_TIM5
  /* Driver initialization.*/
  icuObjectInit(&ICUD5);
  ICUD5.tim = TIM5;
#endif
}

/**
 * @brief   Configures and activates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_start(ICUDriver *icup) {

  if (icup->state == ICU_STOP) {
    /* Clock activation and timer reset.*/
#if STM32_ICU_USE_TIM1
    if (&ICUD1 == icup) {
      RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
      RCC->APB2RSTR = RCC_APB2RSTR_TIM1RST;
      RCC->APB2RSTR = 0;
      NVICEnableVector(TIM1_CC_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_ICU_TIM1_IRQ_PRIORITY));
    }
#endif
#if STM32_ICU_USE_TIM2
    if (&ICUD2 == icup) {
      RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
      RCC->APB1RSTR = RCC_APB1RSTR_TIM2RST;
      RCC->APB1RSTR = 0;
      NVICEnableVector(TIM2_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_ICU_TIM2_IRQ_PRIORITY));
    }
#endif
#if STM32_ICU_USE_TIM3
    if (&ICUD3 == icup) {
      RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
      RCC->APB1RSTR = RCC_APB1RSTR_TIM3RST;
      RCC->APB1RSTR = 0;
      NVICEnableVector(TIM3_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_ICU_TIM3_IRQ_PRIORITY));
    }
#endif
#if STM32_ICU_USE_TIM4
    if (&ICUD4 == icup) {
      RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
      RCC->APB1RSTR = RCC_APB1RSTR_TIM4RST;
      RCC->APB1RSTR = 0;
      NVICEnableVector(TIM4_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_ICU_TIM4_IRQ_PRIORITY));
    }
#endif

#if STM32_ICU_USE_TIM5
    if (&ICUD5 == icup) {
      RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
      RCC->APB1RSTR = RCC_APB1RSTR_TIM5RST;
      RCC->APB1RSTR = 0;
      NVICEnableVector(TIM5_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_ICU_TIM5_IRQ_PRIORITY));
    }
#endif
  }

  /* Timer configuration, PWM input mode.*/
  icup->tim->CR1   = 0;                     /* Initially stopped.           */
  icup->tim->CR2   = 0;
  icup->tim->ARR   = 0xFFFF;
  icup->tim->PSC   = icup->config->psc;     /* Prescaler value.             */
  icup->tim->DIER  = 0;
  /* CCMR1_CC1S = 01 = CH1 Input on TI1.
     CCMR1_CC2S = 10 = CH2 Input on TI2.*/
  icup->tim->CCMR1 = TIM_CCMR1_CC1S_0 |
                     TIM_CCMR1_CC2S_1;
  /* SMCR_TS  = 101, input is TI1FP1.
     SMCR_SMS = 100, reset on rising edge.*/
  icup->tim->SMCR  = TIM_SMCR_TS_2 | TIM_SMCR_TS_0 |
                     TIM_SMCR_SMS_2;
  /* The CCER settings depend on the selected trigger mode.
     ICU_INPUT_ACTIVE_HIGH: Active on rising edge, idle on falling edge.
     ICU_INPUT_ACTIVE_LOW:  Active on falling edge, idle on rising edge.*/
  if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH)
    icup->tim->CCER = TIM_CCER_CC1E |
                      TIM_CCER_CC2E | TIM_CCER_CC2P;
  else
    icup->tim->CCER = TIM_CCER_CC1E | TIM_CCER_CC1P |
                      TIM_CCER_CC2E;
}

/**
 * @brief   Deactivates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_stop(ICUDriver *icup) {

  if (icup->state == ICU_READY) {
    /* Clock deactivation.*/
    icup->tim->CR1  = 0;                    /* Timer disabled.              */
    icup->tim->DIER = 0;                    /* All IRQs disabled.           */

#if STM32_ICU_USE_TIM1
    if (&ICUD1 == icup) {
      NVICDisableVector(TIM1_CC_IRQn);
      RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;
    }
#endif
  }
#if STM32_ICU_USE_TIM2
    if (&ICUD2 == icup) {
      NVICDisableVector(TIM2_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
    }
#endif
#if STM32_ICU_USE_TIM3
    if (&ICUD3 == icup) {
      NVICDisableVector(TIM3_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;
    }
#endif
#if STM32_ICU_USE_TIM4
    if (&ICUD4 == icup) {
      NVICDisableVector(TIM4_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN;
    }
#endif
#if STM32_ICU_USE_TIM5
    if (&ICUD5 == icup) {
      NVICDisableVector(TIM5_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;
    }
#endif
}

/**
 * @brief   Enables the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_enable(ICUDriver *icup) {

  icup->tim->SR   = 0;                      /* Clear pending IRQs (if any). */
  if (icup->config->period_cb != NULL)
    icup->tim->DIER |= TIM_DIER_CC1IE;
  if (icup->config->width_cb != NULL)
    icup->tim->DIER |= TIM_DIER_CC2IE;
  icup->tim->CR1  = TIM_CR1_URS | TIM_CR1_CEN;
}

/**
 * @brief   Disables the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_disable(ICUDriver *icup) {

  icup->tim->CR1  = 0;                      /* Initially stopped.           */
  icup->tim->SR   = 0;                      /* Clear pending IRQs (if any). */
  icup->tim->DIER = 0;                      /* Interrupts disabled.         */
}

#endif /* HAL_USE_ICU */

/** @} */
