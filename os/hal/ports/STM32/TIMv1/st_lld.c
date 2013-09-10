/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    STM32/st_lld.c
 * @brief   ST Driver subsystem low level driver code.
 *
 * @addtogroup ST
 * @{
 */

#include "hal.h"

#if (OSAL_ST_MODE != OSAL_ST_MODE_NONE) || defined(__DOXYGEN__)

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING

/* The following checks and settings are unusually done here because the
   file st.h needs to not have external dependencies. In this case there
   would be a dependency on osal.h and mcuconf.h.*/
#if !defined(HAL_ST_USE_TIM5)

#if !STM32_HAS_TIM2
#error "TIM2 not present in the selected device"
#endif

#if !STM32_TIM2_IS_32BITS
#error "TIM2 is not a 32 bits timer"
#endif

#else /* defined(HAL_ST_USE_TIM5) */

#if !STM32_HAS_TIM5
#error "TIM5 not present in the selected device"
#endif

#if !STM32_TIM5_IS_32BITS
#error "TIM5 is not a 32 bits timer"
#endif
#endif /* defined(HAL_ST_USE_TIM5) */

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SysTick timer IRQ priority.
 */
#if !defined(STM32_ST_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ST_IRQ_PRIORITY               8
#endif
/** @} */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC) || defined(__DOXYGEN__)
/**
 * @brief   System Timer vector.
 * @details This interrupt is used for system tick in periodic mode.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SysTick_Handler) {

  OSAL_IRQ_PROLOGUE();

  osalSysLockFromISR();
  osalOsTimerHandlerI();
  osalSysUnlockFromISR();

  OSAL_IRQ_EPILOGUE();
}
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) || defined(__DOXYGEN__)
/**
 * @brief   TIM2 interrupt handler.
 * @details This interrupt is used for system tick in free running mode.
 *
 * @isr
 */
#if !defined(HAL_ST_USE_TIM5)
OSAL_IRQ_HANDLER(STM32_TIM2_HANDLER) {
#else
OSAL_IRQ_HANDLER(STM32_TIM5_HANDLER) {
#endif

  OSAL_IRQ_PROLOGUE();

  ST_TIM->SR = 0;

  osalSysLockFromISR();
  osalOsTimerHandlerI();
  osalSysUnlockFromISR();

  OSAL_IRQ_EPILOGUE();
}
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ST driver initialization.
 *
 * @notapi
 */
void st_lld_init(void) {

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
  /* Free running counter mode.*/
#if !defined(HAL_ST_USE_TIM5)
  rccEnableTIM2(FALSE);
#else
  rccEnableTIM5(FALSE);
#endif

  /* Initializing the counter in free running mode.*/
  ST_TIM->PSC    = STM32_TIMCLK1 / OSAL_SYSTICK_FREQUENCY - 1;
  ST_TIM->ARR    = 0xFFFFFFFF;
  ST_TIM->CCMR1  = 0;
  ST_TIM->CCR[0] = 0;
  ST_TIM->DIER   = 0;
  ST_TIM->CR2    = 0;
  ST_TIM->EGR    = TIM_EGR_UG;
  ST_TIM->CR1    = TIM_CR1_CEN;

  /* IRQ enabled.*/
#if !defined(HAL_ST_USE_TIM5)
  nvicEnableVector(STM32_TIM2_NUMBER, STM32_ST_IRQ_PRIORITY);
#else
  nvicEnableVector(STM32_TIM5_NUMBER, STM32_ST_IRQ_PRIORITY);
#endif
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
  /* Periodic systick mode, the Cortex-Mx internal systick timer is used
     in this mode.*/
  SysTick->LOAD = STM32_HCLK / OSAL_SYSTICK_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* IRQ enabled.*/
  nvicSetSystemHandlerPriority(SysTick_IRQn, STM32_ST_IRQ_PRIORITY);
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */
}

#endif /* OSAL_ST_MODE != OSAL_ST_MODE_NONE */

/** @} */
