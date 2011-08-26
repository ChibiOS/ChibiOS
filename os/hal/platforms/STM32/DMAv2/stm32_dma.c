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
 * @file    DMAv2/stm32_dma.c
 * @brief   Enhanced DMA helper driver code.
 *
 * @addtogroup STM32_DMA
 * @details DMA sharing helper driver. In the STM32 the DMA streams are a
 *          shared resource, this driver allows to allocate and free DMA
 *          streams at runtime in order to allow all the other device
 *          drivers to coordinate the access to the resource.
 * @note    The DMA ISR handlers are all declared into this module because
 *          sharing, the various device drivers can associate a callback to
 *          IRSs when allocating streams.
 * @{
 */

#include "ch.h"
#include "hal.h"

/* The following macro is only defined if some driver requiring DMA services
   has been enabled.*/
#if defined(STM32_DMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Mask of the DMA1 streams in @p dma_streams_mask.
 */
#define STM32_DMA1_STREAMS_MASK     0x000000FF

/**
 * @brief   Mask of the DMA2 streams in @p dma_streams_mask.
 */
#define STM32_DMA2_STREAMS_MASK     0x0000FF00

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   DMA streams descriptors.
 * @details This table keeps the association between an unique stream
 *          identifier and the involved physical registers.
 * @note    Don't use this array directly, use the appropriate wrapper macros
 *          instead: @p STM32_DMA1_STREAM0, @p STM32_DMA1_STREAM1 etc.
 */
const stm32_dma_stream_t _stm32_dma_streams[STM32_DMA_STREAMS] = {
  {0, DMA1, DMA1_Stream0, &DMA1->LIFCR, 0},
  {1, DMA1, DMA1_Stream1, &DMA1->LIFCR, 6},
  {2, DMA1, DMA1_Stream2, &DMA1->LIFCR, 16},
  {3, DMA1, DMA1_Stream3, &DMA1->LIFCR, 22},
  {4, DMA1, DMA1_Stream4, &DMA1->HIFCR, 0},
  {5, DMA1, DMA1_Stream5, &DMA1->HIFCR, 6},
  {6, DMA1, DMA1_Stream6, &DMA1->HIFCR, 16},
  {7, DMA1, DMA1_Stream7, &DMA1->HIFCR, 22},
  {8, DMA2, DMA2_Stream0, &DMA2->LIFCR, 0},
  {9, DMA2, DMA2_Stream1, &DMA2->LIFCR, 6},
  {10, DMA2, DMA2_Stream2, &DMA2->LIFCR, 16},
  {11, DMA2, DMA2_Stream3, &DMA2->LIFCR, 22},
  {12, DMA2, DMA2_Stream4, &DMA2->HIFCR, 0},
  {13, DMA2, DMA2_Stream5, &DMA2->HIFCR, 6},
  {14, DMA2, DMA2_Stream6, &DMA2->HIFCR, 16},
  {15, DMA2, DMA2_Stream7, &DMA2->HIFCR, 22},
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   DMA ISR redirector type.
 */
typedef struct {
  stm32_dmaisr_t        dma_func;
  void                  *dma_param;
} dma_isr_redir_t;

/**
 * @brief   Mask of the allocated streams.
 */
static uint32_t dma_streams_mask;

/**
 * @brief   DMA IRQ redirectors.
 */
static dma_isr_redir_t dma_isr_redir[STM32_DMA_STREAMS];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   DMA1 stream 0 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Stream0_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA1->LISR >> 0) & STM32_DMA_ISR_MASK;
  DMA1->LIFCR = STM32_DMA_ISR_MASK << 0;
  if (dma_isr_redir[0].dma_func)
    dma_isr_redir[0].dma_func(dma_isr_redir[0].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 1 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Stream1_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA1->LISR >> 6) & STM32_DMA_ISR_MASK;
  DMA1->LIFCR = STM32_DMA_ISR_MASK << 6;
  if (dma_isr_redir[1].dma_func)
    dma_isr_redir[1].dma_func(dma_isr_redir[0].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 2 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Stream2_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA1->LISR >> 16) & STM32_DMA_ISR_MASK;
  DMA1->LIFCR = STM32_DMA_ISR_MASK << 16;
  if (dma_isr_redir[2].dma_func)
    dma_isr_redir[2].dma_func(dma_isr_redir[2].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 3 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Stream3_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA1->LISR >> 22) & STM32_DMA_ISR_MASK;
  DMA1->LIFCR = STM32_DMA_ISR_MASK << 22;
  if (dma_isr_redir[3].dma_func)
    dma_isr_redir[3].dma_func(dma_isr_redir[3].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 4 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Stream4_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA1->HISR >> 0) & STM32_DMA_ISR_MASK;
  DMA1->HIFCR = STM32_DMA_ISR_MASK << 0;
  if (dma_isr_redir[4].dma_func)
    dma_isr_redir[4].dma_func(dma_isr_redir[4].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 5 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Stream5_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA1->HISR >> 6) & STM32_DMA_ISR_MASK;
  DMA1->HIFCR = STM32_DMA_ISR_MASK << 6;
  if (dma_isr_redir[5].dma_func)
    dma_isr_redir[5].dma_func(dma_isr_redir[5].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 6 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Stream6_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA1->HISR >> 16) & STM32_DMA_ISR_MASK;
  DMA1->HIFCR = STM32_DMA_ISR_MASK << 16;
  if (dma_isr_redir[6].dma_func)
    dma_isr_redir[6].dma_func(dma_isr_redir[6].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 7 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Stream7_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA1->HISR >> 22) & STM32_DMA_ISR_MASK;
  DMA1->HIFCR = STM32_DMA_ISR_MASK << 22;
  if (dma_isr_redir[7].dma_func)
    dma_isr_redir[7].dma_func(dma_isr_redir[7].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 0 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Stream0_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA2->LISR >> 0) & STM32_DMA_ISR_MASK;
  DMA2->LIFCR = STM32_DMA_ISR_MASK << 0;
  if (dma_isr_redir[8].dma_func)
    dma_isr_redir[8].dma_func(dma_isr_redir[8].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 1 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Stream1_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA2->LISR >> 6) & STM32_DMA_ISR_MASK;
  DMA2->LIFCR = STM32_DMA_ISR_MASK << 6;
  if (dma_isr_redir[9].dma_func)
    dma_isr_redir[9].dma_func(dma_isr_redir[9].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 2 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Stream2_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA2->LISR >> 16) & STM32_DMA_ISR_MASK;
  DMA2->LIFCR = STM32_DMA_ISR_MASK << 16;
  if (dma_isr_redir[10].dma_func)
    dma_isr_redir[10].dma_func(dma_isr_redir[10].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 3 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Stream3_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA2->LISR >> 22) & STM32_DMA_ISR_MASK;
  DMA2->LIFCR = STM32_DMA_ISR_MASK << 22;
  if (dma_isr_redir[11].dma_func)
    dma_isr_redir[11].dma_func(dma_isr_redir[11].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 4 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Stream4_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA2->HISR >> 0) & STM32_DMA_ISR_MASK;
  DMA2->HIFCR = STM32_DMA_ISR_MASK << 0;
  if (dma_isr_redir[12].dma_func)
    dma_isr_redir[12].dma_func(dma_isr_redir[12].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 5 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Stream5_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA2->HISR >> 6) & STM32_DMA_ISR_MASK;
  DMA2->HIFCR = STM32_DMA_ISR_MASK << 6;
  if (dma_isr_redir[13].dma_func)
    dma_isr_redir[13].dma_func(dma_isr_redir[13].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 6 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Stream6_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA2->HISR >> 16) & STM32_DMA_ISR_MASK;
  DMA2->HIFCR = STM32_DMA_ISR_MASK << 16;
  if (dma_isr_redir[14].dma_func)
    dma_isr_redir[14].dma_func(dma_isr_redir[14].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 7 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Stream7_IRQHandler) {
  uint32_t flags;

  CH_IRQ_PROLOGUE();

  flags = (DMA2->HISR >> 22) & STM32_DMA_ISR_MASK;
  DMA2->HIFCR = STM32_DMA_ISR_MASK << 22;
  if (dma_isr_redir[15].dma_func)
    dma_isr_redir[15].dma_func(dma_isr_redir[15].dma_param, flags);

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   STM32 DMA helper initialization.
 *
 * @init
 */
void dmaInit(void) {
  int i;

  dma_streams_mask = 0;
  for (i = 0; i < STM32_DMA_STREAMS; i--) {
    _stm32_dma_streams[i].stream->CR = 0;
    dma_isr_redir[i].dma_func = NULL;
  }
  DMA1->LIFCR = 0xFFFFFFFF;
  DMA1->HIFCR = 0xFFFFFFFF;
  DMA2->LIFCR = 0xFFFFFFFF;
  DMA2->HIFCR = 0xFFFFFFFF;
}

/**
 * @brief   Allocates a DMA stream.
 * @details The stream is allocated and, if required, the DMA clock enabled.
 *          Trying to allocate a stream already allocated is an illegal
 *          operation and is trapped if assertions are enabled.
 * @pre     The stream must not be already in use.
 * @post    The stream is allocated and the default ISR handler redirected
 *          to the specified function.
 * @post    The stream must be freed using @p dmaRelease() before it can
 *          be reused with another peripheral.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              The operation status.
 * @retval FALSE        no error, stream taken.
 * @retval TRUE         error, stream already taken.
 *
 * @special
 */
bool_t dmaAllocate(stm32_dma_stream_t *dmastp,
                   stm32_dmaisr_t func, void *param) {

  chDbgCheck(dmastp != NULL, "dmaAllocate");

  /* Checks if the stream is already taken.*/
  if ((dma_streams_mask & dmastp->mask) != 0)
    return TRUE;

  /* Marks the stream as allocated.*/
  dma_isr_redir[dmastp->selfindex].dma_func  = func;
  dma_isr_redir[dmastp->selfindex].dma_param = param;
  dma_streams_mask |= (1 << dmastp->selfindex);

  /* Enabling DMA clocks required by the current streams set.*/
  if ((dma_streams_mask & STM32_DMA1_STREAMS_MASK) != 0) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    RCC->AHB1LPENR |= RCC_AHB1LPENR_DMA1LPEN;
  }
  if ((dma_streams_mask & STM32_DMA2_STREAMS_MASK) != 0) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    RCC->AHB1LPENR |= RCC_AHB1LPENR_DMA2LPEN;
  }

  /* Making sure there are no spurious interrupts flags pending.*/
  dmaStreamClearInterrupt(dmastp);
  return FALSE;
}

/**
 * @brief   Releases a DMA stream.
 * @details The stream is freed and, if required, the DMA clock disabled.
 *          Trying to release a unallocated stream is an illegal operation
 *          and is trapped if assertions are enabled.
 * @pre     The stream must have been allocated using @p dmaRequest().
 * @post    The stream is again available.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 *
 * @special
 */
void dmaRelease(stm32_dma_stream_t *dmastp) {

  chDbgCheck(dmastp != NULL, "dmaRelease");

  /* Check if the streams is not taken.*/
  chDbgAssert((dma_streams_mask & dmastp->mask) != 0,
              "dmaRelease(), #1", "not allocated");

  /* Marks the stream as not allocated.*/
  dma_streams_mask &= ~(1 << dmastp->selfindex);

  /* Shutting down clocks that are no more required, if any.*/
  if ((dma_streams_mask & STM32_DMA1_STREAMS_MASK) == 0) {
    RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA1EN;
    RCC->AHB1LPENR &= ~RCC_AHB1LPENR_DMA1LPEN;
  }
  if ((dma_streams_mask & STM32_DMA2_STREAMS_MASK) == 0) {
    RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA2EN;
    RCC->AHB1LPENR &= ~RCC_AHB1LPENR_DMA2LPEN;
  }
}

#endif /* STM32_DMA_REQUIRED */

/** @} */
