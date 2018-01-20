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
 * @file    DMAv3/stm32_dma.h
 * @brief   Enhanced-DMA helper driver header.
 *
 * @addtogroup STM32_DMA
 * @{
 */

#ifndef STM32_DMA_H
#define STM32_DMA_H

#include "stm32_dmamux.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Total number of DMA streams.
 * @details This is the total number of streams among all the DMA units.
 */
#define STM32_DMA_STREAMS           16U

/**
 * @brief   Mask of the ISR bits passed to the DMA callback functions.
 */
#define STM32_DMA_ISR_MASK          0x3DU

/**
 * @brief   Checks if a DMA priority is within the valid range.
 *
 * @param[in] prio      DMA priority
 * @retval              The check result.
 * @retval FALSE        invalid DMA priority.
 * @retval TRUE         correct DMA priority.
 */
#define STM32_DMA_IS_VALID_PRIORITY(prio) (((prio) >= 0U) && ((prio) <= 3U))

/**
 * @brief   Checks if a DMA channel is within the valid range.
 *
 * @param[in] ch        DMA channel
 * @retval              The check result.
 * @retval FALSE        invalid DMA channel.
 * @retval TRUE         correct DMA channel.
 */
#define STM32_DMA_IS_VALID_CHANNEL(ch) (((ch) >= 0U) && ((ch) <= 15U))

/**
 * @name    DMA streams identifiers
 * @{
 */
/**
 * @brief   Returns a pointer to a @p stm32_dma_stream_t structure.
 *
 * @param[in] id        the stream numeric identifier
 * @return              A pointer to the stm32_dma_stream_t constant structure
 *                      associated to the DMA stream.
 */
#define STM32_DMA_STREAM(id)        (&_stm32_dma_streams[id])

#define STM32_DMA1_STREAM0          STM32_DMA_STREAM(0)
#define STM32_DMA1_STREAM1          STM32_DMA_STREAM(1)
#define STM32_DMA1_STREAM2          STM32_DMA_STREAM(2)
#define STM32_DMA1_STREAM3          STM32_DMA_STREAM(3)
#define STM32_DMA1_STREAM4          STM32_DMA_STREAM(4)
#define STM32_DMA1_STREAM5          STM32_DMA_STREAM(5)
#define STM32_DMA1_STREAM6          STM32_DMA_STREAM(6)
#define STM32_DMA1_STREAM7          STM32_DMA_STREAM(7)
#define STM32_DMA2_STREAM0          STM32_DMA_STREAM(8)
#define STM32_DMA2_STREAM1          STM32_DMA_STREAM(9)
#define STM32_DMA2_STREAM2          STM32_DMA_STREAM(10)
#define STM32_DMA2_STREAM3          STM32_DMA_STREAM(11)
#define STM32_DMA2_STREAM4          STM32_DMA_STREAM(12)
#define STM32_DMA2_STREAM5          STM32_DMA_STREAM(13)
#define STM32_DMA2_STREAM6          STM32_DMA_STREAM(14)
#define STM32_DMA2_STREAM7          STM32_DMA_STREAM(15)
/** @} */

/**
 * @name    CR register constants
 * @{
 */
#define STM32_DMA_CR_RESET_VALUE    0x00000000U
#define STM32_DMA_CR_EN             DMA_SxCR_EN
#define STM32_DMA_CR_DMEIE          DMA_SxCR_DMEIE
#define STM32_DMA_CR_TEIE           DMA_SxCR_TEIE
#define STM32_DMA_CR_HTIE           DMA_SxCR_HTIE
#define STM32_DMA_CR_TCIE           DMA_SxCR_TCIE
#define STM32_DMA_CR_PFCTRL         DMA_SxCR_PFCTRL
#define STM32_DMA_CR_DIR_MASK       DMA_SxCR_DIR_Msk
#define STM32_DMA_CR_DIR_P2M        0
#define STM32_DMA_CR_DIR_M2P        DMA_SxCR_DIR_0
#define STM32_DMA_CR_DIR_M2M        DMA_SxCR_DIR_1
#define STM32_DMA_CR_CIRC           DMA_SxCR_CIRC
#define STM32_DMA_CR_PINC           DMA_SxCR_PINC
#define STM32_DMA_CR_MINC           DMA_SxCR_MINC
#define STM32_DMA_CR_PSIZE_MASK     DMA_SxCR_PSIZE_Msk
#define STM32_DMA_CR_PSIZE_BYTE     0
#define STM32_DMA_CR_PSIZE_HWORD    DMA_SxCR_PSIZE_0
#define STM32_DMA_CR_PSIZE_WORD     DMA_SxCR_PSIZE_1
#define STM32_DMA_CR_MSIZE_MASK     DMA_SxCR_MSIZE_Msk
#define STM32_DMA_CR_MSIZE_BYTE     0
#define STM32_DMA_CR_MSIZE_HWORD    DMA_SxCR_MSIZE_0
#define STM32_DMA_CR_MSIZE_WORD     DMA_SxCR_MSIZE_1
#define STM32_DMA_CR_SIZE_MASK      (STM32_DMA_CR_PSIZE_MASK |              \
                                     STM32_DMA_CR_MSIZE_MASK)
#define STM32_DMA_CR_PINCOS         DMA_SxCR_PINCOS
#define STM32_DMA_CR_PL_MASK        DMA_SxCR_PL_Msk
#define STM32_DMA_CR_PL(n)          ((n) << 16U)
#define STM32_DMA_CR_DBM            DMA_SxCR_DBM
#define STM32_DMA_CR_CT             DMA_SxCR_CT
#define STM32_DMA_CR_PBURST_MASK    DMA_SxCR_PBURST_Msk
#define STM32_DMA_CR_PBURST_SINGLE  0
#define STM32_DMA_CR_PBURST_INCR4   DMA_SxCR_PBURST_0
#define STM32_DMA_CR_PBURST_INCR8   DMA_SxCR_PBURST_1
#define STM32_DMA_CR_PBURST_INCR16  (DMA_SxCR_PBURST_0 | DMA_SxCR_PBURST_1)
#define STM32_DMA_CR_MBURST_MASK    DMA_SxCR_MBURST_Msk
#define STM32_DMA_CR_MBURST_SINGLE  0
#define STM32_DMA_CR_MBURST_INCR4   DMA_SxCR_MBURST_0
#define STM32_DMA_CR_MBURST_INCR8   DMA_SxCR_MBURST_1
#define STM32_DMA_CR_MBURST_INCR16  (DMA_SxCR_MBURST_0 | DMA_SxCR_MBURST_1)
/** @} */

/**
 * @name    FCR register constants
 * @{
 */
#define STM32_DMA_FCR_RESET_VALUE   0x00000021U
#define STM32_DMA_FCR_FTH_MASK      DMA_SxFCR_FTH_Msk
#define STM32_DMA_FCR_FTH_1Q        0
#define STM32_DMA_FCR_FTH_HALF      DMA_SxFCR_FTH_0
#define STM32_DMA_FCR_FTH_3Q        DMA_SxFCR_FTH_1
#define STM32_DMA_FCR_FTH_FULL      (DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1)
#define STM32_DMA_FCR_DMDIS         DMA_SxFCR_DMDIS
#define STM32_DMA_FCR_FEIE          DMA_SxFCR_FEIE
#define STM32_DMA_FCR_FS_MASK       DMA_SxFCR_FS_Msk
/** @} */

/**
 * @name    Status flags passed to the ISR callbacks
 */
#define STM32_DMA_ISR_FEIF          DMA_LISR_FEIF0
#define STM32_DMA_ISR_DMEIF         DMA_LISR_DMEIF0
#define STM32_DMA_ISR_TEIF          DMA_LISR_TEIF0
#define STM32_DMA_ISR_HTIF          DMA_LISR_HTIF0
#define STM32_DMA_ISR_TCIF          DMA_LISR_TCIF0
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_HAS_DMA1)
#error "STM32_HAS_DMA1 missing in registry"
#endif

#if !defined(STM32_HAS_DMA2)
#error "STM32_HAS_DMA2 missing in registry"
#endif

#if !defined(STM32_DMA1_CH0_HANDLER)
#error "STM32_DMA1_CH0_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA1_CH1_HANDLER)
#error "STM32_DMA1_CH1_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA1_CH2_HANDLER)
#error "STM32_DMA1_CH2_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA1_CH3_HANDLER)
#error "STM32_DMA1_CH3_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA1_CH4_HANDLER)
#error "STM32_DMA1_CH4_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA1_CH5_HANDLER)
#error "STM32_DMA1_CH5_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA1_CH6_HANDLER)
#error "STM32_DMA1_CH6_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA1_CH7_HANDLER)
#error "STM32_DMA1_CH7_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA2_CH0_HANDLER)
#error "STM32_DMA2_CH0_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA2_CH1_HANDLER)
#error "STM32_DMA2_CH1_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA2_CH2_HANDLER)
#error "STM32_DMA2_CH2_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA2_CH3_HANDLER)
#error "STM32_DMA2_CH3_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA2_CH4_HANDLER)
#error "STM32_DMA2_CH4_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA2_CH5_HANDLER)
#error "STM32_DMA2_CH5_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA2_CH6_HANDLER)
#error "STM32_DMA2_CH6_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA2_CH7_HANDLER)
#error "STM32_DMA2_CH7_HANDLER missing in registry"
#endif

#if !defined(STM32_DMA1_CH0_NUMBER)
#error "STM32_DMA1_CH0_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA1_CH1_NUMBER)
#error "STM32_DMA1_CH1_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA1_CH2_NUMBER)
#error "STM32_DMA1_CH2_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA1_CH3_NUMBER)
#error "STM32_DMA1_CH3_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA1_CH4_NUMBER)
#error "STM32_DMA1_CH4_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA1_CH5_NUMBER)
#error "STM32_DMA1_CH5_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA1_CH6_NUMBER)
#error "STM32_DMA1_CH6_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA1_CH7_NUMBER)
#error "STM32_DMA1_CH7_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA2_CH0_NUMBER)
#error "STM32_DMA2_CH0_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA2_CH1_NUMBER)
#error "STM32_DMA2_CH1_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA2_CH2_NUMBER)
#error "STM32_DMA2_CH2_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA2_CH3_NUMBER)
#error "STM32_DMA2_CH3_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA2_CH4_NUMBER)
#error "STM32_DMA2_CH4_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA2_CH5_NUMBER)
#error "STM32_DMA2_CH5_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA2_CH6_NUMBER)
#error "STM32_DMA2_CH6_NUMBER missing in registry"
#endif

#if !defined(STM32_DMA2_CH7_NUMBER)
#error "STM32_DMA2_CH7_NUMBER missing in registry"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   STM32 DMA stream descriptor structure.
 */
typedef struct {
  DMA_Stream_TypeDef        *stream;    /**< @brief Associated DMA stream.  */
  volatile uint32_t         *ifcr;      /**< @brief Associated IFCR reg.    */
  uint8_t                   ishift;     /**< @brief Bits offset in xIFCR
                                             register.                      */
  DMAMUX_Channel_TypeDef    *mux;       /**< @brief Associated DMA stream.  */
  uint8_t                   selfindex;  /**< @brief Index to self in array. */
  uint8_t                   vector;     /**< @brief Associated IRQ vector.  */
} stm32_dma_stream_t;

/**
 * @brief   STM32 DMA ISR function type.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] flags     pre-shifted content of the xISR register, the bits
 *                      are aligned to bit zero
 */
typedef void (*stm32_dmaisr_t)(void *p, uint32_t flags);

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Associates a peripheral data register to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 * @param[in] addr      value to be written in the PAR register
 *
 * @special
 */
#define dmaStreamSetPeripheral(stp, addr) {                                 \
  (stp)->stream->PAR  = (uint32_t)(addr);                                   \
}

/**
 * @brief   Associates a memory destination to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 * @param[in] addr      value to be written in the M0AR register
 *
 * @special
 */
#define dmaStreamSetMemory0(stp, addr) {                                    \
  (stp)->stream->M0AR  = (uint32_t)(addr);                                  \
}

/**
 * @brief   Associates an alternate memory destination to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 * @param[in] addr      value to be written in the M1AR register
 *
 * @special
 */
#define dmaStreamSetMemory1(stp, addr) {                                    \
  (stp)->stream->M1AR  = (uint32_t)(addr);                                  \
}

/**
 * @brief   Sets the number of transfers to be performed.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 * @param[in] size      value to be written in the CNDTR register
 *
 * @special
 */
#define dmaStreamSetTransactionSize(stp, size) {                            \
  (stp)->stream->NDTR  = (uint32_t)(size);                                  \
}

/**
 * @brief   Returns the number of transfers to be performed.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 * @return              The number of transfers to be performed.
 *
 * @special
 */
#define dmaStreamGetTransactionSize(stp) ((size_t)((stp)->stream->NDTR))

/**
 * @brief   Programs the stream mode settings.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 * @param[in] mode      value to be written in the CR register
 *
 * @special
 */
#define dmaStreamSetMode(stp, mode) {                                       \
  (stp)->stream->CR  = (uint32_t)(mode);                                    \
}

/**
 * @brief   Programs the stream FIFO settings.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 * @param[in] mode      value to be written in the FCR register
 *
 * @special
 */
#define dmaStreamSetFIFO(stp, mode) {                                       \
  (stp)->stream->FCR = (uint32_t)(mode);                                    \
}

/**
 * @brief   DMA stream enable.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamEnable(stp) {                                              \
  (stp)->stream->CR |= STM32_DMA_CR_EN;                                     \
}

/**
 * @brief   DMA stream disable.
 * @details The function disables the specified stream, waits for the disable
 *          operation to complete and then clears any pending interrupt.
 * @note    This function can be invoked in both ISR or thread context.
 * @note    Interrupts enabling flags are set to zero after this call, see
 *          bug 3607518.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamDisable(stp) {                                             \
  (stp)->stream->CR &= ~(STM32_DMA_CR_TCIE | STM32_DMA_CR_HTIE  |           \
                         STM32_DMA_CR_TEIE | STM32_DMA_CR_DMEIE |           \
                         STM32_DMA_CR_EN);                                  \
  while (((stp)->stream->CR & STM32_DMA_CR_EN) != 0)                        \
    ;                                                                       \
  dmaStreamClearInterrupt(stp);                                             \
}

/**
 * @brief   DMA stream interrupt sources clear.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamClearInterrupt(stp) {                                      \
  *(stp)->ifcr = STM32_DMA_ISR_MASK << (stp)->ishift;                       \
}

/**
 * @brief   Starts a memory to memory operation using the specified stream.
 * @note    The default transfer data mode is "byte to byte" but it can be
 *          changed by specifying extra options in the @p mode parameter.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 * @param[in] mode      value to be written in the CCR register, this value
 *                      is implicitly ORed with:
 *                      - @p STM32_DMA_CR_MINC
 *                      - @p STM32_DMA_CR_PINC
 *                      - @p STM32_DMA_CR_DIR_M2M
 *                      - @p STM32_DMA_CR_EN
 *                      .
 * @param[in] src       source address
 * @param[in] dst       destination address
 * @param[in] n         number of data units to copy
 */
#define dmaStartMemCopy(stp, mode, src, dst, n) {                           \
  dmaStreamSetPeripheral(stp, src);                                         \
  dmaStreamSetMemory0(stp, dst);                                            \
  dmaStreamSetTransactionSize(stp, n);                                      \
  dmaStreamSetMode(stp, (mode) |                                            \
                        STM32_DMA_CR_MINC | STM32_DMA_CR_PINC |             \
                        STM32_DMA_CR_DIR_M2M);                              \
  dmaStreamEnable(stp);                                                     \
}

/**
 * @brief   Polled wait for DMA transfer end.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 */
#define dmaWaitCompletion(stp) {                                            \
  (stp)->stream->CR &= ~(STM32_DMA_CR_TCIE | STM32_DMA_CR_HTIE  |           \
                         STM32_DMA_CR_TEIE | STM32_DMA_CR_DMEIE);           \
  while ((stp)->stream->CR & STM32_DMA_CR_EN)                               \
    ;                                                                       \
  dmaStreamClearInterrupt(stp);                                             \
}

/**
 * @brief   DMA stream current target.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAllocate().
 * @post    After use the stream can be released using @p dmaStreamRelease().
 *
 * @param[in] stp       pointer to a @p stm32_dma_stream_t structure
 * @return              Current memory target index.
 *
 * @special
 */
#define dmaStreamGetCurrentTarget(stp)                                      \
  (((stp)->stream->CR >> DMA_SxCR_CT_Pos) & 1U)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern const stm32_dma_stream_t _stm32_dma_streams[STM32_DMA_STREAMS];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
  bool dmaStreamAllocate(const stm32_dma_stream_t *stp,
                         uint32_t priority,
                         stm32_dmaisr_t func,
                         void *param);
  void dmaSetRequestSource(const stm32_dma_stream_t *stp, uint32_t per);
  void dmaStreamRelease(const stm32_dma_stream_t *stp);
#ifdef __cplusplus
}
#endif

#endif /* STM32_DMA_H */

/** @} */
