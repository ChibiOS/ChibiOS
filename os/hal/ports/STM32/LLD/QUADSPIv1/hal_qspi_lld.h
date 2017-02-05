/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    QUADSPIv1/hal_qspi_lld.h
 * @brief   STM32 QSPI subsystem low level driver header.
 *
 * @addtogroup QSPI
 * @{
 */

#ifndef HAL_QSPI_LLD_H
#define HAL_QSPI_LLD_H

#if (HAL_USE_QSPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    QSPI capabilities
 * @{
 */
#define QSPI_SUPPORTS_MEMMAP                TRUE
/** @} */

/**
 * @name    DCR register options
 * @{
 */
#define STM32_DCR_CK_MODE                   (1U << 0U)
#define STM32_DCR_CSHT_MASK                 (7U << 8U)
#define STM32_DCR_CSHT(n)                   ((n) << 8U)
#define STM32_DCR_FSIZE_MASK                (31U << 16U)
#define STM32_DCR_FSIZE(n)                  ((n) << 16U)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   QUADSPI1 driver enable switch.
 * @details If set to @p TRUE the support for QUADSPI1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_QSPI_USE_QUADSPI1) || defined(__DOXYGEN__)
#define STM32_QSPI_USE_QUADSPI1             FALSE
#endif

/**
 * @brief   QUADSPI1 prescaler setting.
 * @note    This is the prescaler divider value 1..256. The maximum frequency
 *          varies depending on the STM32 model and operating conditions,
 *          find the details in the data sheet.
 */
#if !defined(STM32_QSPI_QUADSPI1_PRESCALER_VALUE) || defined(__DOXYGEN__)
#define STM32_QSPI_QUADSPI1_PRESCALER_VALUE 1
#endif

/**
 * @brief   QUADSPI1 interrupt priority level setting.
 */
#if !defined(STM32_QSPI_QUADSPI1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_QSPI_QUADSPI1_IRQ_PRIORITY    10
#endif

/**
 * @brief   QUADSPI1 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_QSPI_QUADSPI1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_QSPI_QUADSPI1_DMA_PRIORITY    1
#endif

/**
 * @brief   QUADSPI1 DMA interrupt priority level setting.
 */
#if !defined(STM32_QSPI_QUADSPI1_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_QSPI_QUADSPI1_DMA_IRQ_PRIORITY 10
#endif

/**
 * @brief   QUADSPI DMA error hook.
 */
#if !defined(STM32_QSPI_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_QSPI_DMA_ERROR_HOOK(qspip)    osalSysHalt("DMA failure")
#endif

/**
 * @brief   Enables a workaround for a STM32L476 QUADSPI errata.
 * @details The document DM00111498 states: "QUADSPI_BK1_IO1 is always an
 *          input when the command is sent in dual or quad SPI mode".
 *          This workaround makes commands without address or data phases
 *          to be sent as alternate bytes.
 */
#if !defined(STM32_USE_STM32_D1_WORKAROUND) || defined(__DOXYGEN__)
#define STM32_USE_STM32_D1_WORKAROUND       TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_QSPI_USE_QUADSPI1 && !STM32_HAS_QUADSPI1
#error "QUADSPI1 not present in the selected device"
#endif

#if !STM32_QSPI_USE_QUADSPI1
#error "QSPI driver activated but no QUADSPI peripheral assigned"
#endif

#if STM32_QSPI_USE_QUADSPI1 &&                                              \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_QSPI_QUADSPI1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to QUADSPI1"
#endif

#if STM32_QSPI_USE_QUADSPI1 &&                                              \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_QSPI_QUADSPI1_DMA_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to QUADSPI1 DMA"
#endif

#if STM32_QSPI_USE_QUADSPI1 &&                                              \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_QSPI_QUADSPI1_DMA_PRIORITY)
#error "Invalid DMA priority assigned to QUADSPI1"
#endif

#if (STM32_QSPI_QUADSPI1_PRESCALER_VALUE < 1) ||                            \
    (STM32_QSPI_QUADSPI1_PRESCALER_VALUE > 256)
#error "STM32_QSPI_QUADSPI1_PRESCALER_VALUE not within 1..256"
#endif

/* The following checks are only required when there is a DMA able to
   reassign streams to different channels.*/
#if STM32_ADVANCED_DMA
/* Check on the presence of the DMA streams settings in mcuconf.h.*/
#if STM32_QSPI_USE_QUADSPI1 && !defined(STM32_QSPI_QUADSPI1_DMA_STREAM)
#error "QUADSPI1 DMA stream not defined"
#endif

/* Check on the validity of the assigned DMA channels.*/
#if STM32_QSPI_USE_QUADSPI1 &&                                                   \
    !STM32_DMA_IS_VALID_ID(STM32_QSPI_QUADSPI1_DMA_STREAM, STM32_QUADSPI1_DMA_MSK)
#error "invalid DMA stream associated to QUADSPI1"
#endif
#endif /* STM32_ADVANCED_DMA */

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an QSPI driver.
 */
typedef struct QSPIDriver QSPIDriver;

/**
 * @brief   Type of a QSPI notification callback.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object triggering the
 *                      callback
 */
typedef void (*qspicallback_t)(QSPIDriver *qspip);

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief   Operation complete callback or @p NULL.
   */
  qspicallback_t             end_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief   DCR register initialization data.
   */
  uint32_t                  dcr;
} QSPIConfig;

/**
 * @brief   Structure representing an QSPI driver.
 */
struct QSPIDriver {
  /**
   * @brief   Driver state.
   */
  qspistate_t               state;
  /**
   * @brief   Current configuration data.
   */
  const QSPIConfig           *config;
#if (QSPI_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread.
   */
  thread_reference_t        thread;
#endif /* QSPI_USE_WAIT */
#if (QSPI_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the peripheral.
   */
  mutex_t                   mutex;
#endif /* QSPI_USE_MUTUAL_EXCLUSION */
#if defined(QSPI_DRIVER_EXT_FIELDS)
  QSPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Pointer to the QUADSPIx registers block.
   */
  QUADSPI_TypeDef           *qspi;
  /**
   * @brief   QUADSPI DMA stream.
   */
  const stm32_dma_stream_t  *dma;
  /**
   * @brief   QUADSPI DMA mode bit mask.
   */
  uint32_t                  dmamode;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (STM32_QSPI_USE_QUADSPI1 == TRUE) && !defined(__DOXYGEN__)
extern QSPIDriver QSPID1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void qspi_lld_init(void);
  void qspi_lld_start(QSPIDriver *qspip);
  void qspi_lld_stop(QSPIDriver *qspip);
  void qspi_lld_command(QSPIDriver *qspip, const qspi_command_t *cmdp);
  void qspi_lld_send(QSPIDriver *qspip, const qspi_command_t *cmdp,
                     size_t n, const uint8_t *txbuf);
  void qspi_lld_receive(QSPIDriver *qspip, const qspi_command_t *cmdp,
                        size_t n, uint8_t *rxbuf);
#if QSPI_SUPPORTS_MEMMAP == TRUE
  void qspi_lld_map_flash(QSPIDriver *qspip,
                          const qspi_command_t *cmdp,
                          uint8_t **addrp);
  void qspi_lld_unmap_flash(QSPIDriver *qspip);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_QSPI */

#endif /* HAL_QSPI_LLD_H */

/** @} */
