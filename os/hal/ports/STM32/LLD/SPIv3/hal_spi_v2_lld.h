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
 * @file    SPIv3/hal_spi_v2_lld.h
 * @brief   STM32 SPI (v2) subsystem low level driver header.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef HAL_SPI_V2_LLD_H
#define HAL_SPI_V2_LLD_H

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Circular mode support flag.
 */
#define SPI_SUPPORTS_CIRCULAR           TRUE

/**
 * @brief   Slave mode support flag.
 */
#define SPI_SUPPORTS_SLAVE_MODE         TRUE

/**
 * @name    Register helpers not found in ST headers
 * @{
 */
#define SPI_CFG1_MBR_VALUE(n)           ((n) << SPI_CFG1_MBR_Pos)
#define SPI_CFG1_MBR_DIV2               SPI_CFG1_MBR_VALUE(0)
#define SPI_CFG1_MBR_DIV4               SPI_CFG1_MBR_VALUE(1)
#define SPI_CFG1_MBR_DIV8               SPI_CFG1_MBR_VALUE(2)
#define SPI_CFG1_MBR_DIV16              SPI_CFG1_MBR_VALUE(3)
#define SPI_CFG1_MBR_DIV32              SPI_CFG1_MBR_VALUE(4)
#define SPI_CFG1_MBR_DIV64              SPI_CFG1_MBR_VALUE(5)
#define SPI_CFG1_MBR_DIV128             SPI_CFG1_MBR_VALUE(6)
#define SPI_CFG1_MBR_DIV256             SPI_CFG1_MBR_VALUE(7)
#define SPI_CFG1_CRCSIZE_VALUE(n)       ((n) << SPI_CFG1_CRCSIZE_Pos)
#define SPI_CFG1_UDRDET_VALUE(n)        ((n) << SPI_CFG1_UDRDET_Pos)
#define SPI_CFG1_UDRCFG_VALUE(n)        ((n) << SPI_CFG1_UDRCFG_Pos)
#define SPI_CFG1_FTHLV_VALUE(n)         ((n) << SPI_CFG1_FTHLV_Pos)
#define SPI_CFG1_DSIZE_VALUE(n)         ((n) << SPI_CFG1_DSIZE_Pos)
#define SPI_CFG1_DSIZE_4BITS            SPI_CFG1_DSIZE_VALUE(3U)
#define SPI_CFG1_DSIZE_5BITS            SPI_CFG1_DSIZE_VALUE(4U)
#define SPI_CFG1_DSIZE_6BITS            SPI_CFG1_DSIZE_VALUE(5U)
#define SPI_CFG1_DSIZE_7BITS            SPI_CFG1_DSIZE_VALUE(6U)
#define SPI_CFG1_DSIZE_8BITS            SPI_CFG1_DSIZE_VALUE(7U)
#define SPI_CFG1_DSIZE_9BITS            SPI_CFG1_DSIZE_VALUE(8U)
#define SPI_CFG1_DSIZE_10BITS           SPI_CFG1_DSIZE_VALUE(9U)
#define SPI_CFG1_DSIZE_11BITS           SPI_CFG1_DSIZE_VALUE(10U)
#define SPI_CFG1_DSIZE_12BITS           SPI_CFG1_DSIZE_VALUE(11U)
#define SPI_CFG1_DSIZE_13BITS           SPI_CFG1_DSIZE_VALUE(12U)
#define SPI_CFG1_DSIZE_14BITS           SPI_CFG1_DSIZE_VALUE(13U)
#define SPI_CFG1_DSIZE_15BITS           SPI_CFG1_DSIZE_VALUE(14U)
#define SPI_CFG1_DSIZE_16BITS           SPI_CFG1_DSIZE_VALUE(15U)
#define SPI_CFG1_DSIZE_17BITS           SPI_CFG1_DSIZE_VALUE(16U)
#define SPI_CFG1_DSIZE_18BITS           SPI_CFG1_DSIZE_VALUE(17U)
#define SPI_CFG1_DSIZE_19BITS           SPI_CFG1_DSIZE_VALUE(18U)
#define SPI_CFG1_DSIZE_20BITS           SPI_CFG1_DSIZE_VALUE(19U)
#define SPI_CFG1_DSIZE_21BITS           SPI_CFG1_DSIZE_VALUE(20U)
#define SPI_CFG1_DSIZE_22BITS           SPI_CFG1_DSIZE_VALUE(21U)
#define SPI_CFG1_DSIZE_23BITS           SPI_CFG1_DSIZE_VALUE(22U)
#define SPI_CFG1_DSIZE_24BITS           SPI_CFG1_DSIZE_VALUE(23U)
#define SPI_CFG1_DSIZE_25BITS           SPI_CFG1_DSIZE_VALUE(24U)
#define SPI_CFG1_DSIZE_26BITS           SPI_CFG1_DSIZE_VALUE(25U)
#define SPI_CFG1_DSIZE_27BITS           SPI_CFG1_DSIZE_VALUE(26U)
#define SPI_CFG1_DSIZE_28BITS           SPI_CFG1_DSIZE_VALUE(27U)
#define SPI_CFG1_DSIZE_29BITS           SPI_CFG1_DSIZE_VALUE(28U)
#define SPI_CFG1_DSIZE_30BITS           SPI_CFG1_DSIZE_VALUE(29U)
#define SPI_CFG1_DSIZE_31BITS           SPI_CFG1_DSIZE_VALUE(30U)
#define SPI_CFG1_DSIZE_32BITS           SPI_CFG1_DSIZE_VALUE(31U)

#define SPI_CFG2_SP_VALUE(n)            ((n) << SPI_CFG2_SP_Pos)
#define SPI_CFG2_COMM_VALUE(n)          ((n) << SPI_CFG2_COMM_Pos)
#define SPI_CFG2_COMM_FULL_DUPLEX       SPI_CFG2_COMM_VALUE(0)
#define SPI_CFG2_COMM_TRANSMITTER       SPI_CFG2_COMM_VALUE(1)
#define SPI_CFG2_COMM_RECEIVER          SPI_CFG2_COMM_VALUE(2)
#define SPI_CFG2_COMM_HALF_DUPLEX       SPI_CFG2_COMM_VALUE(3)
#define SPI_CFG2_MIDI_VALUE(n)          ((n) << SPI_CFG2_MIDI_Pos)
#define SPI_CFG2_MSSI_VALUE(n)          ((n) << SPI_CFG2_MSSI_Pos)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SPI1 driver enable switch.
 * @details If set to @p TRUE the support for SPI1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SPI_USE_SPI1) || defined(__DOXYGEN__)
#define STM32_SPI_USE_SPI1                  FALSE
#endif

/**
 * @brief   SPI2 driver enable switch.
 * @details If set to @p TRUE the support for SPI2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SPI_USE_SPI2) || defined(__DOXYGEN__)
#define STM32_SPI_USE_SPI2                  FALSE
#endif

/**
 * @brief   SPI3 driver enable switch.
 * @details If set to @p TRUE the support for SPI3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SPI_USE_SPI3) || defined(__DOXYGEN__)
#define STM32_SPI_USE_SPI3                  FALSE
#endif

/**
 * @brief   SPI4 driver enable switch.
 * @details If set to @p TRUE the support for SPI4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SPI_USE_SPI4) || defined(__DOXYGEN__)
#define STM32_SPI_USE_SPI4                  FALSE
#endif

/**
 * @brief   SPI5 driver enable switch.
 * @details If set to @p TRUE the support for SPI5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SPI_USE_SPI5) || defined(__DOXYGEN__)
#define STM32_SPI_USE_SPI5                  FALSE
#endif

/**
 * @brief   SPI6 driver enable switch.
 * @details If set to @p TRUE the support for SPI6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_SPI_USE_SPI6) || defined(__DOXYGEN__)
#define STM32_SPI_USE_SPI6                  FALSE
#endif

/**
 * @brief   Filler pattern used when there is nothing to transmit.
 */
#if !defined(STM32_SPI_FILLER_PATTERN) || defined(__DOXYGEN__)
#define STM32_SPI_FILLER_PATTERN            0xFFFFFFFFU
#endif

/**
 * @brief   SPI1 interrupt priority level setting.
 */
#if !defined(STM32_SPI_SPI1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI1_IRQ_PRIORITY         10
#endif

/**
 * @brief   SPI2 interrupt priority level setting.
 */
#if !defined(STM32_SPI_SPI2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI2_IRQ_PRIORITY         10
#endif

/**
 * @brief   SPI3 interrupt priority level setting.
 */
#if !defined(STM32_SPI_SPI3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI3_IRQ_PRIORITY         10
#endif

/**
 * @brief   SPI4 interrupt priority level setting.
 */
#if !defined(STM32_SPI_SPI4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI4_IRQ_PRIORITY         10
#endif

/**
 * @brief   SPI5 interrupt priority level setting.
 */
#if !defined(STM32_SPI_SPI5_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI5_IRQ_PRIORITY         10
#endif

/**
 * @brief   SPI6 interrupt priority level setting.
 */
#if !defined(STM32_SPI_SPI6_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI6_IRQ_PRIORITY         10
#endif

/**
 * @brief   SPI1 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA streams but
 *          because of the streams ordering the RX stream has always priority
 *          over the TX stream.
 */
#if !defined(STM32_SPI_SPI1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI1_DMA_PRIORITY         1
#endif

/**
 * @brief   SPI2 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA streams but
 *          because of the streams ordering the RX stream has always priority
 *          over the TX stream.
 */
#if !defined(STM32_SPI_SPI2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI2_DMA_PRIORITY         1
#endif

/**
 * @brief   SPI3 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA streams but
 *          because of the streams ordering the RX stream has always priority
 *          over the TX stream.
 */
#if !defined(STM32_SPI_SPI3_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI3_DMA_PRIORITY         1
#endif

/**
 * @brief   SPI4 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA streams but
 *          because of the streams ordering the RX stream has always priority
 *          over the TX stream.
 */
#if !defined(STM32_SPI_SPI4_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI4_DMA_PRIORITY         1
#endif

/**
 * @brief   SPI5 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA streams but
 *          because of the streams ordering the RX stream has always priority
 *          over the TX stream.
 */
#if !defined(STM32_SPI_SPI5_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI5_DMA_PRIORITY         1
#endif

/**
 * @brief   SPI6 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA streams but
 *          because of the streams ordering the RX stream has always priority
 *          over the TX stream.
 */
#if !defined(STM32_SPI_SPI6_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_SPI_SPI6_DMA_PRIORITY         1
#endif

/**
 * @brief   SPI DMA error hook.
 */
#if !defined(STM32_SPI_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_SPI_DMA_ERROR_HOOK(spip)      osalSysHalt("DMA failure")
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_SPI_USE_SPI1 && !STM32_HAS_SPI1
#error "SPI1 not present in the selected device"
#endif

#if STM32_SPI_USE_SPI2 && !STM32_HAS_SPI2
#error "SPI2 not present in the selected device"
#endif

#if STM32_SPI_USE_SPI3 && !STM32_HAS_SPI3
#error "SPI3 not present in the selected device"
#endif

#if STM32_SPI_USE_SPI4 && !STM32_HAS_SPI4
#error "SPI4 not present in the selected device"
#endif

#if STM32_SPI_USE_SPI5 && !STM32_HAS_SPI5
#error "SPI5 not present in the selected device"
#endif

#if STM32_SPI_USE_SPI6 && !STM32_HAS_SPI6
#error "SPI6 not present in the selected device"
#endif

#if !STM32_SPI_USE_SPI1 && !STM32_SPI_USE_SPI2 && !STM32_SPI_USE_SPI3 &&    \
    !STM32_SPI_USE_SPI4 && !STM32_SPI_USE_SPI5 && !STM32_SPI_USE_SPI6
#error "SPI driver activated but no SPI peripheral assigned"
#endif

#if STM32_SPI_USE_SPI1 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_SPI_SPI1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI1"
#endif

#if STM32_SPI_USE_SPI2 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_SPI_SPI2_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI2"
#endif

#if STM32_SPI_USE_SPI3 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_SPI_SPI3_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI3"
#endif

#if STM32_SPI_USE_SPI4 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_SPI_SPI4_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI4"
#endif

#if STM32_SPI_USE_SPI5 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_SPI_SPI5_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI5"
#endif

#if STM32_SPI_USE_SPI6 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(STM32_SPI_SPI6_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI6"
#endif

/* Check on the presence of the DMA streams settings in mcuconf.h.*/
#if STM32_SPI_USE_SPI1 && (!defined(STM32_SPI_SPI1_RX_DMA_STREAM) ||        \
                           !defined(STM32_SPI_SPI1_TX_DMA_STREAM))
#error "SPI1 DMA streams not defined"
#endif

#if STM32_SPI_USE_SPI2 && (!defined(STM32_SPI_SPI2_RX_DMA_STREAM) ||        \
                           !defined(STM32_SPI_SPI2_TX_DMA_STREAM))
#error "SPI2 DMA streams not defined"
#endif

#if STM32_SPI_USE_SPI3 && (!defined(STM32_SPI_SPI3_RX_DMA_STREAM) ||        \
                           !defined(STM32_SPI_SPI3_TX_DMA_STREAM))
#error "SPI3 DMA streams not defined"
#endif

#if STM32_SPI_USE_SPI4 && (!defined(STM32_SPI_SPI4_RX_DMA_STREAM) ||        \
                           !defined(STM32_SPI_SPI4_TX_DMA_STREAM))
#error "SPI4 DMA streams not defined"
#endif

#if STM32_SPI_USE_SPI5 && (!defined(STM32_SPI_SPI5_RX_DMA_STREAM) ||        \
                           !defined(STM32_SPI_SPI5_TX_DMA_STREAM))
#error "SPI5 DMA streams not defined"
#endif

#if STM32_SPI_USE_SPI6 && (!defined(STM32_SPI_SPI6_RX_BDMA_STREAM) ||       \
                           !defined(STM32_SPI_SPI6_TX_BDMA_STREAM))
#error "SPI6 BDMA streams not defined"
#endif

/* Check on the validity of the assigned DMA streams.*/
#if STM32_SPI_USE_SPI1 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI1_RX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI1 RX"
#endif

#if STM32_SPI_USE_SPI1 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI1_TX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI1 TX"
#endif

#if STM32_SPI_USE_SPI2 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI2_RX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI2 RX"
#endif

#if STM32_SPI_USE_SPI2 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI2_TX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI2 TX"
#endif

#if STM32_SPI_USE_SPI3 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI3_RX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI3 RX"
#endif

#if STM32_SPI_USE_SPI3 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI3_TX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI3 TX"
#endif

#if STM32_SPI_USE_SPI4 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI4_RX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI4 RX"
#endif

#if STM32_SPI_USE_SPI4 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI4_TX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI4 TX"
#endif

#if STM32_SPI_USE_SPI5 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI5_RX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI5 RX"
#endif

#if STM32_SPI_USE_SPI5 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_SPI_SPI5_TX_DMA_STREAM)
#error "Invalid DMA stream assigned to SPI5 TX"
#endif

#if STM32_SPI_USE_SPI6 &&                                                   \
    !STM32_BDMA_IS_VALID_STREAM(STM32_SPI_SPI6_RX_BDMA_STREAM)
#error "Invalid BDMA stream assigned to SPI6 RX"
#endif

#if STM32_SPI_USE_SPI6 &&                                                   \
    !STM32_BDMA_IS_VALID_STREAM(STM32_SPI_SPI6_TX_BDMA_STREAM)
#error "Invalid BDMA stream assigned to SPI6 TX"
#endif

#if STM32_SPI_USE_SPI1 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_SPI_SPI1_DMA_PRIORITY)
#error "Invalid DMA priority assigned to SPI1"
#endif

#if STM32_SPI_USE_SPI2 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_SPI_SPI2_DMA_PRIORITY)
#error "Invalid DMA priority assigned to SPI2"
#endif

#if STM32_SPI_USE_SPI3 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_SPI_SPI3_DMA_PRIORITY)
#error "Invalid DMA priority assigned to SPI3"
#endif

#if STM32_SPI_USE_SPI4 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_SPI_SPI4_DMA_PRIORITY)
#error "Invalid DMA priority assigned to SPI4"
#endif

#if STM32_SPI_USE_SPI5 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_SPI_SPI5_DMA_PRIORITY)
#error "Invalid DMA priority assigned to SPI5"
#endif

#if STM32_SPI_USE_SPI6 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_SPI_SPI6_DMA_PRIORITY)
#error "Invalid DMA priority assigned to SPI6"
#endif

#if STM32_SPI_USE_SPI1 || STM32_SPI_USE_SPI2 || STM32_SPI_USE_SPI3 ||       \
    STM32_SPI_USE_SPI4 || STM32_SPI_USE_SPI5
#define STM32_SPI_DMA_REQUIRED
#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif
#endif

#if STM32_SPI_USE_SPI6
#define STM32_SPI_BDMA_REQUIRED
#if !defined(STM32_BDMA_REQUIRED)
#define STM32_BDMA_REQUIRED
#endif
#endif

#if SPI_SELECT_MODE == SPI_SELECT_MODE_LLD
#error "SPI_SELECT_MODE_LLD not supported by this driver"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#if (defined(STM32_SPI_DMA_REQUIRED) &&                                     \
     defined(STM32_SPI_BDMA_REQUIRED)) || defined(__DOXYGEN__)
#define spi_lld_driver_fields                                               \
  /* Pointer to the SPIx registers block.*/                                 \
  SPI_TypeDef               *spi;                                           \
  /** DMA type for this instance.*/                                         \
  bool                      is_bdma;                                        \
  /** Union of the RX DMA streams.*/                                        \
  union {                                                                   \
    /* Receive DMA stream.*/                                                \
    const stm32_dma_stream_t  *dma;                                         \
    /* Receive BDMA stream.*/                                               \
    const stm32_bdma_stream_t *bdma;                                        \
  } rx;                                                                     \
  /* Union of the TX DMA streams.*/                                         \
  union {                                                                   \
    /* Transmit DMA stream.*/                                               \
    const stm32_dma_stream_t  *dma;                                         \
    /* Transmit DMA stream.*/                                               \
    const stm32_bdma_stream_t *bdma;                                        \
  } tx;                                                                     \
  /* RX DMA mode bit mask.*/                                                \
  uint32_t                  rxdmamode;                                      \
  /* TX DMA mode bit mask.*/                                                \
  uint32_t                  txdmamode;                                      \
  /* Sink for discarded data.*/                                             \
  uint32_t                  rxsink;                                         \
  /* Source for default TX pattern.*/                                       \
  uint32_t                  txsource
#endif

#if defined(STM32_SPI_DMA_REQUIRED) && !defined(STM32_SPI_BDMA_REQUIRED)
#define spi_lld_driver_fields                                               \
  /* Pointer to the SPIx registers block.*/                                 \
  SPI_TypeDef               *spi;                                           \
  /** Union of the RX DMA streams.*/                                        \
  union {                                                                   \
    /* Receive DMA stream.*/                                                \
    const stm32_dma_stream_t  *dma;                                         \
  } rx;                                                                     \
  /* Union of the TX DMA streams.*/                                         \
  union {                                                                   \
    /* Transmit DMA stream.*/                                               \
    const stm32_dma_stream_t  *dma;                                         \
  } tx;                                                                     \
  /* RX DMA mode bit mask.*/                                                \
  uint32_t                  rxdmamode;                                      \
  /* TX DMA mode bit mask.*/                                                \
  uint32_t                  txdmamode;                                      \
  /* Sink for discarded data.*/                                             \
  uint32_t                  rxsink;                                         \
  /* Source for default TX pattern.*/                                       \
  uint32_t                  txsource
#endif

#if !defined(STM32_SPI_DMA_REQUIRED) && defined(STM32_SPI_BDMA_REQUIRED)
#define spi_lld_driver_fields                                               \
  /* Pointer to the SPIx registers block.*/                                 \
  SPI_TypeDef               *spi;                                           \
  /** Union of the RX DMA streams.*/                                        \
  union {                                                                   \
    /* Receive BDMA stream.*/                                               \
    const stm32_bdma_stream_t  *bdma;                                       \
  } rx;                                                                     \
  /* Union of the TX DMA streams.*/                                         \
  union {                                                                   \
    /* Transmit DMA stream.*/                                               \
    const stm32_bdma_stream_t  *bdma;                                       \
  } tx;                                                                     \
  /* RX DMA mode bit mask.*/                                                \
  uint32_t                  rxdmamode;                                      \
  /* TX DMA mode bit mask.*/                                                \
  uint32_t                  txdmamode;                                      \
  /* Sink for discarded data.*/                                             \
  uint32_t                  rxsink;                                         \
  /* Source for default TX pattern.*/                                       \
  uint32_t                  txsource
#endif

/**
 * @brief   Low level fields of the SPI configuration structure.
 */
#define spi_lld_config_fields                                               \
  /* SPI CFG1 register initialization data.*/                               \
  uint32_t                  cfg1;                                           \
  /* SPI CFG2 register initialization data.*/                               \
  uint32_t                  cfg2

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_SPI_USE_SPI1 && !defined(__DOXYGEN__)
extern SPIDriver SPID1;
#endif

#if STM32_SPI_USE_SPI2 && !defined(__DOXYGEN__)
extern SPIDriver SPID2;
#endif

#if STM32_SPI_USE_SPI3 && !defined(__DOXYGEN__)
extern SPIDriver SPID3;
#endif

#if STM32_SPI_USE_SPI4 && !defined(__DOXYGEN__)
extern SPIDriver SPID4;
#endif

#if STM32_SPI_USE_SPI5 && !defined(__DOXYGEN__)
extern SPIDriver SPID5;
#endif

#if STM32_SPI_USE_SPI6 && !defined(__DOXYGEN__)
extern SPIDriver SPID6;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void spi_lld_init(void);
  msg_t spi_lld_start(SPIDriver *spip);
  void spi_lld_stop(SPIDriver *spip);
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_LLD) || defined(__DOXYGEN__)
  void spi_lld_select(SPIDriver *spip);
  void spi_lld_unselect(SPIDriver *spip);
#endif
  msg_t spi_lld_ignore(SPIDriver *spip, size_t n);
  msg_t spi_lld_exchange(SPIDriver *spip, size_t n,
                         const void *txbuf, void *rxbuf);
  msg_t spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf);
  msg_t spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf);
  msg_t spi_lld_stop_transfer(SPIDriver *spip, size_t *sizep);
  uint32_t spi_lld_polled_exchange(SPIDriver *spip, uint32_t frame);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SPI */

#endif /* HAL_SPI_V2_LLD_H */

/** @} */
