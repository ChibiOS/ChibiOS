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

/* The following macro is only defined if some driver requiring DMA services
   has been enabled.*/
#if defined(STM32_DMA_REQUIRED) || defined(__DOXYGEN__)

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
const stm32_dma_channel_t _stm32_dma_channels[STM32_GPDMA_CHANNELS] = {
#if STM32_DMA1_NUM_CHANNELS > 0
  {DMA1, DMA1_Channel1, STM32_DMA1_CH1_CMASK, DMA1_CH1_VARIANT,  0, 0, STM32_DMA1_CH1_NUMBER},
#endif
#if STM32_DMA1_NUM_CHANNELS > 1
  {DMA1, DMA1_Channel2, STM32_DMA1_CH2_CMASK, DMA1_CH2_VARIANT,  4, 1, STM32_DMA1_CH2_NUMBER},
#endif
#if STM32_DMA1_NUM_CHANNELS > 2
  {DMA1, DMA1_Channel3, STM32_DMA1_CH3_CMASK, DMA1_CH3_VARIANT,  8, 2, STM32_DMA1_CH3_NUMBER},
#endif
#if STM32_DMA1_NUM_CHANNELS > 3
  {DMA1, DMA1_Channel4, STM32_DMA1_CH4_CMASK, DMA1_CH4_VARIANT, 12, 3, STM32_DMA1_CH4_NUMBER},
#endif
#if STM32_DMA1_NUM_CHANNELS > 4
  {DMA1, DMA1_Channel5, STM32_DMA1_CH5_CMASK, DMA1_CH5_VARIANT, 16, 4, STM32_DMA1_CH5_NUMBER},
#endif
#if STM32_DMA1_NUM_CHANNELS > 5
  {DMA1, DMA1_Channel6, STM32_DMA1_CH6_CMASK, DMA1_CH6_VARIANT, 20, 5, STM32_DMA1_CH6_NUMBER},
#endif
#if STM32_DMA1_NUM_CHANNELS > 6
  {DMA1, DMA1_Channel7, STM32_DMA1_CH7_CMASK, DMA1_CH7_VARIANT, 24, 6, STM32_DMA1_CH7_NUMBER},
#endif
#if STM32_DMA1_NUM_CHANNELS > 7
  {DMA1, DMA1_Channel8, STM32_DMA1_CH8_CMASK, DMA1_CH8_VARIANT, 28, 7, STM32_DMA1_CH8_NUMBER},
#endif
#if STM32_DMA2_NUM_CHANNELS > 0
  {DMA2, DMA2_Channel1, STM32_DMA2_CH1_CMASK, DMA2_CH1_VARIANT,  0, 0 + STM32_DMA1_NUM_CHANNELS, STM32_DMA2_CH1_NUMBER},
#endif
#if STM32_DMA2_NUM_CHANNELS > 1
  {DMA2, DMA2_Channel2, STM32_DMA2_CH2_CMASK, DMA2_CH2_VARIANT,  4, 1 + STM32_DMA1_NUM_CHANNELS, STM32_DMA2_CH2_NUMBER},
#endif
#if STM32_DMA2_NUM_CHANNELS > 2
  {DMA2, DMA2_Channel3, STM32_DMA2_CH3_CMASK, DMA2_CH3_VARIANT,  8, 2 + STM32_DMA1_NUM_CHANNELS, STM32_DMA2_CH3_NUMBER},
#endif
#if STM32_DMA2_NUM_CHANNELS > 3
  {DMA2, DMA2_Channel4, STM32_DMA2_CH4_CMASK, DMA2_CH4_VARIANT, 12, 3 + STM32_DMA1_NUM_CHANNELS, STM32_DMA2_CH4_NUMBER},
#endif
#if STM32_DMA2_NUM_CHANNELS > 4
  {DMA2, DMA2_Channel5, STM32_DMA2_CH5_CMASK, DMA2_CH5_VARIANT, 16, 4 + STM32_DMA1_NUM_CHANNELS, STM32_DMA2_CH5_NUMBER},
#endif
#if STM32_DMA2_NUM_CHANNELS > 5
  {DMA2, DMA2_Channel6, STM32_DMA2_CH6_CMASK, DMA2_CH6_VARIANT, 20, 5 + STM32_DMA1_NUM_CHANNELS, STM32_DMA2_CH6_NUMBER},
#endif
#if STM32_DMA2_NUM_CHANNELS > 6
  {DMA2, DMA2_Channel7, STM32_DMA2_CH7_CMASK, DMA2_CH7_VARIANT, 24, 6 + STM32_DMA1_NUM_CHANNELS, STM32_DMA2_CH7_NUMBER},
#endif
#if STM32_DMA2_NUM_CHANNELS > 7
  {DMA2, DMA2_Channel8, STM32_DMA2_CH8_CMASK, DMA2_CH8_VARIANT, 28, 7 + STM32_DMA1_NUM_CHANNELS, STM32_DMA2_CH8_NUMBER},
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
  uint32_t          allocated_mask;
  /**
   * @brief   Mask of the enabled channels ISRs.
   */
  uint32_t          isr_mask;
  /**
   * @brief   DMA IRQ redirectors.
   */
  struct {
    /**
     * @brief   DMA callback function.
     */
    stm32_dmaisr_t    func;
    /**
     * @brief   DMA callback parameter.
     */
    void              *param;
  } channels[STM32_GPDMA_CHANNELS];
} dma;

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
  int i;

  dma.allocated_mask = 0U;
  dma.isr_mask       = 0U;
  for (i = 0; i < STM32_DMA_CHANNELS; i++) {
    _stm32_dma_channels[i].channel->CCR = STM32_DMA_CCR_RESET_VALUE;
    dma.channels[i].func = NULL;
  }
  DMA1->IFCR = 0xFFFFFFFFU;
#if STM32_DMA2_NUM_CHANNELS > 0
  DMA2->IFCR = 0xFFFFFFFFU;
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
const stm32_dma_channel_t *dmaStreamAllocI(uint32_t id,
                                          uint32_t priority,
                                          stm32_dmaisr_t func,
                                          void *param) {
  uint32_t i, startid, endid;

  osalDbgCheckClassI();

  if (id < STM32_DMA_CHANNELS) {
    startid = id;
    endid   = id;
  }
#if STM32_DMA_SUPPORTS_DMAMUX == TRUE
  else if (id == STM32_DMA_CHANNEL_ID_ANY) {
    startid = 0U;
    endid   = STM32_DMA_CHANNELS - 1U;
  }
  else if (id == STM32_DMA_CHANNEL_ID_ANY_DMA1) {
    startid = 0U;
    endid   = STM32_DMA1_NUM_CHANNELS - 1U;
  }
#if STM32_DMA2_NUM_CHANNELS > 0
  else if (id == STM32_DMA_CHANNEL_ID_ANY_DMA2) {
    startid = STM32_DMA1_NUM_CHANNELS;
    endid   = STM32_DMA_CHANNELS - 1U;
  }
#endif
#endif
  else {
    osalDbgCheck(false);
    return NULL;
  }

  for (i = startid; i <= endid; i++) {
    uint32_t mask = (1U << i);
    if ((dma.allocated_mask & mask) == 0U) {
      const stm32_dma_channel_t *dmachp = STM32_DMA_CHANNEL(i);

      /* Installs the DMA handler.*/
      dma.channels[i].func  = func;
      dma.channels[i].param = param;
      dma.allocated_mask  |= mask;

      /* Enabling DMA clocks required by the current channels set.*/
      if ((STM32_DMA1_CHANNELS_MASK & mask) != 0U) {
        rccEnableDMA1(true);
      }
#if STM32_DMA2_NUM_CHANNELS > 0
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
#if STM32_DMA2_NUM_CHANNELS > 0
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

/** @} */
