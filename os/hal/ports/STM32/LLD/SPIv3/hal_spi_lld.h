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
 * @file    SPIv3/hal_spi_lld.h
 * @brief   STM32 SPI subsystem low level driver header.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef HAL_SPI_LLD_H
#define HAL_SPI_LLD_H

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Circular mode support flag.
 */
#define SPI_SUPPORTS_CIRCULAR           TRUE

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
 * @brief   SPI1 RX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI1_RX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI1_RX_DMA_CHANNEL       10
#endif

/**
 * @brief   SPI1 TX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI1_TX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI1_TX_DMA_CHANNEL       11
#endif

/**
 * @brief   SPI2 RX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI2_RX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI2_RX_DMA_CHANNEL       12
#endif

/**
 * @brief   SPI2 TX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI2_TX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI2_TX_DMA_CHANNEL       13
#endif

/**
 * @brief   SPI3 RX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI3_RX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI3_RX_DMA_CHANNEL       10
#endif

/**
 * @brief   SPI3 TX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI3_TX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI3_TX_DMA_CHANNEL       11
#endif

/**
 * @brief   SPI4 RX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI4_RX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI4_RX_DMA_CHANNEL       12
#endif

/**
 * @brief   SPI4 TX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI4_TX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI4_TX_DMA_CHANNEL       13
#endif

/**
 * @brief   SPI5 RX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI5_RX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI5_RX_DMA_CHANNEL       12
#endif

/**
 * @brief   SPI5 TX DMA channel setting.
 */
#if !defined(STM32_SPI_SPI5_TX_DMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI5_TX_DMA_CHANNEL       13
#endif

/**
 * @brief   SPI6 RX BDMA channel setting.
 */
#if !defined(STM32_SPI_SPI6_RX_BDMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI6_RX_BDMA_CHANNEL      2
#endif

/**
 * @brief   SPI6 TX BDMA channel setting.
 */
#if !defined(STM32_SPI_SPI6_TX_BDMA_CHANNEL) || defined(__DOXYGEN__)
#define STM32_SPI_SPI6_TX_BDMA_CHANNEL      3
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

#if STM32_SPI_USE_SPI1 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI1_RX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI1 RX"
#endif

#if STM32_SPI_USE_SPI1 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI1_TX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI1 TX"
#endif

#if STM32_SPI_USE_SPI2 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI2_RX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI2 RX"
#endif

#if STM32_SPI_USE_SPI2 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI2_TX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI2 TX"
#endif

#if STM32_SPI_USE_SPI3 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI3_RX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI3 RX"
#endif

#if STM32_SPI_USE_SPI3 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI3_TX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI3 TX"
#endif

#if STM32_SPI_USE_SPI4 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI4_RX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI4 RX"
#endif

#if STM32_SPI_USE_SPI4 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI4_TX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI4 TX"
#endif

#if STM32_SPI_USE_SPI5 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI5_RX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI5 RX"
#endif

#if STM32_SPI_USE_SPI5 &&                                                   \
    !STM32_DMA_IS_VALID_CHANNEL(STM32_SPI_SPI5_TX_DMA_CHANNEL)
#error "Invalid DMA channel assigned to SPI5 TX"
#endif

#if STM32_SPI_USE_SPI6 &&                                                   \
    !STM32_BDMA_IS_VALID_CHANNEL(STM32_SPI_SPI6_RX_BDMA_CHANNEL)
#error "Invalid BDMA channel assigned to SPI6 RX"
#endif

#if STM32_SPI_USE_SPI6 &&                                                   \
    !STM32_BDMA_IS_VALID_CHANNEL(STM32_SPI_SPI6_TX_BDMA_CHANNEL)
#error "Invalid BDMA channel assigned to SPI6 TX"
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

#if STM32_SPI_USE_SPI1 || STM32_SPI_USE_SPI2 || STM32_SPI_USE_SPI1 ||       \
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
 */
typedef struct {
#if (SPI_SUPPORTS_CIRCULAR == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Enables the circular buffer mode.
   */
  bool                      circular;
#endif
  /**
   * @brief Operation complete callback or @p NULL.
   */
  spicallback_t             end_cb;
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_LINE) || defined(__DOXYGEN__)
  /**
   * @brief The chip select line.
   */
  ioline_t                  ssline;
#endif
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_PORT) || defined(__DOXYGEN__)
  /**
   * @brief The chip select port.
   */
  ioportid_t                ssport;
  /**
   * @brief The chip select port mask.
   */
  ioportmask_t              ssmask;
#endif
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_PAD) || defined(__DOXYGEN__)
  /**
   * @brief The chip select port.
   */
  ioportid_t                ssport;
  /**
   * @brief The chip select pad number.
   */
  uint_fast8_t              sspad;
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief SPI CFG1 register initialization data.
   */
  uint32_t                  cfg1;
  /**
   * @brief SPI CFG2 register initialization data.
   */
  uint32_t                  cfg2;
} SPIConfig;

/**
 * @brief   Structure representing an SPI driver.
 */
struct SPIDriver {
  /**
   * @brief Driver state.
   */
  spistate_t                state;
  /**
   * @brief Current configuration data.
   */
  const SPIConfig           *config;
#if SPI_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread.
   */
  thread_reference_t        thread;
#endif /* SPI_USE_WAIT */
#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the peripheral.
   */
  mutex_t                   mutex;
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#if defined(SPI_DRIVER_EXT_FIELDS)
  SPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the SPIx registers block.
   */
  SPI_TypeDef               *spi;
#if defined(STM32_SPI_DMA_REQUIRED) && defined(STM32_SPI_BDMA_REQUIRED)
  /**
   * @brief   DMA type for this instance.
   */
  bool                      is_bdma;
#endif
  /**
   * @brief   Union of the RX DMA streams.
   */
  union {
#if defined(STM32_SPI_DMA_REQUIRED) || defined(__DOXYGEN__)
    /**
     * @brief   Receive DMA stream.
     */
    const stm32_dma_stream_t  *dma;
#endif
#if defined(STM32_SPI_BDMA_REQUIRED) || defined(__DOXYGEN__)
    /**
     * @brief   Receive BDMA stream.
     */
    const stm32_bdma_stream_t  *bdma;
#endif
  } rx;
  /**
   * @brief   Union of the TX DMA streams.
   */
  union {
#if defined(STM32_SPI_DMA_REQUIRED) || defined(__DOXYGEN__)
    /**
     * @brief   Transmit DMA stream.
     */
    const stm32_dma_stream_t  *dma;
#endif
#if defined(STM32_SPI_BDMA_REQUIRED) || defined(__DOXYGEN__)
    /**
     * @brief   Transmit DMA stream.
     */
    const stm32_bdma_stream_t  *bdma;
#endif
  } tx;
  /**
   * @brief   RX DMA mode bit mask.
   */
  uint32_t                  rxdmamode;
  /**
   * @brief   TX DMA mode bit mask.
   */
  uint32_t                  txdmamode;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

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
  void spi_lld_start(SPIDriver *spip);
  void spi_lld_stop(SPIDriver *spip);
#if (SPI_SELECT_MODE == SPI_SELECT_MODE_LLD) || defined(__DOXYGEN__)
  void spi_lld_select(SPIDriver *spip);
  void spi_lld_unselect(SPIDriver *spip);
#endif
  void spi_lld_ignore(SPIDriver *spip, size_t n);
  void spi_lld_exchange(SPIDriver *spip, size_t n,
                        const void *txbuf, void *rxbuf);
  void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf);
  void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf);
#if (SPI_SUPPORTS_CIRCULAR == TRUE) || defined(__DOXYGEN__)
  void spi_lld_abort(SPIDriver *spip);
#endif
  uint32_t spi_lld_polled_exchange(SPIDriver *spip, uint32_t frame);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SPI */

#endif /* HAL_SPI_LLD_H */

/** @} */
