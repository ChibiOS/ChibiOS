/*
    ChibiOS - Copyright (C) 2016 Rocco Marco Guglielmi

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
 * @file    lis3dsh.h
 * @brief   LIS3DSH MEMS interface module header.
 *
 * @{
 */

#ifndef _LIS3DSH_H_
#define _LIS3DSH_H_

#include "hal_accelerometer.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   LIS3DSH number of axes.
 */
#define  LIS3DSH_NUMBER_OF_AXES             ((size_t)   3U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options.
 * @{
 */
/**
 * @brief   LIS3DSH SPI interface switch.
 * @details If set to @p TRUE the support for SPI is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LIS3DSH_USE_SPI) || defined(__DOXYGEN__)
#define LIS3DSH_USE_SPI                     TRUE
#endif

/**
 * @brief   LIS3DSH I2C interface switch.
 * @details If set to @p TRUE the support for I2C is included.
 * @note    The default is @p FALSE.
 */
#if !defined(LIS3DSH_USE_I2C) || defined(__DOXYGEN__)
#define LIS3DSH_USE_I2C                     FALSE
#endif

/**
 * @brief   LIS3DSH shared SPI switch.
 * @details If set to @p TRUE the device acquires SPI bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires SPI_USE_MUTUAL_EXCLUSION.
 */
#if !defined(LIS3DSH_SHARED_SPI) || defined(__DOXYGEN__)
#define LIS3DSH_SHARED_SPI                  FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !(LIS3DSH_USE_SPI ^ LIS3DSH_USE_I2C)
#error "LIS3DSH_USE_SPI and LIS3DSH_USE_I2C cannot be both true or both false"
#endif

#if LIS3DSH_USE_SPI && !HAL_USE_SPI
#error "LIS3DSH_USE_SPI requires HAL_USE_SPI"
#endif

#if LIS3DSH_USE_I2C && !HAL_USE_I2C
#error "LIS3DSH_USE_I2C requires HAL_USE_I2C"
#endif

#if LIS3DSH_SHARED_SPI && !SPI_USE_MUTUAL_EXCLUSION
#error "LIS3DSH_SHARED_SPI requires SPI_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @name    LIS3DSH data structures and types
 * @{
 */
/**
 * @brief   LIS3DSH full scale.
 */
typedef enum {
  LIS3DSH_FS_2G = 0x00,             /**< Full scale ±2g.                    */
  LIS3DSH_FS_4G = 0x08,             /**< Full scale ±4g.                    */
  LIS3DSH_FS_6G = 0x10,             /**< Full scale ±6g.                    */
  LIS3DSH_FS_8G = 0x18,             /**< Full scale ±8g.                    */
  LIS3DSH_FS_16G = 0x20             /**< Full scale ±16g.                   */
}lis3dsh_fs_t;

/**
 * @brief   LIS3DSH output data rate.
 */
typedef enum {
  LIS3DSH_ODR_PD = 0x00,            /**< ODR 100 Hz.                        */
  LIS3DSH_ODR_3_125HZ = 0x10,       /**< ODR 3.125 Hz.                      */
  LIS3DSH_ODR_6_25HZ = 0x20,        /**< ODR 6.25 Hz.                       */
  LIS3DSH_ODR_12_5HZ = 0x30,        /**< ODR 12.5 Hz.                       */
  LIS3DSH_ODR_25HZ = 0x40,          /**< ODR 25 Hz.                         */
  LIS3DSH_ODR_50HZ = 0x50,          /**< ODR 50 Hz.                         */
  LIS3DSH_ODR_100HZ = 0x60,         /**< ODR 100 Hz.                        */
  LIS3DSH_ODR_400HZ = 0x70,         /**< ODR 400 Hz.                        */
  LIS3DSH_ODR_800HZ = 0x80,         /**< ODR 800 Hz.                        */
  LIS3DSH_ODR_1600HZ = 0x90         /**< ODR 1600 Hz.                       */
}lis3dsh_odr_t;

/**
 * @brief   LIS3DSH anti-aliasing bandwidth.
 */
typedef enum {
  LIS3DSH_BW_800HZ = 0x00,          /**< AA filter BW 800Hz.                */
  LIS3DSH_BW_200HZ = 0x40,          /**< AA filter BW 200Hz.                */
  LIS3DSH_BW_400HZ = 0x80,          /**< AA filter BW 400Hz.                */
  LIS3DSH_BW_50HZ = 0xC0            /**< AA filter BW 50Hz.                 */
}lis3dsh_bw_t;

/**
 * @brief   LIS3DSH axes enabling.
 */
typedef enum {
  LIS3DSH_AE_DISABLED = 0x00,       /**< All axes disabled.                 */
  LIS3DSH_AE_X = 0x01,              /**< Only X-axis enabled.               */
  LIS3DSH_AE_Y = 0x02,              /**< Only Y-axis enabled.               */
  LIS3DSH_AE_XY = 0x03,             /**< X and Y axes enabled.              */
  LIS3DSH_AE_Z = 0x04,              /**< Only Z-axis enabled.               */
  LIS3DSH_AE_XZ = 0x05,             /**< X and Z axes enabled.              */
  LIS3DSH_AE_YZ = 0x06,             /**< Y and Z axes enabled.              */
  LIS3DSH_AE_XYZ = 0x07             /**< All axes enabled.                  */
}lis3dsh_ae_t;

/**
 * @brief   LIS3DSH block data update.
 */
typedef enum {
  LIS3DSH_BDU_CONTINUOUS = 0x00,    /**< Block data continuously updated.   */
  LIS3DSH_BDU_BLOCKED = 0x80        /**< Block data updated after reading.  */
} lis3dsh_bdu_t;

/**
 * @brief   LIS3DSH accelerometer subsystem unit.
 */
typedef enum {
  LIS3DSH_ACC_UNIT_G = 0x00,        /**< Cooked data in g.                  */
  LIS3DSH_ACC_UNIT_MG = 0x01,       /**< Cooked data in mg.                 */
  LIS3DSH_ACC_UNIT_SI = 0x02,       /**< Cooked data in m/s^2.              */
} lis3dsh_acc_unit_t;

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  LIS3DSH_UNINIT = 0,               /**< Not initialized.                   */
  LIS3DSH_STOP = 1,                 /**< Stopped.                           */
  LIS3DSH_READY = 2,                /**< Ready.                             */
} lis3dsh_state_t;

/**
 * @brief   LIS3DSH configuration structure.
 */
typedef struct {

#if (LIS3DSH_USE_SPI) || defined(__DOXYGEN__)
  /**
   * @brief SPI driver associated to this LIS3DSH.
   */
  SPIDriver                 *spip;
  /**
   * @brief SPI configuration associated to this LIS3DSH.
   */
  const SPIConfig           *spicfg;
#endif /* LIS3DSH_USE_SPI */
#if (LIS3DSH_USE_I2C) || defined(__DOXYGEN__)
  /**
   * @brief I2C driver associated to this LIS3DSH.
   */
  I2CDriver                 *i2cp;
  /**
   * @brief I2C configuration associated to this LIS3DSH.
   */
  const I2CConfig           *i2ccfg;
#endif /* LIS3DSH_USE_I2C */
  /**
   * @brief LIS3DSH full scale value.
   */
  lis3dsh_fs_t               fullscale;
  /**
   * @brief LIS3DSH output data rate selection.
   */
  lis3dsh_odr_t              outputdatarate;
  /**
   * @brief   LIS3DSH anti-aliasing bandwidth.
   */
  lis3dsh_bw_t               antialiasing;
  /**
   * @brief LIS3DSH axes enabling.
   */
  lis3dsh_ae_t               axesenabling;
  /**
   * @brief   LIS3DSH block data update.
   */  
  lis3dsh_bdu_t              blockdataupdate;
  /**
   * @brief LIS3DSH unit for cooked data.
   */
  lis3dsh_acc_unit_t         unit;
} LIS3DSHConfig;

/**
 * @brief   Structure representing a LIS3DSH driver.
 */
typedef struct LIS3DSHDriver LIS3DSHDriver;

/**
 * @brief   @p LIS3DSH specific methods.
 */
#define _lis3dsh_methods                                                    \
  _base_accelerometer_methods                                               \
  /* Retrieve the temperature of L3GD20 chip.*/                             \
  msg_t (*get_temperature)(void *instance, int8_t* temperature);

/**
 * @extends BaseAccelerometerVMT
 *
 * @brief   @p LIS3DSH virtual methods table.
 */
struct LIS3DSHVMT {
  _lis3dsh_methods
};

/**
 * @brief   @p LIS3DSHDriver specific data.
 */
#define _lis3dsh_data                                                       \
  _base_accelerometer_data                                                  \
  /* Driver state.*/                                                        \
  lis3dsh_state_t            state;                                         \
  /* Current configuration data.*/                                          \
  const LIS3DSHConfig        *config;                                       \
  /* Current sensitivity.*/                                                 \
  float                     sensitivity[LIS3DSH_NUMBER_OF_AXES];            \
  /* Bias data.*/                                                           \
  int32_t                   bias[LIS3DSH_NUMBER_OF_AXES];

/**
 * @extends BaseAccelerometer
 *
 * @brief   LIS3DSH 3-axis accelerometer class.
 * @details This class extends @p BaseAccelerometer by adding physical
 *          driver implementation.
 */
struct LIS3DSHDriver {
  /** @brief BaseSensor Virtual Methods Table. */
  const struct BaseSensorVMT *vmt_basesensor;
  /** @brief BaseAccelerometer Virtual Methods Table. */
  const struct BaseAccelerometerVMT *vmt_baseaccelerometer;
  /** @brief LIS3DSH Virtual Methods Table. */
  const struct LIS3DSHVMT *vmt_lis3dsh;
  _lis3dsh_data
};
/** @} */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Get current MEMS temperature.
 * @detail  This information is very useful especially for high accuracy IMU
 * @note    Value is raw since there is a lack of information in datasheet.
 *
 * @param[in] ip        pointer to a @p BaseAccelerometer class.
 * @param[out] temp     the MEMS temperature as raw data.
 *
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 * @api
 */
#define accelerometerGetTemp(ip, tpp)                                       \
        (ip)->vmt_lis3dsh->get_temperature(ip, tpp)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void lis3dshObjectInit(LIS3DSHDriver *devp);
  void lis3dshStart(LIS3DSHDriver *devp, const LIS3DSHConfig *config);
  void lis3dshStop(LIS3DSHDriver *devp);
#ifdef __cplusplus
}
#endif

#endif /* _LIS3DSH_H_ */

/** @} */

