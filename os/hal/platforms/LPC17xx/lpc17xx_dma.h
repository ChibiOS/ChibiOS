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
 * @file    LPC17xx/LPC17xx_dma.h
 * @brief   DMA driver header.
 *
 * @addtogroup LPC17xx_DMA
 * @{
 */

#ifndef _LPC17xx_DMA_H_
#define _LPC17xx_DMA_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define DMACCONFIG_E                        (1UL << 0)
#define DMACCONFIG_M                        (1UL << 1)

/**
 * @brief   Total number of DMA streams.
 * @note    This is the total number of streams among all the DMA units.
 */
#define LPC17xx_DMA_CHANNELS                8

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
 * @name    Transfer type.
 * @{
 */
#define DMA_CFG_TTYPE_M2M                   (0 << 11)
#define DMA_CFG_TTYPE_M2P                   (1UL << 11)
#define DMA_CFG_TTYPE_P2M                   (2UL << 11)
#define DMA_CFG_TTYPE_P2P                   (3UL << 11)
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

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   DMA interrupt priority level setting.
 */
#if !defined(LPC17xx_DMA_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC17xx_DMA_IRQ_PRIORITY            3
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
} lpc17xx_dma_channel_config_t;

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

} lpc17xx_dma_channel_t;

 /**
  * @brief   DMA source or destination type.
  */
 typedef enum {
   DMA_SSP0_TX          = 0,
   DMA_SSP0_RX          = 1,
   DMA_SSP1_TX          = 2,
   DMA_SSP1_RX          = 3,
   DMA_ADC              = 4,
   DMA_I2S_CH0          = 5,
   DMA_I2S_CH1          = 6,
   DMA_DAC              = 7,
   DMA_UART0_TX_MAT0_0  = 8,
   DMA_UART0_RX_MAT0_1  = 9,
   DMA_UART1_TX_MAT1_0  = 10,
   DMA_UART1_RX_MAT1_1  = 11,
   DMA_UART2_TX_MAT2_0  = 12,
   DMA_UART2_RX_MAT2_1  = 13,
   DMA_UART3_TX_MAT3_0  = 14,
   DMA_UART3_RX_MAT3_1  = 15
 } lpc17xx_dma_src_dst_t;

/**
 * @brief   LPC17xx DMA ISR function type.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] flags     pre-shifted content of the xISR register, the bits
 *                      are aligned to bit zero
 */
typedef void (*lpc17xx_dmaisr_t)(void *p, uint32_t flags);

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */

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
  _lpc17xx_dma_channel_config_t[dmach]->CSrcAddr = (uint32_t)(addr)

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
  _lpc17xx_dma_channel_config_t[dmach]->CDestAddr = (uint32_t)(addr)

/**
 * @brief   Associates a linked list item address to a DMA channel.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The channel must have been allocated using @p dmaChannelAllocate().
 * @post    After use the channel can be released using @p dmaChannelRelease().
 *
 * @param[in] dmach     DMA channel number
 * @param[in] addr      pointer to a linked list item
 *
 * @special
 */
#define dmaChannelLinkedList(dmach, addr)                                 \
  _lpc17xx_dma_channel_config_t[dmach]->CLLI = (((uint32_t)(addr)) << 2)

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
  _lpc17xx_dma_channel_config_t[dmach]->CControl = (ctrl)

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
#define dmaChannelConfig(dmach, config)                                    \
  _lpc17xx_dma_channel_config_t[dmach]->CConfig = (config)

/**
 * @brief   Trigger DMA software burst transfer request.
 *
 * @param[in] src   peripheral source request
 *
 * @special
 */
#define dmaSoftBurstRequest(src)                                            \
  LPC_GPDMA->SoftBReq = (src)

/**
 * @brief   Trigger DMA software single transfer request.
 *
 * @param[in] src   peripheral source request
 *
 * @special
 */
#define dmaSoftSingleRequest(src)                                           \
  LPC_GPDMA->SoftSReq = (src)

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
  _lpc17xx_dma_channel_config_t[dmach]->CConfig |= (DMA_CFG_CH_ENABLE)

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
  _lpc17xx_dma_channel_config_t[dmach]->CConfig &= ~(DMA_CFG_CH_ENABLE)

/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern LPC_GPDMACH_TypeDef * _lpc17xx_dma_channel_config_t[];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
  bool_t dmaChannelAllocate(lpc17xx_dma_channel_t dmach,
                           lpc17xx_dmaisr_t func,
                           void *param);
  void dmaChannelRelease(lpc17xx_dma_channel_t dmach);
#ifdef __cplusplus
}
#endif

#endif /* _LPC17xx_DMA_H_ */

/** @} */
