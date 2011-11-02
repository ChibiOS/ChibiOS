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
 * @file    STM32F4xx/stm32_dma.h
 * @brief   Enhanced-DMA helper driver header.
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
#define STM32_DMA_STREAMS           16

/**
 * @brief   Mask of the ISR bits passed to the DMA callback functions.
 */
#define STM32_DMA_ISR_MASK          0x3D

/**
 * @name    DMA streams identifiers
 * @{
 */
#define STM32_DMA1_STREAM0          (&_stm32_dma_streams[0])
#define STM32_DMA1_STREAM1          (&_stm32_dma_streams[1])
#define STM32_DMA1_STREAM2          (&_stm32_dma_streams[2])
#define STM32_DMA1_STREAM3          (&_stm32_dma_streams[3])
#define STM32_DMA1_STREAM4          (&_stm32_dma_streams[4])
#define STM32_DMA1_STREAM5          (&_stm32_dma_streams[5])
#define STM32_DMA1_STREAM6          (&_stm32_dma_streams[6])
#define STM32_DMA1_STREAM7          (&_stm32_dma_streams[7])
#define STM32_DMA2_STREAM0          (&_stm32_dma_streams[8])
#define STM32_DMA2_STREAM1          (&_stm32_dma_streams[9])
#define STM32_DMA2_STREAM2          (&_stm32_dma_streams[10])
#define STM32_DMA2_STREAM3          (&_stm32_dma_streams[11])
#define STM32_DMA2_STREAM4          (&_stm32_dma_streams[12])
#define STM32_DMA2_STREAM5          (&_stm32_dma_streams[13])
#define STM32_DMA2_STREAM6          (&_stm32_dma_streams[14])
#define STM32_DMA2_STREAM7          (&_stm32_dma_streams[15])
/** @} */

/**
 * @name    CR register constants common to all DMA types
 */
#define STM32_DMA_CR_EN             DMA_SxCR_EN
#define STM32_DMA_CR_TEIE           DMA_SxCR_TEIE
#define STM32_DMA_CR_HTIE           DMA_SxCR_HTIE
#define STM32_DMA_CR_TCIE           DMA_SxCR_TCIE
#define STM32_DMA_CR_DIR_MASK       DMA_SxCR_DIR
#define STM32_DMA_CR_DIR_P2M        0
#define STM32_DMA_CR_DIR_M2P        DMA_SxCR_DIR_0
#define STM32_DMA_CR_DIR_M2M        DMA_SxCR_DIR_1
#define STM32_DMA_CR_CIRC           DMA_SxCR_CIRC
#define STM32_DMA_CR_PINC           DMA_SxCR_PINC
#define STM32_DMA_CR_MINC           DMA_SxCR_MINC
#define STM32_DMA_CR_PSIZE_MASK     DMA_SxCR_PSIZE
#define STM32_DMA_CR_PSIZE_BYTE     0
#define STM32_DMA_CR_PSIZE_HWORD    DMA_SxCR_PSIZE_0
#define STM32_DMA_CR_PSIZE_WORD     DMA_SxCR_PSIZE_1
#define STM32_DMA_CR_MSIZE_MASK     DMA_SxCR_MSIZE
#define STM32_DMA_CR_MSIZE_BYTE     0
#define STM32_DMA_CR_MSIZE_HWORD    DMA_SxCR_MSIZE_0
#define STM32_DMA_CR_MSIZE_WORD     DMA_SxCR_MSIZE_1
#define STM32_DMA_CR_PL_MASK        DMA_SxCR_PL
#define STM32_DMA_CR_PL(n)          ((n) << 16)
/** @} */

/**
 * @name    CR register constants only found in STM32F2xx
 */
#define STM32_DMA_CR_DMEIE          DMA_SxCR_DMEIE
#define STM32_DMA_CR_PFCTRL         DMA_SxCR_PFCTRL
#define STM32_DMA_CR_PINCOS         DMA_SxCR_PINCOS
#define STM32_DMA_CR_DBM            DMA_SxCR_DBM
#define STM32_DMA_CR_CT             DMA_SxCR_CT
#define STM32_DMA_CR_PBURST_MASK    DMA_SxCR_PBURST
#define STM32_DMA_CR_PBURST_SINGLE  0
#define STM32_DMA_CR_PBURST_INCR4   DMA_SxCR_PBURST_0
#define STM32_DMA_CR_PBURST_INCR8   DMA_SxCR_PBURST_1
#define STM32_DMA_CR_PBURST_INCR16  (DMA_SxCR_PBURST_0 | DMA_SxCR_PBURST_1)
#define STM32_DMA_CR_MBURST_MASK    DMA_SxCR_MBURST
#define STM32_DMA_CR_MBURST_SINGLE  0
#define STM32_DMA_CR_MBURST_INCR4   DMA_SxCR_MBURST_0
#define STM32_DMA_CR_MBURST_INCR8   DMA_SxCR_MBURST_1
#define STM32_DMA_CR_MBURST_INCR16  (DMA_SxCR_MBURST_0 | DMA_SxCR_MBURST_1)
#define STM32_DMA_CR_CHSEL_MASK     DMA_SxCR_CHSEL
#define STM32_DMA_CR_CHSEL(n)       ((n) << 25)
/** @} */

/**
 * @name    FCR register constants only found in STM32F2xx
 */
#define STM32_DMA_FCR_FEIE          DMA_SxFCR_FEIE
#define STM32_DMA_FCR_FS_MASK       DMA_SxFCR_FS
#define STM32_DMA_FCR_DMDIS         DMA_SxFCR_DMDIS
#define STM32_DMA_FCR_FTH_MASK      DMA_SxFCR_FTH
#define STM32_DMA_FCR_FTH_1Q        0
#define STM32_DMA_FCR_FTH_HALF      DMA_SxFCR_FTH_0
#define STM32_DMA_FCR_FTH_3Q        DMA_SxFCR_FTH_1
#define STM32_DMA_FCR_FTH_FULL      (DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1)
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

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   STM32 DMA stream descriptor structure.
 */
typedef struct {
  DMA_Channel_TypeDef   *channel;       /**< @brief Associated DMA channel. */
  volatile uint32_t     *ifcr;          /**< @brief Associated IFCR reg.    */
  uint8_t               ishift;         /**< @brief Bits offset in xIFCR
                                             register.                      */
  uint8_t               selfindex;      /**< @brief Index to self in array. */
  uint8_t               vector;         /**< @brief Associated IRQ vector.  */
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
 * @brief   Sets the number of transfers to be performed.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] size      value to be written in the CNDTR register
 *
 * @special
 */
#define dmaStreamSetTransactionSize(dmastp, size) {                         \
  (dmastp)->stream->NDTR  = (uint32_t)(size);                               \
}

/**
 * @brief   Returns the number of transfers to be performed.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @return              The number of transfers to be performed.
 *
 * @special
 */
#define dmaStreamGetTransactionSize(dmastp) ((size_t)((dmastp)->stream->NDTR))

/**
 * @brief   Programs the stream mode settings.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] mode      value to be written in the CR register
 *
 * @special
 */
#define dmaStreamSetMode(dmastp, mode) {                                    \
  (dmastp)->stream->CR  = (uint32_t)(mode);                                 \
}

/**
 * @brief   Programs the stream FIFO settings.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] mode      value to be written in the FCR register
 *
 * @special
 */
#define dmaStreamSetFIFO(dmastp, mode) {                                    \
  (dmastp)->stream->FCR = (uint32_t)(mode);                                 \
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
  (dmastp)->stream->CR |= STM32_DMA_CR_EN;                                  \
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
  (dmastp)->stream->CR &= ~STM32_DMA_CR_EN;                                 \
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
  bool_t dmaStreamAllocate(const stm32_dma_stream_t *dmastp,
                           uint32_t priority,
                           stm32_dmaisr_t func,
                           void *param);
  void dmaStreamRelease(const stm32_dma_stream_t *dmastp);
#ifdef __cplusplus
}
#endif

#endif /* _STM32_DMA_H_ */

/** @} */
