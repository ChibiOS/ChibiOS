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
 * @file    DMAv1/rp_dma.h
 * @brief   DMA helper driver header.
 *
 * @addtogroup RP_DMA
 * @{
 */

#ifndef RP_DMA_H
#define RP_DMA_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Total number of DMA channels.
 */
#define RP_DMA_CHANNELS                 12U

/**
 * @brief   Checks if a DMA channels id is within the valid range.
 *
 * @param[in] id        DMA channels id
 * @retval              The check result.
 * @retval false        invalid DMA channel.
 * @retval true         correct DMA channel.
 */
#define RP_DMA_IS_VALID_CHANNEL(chn)    (((chn) >= 0U) &&                   \
                                         ((id) <= (RP_DMA_CHANNELS + 1U)))

/**
 * @name    Special stream identifiers
 * @{
 */
#define RP_DMA_STREAM_ID_ANY            RP_DMA_CHANNELS
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
 * @brief   Type of a DMA callback.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] flags     pre-shifted content of the ISR register, the bits
 *                      are aligned to bit zero
 */
typedef void (*rp_dmaisr_t)(void *p, uint32_t flags);

/**
 * @brief   RP DMA stream descriptor structure.
 */
typedef struct {
  DMA_TypeDef           *dma;           /**< @brief Associated DMA.         */
} rp_dma_channel_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/** @} */

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
  const rp_dma_channel_t *dmaStreamAllocI(uint32_t id,
                                          rp_dmaisr_t func,
                                          void *param);
  const rp_dma_channel_t *dmaStreamAlloc(uint32_t id,
                                         rp_dmaisr_t func,
                                         void *param);
  void dmaStreamFreeI(const rp_dma_channel_t *dmachp);
  void dmaStreamFree(const rp_dma_channel_t *dmachp);
  void dmaServeInterrupt(const rp_dma_channel_t *dmachp);
#ifdef __cplusplus
}
#endif

#endif /* RP_DMA_H */

/** @} */
