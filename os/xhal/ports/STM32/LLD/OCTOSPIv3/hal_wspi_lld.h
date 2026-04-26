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
 * @file    OCTOSPIv3/hal_wspi_lld.h
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
#define WSPI_CFG_CMD_MODE_MASK              (7U << 0U)
#define WSPI_CFG_CMD_MODE_NONE              (0U << 0U)
#define WSPI_CFG_CMD_MODE_ONE_LINE          (1U << 0U)
#define WSPI_CFG_CMD_MODE_TWO_LINES         (2U << 0U)
#define WSPI_CFG_CMD_MODE_FOUR_LINES        (3U << 0U)
#define WSPI_CFG_CMD_MODE_EIGHT_LINES       (4U << 0U)

#define WSPI_CFG_CMD_DTR                    (1U << 3U)

#define WSPI_CFG_CMD_SIZE_MASK              (3U << 4U)
#define WSPI_CFG_CMD_SIZE_8                 (0U << 4U)
#define WSPI_CFG_CMD_SIZE_16                (1U << 4U)
#define WSPI_CFG_CMD_SIZE_24                (2U << 4U)
#define WSPI_CFG_CMD_SIZE_32                (3U << 4U)

#define WSPI_CFG_ADDR_MODE_MASK             (7U << 8U)
#define WSPI_CFG_ADDR_MODE_NONE             (0U << 8U)
#define WSPI_CFG_ADDR_MODE_ONE_LINE         (1U << 8U)
#define WSPI_CFG_ADDR_MODE_TWO_LINES        (2U << 8U)
#define WSPI_CFG_ADDR_MODE_FOUR_LINES       (3U << 8U)
#define WSPI_CFG_ADDR_MODE_EIGHT_LINES      (4U << 8U)

#define WSPI_CFG_ADDR_DTR                   (1U << 11U)

#define WSPI_CFG_ADDR_SIZE_MASK             (3U << 12U)
#define WSPI_CFG_ADDR_SIZE_8                (0U << 12U)
#define WSPI_CFG_ADDR_SIZE_16               (1U << 12U)
#define WSPI_CFG_ADDR_SIZE_24               (2U << 12U)
#define WSPI_CFG_ADDR_SIZE_32               (3U << 12U)

#define WSPI_CFG_ALT_MODE_MASK              (7U << 16U)
#define WSPI_CFG_ALT_MODE_NONE              (0U << 16U)
#define WSPI_CFG_ALT_MODE_ONE_LINE          (1U << 16U)
#define WSPI_CFG_ALT_MODE_TWO_LINES         (2U << 16U)
#define WSPI_CFG_ALT_MODE_FOUR_LINES        (3U << 16U)
#define WSPI_CFG_ALT_MODE_EIGHT_LINES       (4U << 16U)

#define WSPI_CFG_ALT_DTR                    (1U << 19U)

#define WSPI_CFG_ALT_SIZE_MASK              (3U << 20U)
#define WSPI_CFG_ALT_SIZE_8                 (0U << 20U)
#define WSPI_CFG_ALT_SIZE_16                (1U << 20U)
#define WSPI_CFG_ALT_SIZE_24                (2U << 20U)
#define WSPI_CFG_ALT_SIZE_32                (3U << 20U)

#define WSPI_CFG_DATA_MODE_MASK             (7U << 24U)
#define WSPI_CFG_DATA_MODE_NONE             (0U << 24U)
#define WSPI_CFG_DATA_MODE_ONE_LINE         (1U << 24U)
#define WSPI_CFG_DATA_MODE_TWO_LINES        (2U << 24U)
#define WSPI_CFG_DATA_MODE_FOUR_LINES       (3U << 24U)
#define WSPI_CFG_DATA_MODE_EIGHT_LINES      (4U << 24U)

#define WSPI_CFG_DATA_DTR                   (1U << 27U)

#define WSPI_CFG_DQS_ENABLE                 (1U << 29U)

#define WSPI_CFG_SIOO                       (1U << 31U)

#define WSPI_CFG_ALL_DTR                    (WSPI_CFG_CMD_DTR   |           \
                                             WSPI_CFG_ADDR_DTR  |           \
                                             WSPI_CFG_ALT_DTR   |           \
                                             WSPI_CFG_DATA_DTR)
/** @} */

/**
 * @name    DCR1 register options
 * @{
 */
#define STM32_DCR1_CK_MODE                  (1U << 0U)
#define STM32_DCR1_FRCK_MODE                (1U << 1U)
#define STM32_DCR1_DLYBYP_MODE              (1U << 3U)
#define STM32_DCR1_CSHT_MASK                (63U << 8U)
#define STM32_DCR1_CSHT(n)                  ((n) << 8U)
#define STM32_DCR1_DEVSIZE_MASK             (31U << 16U)
#define STM32_DCR1_DEVSIZE(n)               ((n) << 16U)
#define STM32_DCR1_MTYP_MASK                (7U << 24U)
#define STM32_DCR1_MTYP(n)                  ((n) << 24U)
/** @} */

/**
 * @name    DCR2 register options
 * @{
 */
#define STM32_DCR2_PRESCALER_MASK           (255U << 0U)
#define STM32_DCR2_PRESCALER(n)             ((n) << 0U)
#define STM32_DCR2_WRAPSIZE_MASK            (7U << 16U)
#define STM32_DCR2_WRAPSIZE(n)              ((n) << 16U)
/** @} */

/**
 * @name    DCR3 register options
 * @{
 */
#define STM32_DCR3_MAXTRAN_MASK             (255U << 0U)
#define STM32_DCR3_MAXTRAN(n)               ((n) << 0U)
#define STM32_DCR3_CSBOUND_MASK             (31U << 16U)
#define STM32_DCR3_CSBOUND(n)               ((n) << 16U)
/** @} */

/**
 * @name    DCR4 register options
 * @{
 */
#define STM32_DCR4_REFRESH_MASK             (0xFFFFFFFFU)
#define STM32_DCR4_REFRESH(n)               ((n) << 0U)
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
 */
#if !defined(STM32_WSPI_USE_OCTOSPI1) || defined(__DOXYGEN__)
#define STM32_WSPI_USE_OCTOSPI1             FALSE
#endif

/**
 * @brief   WSPID1 prescaler setting.
 */
#if !defined(STM32_WSPI_OCTOSPI1_PRESCALER_VALUE) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_PRESCALER_VALUE 1
#endif

/**
 * @brief   OCTOSPI1 TCR_SSHIFT enforcing.
 */
#if !defined(STM32_WSPI_OCTOSPI1_SSHIFT) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_SSHIFT          FALSE
#endif

/**
 * @brief   OCTOSPI1 TCR_DHQC enforcing.
 */
#if !defined(STM32_WSPI_OCTOSPI1_DHQC) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_DHQC            FALSE
#endif

/**
 * @brief   OCTOSPI1 DMA3 channels mask.
 */
#if !defined(STM32_WSPI_OCTOSPI1_DMA3_CHANNEL) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_DMA3_CHANNEL    STM32_DMA3_MASK_ANY
#endif

/**
 * @brief   OCTOSPI1 DMA3 priority (0..3|lowest..highest).
 */
#if !defined(STM32_WSPI_OCTOSPI1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_DMA_PRIORITY    1
#endif

/**
 * @brief   OCTOSPI1 DMA3 interrupt priority level setting.
 */
#if !defined(STM32_WSPI_OCTOSPI1_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_WSPI_OCTOSPI1_DMA_IRQ_PRIORITY 10
#endif

/**
 * @brief   Default DCR1 value for the WSPI default configuration.
 */
#if !defined(STM32_WSPI_DEFAULT_DCR1) || defined(__DOXYGEN__)
#define STM32_WSPI_DEFAULT_DCR1             0U
#endif

/**
 * @brief   Default DCR2 value for the WSPI default configuration.
 */
#if !defined(STM32_WSPI_DEFAULT_DCR2) || defined(__DOXYGEN__)
#define STM32_WSPI_DEFAULT_DCR2             0U
#endif

/**
 * @brief   Default DCR3 value for the WSPI default configuration.
 */
#if !defined(STM32_WSPI_DEFAULT_DCR3) || defined(__DOXYGEN__)
#define STM32_WSPI_DEFAULT_DCR3             0U
#endif

/**
 * @brief   Default DCR4 value for the WSPI default configuration.
 */
#if !defined(STM32_WSPI_DEFAULT_DCR4) || defined(__DOXYGEN__)
#define STM32_WSPI_DEFAULT_DCR4             0U
#endif

/**
 * @brief   OCTOSPI DMA error hook.
 */
#if !defined(STM32_WSPI_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_WSPI_DMA_ERROR_HOOK(wspip)    osalSysHalt("DMA failure")
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Workarounds for bugs in ST headers.*/
#if !defined(OCTOSPI_FCR_CTOF) && defined(OCTOSPI_FCR_TOF)
#define OCTOSPI_FCR_CTOF                    OCTOSPI_FCR_TOF
#endif

#if !defined(STM32_HAS_OCTOSPI1)
#define STM32_HAS_OCTOSPI1                  FALSE
#endif

#if !STM32_HAS_OCTOSPI1
#error "OCTOSPI1 not present in the selected device"
#endif

#if !STM32_WSPI_USE_OCTOSPI1
#error "WSPI driver activated but no OCTOSPI peripheral assigned"
#endif

#if (STM32_WSPI_OCTOSPI1_PRESCALER_VALUE < 1) ||                            \
    (STM32_WSPI_OCTOSPI1_PRESCALER_VALUE > 256)
#error "STM32_WSPI_OCTOSPI1_PRESCALER_VALUE not within 1..256"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_WSPI_OCTOSPI1_DMA_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to OCTOSPI1 DMA"
#endif

#if !STM32_DMA3_ARE_VALID_CHANNELS(STM32_WSPI_OCTOSPI1_DMA3_CHANNEL)
#error "invalid DMA3 channel mask associated to OCTOSPI1"
#endif

#if !STM32_DMA3_IS_VALID_PRIORITY(STM32_WSPI_OCTOSPI1_DMA_PRIORITY)
#error "Invalid DMA3 priority assigned to OCTOSPI1"
#endif

#if !defined(STM32_DMA3_REQ_OSPI1)
#error "STM32_DMA3_REQ_OSPI1 not defined in registry"
#endif

#if !defined(STM32_DMA3_REQUIRED)
#define STM32_DMA3_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Low level fields of the WSPI configuration structure.
 */
#define wspi_lld_config_fields                                              \
  uint32_t                  dcr1;                                           \
  uint32_t                  dcr2;                                           \
  uint32_t                  dcr3;                                           \
  uint32_t                  dcr4;

/**
 * @brief   Low level fields of the WSPI driver structure.
 */
#define wspi_lld_driver_fields                                              \
  uint32_t                  extra_tcr;                                      \
  OCTOSPI_TypeDef           *ospi;                                          \
  const stm32_dma3_channel_t *dmachp;                                       \
  uint8_t                   dreq;                                           \
  uint8_t                   dprio;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_WSPI_USE_OCTOSPI1 && !defined(__DOXYGEN__)
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
