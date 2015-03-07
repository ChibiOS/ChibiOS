/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
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

/**
 * @name    SD card types
 * @{
 */
#define SDC_MODE_CARDTYPE_MASK              0xF /**< @brief Card type mask. */
#define SDC_MODE_CARDTYPE_SDV11             0
#define SDC_MODE_CARDTYPE_SDV20             1
#define SDC_MODE_CARDTYPE_MMC               2
#define SDC_MODE_HIGH_CAPACITY              0x10
/** @} */

/**
 * @name    SDC bus error conditions
 * @{
 */
#define SDC_NO_ERROR                        0
#define SDC_CMD_CRC_ERROR                   1
#define SDC_DATA_CRC_ERROR                  2
#define SDC_DATA_TIMEOUT                    4
#define SDC_COMMAND_TIMEOUT                 8
#define SDC_TX_UNDERRUN                     16
#define SDC_RX_OVERRUN                      32
#define SDC_STARTBIT_ERROR                  64
#define SDC_OVERFLOW_ERROR                  128
#define SDC_UNHANDLED_ERROR                 0xFFFFFFFF
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    SDC configuration options
 * @{
 */
/**
 * @brief   Number of initialization attempts before rejecting the card.
 * @note    Attempts are performed at 10mS intervals.
 */
#if !defined(SDC_INIT_RETRY) || defined(__DOXYGEN__)
#define SDC_INIT_RETRY                      100
#endif

/**
 * @brief   Include support for MMC cards.
 * @note    MMC support is not yet implemented so this option must be kept
 *          at @p FALSE.
 */
#if !defined(SDC_MMC_SUPPORT) || defined(__DOXYGEN__)
#define SDC_MMC_SUPPORT                     FALSE
#endif

/**
 * @brief   Delays insertions.
 * @details If enabled this options inserts delays into the MMC waiting
 *          routines releasing some extra CPU time for the threads with
 *          lower priority, this may slow down the driver a bit however.
 */
#if !defined(SDC_NICE_WAITING) || defined(__DOXYGEN__)
#define SDC_NICE_WAITING                    TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of SDIO bus mode.
 */
typedef enum {
  SDC_MODE_1BIT = 0,
  SDC_MODE_4BIT,
  SDC_MODE_8BIT
} sdcbusmode_t;

/**
 * @brief   Max supported clock.
 */
typedef enum {
  SDC_CLK_25MHz = 0,
  SDC_CLK_50MHz,
} sdcbusclk_t;

#include "sdc_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Returns the card insertion status.
 * @note    This macro wraps a low level function named
 *          @p sdc_lld_is_card_inserted(), this function must be
 *          provided by the application because it is not part of the
 *          SDC driver.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @return              The card state.
 * @retval FALSE        card not inserted.
 * @retval TRUE         card inserted.
 *
 * @api
 */
#define sdcIsCardInserted(sdcp) (sdc_lld_is_card_inserted(sdcp))

/**
 * @brief   Returns the write protect status.
 * @note    This macro wraps a low level function named
 *          @p sdc_lld_is_write_protected(), this function must be
 *          provided by the application because it is not part of the
 *          SDC driver.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @return              The card state.
 * @retval FALSE        not write protected.
 * @retval TRUE         write protected.
 *
 * @api
 */
#define sdcIsWriteProtected(sdcp) (sdc_lld_is_write_protected(sdcp))
/** @} */

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
  bool sdcConnect(SDCDriver *sdcp);
  bool sdcDisconnect(SDCDriver *sdcp);
  bool sdcRead(SDCDriver *sdcp, uint32_t startblk,
               uint8_t *buffer, uint32_t n);
  bool sdcWrite(SDCDriver *sdcp, uint32_t startblk,
                const uint8_t *buffer, uint32_t n);
  sdcflags_t sdcGetAndClearErrors(SDCDriver *sdcp);
  bool sdcSync(SDCDriver *sdcp);
  bool sdcGetInfo(SDCDriver *sdcp, BlockDeviceInfo *bdip);
  bool _sdc_wait_for_transfer_state(SDCDriver *sdcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SDC */

#endif /* _SDC_H_ */

/** @} */
