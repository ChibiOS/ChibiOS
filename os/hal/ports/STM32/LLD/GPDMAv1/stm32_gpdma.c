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
 * @file    GPDMAv1/stm32_gpdma.c
 * @brief   GPDMA helper driver code.
 *
 * @addtogroup STM32_GPDMA
 * @details GPDMA sharing helper driver. In the STM32 the DMA channels are a
 *          shared resource, this driver allows to allocate and free DMA
 *          channels at runtime in order to allow all the other device
 *          drivers to coordinate the access to the resource.
 * @note    The DMA ISR handlers are all declared into this module because
 *          sharing, the various device drivers can associate a callback to
 *          ISRs when allocating channels.
 * @{
 */

#include "hal.h"

/* The following macro is only defined if some driver requiring GPDMA services
   has been enabled.*/
#if defined(STM32_GPDMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   GPDMA channels descriptors.
 * @details This table keeps the association between an unique channel
 *          identifier and the involved physical registers.
 * @note    Don't use this array directly, use the appropriate wrapper macros
 *          instead: @p STM32_DMA1_CHANNEL1, @p STM32_DMA1_CHANNEL2 etc.
 */
const stm32_gpdma_channel_t __stm32_gpdma_channels[STM32_GPDMA_CHANNELS] = {
#if STM32_GPDMA1_NUM_CHANNELS > 0
  {GPDMA1_Channel0},
#endif
#if STM32_GPDMA1_NUM_CHANNELS > 1
  {GPDMA1_Channel1},
#endif
#if STM32_GPDMA1_NUM_CHANNELS > 2
  {GPDMA1_Channel2},
#endif
#if STM32_GPDMA1_NUM_CHANNELS > 3
  {GPDMA1_Channel3},
#endif
#if STM32_GPDMA1_NUM_CHANNELS > 4
  {GPDMA1_Channel4},
#endif
#if STM32_GPDMA1_NUM_CHANNELS > 5
  {GPDMA1_Channel5},
#endif
#if STM32_GPDMA1_NUM_CHANNELS > 6
  {GPDMA1_Channel6},
#endif
#if STM32_GPDMA1_NUM_CHANNELS > 7
  {GPDMA1_Channel7},
#endif
#if STM32_GPDMA2_NUM_CHANNELS > 0
  {GPDMA2_Channel0},
#endif
#if STM32_GPDMA2_NUM_CHANNELS > 1
  {GPDMA2_Channel1},
#endif
#if STM32_GPDMA2_NUM_CHANNELS > 2
  {GPDMA2_Channel2},
#endif
#if STM32_GPDMA2_NUM_CHANNELS > 3
  {GPDMA2_Channel3},
#endif
#if STM32_GPDMA2_NUM_CHANNELS > 4
  {GPDMA2_Channel4},
#endif
#if STM32_GPDMA2_NUM_CHANNELS > 5
  {GPDMA2_Channel5},
#endif
#if STM32_GPDMA2_NUM_CHANNELS > 6
  {GPDMA2_Channel6},
#endif
#if STM32_GPDMA2_NUM_CHANNELS > 7
  {GPDMA2_Channel7},
#endif
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Global DMA-related data structures.
 */
static struct {
  /**
   * @brief   Mask of the allocated channels.
   */
  uint32_t              allocated_mask;
  /**
   * @brief   DMA IRQ redirectors.
   */
  struct {
    /**
     * @brief   DMA callback function.
     */
    stm32_gpdmaisr_t    func;
    /**
     * @brief   DMA callback parameter.
     */
    void                *param;
  } channels[STM32_GPDMA_CHANNELS];
} gpdma;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   STM32 DMA helper initialization.
 *
 * @init
 */
void dmaInit(void) {
  unsigned i;

  gpdma.allocated_mask = 0U;
  for (i = 0; i < STM32_GPDMA_CHANNELS; i++) {
    __stm32_gpdma_channels[i].channel->CCR = 0U;
    gpdma.channels[i].func = NULL;
  }
#if STM32_GPDMA2_NUM_CHANNELS > 0
#endif
}

/**
 * @brief   Allocates a DMA channel.
 * @details The channel is allocated and, if required, the DMA clock enabled.
 *          The function also enables the IRQ vector associated to the channel
 *          and initializes its priority.
 *
 * @param[in] cmask     channels mask where to search for an available chennel
 * @param[in] irqprio   IRQ priority for the DMA channel
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p stm32_dma_channel_t
 *                      structure.
 * @retval NULL         if a/the channel is not available.
 *
 * @iclass
 */
const stm32_gpdma_channel_t *dmaChannelAllocI(uint32_t cmask,
                                              uint32_t irqprio,
                                              stm32_gpdmaisr_t func,
                                              void *param) {
  unsigned i;
  uint32_t available;

  osalDbgCheckClassI();

  /* Mask of the available channels within the specified channels.*/
  available = gpdma.allocated_mask & cmask;

  /* Searching for a free channel.*/
  for (i = 0U; i <= STM32_GPDMA_CHANNELS; i++) {
    uint32_t mask = (uint32_t)(1U << i);
    if ((available & mask) == 0U) {
      /* Channel found.*/
      const stm32_dma_channel_t *dmachp = STM32_DMA_CHANNEL(i);

      /* Installs the DMA handler.*/
      dma.channels[i].func  = func;
      dma.channels[i].param = param;
      dma.allocated_mask  |= mask;

      /* Enabling DMA clocks required by the current channels set.*/
      if ((STM32_DMA1_CHANNELS_MASK & mask) != 0U) {
        rccEnableDMA1(true);
      }
#if STM32_GPDMA2_NUM_CHANNELS > 0
      if ((STM32_DMA2_CHANNELS_MASK & mask) != 0U) {
        rccEnableDMA2(true);
      }
#endif

#if (STM32_DMA_SUPPORTS_DMAMUX == TRUE) && defined(rccEnableDMAMUX)
      /* Enabling DMAMUX if present.*/
      if (dma.allocated_mask != 0U) {
        rccEnableDMAMUX(true);
      }
#endif

      /* Enables the associated IRQ vector if not already enabled and if a
         callback is defined.*/
      if (func != NULL) {
        if ((dma.isr_mask & dmachp->cmask) == 0U) {
          nvicEnableVector(dmachp->vector, priority);
        }
        dma.isr_mask |= mask;
      }

      /* Putting the channel in a known state.*/
      dmaStreamDisable(dmachp);
      dmachp->channel->CCR = STM32_DMA_CCR_RESET_VALUE;

      return dmachp;
    }
  }

  return NULL;
}

/**
 * @brief   Allocates a DMA channel.
 * @details The channel is allocated and, if required, the DMA clock enabled.
 *          The function also enables the IRQ vector associated to the channel
 *          and initializes its priority.
 *
 * @param[in] cmask     channels mask where to search for an available chennel
 * @param[in] irqprio   IRQ priority for the DMA channel
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p stm32_dma_channel_t
 *                      structure.
 * @retval NULL         if a/the channel is not available.
 *
 * @api
 */
const stm32_dma_channel_t *dmaStreamAlloc(uint32_t id,
                                         uint32_t priority,
                                         stm32_dmaisr_t func,
                                         void *param) {
  const stm32_dma_channel_t *dmachp;

  osalSysLock();
  dmachp = dmaStreamAllocI(id, priority, func, param);
  osalSysUnlock();

  return dmachp;
}

/**
 * @brief   Releases a DMA channel.
 * @details The channel is freed and, if required, the DMA clock disabled.
 *          Trying to release a unallocated channel is an illegal operation
 *          and is trapped if assertions are enabled.
 *
 * @param[in] dmachp    pointer to a stm32_dma_channel_t structure
 *
 * @iclass
 */
void dmaStreamFreeI(const stm32_dma_channel_t *dmachp) {
  uint32_t selfindex = (uint32_t)dmachp->selfindex;

  osalDbgCheck(dmachp != NULL);

  /* Check if the channels is not taken.*/
  osalDbgAssert((dma.allocated_mask & (1 << selfindex)) != 0U,
                "not allocated");

  /* Marks the channel as not allocated.*/
  dma.allocated_mask &= ~(1U << selfindex);
  dma.isr_mask &= ~(1U << selfindex);

  /* Disables the associated IRQ vector if it is no more in use.*/
  if ((dma.isr_mask & dmachp->cmask) == 0U) {
    nvicDisableVector(dmachp->vector);
  }

  /* Removes the DMA handler.*/
  dma.channels[selfindex].func  = NULL;
  dma.channels[selfindex].param = NULL;

  /* Shutting down clocks that are no more required, if any.*/
  if ((dma.allocated_mask & STM32_DMA1_CHANNELS_MASK) == 0U) {
    rccDisableDMA1();
  }
#if STM32_GPDMA2_NUM_CHANNELS > 0
  if ((dma.allocated_mask & STM32_DMA2_CHANNELS_MASK) == 0U) {
    rccDisableDMA2();
  }
#endif

#if (STM32_DMA_SUPPORTS_DMAMUX == TRUE) && defined(rccDisableDMAMUX)
  /* Shutting down DMAMUX if present.*/
  if (dma.allocated_mask == 0U) {
    rccDisableDMAMUX();
  }
#endif
}

/**
 * @brief   Releases a DMA channel.
 * @details The channel is freed and, if required, the DMA clock disabled.
 *          Trying to release a unallocated channel is an illegal operation
 *          and is trapped if assertions are enabled.
 *
 * @param[in] dmachp    pointer to a stm32_dma_channel_t structure
 *
 * @api
 */
void dmaStreamFree(const stm32_dma_channel_t *dmachp) {

  osalSysLock();
  dmaStreamFreeI(dmachp);
  osalSysUnlock();
}

/**
 * @brief   Serves a DMA IRQ.
 *
 * @param[in] dmachp    pointer to a stm32_dma_channel_t structure
 *
 * @special
 */
void dmaServeInterrupt(const stm32_dma_channel_t *dmachp) {
  uint32_t flags;
  uint32_t selfindex = (uint32_t)dmachp->selfindex;

  flags = (dmachp->dma->ISR >> dmachp->shift) & STM32_DMA_ISR_MASK;
  if (flags & dmachp->channel->CCR) {
    dmachp->dma->IFCR = flags << dmachp->shift;
    if (dma.channels[selfindex].func) {
      dma.channels[selfindex].func(dma.channels[selfindex].param, flags);
    }
  }
}

#endif /* defined(STM32_GPDMA_REQUIRED) */

/** @} */
