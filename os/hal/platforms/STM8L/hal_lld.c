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
 * @file    STM8L/hal_lld.c
 * @brief   STM8L HAL subsystem low level driver source.
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
 * @note    If the @p STM8L_CLOCK_INIT option is set to @p FALSE then the
 *          initialization is not performed and is left to the application.
 *
 * @notapi
 */
void hal_lld_init(void) {

#if !STM8L_NO_CLOCK_INIT
  /* Makes sure that HSI is stable before proceeding.*/
  CLK->ICKCR |= CLK_ICKCR_HSION;
  while ((CLK->ICKCR & CLK_ICKCR_HSIRDY) == 0)
    ;

  /* LSI startup and stabilization if required.*/
#if STM8L_LSI_ENABLED
  CLK->ICKCR |= CLK_ICKCR_LSION;
  while ((CLK->ICKCR & CLK_ICKCR_LSIRDY) == 0)
    ;
#endif

  /* HSE startup and stabilization if required.*/
#if STM8L_HSE_ENABLED
#if HSEBYPASS
  CLK->ECKCR |= CLK_ECKCR_HSEON | CLK_ECKCR_HSEBYP;
#else
  CLK->ECKCR |= CLK_ECKCR_HSEON;
#endif
  while ((CLK->ECKCR & CLK_ECKCR_HSERDY) == 0)
    ;
#endif

  /* LSE startup and stabilization if required.*/
#if STM8L_LSE_ENABLED
#if LSEBYPASS
  CLK->ECKCR |= CLK_ECKCR_LSEON | CLK_ECKCR_LSEBYP;
#else
  CLK->ECKCR |= CLK_ECKCR_LSEON;
#endif
  while ((CLK->ECKCR & CLK_ECKCR_LSERDY) == 0)
    ;
#endif

  /* Setting up clock dividers.*/
  CLK->CKDIVR = STM8L_SYSCLK_DIVIDER << 0;

  /* SYSCLK switch to the selected source, not necessary if it is HSI.*/
#if STM8L_SYSCLK_SOURCE != CLK_SYSSEL_HSI
  /* Switching clock (manual switch mode).*/
  CLK->SWR  = STM8L_SYSCLK_SOURCE;
  while ((CLK->SWCR & CLK_SWCR_SWIF) == 0)
    ;
  CLK->SWCR = CLK_SWCR_SWEN;
#endif

  /* Clocks initially all disabled, note the boot ROM clock is disabled
     because the boot loader is no more required and it draws precious uAs.*/
  CLK->PCKENR1 = 0;
  CLK->PCKENR2 = 0;
  CLK->PCKENR3 = 0;

  /* Other clock related initializations.*/
  CLK->CSSR = 0;
  CLK->CCOR = 0;

  /* HSI disabled if it is no more required.*/
#if !STM8L_HSI_ENABLED
  CLK->ICKCR &= ~CLK_ICKCR_HSION;
#endif
#endif /* !STM8L_NO_CLOCK_INIT */
}

/** @} */
