/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    STM32/gpt_lld.c
 * @brief   STM32 GPT subsystem low level driver source.
 *
 * @addtogroup GPT
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   GPTD1 driver identifier.
 * @note    The driver GPTD1 allocates the complex timer TIM1 when enabled.
 */
#if STM32_GPT_USE_TIM1 || defined(__DOXYGEN__)
GPTDriver GPTD1;
#endif

/**
 * @brief   GPTD2 driver identifier.
 * @note    The driver GPTD2 allocates the timer TIM2 when enabled.
 */
#if STM32_GPT_USE_TIM2 || defined(__DOXYGEN__)
GPTDriver GPTD2;
#endif

/**
 * @brief   GPTD3 driver identifier.
 * @note    The driver GPTD3 allocates the timer TIM3 when enabled.
 */
#if STM32_GPT_USE_TIM3 || defined(__DOXYGEN__)
GPTDriver GPTD3;
#endif

/**
 * @brief   GPTD4 driver identifier.
 * @note    The driver GPTD4 allocates the timer TIM4 when enabled.
 */
#if STM32_GPT_USE_TIM4 || defined(__DOXYGEN__)
GPTDriver GPTD4;
#endif

/**
 * @brief   GPTD5 driver identifier.
 * @note    The driver GPTD5 allocates the timer TIM5 when enabled.
 */
#if STM32_GPT_USE_TIM5 || defined(__DOXYGEN__)
GPTDriver GPTD5;
#endif

/**
 * @brief   GPTD8 driver identifier.
 * @note    The driver GPTD8 allocates the timer TIM8 when enabled.
 */
#if STM32_GPT_USE_TIM8 || defined(__DOXYGEN__)
GPTDriver GPTD8;
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
 * @param[in] gptp      pointer to a @p GPTDriver object
 */
static void gpt_lld_serve_interrupt(GPTDriver *gptp) {

  gptp->tim->SR = 0;
  if (gptp->state == GPT_ONESHOT) {
    gptp->state = GPT_READY;                /* Back in GPT_READY state.     */
    gpt_lld_stop_timer(gptp);               /* Timer automatically stopped. */
  }
  gptp->config->callback(gptp);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_GPT_USE_TIM1
/**
 * @brief   TIM2 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM1_UP_IRQHandler) {

  CH_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD1);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_GPT_USE_TIM1 */

#if STM32_GPT_USE_TIM2
/**
 * @brief   TIM2 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM2_IRQHandler) {

  CH_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD2);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_GPT_USE_TIM2 */

#if STM32_GPT_USE_TIM3
/**
 * @brief   TIM3 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM3_IRQHandler) {

  CH_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD3);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_GPT_USE_TIM3 */

#if STM32_GPT_USE_TIM4
/**
 * @brief   TIM4 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM4_IRQHandler) {

  CH_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD4);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_GPT_USE_TIM4 */

#if STM32_GPT_USE_TIM5
/**
 * @brief   TIM5 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM5_IRQHandler) {

  CH_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD5);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_GPT_USE_TIM5 */

#if STM32_GPT_USE_TIM8
/**
 * @brief   TIM8 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(TIM8_IRQHandler) {

  CH_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD8);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_GPT_USE_TIM8 */

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
  /* Driver initialization.*/
  GPTD1.tim = STM32_TIM1;
  gptObjectInit(&GPTD1);
#endif

#if STM32_GPT_USE_TIM2
  /* Driver initialization.*/
  GPTD2.tim = STM32_TIM2;
  gptObjectInit(&GPTD2);
#endif

#if STM32_GPT_USE_TIM3
  /* Driver initialization.*/
  GPTD3.tim = STM32_TIM3;
  gptObjectInit(&GPTD3);
#endif

#if STM32_GPT_USE_TIM4
  /* Driver initialization.*/
  GPTD4.tim = STM32_TIM4;
  gptObjectInit(&GPTD4);
#endif

#if STM32_GPT_USE_TIM5
  /* Driver initialization.*/
  GPTD5.tim = STM32_TIM5;
  gptObjectInit(&GPTD5);
#endif

#if STM32_GPT_USE_TIM8
  /* Driver initialization.*/
  GPTD8.tim = STM32_TIM8;
  gptObjectInit(&GPTD8);
#endif
}

/**
 * @brief   Configures and activates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_start(GPTDriver *gptp) {
  uint16_t psc;

  if (gptp->state == GPT_STOP) {
    /* Clock activation.*/
#if STM32_GPT_USE_TIM1
    if (&GPTD1 == gptp) {
      rccEnableTIM1(FALSE);
      rccResetTIM1();
      nvicEnableVector(TIM1_UP_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_GPT_TIM1_IRQ_PRIORITY));
      gptp->clock = STM32_TIMCLK2;
    }
#endif
#if STM32_GPT_USE_TIM2
    if (&GPTD2 == gptp) {
      rccEnableTIM2(FALSE);
      rccResetTIM2();
      nvicEnableVector(TIM2_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_GPT_TIM2_IRQ_PRIORITY));
      gptp->clock = STM32_TIMCLK1;
    }
#endif
#if STM32_GPT_USE_TIM3
    if (&GPTD3 == gptp) {
      rccEnableTIM3(FALSE);
      rccResetTIM3();
      nvicEnableVector(TIM3_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_GPT_TIM3_IRQ_PRIORITY));
      gptp->clock = STM32_TIMCLK1;
    }
#endif
#if STM32_GPT_USE_TIM4
    if (&GPTD4 == gptp) {
      rccEnableTIM4(FALSE);
      rccResetTIM4();
      nvicEnableVector(TIM4_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_GPT_TIM4_IRQ_PRIORITY));
      gptp->clock = STM32_TIMCLK1;
    }
#endif

#if STM32_GPT_USE_TIM5
    if (&GPTD5 == gptp) {
      rccEnableTIM5(FALSE);
      rccResetTIM5();
      nvicEnableVector(TIM5_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_GPT_TIM5_IRQ_PRIORITY));
      gptp->clock = STM32_TIMCLK1;
    }
#endif

#if STM32_GPT_USE_TIM8
    if (&GPTD8 == gptp) {
      rccEnableTIM8(FALSE);
      rccResetTIM8();
      nvicEnableVector(TIM8_UP_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_GPT_TIM8_IRQ_PRIORITY));
      gptp->clock = STM32_TIMCLK2;
    }
#endif
  }

  /* Prescaler value calculation.*/
  psc = (uint16_t)((gptp->clock / gptp->config->frequency) - 1);
  chDbgAssert(((uint32_t)(psc + 1) * gptp->config->frequency) == gptp->clock,
              "gpt_lld_start(), #1", "invalid frequency");

  /* Timer configuration.*/
  gptp->tim->CR1  = 0;                      /* Initially stopped.           */
  gptp->tim->CR2  = TIM_CR2_CCDS;           /* DMA on UE (if any).          */
  gptp->tim->PSC  = psc;                    /* Prescaler value.             */
  gptp->tim->DIER = 0;
}

/**
 * @brief   Deactivates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop(GPTDriver *gptp) {

  if (gptp->state == GPT_READY) {
    gptp->tim->CR1  = 0;                    /* Timer disabled.              */
    gptp->tim->DIER = 0;                    /* All IRQs disabled.           */
    gptp->tim->SR   = 0;                    /* Clear eventual pending IRQs. */

#if STM32_GPT_USE_TIM1
    if (&GPTD1 == gptp) {
      nvicDisableVector(TIM1_UP_IRQn);
      rccDisableTIM1(FALSE);
    }
#endif
#if STM32_GPT_USE_TIM2
    if (&GPTD2 == gptp) {
      nvicDisableVector(TIM2_IRQn);
      rccDisableTIM2(FALSE);
    }
#endif
#if STM32_GPT_USE_TIM3
    if (&GPTD3 == gptp) {
      nvicDisableVector(TIM3_IRQn);
      rccDisableTIM3(FALSE);
    }
#endif
#if STM32_GPT_USE_TIM4
    if (&GPTD4 == gptp) {
      nvicDisableVector(TIM4_IRQn);
      rccDisableTIM4(FALSE);
    }
#endif
#if STM32_GPT_USE_TIM5
    if (&GPTD5 == gptp) {
      nvicDisableVector(TIM5_IRQn);
      rccDisableTIM5(FALSE);
    }
#endif
#if STM32_GPT_USE_TIM8
    if (&GPTD8 == gptp) {
      nvicDisableVector(TIM8_UP_IRQn);
      rccDisableTIM8(FALSE);
    }
#endif
  }
}

/**
 * @brief   Starts the timer in continuous mode.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  period in ticks
 *
 * @notapi
 */
void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t interval) {

  gptp->tim->ARR  = interval - 1;           /* Time constant.               */
  gptp->tim->EGR  = TIM_EGR_UG;             /* Update event.                */
  gptp->tim->CNT  = 0;                      /* Reset counter.               */
  /* NOTE: After generating the UG event it takes several clock cycles before
     SR bit 0 goes to 1. This is because the clearing of CNT has been inserted
     before the clearing of SR, to give it some time.*/
  gptp->tim->SR   = 0;                      /* Clear pending IRQs (if any). */
  gptp->tim->DIER = TIM_DIER_UIE;           /* Update Event IRQ enabled.    */
  gptp->tim->CR1  = TIM_CR1_URS | TIM_CR1_CEN;
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop_timer(GPTDriver *gptp) {

  gptp->tim->CR1  = 0;                      /* Initially stopped.           */
  gptp->tim->SR   = 0;                      /* Clear pending IRQs (if any). */
  gptp->tim->DIER = 0;                      /* Interrupts disabled.         */
}

/**
 * @brief   Starts the timer in one shot mode and waits for completion.
 * @details This function specifically polls the timer waiting for completion
 *          in order to not have extra delays caused by interrupt servicing,
 *          this function is only recommended for short delays.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  time interval in ticks
 *
 * @notapi
 */
void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval) {

  gptp->tim->ARR  = interval - 1;           /* Time constant.               */
  gptp->tim->EGR  = TIM_EGR_UG;             /* Update event.                */
  gptp->tim->SR   = 0;                      /* Clear pending IRQs (if any). */
  gptp->tim->CR1  = TIM_CR1_OPM | TIM_CR1_URS | TIM_CR1_CEN;
  while (!(gptp->tim->SR & TIM_SR_UIF))
    ;
}

#endif /* HAL_USE_GPT */

/** @} */
