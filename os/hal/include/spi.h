/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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
 * @file spi.h
 * @brief SPI Driver macros and structures.
 * @addtogroup SPI
 * @{
 */

#ifndef _SPI_H_
#define _SPI_H_

#if CH_HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief Enables the mutual exclusion APIs on the SPI bus.
 */
#if !defined(SPI_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define SPI_USE_MUTUAL_EXCLUSION    TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if SPI_USE_MUTUAL_EXCLUSION && !CH_USE_MUTEXES && !CH_USE_SEMAPHORES
#error "SPI_USE_MUTUAL_EXCLUSION requires CH_USE_MUTEXES and/or CH_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

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

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void spiInit(void);
  void spiObjectInit(SPIDriver *spip);
  void spiStart(SPIDriver *spip, const SPIConfig *config);
  void spiStop(SPIDriver *spip);
  void spiSelect(SPIDriver *spip);
  void spiUnselect(SPIDriver *spip);
  void spiIgnore(SPIDriver *spip, size_t n);
  void spiExchange(SPIDriver *spip, size_t n, const void *txbuf, void *rxbuf);
  void spiSend(SPIDriver *spip, size_t n, const void *txbuf);
  void spiReceive(SPIDriver *spip, size_t n, void *rxbuf);
#if SPI_USE_MUTUAL_EXCLUSION
  void spiAcquireBus(SPIDriver *spip);
  void spiReleaseBus(SPIDriver *spip);
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#ifdef __cplusplus
}
#endif

#endif /* CH_HAL_USE_SPI */

#endif /* _SPI_H_ */

/** @} */
