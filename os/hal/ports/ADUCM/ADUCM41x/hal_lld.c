/*
    ChibiOS - Copyright (C) 2019 Rocco Marco Guglielmi

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
 * @file    ADUCM41x/hal_lld.c
 * @brief   ADUCM41x HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Reset value of the CLKCON0 register.
 */
#define ADUCM_CLKCON0_RESET                 0x043CU

/**
 * @brief   Reset value of the CLKCON0 register.
 */
#define ADUCM_CLKCON1_RESET                 0x0048U

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   CMSIS system core clock variable.
 * @note    It is declared in system_ADuCM41x.h.
 */
uint32_t SystemCoreClock = ADUCM_HCLK;

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
 *
 * @notapi
 */
void hal_lld_init(void) {

  /* DMA subsystems initialization.*/
#if defined(ADUCM_DMA_REQUIRED)
  dmaInit();
#endif

  /* IRQ subsystem initialization.*/
  irqInit();

  /* Disabling Watchdog timer which is enabled by default. */
#if HAL_CFG_DISABLE_WDG
  pADI_WDT->CON = 0;
#endif
}

/**
 * @brief   ADUCM clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function should be invoked just after the system reset.
 *
 * @special
 */
void aducm_clock_init(void) {

#if !ADUCM_NO_INIT

  /* Switching the clock source to the internal oscillator. */
  pADI_CLK->CLKCON0 = ADUCM_CLKMUX_HFOSC;

  /* Configuring the clock sources. */
  pADI_CLK->CLKCON0 |= ADUCM_CLKOUT | ADUCM_AMUX | ADUCM_ANAROOT;

  /* Managing the clock dividers. */
  pADI_CLK->CLKCON1 = ADUCM_CDHCLK_DIV | ADUCM_CDPCLK0_DIV |
                      ADUCM_CDPCLK1_DIV | ADUCM_CDANACLK_DIV;

#if((ADUCM_CLKMUX == ADUCM_CLKMUX_SPLL) ||                                  \
    (ADUCM_CLKOUT == ADUCM_CLKOUT_SPLL) ||                                  \
    (ADUCM_ANAROOT == ADUCM_ANAROOT_SPLL))

  /* Enabling PLL auto-gate.*/
  pADI_PLL->PLLPDCTRL |= BITM_PLL_MMRS_PLLPDCTRL_TOTPDB;
#endif

  /* Changing the clock source. Note that the field is already zeroed by the
     previous configuration. */
  pADI_CLK->CLKCON0 |= ADUCM_CLKMUX;

#if((ADUCM_CLKMUX == ADUCM_CLKMUX_SPLL) ||                                  \
    (ADUCM_CLKOUT == ADUCM_CLKOUT_SPLL) ||                                  \
    (ADUCM_ANAROOT == ADUCM_ANAROOT_SPLL))
  
  /* Waits until the SPLL is stable */
  while ((pADI_CLK->CLKSTAT0 & ADUCM_SPLL_STA_MASK) == ADUCM_SPLL_STA_UNLOCKED)
    ;
#endif


#endif /* !ADUCM_NO_INIT */
}
/** @} */
