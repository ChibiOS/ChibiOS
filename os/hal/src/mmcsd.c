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
 * @file    mmcsd.c
 * @brief   MMC/SD cards common code.
 *
 * @addtogroup MMCSD
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_MMC_SPI || HAL_USE_SDC || defined(__DOXYGEN__)

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

/**
 * @brief   Get slice with data from uint32_t[4] array.
 *
 * @notapi
 */
static uint32_t mmcsd_get_slice(uint32_t *data, uint32_t end, uint32_t start) {
  uint32_t word = 0;
  uint32_t mask = 0;

  chDbgCheck(end >= start, "sdc_get_slice");

  while ((start - 32 * word) > 31) {
    word++;
    data++;
  }

  end   -= 32 * word;
  start -= 32 * word;

  if (end < 31) {
    /* Value lays in one word.*/
    mask = (1 << (end - start + 1)) - 1;
    return (*data >> start) & mask;
  }
  else {
    /* Value spread on separate words.*/
    uint32_t lsb, msb;
    lsb = *data >> start;
    data++;
    mask = (1 << (end - 32 + 1)) - 1;
    msb = *data & mask;
    msb = msb << (32 - start);
    return msb | lsb;
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Extract card capacity from a CSD.
 * @details The capacity is returned as number of available blocks.
 *
 * @param[in] csd       the CSD record
 *
 * @return              The card capacity.
 * @retval 0            CSD format error
 */
uint32_t mmcsdGetCapacity(uint32_t csd[4]) {

  switch (csd[3] >> 30) {
  uint32_t a, b, c;
  case 0:
    /* CSD version 1.0 */
    a = mmcsd_get_slice(csd, MMCSD_CSD_10_C_SIZE_SLICE);
    b = mmcsd_get_slice(csd, MMCSD_CSD_10_C_SIZE_MULT_SLICE);
    c = mmcsd_get_slice(csd, MMCSD_CSD_10_READ_BL_LEN_SLICE);
    return (a + 1) << (b + 2) << (c - 9);   /* 2^9 == MMCSD_BLOCK_SIZE.     */
  case 1:
    /* CSD version 2.0.*/
    return 1024 * (mmcsd_get_slice(csd, MMCSD_CSD_20_C_SIZE_SLICE) + 1);
  default:
    /* Reserved value detected.*/
    return 0;
  }
}

#endif /* HAL_USE_MMC_SPI || HAL_USE_SDC */

/** @} */
