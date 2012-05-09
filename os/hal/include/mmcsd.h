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
*/

/**
 * @file    mmcsd.h
 * @brief   MMC/SD cards header.
 *
 * @addtogroup MMCSD
 * @{
 */

#ifndef _MMCSD_H_
#define _MMCSD_H_

#if HAL_USE_MMC_SPI || HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Fixed block size for MMC/SD block devices.
 */
#define SDMMC_BLOCK_SIZE                512

/**
 * @brief   Mask of error bits in R1 responses.
 */
#define SDMMC_R1_ERROR_MASK             0xFDFFE008

/**
 * @brief   Fixed pattern for CMD8.
 */
#define SDMMC_CMD8_PATTERN              0x000001AA

/**
 * @name    SD/MMC status conditions
 * @{
 */
#define SDMMC_STS_IDLE                  0
#define SDMMC_STS_READY                 1
#define SDMMC_STS_IDENT                 2
#define SDMMC_STS_STBY                  3
#define SDMMC_STS_TRAN                  4
#define SDMMC_STS_DATA                  5
#define SDMMC_STS_RCV                   6
#define SDMMC_STS_PRG                   7
#define SDMMC_STS_DIS                   8
/** @} */

/**
 * @name    SD/MMC commands
 * @{
 */
#define SDMMC_CMD_GO_IDLE_STATE         0
#define SDMMC_CMD_INIT                  1
#define SDMMC_CMD_ALL_SEND_CID          2
#define SDMMC_CMD_SEND_RELATIVE_ADDR    3
#define SDMMC_CMD_SET_BUS_WIDTH         6
#define SDMMC_CMD_SEL_DESEL_CARD        7
#define SDMMC_CMD_SEND_IF_COND          8
#define SDMMC_CMD_SEND_CSD              9
#define SDMMC_CMD_STOP_TRANSMISSION     12
#define SDMMC_CMD_SEND_STATUS           13
#define SDMMC_CMD_SET_BLOCKLEN          16
#define SDMMC_CMD_READ_SINGLE_BLOCK     17
#define SDMMC_CMD_READ_MULTIPLE_BLOCK   18
#define SDMMC_CMD_SET_BLOCK_COUNT       23
#define SDMMC_CMD_WRITE_BLOCK           24
#define SDMMC_CMD_WRITE_MULTIPLE_BLOCK  25
#define SDMMC_CMD_APP_OP_COND           41
#define SDMMC_CMD_LOCK_UNLOCK           42
#define SDMMC_CMD_APP_CMD               55
#define SDMMC_CMD_READ_OCR              58
/** @} */

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
 * @brief   @p MMCSDBlockDevice specific methods.
 */
#define _mmcsd_block_device_methods                                         \
  _base_block_device_methods

/**
 * @brief   @p MMCSDBlockDevice specific data.
 * @note    It is empty because @p MMCSDBlockDevice is only an interface
 *          without implementation.
 */
#define _mmcsd_block_device_data                                            \
  _base_block_device_data

/**
 * @extends BaseBlockDeviceVMT
 *
 * @brief   @p MMCSDBlockDevice virtual methods table.
 */
struct MMCSDBlockDeviceVMT {
  _base_block_device_methods
};

/**
 * @extends BaseBlockDevice
 *
 * @brief   MCC/SD block device class.
 * @details This class represents a, block-accessible, MMC/SD device.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct MMCSDBlockDeviceVMT *vmt;
  _mmcsd_block_device_data
} MMCSDBlockDevice;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_MMC_SPI || HAL_USE_MMC_SDC*/

#endif /* _MMCSD_H_ */

/** @} */
