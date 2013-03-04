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
