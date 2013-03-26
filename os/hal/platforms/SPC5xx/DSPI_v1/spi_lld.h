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
 * @file    templates/spi_lld.h
 * @brief   SPI Driver subsystem low level driver header template.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef _SPI_LLD_H_
#define _SPI_LLD_H_

#if HAL_USE_SPI || defined(__DOXYGEN__)

#include "spc5_dspi.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    MCR register definitions
 * @{
 */
#define SPC5_MCR_MSTR                       (1U << 31)
#define SPC5_MCR_CONT_SCKE                  (1U << 30)
#define SPC5_MCR_DCONF_MASK                 (3U << 28)
#define SPC5_MCR_FRZ                        (1U << 27)
#define SPC5_MCR_MTFE                       (1U << 26)
#define SPC5_MCR_PCSSE                      (1U << 25)
#define SPC5_MCR_ROOE                       (1U << 24)
#define SPC5_MCR_PCSIS7                     (1U << 23)
#define SPC5_MCR_PCSIS6                     (1U << 22)
#define SPC5_MCR_PCSIS5                     (1U << 21)
#define SPC5_MCR_PCSIS4                     (1U << 20)
#define SPC5_MCR_PCSIS3                     (1U << 19)
#define SPC5_MCR_PCSIS2                     (1U << 18)
#define SPC5_MCR_PCSIS1                     (1U << 17)
#define SPC5_MCR_PCSIS0                     (1U << 16)
#define SPC5_MCR_DOZE                       (1U << 15)
#define SPC5_MCR_MDIS                       (1U << 14)
#define SPC5_MCR_DIS_TXF                    (1U << 13)
#define SPC5_MCR_DIS_RXF                    (1U << 12)
#define SPC5_MCR_CLR_TXF                    (1U << 11)
#define SPC5_MCR_CLR_RXF                    (1U << 10)
#define SPC5_MCR_SMPL_PT_MASK               (3U << 8)
#define SPC5_MCR_SMPL_PT(n)                 ((n) << 8)
#define SPC5_MCR_FCPCS                      (1U << 2)
#define SPC5_MCR_PES                        (1U << 1)
#define SPC5_MCR_HALT                       (1U << 0)
/** @} */

/**
 * @name    RSER register definitions
 * @{
 */
#define SPC5_RSER_TCF_RE                    (1U << 31)
#define SPC5_RSER_DSITCF_RE                 (1U << 29)
#define SPC5_RSER_EOQF_RE                   (1U << 28)
#define SPC5_RSER_TFUF_RE                   (1U << 27)
#define SPC5_RSER_SPITCF_RE                 (1U << 26)
#define SPC5_RSER_TFFF_RE                   (1U << 25)
#define SPC5_RSER_TFFF_DIRS                 (1U << 24)
#define SPC5_RSER_DPEF_RE                   (1U << 22)
#define SPC5_RSER_SPEF_RE                   (1U << 21)
#define SPC5_RSER_DDIF_RE                   (1U << 20)
#define SPC5_RSER_RFOF_RE                   (1U << 19)
#define SPC5_RSER_RFDF_RE                   (1U << 17)
#define SPC5_RSER_RFDF_DIRS                 (1U << 16)
/** @} */

/**
 * @name    PUSHR register definitions
 * @{
 */
#define SPC5_PUSHR_CONT                     (1U << 31)
#define SPC5_PUSHR_CTAS_MASK                (3U << 28)
#define SPC5_PUSHR_CTAS(n)                  ((n) << 29)
#define SPC5_PUSHR_EOQ                      (1U << 27)
#define SPC5_PUSHR_CTCNT                    (1U << 26)
#define SPC5_PUSHR_MASC                     (1U << 25)
#define SPC5_PUSHR_MCSC                     (1U << 24)
#define SPC5_PUSHR_PCS_MASK                 (255U << 16)
#define SPC5_PUSHR_PCS(n)                   ((1U << (n)) << 16)
#define SPC5_PUSHR_TXDATA_MASK              (0xFFFFU << 0)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SPID1 driver enable switch.
 * @details If set to @p TRUE the support for DSPI0 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI0) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI0                  FALSE
#endif

/**
 * @brief   SPID2 driver enable switch.
 * @details If set to @p TRUE the support for DSPI1 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI1) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI1                  FALSE
#endif
/**
 * @brief   SPID3 driver enable switch.
 * @details If set to @p TRUE the support for DSPI2 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI2) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI2                  FALSE
#endif

/**
 * @brief   SPID4 driver enable switch.
 * @details If set to @p TRUE the support for DSPI3 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI3) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI3                  FALSE
#endif

/**
 * @brief   DSPI0 DMA priority.
 */
#if !defined(SPC5_SPI_DSPI0_DMA_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_DMA_PRIO             10
#endif

/**
 * @brief   DSPI1 DMA priority.
 */
#if !defined(SPC5_SPI_DSPI1_DMA_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_DMA_PRIO             10
#endif

/**
 * @brief   DSPI2 DMA priority.
 */
#if !defined(SPC5_SPI_DSPI2_DMA_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_DMA_PRIO             10
#endif

/**
 * @brief   DSPI3 DMA priority.
 */
#if !defined(SPC5_SPI_DSPI3_DMA_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_DMA_PRIO             10
#endif

/**
 * @brief   DSPI0 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI0_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI1 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI1_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI2 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI2_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   DSPI3 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI3_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_DMA_IRQ_PRIO         10
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if SPC5_SPI_USE_DSPI0 && !SPC5_HAS_DSPI0
#error "DSPI0 not present in the selected device"
#endif

#if SPC5_SPI_USE_DSPI1 && !SPC5_HAS_DSPI1
#error "DSPI1 not present in the selected device"
#endif

#if SPC5_SPI_USE_DSPI2 && !SPC5_HAS_DSPI2
#error "DSPI2 not present in the selected device"
#endif

#if SPC5_SPI_USE_DSPI3 && !SPC5_HAS_DSPI3
#error "DSPI3 not present in the selected device"
#endif

#if !SPC5_SPI_USE_DSPI0 && !SPC5_SPI_USE_DSPI1 &&                           \
    !SPC5_SPI_USE_DSPI2 && !SPC5_SPI_USE_DSPI3
#error "SPI driver activated but no DSPI peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an SPI driver.
 */
typedef struct SPIDriver SPIDriver;

/**
 * @brief   SPI notification callback type.
 *
 * @param[in] spip      pointer to the @p SPIDriver object triggering the
 *                      callback
 */
typedef void (*spicallback_t)(SPIDriver *spip);

/**
 * @brief   Driver configuration structure.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /**
   * @brief Operation complete callback.
   */
  spicallback_t         end_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief   DSPI MCR value for this session.
   * @note    Some bits are ignored: CONT_SCKE, DCONF, ROOE, MDIS, DIS_TXF,
   *          DIS_RXF, CLR_TXF, CLR_RXF, HALT.
   */
  uint32_t              mcr;
  /**
   * @brief   DSPI CTAR0 value for this session.
   */
  uint32_t              ctar0;
  /**
   * @brief   DSPI PUSHR command for this session.
   * @note    Only CTAR0 can be referenced, the other CTARs are not
   *          initialized. The data part must be left to zero.
   */
  uint32_t              pushr;
} SPIConfig;

/**
 * @brief   Structure representing an SPI driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct SPIDriver {
  /**
   * @brief Driver state.
   */
  spistate_t            state;
  /**
   * @brief Current configuration data.
   */
  const SPIConfig       *config;
#if SPI_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief Waiting thread.
   */
  Thread                *thread;
#endif /* SPI_USE_WAIT */
#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the bus.
   */
  Mutex                 mutex;
#elif CH_USE_SEMAPHORES
  Semaphore             semaphore;
#endif
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#if defined(SPI_DRIVER_EXT_FIELDS)
  SPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Pointer to the DSPI registers block.
   */
  struct spc5_dspi          *dspi;
  /**
   * @brief   EDMA channel used for transmit.
   */
  edma_channel_t            tx_channel;
  /**
   * @brief   EDMA channel used for receive.
   */
  edma_channel_t            rx_channel;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if SPC5_SPI_USE_DSPI0 && !defined(__DOXYGEN__)
extern SPIDriver SPID1;
#endif

#if SPC5_SPI_USE_DSPI1 && !defined(__DOXYGEN__)
extern SPIDriver SPID2;
#endif

#if SPC5_SPI_USE_DSPI2 && !defined(__DOXYGEN__)
extern SPIDriver SPID3;
#endif

#if SPC5_SPI_USE_DSPI3 && !defined(__DOXYGEN__)
extern SPIDriver SPID4;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void spi_lld_init(void);
  void spi_lld_start(SPIDriver *spip);
  void spi_lld_stop(SPIDriver *spip);
  void spi_lld_select(SPIDriver *spip);
  void spi_lld_unselect(SPIDriver *spip);
  void spi_lld_ignore(SPIDriver *spip, size_t n);
  void spi_lld_exchange(SPIDriver *spip, size_t n,
                        const void *txbuf, void *rxbuf);
  void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf);
  void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf);
  uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SPI */

#endif /* _SPI_LLD_H_ */

/** @} */
