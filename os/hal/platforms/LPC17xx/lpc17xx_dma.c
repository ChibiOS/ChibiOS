/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC17xx DMA driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC17xx/LPC17xx_dma.c
 * @brief   DMA driver code.
 *
 * @addtogroup LPC17xx_DMA
 * @details DMA sharing helper driver. In the LPC17xx the DMA streams are a
 *          shared resource, this driver allows to allocate and free DMA
 *          streams at runtime in order to allow all the other device
 *          drivers to coordinate the access to the resource.
 * @note    The DMA ISR handlers are all declared into this module because
 *          sharing, the various device drivers can associate a callback to
 *          ISRs when allocating streams.
 * @{
 */

#include "ch.h"
#include "hal.h"

/* The following macro is only defined if some driver requiring DMA services
   has been enabled.*/
#if defined(LPC17xx_DMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/


/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

  LPC_GPDMACH_TypeDef * \
  _lpc17xx_dma_channel_config_t[LPC17xx_DMA_CHANNELS] = {
  LPC_GPDMACH0, LPC_GPDMACH1, LPC_GPDMACH2, LPC_GPDMACH3, LPC_GPDMACH4,
  LPC_GPDMACH5, LPC_GPDMACH6, LPC_GPDMACH7 };

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/


/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   DMA ISR redirector type.
 */
typedef struct {
  lpc17xx_dmaisr_t  dma_func;       /**< @brief DMA callback function.  */
  void              *dma_param;     /**< @brief DMA callback parameter. */
} dma_isr_redir_t;

/**
 * @brief   Mask of the allocated streams.
 */
static uint32_t dma_streams_mask;

/**
 * @brief   DMA IRQ redirectors.
 */
static dma_isr_redir_t dma_isr_redir[LPC17xx_DMA_CHANNELS];

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   DMA interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(VectorA8) {
  uint32_t irq_status;
  uint32_t err_status;
  uint8_t i;

  CH_IRQ_PROLOGUE();

  irq_status = LPC_GPDMA->IntTCStat;
  LPC_GPDMA->IntTCClear = irq_status;  /* Clear DMA interrupt flag */
  err_status = LPC_GPDMA->IntErrStat;
  LPC_GPDMA->IntErrClr = err_status;   /* Clear DMA error flag if any*/

  for (i = 0; i < LPC17xx_DMA_CHANNELS; i++) {
    if (irq_status & (1UL << i)) {
      if (dma_isr_redir[i].dma_func)
        dma_isr_redir[i].dma_func(dma_isr_redir[i].dma_param, err_status);
    }
  }


  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   LPC17xx DMA initialization.
 *
 * @init
 */
void dmaInit(void) {
  uint8_t i;

  LPC_SC->PCONP |= (1UL << 29);                   /* Enable DMA power */

                                                  /* Disable all channels */
  for (i = 0; i < LPC17xx_DMA_CHANNELS; i++)
    _lpc17xx_dma_channel_config_t[i]->CConfig = 0;

  LPC_GPDMA->IntTCClear = 0xFF;
  LPC_GPDMA->IntErrClr = 0xFF;

  LPC_GPDMA->Config = DMACCONFIG_E;               /* Enable DMA Controller */
  while((LPC_GPDMA->Config & DMACCONFIG_E) != 0x01)
    ;

  nvicEnableVector(DMA_IRQn, CORTEX_PRIORITY_MASK(LPC17xx_DMA_IRQ_PRIORITY));
}

/**
 * @brief   Allocates a DMA channel.
 * @details The channel is allocated.
 * @pre     The channel must not be already in use or an error is returned.
 * @post    The channel is allocated and the default ISR handler redirected
 *          to the specified function.
 * @post    The channel must be freed using @p dmaChannelRelease() before it can
 *          be reused with another peripheral.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmach     DMA channel number
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              The operation status.
 * @retval FALSE        no error, stream taken.
 * @retval TRUE         error, stream already taken.
 *
 * @special
 */
bool_t dmaChannelAllocate(lpc17xx_dma_channel_t dmach,
                         lpc17xx_dmaisr_t func,
                         void *param) {
  uint32_t channel;
  channel = (1UL << dmach);

  /* Checks if the channel is already taken.*/
  if ((dma_streams_mask & channel) != 0)
    return TRUE;

  /* Marks the stream as allocated.*/
  dma_isr_redir[dmach].dma_func  = func;
  dma_isr_redir[dmach].dma_param = param;
  dma_streams_mask |= channel;

  return FALSE;
}

/**
 * @brief   Releases a DMA channel.
 * @details The channel is freed.
 *          Trying to release a unallocated channel is an illegal operation
 *          and is trapped if assertions are enabled.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    The channel is again available.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmach     DMA channel number
 *
 * @special
 */
void dmaChannelRelease(lpc17xx_dma_channel_t dmach) {

  uint32_t channel;
  channel = (1UL << dmach);

  /* Check if the streams is not taken.*/
  chDbgAssert((dma_streams_mask & channel) != 0,
                "dmaStreamRelease(), #1", "not allocated");

  dma_streams_mask &= ~channel; /* Marks the stream as not allocated.*/
}

#endif /* LPC17xx_DMA_REQUIRED */

/** @} */
