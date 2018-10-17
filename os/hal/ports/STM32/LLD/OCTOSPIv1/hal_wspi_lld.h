/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    OCTOSPIv1/hal_wspi_lld.h
 * @brief   STM32 WSPI subsystem low level driver header.
 *
 * @addtogroup WSPI
 * @{
 */

#ifndef HAL_WSPI_LLD_H
#define HAL_WSPI_LLD_H

#if (HAL_USE_WSPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    WSPI implementation capabilities
 * @{
 */
#define WSPI_SUPPORTS_MEMMAP                TRUE
#define WSPI_DEFAULT_CFG_MASKS              TRUE
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   WSPID1 driver enable switch.
 * @details If set to @p TRUE the support for OCTOSPI1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_WSPI_USE_OCTOSPI1) || defined(__DOXYGEN__)
#define STM32_WSPI_USE_OCTOSPI1             FALSE
#endif

/**
 * @brief   OCTOSPI1 prescaler setting.
 * @note    This is the prescaler divider value 1..256. The maximum frequency
 *          varies depending on the STM32 model and operating conditions,
 *          find the details in the data sheet.
 */
#if !defined(STM32_WSPI_OCTOSPI1_PRESCALER_VALUE) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_PRESCALER_VALUE 1
#endif

/**
 * @brief   OCTOSPI1 interrupt priority level setting.
 */
#if !defined(STM32_WSPI_OCTOSPI1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_IRQ_PRIORITY    10
#endif

/**
 * @brief   OCTOSPI1 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_WSPI_OCTOSPI1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_DMA_PRIORITY    1
#endif

/**
 * @brief   OCTOSPI1 DMA interrupt priority level setting.
 */
#if !defined(STM32_WSPI_OCTOSPI1_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_DMA_IRQ_PRIORITY 10
#endif

/**
 * @brief   OCTOSPI DMA error hook.
 */
#if !defined(STM32_WSPI_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_WSPI_DMA_ERROR_HOOK(qspip)    osalSysHalt("DMA failure")
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_HAS_OCTOSPI1)
#define STM32_HAS_OCTOSPI1                  FALSE
#endif

#if STM32_WSPI_USE_OCTOSPI1 && !STM32_HAS_OCTOSPI1
#error "OCTOSPI1 not present in the selected device"
#endif

#if !STM32_WSPI_USE_OCTOSPI1
#error "WSPI driver activated but no OCTOSPI peripheral assigned"
#endif

/* Check on OCTOSPI prescaler setting.*/
#if (STM32_WSPI_OCTOSPI1_PRESCALER_VALUE < 1) ||                            \
    (STM32_WSPI_OCTOSPI1_PRESCALER_VALUE > 256)
#error "STM32_WSPI_OCTOSPI1_PRESCALER_VALUE not within 1..256"
#endif

/* Check on IRQ priorities.*/
#if STM32_WSPI_USE_OCTOSPI1 &&                                              \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_WSPI_OCTOSPI1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to OCTOSPI1"
#endif

#if STM32_WSPI_USE_OCTOSPI1 &&                                              \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_WSPI_OCTOSPI1_DMA_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to OCTOSPI1 DMA"
#endif

/* Check on the presence of the DMA chennels settings in mcuconf.h.*/
#if STM32_WSPI_USE_OCTOSPI1 && !defined(STM32_WSPI_OCTOSPI1_DMA_CHANNEL)
#error "OCTOSPI1 DMA channel not defined"
#endif

/* Check on the validity of the assigned DMA channels.*/
#if STM32_WSPI_USE_OCTOSPI1 &&                                              \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_OCTOSPI1_DMA_MSK)
#error "invalid DMA stream associated to OCTOSPI1"
#endif

/* Check on DMA channels priority.*/
#if STM32_WSPI_USE_OCTOSPI1 &&                                              \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_WSPI_OCTOSPI1_DMA_PRIORITY)
#error "Invalid DMA priority assigned to OCTOSPI1"
#endif

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver configuration structure.
 */
struct hal_wspi_config {
  /**
   * @brief   Operation complete callback or @p NULL.
   */
  wspicallback_t            end_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief   DCR1 register initialization data.
   */
  uint32_t                  dcr1;
  /**
   * @brief   DCR2 register initialization data.
   */
  uint32_t                  dcr2;
  /**
   * @brief   DCR3 register initialization data.
   */
  uint32_t                  dcr3;
};

/**
 * @brief   Structure representing an WSPI driver.
 */
struct hal_wspi_driver {
  /**
   * @brief   Driver state.
   */
  wspistate_t               state;
  /**
   * @brief   Current configuration data.
   */
  const WSPIConfig          *config;
#if (WSPI_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread.
   */
  thread_reference_t        thread;
#endif /* WSPI_USE_WAIT */
#if (WSPI_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the peripheral.
   */
  mutex_t                   mutex;
#endif /* WSPI_USE_MUTUAL_EXCLUSION */
#if defined(WSPI_DRIVER_EXT_FIELDS)
  WSPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Pointer to the OCTOSPIx registers block.
   */
  OCTOSPI_TypeDef           *ospi;
  /**
   * @brief   OCTOSPI DMA stream.
   */
  const stm32_dma_stream_t  *dma;
  /**
   * @brief   OCTOSPI DMA mode bit mask.
   */
  uint32_t                  dmamode;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (STM32_WSPI_USE_OCTOSPI1 == TRUE) && !defined(__DOXYGEN__)
extern WSPIDriver WSPID1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void wspi_lld_init(void);
  void wspi_lld_start(WSPIDriver *wspip);
  void wspi_lld_stop(WSPIDriver *wspip);
  void wspi_lld_command(WSPIDriver *wspip, const wspi_command_t *cmdp);
  void wspi_lld_send(WSPIDriver *wspip, const wspi_command_t *cmdp,
                     size_t n, const uint8_t *txbuf);
  void wspi_lld_receive(WSPIDriver *wspip, const wspi_command_t *cmdp,
                        size_t n, uint8_t *rxbuf);
#if WSPI_SUPPORTS_MEMMAP == TRUE
  void wspi_lld_map_flash(WSPIDriver *wspip,
                          const wspi_command_t *cmdp,
                          uint8_t **addrp);
  void wspi_lld_unmap_flash(WSPIDriver *wspip);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_WSPI */

#endif /* HAL_WSPI_LLD_H */

/** @} */
