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
 * @file    emcnand.h
 * @brief   EMCNAND Driver macros and structures.
 *
 * @addtogroup EMCNAND
 * @{
 */

#ifndef _EMCNAND_H_
#define _EMCNAND_H_

#if HAL_USE_EMCNAND || defined(__DOXYGEN__)

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
#if !defined(EMCNAND_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define EMCNAND_USE_MUTUAL_EXCLUSION     FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/
#if EMCNAND_USE_MUTUAL_EXCLUSION && !CH_CFG_USE_MUTEXES && !CH_CFG_USE_SEMAPHORES
#error "EMCNAND_USE_MUTUAL_EXCLUSION requires CH_CFG_USE_MUTEXES and/or CH_CFG_USE_SEMAPHORES"
#endif

#if !HAL_USE_EMC
#error "This driver requires EMC controller"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  EMCNAND_UNINIT = 0,                   /**< Not initialized.                */
  EMCNAND_STOP = 1,                     /**< Stopped.                        */
  EMCNAND_READY = 2,                    /**< Ready.                          */
  EMCNAND_PROGRAM = 3,                  /**< Programming in progress.        */
  EMCNAND_ERASE = 4,                    /**< Erasing in progress.            */
  EMCNAND_WRITE = 5,                    /**< Writing to NAND buffer.         */
  EMCNAND_READ = 6,                     /**< Reading from NAND.              */
  EMCNAND_DMA_TX = 7,                   /**< DMA transmitting.               */
  EMCNAND_DMA_RX = 8,                   /**< DMA receiving.                  */
} emcnandstate_t;

/**
 * @brief   Type of a structure representing a EMCNAND driver.
 */
typedef struct EMCNANDDriver EMCNANDDriver;

#include "emcnand_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void emcnandInit(void);
  void emcnandObjectInit(EMCNANDDriver *emcnandp);
  void emcnandStart(EMCNANDDriver *emcnandp, const EMCNANDConfig *config);
  void emcnandStop(EMCNANDDriver *emcnandp);
  void emcnandReadPageWhole(EMCNANDDriver *emcnandp, uint32_t block,
          uint32_t page, uint8_t *data, size_t datalen);
  uint8_t emcnandWritePageWhole(EMCNANDDriver *emcnandp, uint32_t block,
          uint32_t page, const uint8_t *data, size_t datalen);
  void emcnandReadPageData(EMCNANDDriver *emcnandp, uint32_t block,
          uint32_t page, uint8_t *data, size_t datalen, uint32_t *ecc);
  uint8_t emcnandWritePageData(EMCNANDDriver *emcnandp, uint32_t block,
          uint32_t page, const uint8_t *data, size_t datalen, uint32_t *ecc);
  void emcnandReadPageSpare(EMCNANDDriver *emcnandp, uint32_t block,
          uint32_t page, uint8_t *spare, size_t sparelen);
  uint8_t emcnandWritePageSpare(EMCNANDDriver *emcnandp, uint32_t block,
          uint32_t page, const uint8_t *spare, size_t sparelen);
  void emcnandMarkBad(EMCNANDDriver *emcnandp, uint32_t block);
  uint8_t emcnandReadBadMark(EMCNANDDriver *emcnandp,
          uint32_t block, uint32_t page);
  uint8_t emcnandErase(EMCNANDDriver *emcnandp, uint32_t block);
  bool emcnandIsBad(EMCNANDDriver *emcnandp, uint32_t block);

#if EMCNAND_USE_MUTUAL_EXCLUSION
  void emcnandAcquireBus(EMCNANDDriver *emcnandp);
  void emcnandReleaseBus(EMCNANDDriver *emcnandp);
#endif /* EMCNAND_USE_MUTUAL_EXCLUSION */

#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EMCNAND */

#endif /* _EMCNAND_H_ */

/** @} */
