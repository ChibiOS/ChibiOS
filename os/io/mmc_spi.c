/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file mmc_spi.c
 * @brief MMC over SPI driver code
 * @addtogroup MMC_SPI
 * @{
 */

#include <ch.h>
#include <spi.h>
#include <mmc_spi.h>

/**
 * @brief MMC over SPI driver initialization.
 */
void mmcInit(void) {

}

/**
 * @brief Initializes an instance.
 *
 * @param[in] mmcp      pointer to the @p MMCDriver object
 */
void mmcObjectInit(MMCDriver *mmcp, SPIDriver *spip,
                   const SPIConfig *lscfg, const SPIConfig *hscfg,
                   mmcquery_t is_protected, mmcquery_t is_inserted) {

  mmcp->mmc_state = MMC_STOP;
  mmcp->mmc_config = NULL;
  mmcp->mmc_spip = spip;
  mmcp->mmc_lscfg = lscfg;
  mmcp->mmc_hscfg = hscfg;
  mmcp->mmc_is_protected = is_protected;
  mmcp->mmc_is_inserted = is_inserted;
}

/**
 * @brief Configures and activates the MMC peripheral.
 *
 * @param[in] mmcp      pointer to the @p MMCDriver object
 * @param[in] config    pointer to the @p MMCConfig object
 */
void mmcStart(MMCDriver *mmcp, const MMCConfig *config) {

  chDbgCheck((mmcp != NULL) && (config != NULL), "mmcStart");

  chSysLock();
  chDbgAssert((mmcp->mmc_state == MMC_STOP) || (mmcp->mmc_state == MMC_READY),
              "mmcStart(), #1",
              "invalid state");
  mmcp->mmc_config = config;
  mmcp->mmc_state = MMC_READY;
  chSysUnlock();
}

/**
 * @brief Deactivates the MMC peripheral.
 *
 * @param[in] mmcp      pointer to the @p MMCDriver object
 */
void mmcStop(MMCDriver *mmcp) {

  chDbgCheck(mmcp != NULL, "mmcStop");

  chSysLock();
  chDbgAssert((mmcp->mmc_state == MMC_STOP) || (mmcp->mmc_state == MMC_READY),
              "mmcStop(), #1",
              "invalid state");
  mmcp->mmc_state = MMC_STOP;
  chSysUnlock();
}

/** @} */
