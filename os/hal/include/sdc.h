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

#define SDC_BLOCK_SIZE                  512     /**< Fixed block size.      */

/**
 * @brief   Fixed pattern for CMD8.
 */
#define SDC_CMD8_PATTERN                0x000001AA

/**
 * @name    SD cart types
 * @{
 */
#define SDC_MODE_CARDTYPE_MASK          0xF     /**< @brief Card type mask. */
#define SDC_MODE_CARDTYPE_SDV11         0       /**< @brief Card is SD V1.1.*/
#define SDC_MODE_CARDTYPE_SDV20         1       /**< @brief Card is SD V2.0.*/
#define SDC_MODE_CARDTYPE_MMC           2       /**< @brief Card is MMC.    */
#define SDC_MODE_HIGH_CAPACITY          0x10    /**< @brief High cap.card.  */
/** @} */

/**
 * @brief   Mask of error bits in R1 responses.
 */
#define SDC_R1_ERROR_MASK               0xFDFFE008

#define SDC_STS_IDLE                    0
#define SDC_STS_READY                   1
#define SDC_STS_IDENT                   2
#define SDC_STS_STBY                    3
#define SDC_STS_TRAN                    4
#define SDC_STS_DATA                    5
#define SDC_STS_RCV                     6
#define SDC_STS_PRG                     7
#define SDC_STS_DIS                     8

#define SDC_CMD_GO_IDLE_STATE           0
#define SDC_CMD_INIT                    1
#define SDC_CMD_ALL_SEND_CID            2
#define SDC_CMD_SEND_RELATIVE_ADDR      3
#define SDC_CMD_SET_BUS_WIDTH           6
#define SDC_CMD_SEL_DESEL_CARD          7
#define SDC_CMD_SEND_IF_COND            8
#define SDC_CMD_SEND_CSD                9
#define SDC_CMD_STOP_TRANSMISSION       12
#define SDC_CMD_SEND_STATUS             13
#define SDC_CMD_SET_BLOCKLEN            16
#define SDC_CMD_READ_SINGLE_BLOCK       17
#define SDC_CMD_READ_MULTIPLE_BLOCK     18
#define SDC_CMD_SET_BLOCK_COUNT         23
#define SDC_CMD_WRITE_BLOCK             24
#define SDC_CMD_WRITE_MULTIPLE_BLOCK    25
#define SDC_CMD_APP_OP_COND             41
#define SDC_CMD_LOCK_UNLOCK             42
#define SDC_CMD_APP_CMD                 55

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    SDC configuration options
 * @{
 */
/**
 * @brief   Number of initialization attempts before rejecting the card.
 * @note    Attempts are performed at 10mS intevals.
 */
#if !defined(SDC_INIT_RETRY) || defined(__DOXYGEN__)
#define SDC_INIT_RETRY                  100
#endif

/**
 * @brief   Include support for MMC cards.
 * @note    MMC support is not yet implemented so this option must be kept
 *          at @p FALSE.
 */
#if !defined(SDC_MMC_SUPPORT) || defined(__DOXYGEN__)
#define SDC_MMC_SUPPORT                 FALSE
#endif

/**
 * @brief   Delays insertions.
 * @details If enabled this options inserts delays into the MMC waiting
 *          routines releasing some extra CPU time for the threads with
 *          lower priority, this may slow down the driver a bit however.
 */
#if !defined(SDC_NICE_WAITING) || defined(__DOXYGEN__)
#define SDC_NICE_WAITING                TRUE
#endif
/** @} */

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
  SDC_CONNECTING = 3,               /**< Card connection in progress.       */
  SDC_DISCONNECTING = 4,            /**< Card disconnection in progress.    */
  SDC_ACTIVE = 5,                   /**< Cart initialized.                  */
  SDC_READING = 6,                  /**< Read operation in progress.        */
  SDC_WRITING = 7,                  /**< Write operation in progress.       */
} sdcstate_t;

#include "sdc_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    R1 response utilities
 * @{
 */
/**
 * @brief   Evaluates to @p TRUE if the R1 response contains error flags.
 *
 * @param[in] r1        the r1 response
 */
#define SDC_R1_ERROR(r1)                (((r1) & SDC_R1_ERROR_MASK) != 0)

/**
 * @brief   Returns the status field of an R1 response.
 *
 * @param[in] r1        the r1 response
 */
#define SDC_R1_STS(r1)                  (((r1) >> 9) & 15)

/**
 * @brief   Evaluates to @p TRUE if the R1 response indicates a locked card.
 *
 * @param[in] r1        the r1 response
 */
#define SDC_R1_IS_CARD_LOCKED(r1)       (((r1) >> 21) & 1)
/** @} */

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Returns the driver state.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @return              The driver state.
 *
 * @api
 */
#define sdcGetDriverState(sdcp)         ((sdcp)->state)

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
 * @retval FALSE        card not inserted.
 * @retval TRUE         card inserted.
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
  bool_t sdcConnect(SDCDriver *sdcp);
  bool_t sdcDisconnect(SDCDriver *sdcp);
  bool_t sdcRead(SDCDriver *sdcp, uint32_t startblk,
                 uint8_t *buffer, uint32_t n);
  bool_t sdcWrite(SDCDriver *sdcp, uint32_t startblk,
                  const uint8_t *buffer, uint32_t n);
  bool_t _sdc_wait_for_transfer_state(SDCDriver *sdcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SDC */

#endif /* _SDC_H_ */

/** @} */
