/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    templates/hal_lld.c
 * @brief   HAL Driver subsystem low level driver source template.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define SAM_PM_UNLOCK(addr)                                                 \
  PM->PM_UNLOCK = BPM_UNLOCK_KEY(0xAAu) | BPM_UNLOCK_ADDR(addr)

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
 * @brief   Enables a module on one of the PBx buses.
 * @note    PB bridges are assumed to be already enabled.
 *
 * @param[in] bus_id    id of the bus
 * @param[in] module    module address
 */
void sam_enable_module(uint32_t bus_id, uint32_t module) {
  uint32_t   mask;

  mask = *(&PM->PM_CPUMASK + bus_id);
  mask |= 1U << module;
  SAM_PM_UNLOCK(((uint32_t)&PM->PM_CPUMASK - (uint32_t)PM) + (4 * bus_id));
  *(&PM->PM_CPUMASK + bus_id) |= mask;
}

/**
 * @brief   Disables a module on one of the PBx buses.
 * @note    PB bridges are assumed to be already enabled.
 *
 * @param[in] bus_id    id of the bus
 * @param[in] module    module index
 */
void sam_disable_module(uint32_t bus_id, uint32_t module) {
  uint32_t   mask;

  mask = *(&PM->PM_CPUMASK + bus_id);
  mask &= ~(1U << module);
  SAM_PM_UNLOCK(((uint32_t)&PM->PM_CPUMASK - (uint32_t)PM) + (4 * bus_id));
  *(&PM->PM_CPUMASK + bus_id) = mask;
}

/**
 * @brief   Clock initialization.
 */
void sam_clock_init(void) {

#if SAM_NO_INIT
  /* Enables bridges.*/
  sam_enable_module(SAM_CLK_GRP_HSB, SAM_HSB_PBA_BRIDGE);
  sam_enable_module(SAM_CLK_GRP_HSB, SAM_HSB_PBB_BRIDGE);
  sam_enable_module(SAM_CLK_GRP_HSB, SAM_HSB_PBC_BRIDGE);
  sam_enable_module(SAM_CLK_GRP_HSB, SAM_HSB_PBD_BRIDGE);

#if SAM_USE_PICOCACHE
  /* Enable the PicoCache.*/
  sam_enable_module(SAM_CLK_GRP_PBB, SAM_PBB_HRAMC1_DATA);
  sam_enable_module(SAM_CLK_GRP_PBB, SAM_PBB_HRAMC1_REGS);
  HCACHE->HCACHE_CTRL = HCACHE_CTRL_CEN_YES;
  while ((HCACHE->HCACHE_SR & HCACHE_SR_CSTS_EN) == 0)
    ;
#endif

  /* Setting up prescalers.*/
  SAM_PM_UNLOCK((uint32_t)&PM->PM_CPUSEL);
  PM->PM_CPUSEL = SAM_CPUSEL;
  SAM_PM_UNLOCK((uint32_t)&PM->PM_PBASEL);
  PM->PM_PBASEL = SAM_PBASEL;
  SAM_PM_UNLOCK((uint32_t)&PM->PM_PBBSEL);
  PM->PM_PBBSEL = SAM_PBBSEL;
  SAM_PM_UNLOCK((uint32_t)&PM->PM_PBCSEL);
  PM->PM_PBCSEL = SAM_PBCSEL;
  SAM_PM_UNLOCK((uint32_t)&PM->PM_PBDSEL);
  PM->PM_PBDSEL = SAM_PBDSEL;

#endif /* SAM_NO_INIT */
}

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

}

/** @} */
