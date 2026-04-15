/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    QUADSPIv1/hal_wspi_lld.h
 * @brief   STM32 WSPI subsystem low level driver header.
 *
 * @addtogroup WSPI
 * @{
 */

#ifndef HAL_WSPI_LLD_H
#define HAL_WSPI_LLD_H

#if HAL_USE_WSPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    WSPI implementation capabilities
 * @{
 */
#define WSPI_SUPPORTS_MEMMAP                TRUE
#define WSPI_DEFAULT_CFG_MASKS              FALSE
/** @} */

/**
 * @name    Transfer options
 * @{
 */
#define WSPI_CFG_CMD_MODE_MASK              (3U << 8)
#define WSPI_CFG_CMD_MODE_NONE              (0U << 8)
#define WSPI_CFG_CMD_MODE_ONE_LINE          (1U << 8)
#define WSPI_CFG_CMD_MODE_TWO_LINES         (2U << 8)
#define WSPI_CFG_CMD_MODE_FOUR_LINES        (3U << 8)
#define WSPI_CFG_CMD_MODE_EIGHT_LINES       0U

#define WSPI_CFG_CMD_DTR                    (1U << 31)

#define WSPI_CFG_CMD_SIZE_MASK              0U
#define WSPI_CFG_CMD_SIZE_8                 0U
#define WSPI_CFG_CMD_SIZE_16                0U
#define WSPI_CFG_CMD_SIZE_24                0U
#define WSPI_CFG_CMD_SIZE_32                0U

#define WSPI_CFG_ADDR_MODE_MASK             (3U << 10)
#define WSPI_CFG_ADDR_MODE_NONE             (0U << 10)
#define WSPI_CFG_ADDR_MODE_ONE_LINE         (1U << 10)
#define WSPI_CFG_ADDR_MODE_TWO_LINES        (2U << 10)
#define WSPI_CFG_ADDR_MODE_FOUR_LINES       (3U << 10)
#define WSPI_CFG_ADDR_MODE_EIGHT_LINES      0U

#define WSPI_CFG_ADDR_DTR                   (1U << 31)

#define WSPI_CFG_ADDR_SIZE_MASK             (3U << 12)
#define WSPI_CFG_ADDR_SIZE_8                (0U << 12)
#define WSPI_CFG_ADDR_SIZE_16               (1U << 12)
#define WSPI_CFG_ADDR_SIZE_24               (2U << 12)
#define WSPI_CFG_ADDR_SIZE_32               (3U << 12)

#define WSPI_CFG_ALT_MODE_MASK              (3U << 14)
#define WSPI_CFG_ALT_MODE_NONE              (0U << 14)
#define WSPI_CFG_ALT_MODE_ONE_LINE          (1U << 14)
#define WSPI_CFG_ALT_MODE_TWO_LINES         (2U << 14)
#define WSPI_CFG_ALT_MODE_FOUR_LINES        (3U << 14)
#define WSPI_CFG_ALT_MODE_EIGHT_LINES       0U

#define WSPI_CFG_ALT_DTR                    (1U << 31)

#define WSPI_CFG_ALT_SIZE_MASK              (3U << 16)
#define WSPI_CFG_ALT_SIZE_8                 (0U << 16)
#define WSPI_CFG_ALT_SIZE_16                (1U << 16)
#define WSPI_CFG_ALT_SIZE_24                (2U << 16)
#define WSPI_CFG_ALT_SIZE_32                (3U << 16)

#define WSPI_CFG_DATA_MODE_MASK             (3U << 24)
#define WSPI_CFG_DATA_MODE_NONE             (0U << 24)
#define WSPI_CFG_DATA_MODE_ONE_LINE         (1U << 24)
#define WSPI_CFG_DATA_MODE_TWO_LINES        (2U << 24)
#define WSPI_CFG_DATA_MODE_FOUR_LINES       (3U << 24)
#define WSPI_CFG_DATA_MODE_EIGHT_LINES      0U

#define WSPI_CFG_DATA_DTR                   (1U << 31)

#define WSPI_CFG_DQS_ENABLE                 0U

#define WSPI_CFG_SIOO                       (1U << 28)

#define WSPI_CFG_ALL_DTR                    (WSPI_CFG_CMD_DTR   |           \
                                             WSPI_CFG_ADDR_DTR  |           \
                                             WSPI_CFG_ALT_DTR   |           \
                                             WSPI_CFG_DATA_DTR)
/** @} */

/**
 * @name    QUADSPI helper macros
 * @{
 */
#define QUADSPI_CCR_DUMMY_CYCLES_MASK       (0x1FU << 18)
#define QUADSPI_CCR_DUMMY_CYCLES(n)         ((n) << 18)
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
 */
#if !defined(STM32_WSPI_USE_QUADSPI1) || defined(__DOXYGEN__)
#define STM32_WSPI_USE_QUADSPI1             FALSE
#endif

/**
 * @brief   QUADSPI1 prescaler setting.
 */
#if !defined(STM32_WSPI_QUADSPI1_PRESCALER_VALUE) || defined(__DOXYGEN__)
#define STM32_WSPI_QUADSPI1_PRESCALER_VALUE 1
#endif

/**
 * @brief   QUADSPI1 interrupt priority level setting.
 */
#if !defined(STM32_IRQ_QUADSPI1_PRIORITY) || defined(__DOXYGEN__)
#define STM32_IRQ_QUADSPI1_PRIORITY         10
#endif

/**
 * @brief   QUADSPI1 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_WSPI_QUADSPI1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_WSPI_QUADSPI1_DMA_PRIORITY    1
#endif

/**
 * @brief   QUADSPI1 DMA interrupt priority level setting.
 */
#if !defined(STM32_WSPI_QUADSPI1_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_WSPI_QUADSPI1_DMA_IRQ_PRIORITY 10
#endif

/**
 * @brief   QUADSPI DMA error hook.
 */
#if !defined(STM32_WSPI_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_WSPI_DMA_ERROR_HOOK(wspip)    osalSysHalt("DMA failure")
#endif

/**
 * @brief   Enables a workaround for a STM32 QUADSPI errata.
 */
#if !defined(STM32_USE_STM32_D1_WORKAROUND) || defined(__DOXYGEN__)
#define STM32_USE_STM32_D1_WORKAROUND       TRUE
#endif

/**
 * @brief   Default DCR value for the WSPI default configuration.
 */
#if !defined(STM32_WSPI_DEFAULT_DCR) || defined(__DOXYGEN__)
#define STM32_WSPI_DEFAULT_DCR              0U
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_HAS_QUADSPI1)
#define STM32_HAS_QUADSPI1                  FALSE
#endif

#if STM32_WSPI_USE_QUADSPI1 && !STM32_HAS_QUADSPI1
#error "QUADSPI1 not present in the selected device"
#endif

#if !STM32_WSPI_USE_QUADSPI1
#error "WSPI driver activated but no QUADSPI peripheral assigned"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_QUADSPI1_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_QUADSPI1_PRIORITY"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_WSPI_QUADSPI1_DMA_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_WSPI_QUADSPI1_DMA_IRQ_PRIORITY"
#endif

#if !STM32_DMA_IS_VALID_PRIORITY(STM32_WSPI_QUADSPI1_DMA_PRIORITY)
#error "Invalid DMA priority assigned to QUADSPI1"
#endif

#if (STM32_WSPI_QUADSPI1_PRESCALER_VALUE < 1) ||                            \
    (STM32_WSPI_QUADSPI1_PRESCALER_VALUE > 256)
#error "STM32_WSPI_QUADSPI1_PRESCALER_VALUE not within 1..256"
#endif

#if !defined(STM32_WSPI_QUADSPI1_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_WSPI_QUADSPI1_DMA_STREAM      STM32_DMA_STREAM_ID_ANY
#endif

#if !STM32_DMA_IS_VALID_STREAM(STM32_WSPI_QUADSPI1_DMA_STREAM)
#error "invalid DMA stream associated to QUADSPI1"
#endif

#if !STM32_DMA_SUPPORTS_DMAMUX
#if !STM32_DMA_IS_VALID_ID(STM32_WSPI_QUADSPI1_DMA_STREAM, STM32_QUADSPI1_DMA_MSK)
#error "invalid DMA stream associated to QUADSPI1"
#endif
#endif

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Low level fields of the WSPI configuration structure.
 */
#define wspi_lld_config_fields                                              \
  uint32_t                  dcr;

/**
 * @brief   Low level fields of the WSPI driver structure.
 */
#define wspi_lld_driver_fields                                              \
  QUADSPI_TypeDef           *qspi;                                          \
  const stm32_dma_stream_t  *dma;                                           \
  uint32_t                  dmamode;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_WSPI_USE_QUADSPI1 && !defined(__DOXYGEN__)
extern hal_wspi_driver_c WSPID1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void wspi_lld_init(void);
  msg_t wspi_lld_start(hal_wspi_driver_c *wspip);
  void wspi_lld_stop(hal_wspi_driver_c *wspip);
  const hal_wspi_config_t *wspi_lld_setcfg(hal_wspi_driver_c *wspip,
                                           const hal_wspi_config_t *config);
  const hal_wspi_config_t *wspi_lld_selcfg(hal_wspi_driver_c *wspip,
                                           unsigned cfgnum);
  void wspi_lld_command(hal_wspi_driver_c *wspip, const wspi_command_t *cmdp);
  void wspi_lld_send(hal_wspi_driver_c *wspip, const wspi_command_t *cmdp,
                     size_t n, const uint8_t *txbuf);
  void wspi_lld_receive(hal_wspi_driver_c *wspip, const wspi_command_t *cmdp,
                        size_t n, uint8_t *rxbuf);
#if WSPI_SUPPORTS_MEMMAP == TRUE
  void wspi_lld_map_flash(hal_wspi_driver_c *wspip,
                          const wspi_command_t *cmdp,
                          uint8_t **addrp);
  void wspi_lld_unmap_flash(hal_wspi_driver_c *wspip);
#endif
  void wspi_lld_serve_interrupt(hal_wspi_driver_c *wspip);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_WSPI */

#endif /* HAL_WSPI_LLD_H */

/** @} */
