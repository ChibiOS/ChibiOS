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

/**
 * @brief Enables the mutual exclusion APIs on the SPI bus.
 */
#if !defined(SPI_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define SPI_USE_MUTUAL_EXCLUSION    TRUE
#endif

#if SPI_USE_MUTUAL_EXCLUSION && !CH_USE_MUTEXES && !CH_USE_SEMAPHORES
#error "SPI_USE_MUTUAL_EXCLUSION requires CH_USE_MUTEXES and/or CH_USE_SEMAPHORES"
#endif

/**
 * @brief Driver state machine possible states.
 */
typedef enum {
  SPI_UNINIT = 0,                           /**< @brief Not initialized.    */
  SPI_STOP = 1,                             /**< @brief Stopped.            */
  SPI_READY = 2,                            /**< @brief Ready.              */
  SPI_ACTIVE = 3                            /**< @brief Slave selected.     */
} spistate_t;

#include "spi_lld.h"

#ifdef __cplusplus
extern "C" {
#endif
  void spiInit(void);
  void spiObjectInit(SPIDriver *spip);
  void spiStart(SPIDriver *spip, const SPIConfig *config);
  void spiStop(SPIDriver *spip);
  void spiSelect(SPIDriver *spip);
  void spiUnselect(SPIDriver *spip);
  msg_t spiExchange(SPIDriver *spip, size_t n, void *rxbuf, void *txbuf);
  msg_t spiSend(SPIDriver *spip, size_t n, void *txbuf);
  msg_t spiReceive(SPIDriver *spip, size_t n, void *rxbuf);
#if SPI_USE_MUTUAL_EXCLUSION
  void spiAcquireBus(SPIDriver *spip);
  void spiReleaseBus(SPIDriver *spip);
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#ifdef __cplusplus
}
#endif

#endif /* _SPI_H_ */

/** @} */
