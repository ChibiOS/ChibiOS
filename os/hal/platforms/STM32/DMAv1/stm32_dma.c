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
 * @file    stm32_dma.c
 * @brief   STM32 DMA helper driver code.
 *
 * @addtogroup STM32_DMA
 * @details DMA sharing helper driver. In the STM32 the DMA channels are a
 *          shared resource, this driver allows to allocate and free DMA
 *          channels at runtime in order to allow all the other device
 *          drivers to coordinate the access to the resource.
 * @note    The DMA ISR handlers are all declared into this module because
 *          sharing, the various device drivers can associate a callback to
 *          IRSs when allocating channels.
 * @{
 */

#include "ch.h"
#include "hal.h"

#if defined(STM32_DMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   DMA ISR redirector type.
 */
typedef struct {
  stm32_dmaisr_t        dmaisrfunc;
  void                  *dmaisrparam;
} dma_isr_redir_t;

static uint32_t dmamsk1;
static dma_isr_redir_t dma1[7];

#if STM32_HAS_DMA2
static uint32_t dmamsk2;
static dma_isr_redir_t dma2[5];
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   DMA1 channel 1 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch1_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA1->ISR >> (STM32_DMA_CHANNEL_1 * 4);
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_1);
  if (dma1[0].dmaisrfunc)
    dma1[0].dmaisrfunc(dma1[0].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 2 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch2_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA1->ISR >> (STM32_DMA_CHANNEL_2 * 4);
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_2);
  if (dma1[1].dmaisrfunc)
    dma1[1].dmaisrfunc(dma1[1].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 3 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch3_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA1->ISR >> (STM32_DMA_CHANNEL_3 * 4);
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_3);
  if (dma1[2].dmaisrfunc)
    dma1[2].dmaisrfunc(dma1[2].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 4 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch4_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA1->ISR >> (STM32_DMA_CHANNEL_4 * 4);
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_4);
  if (dma1[3].dmaisrfunc)
    dma1[3].dmaisrfunc(dma1[3].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 5 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch5_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA1->ISR >> (STM32_DMA_CHANNEL_5 * 4);
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_5);
  if (dma1[4].dmaisrfunc)
    dma1[4].dmaisrfunc(dma1[4].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 6 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch6_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA1->ISR >> (STM32_DMA_CHANNEL_6 * 4);
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_6);
  if (dma1[5].dmaisrfunc)
    dma1[5].dmaisrfunc(dma1[5].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 7 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch7_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA1->ISR >> (STM32_DMA_CHANNEL_7 * 4);
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_7);
  if (dma1[6].dmaisrfunc)
    dma1[6].dmaisrfunc(dma1[6].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

#if STM32_HAS_DMA2 || defined(__DOXYGEN__)
/**
 * @brief   DMA2 channel 1 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Ch1_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA2->ISR >> (STM32_DMA_CHANNEL_1 * 4);
  dmaClearChannel(STM32_DMA2, STM32_DMA_CHANNEL_1);
  if (dma2[0].dmaisrfunc)
    dma2[0].dmaisrfunc(dma2[0].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 channel 2 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Ch2_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA2->ISR >> (STM32_DMA_CHANNEL_2 * 4);
  dmaClearChannel(STM32_DMA2, STM32_DMA_CHANNEL_2);
  if (dma2[1].dmaisrfunc)
    dma2[1].dmaisrfunc(dma2[1].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 channel 3 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Ch3_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA2->ISR >> (STM32_DMA_CHANNEL_3 * 4);
  dmaClearChannel(STM32_DMA2, STM32_DMA_CHANNEL_3);
  if (dma2[2].dmaisrfunc)
    dma2[2].dmaisrfunc(dma2[2].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

#if defined(STM32F10X_CL) || defined(__DOXYGEN__)
/**
 * @brief   DMA2 channel 4 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Ch4_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA2->ISR >> (STM32_DMA_CHANNEL_4 * 4);
  dmaClearChannel(STM32_DMA2, STM32_DMA_CHANNEL_4);
  if (dma2[3].dmaisrfunc)
    dma2[3].dmaisrfunc(dma2[3].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 channel 5 shared interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Ch5_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = STM32_DMA2->ISR >> (STM32_DMA_CHANNEL_5 * 4);
  dmaClearChannel(STM32_DMA2, STM32_DMA_CHANNEL_5);
  if (dma2[4].dmaisrfunc)
    dma2[4].dmaisrfunc(dma2[4].dmaisrparam, isr);

  CH_IRQ_EPILOGUE();
}

#else /* !STM32F10X_CL */
/**
 * @brief   DMA2 channels 4 and 5 shared interrupt handler.
 * @note    This IRQ is shared between DMA2 channels 4 and 5 so it is a
 *          bit less efficient because an extra check.
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Ch4_5_IRQHandler) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  /* Check on channel 4.*/
  isr = STM32_DMA2->ISR >> (STM32_DMA_CHANNEL_5 * 4);
  if (isr & DMA_ISR_GIF1) {
    dmaClearChannel(STM32_DMA2, STM32_DMA_CHANNEL_5);
    if (dma2[3].dmaisrfunc)
      dma2[3].dmaisrfunc(dma2[3].dmaisrparam, isr);
  }

  /* Check on channel 5.*/
  isr = STM32_DMA2->ISR >> (STM32_DMA_CHANNEL_4 * 4);
  if (isr & DMA_ISR_GIF1) {
    dmaClearChannel(STM32_DMA2, STM32_DMA_CHANNEL_5);
    if (dma2[4].dmaisrfunc)
      dma2[4].dmaisrfunc(dma2[4].dmaisrparam, isr);
  }

  CH_IRQ_EPILOGUE();
}
#endif /* !STM32F10X_CL */
#endif /* STM32_HAS_DMA2 */

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

  dmamsk1 = 0;
  for (i = STM32_DMA_CHANNEL_7; i >= STM32_DMA_CHANNEL_1; i--) {
    dmaDisableChannel(STM32_DMA1, i);
    dma1[i].dmaisrfunc = NULL;
  }
  STM32_DMA1->IFCR = 0xFFFFFFFF;
#if STM32_HAS_DMA2
  dmamsk2 = 0;
  for (i = STM32_DMA_CHANNEL_5; i >= STM32_DMA_CHANNEL_1; i--) {
    dmaDisableChannel(STM32_DMA2, i);
    dma2[i].dmaisrfunc = NULL;
  }
  STM32_DMA1->IFCR = 0xFFFFFFFF;
#endif
}

/**
 * @brief   Allocates a DMA channel.
 * @details The channel is allocated and, if required, the DMA clock enabled.
 *          Trying to allocate a channel already allocated is an illegal
 *          operation and is trapped if assertions are enabled.
 * @pre     The channel must not be already in use.
 * @post    The channel is allocated and the default ISR handler redirected
 *          to the specified function.
 * @post    The channel must be freed using @p dmaRelease() before it can
 *          be reused with another peripheral.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dma       DMA controller id
 * @param[in] channel   requested channel id
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 *
 * @special
 */
void dmaAllocate(uint32_t dma, uint32_t channel,
                 stm32_dmaisr_t func, void *param) {

#if STM32_HAS_DMA2
  switch (dma) {
  case STM32_DMA1_ID:
#else
  (void)dma;
#endif
    /* Check if the channel is already taken.*/
    chDbgAssert((dmamsk1 & (1 << channel)) == 0,
                "dmaAllocate(), #1", "already allocated");

    /* If the DMA unit was idle then the clock is enabled.*/
    if (dmamsk1 == 0) {
      RCC->AHBENR |= RCC_AHBENR_DMA1EN;
      DMA1->IFCR = 0x0FFFFFFF;
    }

    dmamsk1 |= 1 << channel;
    dma1[channel].dmaisrfunc  = func;
    dma1[channel].dmaisrparam = param;
#if STM32_HAS_DMA2
    break;
  case STM32_DMA2_ID:
    /* Check if the channel is already taken.*/
    chDbgAssert((dmamsk2 & (1 << channel)) == 0,
                "dmaAllocate(), #2", "already allocated");

    /* If the DMA unit was idle then the clock is enabled.*/
    if (dmamsk2 == 0) {
      RCC->AHBENR |= RCC_AHBENR_DMA2EN;
      DMA2->IFCR = 0x0FFFFFFF;
    }

    dmamsk2 |= 1 << channel;
    dma2[channel].dmaisrfunc  = func;
    dma2[channel].dmaisrparam = param;
    break;
  }
#endif
}

/**
 * @brief   Releases a DMA channel.
 * @details The channel is freed and, if required, the DMA clock disabled.
 *          Trying to release a unallocated channel is an illegal operation
 *          and is trapped if assertions are enabled.
 * @pre     The channel must have been allocated using @p dmaRequest().
 * @post    The channel is again available.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dma       DMA controller id
 * @param[in] channel   requested channel id
 *
 * @special
 */
void dmaRelease(uint32_t dma, uint32_t channel) {

#if STM32_HAS_DMA2
  switch (dma) {
  case STM32_DMA1_ID:
#else
  (void)dma;
#endif
    /* Check if the channel is not taken.*/
    chDbgAssert((dmamsk1 & (1 << channel)) != 0,
                "dmaRelease(), #1", "not allocated");

    dma1[channel].dmaisrfunc = NULL;
    dmamsk1 &= ~(1 << channel);
    if (dmamsk1 == 0)
      RCC->AHBENR &= ~RCC_AHBENR_DMA1EN;
#if STM32_HAS_DMA2
    break;
  case STM32_DMA2_ID:
    /* Check if the channel is not taken.*/
    chDbgAssert((dmamsk2 & (1 << channel)) != 0,
                "dmaRelease(), #2", "not allocated");

    dma2[channel].dmaisrfunc = NULL;
    dmamsk2 &= ~(1 << channel);
    if (dmamsk2 == 0)
      RCC->AHBENR &= ~RCC_AHBENR_DMA2EN;
    break;
  }
#endif
}

#endif /* STM32_DMA_REQUIRED */

/** @} */
