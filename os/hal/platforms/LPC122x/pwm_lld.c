/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC122x PWM driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC122x/pwm_lld.c
 * @brief   LPC122x PWM subsystem low level driver header.
 *
 * @addtogroup PWM
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PWMD1 driver identifier.
 * @note    The driver PWMD1 allocates the complex timer TIM1 when enabled.
 */
#if LPC122x_PWM_USE_CT16B0 || defined(__DOXYGEN__)
PWMDriver PWMD1;
#endif

/**
 * @brief   PWMD2 driver identifier.
 * @note    The driver PWMD2 allocates the timer TIM2 when enabled.
 */
#if LPC122x_PWM_USE_CT16B1 || defined(__DOXYGEN__)
PWMDriver PWMD2;
#endif

/**
 * @brief   PWMD3 driver identifier.
 * @note    The driver PWMD3 allocates the timer TIM3 when enabled.
 */
#if LPC122x_PWM_USE_CT32B0 || defined(__DOXYGEN__)
PWMDriver PWMD3;
#endif

/**
 * @brief   PWMD4 driver identifier.
 * @note    The driver PWMD4 allocates the timer TIM4 when enabled.
 */
#if LPC122x_PWM_USE_CT32B1 || defined(__DOXYGEN__)
PWMDriver PWMD4;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if LPC122x_PWM_USE_CT16B0 || LPC122x_PWM_USE_CT16B1 ||                       \
  LPC122x_PWM_USE_CT32B0 || LPC122x_PWM_USE_CT32B1 || defined(__DOXYGEN__)
/**
 * @brief   Common TIM2...TIM5 IRQ handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 */
static void pwm_lld_serve_interrupt(PWMDriver *pwmp) {
  uint16_t sr;

  sr  = pwmp->tim->IR;
  pwmp->tim->IR = sr;
  if ((sr & IR_MR0INT) != 0)
    pwmp->config->channels[0].callback(pwmp);
  if ((sr & IR_MR1INT) != 0)
    pwmp->config->channels[1].callback(pwmp);
  if ((sr & IR_MR3INT) != 0)
    pwmp->config->callback(pwmp);
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if LPC122x_PWM_USE_CT16B0 || defined(__DOXYGEN__)
/**
 * @brief   CT16B0 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(Vector74) {

  CH_IRQ_PROLOGUE();
  pwm_lld_serve_interrupt(&PWMD1);
  CH_IRQ_EPILOGUE();
}

#endif /* STM32_PWM_USE_TIM1 */

#if LPC122x_PWM_USE_CT16B1 || defined(__DOXYGEN__)
/**
 * @brief   CT16B1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(Vector78) {

  CH_IRQ_PROLOGUE();
  pwm_lld_serve_interrupt(&PWMD2);
  CH_IRQ_EPILOGUE();
}
#endif /* LPC122x_PWM_USE_CT16B1 */

#if LPC122x_PWM_USE_CT32B0 || defined(__DOXYGEN__)
/**
 * @brief   CT32B0 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(Vector7C) {

  CH_IRQ_PROLOGUE();
  pwm_lld_serve_interrupt(&PWMD3);
  CH_IRQ_EPILOGUE();
}
#endif /* LPC122x_PWM_USE_CT32B0 */

#if LPC122x_PWM_USE_CT32B1 || defined(__DOXYGEN__)
/**
 * @brief   CT32B1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(Vector80) {

  CH_IRQ_PROLOGUE();
  pwm_lld_serve_interrupt(&PWMD4);
  CH_IRQ_EPILOGUE();
}
#endif /* LPC122x_PWM_USE_CT32B1 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PWM driver initialization.
 *
 * @notapi
 */
void pwm_lld_init(void) {

#if LPC122x_PWM_USE_CT16B0
  /* Driver initialization.*/
  pwmObjectInit(&PWMD1);
  PWMD1.tim = LPC_CT16B0;

#if LPC122x_PWM_USE_CT16B0_CH0
#if LPC122x_PWM_CT16B0_CH0_SELECTOR == PWM_CT16B0_CH0_IS_PIO0_11
  LPC_IOCON->PIO0_11 = 0x84;
#elif LPC122x_PWM_CT16B0_CH0_SELECTOR == PWM_CT16B0_CH0_IS_PIO0_28
  LPC_IOCON->PIO0_28 = 0x84;
#else /* LPC122x_PWM_CT16B0_CH0_SELECTOR == PWM_CT16B0_CH0_IS_PIO2_0  */
  LPC_IOCON->PIO2_0 = 0x84;
#endif
#endif

#if LPC122x_PWM_USE_CT16B0_CH1
#if LPC122x_PWM_CT16B0_CH1_SELECTOR == PWM_CT16B0_CH1_IS_PIO0_12
  LPC_IOCON->PIO0_12 = 0x84;
#elif LPC122x_PWM_CT16B0_CH1_SELECTOR == PWM_CT16B0_CH1_IS_PIO0_29
  LPC_IOCON->PIO0_29 = 0x84;
#else /* LPC122x_PWM_CT16B0_CH1_SELECTOR == PWM_CT16B0_CH1_IS_PIO2_1  */
  LPC_IOCON->PIO2_1 = 0x83;
#endif
#endif
#endif

#if LPC122x_PWM_USE_CT16B1
  /* Driver initialization.*/
  pwmObjectInit(&PWMD2);
  PWMD2.tim = LPC_CT16B1;

#if LPC122x_PWM_USE_CT16B1_CH0
#if LPC122x_PWM_CT16B1_CH0_SELECTOR == PWM_CT16B1_CH0_IS_PIO0_15
  LPC_IOCON->PIO0_15 = 0x84;
#elif LPC122x_PWM_CT16B1_CH0_SELECTOR == PWM_CT16B1_CH0_IS_PIO1_5
  LPC_IOCON->PIO1_5 = 0x83;
#else /* LPC122x_PWM_CT16B1_CH0_SELECTOR == PWM_CT16B1_CH0_IS_PIO2_2 */
  LPC_IOCON->PIO2_2 = 0x83;
#endif
#endif

#if LPC122x_PWM_USE_CT16B1_CH1
#if LPC122x_PWM_CT16B1_CH1_SELECTOR == PWM_CT16B1_CH1_IS_PIO0_16
  LPC_IOCON->PIO0_16 = 0x84;
#elif LPC122x_PWM_CT16B1_CH1_SELECTOR == PWM_CT16B1_CH1_IS_PIO1_6
  LPC_IOCON->PIO1_6 = 0x82;
#else /* LPC122x_PWM_CT16B1_CH1_SELECTOR == PWM_CT16B1_CH1_IS_PIO2_3 */
  LPC_IOCON->PIO2_3 = 0x83;
#endif
#endif
#endif

#if LPC122x_PWM_USE_CT32B0
  /* Driver initialization.*/
  pwmObjectInit(&PWMD3);
  PWMD3.tim = LPC_CT32B0;

#if LPC122x_PWM_USE_CT32B0_CH0
#if LPC122x_PWM_CT32B0_CH0_SELECTOR == PWM_CT32B0_CH0_IS_PIO0_1
  LPC_IOCON->PIO0_1 = 0x84;
#elif LPC122x_PWM_CT32B0_CH0_SELECTOR == PWM_CT32B0_CH0_IS_PIO0_18
  LPC_IOCON->PIO0_18 = 0x84;
#else /* LPC122x_PWM_CT32B0_CH0_SELECTOR == PWM_CT32B0_CH0_IS_PIO2_4 */
  LPC_IOCON->PIO2_4 = 0x83;
#endif
#endif

#if  LPC122x_PWM_USE_CT32B0_CH1
#if LPC122x_PWM_CT32B0_CH1_SELECTOR == PWM_CT32B0_CH1_IS_PIO0_2
  LPC_IOCON->PIO0_2 = 0x84;
#elif LPC122x_PWM_CT32B0_CH1_SELECTOR == PWM_CT32B0_CH1_IS_PIO0_19
  LPC_IOCON->PIO0_19 = 0x84;
#else /* LPC122x_PWM_CT32B0_CH1_SELECTOR == PWM_CT32B0_CH1_IS_PIO2_5  */
  LPC_IOCON->PIO2_5 = 0x83;
#endif
#endif
#endif

#if LPC122x_PWM_USE_CT32B1
  /* Driver initialization.*/
  pwmObjectInit(&PWMD4);
  PWMD4.tim = LPC_CT32B1;

#if LPC122x_PWM_USE_CT32B1_CH0
#if LPC122x_PWM_CT32B1_CH0_SELECTOR == PWM_CT32B1_CH0_IS_PIO0_6
  LPC_IOCON->PIO0_6 = 0x84;
#elif LPC122x_PWM_CT32B1_CH0_SELECTOR == PWM_CT32B1_CH0_IS_PIO0_23
  LPC_IOCON->PIO0_23 = 0x84;
#else /* LPC122x_PWM_CT32B1_CH0_SELECTOR == PWM_CT32B1_CH0_IS_PIO2_8 */
  LPC_IOCON->PIO2_8 = 0x83;
#endif
#endif

#if LPC122x_PWM_USE_CT32B1_CH1
#if LPC122x_PWM_CT32B1_CH1_SELECTOR == PWM_CT32B1_CH1_IS_PIO0_7
  LPC_IOCON->PIO0_7 = 0x84;
#elif LPC122x_PWM_CT32B1_CH1_SELECTOR == PWM_CT32B1_CH1_IS_PIO0_24
  LPC_IOCON->PIO0_24 = 0x84;
#else /* LPC122x_PWM_CT32B1_CH1_SELECTOR == PWM_CT32B1_CH1_IS_PIO2_9  */
  LPC_IOCON->PIO2_9 = 0x83;
#endif
#endif
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
  uint32_t pr;

  if (pwmp->state == PWM_STOP) {
    /* Clock activation and timer reset.*/
#if LPC122x_PWM_USE_CT16B0
    if (&PWMD1 == pwmp) {
      LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 7);
      nvicEnableVector(TIMER_16_0_IRQn, LPC122x_PWM_CT16B0_IRQ_PRIORITY);
    }
#endif
#if LPC122x_PWM_USE_CT16B1
    if (&PWMD2 == pwmp) {
      LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 8);
      nvicEnableVector(TIMER_16_1_IRQn, LPC122x_PWM_CT16B1_IRQ_PRIORITY);
    }
#endif
#if LPC122x_PWM_USE_CT32B0
    if (&PWMD3 == pwmp) {
      LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 9);
      nvicEnableVector(TIMER_32_0_IRQn, LPC122x_PWM_CT32B0_IRQ_PRIORITY);
    }
#endif
#if LPC122x_PWM_USE_CT32B1
     if (&PWMD4 == pwmp) {
       LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 10);
       nvicEnableVector(TIMER_32_1_IRQn, LPC122x_PWM_CT32B1_IRQ_PRIORITY);
     }
#endif
  }
  else {
    /* Driver re-configuration scenario, it must be stopped first.*/
    pwmp->tim->TCR = 0;
  }

  /* Output enables and polarities setup.*/
  if(pwmp->config->channels[0].mode == PWM_OUTPUT_ACTIVE_LOW)
    pwmp->tim->PWMC = (1 << 0);

  if(pwmp->config->channels[1].mode == PWM_OUTPUT_ACTIVE_LOW)
    pwmp->tim->PWMC |= (1 << 1);

  /* Timer configured and started.*/
  pr = (uint16_t)((LPC122x_SYSCLK / pwmp->config->frequency) - 1);
  chDbgAssert(((uint32_t)(pr + 1) * pwmp->config->frequency) == LPC122x_SYSCLK,
      "pwm_lld_start(), #1", "invalid frequency");

  pwmp->tim->TC = 0;
  pwmp->tim->PR  = pr;
  pwmp->tim->IR  = 0xFF;
  pwmp->tim->MCR = MCR_MR3R;              /* Reset on Match3 */
  pwmp->tim->MR3 = pwmp->config->period;

  if (pwmp->config->callback != NULL)
    pwmp->tim->MCR |= MCR_MR3I;

  pwmp->tim->TCR = 1;                     /* Timer start */
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_stop(PWMDriver *pwmp) {

  /* If in ready state then disables the PWM clock.*/
  if (pwmp->state == PWM_READY) {
    pwmp->tim->TCR = 0;                    /* Timer disabled.              */
    pwmp->tim->MCR = 0;                    /* All IRQs disabled.           */
    pwmp->tim->IR  = 0xFF;                 /* Clear eventual pending IRQs. */
    pwmp->tim->PWMC = 0;                   /* PWM outputs disable          */

#if LPC122x_PWM_USE_CT16B0
    if (&PWMD1 == pwmp) {
      nvicDisableVector(TIMER_16_0_IRQn);
      LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 7);
    }
#endif
#if LPC122x_PWM_USE_CT16B1
    if (&PWMD2 == pwmp) {
      nvicDisableVector(TIMER_16_1_IRQn);
      LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 8);
    }
#endif
#if LPC122x_PWM_USE_CT32B0
    if (&PWMD3 == pwmp) {
      nvicDisableVector(TIMER_32_0_IRQn);
      LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 9);
    }
#endif
#if LPC122x_PWM_USE_CT32B1
    if (&PWMD4 == pwmp) {
      nvicDisableVector(TIMER_32_1_IRQn);
      LPC_SYSCON->SYSAHBCLKCTRL &= ~(1 << 10);
    }
#endif
  }
}

/**
 * @brief   Enables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is active using the specified configuration.
 * @note    The function has effect at the next cycle start.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 * @param[in] width     PWM pulse width as clock pulses number
 *
 * @notapi
 */
void pwm_lld_enable_channel(PWMDriver *pwmp,
                            pwmchannel_t channel,
                            pwmcnt_t width) {

  pwmp->tim->MCR &= ~(7 << (channel * 3));

  if ( channel == 0)
    pwmp->tim->MR0 = width;                  /* New duty cycle. */
  else
    pwmp->tim->MR1 = width;                  /* New duty cycle. */
  /* If there is a callback defined for the channel then the associated
     interrupt must be enabled.*/
  if (pwmp->config->channels[channel].callback != NULL) {
    pwmp->tim->IR = (1 << channel);         /* Clear interrupt flag*/
    pwmp->tim->MCR |= (1 << (channel * 3)); /* Set interrupt on selected channel */
  }

}

/**
 * @brief   Disables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is disabled and its output line returned to the
 *          idle state.
 * @note    The function has effect at the next cycle start.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 *
 * @notapi
 */
void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {

  if ( channel == 0)
    pwmp->tim->MR0 = 0;
  else
    pwmp->tim->MR1 = 0;
  pwmp->tim->MCR &= ~(7 << (channel * 3));
  pwmp->tim->IR = (1 << channel);
}

#endif /* HAL_USE_PWM */

/** @} */
