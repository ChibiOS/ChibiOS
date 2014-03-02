/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC17xx HAL driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC17xx/hal_lld.c
 * @brief   LPC17xx HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/**
 * @brief   Register missing in NXP header file.
 */

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
 *
 * @notapi
 */
void hal_lld_init(void) {

  /* SysTick initialization using the system clock.*/
  nvicSetSystemHandlerPriority(HANDLER_SYSTICK, CORTEX_PRIORITY_SYSTICK);
  SysTick->LOAD = LPC17xx_CCLK / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* DWT cycle counter enable.*/
  SCS_DEMCR |= SCS_DEMCR_TRCENA;
  DWT_CTRL  |= DWT_CTRL_CYCCNTENA;

#if defined(LPC17xx_DMA_REQUIRED)
  dmaInit();
#endif
}

/**
 * @brief   LPC17xx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function must be invoked only after the system reset.
 *
 * @special
 */
void LPC17xx_clock_init(void) {

  /* Flash wait states setting, the code takes care to not touch TBD bits.*/
  LPC_SC->FLASHCFG = (LPC_SC->FLASHCFG & ~(0x0000000F << 12)) | (LPC17xx_FLASHCFG_FLASHTIM << 12);

  /* System oscillator initialization if required.*/

#if LPC17xx_MAINOSC_ENABLE
  LPC_SC->SCS = (1 << 5) | (LPC17xx_OSCRANGE << 4);     /* Enable Main oscillator */
  while (!(LPC_SC->SCS & (1 << 6)))
    ;                                                   /* Wait for main oscillator to be ready */
#endif

  /* Peripheral clock divider initialization, must be set before enabling Main PLL (PLL0).
     Read errata sheet ES_LPC176x. */
  LPC_SC->PCLKSEL0 = LPC17xx_PCLKSEL0;
  LPC_SC->PCLKSEL1 = LPC17xx_PCLKSEL1;

  LPC_SC->CCLKCFG = LPC17xx_CCLK_DIV - 1;               /* Set CPU clock divider */

  LPC_SC->CLKSRCSEL = LPC17xx_SYSCLK_SELECT;            /* Select clock source for PLL0 if enabled or CPU */

#if LPC17xx_MAINPLL_ENABLE

  /* PLL0 configuration and start */
  LPC_SC->PLL0CFG = (LPC17xx_PLL0CFG_NSEL0 << 16) | LPC17xx_PLL0CFG_MSEL0;
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;

  LPC_SC->PLL0CON = 0x01;      	/* Enable PLL0. */
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;
  while (!(LPC_SC->PLL0STAT & (1UL << 26)))
      ;                         /* Wait for PLL0 locked */

  LPC_SC->PLL0CON = 0x03;      	/* Enable and Connect PLL0. */
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;
  while (!(LPC_SC->PLL0STAT & ((1UL << 25) | (1UL << 24))))
        ;                       /* Wait for PLL0 connected */

#endif /* LPC17xx_MAINPLL_ENABLE == TRUE */

#if LPC17xx_USBPLL_ENABLE
  /* PLL1 configuration and start */
  LPC_SC->PLL1CFG = (LPC17xx_PLL1CFG_PSEL1 << 5) | LPC17xx_PLL1CFG_MSEL1;
  LPC_SC->PLL1FEED = 0xAA;
  LPC_SC->PLL1FEED = 0x55;

  LPC_SC->PLL1CON = 0x01;      	/* Enable PLL1. */
  LPC_SC->PLL1FEED = 0xAA;
  LPC_SC->PLL1FEED = 0x55;
  while (!(LPC_SC->PLL1STAT & (1UL << 10)))
    ;                           /* Wait for PLL1 locked */

   LPC_SC->PLL1CON = 0x03;      /* Enable and Connect PLL1. */
   LPC_SC->PLL1FEED = 0xAA;
   LPC_SC->PLL1FEED = 0x55;
   while (!(LPC_SC->PLL1STAT & ((1UL << 9) | (1UL << 8))))
     ;                          /* Wait for PLL1 connected */
#endif /* LPC17xx_USBPLL_ENABLE == TRUE */

#if !LPC17xx_USBPLL_ENABLE && HAL_USE_USB
   LPC_SC->USBCLKCFG = LPC17xx_USBCLKPLL0_SELECT;
#endif

  /* Power control configuration */
  LPC_SC->PCONP = (1 << 15) | (1 << 9); /* Enable power for GPIO and RTC */

#if LPC17xx_CLKOUT_ENABLE
  LPC_SC->CLKOUTCFG = (1UL << 8) | ((LPC17xx_CLKOUT_DIV - 1) << 4) | LPC17xx_CLKOUT_SELECT;
#endif
}

/** @} */
