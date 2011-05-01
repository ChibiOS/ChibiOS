/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    sdc.h
 * @brief   SDC Driver macros and structures.
 *
 * @addtogroup SDC
 * @{
 */

#ifndef _SDC_H_
#define _SDC_H_

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define SDC_CMD_GO_IDLE_STATE         0
#define SDC_CMD_SEND_IF_COND          8
#define SDC_CMD_APP_OP_COND           41
#define SDC_CMD_APP_CMD               55

#define SDC_MODE_CARDTYPE_MASK        0xF
#define SDC_MODE_CARDTYPE_SD          0   /**< Old SD card.                 */
#define SDC_MODE_CARDTYPE_SDV20       1   /**< Card is V2.0 compliant.      */
#define SDC_MODE_CARDTYPE_MMC         2   /**< Card is MMC.                 */

#define SDC_CMD8_PATTERN              0x000001AA

#define SDC_ACMD41_RETRY              100

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
 * @brief   Driver state machine possible states.
 */
typedef enum {
  SDC_UNINIT = 0,                   /**< Not initialized.                   */
  SDC_STOP = 1,                     /**< Stopped.                           */
  SDC_READY = 2,                    /**< Ready.                             */
  SDC_INITNG = 3,                   /**< Card initialization in progress.   */
  SDC_ACTIVE = 4,                   /**< Cart initialized.                  */
  SDC_READING = 5,                  /**< Read operation in progress.        */
  SDC_WRITING = 6,                  /**< Write operation in progress.       */
} sdcstate_t;

#include "sdc_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sdcInit(void);
  void sdcObjectInit(SDCDriver *sdcp);
  void sdcStart(SDCDriver *sdcp, const SDCConfig *config);
  void sdcStop(SDCDriver *sdcp);
  bool_t sdcConnect(SDCDriver *sdcp);
  bool_t sdcDisconnect(SDCDriver *sdcp);
  bool_t sdcRead(SDCDriver *sdcp, uint32_t startblk,
                 uint8_t *buffer, uint32_t n);
  bool_t sdcWrite(SDCDriver *sdcp, uint32_t startblk,
                  const uint8_t *buffer, uint32_t n);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SDC */

#endif /* _SDC_H_ */

/** @} */
