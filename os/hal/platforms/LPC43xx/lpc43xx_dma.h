/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC43xx DMA driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC43xx/LPC43xx_dma.h
 * @brief   DMA driver header.
 *
 * @addtogroup LPC43xx_DMA
 * @{
 */

#ifndef _LPC43xx_DMA_H_
#define _LPC43xx_DMA_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define LPC_GPDMACH0 ((lpc_dma_channel_config_t *) (LPC_GPDMA_BASE + 0x100))
#define LPC_GPDMACH1 ((lpc_dma_channel_config_t *) (LPC_GPDMA_BASE + 0x120))
#define LPC_GPDMACH2 ((lpc_dma_channel_config_t *) (LPC_GPDMA_BASE + 0x140))
#define LPC_GPDMACH3 ((lpc_dma_channel_config_t *) (LPC_GPDMA_BASE + 0x160))
#define LPC_GPDMACH4 ((lpc_dma_channel_config_t *) (LPC_GPDMA_BASE + 0x180))
#define LPC_GPDMACH5 ((lpc_dma_channel_config_t *) (LPC_GPDMA_BASE + 0x1A0))
#define LPC_GPDMACH6 ((lpc_dma_channel_config_t *) (LPC_GPDMA_BASE + 0x1C0))
#define LPC_GPDMACH7 ((lpc_dma_channel_config_t *) (LPC_GPDMA_BASE + 0x1E0))

#define DMACCONFIG_E                        (1UL << 0)
#define DMACCONFIG_M                        (1UL << 1)

/**
 * @brief   Total number of DMA streams.
 * @note    This is the total number of streams among all the DMA units.
 */
#define LPC_DMA_CHANNELS                    8

/**
 * @name    DMA control data configuration
 * @{
 */

/**
 * @brief   DMA transfer size.
 *
 * @param[in] n    DMA transfer size
 */
#define DMA_CTRL_TRANSFER_SIZE(n)           (n)

/**
 * @brief   DMA source burst size.
 */
#define DMA_CTRL_SRC_BSIZE_1                (0 << 12)
#define DMA_CTRL_SRC_BSIZE_4                (1UL << 12)
#define DMA_CTRL_SRC_BSIZE_8                (2UL << 12)
#define DMA_CTRL_SRC_BSIZE_16               (3UL << 12)
#define DMA_CTRL_SRC_BSIZE_32               (4UL << 12)
#define DMA_CTRL_SRC_BSIZE_64               (5UL << 12)
#define DMA_CTRL_SRC_BSIZE_128              (6UL << 12)
#define DMA_CTRL_SRC_BSIZE_256              (7UL << 12)

/**
 * @brief   DMA destination burst size.
 * @{
 */
#define DMA_CTRL_DST_BSIZE_1                (0 << 15)
#define DMA_CTRL_DST_BSIZE_4                (1UL << 15)
#define DMA_CTRL_DST_BSIZE_8                (2UL << 15)
#define DMA_CTRL_DST_BSIZE_16               (3UL << 15)
#define DMA_CTRL_DST_BSIZE_32               (4UL << 15)
#define DMA_CTRL_DST_BSIZE_64               (5UL << 15)
#define DMA_CTRL_DST_BSIZE_128              (6UL << 15)
#define DMA_CTRL_DST_BSIZE_256              (7UL << 15)
/** @} */

/**
 * @name    DMA source transfer width.
 * @{
 */
#define DMA_CTRL_SRC_WIDTH_BYTE             (0 << 18)
#define DMA_CTRL_SRC_WIDTH_HWORD            (1UL << 18)
#define DMA_CTRL_SRC_WIDTH_WORD             (2UL << 18)
/** @} */

/**
 * @name    DMA destination transfer width.
 * @{
 */
#define DMA_CTRL_DST_WIDTH_BYTE             (0 << 21)
#define DMA_CTRL_DST_WIDTH_HWORD            (1UL << 21)
#define DMA_CTRL_DST_WIDTH_WORD             (2UL << 21)

/**
 * @name    DMA source, source AHB master select.
 * @{
 */
#define DMA_CTRL_SRC_AHBM0                  (0 << 24)
#define DMA_CTRL_SRC_AHBM1                  (1 << 24)
/** @} */

/**
 * @name    DMA destination AHB master select.
 * @{
 */
#define DMA_CTRL_DST_AHBM0                  (0 << 25)
#define DMA_CTRL_DST_AHBM1                  (1 << 25)
/** @} */

/**
 * @name    DMA source increment after each transfer.
 * @{
 */
#define DMA_CTRL_SRC_NOINC                  (0UL << 26)
#define DMA_CTRL_SRC_INC                    (1UL << 26)

/**
 * @name    DMA destination increment after each transfer.
 * @{
 */
#define DMA_CTRL_DST_NOINC                  (0UL << 27)
#define DMA_CTRL_DST_INC                    (1UL << 27)

/**
 * @name    DMA bus access bits.
 * @{
 */
#define DMA_CTRL_PROT1_USER                 (0 << 28)
#define DMA_CTRL_PROT1_PRIV                 (1UL << 28)

#define DMA_CTRL_PROT2_NONBUFF              (0 << 29)
#define DMA_CTRL_PROT2_BUFF                 (1UL << 29)

#define DMA_CTRL_PROT3_NONCACHE             (0 << 30)
#define DMA_CTRL_PROT3_CACHE                (1UL << 30)
/** @} */

/**
 * @name    DMA terminal count interrupt enable.
 * @{
 */
#define DMA_CTRL_INT                        (1UL << 31)
/** @} */

/**
 * @name    DMA channel enable.
 * @{
 */
#define DMA_CFG_CH_ENABLE                   (1UL << 0)

/**
 * @brief   Source peripheral.
 *
 * @param[in] source        source peripheral
 */
#define DMA_CFG_SRC_PERIPH(src)             ((src) << 1)

/**
 * @brief   Destination peripheral.
 *
 * @param[in] destination   destination peripheral
 */
#define DMA_CFG_DST_PERIPH(dst)             ((dst) << 6)

/**
 * @name    Flow control and transfer type.
 * @{
 */
#define DMA_CFG_FCTRL_M2M               (0UL << 11)
#define DMA_CFG_FCTRL_M2P               (1UL << 11)
#define DMA_CFG_FCTRL_P2M               (2UL << 11)
#define DMA_CFG_FCTRL_P2P_DMA_CTRL      (3UL << 11)
#define DMA_CFG_FCTRL_P2P_DST_CTRL      (4UL << 11)
#define DMA_CFG_FCTRL_M2P_PER_CTRL      (5UL << 11)
#define DMA_CFG_FCTRL_P2M_PER_CTRL      (6UL << 11)
#define DMA_CFG_FCTRL_P2P_SRC_CTRL      (7UL << 11)
/** @} */

/**
 * @name    Interrupt error mask.
 * @{
 */
#define DMA_CFG_IE                          (1UL << 14)
/** @} */

/**
 * @name    Terminal count interrupt mask.
 * @{
 */
#define DMA_CFG_ITC                         (1UL << 15)
/** @} */

/**
 * @name    Active.
 * @note    Read only
 * @{
 */
#define DMA_CFG_ACTIVE                      (1UL << 17)
/** @} */

/**
 * @name    Halt.
 * @{
 */
#define DMA_CFG_HALT                        (1UL << 18)
/** @} */
/** @} */

/**
 * @name    AHB master select for loading the next LLI.
 * @{
 */
#define DMA_LLI_AHBM0                        0
#define DMA_LLI_AHBM1                        1
/** @} */
/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   DMA interrupt priority level setting.
 */
#if !defined(LPC_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC_DMA_IRQ_PRIORITY                3
#endif


/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/



/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef struct {
  volatile uint32_t srcaddr;        /**< @brief Source address.         */
  volatile uint32_t dstaddr;        /**< @brief Destination address.    */
  volatile uint32_t lli;            /**< @brief Linked List Item.       */
  volatile uint32_t control;        /**< @brief Control.                */
  volatile uint32_t config;         /**< @brief Configuration.          */
} lpc_dma_channel_config_t;

typedef struct {
  volatile uint32_t srcaddr;        /**< @brief Source address.         */
  volatile uint32_t dstaddr;        /**< @brief Destination address.    */
  volatile uint32_t lli;            /**< @brief Linked List Item.       */
  volatile uint32_t control;        /**< @brief Control.                */
} lpc_dma_lli_config_t;

/**
 * @brief   DMA channel number.
 */
typedef enum {
  DMA_CHANNEL0               = 0,
  DMA_CHANNEL1               = 1,
  DMA_CHANNEL2               = 2,
  DMA_CHANNEL3               = 3,
  DMA_CHANNEL4               = 4,
  DMA_CHANNEL5               = 5,
  DMA_CHANNEL6               = 6,
  DMA_CHANNEL7               = 7
} lpc_dma_channel_t;

 /**
  * @brief   DMA source or destination type.
  */
 typedef enum {
   PERIPHERAL0          = 0,
   PERIPHERAL1          = 1,
   PERIPHERAL2          = 2,
   PERIPHERAL3          = 3,
   PERIPHERAL4          = 4,
   PERIPHERAL5          = 5,
   PERIPHERAL6          = 6,
   PERIPHERAL7          = 7,
   PERIPHERAL8          = 8,
   PERIPHERAL9          = 9,
   PERIPHERAL10         = 10,
   PERIPHERAL11         = 11,
   PERIPHERAL12         = 12,
   PERIPHERAL13         = 13,
   PERIPHERAL14         = 14,
   PERIPHERAL15         = 15
 } lpc_dma_src_dst_t;

/**
 * @brief   LPC DMA ISR function type.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] flags     pre-shifted content of the xISR register, the bits
 *                      are aligned to bit zero
 */
typedef void (*lpc_dmaisr_t)(void *p, uint32_t flags);

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */

/**
 * @brief   Set dma peripheral connection.
 *
 * @param[in] periphn   dma peripheral connection number
 * @param[in] select    selected peripheral
 *
 * @special
 */
#define dmaMuxSet(periphn, select)                                          \
  LPC_CREG->DMAMUX &= ~(3UL << ((periphn) * 2));                            \
  LPC_CREG->DMAMUX |= (select) << ((periphn) * 2)

/**
 * @brief   Associates a memory source to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmach     DMA channel number
 * @param[in] addr      pointer to a source address
 *
 * @special
 */
#define dmaChannelSrcAddr(dmach, addr)                                     \
  _lpc_dma_channel_config[dmach]->srcaddr = (uint32_t)(addr)

/**
 * @brief   Associates a memory destination to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmach     DMA channel number
 * @param[in] addr      pointer to a destination address
 *
 * @special
 */
#define dmaChannelDstAddr(dmach, addr)                                    \
  _lpc_dma_channel_config[dmach]->dstaddr = (uint32_t)(addr)

/**
 * @brief   Associates a linked list item address to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmach     DMA channel number
 * @param[in] addr      pointer to a linked list item
 * @param[in] master    AHB master select for loading next LLI, 0 or 1
 *
 * @special
 */
#define dmaChannelLinkedList(dmach, addr, master)                                 \
  _lpc_dma_channel_config[dmach]->lli = (((uint32_t)(addr)) | master)

/**
 * @brief   Set control configuration to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmach     DMA channel number
 * @param[in] ctrl      control configuration value
 *
 * @special
 */
#define dmaChannelControl(dmach, ctrl)                                     \
  _lpc_dma_channel_config[dmach]->control = (ctrl)

/**
 * @brief   Set configuration to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaStreamAllocate().
 * @post    After use the channel can be released using @p dmaStreamRelease().
 *
 * @param[in] dmach     DMA channel number
 * @param[in] config    dma channel configuration value
 *
 * @special
 */
#define dmaChannelConfig(dmach, cfg)                                    \
  _lpc_dma_channel_config[dmach]->config = (cfg)

/**
 * @brief   Trigger DMA software burst transfer request.
 *
 * @param[in] src   peripheral source request
 *
 * @special
 */
#define dmaSoftBurstRequest(src)                                            \
  LPC_GPDMA->SOFTBREQ = (1UL << (src))

/**
 * @brief   Trigger DMA software single transfer request.
 *
 * @param[in] src   peripheral source request
 *
 * @special
 */
#define dmaSoftSingleRequest(src)                                           \
  LPC_GPDMA->SOFTSREQ = (1UL << (src))

/**
 * @brief   DMA channel enable.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmach     DMA channel number
 *
 * @special
 */
#define dmaChannelEnable(dmach)                                            \
  _lpc_dma_channel_config[dmach]->config |= (DMA_CFG_CH_ENABLE)

/**
 * @brief   DMA channel disable.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmach     DMA channel number
 *
 * @special
 */
#define dmaChannelDisable(dmach)                                           \
  _lpc_dma_channel_config[dmach]->config &= ~(DMA_CFG_CH_ENABLE)

/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern lpc_dma_channel_config_t * _lpc_dma_channel_config[];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
  bool_t dmaChannelAllocate(lpc_dma_channel_t dmach,
                           lpc_dmaisr_t func,
                           void *param);
  void dmaChannelRelease(lpc_dma_channel_t dmach);
#ifdef __cplusplus
}
#endif

#endif /* _LPC43xx_DMA_H_ */

/** @} */
