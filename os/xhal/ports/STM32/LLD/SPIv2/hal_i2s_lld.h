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
 * @file    SPIv2/hal_i2s_lld.h
 * @brief   STM32 I2S subsystem low level driver header.
 *
 * @addtogroup I2S
 * @{
 */

#ifndef HAL_I2S_LLD_H
#define HAL_I2S_LLD_H

#if HAL_USE_I2S || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Static I2S modes
 * @{
 */
#define STM32_I2S_MODE_SLAVE                0U
#define STM32_I2S_MODE_MASTER               1U
#define STM32_I2S_MODE_RX                   2U
#define STM32_I2S_MODE_TX                   4U
#define STM32_I2S_MODE_RXTX                 (STM32_I2S_MODE_RX |              \
                                             STM32_I2S_MODE_TX)
/** @} */

/**
 * @name    Mode checks
 * @{
 */
#define STM32_I2S_IS_MASTER(mode)           (((mode) & STM32_I2S_MODE_MASTER) != 0U)
#define STM32_I2S_RX_ENABLED(mode)          (((mode) & STM32_I2S_MODE_RX) != 0U)
#define STM32_I2S_TX_ENABLED(mode)          (((mode) & STM32_I2S_MODE_TX) != 0U)
/** @} */

/**
 * @name    I2S error flags
 * @{
 */
#define I2S_NO_ERROR                        0U
#define I2S_ERR_DMAFAILURE                  (1U << 0)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
#if !defined(STM32_I2S_USE_SPI1) || defined(__DOXYGEN__)
#define STM32_I2S_USE_SPI1                  FALSE
#endif

#if !defined(STM32_I2S_USE_SPI2) || defined(__DOXYGEN__)
#define STM32_I2S_USE_SPI2                  FALSE
#endif

#if !defined(STM32_I2S_USE_SPI3) || defined(__DOXYGEN__)
#define STM32_I2S_USE_SPI3                  FALSE
#endif

#if !defined(STM32_I2S_SPI1_MODE) || defined(__DOXYGEN__)
#define STM32_I2S_SPI1_MODE                 (STM32_I2S_MODE_MASTER |          \
                                             STM32_I2S_MODE_RX)
#endif

#if !defined(STM32_I2S_SPI2_MODE) || defined(__DOXYGEN__)
#define STM32_I2S_SPI2_MODE                 (STM32_I2S_MODE_MASTER |          \
                                             STM32_I2S_MODE_RX)
#endif

#if !defined(STM32_I2S_SPI3_MODE) || defined(__DOXYGEN__)
#define STM32_I2S_SPI3_MODE                 (STM32_I2S_MODE_MASTER |          \
                                             STM32_I2S_MODE_RX)
#endif

#if !defined(STM32_I2S_SPI1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2S_SPI1_IRQ_PRIORITY         10
#endif

#if !defined(STM32_I2S_SPI2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2S_SPI2_IRQ_PRIORITY         10
#endif

#if !defined(STM32_I2S_SPI3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2S_SPI3_IRQ_PRIORITY         10
#endif

#if !defined(STM32_I2S_SPI1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2S_SPI1_DMA_PRIORITY         1
#endif

#if !defined(STM32_I2S_SPI2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2S_SPI2_DMA_PRIORITY         1
#endif

#if !defined(STM32_I2S_SPI3_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2S_SPI3_DMA_PRIORITY         1
#endif

#if !defined(STM32_I2S_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_I2S_DMA_ERROR_HOOK(i2sp)      osalSysHalt("DMA failure")
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_I2S_USE_SPI1 && !STM32_SPI1_SUPPORTS_I2S
#error "SPI1 does not support I2S mode"
#endif

#if STM32_I2S_USE_SPI2 && !STM32_SPI2_SUPPORTS_I2S
#error "SPI2 does not support I2S mode"
#endif

#if STM32_I2S_USE_SPI3 && !STM32_SPI3_SUPPORTS_I2S
#error "SPI3 does not support I2S mode"
#endif

#if STM32_I2S_USE_SPI1 && !STM32_HAS_SPI1
#error "SPI1 not present in the selected device"
#endif

#if STM32_I2S_USE_SPI2 && !STM32_HAS_SPI2
#error "SPI2 not present in the selected device"
#endif

#if STM32_I2S_USE_SPI3 && !STM32_HAS_SPI3
#error "SPI3 not present in the selected device"
#endif

#if !STM32_I2S_USE_SPI1 && !STM32_I2S_USE_SPI2 && !STM32_I2S_USE_SPI3
#error "I2S driver activated but no SPI peripheral assigned"
#endif

#if STM32_I2S_USE_SPI1 && !STM32_I2S_RX_ENABLED(STM32_I2S_SPI1_MODE) &&      \
    !STM32_I2S_TX_ENABLED(STM32_I2S_SPI1_MODE)
#error "I2S1 neither RX nor TX enabled"
#endif

#if STM32_I2S_USE_SPI2 && !STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE) &&      \
    !STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
#error "I2S2 neither RX nor TX enabled"
#endif

#if STM32_I2S_USE_SPI3 && !STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE) &&      \
    !STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
#error "I2S3 neither RX nor TX enabled"
#endif

#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI1_MODE) &&                              \
    STM32_I2S_TX_ENABLED(STM32_I2S_SPI1_MODE)
#error "I2S1 RX and TX mode not supported in this driver implementation"
#endif

#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE) &&                              \
    STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
#error "I2S2 RX and TX mode not supported in this driver implementation"
#endif

#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE) &&                              \
    STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
#error "I2S3 RX and TX mode not supported in this driver implementation"
#endif

#if STM32_I2S_USE_SPI1 && !OSAL_IRQ_IS_VALID_PRIORITY(STM32_I2S_SPI1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI1"
#endif

#if STM32_I2S_USE_SPI2 && !OSAL_IRQ_IS_VALID_PRIORITY(STM32_I2S_SPI2_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI2"
#endif

#if STM32_I2S_USE_SPI3 && !OSAL_IRQ_IS_VALID_PRIORITY(STM32_I2S_SPI3_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI3"
#endif

#if STM32_I2S_USE_SPI1 && !STM32_DMA_IS_VALID_PRIORITY(STM32_I2S_SPI1_DMA_PRIORITY)
#error "Invalid DMA priority assigned to SPI1"
#endif

#if STM32_I2S_USE_SPI2 && !STM32_DMA_IS_VALID_PRIORITY(STM32_I2S_SPI2_DMA_PRIORITY)
#error "Invalid DMA priority assigned to SPI2"
#endif

#if STM32_I2S_USE_SPI3 && !STM32_DMA_IS_VALID_PRIORITY(STM32_I2S_SPI3_DMA_PRIORITY)
#error "Invalid DMA priority assigned to SPI3"
#endif

#if STM32_I2S_USE_SPI1 && (!defined(STM32_I2S_SPI1_RX_DMA_STREAM) ||         \
                           !defined(STM32_I2S_SPI1_TX_DMA_STREAM))
#error "SPI1 DMA streams not defined"
#endif

#if STM32_I2S_USE_SPI2 && (!defined(STM32_I2S_SPI2_RX_DMA_STREAM) ||         \
                           !defined(STM32_I2S_SPI2_TX_DMA_STREAM))
#error "SPI2 DMA streams not defined"
#endif

#if STM32_I2S_USE_SPI3 && (!defined(STM32_I2S_SPI3_RX_DMA_STREAM) ||         \
                           !defined(STM32_I2S_SPI3_TX_DMA_STREAM))
#error "SPI3 DMA streams not defined"
#endif

#if STM32_I2S_USE_SPI1 && !STM32_DMA_IS_VALID_STREAM(STM32_I2S_SPI1_RX_DMA_STREAM)
#error "invalid DMA stream associated to SPI1 RX"
#endif

#if STM32_I2S_USE_SPI1 && !STM32_DMA_IS_VALID_STREAM(STM32_I2S_SPI1_TX_DMA_STREAM)
#error "invalid DMA stream associated to SPI1 TX"
#endif

#if STM32_I2S_USE_SPI2 && !STM32_DMA_IS_VALID_STREAM(STM32_I2S_SPI2_RX_DMA_STREAM)
#error "invalid DMA stream associated to SPI2 RX"
#endif

#if STM32_I2S_USE_SPI2 && !STM32_DMA_IS_VALID_STREAM(STM32_I2S_SPI2_TX_DMA_STREAM)
#error "invalid DMA stream associated to SPI2 TX"
#endif

#if STM32_I2S_USE_SPI3 && !STM32_DMA_IS_VALID_STREAM(STM32_I2S_SPI3_RX_DMA_STREAM)
#error "invalid DMA stream associated to SPI3 RX"
#endif

#if STM32_I2S_USE_SPI3 && !STM32_DMA_IS_VALID_STREAM(STM32_I2S_SPI3_TX_DMA_STREAM)
#error "invalid DMA stream associated to SPI3 TX"
#endif

#if STM32_ADVANCED_DMA && !STM32_DMA_SUPPORTS_DMAMUX

#if STM32_I2S_USE_SPI1 &&                                                     \
    !STM32_DMA_IS_VALID_ID(STM32_I2S_SPI1_RX_DMA_STREAM, STM32_I2S1_RX_DMA_MSK)
#error "invalid DMA stream associated to SPI1 RX"
#endif

#if STM32_I2S_USE_SPI1 &&                                                     \
    !STM32_DMA_IS_VALID_ID(STM32_I2S_SPI1_TX_DMA_STREAM, STM32_I2S1_TX_DMA_MSK)
#error "invalid DMA stream associated to SPI1 TX"
#endif

#if STM32_I2S_USE_SPI2 &&                                                     \
    !STM32_DMA_IS_VALID_ID(STM32_I2S_SPI2_RX_DMA_STREAM, STM32_I2S2_RX_DMA_MSK)
#error "invalid DMA stream associated to SPI2 RX"
#endif

#if STM32_I2S_USE_SPI2 &&                                                     \
    !STM32_DMA_IS_VALID_ID(STM32_I2S_SPI2_TX_DMA_STREAM, STM32_I2S2_TX_DMA_MSK)
#error "invalid DMA stream associated to SPI2 TX"
#endif

#if STM32_I2S_USE_SPI3 &&                                                     \
    !STM32_DMA_IS_VALID_ID(STM32_I2S_SPI3_RX_DMA_STREAM, STM32_I2S3_RX_DMA_MSK)
#error "invalid DMA stream associated to SPI3 RX"
#endif

#if STM32_I2S_USE_SPI3 &&                                                     \
    !STM32_DMA_IS_VALID_ID(STM32_I2S_SPI3_TX_DMA_STREAM, STM32_I2S3_TX_DMA_MSK)
#error "invalid DMA stream associated to SPI3 TX"
#endif

#endif /* STM32_ADVANCED_DMA && !STM32_DMA_SUPPORTS_DMAMUX */

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef uint32_t i2serror_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the I2S driver structure.
 */
#define i2s_lld_driver_fields                                                 \
  SPI_TypeDef               *spi;                                             \
  uint16_t                  cfg;                                              \
  const stm32_dma_stream_t  *dmarx;                                           \
  const stm32_dma_stream_t  *dmatx;                                           \
  uint32_t                  rxdmamode;                                        \
  uint32_t                  txdmamode

/**
 * @brief   Low level fields of the I2S configuration structure.
 */
#define i2s_lld_config_fields                                                 \
  int16_t                   i2scfgr;                                          \
  int16_t                   i2spr

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_I2S_USE_SPI1 && !defined(__DOXYGEN__)
extern I2SDriver I2SD1;
#endif

#if STM32_I2S_USE_SPI2 && !defined(__DOXYGEN__)
extern I2SDriver I2SD2;
#endif

#if STM32_I2S_USE_SPI3 && !defined(__DOXYGEN__)
extern I2SDriver I2SD3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void i2s_lld_init(void);
  msg_t i2s_lld_start(I2SDriver *i2sp);
  void i2s_lld_stop(I2SDriver *i2sp);
  const I2SConfig *i2s_lld_setcfg(I2SDriver *i2sp, const I2SConfig *config);
  const I2SConfig *i2s_lld_selcfg(I2SDriver *i2sp, unsigned cfgnum);
  msg_t i2s_lld_start_exchange(I2SDriver *i2sp);
  void i2s_lld_stop_exchange(I2SDriver *i2sp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_I2S */

#endif /* HAL_I2S_LLD_H */

/** @} */
