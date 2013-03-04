/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * @file    SPC5xx/edma.c
 * @brief   EDMA helper driver code.
 *
 * @addtogroup SPC5xx_EDMA
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static edma_channel_config_t channels[SPC5_EDMA_NCHANNELS];

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
 * @brief   EDMA driver initialization.
 *
 * @special
 */
void edmaInit(void) {

}

/**
 * @brief   EDMA channel allocation.
 *
 * @param[in] ccfg      channel configuration
 * @return              The channel TCD pointer.
 * @retval EDMA_ERROR   if the channel cannot be allocated.
 *
 * @special
 */
edma_channel_t edmaAllocChannel(const edma_channel_config_t *ccfg) {

}

/**
 * @brief   EDMA channel allocation.
 *
 * @param[in] channel   the channel number
 *
 * @special
 */
void edmaReleaseChannel(edma_channel_t channel) {

}

/** @} */
