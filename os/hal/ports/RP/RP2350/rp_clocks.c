/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    RP2350/rp_clocks.c
 * @brief   RP2350 clock driver source.
 * @note    See RP2350 Datasheet 8 Clocks
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
 * @note    RP2350 ROSC varies 4.6-19.6 MHz, we assume ~6 MHz.
 *          This gives roughly +/-60% accuracy which is acceptable for
 *          safety timeouts during early clock initialization.
 */
#define RP_ROSC_ASSUMED_HZ      6000000U
#define RP_VREG_TIMEOUT_US      100000U

#if defined(RP_VREG_VSEL) && (RP_VREG_VSEL > 0x1FU)
#error "RP_VREG_VSEL must fit the POWMAN VREG.VSEL field"
#endif

#if defined(RP_QMI_CLKDIV) != defined(RP_QMI_RXDELAY)
#error "RP_QMI_CLKDIV and RP_QMI_RXDELAY must be defined together"
#endif

#if defined(RP_QMI_CLKDIV) && (RP_QMI_CLKDIV > 0xFFU)
#error "RP_QMI_CLKDIV must fit the QMI M0_TIMING.CLKDIV field"
#endif

#if defined(RP_QMI_RXDELAY) && (RP_QMI_RXDELAY > 0x7U)
#error "RP_QMI_RXDELAY must fit the QMI M0_TIMING.RXDELAY field"
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if defined(RP_VREG_VSEL)
/**
 * @brief   Raises the on-chip VREG output voltage before switching to a
 *          higher PLL frequency.
 *
 * The POWMAN voltage regulator (VREG) defaults to 1.1 V. Boards operating
 * outside the datasheet-qualified clock range may select a higher voltage,
 * which must be applied before the PLL is switched to the target frequency.
 *
 * This function unlocks the VREG control interface and programs the voltage
 * level requested by the RP_VREG_VSEL hwdef.dat define.  The sequence mirrors
 * the MicroPython reference implementation by MichaelBell (power.py):
 *   1. Unlock VREG_CTRL with the POWMAN password (0x5AFE in upper 16 bits).
 *   2. Configure BOD (brown-out detector) threshold to "normal".
 *   3. Write VSEL field in VREG register to the desired voltage level.
 *   4. Wait with a safety timeout for UPDATE_IN_PROGRESS to clear and
 *      VOUT_OK to assert.
 *
 * POWMAN base address: 0x40100000 (__POWMAN_BASE).  Register offsets:
 *   +0x04  VREG_CTRL  — unlock (bit 13), RST_N (bit 15), HT_TH (bits 6:4)
 *   +0x08  VREG_STS   — VOUT_OK (bit 4)
 *   +0x0C  VREG       — VSEL (bits 8:4), UPDATE_IN_PROGRESS (bit 15)
 *   +0x1C  BOD        — brown-out detector settings
 *
 * RP_VREG_VSEL encoding (datasheet Table 481, VREG register):
 *   0x0b = 1.10 V (default)
 *   0x0c = 1.15 V
 *   0x0d = 1.20 V
 *   0x0e = 1.25 V
 *   0x0f = 1.30 V (voltage-limit cap)
 * Voltages above 1.30 V require DISABLE_VOLTAGE_LIMIT in VREG_CTRL first.
 *
 * All writes to POWMAN addresses <= base+0xAC require the 0x5AFE password
 * in the upper 16 bits; reads do NOT return the password (protected).
 */
static void rp2350_vreg_init(void) {
  volatile uint32_t *vreg_ctrl = (volatile uint32_t *)(__POWMAN_BASE + 0x04U);
  volatile uint32_t *vreg_sts  = (volatile uint32_t *)(__POWMAN_BASE + 0x08U);
  volatile uint32_t *vreg      = (volatile uint32_t *)(__POWMAN_BASE + 0x0CU);
  volatile uint32_t *bod       = (volatile uint32_t *)(__POWMAN_BASE + 0x1CU);

  /* Unlock VREG control: password | RST_N(bit15) | UNLOCK(bit13) | HT_TH=5/125C(bits6:4).
   * Once unlocked the interface cannot be re-locked — one-way operation. */
  *vreg_ctrl = 0x5AFEA050U;

  /* Set brown-out detector to "normal" threshold (appropriate for >= 1.1 V). */
  *bod = 0x5AFE0091U;

  /* Busy-wait ~16 ms at the assumed 6 MHz ROSC for BOD to settle before
   * changing the output voltage.  No OS timer is available this early. */
  for (volatile uint32_t i = 0U; i < 50000U; i++) {
    __asm__ volatile ("nop");
  }

  /* Write VSEL to raise the regulator output.  VSEL occupies bits [8:4].
   * The upper 16 bits carry the mandatory 0x5AFE write password. */
  *vreg = 0x5AFE0000U | ((uint32_t)(RP_VREG_VSEL) << 4U);

  /* Bound both waits so a regulator or board-power fault is diagnosable. */
  halSftFailOnError(halRegWaitAllClear32X(vreg, 1U << 15U,
                                          RP_VREG_TIMEOUT_US, NULL),
                    "RP VREG update timeout");
  halSftFailOnError(halRegWaitAnySet32X(vreg_sts, 1U << 4U,
                                        RP_VREG_TIMEOUT_US, NULL),
                    "RP VREG regulation timeout");
}
#endif /* RP_VREG_VSEL */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes all clocks.
 * @note    Most of this is derived from the RP2350 datasheet which directly
 *          references suggested code from the Pico SDK which is Copyright 
 *          2020 Raspberry Pi (Trading) Ltd and licensed under the
 *          BSD-3-Clause license. We always start with ROSC and then switch
 *          to XOSC.
 * @note    See RP2350 Datasheet 8.1.3.1 Clock Instances (Table 541)
 */
void rp_clock_init(void) {

  /* Copy .ramtext (RAMFUNC) section to SRAM before touching the PLL.
   *
   * CRT0 copies the vector table (INIT_VECTORS) and .data+.ramtext
   * (INIT_DATA) AFTER calling __early_init(), which is where this
   * function runs.  So when rp_clock_init() executes, the RAM fault
   * handlers aren't in RAM yet — their VMA addresses are correct but
   * the SRAM hasn't been populated.  If a fault occurs during the PLL
   * switch (e.g. XIP timing glitch at the new higher frequency) the
   * CPU jumps to the handler VMA (SRAM) but finds uninitialised memory,
   * causing a double-fault lockup at PC=0xEFFFFFFE.
   *
   * Fix: copy just the .ramtext region now, before XOSC/PLL init, so
   * the RAM fault handlers are live before the clock risk window.
   * The linker exports __ramfunc_start__ (VMA) and __textramfunc_base__
   * (LMA in flash) and __ramfunc_end__ (VMA end).
   */
  {
    extern uint32_t __ramfunc_start__;  /* VMA: destination in SRAM      */
    extern uint32_t __ramfunc_end__;    /* VMA: end of ramtext in SRAM    */
    /* LMA of ramtext is immediately after the last item in .data in flash.
     * The linker puts .ramtext inside the .data output section so its LMA
     * follows __textdata_base__ + (size of .data before ramtext).
     * We can compute it as: LMA = flash_base_of_.data + offset_of_ramtext.
     * The most portable way is to use __textdata_base__ and walk past .data.
     * ChibiOS ld exports __textdata_base__ for exactly this purpose. */
    extern uint32_t __textdata_base__; /* LMA: start of .data+ramtext in flash */
    extern uint32_t __data_base__;     /* VMA: start of .data in SRAM          */

    /* Compute LMA of ramtext = textdata_base + (ramtext_vma - data_vma). */
    uint32_t *src = &__textdata_base__ +
                    (&__ramfunc_start__ - &__data_base__);
    uint32_t *dst = &__ramfunc_start__;
    uint32_t *end = &__ramfunc_end__;
    while (dst < end) {
      *dst++ = *src++;
    }
  }

  /* Start early tick generator for safety module timeouts. */
  rp_peripheral_unreset(RESETS_ALLREG_TIMER0);

  /* Configure tick generator for ~1 us ticks. */
  TICKS->TICK[TICKS_TIMER0].CYCLES = RP_ROSC_ASSUMED_HZ / 1000000U;;
  TICKS->TICK[TICKS_TIMER0].CTRL = TICKS_CTRL_ENABLE;

  /* Clear clock resus that may be in an unkown state */
  CLOCKS->RESUS.CTRL = 0U;

  rp_xosc_init();

  /* Switch clk_sys and clk_ref to safe sources */
  CLOCKS->CLR.CLK[RP_CLK_SYS].CTRL = CLOCKS_CLK_SYS_CTRL_SRC_Msk;
  while ((CLOCKS->CLK[RP_CLK_SYS].SELECTED & 1U) == 0U) {
    /* Wait for clk_sys to switch to clk_ref */
  }
  CLOCKS->CLR.CLK[RP_CLK_REF].CTRL = CLOCKS_CLK_REF_CTRL_SRC_Msk;
  while ((CLOCKS->CLK[RP_CLK_REF].SELECTED & 1U) == 0U) {
    /* Wait for clk_ref to switch to ROSC */
  }

#if defined(RP_VREG_VSEL)
  /* Set the board-selected core voltage before initializing the PLL. The CPU
   * is still running on the slow ROSC, so the voltage change precedes any
   * increase in system frequency. */
  rp2350_vreg_init();
#endif

#if defined(RP_QMI_CLKDIV)
  /* Update QMI M0_TIMING before switching PLL_SYS to the target frequency.
   * Bootloader timing may not be safe at the selected application clock, so
   * the board supplies a characterized CLKDIV and RXDELAY. Apply these values
   * before raising sys_clk, then issue a dummy flash access and barriers.
   * Only CLKDIV and RXDELAY are replaced; all other fields (MAX_SELECT,
   * MIN_DESELECT, etc.) are preserved from the bootloader value. */
  {
    volatile uint32_t *m0_timing = (volatile uint32_t *)(0x400D000CU);
    uint32_t t = *m0_timing;
    t &= ~0x7FFU;                                    /* clear CLKDIV[7:0] and RXDELAY[10:8] */
    t |= ((uint32_t)(RP_QMI_RXDELAY) << 8) | (uint32_t)(RP_QMI_CLKDIV);
    *m0_timing = t;
    /* DSB ensures the MMIO write reaches the QMI peripheral before the dummy
     * flash read triggers a QMI transaction with the new timing. */
    __asm volatile ("dsb sy" ::: "memory");
    __asm volatile ("isb" ::: "memory");
    /* Dummy flash read to flush any in-flight XIP prefetch with old timing. */
    (void)(*(volatile uint32_t *)0x10000000U);
    __asm volatile ("dsb sy" ::: "memory");
    __asm volatile ("isb" ::: "memory");
  }
#endif

  /* Initialize PLL_SYS: 12 MHz * 125 / 5 / 2 = 150 MHz. */
  rp_pll_init(PLL_SYS, RP_PLL_SYS_REFDIV, RP_PLL_SYS_VCO_FREQ,
              RP_PLL_SYS_POSTDIV1, RP_PLL_SYS_POSTDIV2);

  /* Initialize PLL_USB: 12 MHz * 100 / 5 / 5 = 48 MHz. */
  rp_pll_init(PLL_USB, RP_PLL_USB_REFDIV, RP_PLL_USB_VCO_FREQ,
              RP_PLL_USB_POSTDIV1, RP_PLL_USB_POSTDIV2);

  /* CLK_REF = XOSC = 12 MHz */
  {
    uint32_t src = CLOCKS_CLK_REF_CTRL_SRC_XOSC >> CLOCKS_CLK_REF_CTRL_SRC_Pos;
    CLOCKS->CLK[RP_CLK_REF].DIV = 1U << 16;
    CLOCKS->XOR.CLK[RP_CLK_REF].CTRL =
        (CLOCKS->CLK[RP_CLK_REF].CTRL ^ (src << CLOCKS_CLK_REF_CTRL_SRC_Pos)) &
        CLOCKS_CLK_REF_CTRL_SRC_Msk;
    while ((CLOCKS->CLK[RP_CLK_REF].SELECTED & (1U << src)) == 0U) {
      /* Wait for switch to XOSC */
    }
  }

  /* CLK_SYS = PLL_SYS = 150 MHz */
  CLOCKS->CLR.CLK[RP_CLK_SYS].CTRL = CLOCKS_CLK_SYS_CTRL_SRC_Msk;
  while ((CLOCKS->CLK[RP_CLK_SYS].SELECTED & 1U) == 0U) {
    /* Wait for switch to clk_ref */
  }
  CLOCKS->XOR.CLK[RP_CLK_SYS].CTRL =
      (CLOCKS->CLK[RP_CLK_SYS].CTRL ^ CLOCKS_CLK_SYS_CTRL_AUXSRC_PLL_SYS) &
      CLOCKS_CLK_SYS_CTRL_AUXSRC_Msk;
  CLOCKS->SET.CLK[RP_CLK_SYS].CTRL = CLOCKS_CLK_SYS_CTRL_SRC_AUX;
  while ((CLOCKS->CLK[RP_CLK_SYS].SELECTED & 2U) == 0U) {
    /* Wait for switch to aux */
  }

  /* CLK_USB = PLL_USB = 48 MHz */
  CLOCKS->XOR.CLK[RP_CLK_USB].CTRL =
      (CLOCKS->CLK[RP_CLK_USB].CTRL ^ CLOCKS_CLK_USB_CTRL_AUXSRC_PLL_USB) &
      CLOCKS_CLK_USB_CTRL_AUXSRC_Msk;
  CLOCKS->CLK[RP_CLK_USB].DIV = 1U << 16;
  CLOCKS->SET.CLK[RP_CLK_USB].CTRL = CLOCKS_CLK_PERI_CTRL_ENABLE;

  /* CLK_ADC = PLL_USB = 48 MHz */
  CLOCKS->XOR.CLK[RP_CLK_ADC].CTRL =
      (CLOCKS->CLK[RP_CLK_ADC].CTRL ^ CLOCKS_CLK_ADC_CTRL_AUXSRC_PLL_USB) &
      CLOCKS_CLK_ADC_CTRL_AUXSRC_Msk;
  CLOCKS->CLK[RP_CLK_ADC].DIV = 1U << 16;
  CLOCKS->SET.CLK[RP_CLK_ADC].CTRL = CLOCKS_CLK_PERI_CTRL_ENABLE;

  /* CLK_PERI = CLK_SYS = 150 MHz */
  CLOCKS->XOR.CLK[RP_CLK_PERI].CTRL =
      (CLOCKS->CLK[RP_CLK_PERI].CTRL ^ CLOCKS_CLK_PERI_CTRL_AUXSRC_SYS) &
      CLOCKS_CLK_PERI_CTRL_AUXSRC_Msk;
  CLOCKS->CLK[RP_CLK_PERI].DIV = 1U << 16;
  CLOCKS->SET.CLK[RP_CLK_PERI].CTRL = CLOCKS_CLK_PERI_CTRL_ENABLE;

  /* Calculate cycles for 1us tick based on clk_ref frequency. */
  uint32_t cycles = RP_XOSCCLK / 1000000U;

  /* Start tick generators */
  for (uint32_t i = 0U; i < 6U; i++) {
    TICKS->TICK[i].CYCLES = cycles;
    TICKS->TICK[i].CTRL = TICKS_CTRL_ENABLE;
  }
}

/**
 * @brief   Returns the frequency of a clock in Hz.
 * @note    Uses compile-time constants so this function is safe to call
 *          before BSS/DATA initialization.
 *
 * @param[in] clk_index     clock index (RP_CLK_xxx)
 * @return                  clock frequency in Hz
 */
uint32_t rp_clock_get_hz(uint32_t clk_index) {

  osalDbgAssert(clk_index < RP_CLK_COUNT, "invalid clock index");

  switch (clk_index) {
  case RP_CLK_REF:
    return RP_CLK_REF_FREQ;
  case RP_CLK_SYS:
    return RP_CLK_SYS_FREQ;
  case RP_CLK_PERI:
    return RP_CLK_PERI_FREQ;
  case RP_CLK_USB:
    return RP_CLK_USB_FREQ;
  case RP_CLK_ADC:
    return RP_CLK_ADC_FREQ;
  default:
    return 0U;
  }
}

/** @} */
