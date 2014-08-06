/*
    ChibiOS/HAL - Copyright (C) 2006,2007,2008,2009,2010,
                  2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS/HAL

    ChibiOS/HAL is free software; you can redistribute it and/or modify
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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    nand.h
 * @brief   NAND Driver macros and structures.
 *
 * @addtogroup NAND
 * @{
 */

#ifndef _NAND_H_
#define _NAND_H_

#if HAL_USE_NAND || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * Standard NAND flash commands
 */
#define NAND_CMD_READ0          0x00
#define NAND_CMD_RNDOUT         0x05
#define NAND_CMD_PAGEPROG       0x10
#define NAND_CMD_READ0_CONFIRM  0x30
#define NAND_CMD_READOOB        0x50
#define NAND_CMD_ERASE          0x60
#define NAND_CMD_STATUS         0x70
#define NAND_CMD_STATUS_MULTI   0x71
#define NAND_CMD_WRITE          0x80
#define NAND_CMD_RNDIN          0x85
#define NAND_CMD_READID         0x90
#define NAND_CMD_ERASE_CONFIRM  0xD0
#define NAND_CMD_RESET          0xFF

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/
/**
 * @brief   Enables the mutual exclusion APIs on the NAND.
 */
#if !defined(NAND_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define NAND_USE_MUTUAL_EXCLUSION     FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/
#if NAND_USE_MUTUAL_EXCLUSION && !CH_CFG_USE_MUTEXES && !CH_CFG_USE_SEMAPHORES
#error "NAND_USE_MUTUAL_EXCLUSION requires CH_CFG_USE_MUTEXES and/or CH_CFG_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  NAND_UNINIT = 0,                   /**< Not initialized.                */
  NAND_STOP = 1,                     /**< Stopped.                        */
  NAND_READY = 2,                    /**< Ready.                          */
  NAND_PROGRAM = 3,                  /**< Programming in progress.        */
  NAND_ERASE = 4,                    /**< Erasing in progress.            */
  NAND_WRITE = 5,                    /**< Writing to NAND buffer.         */
  NAND_READ = 6,                     /**< Reading from NAND.              */
  NAND_DMA_TX = 7,                   /**< DMA transmitting.               */
  NAND_DMA_RX = 8,                   /**< DMA receiving.                  */
} nandstate_t;

/**
 * @brief   Type of a structure representing a NAND driver.
 */
typedef struct NANDDriver NANDDriver;

#include "nand_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void nandInit(void);
  void nandObjectInit(NANDDriver *nandp);
  void nandStart(NANDDriver *nandp, const NANDConfig *config);
  void nandStop(NANDDriver *nandp);
  void nandReadPageWhole(NANDDriver *nandp, uint32_t block,
          uint32_t page, uint8_t *data, size_t datalen);
  uint8_t nandWritePageWhole(NANDDriver *nandp, uint32_t block,
          uint32_t page, const uint8_t *data, size_t datalen);
  void nandReadPageData(NANDDriver *nandp, uint32_t block,
          uint32_t page, uint8_t *data, size_t datalen, uint32_t *ecc);
  uint8_t nandWritePageData(NANDDriver *nandp, uint32_t block,
          uint32_t page, const uint8_t *data, size_t datalen, uint32_t *ecc);
  void nandReadPageSpare(NANDDriver *nandp, uint32_t block,
          uint32_t page, uint8_t *spare, size_t sparelen);
  uint8_t nandWritePageSpare(NANDDriver *nandp, uint32_t block,
          uint32_t page, const uint8_t *spare, size_t sparelen);
  void nandMarkBad(NANDDriver *nandp, uint32_t block);
  uint8_t nandReadBadMark(NANDDriver *nandp,
          uint32_t block, uint32_t page);
  uint8_t nandErase(NANDDriver *nandp, uint32_t block);
  bool nandIsBad(NANDDriver *nandp, uint32_t block);

#if NAND_USE_MUTUAL_EXCLUSION
  void nandAcquireBus(NANDDriver *nandp);
  void nandReleaseBus(NANDDriver *nandp);
#endif /* NAND_USE_MUTUAL_EXCLUSION */

#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_NAND */

#endif /* _NAND_H_ */

/** @} */
