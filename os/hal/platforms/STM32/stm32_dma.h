/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    stm32_dma.h
 * @brief   STM32 DMA helper driver header.
 * @note    This file requires definitions from the ST STM32 header file
 *          stm3232f10x.h.
 *
 * @addtogroup STM32_DMA
 * @{
 */

#ifndef _STM32_DMA_H_
#define _STM32_DMA_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/** @brief DMA1 identifier.*/
#define DMA1_ID 0

/** @brief DMA2 identifier.*/
#if STM32_HAS_DMA2 || defined(__DOXYGEN__)
#define DMA2_ID 1
#endif

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
 * @brief   STM32 DMA channel memory structure.
 */
typedef struct {
  volatile uint32_t     CCR;
  volatile uint32_t     CNDTR;
  volatile uint32_t     CPAR;
  volatile uint32_t     CMAR;
  volatile uint32_t     dummy;
} stm32_dma_channel_t;

/**
 * @brief   STM32 DMA subsystem memory structure.
 * @note    This structure has been redefined here because it is convenient to
 *          have the channels organized as an array, the ST header does not
 *          do that.
 */
typedef struct {
  volatile uint32_t     ISR;
  volatile uint32_t     IFCR;
  stm32_dma_channel_t   channels[7];
} stm32_dma_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/** DMA1 registers block numeric address.*/
#define STM32_DMA1_BASE         (AHBPERIPH_BASE + 0x0000)
/** Pointer to the DMA1 registers block.*/
#define STM32_DMA1              ((stm32_dma_t *)STM32_DMA1_BASE)
/** Pointer to the DMA1 channel 1 registers block.*/
#define STM32_DMA1_CH1          (&STM32_DMA1->channels[0])
/** Pointer to the DMA1 channel 2 registers block.*/
#define STM32_DMA1_CH2          (&STM32_DMA1->channels[1])
/** Pointer to the DMA1 channel 3 registers block.*/
#define STM32_DMA1_CH3          (&STM32_DMA1->channels[2])
/** Pointer to the DMA1 channel 4 registers block.*/
#define STM32_DMA1_CH4          (&STM32_DMA1->channels[3])
/** Pointer to the DMA1 channel 5 registers block.*/
#define STM32_DMA1_CH5          (&STM32_DMA1->channels[4])
/** Pointer to the DMA1 channel 6 registers block.*/
#define STM32_DMA1_CH6          (&STM32_DMA1->channels[5])
/** Pointer to the DMA1 channel 7 registers block.*/
#define STM32_DMA1_CH7          (&STM32_DMA1->channels[6])

#if STM32_HAS_DMA2 || defined(__DOXYGEN__)
/** DMA2 registers block numeric address.*/
#define STM32_DMA2_BASE         (AHBPERIPH_BASE + 0x0400)
/** Pointer to the DMA2 registers block.*/
#define STM32_DMA2              ((stm32_dma_t *)STM32_DMA2_BASE)
/** Pointer to the DMA2 channel 1 registers block.*/
#define STM32_DMA2_CH1          (&STM32_DMA2->channels[0])
/** Pointer to the DMA2 channel 2 registers block.*/
#define STM32_DMA2_CH2          (&STM32_DMA2->channels[1])
/** Pointer to the DMA2 channel 3 registers block.*/
#define STM32_DMA2_CH3          (&STM32_DMA2->channels[2])
/** Pointer to the DMA2 channel 4 registers block.*/
#define STM32_DMA2_CH4          (&STM32_DMA2->channels[3])
/** Pointer to the DMA2 channel 5 registers block.*/
#define STM32_DMA2_CH5          (&STM32_DMA2->channels[4])
#endif

#define STM32_DMA_CHANNEL_1     0       /**< @brief DMA channel 1.          */
#define STM32_DMA_CHANNEL_2     1       /**< @brief DMA channel 2.          */
#define STM32_DMA_CHANNEL_3     2       /**< @brief DMA channel 3.          */
#define STM32_DMA_CHANNEL_4     3       /**< @brief DMA channel 4.          */
#define STM32_DMA_CHANNEL_5     4       /**< @brief DMA channel 5.          */
#define STM32_DMA_CHANNEL_6     5       /**< @brief DMA channel 6.          */
#define STM32_DMA_CHANNEL_7     6       /**< @brief DMA channel 7.          */

/**
 * @brief   Associates a peripheral data register to a DMA channel.
 *
 * @param[in] dmachp    dmachp to a stm32_dma_channel_t structure
 * @param[in] cpar      value to be written in the CPAR register
 *
 * @api
 */
#define dmaChannelSetPeripheral(dmachp, cpar) {                             \
  (dmachp)->CPAR  = (uint32_t)(cpar);                                       \
}

/**
 * @brief   DMA channel setup by channel pointer.
 * @note    This macro does not change the CPAR register because that register
 *          value does not change frequently, it usually points to a peripheral
 *          data register.
 *
 * @param[in] dmachp    dmachp to a stm32_dma_channel_t structure
 * @param[in] cndtr     value to be written in the CNDTR register
 * @param[in] cmar      value to be written in the CMAR register
 * @param[in] ccr       value to be written in the CCR register
 *
 * @api
 */
#define dmaChannelSetup(dmachp, cndtr, cmar, ccr) {                         \
  (dmachp)->CNDTR = (uint32_t)(cndtr);                                      \
  (dmachp)->CMAR  = (uint32_t)(cmar);                                       \
  (dmachp)->CCR   = (uint32_t)(ccr);                                        \
}

/**
 * @brief   DMA channel enable by channel pointer.
 *
 * @param[in] dmachp    dmachp to a stm32_dma_channel_t structure
 *
 * @api
 */
#define dmaChannelEnable(dmachp) {                                          \
  (dmachp)->CCR |= DMA_CCR1_EN;                                             \
}


/**
 * @brief   DMA channel disable by channel pointer.
 *
 * @param[in] dmachp    dmachp to a stm32_dma_channel_t structure
 *
 * @api
 */
#define dmaChannelDisable(dmachp) {                                         \
  (dmachp)->CCR = 0;                                                        \
}

/**
 * @brief   DMA channel setup by channel ID.
 * @note    This macro does not change the CPAR register because that register
 *          value does not change frequently, it usually points to a peripheral
 *          data register.
 * @note    Channels are numbered from 0 to 6, use the appropriate macro
 *          as parameter.
 *
 * @param[in] dmap      pointer to a stm32_dma_t structure
 * @param[in] ch        channel number
 * @param[in] cndtr     value to be written in the CNDTR register
 * @param[in] cmar      value to be written in the CMAR register
 * @param[in] ccr       value to be written in the CCR register
 *
 * @api
 */
#define dmaSetupChannel(dmap, ch, cndtr, cmar, ccr) {                       \
  dmaChannelSetup(&(dmap)->channels[ch], (cndtr), (cmar), (ccr));           \
}

/**
 * @brief   DMA channel enable by channel ID.
 * @note    Channels are numbered from 0 to 6, use the appropriate macro
 *          as parameter.
 *
 * @param[in] dmap      pointer to a stm32_dma_t structure
 * @param[in] ch        channel number
 *
 * @api
 */
#define dmaEnableChannel(dmap, ch) {                                        \
  dmaChannelEnable(&(dmap)->channels[ch]);                                  \
}

/**
 * @brief   DMA channel disable by channel ID.
 * @note    Channels are numbered from 0 to 6, use the appropriate macro
 *          as parameter.
 *
 * @param[in] dmap      pointer to a stm32_dma_t structure
 * @param[in] ch        channel number
 *
 * @api
 */
#define dmaDisableChannel(dmap, ch) {                                       \
  dmaChannelDisable(&(dmap)->channels[ch]);                                 \
}

/**
 * @brief   DMA channel interrupt sources clear.
 * @details Sets the appropriate CGIF bit into the IFCR register in order to
 *          withdraw all the pending interrupt bits from the ISR register.
 * @note    Channels are numbered from 0 to 6, use the appropriate macro
 *          as parameter.
 *
 * @param[in] dmap      pointer to a stm32_dma_t structure
 * @param[in] ch        channel number
 *
 * @api
 */
#define dmaClearChannel(dmap, ch){                                          \
  (dmap)->IFCR = 1 << ((ch) * 4);                                           \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
  void dmaEnable(uint32_t dma);
  void dmaDisable(uint32_t dma);
#ifdef __cplusplus
}
#endif

#endif /* _STM32_DMA_H_ */

/** @} */
