/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file    RP2040/rp_clocks.c
 * @brief   RP2040 clocks driver source.
 * @note    See RP2040 Datasheet 2.15 Clocks
 *
 * @addtogroup RP_CLOCKS
 * @{
 */

#include "hal.h"
#include "rp_clocks.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Estimated ROSC frequency for early timing.
 * @note    RP2040 ROSC varies 1.8-12 MHz, we assume ~6 MHz.
 *          This gives roughly +/-50% accuracy which is acceptable for
 *          safety timeouts during early clock initialization.
 */
#define RP_ROSC_ASSUMED_HZ      6000000U

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Configured clock frequencies
 */
static uint32_t configured_freq[RP_CLK_COUNT];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes all clocks.
 * @note    Most of this is derived from the RP2040 datasheet which directly
 *          references suggested code from the Pico SDK which is Copyright 
 *          2020 Raspberry Pi (Trading) Ltd and licensed under the
 *          BSD-3-Clause license. We always start with ROSC and then switch
 *          to XOSC. With the RP2040 WATCHDOG->TICK services the watchdog,
 *          the system timer and SYSTICK.
 * @note    See RP2040 Datasheet 2.15.3.1 Clock Instances (Table 205)
 */
void rp_clock_init(void) {

  /* Start early tick generator for safety module timeouts. */
  hal_lld_peripheral_unreset(RESETS_ALLREG_TIMER0);

  /* Configure tick generator for ~1 us ticks. */
  WATCHDOG->TICK = WATCHDOG_TICK_ENABLE | (RP_ROSC_ASSUMED_HZ / RP_ROSC_ASSUMED_HZ);

  /* Clear clock resus that may be in an unkown state */
  CLOCKS->RESUS.CTRL = 0U;

  rp_xosc_init();

  /* Switch clk_sys and clk_ref to safe sources */
  rp_clear_bits(&CLOCKS->CLK[RP_CLK_SYS].CTRL, CLOCKS_CLK_SYS_CTRL_SRC_Msk);
  while ((CLOCKS->CLK[RP_CLK_SYS].SELECTED & 1U) == 0U) {
    /* Wait for clk_sys to switch to clk_ref */
  }
  rp_clear_bits(&CLOCKS->CLK[RP_CLK_REF].CTRL, CLOCKS_CLK_REF_CTRL_SRC_Msk);
  while ((CLOCKS->CLK[RP_CLK_REF].SELECTED & 1U) == 0U) {
    /* Wait for clk_ref to switch to ROSC */
  }

  /* Initialize PLL_SYS: 12 MHz * 125 / 6 / 2 = 125 MHz. */
  rp_pll_init(PLL_SYS, RP_PLL_SYS_REFDIV, RP_PLL_SYS_VCO_FREQ,
              RP_PLL_SYS_POSTDIV1, RP_PLL_SYS_POSTDIV2);

  /* Initialize PLL_USB: 12 MHz * 100 / 5 / 5 = 48 MHz. */
  rp_pll_init(PLL_USB, RP_PLL_USB_REFDIV, RP_PLL_USB_VCO_FREQ,
              RP_PLL_USB_POSTDIV1, RP_PLL_USB_POSTDIV2);

  /* CLK_REF = XOSC = 12 MHz */
  {
    uint32_t src = CLOCKS_CLK_REF_CTRL_SRC_XOSC >> CLOCKS_CLK_REF_CTRL_SRC_Pos;
    CLOCKS->CLK[RP_CLK_REF].DIV = 1U << 8;
    rp_write_masked(&CLOCKS->CLK[RP_CLK_REF].CTRL,
                    src << CLOCKS_CLK_REF_CTRL_SRC_Pos,
                    CLOCKS_CLK_REF_CTRL_SRC_Msk);
    while ((CLOCKS->CLK[RP_CLK_REF].SELECTED & (1U << src)) == 0U) {
      /* Wait for switch to XOSC */
    }
    configured_freq[RP_CLK_REF] = RP_XOSCCLK;
  }

  /* CLK_SYS = PLL_SYS = 125 MHz */
  rp_clear_bits(&CLOCKS->CLK[RP_CLK_SYS].CTRL, CLOCKS_CLK_SYS_CTRL_SRC_Msk);
  while ((CLOCKS->CLK[RP_CLK_SYS].SELECTED & 1U) == 0U) {
    /* Wait for switch to clk_ref */
  }
  rp_write_masked(&CLOCKS->CLK[RP_CLK_SYS].CTRL,
                  CLOCKS_CLK_SYS_CTRL_AUXSRC_PLL_SYS,
                  CLOCKS_CLK_SYS_CTRL_AUXSRC_Msk);
  rp_set_bits(&CLOCKS->CLK[RP_CLK_SYS].CTRL, CLOCKS_CLK_SYS_CTRL_SRC_AUX);
  while ((CLOCKS->CLK[RP_CLK_SYS].SELECTED & 2U) == 0U) {
    /* Wait for switch to aux */
  }
  configured_freq[RP_CLK_SYS] = RP_PLL_SYS_CLK;

  /* CLK_USB = PLL_USB = 48 MHz */
  rp_write_masked(&CLOCKS->CLK[RP_CLK_USB].CTRL,
                  CLOCKS_CLK_USB_CTRL_AUXSRC_PLL_USB,
                  CLOCKS_CLK_USB_CTRL_AUXSRC_Msk);
  CLOCKS->CLK[RP_CLK_USB].DIV = 1U << 8;
  rp_set_bits(&CLOCKS->CLK[RP_CLK_USB].CTRL, CLOCKS_CLK_PERI_CTRL_ENABLE);
  configured_freq[RP_CLK_USB] = RP_PLL_USB_CLK;

  /* CLK_ADC = PLL_USB = 48 MHz */
  rp_write_masked(&CLOCKS->CLK[RP_CLK_ADC].CTRL,
                  CLOCKS_CLK_ADC_CTRL_AUXSRC_PLL_USB,
                  CLOCKS_CLK_ADC_CTRL_AUXSRC_Msk);
  CLOCKS->CLK[RP_CLK_ADC].DIV = 1U << 8;
  rp_set_bits(&CLOCKS->CLK[RP_CLK_ADC].CTRL, CLOCKS_CLK_PERI_CTRL_ENABLE);
  configured_freq[RP_CLK_ADC] = RP_PLL_USB_CLK;

  /* CLK_RTC = PLL_USB / 1024 = 46875Hz */
  rp_write_masked(&CLOCKS->CLK[RP_CLK_RTC].CTRL,
                  CLOCKS_CLK_RTC_CTRL_AUXSRC_PLL_USB,
                  CLOCKS_CLK_RTC_CTRL_AUXSRC_Msk);
  CLOCKS->CLK[RP_CLK_RTC].DIV = RP_RTC_CLK_DIV << 8;
  rp_set_bits(&CLOCKS->CLK[RP_CLK_RTC].CTRL, CLOCKS_CLK_PERI_CTRL_ENABLE);
  configured_freq[RP_CLK_RTC] = RP_PLL_USB_CLK / RP_RTC_CLK_DIV;

  /* CLK_ADC = PLL_USB = 48 MHz */
  rp_write_masked(&CLOCKS->CLK[RP_CLK_PERI].CTRL,
                  CLOCKS_CLK_PERI_CTRL_AUXSRC_SYS,
                  CLOCKS_CLK_PERI_CTRL_AUXSRC_Msk);
  CLOCKS->CLK[RP_CLK_PERI].DIV = 1U << 8;
  rp_set_bits(&CLOCKS->CLK[RP_CLK_PERI].CTRL, CLOCKS_CLK_PERI_CTRL_ENABLE);
  configured_freq[RP_CLK_PERI] = RP_PLL_SYS_CLK;

  /* Calculate cycles for 1us tick based on clk_ref frequency. */
  WATCHDOG->TICK = WATCHDOG_TICK_ENABLE | (configured_freq[RP_CLK_REF] / 1000000U);
}

/**
 * @brief   Returns the frequency of a clock in Hz.
 *
 * @param[in] clk_index     clock index (RP_CLK_xxx)
 * @return                  clock frequency in Hz
 */
uint32_t rp_clock_get_hz(uint32_t clk_index) {

  osalDbgAssert(clk_index < RP_CLK_COUNT, "invalid clock index");

  return configured_freq[clk_index];
}

/** @} */
