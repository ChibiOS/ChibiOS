/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
*/

/**
 * @file    SPC5xx/edma.h
 * @brief   EDMA helper driver header.
 *
 * @addtogroup SPC5xx_EDMA
 * @{
 */

#ifndef _EDMA_H_
#define _EDMA_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   EDMA channel allocation error.
 */
#define EDMA_ERROR -1

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Default EDMA CR register initialization.
 */
#if !defined(SPC5_EDMA_ERROR_HANDLER) || defined(__DOXYGEN__)
#define SPC5_EDMA_CR_SETTING                0x0000C400
#endif

/**
 * @brief   EDMA critical error handler, must not return.
 */
#if !defined(SPC5_EDMA_ERROR_HANDLER) || defined(__DOXYGEN__)
#define SPC5_EDMA_ERROR_HANDLER()           chSysHalt()
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !SPC5_HAS_EDMAA
#error "this device does not have an eDMA unit"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of and EDMA channel number.
 */
typedef int32_t edma_channel_t;

/**
 * @brief   Type of an EDMA TCD.
 */
typedef struct {
  uint32_t              word[8];
} edma_tcd_t;

/**
 * @brief   DMA ISR function type.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 */
typedef void (*edma_callback_t)(edma_channel_t channel, void *p);

/**
 * @brief   Type of an EDMA channel configuration structure.
 */
typedef struct {
  uint8_t               dma_periph;     /**< @brief Peripheral to be
                                             associated to the channel.     */
  uint8_t               dma_prio;       /**< @brief Priority register value
                                             for this channel.              */
  uint8_t               dma_irq_prio;   /**< @brief IRQ priority level for
                                             this channel.                  */
  edma_callback_t       dma_func;       /**< @brief Channel callback.       */
  edma_callback_t       dma_error_func; /**< @brief Channel error callback. */
  void                  *dma_param;     /**< @brief Channel callback param. */
} edma_channel_config_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the TCD address associated to a channel.
 *
 * @param[in] channel   the channel number
 * @return              A pointer to an @p edma_tcd_t structure.
 *
 * @api
 */
#define edmaGetTCD(channel) ((edma_tcd_t *)&EDMA.TCD[channel])

/**
 * @brief   Sets the source address into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 *
 * @api
 */
#define edmaChannelSetSourceAddress(tcdp, src)                              \
  ((tcdp)->word[0] = (uint32_t)(src))

/**
 * @brief   Sets the destination address into a TCD.
 *
 * @param[in] tcdp      pointer to an @p edma_tcd_t structure
 *
 * @api
 */
#define edmaChannelSetDestinationAddress(tcdp, dst)                         \
  ((tcdp)->word[4] = (uint32_t)(dst))

/**
 * @brief   Starts or restarts an EDMA channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#define edmaChannelStart(channel) (EDMA.SERQR.R = (channel))

/**
 * @brief   Stops an EDMA channel.
 *
 * @param[in] channel   the channel number
 *
 * @api
 */
#define edmaChannelStop(channel) (EDMA.CERQR.R = (channel))

/**
 * @brief   EDMA channel setup.
 *
 * @param[in] channel   eDMA channel number
 * @param[in] src       source address
 * @param[in] dst       destination address
 * @param[in] soff      source address offset
 * @param[in] doff      destination address offset
 * @param[in] ssize     source transfer size
 * @param[in] dsize     destination transfer size
 * @param[in] nbytes    minor loop count
 * @param[in] iter      major loop count
 * @param[in] dlast     last destination address adjustment
 * @param[in] slast     last source address adjustment
 * @param[in] mode      LSW of TCD register 7
 *
 * @api
 */
#define edmaChannelSetup(channel, src, dst, soff, doff, ssize, dsize,       \
                         nbytes, iter, slast, dlast,  mode) {               \
  edma_tcd_t *tcdp = edmaGetTCD(channel);                                   \
  tcdp->word[0] = (uint32_t)(src);                                          \
  tcdp->word[1] = ((uint32_t)(ssize) << 24) | ((uint32_t)(dsize) << 16) |   \
                   (uint32_t)(soff);                                        \
  tcdp->word[2] = (uint32_t)(nbytes);                                       \
  tcdp->word[3] = (uint32_t)(slast);                                        \
  tcdp->word[0] = (uint32_t)(dst);                                          \
  tcdp->word[5] = ((uint32_t)(iter) << 16) | (uint32_t)(doff);              \
  tcdp->word[6] = (uint32_t)(dlast);                                        \
  tcdp->word[7] = ((uint32_t)(iter) << 16) | (uint32_t)(mode);              \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void edmaInit(void);
  edma_channel_t edmaChannelAllocate(const edma_channel_config_t *ccfg);
  void edmaChannelRelease(edma_channel_t channel);
#ifdef __cplusplus
}
#endif

#endif /* _EDMA_H_ */

/** @} */
