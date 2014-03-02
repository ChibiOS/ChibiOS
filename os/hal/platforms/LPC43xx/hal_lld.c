/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC43xx HAL driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC43xx/hal_lld.c
 * @brief   LPC43xx HAL subsystem low level driver source.
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

#define LPC_IDIV_ARRAY_NUM            5
#define LPC_BASE_CLK_ARRAY_NUM        27

const uint32_t lpc_idiv[LPC_IDIV_ARRAY_NUM] = {
  (((uint32_t)LPC_IDIVA_SRC) << 24) | (LPC_IDIVA_DIV << 2) | (!LPC_IDIVA_ENABLE),
  (((uint32_t)LPC_IDIVB_SRC) << 24) | (LPC_IDIVB_DIV << 2) | (!LPC_IDIVB_ENABLE),
  (((uint32_t)LPC_IDIVC_SRC) << 24) | (LPC_IDIVC_DIV << 2) | (!LPC_IDIVC_ENABLE),
  (((uint32_t)LPC_IDIVD_SRC) << 24) | (LPC_IDIVD_DIV << 2) | (!LPC_IDIVD_ENABLE),
  (((uint32_t)LPC_IDIVE_SRC) << 24) | (LPC_IDIVE_DIV << 2) | (!LPC_IDIVE_ENABLE)
};

const uint32_t lpc_base_clk[LPC_BASE_CLK_ARRAY_NUM] = {
  (CLK_SEL_PLL0USB << 24) | (!LPC_BASE_USB0_CLK_ENABLE),
  (((uint32_t)LPC_BASE_PERIPH_CLK_SRC) << 24) | (!LPC_BASE_PERIPH_CLK_ENABLE),
  (((uint32_t)LPC_BASE_USB1_CLK_SRC) << 24) | (!LPC_BASE_USB1_CLK_ENABLE),
  (((uint32_t)LPC_BASE_M4_CLK_SRC) << 24) | (!LPC_BASE_M4_CLK_ENABLE),
  (((uint32_t)LPC_BASE_SPIFI_CLK_SRC) << 24) | (!LPC_BASE_SPIFI_CLK_ENABLE),
  (((uint32_t)LPC_BASE_SPI_CLK_SRC) << 24) | (!LPC_BASE_SPI_CLK_ENABLE),
  (((uint32_t)LPC_BASE_PHY_RX_CLK_SRC) << 24) | (!LPC_BASE_PHY_RX_CLK_ENABLE),
  (((uint32_t)LPC_BASE_PHY_TX_CLK_SRC) << 24) | (!LPC_BASE_PHY_TX_CLK_ENABLE),
  (((uint32_t)LPC_BASE_APB1_CLK_SRC) << 24) | (!LPC_BASE_APB1_CLK_ENABLE),
  (((uint32_t)LPC_BASE_APB3_CLK_SRC) << 24) | (!LPC_BASE_APB3_CLK_ENABLE),
  (((uint32_t)LPC_BASE_LCD_CLK_SRC) << 24) | (!LPC_BASE_LCD_CLK_ENABLE),
  0,
  (((uint32_t)LPC_BASE_SDIO_CLK_SRC) << 24) | (!LPC_BASE_SDIO_CLK_ENABLE),
  (((uint32_t)LPC_BASE_SSP0_CLK_SRC) << 24) | (!LPC_BASE_SSP0_CLK_ENABLE),
  (((uint32_t)LPC_BASE_SSP1_CLK_SRC) << 24) | (!LPC_BASE_SSP1_CLK_ENABLE),
  (((uint32_t)LPC_BASE_UART0_CLK_SRC) << 24) | (!LPC_BASE_UART0_CLK_ENABLE),
  (((uint32_t)LPC_BASE_UART1_CLK_SRC) << 24) | (!LPC_BASE_UART1_CLK_ENABLE),
  (((uint32_t)LPC_BASE_UART2_CLK_SRC) << 24) | (!LPC_BASE_UART2_CLK_ENABLE),
  (((uint32_t)LPC_BASE_UART3_CLK_SRC) << 24) | (!LPC_BASE_UART3_CLK_ENABLE),
  (((uint32_t)LPC_BASE_OUT_CLK_SRC) << 24) | (!LPC_BASE_OUT_CLK_ENABLE),
  0,
  0,
  0,
  0,
  (((uint32_t)LPC_BASE_APLL_CLK_SRC) << 24) | (!LPC_BASE_APLL_CLK_ENABLE),
  (((uint32_t)LPC_BASE_CGU_OUT0_CLK_SRC) << 24) | (!LPC_BASE_CGU_OUT0_CLK_ENABLE),
  (((uint32_t)LPC_BASE_CGU_OUT1_CLK_SRC) << 24) | (!LPC_BASE_CGU_OUT1_CLK_ENABLE)
};

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
  SysTick->LOAD = LPC_BASE_M4_CLK / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* DWT cycle counter enable.*/
  SCS_DEMCR |= SCS_DEMCR_TRCENA;
  DWT_CTRL  |= DWT_CTRL_CYCCNTENA;

#if defined(LPC_DMA_REQUIRED)
  dmaInit();
#endif
}

/**
 * @brief   LPC43xx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function must be invoked only after the system reset.
 *
 * @special
 */
void lpc_clock_init(void) {

  uint32_t i;
  volatile uint32_t * preg;

#if !LPC_FLASHLESS
  /* Flash wait states setting.*/
  preg = (volatile uint32_t *) 0x40043120;                          /* FLASHCFGA */
  *preg = (1UL<< 31) | (LPC_FLASHCFG_FLASHTIM << 12) | 0x3A;
  *(preg + 1)= (1UL<< 31) | (LPC_FLASHCFG_FLASHTIM << 12) | 0x3A;   /* FLASHCFGB 0x40043124 */
#endif

  /* System oscillator initialization if required.*/
#if LPC_XTAL_ENABLE
  LPC_CGU->XTAL_OSC_CTRL = (LPC_OSCRANGE << 2);         /* Enable Main oscillator */
  for(i = 0; i < 1000000; i++)
    ;                                                   /* Wait for main oscillator to be ready */

  LPC_CGU->BASE_M4_CLK = (CLK_SEL_XTAL << 24);          /* Select the crystal oscillator */
                                                        /* as clock source for BASE_M4_CLK */
#else
  LPC_CGU->BASE_M4_CLK = (CLK_SEL_IRC << 24);          /* Select IRC as clock source for BASE_M4_CLK */
#endif

  LPC_CGU->PLL1_CTRL = 0;                               /* Power-down PLL1 enabled by Boot ROM */

#if LPC_PLL1_ENABLE /* PLL1 works in direct or non-integer mode. */
  /* Set PLL1 to FCLKOUT/2 */
  LPC_CGU->PLL1_CTRL = (1UL << 24) | (LPC_PLL1_CTRL_MSEL << 16) |
                      (LPC_PLL1_CTRL_NSEL << 12);

  while (!(LPC_CGU->PLL1_STAT & 0x01))
        ;                                               /* Wait for PLL1 locked */

  LPC_CGU->BASE_M4_CLK = (CLK_SEL_PLL1 << 24);          /* Select PPL1 clock as source for BASE_M4_CLK. */

  for(i = 0; i < 200000; i++)
      ;                                                 /* Wait */

#if LPC_PLL1_POSTDIV_ENABLE
  LPC_CGU->PLL1_CTRL |= (LPC_PLL1_CTRL_PSEL << 8);
#else
  LPC_CGU->PLL1_CTRL |= (1UL << 7);                     /* Set PLL1 to FCLKOUT > 156 MHz*/
#endif
#endif /* LPC_PLL1_ENABLE == TRUE */

  /* Config integer dividers. */
  preg = &LPC_CGU->IDIVA_CTRL;
  for (i = 0; i < LPC_IDIV_ARRAY_NUM; i++) {
    *preg = lpc_idiv[i];
    preg++;
  }

  /* Config base clocks. */
  preg = &LPC_CGU->BASE_USB0_CLK;
    for (i = 0; i < LPC_BASE_CLK_ARRAY_NUM; i++) {
      *preg = lpc_base_clk[i];
      preg++;
  }

#if LPC_PLL0USB0_ENABLE
#error "PPL0USB0 not supported."
#endif /* LPC_PLL0USB_ENABLE == TRUE */

#if LPC_PLL0AUDIO_ENABLE
#error "PLL0AUDIO not supported."
#endif /* LPC_PLL0AUDIO == TRUE */
}

/** @} */
