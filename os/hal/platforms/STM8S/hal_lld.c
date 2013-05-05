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
 * @file    STM8S/hal_lld.c
 * @brief   STM8S HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
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

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 * @details Clock sources initialization, HSI is assumed to be already
 *          started after reset.
 * @note    If the @p STM8S_CLOCK_INIT option is set to @p FALSE then the
 *          initialization is not performed and is left to the application.
 *
 * @notapi
 */
void hal_lld_init(void) {

#if !STM8S_NO_CLOCK_INIT
  /* Makes sure that HSI is stable before proceeding.*/
  CLK->ICKR |= CLK_ICKR_HSIEN;
  while ((CLK->ICKR & CLK_ICKR_HSIRDY) == 0)
    ;

  /* LSI startup and stabilization if required.*/
#if STM8S_LSI_ENABLED
  CLK->ICKR |= CLK_ICKR_LSIEN;
  while ((CLK->ICKR & CLK_ICKR_LSIRDY) == 0)
    ;
#endif

  /* HSE startup and stabilization if required.*/
#if STM8S_HSE_ENABLED
  CLK->ECKR |= CLK_ECKR_HSEEN;
  while ((CLK->ECKR & CLK_ECKR_HSERDY) == 0)
    ;
#endif

  /* Setting up clock dividers.*/
  CLK->CKDIVR  = (STM8S_HSI_DIVIDER << 3) | (STM8S_CPU_DIVIDER << 0);

  /* SYSCLK switch to the selected source, not necessary if it is HSI.*/
#if STM8S_SYSCLK_SOURCE != CLK_SYSSEL_HSI
  /* Switching clock (manual switch mode).*/
  CLK->SWR  = STM8S_SYSCLK_SOURCE;
  while ((CLK->SWCR & CLK_SWCR_SWIF) == 0)
    ;
  CLK->SWCR = CLK_SWCR_SWEN;
#endif

  /* Clocks initially all disabled.*/
  CLK->PCKENR1 = 0;
  CLK->PCKENR2 = 0;

  /* Other clock related initializations.*/
  CLK->CSSR    = 0;
  CLK->CCOR    = 0;

  /* HSI disabled if it is no more required.*/
#if !STM8S_HSI_ENABLED
  CLK->ICKR &= ~CLK_ICKR_HSIEN;
#endif
#endif /* !STM8S_NO_CLOCK_INIT */
}

/** @} */
