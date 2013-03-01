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
 * @brief   Type of an EDMA TCD.
 */
typedef struct {
  uint32_t              word[8];
} edma_tcd_t;

/**
 * @brief   DMA ISR function type.
 *
 * @param[in] tcd       pointer to the TCD associated to this ISR
 * @param[in] p         parameter for the registered function
 */
typedef void (*edma_isr_t)(edma_tcd_t *tcd, void *p);

/**
 * @brief   Type of an EDMA channel configuration structure.
 */
typedef struct {
  uint32_t              dma_periph;     /**< @brief Peripheral associated to
                                             the channel.                   */
  edma_isr_t            dma_func;       /**< @brief Channel ISR callback.   */
  void                  *dma_param;     /**< @brief Channel callback param. */

} edma_channel_config_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void edmaInit(void);
  edma_tcd_t *edmaAllocChannel(const edma_channel_config_t *ccfg);
  void edmaReleaseChannel(edma_tcd_t *tcd);
#ifdef __cplusplus
}
#endif

#endif /* _EDMA_H_ */

/** @} */
