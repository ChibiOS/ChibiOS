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
 * @file    SPC5xx/SIUL_v1/pal_lld.c
 * @brief   SPC5xx SIUL low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

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
 * @brief   SPC5xx I/O ports configuration.
 *
 * @param[in] config    the STM32 ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {
  unsigned i;

  /* Initialize PCR registers for undefined pads.*/
  for (i = 0; i < SPC5_SIUL_NUM_PCRS; i++)
    SIU.PCR[i].R = config->default_mode;

  /* Initialize PADSEL registers.*/
  for (i = 0; i < SPC5_SIUL_NUM_PADSELS; i++)
    SIU.PSMI[i].R = config->padsels[i];

  /* Initialize PCR registers for defined pads.*/
  i = 0;
  while (config->inits[i].pcr_value != 0) {
    SIU.GPDO[config->inits[i].pcr_index].R = config->inits[i].gpdo_value;
    SIU.PCR[config->inits[i].pcr_index].R  = config->inits[i].pcr_value;
    i++;
  }
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {
  (void)port;
  (void)mask;
  (void)mode;
}

#endif /* HAL_USE_PAL */

/** @} */
