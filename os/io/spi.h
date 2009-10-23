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
 * @file spi.h
 * @brief SPI Driver macros and structures.
 * @addtogroup SPI
 * @{
 */

#ifndef _SPI_H_
#define _SPI_H_

#include "spi_lld.h"

#ifdef __cplusplus
extern "C" {
#endif
  void spiInit(void);
  void spiObjectInit(SPIDriver *spip);
  void spiSetup(SPIDriver *spip, const SPIConfig *config);
  void spiSelect(SPIDriver *spip);
  void spiUnselect(SPIDriver *spip);
  void spiExchange(SPIDriver *spip, size_t n, void *rxbuf, void *txbuf);
#if SPI_USE_MUTUAL_EXCLUSION
  void spiAcquireBus(SPIDriver *spip);
  void spiReleaseBus(SPIDriver *spip);
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#ifdef __cplusplus
}
#endif

#endif /* _SPI_H_ */

/** @} */
