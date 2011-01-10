/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM32/pwm_lld.c
 * @brief   STM32 PWM subsystem low level driver header.
 *
 * @addtogroup STM32_PWM
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PWM1 driver identifier.
 * @note    The driver PWM1 allocates the complex timer TIM1 when enabled.
 */
#if USE_STM32_PWM1 || defined(__DOXYGEN__)
PWMDriver PWMD1;
#endif

/**
 * @brief   PWM2 driver identifier.
 * @note    The driver PWM2 allocates the timer TIM2 when enabled.
 */
#if USE_STM32_PWM2 || defined(__DOXYGEN__)
PWMDriver PWMD2;
#endif

/**
 * @brief   PWM3 driver identifier.
 * @note    The driver PWM3 allocates the timer TIM3 when enabled.
 */
#if USE_STM32_PWM3 || defined(__DOXYGEN__)
PWMDriver PWMD3;
#endif

/**
 * @brief   PWM4 driver identifier.
 * @note    The driver PWM4 allocates the timer TIM4 when enabled.
 */
#if USE_STM32_PWM4 || defined(__DOXYGEN__)
PWMDriver PWMD4;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if USE_STM32_PWM2 || USE_STM32_PWM3 || USE_STM32_PWM4 || defined(__DOXYGEN__)
/**
 * @brief   Common TIM2...TIM4 IRQ handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 */
static void serve_interrupt(PWMDriver *pwmp) {
  uint16_t sr;

  sr = pwmp->pd_tim->SR & pwmp->pd_tim->DIER;
  pwmp->pd_tim->SR = ~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF |
                       TIM_SR_CC4IF | TIM_SR_UIF);
  if ((sr & TIM_SR_CC1IF) != 0)
    pwmp->pd_config->pc_channels[0].pcc_callback(pwmp);
  if ((sr & TIM_SR_CC2IF) != 0)
    pwmp->pd_config->pc_channels[1].pcc_callback(pwmp);
  if ((sr & TIM_SR_CC3IF) != 0)
    pwmp->pd_config->pc_channels[2].pcc_callback(pwmp);
  if ((sr & TIM_SR_CC4IF) != 0)
    pwmp->pd_config->pc_channels[3].pcc_callback(pwmp);
  if ((sr & TIM_SR_UIF) != 0)
    pwmp->pd_config->pc_callback(pwmp);
}
#endif /* USE_STM32_PWM2 || USE_STM32_PWM3 || USE_STM32_PWM4 */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_STM32_PWM1
/**
 * @brief   TIM1 update interrupt handler.
 * @note    It is assumed that this interrupt is only activated if the callback
 *          pointer is not equal to @p NULL in order to not perform an extra
 *          check in a potentially critical interrupt handler.
 */
CH_IRQ_HANDLER(VectorA4) {

  CH_IRQ_PROLOGUE();

  TIM1->SR = ~TIM_SR_UIF;
  PWMD1.pd_config->pc_callback(&PWMD1);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   TIM1 compare interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 */
CH_IRQ_HANDLER(VectorAC) {
  uint16_t sr;

  CH_IRQ_PROLOGUE();

  sr = TIM1->SR & TIM1->DIER;
  TIM1->SR = ~(TIM_SR_CC1IF | TIM_SR_CC2IF | TIM_SR_CC3IF | TIM_SR_CC4IF);
  if ((sr & TIM_SR_CC1IF) != 0)
    PWMD1.pd_config->pc_channels[0].pcc_callback(&PWMD1);
  if ((sr & TIM_SR_CC2IF) != 0)
    PWMD1.pd_config->pc_channels[1].pcc_callback(&PWMD1);
  if ((sr & TIM_SR_CC3IF) != 0)
    PWMD1.pd_config->pc_channels[2].pcc_callback(&PWMD1);
  if ((sr & TIM_SR_CC4IF) != 0)
    PWMD1.pd_config->pc_channels[3].pcc_callback(&PWMD1);

  CH_IRQ_EPILOGUE();
}
#endif /* USE_STM32_PWM1 */

#if USE_STM32_PWM2
/**
 * @brief   TIM2 interrupt handler.
 */
CH_IRQ_HANDLER(VectorB0) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&PWMD2);

  CH_IRQ_EPILOGUE();
}
#endif /* USE_STM32_PWM2 */

#if USE_STM32_PWM3
/**
 * @brief   TIM3 interrupt handler.
 */
CH_IRQ_HANDLER(VectorB4) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&PWMD3);

  CH_IRQ_EPILOGUE();
}
#endif /* USE_STM32_PWM3 */

#if USE_STM32_PWM4
/**
 * @brief   TIM4 interrupt handler.
 */
CH_IRQ_HANDLER(VectorB8) {

  CH_IRQ_PROLOGUE();

  serve_interrupt(&PWMD4);

  CH_IRQ_EPILOGUE();
}
#endif /* USE_STM32_PWM4 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PWM driver initialization.
 */
void pwm_lld_init(void) {

#if USE_STM32_PWM1
  /* TIM1 reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB2RSTR = RCC_APB2RSTR_TIM1RST;
  RCC->APB2RSTR = 0;

  /* Driver initialization.*/
  pwmObjectInit(&PWMD1);
  PWMD1.pd_enabled_channels = 0;
  PWMD1.pd_tim = TIM1;
#endif

#if USE_STM32_PWM2
  /* TIM2 reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB1RSTR = RCC_APB1RSTR_TIM2RST;
  RCC->APB1RSTR = 0;

  /* Driver initialization.*/
  pwmObjectInit(&PWMD2);
  PWMD2.pd_enabled_channels = 0;
  PWMD2.pd_tim = TIM2;
#endif

#if USE_STM32_PWM3
  /* TIM2 reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB1RSTR = RCC_APB1RSTR_TIM3RST;
  RCC->APB1RSTR = 0;

  /* Driver initialization.*/
  pwmObjectInit(&PWMD3);
  PWMD3.pd_enabled_channels = 0;
  PWMD3.pd_tim = TIM3;
#endif

#if USE_STM32_PWM4
  /* TIM2 reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB1RSTR = RCC_APB1RSTR_TIM4RST;
  RCC->APB1RSTR = 0;

  /* Driver initialization.*/
  pwmObjectInit(&PWMD4);
  PWMD4.pd_enabled_channels = 0;
  PWMD4.pd_tim = TIM4;
#endif
}

/**
 * @brief   Configures and activates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 */
void pwm_lld_start(PWMDriver *pwmp) {
  uint16_t ccer;

  /* Reset channels.*/
  pwmp->pd_enabled_channels = 0;            /* All channels disabled.       */

  if (pwmp->pd_state == PWM_STOP) {
    /* Clock activation and timer reset.*/
#if USE_STM32_PWM1
    if (&PWMD1 == pwmp) {
      RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
      RCC->APB2RSTR = RCC_APB2RSTR_TIM1RST;
      RCC->APB2RSTR = 0;
      NVICEnableVector(TIM1_UP_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_PWM1_IRQ_PRIORITY));
      NVICEnableVector(TIM1_CC_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_PWM1_IRQ_PRIORITY));
    }
#endif
#if USE_STM32_PWM2
    if (&PWMD2 == pwmp) {
      RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
      RCC->APB1RSTR = RCC_APB1RSTR_TIM2RST;
      RCC->APB1RSTR = 0;
      NVICEnableVector(TIM2_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_PWM2_IRQ_PRIORITY));
    }
#endif
#if USE_STM32_PWM3
    if (&PWMD3 == pwmp) {
      RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
      RCC->APB1RSTR = RCC_APB1RSTR_TIM3RST;
      RCC->APB1RSTR = 0;
      NVICEnableVector(TIM3_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_PWM3_IRQ_PRIORITY));
    }
#endif
#if USE_STM32_PWM4
    if (&PWMD4 == pwmp) {
      RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
      RCC->APB1RSTR = RCC_APB1RSTR_TIM4RST;
      RCC->APB1RSTR = 0;
      NVICEnableVector(TIM4_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_PWM4_IRQ_PRIORITY));
    }
#endif

    /* All channels configured in PWM1 mode with preload enabled and will
       stay that way until the driver is stopped.*/
    pwmp->pd_tim->CCMR1 = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |
                          TIM_CCMR1_OC1PE |
                          TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 |
                          TIM_CCMR1_OC2PE;
    pwmp->pd_tim->CCMR2 = TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 |
                          TIM_CCMR2_OC3PE |
                          TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 |
                          TIM_CCMR2_OC4PE;
  }
  else {
    /* Driver re-configuration scenario, it must be stopped first.*/
    /* Really required ?????????? */
    pwmp->pd_tim->CR1  = 0;                   /* Timer stopped.               */
    pwmp->pd_tim->CR2  = 0;                   /* Timer stopped.               */
    pwmp->pd_tim->SMCR = 0;                   /* Slave mode disabled.         */
    pwmp->pd_tim->CCR1 = 0;                   /* Comparator 1 disabled.       */
    pwmp->pd_tim->CCR2 = 0;                   /* Comparator 2 disabled.       */
    pwmp->pd_tim->CCR3 = 0;                   /* Comparator 3 disabled.       */
    pwmp->pd_tim->CCR4 = 0;                   /* Comparator 4 disabled.       */
    pwmp->pd_tim->CNT  = 0;
  }

  /* Timer configuration.*/
  pwmp->pd_tim->CR2  = pwmp->pd_config->pc_cr2;
  pwmp->pd_tim->PSC  = pwmp->pd_config->pc_psc;
  pwmp->pd_tim->ARR  = pwmp->pd_config->pc_arr;
  /* Output enables and polarities setup.*/
  ccer = 0;
  switch (pwmp->pd_config->pc_channels[0].pcc_mode) {
  case PWM_OUTPUT_ACTIVE_LOW:
    ccer |= TIM_CCER_CC1P;
  case PWM_OUTPUT_ACTIVE_HIGH:
    ccer |= TIM_CCER_CC1E;
  default:
    ;
  }
  switch (pwmp->pd_config->pc_channels[1].pcc_mode) {
  case PWM_OUTPUT_ACTIVE_LOW:
    ccer |= TIM_CCER_CC2P;
  case PWM_OUTPUT_ACTIVE_HIGH:
    ccer |= TIM_CCER_CC2E;
  default:
    ;
  }
  switch (pwmp->pd_config->pc_channels[2].pcc_mode) {
  case PWM_OUTPUT_ACTIVE_LOW:
    ccer |= TIM_CCER_CC3P;
  case PWM_OUTPUT_ACTIVE_HIGH:
    ccer |= TIM_CCER_CC3E;
  default:
    ;
  }
  switch (pwmp->pd_config->pc_channels[3].pcc_mode) {
  case PWM_OUTPUT_ACTIVE_LOW:
    ccer |= TIM_CCER_CC4P;
  case PWM_OUTPUT_ACTIVE_HIGH:
    ccer |= TIM_CCER_CC4E;
  default:
    ;
  }
  pwmp->pd_tim->CCER = ccer;
  pwmp->pd_tim->EGR  = TIM_EGR_UG;          /* Update event.                */
  pwmp->pd_tim->SR   = 0;                   /* Clear pending IRQs.          */
  pwmp->pd_tim->DIER = pwmp->pd_config->pc_callback == NULL ? 0 : TIM_DIER_UIE;
  pwmp->pd_tim->BDTR = TIM_BDTR_MOE;
  /* Timer configured and started.*/
  pwmp->pd_tim->CR1  = TIM_CR1_ARPE | TIM_CR1_URS | TIM_CR1_CEN;
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 */
void pwm_lld_stop(PWMDriver *pwmp) {
  /* If in ready state then disables the PWM clock.*/
  if (pwmp->pd_state == PWM_READY) {
    pwmp->pd_enabled_channels = 0;          /* All channels disabled.       */
    pwmp->pd_tim->CR1   = 0;
    pwmp->pd_tim->CR2   = 0;
    pwmp->pd_tim->CCER  = 0;                /* Outputs disabled.            */
    pwmp->pd_tim->CCR1  = 0;                /* Comparator 1 disabled.       */
    pwmp->pd_tim->CCR2  = 0;                /* Comparator 2 disabled.       */
    pwmp->pd_tim->CCR3  = 0;                /* Comparator 3 disabled.       */
    pwmp->pd_tim->CCR4  = 0;                /* Comparator 4 disabled.       */
    pwmp->pd_tim->BDTR  = 0;
    pwmp->pd_tim->DIER  = 0;
    pwmp->pd_tim->SR    = 0;
    pwmp->pd_tim->EGR  = TIM_EGR_UG;        /* Update event.                */

#if USE_STM32_PWM1
    if (&PWMD1 == pwmp) {
      NVICDisableVector(TIM1_UP_IRQn);
      NVICDisableVector(TIM1_CC_IRQn);
      RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;
    }
#endif
#if USE_STM32_PWM2
    if (&PWMD2 == pwmp) {
      NVICDisableVector(TIM2_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
    }
#endif
#if USE_STM32_PWM3
    if (&PWMD3 == pwmp) {
      NVICDisableVector(TIM3_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;
    }
#endif
#if USE_STM32_PWM4
    if (&PWMD4 == pwmp) {
      NVICDisableVector(TIM4_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN;
    }
#endif
  }
}

/**
 * @brief   Enables a PWM channel.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 * @param[in] width     PWM pulse width as clock pulses number
 */
void pwm_lld_enable_channel(PWMDriver *pwmp,
                            pwmchannel_t channel,
                            pwmcnt_t width) {

  *(&pwmp->pd_tim->CCR1 + (channel * 2)) = width;   /* New duty cycle.      */
  if ((pwmp->pd_enabled_channels & (1 << channel)) == 0) {
    /* The channel is not enabled yet.*/
    pwmp->pd_enabled_channels |= (1 << channel);
    /* If there is a callback associated to the channel then the proper
       interrupt is cleared and enabled.*/
    if (pwmp->pd_config->pc_channels[channel].pcc_callback) {
      pwmp->pd_tim->SR    = ~(2 << channel);
      pwmp->pd_tim->DIER |= (2 << channel);
    }
  }
}

/**
 * @brief   Disables a PWM channel.
 * @details The channel is disabled and its output line returned to the
 *          idle state.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 */
void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {

  *(&pwmp->pd_tim->CCR1 + (channel * 2)) = 0;
  pwmp->pd_tim->DIER &= ~(2 << channel);
  pwmp->pd_enabled_channels &= ~(1 << channel);
}

#endif /* CH_HAL_USE_PWM */

/** @} */
