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
 * @file    lis302dl.h
 * @brief   LIS302DL MEMS interface module header.
 *
 * @{
 */

#ifndef _LIS302DL_H_
#define _LIS302DL_H_

#include "hal_accelerometer.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   LIS302DL number of axes.
 */
#define  LIS302DL_NUMBER_OF_AXES            ((size_t)   3U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   LIS302DL SPI interface switch.
 * @details If set to @p TRUE the support for SPI is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LIS302DL_USE_SPI) || defined(__DOXYGEN__)
#define LIS302DL_USE_SPI                    TRUE
#endif

/**
 * @brief   LIS302DL I2C interface switch.
 * @details If set to @p TRUE the support for I2C is included.
 * @note    The default is @p FALSE.
 */
#if !defined(LIS302DL_USE_I2C) || defined(__DOXYGEN__)
#define LIS302DL_USE_I2C                    FALSE
#endif

/**
 * @brief   LIS302DL shared SPI switch.
 * @details If set to @p TRUE the device acquires SPI bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires SPI_USE_MUTUAL_EXCLUSION.
 */
#if !defined(LIS302DL_SHARED_SPI) || defined(__DOXYGEN__)
#define LIS302DL_SHARED_SPI                 FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !(LIS302DL_USE_SPI ^ LIS302DL_USE_I2C)
#error "LIS302DL_USE_SPI and LIS302DL_USE_I2C cannot be both true or both false"
#endif

#if LIS302DL_USE_SPI && !HAL_USE_SPI
#error "LIS302DL_USE_SPI requires HAL_USE_SPI"
#endif

#if LIS302DL_USE_I2C && !HAL_USE_I2C
#error "LIS302DL_USE_I2C requires HAL_USE_I2C"
#endif

#if LIS302DL_SHARED_SPI && !SPI_USE_MUTUAL_EXCLUSION
#error "LIS302DL_SHARED_SPI requires SPI_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @name    LIS302DL data structures and types
 * @{
 */
/**
 * @brief   LIS302DL full scale.
 */
typedef enum {
  LIS302DL_FS_2G = 0x00,            /**< Full scale ±2g.                    */
  LIS302DL_FS_8G = 0x20             /**< Full scale ±8g.                    */
}lis302dl_fs_t;

/**
 * @brief   LIS302DL output data rate and bandwidth.
 */
typedef enum {
  LIS302DL_ODR_100HZ = 0x00,        /**< ODR 100 Hz.                        */
  LIS302DL_ODR_400HZ = 0x80         /**< ODR 400 Hz.                        */
}lis302dl_odr_t;

/**
 * @brief   LIS302DL high pass filtering.
 */
typedef enum {
  LIS302DL_HP_DISABLED = 0x00,      /**< HP bypassed.                       */
  LIS302DL_HP_0 = 0x10,             /**< HP cutoff 2Hz (ODR 100Hz) or 8Hz   */
  LIS302DL_HP_1 = 0x11,             /**< HP cutoff 1Hz or 4Hz               */
  LIS302DL_HP_2 = 0x12,             /**< HP cutoff 0.5Hz or 2Hz             */
  LIS302DL_HP_3 = 0x13              /**< HP cutoff 0.25Hz or 1Hz            */
}lis302dl_hp_t;

/**
 * @brief   LIS302DL axes enabling.
 */
typedef enum {
  LIS302DL_AE_DISABLED = 0x00,      /**< All axes disabled.                 */
  LIS302DL_AE_X = 0x01,             /**< Only X-axis enabled.               */
  LIS302DL_AE_Y = 0x02,             /**< Only Y-axis enabled.               */
  LIS302DL_AE_XY = 0x03,            /**< X and Y axes enabled.              */
  LIS302DL_AE_Z = 0x04,             /**< Only Z-axis enabled.               */
  LIS302DL_AE_XZ = 0x05,            /**< X and Z axes enabled.              */
  LIS302DL_AE_YZ = 0x06,            /**< Y and Z axes enabled.              */
  LIS302DL_AE_XYZ = 0x07            /**< All axes enabled.                  */
}lis302dl_ae_t;

/**
 * @brief   LIS302DL accelerometer subsystem unit.
 */
typedef enum {
  LIS302DL_ACC_UNIT_G = 0x00,       /**< Cooked data in g.                  */
  LIS302DL_ACC_UNIT_MG = 0x01,      /**< Cooked data in mg.                 */
  LIS302DL_ACC_UNIT_SI = 0x02,      /**< Cooked data in m/s^2.              */
} lis302dl_acc_unit_t;

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  LIS302DL_UNINIT = 0,              /**< Not initialized.                   */
  LIS302DL_STOP = 1,                /**< Stopped.                           */
  LIS302DL_READY = 2,               /**< Ready.                             */
} lis302dl_state_t;

/**
 * @brief   LIS302DL configuration structure.
 */
typedef struct {

#if (LIS302DL_USE_SPI) || defined(__DOXYGEN__)
  /**
   * @brief SPI driver associated to this LIS302DL.
   */
  SPIDriver                 *spip;
  /**
   * @brief SPI configuration associated to this LIS302DL.
   */
  const SPIConfig           *spicfg;
#endif /* LIS302DL_USE_SPI */
#if (LIS302DL_USE_I2C) || defined(__DOXYGEN__)
  /**
   * @brief I2C driver associated to this LIS302DL.
   */
  I2CDriver                 *i2cp;
  /**
   * @brief I2C configuration associated to this LIS302DL.
   */
  const I2CConfig           *i2ccfg;
#endif /* LIS302DL_USE_I2C */
  /**
   * @brief LIS302DL full scale value.
   */
  lis302dl_fs_t               fullscale;
  /**
   * @brief LIS302DL output data rate selection.
   */
  lis302dl_odr_t              outputdatarate;
  /**
   * @brief   LIS302DL high pass filtering
   */
  lis302dl_hp_t               highpass;
  /**
   * @brief LIS302DL axes enabling.
   */
  lis302dl_ae_t               axesenabling;
  /**
   * @brief LIS302DL unit for cooked data.
   */
  lis302dl_acc_unit_t         unit;
} LIS302DLConfig;

/**
 * @brief   Structure representing a LIS302DL driver.
 */
typedef struct LIS302DLDriver LIS302DLDriver;

/**
 * @brief   @p LIS302DL specific methods.
 */
#define _lis302dl_methods                                                   \
  _base_accelerometer_methods


/**
 * @extends BaseAccelerometerVMT.
 *
 * @brief   @p LIS302DL virtual methods table.
 */
struct LIS302DLVMT {
  _lis302dl_methods
};

/**
 * @brief   @p LIS302DLDriver specific data.
 */
#define _lis302dl_data                                                      \
  _base_accelerometer_data                                                  \
  /* Driver state.*/                                                        \
  lis302dl_state_t            state;                                        \
  /* Current configuration data.*/                                          \
  const LIS302DLConfig        *config;                                      \
  /* Current sensitivity.*/                                                 \
  float                     sensitivity[LIS302DL_NUMBER_OF_AXES];           \
  /* Bias data.*/                                                           \
  int32_t                   bias[LIS302DL_NUMBER_OF_AXES];

/**
 * @extends BaseAccelerometer.
 *
 * @brief   LIS302DL 3-axis accelerometer class.
 * @details This class extends @p BaseAccelerometer by adding physical
 *          driver implementation.
 */
struct LIS302DLDriver {
  /** @brief BaseSensor Virtual Methods Table. */
  const struct BaseSensorVMT *vmt_basesensor;
  /** @brief BaseAccelerometer Virtual Methods Table. */
  const struct BaseAccelerometerVMT *vmt_baseaccelerometer;
  /** @brief LIS302DL Virtual Methods Table. */
  const struct LIS302DLVMT *vmt_lis302dl;
  _lis302dl_data
};
/** @} */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Get current MEMS temperature.
 * @detail  This information is very useful especially for high accuracy IMU.
 *
 * @param[in] ip        pointer to a @p BaseAccelerometer class.
 * @param[out] temp     the MEMS temperature as single precision floating.
 *
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 * @api
 */
#define accelerometerGetTemp(ip, tpp)                                           \
        (ip)->vmt_lis302dl->get_temperature(ip, tpp)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void lis302dlObjectInit(LIS302DLDriver *devp);
  void lis302dlStart(LIS302DLDriver *devp, const LIS302DLConfig *config);
  void lis302dlStop(LIS302DLDriver *devp);
#ifdef __cplusplus
}
#endif

#endif /* _LIS302DL_H_ */

/** @} */

