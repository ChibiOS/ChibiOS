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
 * @file    MDMAv1/stm32_mdma.c
 * @brief   MDMA helper driver code.
 *
 * @addtogroup STM32_MDMA
 * @details MDMA sharing helper driver. In the STM32 the MDMA channels are a
 *          shared resource, this driver allows to allocate and free MDMA
 *          STM32 at runtime in order to allow all the other device
 *          drivers to coordinate the access to the resource.
 * @note    The MDMA ISR handlers are all declared into this module because
 *          sharing, the various device drivers can associate a callback to
 *          ISRs when allocating channels.
 * @{
 */

#include "hal.h"

/* The following macro is only defined if some driver requiring MDMA services
   has been enabled.*/
#if defined(STM32_MDMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   MDMA channels descriptors.
 * @details This table keeps the association between an unique channel
 *          identifier and the involved physical registers.
 * @note    Don't use this array directly, use the appropriate wrapper macros
 *          instead: @p STM32_MDMA_CHANNEL0, @p STM32_MDMA_CHANNEL1 etc.
 */
const stm32_mdma_channel_t __stm32_mdma_channels[STM32_MDMA_CHANNELS] = {
  {MDMA_Channel0,  0,  STM32_MDMA_CH0_NUMBER},
  {MDMA_Channel1,  1,  STM32_MDMA_CH1_NUMBER},
  {MDMA_Channel2,  2,  STM32_MDMA_CH2_NUMBER},
  {MDMA_Channel3,  3,  STM32_MDMA_CH3_NUMBER},
  {MDMA_Channel4,  4,  STM32_MDMA_CH4_NUMBER},
  {MDMA_Channel5,  5,  STM32_MDMA_CH5_NUMBER},
  {MDMA_Channel6,  6,  STM32_MDMA_CH6_NUMBER},
  {MDMA_Channel7,  7,  STM32_MDMA_CH7_NUMBER},
  {MDMA_Channel8,  8,  STM32_MDMA_CH8_NUMBER},
  {MDMA_Channel9,  9,  STM32_MDMA_CH9_NUMBER},
  {MDMA_Channel10, 10, STM32_MDMA_CH10_NUMBER},
  {MDMA_Channel11, 11, STM32_MDMA_CH11_NUMBER},
  {MDMA_Channel12, 12, STM32_MDMA_CH12_NUMBER},
  {MDMA_Channel13, 13, STM32_MDMA_CH13_NUMBER},
  {MDMA_Channel14, 14, STM32_MDMA_CH14_NUMBER},
  {MDMA_Channel15, 15, STM32_MDMA_CH15_NUMBER},
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Global MDMA-related data structures.
 */
static struct {
  /**
   * @brief   Mask of the allocated channels.
   */
  uint32_t          allocated_mask;
  /**
   * @brief   MDMA IRQ redirectors.
   */
  struct {
    /**
     * @brief   MDMA callback function.
     */
    stm32_mdmaisr_t func;
    /**
     * @brief   MDMA callback parameter.
     */
    void            *param;
  } channels[STM32_MDMA_CHANNELS];
} mdma;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void mdma_serve_interrupt(const stm32_mdma_channel_t *mdmachp) {
  uint32_t idx = mdmachp->selfindex;
  uint32_t flags = mdmachp->channel->CISR;
  mdmachp->channel->CIFCR = flags;
  if (mdma.channels[idx].func != NULL) {
    mdma.channels[idx].func(mdma.channels[idx].param, flags);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   MDMA channel 0 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH0_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL0);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 1 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL1);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 2 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL2);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 3 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH3_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL3);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 4 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH4_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL4);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 5 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH5_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL5);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 6 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH6_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL6);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 7 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH7_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL7);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 8 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH8_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL8);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 9 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH9_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL9);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 10 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH10_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL10);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 11 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH11_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL11);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 12 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH12_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL12);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 13 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH13_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL13);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 14 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH14_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL14);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   MDMA channel 15 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_MDMA_CH15_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  mdma_serve_interrupt(STM32_MDMA_CHANNEL15);

  OSAL_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   STM32 MDMA helper initialization.
 *
 * @init
 */
void mdmaInit(void) {
  unsigned i;

  mdma.allocated_mask = 0U;
  for (i = 0U; i < STM32_MDMA_CHANNELS; i++) {
    mdma.channels[i].func = NULL;
    __stm32_dma_channels[i].channel->CCR   = STM32_MDMA_CCR_RESET_VALUE;
    __stm32_dma_channels[i].channel->CTCR  = STM32_MDMA_CTCR_RESET_VALUE;
    __stm32_dma_channels[i].channel->CIFCR = STM32_MDMA_CIFCR_CTEIF  |
                                             STM32_MDMA_CIFCR_CBRTIF |
                                             STM32_MDMA_CIFCR_CBRTIF |
                                             STM32_MDMA_CIFCR_CCTCIF |
                                             STM32_MDMA_CIFCR_CTEIF;
  }
}

/**
 * @brief   Allocates an MDMA channel.
 * @details The channel is allocated and, if required, the MDMA clock enabled.
 *          The function also enables the IRQ vector associated to the channel
 *          and initializes its priority.
 *
 * @param[in] id        numeric identifiers of a specific channel or:
 *                      - @p STM32_MDMA_CHANNEL_ID_ANY for any channel.
 *                      .
 * @param[in] priority  IRQ priority for the MDMA channel
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p stm32_mdma_channel_t
 *                      structure.
 * @retval NULL         if a/the channel is not available.
 *
 * @iclass
 */
const stm32_mdma_channel_t *dmaChannelAllocI(uint32_t id,
                                             uint32_t priority,
                                             stm32_mdmaisr_t func,
                                             void *param) {
  uint32_t i, startid, endid;

  osalDbgCheckClassI();

  if (id < STM32_MDMA_CHANNELS) {
    startid = id;
    endid   = id;
  }
  else if (id == STM32_MDMA_CHANNEL_ID_ANY) {
    startid = 0U;
    endid   = STM32_MDMA_CHANNELS - 1U;
  }
  else {
    osalDbgCheck(false);
    return NULL;
  }

  for (i = startid; i <= endid; i++) {
    uint32_t mask = (1U << i);
    if ((mdma.allocated_mask & mask) == 0U) {
      const stm32_mdma_channel_t *mdmachp = STM32_MDMA_CHANNEL(i);

      /* Installs the MDMA handler.*/
      mdma.channels[i].func  = func;
      mdma.channels[i].param = param;
      mdma.allocated_mask   |= mask;

      /* Enabling MDMA clocks required by the current channels set.*/
      if (mdma.allocated_mask != 0U) {
        rccEnableMDMA(true);
      }

      /* Enables the associated IRQ vector if a callback is defined.*/
      if (func != NULL) {
        nvicEnableVector(mdmachp->vector, priority);
      }

      return mdmachp;
    }
  }

  return NULL;
}

/**
 * @brief   Allocates a MDMA channel.
 * @details The channel is allocated and, if required, the MDMA clock enabled.
 *          The function also enables the IRQ vector associated to the channel
 *          and initializes its priority.
 *
 * @param[in] id        numeric identifiers of a specific channel or:
 *                      - @p STM32_MDMA_CHANNEL_ID_ANY for any channel.
 *                      .
 * @param[in] priority  IRQ priority for the MDMA channel
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p stm32_mdma_channel_t
 *                      structure.
 * @retval NULL         if a/the channel is not available.
 *
 * @api
 */
const stm32_mdma_channel_t *dmaChannelAlloc(uint32_t id,
                                            uint32_t priority,
                                            stm32_mdmaisr_t func,
                                            void *param) {
  const stm32_mdma_channel_t *mdmachp;

  osalSysLock();
  mdmachp = mdmaChannelAllocI(id, priority, func, param);
  osalSysUnlock();

  return mdmachp;
}

/**
 * @brief   Releases a MDMA channel.
 * @details The channel is freed and, if required, the MDMA clock disabled.
 *          Trying to release a unallocated channel is an illegal operation
 *          and is trapped if assertions are enabled.
 *
 * @param[in] mdmachp   pointer to a stm32_mdma_channel_t structure
 *
 * @iclass
 */
void mdmaChannelFreeI(const stm32_mdma_channel_t *mdmachp) {

  osalDbgCheck(mdmachp != NULL);

  /* Check if the channels is not taken.*/
  osalDbgAssert((dma.allocated_mask & (1U << mmdmachp->selfindex)) != 0U,
                "not allocated");

  /* Disables the associated IRQ vector.*/
  nvicDisableVector(mdmachp->vector);

  /* Marks the channel as not allocated.*/
  mdma.allocated_mask &= ~(1U << mmdmachp->selfindex);

  /* Shutting down clocks that are no more required, if any.*/
  if (dma.allocated_mask == 0U) {
    rccDisableMDMA();
  }
}

/**
 * @brief   Releases a MDMA channel.
 * @details The channel is freed and, if required, the MDMA clock disabled.
 *          Trying to release a unallocated channel is an illegal operation
 *          and is trapped if assertions are enabled.
 *
 * @param[in] mdmachp   pointer to a stm32_mdma_channel_t structure
 *
 * @api
 */
void mdmaChannelFree(const stm32_mdma_channel_t *mdmachp) {

  osalSysLock();
  mdmaChannelFreeI(mdmachp);
  osalSysUnlock();
}

/**
 * @brief   MDMA stream disable.
 * @details The function disables the specified stream, waits for the disable
 *          operation to complete and then clears any pending interrupt.
 * @pre     The stream must have been allocated using @p mdmaChannelAlloc().
 * @post    After use the stream can be released using @p mdmaChannelFree().
 *
 * @param[in] mdmachp   pointer to a stm32_mdma_channel_t structure
 *
 * @xclass
 */
void mdmaStreamDisableX(const stm32_mdma_channel_t *mdmachp) {
  uint32_t ccr = mdmachp->channel->CCR;

  /* Clearing CCR regardless of previous state.*/
  mdmachp->channel->CCR &= ~(STM32_MDMA_CCR_TCIE  | STM32_MDMA_CCR_BTIE  |
                             STM32_MDMA_CCR_BRTIE | STM32_MDMA_CCR_CTCIE |
                             STM32_MDMA_CCR_TEIE  | STM32_MDMA_CCR_EN);

  /* If the channel was enabled then waiting for ongoing operations
     to finish.*/
  if ((ccr & STM32_MDMA_CCR_EN) != 0U) {
    while (((mdmachp)->channel->CISR & STM32_MDMA_CISR_CTCIF) == 0U)
      ;
  }

  /* Clearing IRQ sources.*/
  mdmaStreamClearInterruptX(mdmachp);
}

#endif /* defined(STM32_MDMA_REQUIRED) */

/** @} */
