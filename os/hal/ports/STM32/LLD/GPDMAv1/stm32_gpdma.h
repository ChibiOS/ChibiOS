/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

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
 * @file    GPDMAv1/stm32_gpdma.h
 * @brief   GPDMA helper driver header.
 *
 * @addtogroup STM32_GPDMA
 * @{
 */

#ifndef STM32_GPDMA_H
#define STM32_GPDMA_H

#if defined(STM32_GPDMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Maximum number of transfers in a single operation.
 */
#define STM32_GPDMA_MAX_TRANSFER      65535

/**
 * @brief   Checks if a GPDMA priority is within the valid range.
 * @param[in] prio      GPDMA priority
 *
 * @retval              The check result.
 * @retval false        invalid GPDMA priority.
 * @retval true         correct GPDMA priority.
 */
#define STM32_GPDMA_IS_VALID_PRIORITY(prio)                                 \
  (((prio) >= 0U) && ((prio) <= 3U))

/**
 * @brief   Returns an unique numeric identifier for a GPDMA channel.
 *
 * @param[in] dma       the GPDMA unit number, 1 or 2.
 * @param[in] ch        the channel number
 * @return              An unique numeric channel identifier.
 */
#define STM32_GPDMA_CHANNEL_ID(dma, ch)                                     \
  ((((dma) - 1U) * STM32_GPDMA1_NUM_CHANNELS) + (ch))

/**
 * @brief   Returns a GPDMA channel identifier mask.
 *
 *
 * @param[in] dma       the GPDMA unit number, 1 or 2.
 * @param[in] ch        the channel number
 * @return              A channel mask.
 */
#define STM32_GPDMA_CHANNEL_ID_MSK(dma, ch)                                 \
  (1U << STM32_GPDMA_CHANNEL_ID(dma, ch))

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_GPDMA1_NUM_CHANNELS)
#error "STM32_GPDMA1_NUM_CHANNELS not defined in registry"
#endif

#if !defined(STM32_GPDMA2_NUM_CHANNELS)
#error "STM32_GPDMA2_NUM_CHANNELS not defined in registry"
#endif

#if (STM32_GPDMA1_NUM_CHANNELS < 0) || (STM32_GPDMA1_NUM_CHANNELS > 8)
#error "unsupported channels configuration"
#endif

#if (STM32_GPDMA2_NUM_CHANNELS < 0) || (STM32_GPDMA2_NUM_CHANNELS > 8)
#error "unsupported channels configuration"
#endif

/**
 * @brief   Total number of channels among all GPDMAs.
 */
#define STM32_GPDMA_CHANNELS                                                \
  (STM32_GPDMA1_NUM_CHANNELS + STM32_GPDMA2_NUM_CHANNELS)

#if STM32_GPDMA1_NUM_CHANNELS > 0
#if !defined(STM32_GPDMA1_MASK_FIFO2)
#error "STM32_GPDMA1_MASK_FIFO2 not defined in registry"
#endif
#if !defined(STM32_GPDMA1_MASK_FIFO4)
#error "STM32_GPDMA1_MASK_FIFO4 not defined in registry"
#endif
#if !defined(STM32_GPDMA1_MASK_FIFO4_2D)
#error "STM32_GPDMA1_MASK_FIFO4_2D not defined in registry"
#endif
#endif

#if STM32_GPDMA2_NUM_CHANNELS > 0
#if !defined(STM32_GPDMA2_MASK_FIFO2)
#error "STM32_GPDMA2_MASK_FIFO2 not defined in registry"
#endif
#if !defined(STM32_GPDMA2_MASK_FIFO4)
#error "STM32_GPDMA2_MASK_FIFO4 not defined in registry"
#endif
#if !defined(STM32_GPDMA2_MASK_FIFO4_2D)
#error "STM32_GPDMA2_MASK_FIFO4_2D not defined in registry"
#endif
#endif

/**
 * @brief   Any non-2D channel on GPDMA1.
 */
#define STM32_GPDMA1_MASK_FIFOX                                             \
  (STM32_GPDMA1_MASK_FIFO2 | STM32_GPDMA1_MASK_FIFO4)

/**
 * @brief   Any non-2D channel on GPDMA2.
 */
#define STM32_GPDMA2_MASK_FIFOX                                             \
  (STM32_GPDMA2_MASK_FIFO2 | STM32_GPDMA2_MASK_FIFO4)

/**
 * @brief   Any non-2D channel on any GPDMA.
 */
#define STM32_GPDMA_MASK_FIFOX                                              \
  (STM32_GPDMA1_MASK_FIFOX | STM32_GPDMA2_MASK_FIFOX)

/**
 * @brief   Any channel on GPDMA1.
 */
#define STM32_GPDMA1_MASK_ANY                                               \
  (STM32_GPDMA1_MASK_FIFO2 | STM32_GPDMA1_MASK_FIFO4 | STM32_GPDMA1_MASK_FIFO4_2D)

/**
 * @brief   Any channel on GPDMA2.
 */
#define STM32_GPDMA2_MASK_ANY                                               \
  (STM32_GPDMA2_MASK_FIFO2 | STM32_GPDMA2_MASK_FIFO4 | STM32_GPDMA2_MASK_FIFO4_2D)

/**
 * @brief   Any channel on any GPDMA.
 */
#define STM32_GPDMA_MASK_ANY                                                \
  (STM32_GPDM1_MASK_ANY | STM32_GPDMA2_MASK_ANY)

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a GPDMA callback.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] v       content of the CxSR register
 */
typedef void (*stm32_gpdmaisr_t)(void *p, uint32_t csr);

/**
 * @brief   STM32 DMA channel descriptor structure.
 */
typedef struct {
  DMA_Channel_TypeDef   *channel;       /**< @brief Associated channel.     */
  uint8_t               vector;         /**< @brief Associated IRQ vector.  */
} stm32_gpdma_channel_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Associates a peripheral data register to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 * @param[in] addr      value to be written in the CPAR register
 *
 * @special
 */
#define dmaStreamSetPeripheral(dmastp, addr) {                              \
  (dmastp)->channel->CPAR = (uint32_t)(addr);                               \
}

/**
 * @brief   Associates a memory destination to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 * @param[in] addr      value to be written in the CMAR register
 *
 * @special
 */
#define dmaStreamSetMemory0(dmastp, addr) {                                 \
  (dmastp)->channel->CMAR = (uint32_t)(addr);                               \
}

/**
 * @brief   Sets the number of transfers to be performed.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 * @param[in] size      value to be written in the CNDTR register
 *
 * @special
 */
#define dmaStreamSetTransactionSize(dmastp, size) {                         \
  (dmastp)->channel->CNDTR = (uint32_t)(size);                              \
}

/**
 * @brief   Returns the number of transfers to be performed.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 * @return              The number of transfers to be performed.
 *
 * @special
 */
#define dmaStreamGetTransactionSize(dmastp) ((size_t)((dmastp)->channel->CNDTR))

/**
 * @brief   Programs the channel mode settings.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 * @param[in] mode      value to be written in the CCR register
 *
 * @special
 */
#define dmaStreamSetMode(dmastp, mode) {                                    \
  (dmastp)->channel->CCR  = (uint32_t)(mode);                               \
}

/**
 * @brief   DMA channel enable.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 *
 * @special
 */
#define dmaStreamEnable(dmastp) {                                           \
  (dmastp)->channel->CCR |= STM32_GPDMA_CR_EN;                              \
}

/**
 * @brief   DMA channel disable.
 * @details The function disables the specified channel and then clears any
 *          pending interrupt.
 * @note    This function can be invoked in both ISR or thread context.
 * @note    Interrupts enabling flags are set to zero after this call, see
 *          bug 3607518.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 *
 * @special
 */
#define dmaStreamDisable(dmastp) {                                          \
  (dmastp)->channel->CCR &= ~(STM32_GPDMA_CR_TCIE | STM32_GPDMA_CR_HTIE |   \
                              STM32_GPDMA_CR_TEIE | STM32_GPDMA_CR_EN);     \
  dmaStreamClearInterrupt(dmastp);                                          \
}

/**
 * @brief   DMA channel interrupt sources clear.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 *
 * @special
 */
#define dmaStreamClearInterrupt(dmastp) {                                   \
  (dmastp)->dma->IFCR = STM32_GPDMA_ISR_MASK << (dmastp)->shift;            \
}

/**
 * @brief   Starts a memory to memory operation using the specified channel.
 * @note    The default transfer data mode is "byte to byte" but it can be
 *          changed by specifying extra options in the @p mode parameter.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 * @param[in] mode      value to be written in the CCR register, this value
 *                      is implicitly ORed with:
 *                      - @p STM32_GPDMA_CR_MINC
 *                      - @p STM32_GPDMA_CR_PINC
 *                      - @p STM32_GPDMA_CR_DIR_M2M
 *                      - @p STM32_GPDMA_CR_EN
 *                      .
 * @param[in] src       source address
 * @param[in] dst       destination address
 * @param[in] n         number of data units to copy
 */
#define dmaStartMemCopy(dmastp, mode, src, dst, n) {                        \
  dmaStreamSetPeripheral(dmastp, src);                                      \
  dmaStreamSetMemory0(dmastp, dst);                                         \
  dmaStreamSetTransactionSize(dmastp, n);                                   \
  dmaStreamSetMode(dmastp, (mode) |                                         \
                           STM32_GPDMA_CR_MINC | STM32_GPDMA_CR_PINC |      \
                           STM32_GPDMA_CR_DIR_M2M | STM32_GPDMA_CR_EN);     \
}

/**
 * @brief   Polled wait for DMA transfer end.
 * @pre     The channel must have been allocated using @p dmaStreamAlloc().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmastp    pointer to a stm32_gpdma_channel_t structure
 */
#define dmaWaitCompletion(dmastp) {                                         \
  while ((dmastp)->channel->CNDTR > 0U)                                     \
    ;                                                                       \
  dmaStreamDisable(dmastp);                                                 \
}
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern const stm32_gpdma_channel_t _stm32_gpdma_channels[STM32_GPDMA_CHANNELS];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
  const stm32_gpdma_channel_t *gpdmaChannelAllocI(uint32_t cmask,
                                                  uint32_t irqprio,
                                                  stm32_gpdmaisr_t func,
                                                  void *param);
  const stm32_gpdma_channel_t *gpdmaChannelAlloc(uint32_t cmask,
                                                 uint32_t irqprio,
                                                 stm32_gpdmaisr_t func,
                                                 void *param);
  void gpdmaChannelFreeI(const stm32_gpdma_channel_t *dmachp);
  void gpdmaChannelFree(const stm32_gpdma_channel_t *dmachp);
  void gpdmaServeInterrupt(const stm32_gpdma_channel_t *dmachp);
#ifdef __cplusplus
}
#endif

#endif /* defined(STM32_GPDMA_REQUIRED) */

#endif /* STM32_GPDMA_H */

/** @} */
