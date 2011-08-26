/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    DMAv1/stm32_dma.h
 * @brief   DMA helper driver header.
 * @note    This file requires definitions from the ST header files
 *          stm32f10x.h or stm32l1xx.h.
 * @note    This driver uses the new naming convention used for the STM32F2xx
 *          so the "DMA channels" are referred as "DMA streams".
 *
 * @addtogroup STM32_DMA
 * @{
 */

#ifndef _STM32_DMA_H_
#define _STM32_DMA_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Total number of DMA streams.
 * @note    This is the total number of streams among all the DMA units.
 */
#if STM32_HAS_DMA2 || defined(__DOXYGEN__)
#define STM32_DMA_STREAMS           12
#else
#define STM32_DMA_STREAMS           7
#endif

/**
 * @brief   Mask of the ISR bits passed to the DMA callback functions.
 */
#define STM32_DMA_ISR_MASK          0x0F

/**
 * @name    DMA streams identifiers
 * @{
 */
#define STM32_DMA1_STREAM1          (&_stm32_dma_streams[0])
#define STM32_DMA1_STREAM2          (&_stm32_dma_streams[1])
#define STM32_DMA1_STREAM3          (&_stm32_dma_streams[2])
#define STM32_DMA1_STREAM4          (&_stm32_dma_streams[3])
#define STM32_DMA1_STREAM5          (&_stm32_dma_streams[4])
#define STM32_DMA1_STREAM6          (&_stm32_dma_streams[5])
#define STM32_DMA1_STREAM7          (&_stm32_dma_streams[6])
#define STM32_DMA2_STREAM1          (&_stm32_dma_streams[8])
#define STM32_DMA2_STREAM2          (&_stm32_dma_streams[9])
#define STM32_DMA2_STREAM3          (&_stm32_dma_streams[10])
#define STM32_DMA2_STREAM4          (&_stm32_dma_streams[11])
#define STM32_DMA2_STREAM5          (&_stm32_dma_streams[12])
/** @} */

/**
 * @name    CR register constants common to all DMA types
 */
#define STM32_DMA_CR_EN             DMA_CCR1_EN
#define STM32_DMA_CR_TEIE           DMA_CCR1_TEIE
#define STM32_DMA_CR_HTIE           DMA_CCR1_HTIE
#define STM32_DMA_CR_TCIE           DMA_CCR1_TCIE
#define STM32_DMA_CR_DIR_MASK       (DMA_CCR1_DIR | DMA_CCR1_MEM2MEM)
#define STM32_DMA_CR_DIR_P2M        0
#define STM32_DMA_CR_DIR_M2P        DMA_CCR1_DIR
#define STM32_DMA_CR_DIR_M2M        DMA_CCR1_MEM2MEM
#define STM32_DMA_CR_CIRC           DMA_CCR1_CIRC
#define STM32_DMA_CR_PINC           DMA_CCR1_PINC
#define STM32_DMA_CR_MINC           DMA_CCR1_MINC
#define STM32_DMA_CR_PSIZE_MASK     DMA_CCR1_PSIZE
#define STM32_DMA_CR_PSIZE_BYTE     0
#define STM32_DMA_CR_PSIZE_HWORD    DMA_CCR1_PSIZE_0
#define STM32_DMA_CR_PSIZE_WORD     DMA_CCR1_PSIZE_1
#define STM32_DMA_CR_MSIZE_MASK     DMA_CCR1_MSIZE
#define STM32_DMA_CR_MSIZE_BYTE     0
#define STM32_DMA_CR_MSIZE_HWORD    DMA_CCR1_MSIZE_0
#define STM32_DMA_CR_MSIZE_WORD     DMA_CCR1_MSIZE_1
#define STM32_DMA_CR_PL_MASK        DMA_CCR1_PL
#define STM32_DMA_CR_PL(n)          ((n) << 16)
/** @} */
/**
 * @name    CR register constants only found in enhanced DMA
 */
#define STM32_DMA_CR_CHSEL_MASK     0   /**< @brief Ignored by normal DMA.  */
#define STM32_DMA_CR_CHSEL(n)       0   /**< @brief Ignored by normal DMA.  */
/** @} */

/**
 * @name    Status flags passed to the ISR callbacks
 */
#define STM32_DMA_ISR_FEIF          0
#define STM32_DMA_ISR_DMEIF         0
#define STM32_DMA_ISR_TEIF          DMA_ISR_TEIF1
#define STM32_DMA_ISR_HTIF          DMA_ISR_HTIF1
#define STM32_DMA_ISR_TCIF          DMA_ISR_TCIF1
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   STM32 DMA stream descriptor structure.
 */
typedef struct {
  uint32_t              selfindex;      /**< @brief Index to self in array. */
  DMA_TypeDef           *dma;           /**< @brief Associated DMA unit.    */
  DMA_Channel_TypeDef   *channel;       /**< @brief Associated DMA channel. */
  volatile uint32_t     *ifcr;          /**< @brief Associated IFCR reg.    */
  uint32_t              ishift;         /**< @brief Bits offset in xIFCR
                                             register.                      */
} stm32_dma_stream_t;

/**
 * @brief   STM32 DMA ISR function type.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] flags     pre-shifted content of the ISR register, the bits
 *                      are aligned to bit zero
 */
typedef void (*stm32_dmaisr_t)(void *p, uint32_t flags);

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Associates a peripheral data register to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] addr      value to be written in the CPAR register
 *
 * @special
 */
#define dmaStreamSetPeripheral(dmastp, addr) {                              \
  (dmastp)->channel->CPAR  = (uint32_t)(addr);                              \
}

/**
 * @brief   Associates a memory destination to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] addr      value to be written in the CMAR register
 *
 * @special
 */
#define dmaStreamSetMemory0(dmastp, addr) {                                 \
  (dmastp)->channel->CMAR  = (uint32_t)(addr);                              \
}

/**
 * @brief   Associates an alternate memory destination to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] size      value to be written in the CNDTR register
 *
 * @special
 */
#define dmaStreamSetTransactionSize(dmastp, size) {                         \
  (dmastp)->channel->CNDTR  = (uint32_t)(size);                             \
}

/**
 * @brief   Programs the stream mode settings.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] mode      value to be written in the CCR register
 *
 * @special
 */
#define dmaStreamSetMode(dmastp, mode) {                                    \
  (dmastp)->channel->CCR  = (uint32_t)(mode2);                              \
}

/**
 * @brief   DMA stream enable.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmachp    pointer to a stm32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamEnable(dmachp) {                                           \
  (dmastp)->channel->CCR |= STM32_DMA_CR_EN;                                \
}

/**
 * @brief   DMA stream disable.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamDisable(dmastp) {                                          \
  (dmastp)->channel->CCR &= ~STM32_DMA_CR_EN;                               \
}

/**
 * @brief   DMA stream interrupt sources clear.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamClearInterrupt(dmastp) {                                   \
  *(dmastp)->ifcr = STM32_DMA_ISR_MASK << (dmastp)->ishift;                 \
}

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
  bool_t dmaAllocate(stm32_dma_stream_t *dmastp,
                     stm32_dmaisr_t func, void *param);
  void dmaRelease(stm32_dma_stream_t *dmastp);
#ifdef __cplusplus
}
#endif

#endif /* _STM32_DMA_H_ */

/** @} */
