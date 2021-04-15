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
 * @file    DMAv1/rp_dma.c
 * @brief   DMA helper driver code.
 *
 * @addtogroup RP_DMA
 * @details DMA sharing helper driver. In RP2 the DMA channels are a
 *          shared resource, this driver allows to allocate and free DMA
 *          channels at runtime in order to allow all the other device
 *          drivers to coordinate the access to the resource.
 * @{
 */

#include "hal.h"

/* The following macro is only defined if some driver requiring DMA services
   has been enabled.*/
#if defined(RP_DMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

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
 * @brief   DMA helper initialization.
 *
 * @init
 */
void dmaInit(void) {
}

/**
 * @brief   Allocates a DMA channel.
 *
 * @param[in] id        numeric identifiers of a specific channel or:
 *                      - @p RP_DMA_CHANNEL_ID_ANY for any channel.
 *                      .
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p rp_dma_channel_t
 *                      structure.
 * @retval NULL         if a/the channel is not available.
 *
 * @iclass
 */
const rp_dma_channel_t *dmaStreamAllocI(uint32_t id,
                                        rp_dmaisr_t func,
                                        void *param) {
}

/**
 * @brief   Allocates a DMA channel.
 * @details The channel is allocated and, if required, the DMA clock enabled.
 *          The function also enables the IRQ vector associated to the channel
 *          and initializes its priority.
 *
 * @param[in] id        numeric identifiers of a specific channel or:
 *                      - @p RP_DMA_CHANNEL_ID_ANY for any channel.
 *                      .
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p rp_dma_channel_t
 *                      structure.
 * @retval NULL         if a/the channel is not available.
 *
 * @api
 */
const rp_dma_channel_t *dmaStreamAlloc(uint32_t id,
                                         uint32_t priority,
                                         rp_dmaisr_t func,
                                         void *param) {
}

/**
 * @brief   Releases a DMA channel.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure
 *
 * @iclass
 */
void dmaStreamFreeI(const rp_dma_channel_t *dmachp) {
}

/**
 * @brief   Releases a DMA channel.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure
 *
 * @api
 */
void dmaStreamFree(const rp_dma_channel_t *dmachp) {
}

/**
 * @brief   Serves a DMA IRQ.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure
 *
 * @special
 */
void dmaServeInterrupt(const rp_dma_channel_t *dmachp) {
}

#endif /* RP_DMA_REQUIRED */

/** @} */
