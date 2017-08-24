/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    DMAv1/sama_xdmac.h
 * @brief   Enhanced-DMA helper driver header.
 *
 * @addtogroup SAMA_XDMAC
 * @{
 */

#ifndef SAMA_XDMAC_H
#define SAMA_XDMAC_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   DMA capability.
 * @details if @p TRUE then the DMA is able of burst transfers, FIFOs,
 *          scatter gather and other advanced features.
 */
#define SAMA_XDMAC_ADVANCED         TRUE

/**
 * @brief   Total number of DMA controllers.
 * @details This is the total number of DMA controllers.
 */
#define XDMAC_CONTROLLERS           2

/**
 * @brief   Number of DMA channels.
 * @details This is the number of DMA channel for each controller.
 */
#define XDMAC_CHANNELS              (XDMACCHID_NUMBER)

/**
 * @brief   Total number of DMA channels.
 * @details This is the total number of channels among all the DMA units.
 */
#define XDMAC_CHANNELS_TOT          (XDMACCHID_NUMBER * XDMAC_CONTROLLERS)

/**
 * @brief   Max single transfer size.
 * @details This is the maximum single transfer size supported.
 */
#define XDMAC_MAX_BT_SIZE           0xFFFFFF

/**
 * @brief   Max DMA length of the block.
 * @details This is the maximum length of the block supported.
 */
#define XDMAC_MAX_BLOCK_LEN         0xFFF

/**
 * @brief   States of the channel.
 */
#define SAMA_DMA_FREE              0U
#define SAMA_DMA_NOT_FREE          1U

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
 * @brief   SAMA5D2 DMA ISR function type.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] flags     content of the CIS register.
 */
typedef void (*sama_dmaisr_t)(void *p, uint32_t flags);

/**
 * @brief   SAMA5D2 DMA channel descriptor structure.
 */
typedef struct {
  Xdmac                 *xdmac;     /**< @brief Associated DMA
                                              controller.                    */
  uint8_t               chid;       /**< @brief ID channel                   */
  uint8_t               state;      /**< @brief State channel                */
  sama_dmaisr_t         dma_func;
  void                  *dma_param;
} sama_dma_channel_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/
/**
 * @name    Macro Functions
 * @{
 */

/**
 * @brief   Associates a source to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmachp     pointer to a sama_dma_channel_t structure
 * @param[in] addr       value to be written in the SA register
 *
 * @special
 */
#define dmaChannelSetSource(dmachp, addr) {                                             \
  (dmachp)->xdmac->XDMAC_CHID[(dmachp)->chid].XDMAC_CSA = XDMAC_CSA_SA((uint32_t)addr); \
}

/**
 * @brief   Associates a destination to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmachp    pointer to a sama_dma_channel_t structure
 * @param[in] addr      value to be written in the DA register
 *
 * @special
 */
#define dmaChannelSetDestination(dmachp, addr) {                                        \
  (dmachp)->xdmac->XDMAC_CHID[(dmachp)->chid].XDMAC_CDA = XDMAC_CDA_DA((uint32_t)addr); \
}

/**
 * @brief   Sets the number of transfers to be performed.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmastp    pointer to a sama_dma_channel_t structure
 * @param[in] size      value to be written in the XDMAC_CUBC register
 *
 * @special
 */
#define dmaChannelSetTransactionSize(dmachp, size) {                               \
  (dmachp)->xdmac->XDMAC_CHID[(dmachp)->chid].XDMAC_CUBC = XDMAC_CUBC_UBLEN(size); \
}

/**
 * @brief   Sets the channel mode settings.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmachp    pointer to a sama_dma_channel_t structure
 * @param[in] mode      value to be written in the XDMAC_CC register
 *
 * @special
 */
#define dmaChannelSetMode(dmachp, mode) {                              \
  (dmachp)->xdmac->XDMAC_CHID[(dmachp)->chid].XDMAC_CC = mode;         \
}

/**
 * @brief   DMA channel enable.
 * @note    This function can be invoked in both ISR or thread context.
 *          The hardware disables a channel on transfer completion by clearing
 *          bit XDMAC_GS.STx.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 *
 * @param[in] dmachp    pointer to a sama_dma_channel_t structure
 *
 * @special
 */
#define dmaChannelEnable(dmachp) {                                      \
  (dmachp)->xdmac->XDMAC_GE |= (XDMAC_GE_EN0 << ((dmachp)->chid));      \
}

/**
 * @brief   DMA channel disable.
 * @details The function disables the specified channel, waits for the disable
 *          operation to complete and then clears any pending interrupt.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 *
 * @param[in] dmachp    pointer to a sama_dma_channel_t structure
 *
 * @special
 */
#define dmaChannelDisable(dmachp) {                                       \
  (dmachp)->xdmac->XDMAC_GD |= XDMAC_GD_DI0 << ((dmachp)->chid);          \
  dmaGetChannelInt(dmachp);                                               \
}

/**
 * @brief   Starts a memory to memory operation using the specified channel.
 * @note    The default transfer data mode is "byte to byte" but it can be
 *          changed by specifying extra options in the @p mode parameter.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmachp    pointer to a sama_dma_channel_t structure
 * @param[in] mode      value to be written in the CC register, this value
 *                      is implicitly ORed with:
 *                      - @p XDMAC_CC_TYPE_MEM_TRAN
 *                      - @p XDMAC_CC_SAM_INCREMENTED_AM
 *                      - @p XDMAC_CC_DAM_INCREMENTED_AM
 *                      - @p XDMAC_CC_SWREQ_SWR_CONNECTED
 *                      - @p XDMAC_CC_SIF_AHB_IF0
 *                      - @p XDMAC_CC_DIF_AHB_IF0
 *                      - @p XDMAC_GE
 *                      .
 * @param[in] src       source address
 * @param[in] dst       destination address
 * @param[in] n         number of data units to copy
 */
#define dmaStartMemCopy(dmachp, mode, src, dst, n) {                              \
  dmaChannelSetSource(dmachp, src);                                               \
  dmaChannelSetDestination(dmachp, dst);                                          \
  dmaChannelSetTransactionSize(dmachp, n);                                        \
  dmaChannelSetMode(dmachp, (mode) |                                              \
                     XDMAC_CC_TYPE_MEM_TRAN | XDMAC_CC_SAM_INCREMENTED_AM |       \
                     XDMAC_CC_DAM_INCREMENTED_AM | XDMAC_CC_SWREQ_SWR_CONNECTED | \
                     XDMAC_CC_SIF_AHB_IF0 | XDMAC_CC_DIF_AHB_IF0);                \
  dmaChannelEnable(dmachp);                                                       \
}

/**
 * @brief   Polled wait for DMA transfer end.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmachp    pointer to a sama_dma_channel_t structure
 */
#define dmaWaitCompletion(dmachp) {                                               \
  (dmachp)->xdmac->XDMAC_GID |= XDMAC_GID_ID0 << ((dmachp)->chid));               \
  while ((dmachp)->xdmac->XDMAC_GS & (XDMAC_GS_ST0 << ((dmachp)->chid))))         \
    ;                                                                             \
  dmaGetChannelInt(dmachp);                                                       \
}
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern sama_dma_channel_t _sama_dma_channel_t[XDMAC_CHANNELS_TOT];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
  sama_dma_channel_t* dmaChannelAllocate(uint32_t priority,
                                         sama_dmaisr_t func,
                                         void *param);
  void dmaChannelRelease(sama_dma_channel_t *dmachp);
#ifdef __cplusplus
}
#endif

#endif /* SAMA_DMA_H */

/** @} */
