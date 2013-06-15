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
 * @file    SPC5xx/DSPI_v1/spi_lld.h
 * @brief   SPC5xx SPI subsystem low level driver header.
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
 * @brief   SPID5 driver enable switch.
 * @details If set to @p TRUE the support for DSPI4 is included.
 */
#if !defined(SPC5_SPI_USE_DSPI4) || defined(__DOXYGEN__)
#define SPC5_SPI_USE_DSPI4                  FALSE
#endif

/**
 * @brief   DSPI0 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI0_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI1 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI1_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSP2 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI2_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI3 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI3_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
#endif

/**
 * @brief   DSPI4 MCR PCS defaults.
 */
#if !defined(SPC5_SPI_DSPI4_MCR) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_MCR                  (SPC5_MCR_PCSIS0 |              \
                                             SPC5_MCR_PCSIS1 |              \
                                             SPC5_MCR_PCSIS2 |              \
                                             SPC5_MCR_PCSIS3 |              \
                                             SPC5_MCR_PCSIS4 |              \
                                             SPC5_MCR_PCSIS5 |              \
                                             SPC5_MCR_PCSIS6 |              \
                                             SPC5_MCR_PCSIS7)
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
 * @brief   DSPI4 DMA IRQ priority.
 */
#if !defined(SPC5_SPI_DSPI4_DMA_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_DMA_IRQ_PRIO         10
#endif

/**
 * @brief   SPI DMA error hook.
 */
#if !defined(SPC5_SPI_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define SPC5_SPI_DMA_ERROR_HOOK(spip)       chSysHalt()
#endif

/**
 * @brief   DSPI0 DMA priority.
 */
#if !defined(SPC5_SPI_DSPI0_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI1 DMA priority.
 */
#if !defined(SPC5_SPI_DSPI1_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI2 DMA priority.
 */
#if !defined(SPC5_SPI_DSPI2_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI3 DMA priority.
 */
#if !defined(SPC5_SPI_DSPI3_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI4 DMA priority.
 */
#if !defined(SPC5_SPI_DSPI4_IRQ_PRIO) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_IRQ_PRIO             10
#endif

/**
 * @brief   DSPI0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI0_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI1_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI2 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI2_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI2 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI2_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI2_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI3 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI3_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI3 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI3_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI3_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif

/**
 * @brief   DSPI4 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI4_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_START_PCTL           (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   DSPI4 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_SPI_DSPI4_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_SPI_DSPI4_STOP_PCTL            (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
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

#if SPC5_SPI_USE_DSPI4 && !SPC5_HAS_DSPI4
#error "DSPI4 not present in the selected device"
#endif

#if !SPC5_SPI_USE_DSPI0 && !SPC5_SPI_USE_DSPI1 &&                           \
    !SPC5_SPI_USE_DSPI2 && !SPC5_SPI_USE_DSPI3 &&                           \
    !SPC5_SPI_USE_DSPI4
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
   * @brief   Operation complete callback.
   */
  spicallback_t         end_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief   The chip select line port.
   */
  ioportid_t            ssport;
  /**
   * @brief   The chip select line pad number.
   */
  uint16_t              sspad;
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
   * @brief   Driver state.
   */
  spistate_t            state;
  /**
   * @brief   Current configuration data.
   */
  const SPIConfig       *config;
#if SPI_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread.
   */
  Thread                *thread;
#endif /* SPI_USE_WAIT */
#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the bus.
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
   * @brief   EDMA channel used for data memory to memory copy.
   */
  edma_channel_t            tx1_channel;
  /**
   * @brief   EDMA channel used for transmit.
   */
  edma_channel_t            tx2_channel;
  /**
   * @brief   EDMA channel used for receive.
   */
  edma_channel_t            rx_channel;
  /**
   * @brief   Last frame of a transmission sequence.
   */
  uint32_t                  tx_last;
  /**
   * @brief   TX intermediate buffer.
   * @note    This field is written by the TX1 DMA channel and read by the
   *          TX2 DMA channel.
   */
  uint32_t                  tx_intbuf;
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

#if SPC5_SPI_USE_DSPI4 && !defined(__DOXYGEN__)
extern SPIDriver SPID5;
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
