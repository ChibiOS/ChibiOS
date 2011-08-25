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
 * @file    DMAv2/stm32_dma.h
 * @brief   STM32F2xx Enhanced DMA helper driver header.
 * @note    This file requires definitions from the ST STM32F2xx header file
 *          stm32f2xx.h.
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
#define STM32_DMA_STREAMS   16

/**
 * @brief   Mask of the ISR bits passed to the DMA callback functions.
 */
#define STM32_DMA_ISR_MASK  0x3D

/**
 * @name    DMA streams identifiers
 * @{
 */
#define STM32_DMA1_STREAM0  (&_stm32_dma_streams[0])
#define STM32_DMA1_STREAM1  (&_stm32_dma_streams[1])
#define STM32_DMA1_STREAM2  (&_stm32_dma_streams[2])
#define STM32_DMA1_STREAM3  (&_stm32_dma_streams[3])
#define STM32_DMA1_STREAM4  (&_stm32_dma_streams[4])
#define STM32_DMA1_STREAM5  (&_stm32_dma_streams[5])
#define STM32_DMA1_STREAM6  (&_stm32_dma_streams[6])
#define STM32_DMA1_STREAM7  (&_stm32_dma_streams[7])
#define STM32_DMA2_STREAM0  (&_stm32_dma_streams[8])
#define STM32_DMA2_STREAM1  (&_stm32_dma_streams[9])
#define STM32_DMA2_STREAM2  (&_stm32_dma_streams[10])
#define STM32_DMA2_STREAM3  (&_stm32_dma_streams[11])
#define STM32_DMA2_STREAM4  (&_stm32_dma_streams[12])
#define STM32_DMA2_STREAM5  (&_stm32_dma_streams[13])
#define STM32_DMA2_STREAM6  (&_stm32_dma_streams[14])
#define STM32_DMA2_STREAM7  (&_stm32_dma_streams[15])
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
  DMA_Stream_TypeDef    *stream;        /**< @brief Associated DMA stream.  */
  volatile uint32_t     *isr;           /**< @brief Associated xISR reg.    */
  volatile uint32_t     *ifcr;          /**< @brief Associated xIFCR reg.   */
  uint32_t              ishift;         /**< @brief Bits offset in xISR and
                                             xIFCR registers.               */
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
 * @brief   Associates a peripheral data register to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] addr      value to be written in the PAR register
 *
 * @special
 */
#define dmaStreamSetPeripheral(dmastp, addr) {                              \
  (dmastp)->stream->PAR  = (uint32_t)(addr);                                \
}

/**
 * @brief   Associates a memory destination to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] addr      value to be written in the M0AR register
 *
 * @special
 */
#define dmaStreamSetMemory0(dmastp, addr) {                                 \
  (dmastp)->stream->M0AR  = (uint32_t)(addr);                               \
}

/**
 * @brief   Associates an alternate memory destination to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] addr      value to be written in the M1AR register
 *
 * @special
 */
#define dmaStreamSetMemory1(dmastp, addr) {                                 \
  (dmastp)->stream->M1AR  = (uint32_t)(addr);                               \
}

/**
 * @brief   Associates an alternate memory destination to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] size      value to be written in the NDTR register
 *
 * @special
 */
#define dmaStreamSetTransactionSize(dmastp, size) {                         \
  (dmastp)->stream->NDTR  = (uint32_t)(size);                               \
}

/**
 * @brief   Programs the stream mode settings.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] mode1     value to be written in the FCR register
 * @param[in] mode2     value to be written in the CR register
 *
 * @special
 */
#define dmaStreamSetMode(dmastp, mode1, mode2) {                            \
    (dmastp)->stream->FCR = (uint32_t)(mode1);                              \
    (dmastp)->stream->CR  = (uint32_t)(mode2);                              \
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
  (dmastp)->stream->CR |= DMA_SxCR_EN;                                      \
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
  (dmastp)->stream->CR &= ~DMA_SxCR_EN;                                     \
}

/**
 * @brief   DMA stream interrupt sources clear.
 * @details Sets the appropriate CGIF bit into the IFCR register in order to
 *          withdraw all the pending interrupt bits from the ISR register.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamClearInterrupt(dmastp) {                                   \
  *(dmastp)->stream->ifcr = STM32_DMA_ISR_MASK << (dmastp)->stream->ishift; \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined()
extern _stm32_dma_streams[STM32_DMA_STREAMS];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
  void dmaAllocate(stm32_dma_stream_t *dmastp,
                   stm32_dmaisr_t func, void *param);
  void dmaRelease(stm32_dma_stream_t *dmastp);
#ifdef __cplusplus
}
#endif

#endif /* _STM32_DMA_H_ */

/** @} */
