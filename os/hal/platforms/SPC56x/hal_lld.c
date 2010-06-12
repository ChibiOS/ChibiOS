/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    SPC56x/hal_lld.c
 * @brief   SPC563 HAL subsystem low level driver source.
 *
 * @addtogroup SPC563_HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
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
 */
void hal_lld_init(void) {
  extern void _vectors(void);
  uint32_t n;

  /* Enables the branch prediction, clears and enables the BTB into the
     BUCSR special register (1013).*/
  asm volatile ("li      %%r3, 0x0201          \t\n"
                "mtspr   1013, %%r3": : : "r3");

  /* FLASH wait states and prefetching setup.*/
  CFLASH0.BIUCR.R  = SPC563_FLASH_BIUCR | SPC563_FLASH_WS;
  CFLASH0.BIUCR2.R = 0;
  CFLASH0.PFCR3.R  = 0;

  /* Optimal crossbar settings. The DMA priority is placed above the CPU
     priority in order to not starve I/O activities while the CPU is
     excuting tight loops (FLASH and SRAM slave ports only).
     The SRAM is parked on the load/store port, for some unknown reason it
     is defaulted on the instructions port and this kills performance.*/
  XBAR.SGPCR3.B.PARK = 4;               /* RAM slave on load/store port.*/
  XBAR.MPR0.R = 0x00030201;             /* Flash slave port priorities:
                                            eDMA (1):              0 (highest)
                                            Core Instructions (0): 1
                                            Undocumented (2):      2
                                            Core Data (4):         3        */
  XBAR.MPR3.R = 0x00030201;             /* SRAM slave port priorities:
                                            eDMA (1):              0 (highest)
                                            Core Instructions (0): 1
                                            Undocumented (2):      2
                                            Core Data (4):         3        */

  /* Downcounter timer initialized for system tick use, TB enabled for debug
     and measurements.*/
  n = SPC563_SYSCLK / CH_FREQUENCY;
  asm volatile ("li      %%r3, 0            \t\n"
                "mtspr   284, %%r3          \t\n"   /* Clear TBL register.  */
                "mtspr   285, %%r3          \t\n"   /* Clear TBU register.  */
                "mtspr   22, %[n]           \t\n"   /* Init. DEC register.  */
                "mtspr   54, %[n]           \t\n"   /* Init. DECAR register.*/
                "li      %%r3, 0x4000       \t\n"   /* TBEN bit.            */
                "mtspr   1008, %%r3         \t\n"   /* HID0 register.       */
                "lis     %%r3, 0x0440       \t\n"   /* DIE ARE bits.        */
                "mtspr   340, %%r3"                 /* TCR register.        */
                : : [n] "r" (n) : "r3");

  /* INTC initialization, software vector mode, 4 bytes vectors, starting
     at priority 0.*/
  INTC.MCR.R   = 0;
  INTC.CPR.R   = 0;
  INTC.IACKR.R = (uint32_t)_vectors;
}

/**
 * @brief   SPC563 clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h and
 *          @p hal_lld.h
 */
void spc563_clock_init(void) {

  /* PLL activation.*/
  FMPLL.ESYNCR1.B.EMODE     = 1;
  FMPLL.ESYNCR1.B.CLKCFG   &= 1;                    /* Bypass mode, PLL off.*/
  FMPLL.ESYNCR1.B.CLKCFG   |= 2;                    /* PLL on.              */
  FMPLL.ESYNCR1.B.EPREDIV   = SPC563_CLK_PREDIV;
  FMPLL.ESYNCR1.B.EMFD      = SPC563_CLK_MFD;
  FMPLL.ESYNCR2.B.ERFD      = SPC563_CLK_RFD;
  while (!FMPLL.SYNSR.B.LOCK)
    ;
  FMPLL.ESYNCR1.B.CLKCFG   |= 4;                    /* Clock from the PLL.  */
}

/** @} */
