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
 * @file templates/spi_lld.h
 * @brief SPI Driver subsystem low level driver header template
 * @addtogroup SPI_LLD
 * @{
 */

#ifndef _SPI_LLD_H_
#define _SPI_LLD_H_

#include <pal.h>

#undef FALSE
#undef TRUE
#include <stm32f10x.h>
#define FALSE 0
#define TRUE (!FALSE)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief SPI1 driver enable switch.
 * @details If set to @p TRUE the support for SPI1 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_SPI1) || defined(__DOXYGEN__)
#define USE_STM32_SPI1              TRUE
#endif

/**
 * @brief SPI2 driver enable switch.
 * @details If set to @p TRUE the support for SPI2 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_SPI2) || defined(__DOXYGEN__)
#define USE_STM32_SPI2              TRUE
#endif

/**
 * @brief SPI1 DMA priority (0..3|lowest..highest).
 * @note The priority level is used for both the TX and RX DMA channels but
 *       because of the channels ordering the RX channel has always priority
 *       over the TX channel.
 */
#if !defined(SPI1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define SPI1_DMA_PRIORITY           2
#endif

/**
 * @brief SPI2 DMA priority (0..3|lowest..highest).
 * @note The priority level is used for both the TX and RX DMA channels but
 *       because of the channels ordering the RX channel has always priority
 *       over the TX channel.
 */
#if !defined(SPI2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define SPI2_DMA_PRIORITY           2
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Driver state machine possible states.
 */
typedef enum {
  SPI_UNINIT = 0,//!< SPI_UNINIT
  SPI_IDLE = 1,  //!< SPI_IDLE
  SPI_ACTIVE = 2 //!< SPI_ACTIVE
} spistate_t;

/**
 * @brief Driver configuration structure.
 */
typedef struct {
  /**
   * @brief Clock pulses to be generated after initialization.
   */
  cnt_t                 spc_initcnt;
  /* End of the mandatory fields.*/
  /**
   * @brief The chip select line port.
   */
  ioportid_t            spc_ssport;
  /**
   * @brief The chip select line pad number.
   */
  uint16_t              spc_sspad;
  /**
   * @brief SPI initialization data.
   */
  uint16_t              spc_cr1;
} SPIConfig;

/**
 * @brief Structure representing a MAC driver.
 */
typedef struct {
  /**
   * @brief Driver state.
   */
  spistate_t            spd_state;
#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the bus.
   */
  Mutex                 spd_mutex;
#elif CH_USE_SEMAPHORES
  Semaphore             spd_semaphore;
#endif
#endif /* SPI_USE_MUTUAL_EXCLUSION */
  /**
   * @brief Current configuration data.
   */
  const SPIConfig       *spd_config;
  /* End of the mandatory fields.*/
  /**
   * @brief Thread waiting for I/O completion.
   */
  Thread                *spd_thread;
  /**
   * @brief Pointer to the SPIx registers block.
   */
  SPI_TypeDef           *spd_spi;
  /**
   * @brief Pointer to the receive DMA channel registers block.
   */
  DMA_Channel_TypeDef   *spd_dmarx;
  /**
   * @brief Pointer to the transmit DMA channel registers block.
   */
  DMA_Channel_TypeDef   *spd_dmatx;
  /**
   * @brief DMA priority bit mask.
   */
  uint32_t              spd_dmaprio;
} SPIDriver;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/** @cond never*/
#if USE_STM32_SPI1
extern SPIDriver SPID1;
#endif

#if USE_STM32_SPI2
extern SPIDriver SPID2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void spi_lld_init(void);
  void spi_lld_setup(SPIDriver *spip);
  void spi_lld_select(SPIDriver *spip);
  void spi_lld_unselect(SPIDriver *spip);
  msg_t spi_lld_exchange(SPIDriver *spip, size_t n, void *rxbuf, void *txbuf);
  msg_t spi_lld_send(SPIDriver *spip, size_t n, void *txbuf);
  msg_t spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf);
#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif /* _SPI_LLD_H_ */

/** @} */
