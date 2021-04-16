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
 * @name    Special channel identifiers
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
 * @brief   RP DMA channel descriptor structure.
 */
typedef struct {
  DMA_TypeDef           *dma;           /**< @brief Associated DMA.         */
  DMA_Channel_Typedef   *channel;       /**< @brief Associated DMA channel. */
  uint32_t              chnidx;         /**< @brief Index to self in array. */
  uint32_t              chnmask;        /**< @brief Channel bit mask.       */
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
extern const rp_dma_channel_t __rp_dma_channels[RP_DMA_CHANNELS];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
  const rp_dma_channel_t *dmaChannelAllocI(uint32_t id,
                                          rp_dmaisr_t func,
                                          void *param);
  const rp_dma_channel_t *dmaChannelAlloc(uint32_t id,
                                         rp_dmaisr_t func,
                                         void *param);
  void dmaChannelFreeI(const rp_dma_channel_t *dmachp);
  void dmaChannelFree(const rp_dma_channel_t *dmachp);
  void dmaServeInterrupt(const rp_dma_channel_t *dmachp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Driver inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Setup of the source DMA pointer.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure
 * @param[in] addr      value to be written in the @p READ_ADDR register
 *
 * @special
 */
__STATIC_INLINE void dmaChannelSetSource(const rp_dma_channel_t *dmachp,
                                         uint32_t addr) {

  dmachp->channel->READ_ADDR = addr;
}

/**
 * @brief   Setup of the destination DMA pointer.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure
 * @param[in] addr      value to be written in the @p WRITE_ADDR register
 *
 * @special
 */
__STATIC_INLINE void dmaChannelSetDestination(const rp_dma_channel_t *dmachp,
                                              uint32_t addr) {

  dmachp->channel->WRITE_ADDR = addr;
}

/**
 * @brief   Setup of the DMA transfer counter.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure
 * @param[in] n         value to be written in the @p TRANS_COUNT register
 *
 * @special
 */
__STATIC_INLINE void dmaChannelSetCounter(const rp_dma_channel_t *dmachp,
                                          uint32_t n) {

  dmachp->channel->TRANS_COUNT = n;
}

/**
 * @brief   Setup of the DMA transfer mode without linking.
 * @note    The link field is enforced to "self" meaning no linking.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure
 * @param[in] mode      value to be written in the @p CTRL_TRIG register
 *                      except link field
 *
 * @special
 */
__STATIC_INLINE void dmaChannelSetMode(const rp_dma_channel_t *dmachp,
                                       uint32_t mode) {

  dmachp->channel->CTRL_TRIG = (mode & ~DMA_CTRL_TRIG_CHAIN_TO_Msk) |
                               DMA_CTRL_TRIG_CHAIN_TO(dmachp->chnidx);
}

/**
 * @brief   Enables a DMA channel.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure

 *
 * @special
 */
__STATIC_INLINE void dmaChannelEnable(const rp_dma_channel_t *dmachp) {

  dmachp->channel->CTRL_TRIG |= DMA_CTRL_TRIG_EN;
}

/**
 * @brief   Disables a DMA channel aborting the current transfer.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure

 *
 * @special
 */
__STATIC_INLINE void dmaChannelDisable(const rp_dma_channel_t *dmachp) {

  dmachp->dma->CHAN_ABORT |= dmachp->chnmask;
  while ((dmachp->dma->CHAN_ABORT & dmachp->chnmask) != 0U) {
  }
}

/**
 * @brief   Returns the channel busy state.
 *
 * @param[in] dmachp    pointer to a rp_dma_channel_t structure
 * @return              The channel busy state.
 * @retval false        if the channel is not busy.
 * @retval true         if the channel is busy.
 *
 * @special
 */
__STATIC_INLINE bool dmaChannelIsBusy(const rp_dma_channel_t *dmachp) {

  return (bool)((dmachp->channel->CTRL_TRIG & DMA_CTRL_TRIG_BUSY) != 0U);
}

#endif /* RP_DMA_H */

/** @} */
