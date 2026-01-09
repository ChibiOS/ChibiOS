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
 * @file    RP2350/rp_pll.c
 * @brief   RP2350 PLL source.
 *
 * @addtogroup RP_PLL
 * @{
 */

#include "hal.h"
#include "hal_safety.h"
#include "rp_pll.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Timeout for PLL lock in microseconds.
 * @note    PLL lock typically takes <1 ms. We allow up to 10 ms
 *          to account for ROSC timing variance during early init.
 */
#define RP_PLL_LOCK_TIMEOUT_US          10000U

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
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes a PLL.
 *
 * @param[in] pll       pointer to PLL registers (PLL_SYS or PLL_USB)
 * @param[in] refdiv    reference divider (1-63)
 * @param[in] vco_freq  target VCO frequency in Hz (750-1600 MHz)
 * @param[in] postdiv1  post divider 1 (1-7)
 * @param[in] postdiv2  post divider 2 (1-7)
 *
 * @note    Output frequency = vco_freq / (postdiv1 * postdiv2)
 * @note    VCO frequency = (XOSC / refdiv) * fbdiv
 * @note    See RP2350 Datasheet 8.6 PLL
 */
void rp_pll_init(PLL_TypeDef *pll, uint32_t refdiv, uint32_t vco_freq,
                 uint32_t postdiv1, uint32_t postdiv2) {

  uint32_t ref_freq = RP_XOSCCLK / refdiv;
  uint32_t fbdiv = vco_freq / ref_freq;
  uint32_t pdiv = PLL_PRIM_POSTDIV1(postdiv1) | PLL_PRIM_POSTDIV2(postdiv2);

  osalDbgAssert(vco_freq >= RP_PLL_VCO_MIN_FREQ &&
                vco_freq <= RP_PLL_VCO_MAX_FREQ,
                "VCO frequency out of range");
  osalDbgAssert(fbdiv >= 16U && fbdiv <= 320U, "FBDIV out of range");
  osalDbgAssert(postdiv1 >= 1U && postdiv1 <= 7U, "POSTDIV1 out of range");
  osalDbgAssert(postdiv2 >= 1U && postdiv2 <= 7U, "POSTDIV2 out of range");
  osalDbgAssert(ref_freq <= (vco_freq / 16U), "Reference frequency too high");

  /* Check if PLL is already configured */
  if ((pll->CS & PLL_CS_LOCK) &&
      (refdiv == (pll->CS & PLL_CS_REFDIV_Msk)) &&
      (fbdiv == (pll->FBDIV_INT & PLL_FBDIV_INT_Msk)) &&
      (pdiv == (pll->PRIM & (PLL_PRIM_POSTDIV1_Msk | PLL_PRIM_POSTDIV2_Msk)))) {
    return;
  }

  /* Reset the PLL */
  if (pll == PLL_SYS) {
    hal_lld_peripheral_reset(RESETS_ALLREG_PLL_SYS);
    hal_lld_peripheral_unreset(RESETS_ALLREG_PLL_SYS);
  } else {
    hal_lld_peripheral_reset(RESETS_ALLREG_PLL_USB);
    hal_lld_peripheral_unreset(RESETS_ALLREG_PLL_USB);
  }

  /* Set VCO divider */
  pll->CS = refdiv;
  pll->FBDIV_INT = fbdiv;

  /* Turn on PLL */
  rp_clear_bits(&pll->PWR, PLL_PWR_PD | PLL_PWR_VCOPD);

  /* Wait for PLL lock */
  if (halRegWaitAnySet32X(&pll->CS, PLL_CS_LOCK,
                          RP_PLL_LOCK_TIMEOUT_US, NULL)) {
    halSftFail("PLL lock timeout");
  }

  /* Set post divider */
  pll->PRIM = pdiv;

  /* Turn on post divider */
  rp_clear_bits(&pll->PWR, PLL_PWR_POSTDIVPD);
}

/**
 * @brief   Deinitialize a PLL.
 *
 * @param[in] pll       pointer to PLL register
 */
void rp_pll_deinit(PLL_TypeDef *pll) {

  /* Power down the PLL */
  pll->PWR = PLL_PWR_PD | PLL_PWR_VCOPD | PLL_PWR_POSTDIVPD | PLL_PWR_DSMPD;
}

/** @} */
