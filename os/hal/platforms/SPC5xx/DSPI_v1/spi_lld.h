/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
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
 * @name    CTAR registers definitions
 * @{
 */
#define SPC5_CTAR_DBR                       (1U << 31)
#define SPC5_CTAR_FMSZ_MASK                 (15U << 27)
#define SPC5_CTAR_FMSZ(n)                   (((n) - 1) << 27)
#define SPC5_CTAR_CPOL                      (1U << 26)
#define SPC5_CTAR_CPHA                      (1U << 25)
#define SPC5_CTAR_LSBFE                     (1U << 24)
#define SPC5_CTAR_PCSSCK_MASK               (3U << 22)
#define SPC5_CTAR_PCSSCK_PRE1               (0U << 22)
#define SPC5_CTAR_PCSSCK_PRE3               (1U << 22)
#define SPC5_CTAR_PCSSCK_PRE5               (2U << 22)
#define SPC5_CTAR_PCSSCK_PRE7               (3U << 22)
#define SPC5_CTAR_PASC_MASK                 (3U << 20)
#define SPC5_CTAR_PASC_PRE1                 (0U << 20)
#define SPC5_CTAR_PASC_PRE3                 (1U << 20)
#define SPC5_CTAR_PASC_PRE5                 (2U << 20)
#define SPC5_CTAR_PASC_PRE7                 (3U << 20)
#define SPC5_CTAR_PDT_MASK                  (3U << 18)
#define SPC5_CTAR_PDT_PRE1                  (0U << 18)
#define SPC5_CTAR_PDT_PRE3                  (1U << 18)
#define SPC5_CTAR_PDT_PRE5                  (2U << 18)
#define SPC5_CTAR_PDT_PRE7                  (3U << 18)
#define SPC5_CTAR_PBR_MASK                  (3U << 16)
#define SPC5_CTAR_PBR_PRE2                  (0U << 16)
#define SPC5_CTAR_PBR_PRE3                  (1U << 16)
#define SPC5_CTAR_PBR_PRE5                  (2U << 16)
#define SPC5_CTAR_PBR_PRE7                  (3U << 16)
#define SPC5_CTAR_CSSCK_MASK                (15U << 12)
#define SPC5_CTAR_CSSCK_DIV2                (0U << 12)
#define SPC5_CTAR_CSSCK_DIV4                (1U << 12)
#define SPC5_CTAR_CSSCK_DIV6                (2U << 12)
#define SPC5_CTAR_CSSCK_DIV8                (3U << 12)
#define SPC5_CTAR_CSSCK_DIV16               (4U << 12)
#define SPC5_CTAR_CSSCK_DIV32               (5U << 12)
#define SPC5_CTAR_CSSCK_DIV64               (6U << 12)
#define SPC5_CTAR_CSSCK_DIV128              (7U << 12)
#define SPC5_CTAR_CSSCK_DIV256              (8U << 12)
#define SPC5_CTAR_CSSCK_DIV512              (9U << 12)
#define SPC5_CTAR_CSSCK_DIV1024             (10U << 12)
#define SPC5_CTAR_CSSCK_DIV2048             (11U << 12)
#define SPC5_CTAR_CSSCK_DIV4096             (12U << 12)
#define SPC5_CTAR_CSSCK_DIV8192             (13U << 12)
#define SPC5_CTAR_CSSCK_DIV16384            (14U << 12)
#define SPC5_CTAR_CSSCK_DIV32768            (15U << 12)
#define SPC5_CTAR_ASC_MASK                  (15U << 8)
#define SPC5_CTAR_ASC_DIV2                  (0U << 8)
#define SPC5_CTAR_ASC_DIV4                  (1U << 8)
#define SPC5_CTAR_ASC_DIV6                  (2U << 8)
#define SPC5_CTAR_ASC_DIV8                  (3U << 8)
#define SPC5_CTAR_ASC_DIV16                 (4U << 8)
#define SPC5_CTAR_ASC_DIV32                 (5U << 8)
#define SPC5_CTAR_ASC_DIV64                 (6U << 8)
#define SPC5_CTAR_ASC_DIV128                (7U << 8)
#define SPC5_CTAR_ASC_DIV256                (8U << 8)
#define SPC5_CTAR_ASC_DIV512                (9U << 8)
#define SPC5_CTAR_ASC_DIV1024               (10U << 8)
#define SPC5_CTAR_ASC_DIV2048               (11U << 8)
#define SPC5_CTAR_ASC_DIV4096               (12U << 8)
#define SPC5_CTAR_ASC_DIV8192               (13U << 8)
#define SPC5_CTAR_ASC_DIV16384              (14U << 8)
#define SPC5_CTAR_ASC_DIV32768              (15U << 8)
#define SPC5_CTAR_DT_MASK                   (15U << 4)
#define SPC5_CTAR_DT_DIV2                   (0U << 4)
#define SPC5_CTAR_DT_DIV4                   (1U << 4)
#define SPC5_CTAR_DT_DIV6                   (2U << 4)
#define SPC5_CTAR_DT_DIV8                   (3U << 4)
#define SPC5_CTAR_DT_DIV16                  (4U << 4)
#define SPC5_CTAR_DT_DIV32                  (5U << 4)
#define SPC5_CTAR_DT_DIV64                  (6U << 4)
#define SPC5_CTAR_DT_DIV128                 (7U << 4)
#define SPC5_CTAR_DT_DIV256                 (8U << 4)
#define SPC5_CTAR_DT_DIV512                 (9U << 4)
#define SPC5_CTAR_DT_DIV1024                (10U << 4)
#define SPC5_CTAR_DT_DIV2048                (11U << 4)
#define SPC5_CTAR_DT_DIV4096                (12U << 4)
#define SPC5_CTAR_DT_DIV8192                (13U << 4)
#define SPC5_CTAR_DT_DIV16384               (14U << 4)
#define SPC5_CTAR_DT_DIV32768               (15U << 4)
#define SPC5_CTAR_BR_MASK                   (15U << 0)
#define SPC5_CTAR_BR_DIV2                   (0U << 0)
#define SPC5_CTAR_BR_DIV4                   (1U << 0)
#define SPC5_CTAR_BR_DIV6                   (2U << 0)
#define SPC5_CTAR_BR_DIV8                   (3U << 0)
#define SPC5_CTAR_BR_DIV16                  (4U << 0)
#define SPC5_CTAR_BR_DIV32                  (5U << 0)
#define SPC5_CTAR_BR_DIV64                  (6U << 0)
#define SPC5_CTAR_BR_DIV128                 (7U << 0)
#define SPC5_CTAR_BR_DIV256                 (8U << 0)
#define SPC5_CTAR_BR_DIV512                 (9U << 0)
#define SPC5_CTAR_BR_DIV1024                (10U << 0)
#define SPC5_CTAR_BR_DIV2048                (11U << 0)
#define SPC5_CTAR_BR_DIV4096                (12U << 0)
#define SPC5_CTAR_BR_DIV8192                (13U << 0)
#define SPC5_CTAR_BR_DIV16384               (14U << 0)
#define SPC5_CTAR_BR_DIV32768               (15U << 0)
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

/**
 * @brief   SPI DMA error hook.
 */
#if !defined(SPC5_SPI_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define SPC5_SPI_DMA_ERROR_HOOK(spip)       chSysHalt()
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
   * @brief The chip select line port.
   */
  ioportid_t            ssport;
  /**
   * @brief The chip select line pad number.
   */
  uint16_t              sspad;
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
