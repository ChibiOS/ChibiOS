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
 * @file    lis3mdl.h
 * @brief   LIS3MDL MEMS interface module header.
 *
 * @{
 */

#ifndef _LIS3MDL_H_
#define _LIS3MDL_H_

#include "hal_compass.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   LIS3MDL number of axes.
 */
#define  LIS3MDL_NUMBER_OF_AXES             ((size_t)   3U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   LIS3MDL SPI interface switch.
 * @details If set to @p TRUE the support for SPI is included.
 * @note    The default is @p FALSE.
 */
#if !defined(LIS3MDL_USE_SPI) || defined(__DOXYGEN__)
#define LIS3MDL_USE_SPI                     FALSE
#endif

/**
 * @brief   LIS3MDL I2C interface switch.
 * @details If set to @p TRUE the support for I2C is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LIS3MDL_USE_I2C) || defined(__DOXYGEN__)
#define LIS3MDL_USE_I2C                     TRUE
#endif

/**
 * @brief   LIS3MDL shared I2C switch.
 * @details If set to @p TRUE the device acquires I2C bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires I2C_USE_MUTUAL_EXCLUSION
 */
#if !defined(LIS3MDL_SHARED_I2C) || defined(__DOXYGEN__)
#define LIS3MDL_SHARED_I2C                  FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !(LIS3MDL_USE_SPI ^ LIS3MDL_USE_I2C)
#error "LIS3MDL_USE_SPI and LIS3MDL_USE_I2C cannot be both true or both false"
#endif

#if LIS3MDL_USE_SPI && !HAL_USE_SPI
#error "LIS3MDL_USE_SPI requires HAL_USE_SPI"
#endif

#if LIS3MDL_USE_I2C && !HAL_USE_I2C
#error "LIS3MDL_USE_I2C requires HAL_USE_I2C"
#endif

#if LIS3MDL_SHARED_I2C && !I2C_USE_MUTUAL_EXCLUSION
#error "LIS3MDL_SHARED_I2C requires I2C_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @name    LIS3MDL data structures and types
 * @{
 */
/**
 * @brief  LIS3MDL slave address
 */
typedef enum {
  LIS3MDL_SAD_GND = 0x1C,           /**< Slave Address when SA1 is to GND   */
  LIS3MDL_SAD_VCC = 0x1E            /**< Slave Address when SA1 is to VCC   */
}lis3mdl_sad_t;

/**
 * @brief   LIS3MDL full scale
 */
typedef enum {
  LIS3MDL_FS_4GA = 0x00,            /**< ±4 Gauss                           */
  LIS3MDL_FS_8GA = 0x02,            /**< ±8 Gauss                           */
  LIS3MDL_FS_12GA = 0x04,           /**< ±12 Gauss                          */
  LIS3MDL_FS_16GA = 0x0C            /**< ±16 Gauss                          */
}lis3mdl_fs_t;

/**
 * @brief   LIS3MDL output data rate
 */
typedef enum {
  LIS3MDL_ODR_0_625HZ = 0x00,       /**< Output Data Rate = 0.625 Hz        */
  LIS3MDL_ODR_1_25HZ = 0x04,        /**< Output Data Rate = 1.25 Hz         */
  LIS3MDL_ODR_2_5HZ = 0x08,         /**< Output Data Rate = 2.5 Hz          */
  LIS3MDL_ODR_5HZ = 0x0C,           /**< Output Data Rate = 5 Hz            */
  LIS3MDL_ODR_10HZ = 0x10,          /**< Output Data Rate = 10 Hz           */
  LIS3MDL_ODR_20HZ = 0x14,          /**< Output Data Rate = 20 Hz           */
  LIS3MDL_ODR_40HZ = 0x18,          /**< Output Data Rate = 40 Hz           */
  LIS3MDL_ODR_80HZ = 0x1C           /**< Output Data Rate = 80 Hz           */
}lis3mdl_odr_t;

/**
 * @brief  LIS3MDL low power mode configuration
 */
typedef enum {
  LIS3MDL_LP_DISABLED = 0x00,       /**< Low Power mode disabled            */
  LIS3MDL_LP_ENABLED = 0x20         /**< Low Power mode enabled             */
}lis3mdl_lp_t;

/**
 * @brief  LIS3MDL conversion mode
 */
typedef enum {
  LIS3MDL_MD_CONTINUOUS = 0x00,     /**< Continuous conversion mode         */
  LIS3MDL_MD_SINGLE = 0x01,         /**< Single conversion mode             */
  LIS3MDL_MD_POWER_DOWN = 0x02      /**< Power down mode                    */
}lis3mdl_md_t;

/**
 * @brief  LIS3MDL operation mode for X and Y axes
 */
typedef enum {
  LIS3MDL_OMXY_LOW_POWER  = 0x00,   /**< X-Y axes low power mode            */
  LIS3MDL_OMXY_MEDIUM = 0x20,       /**< X-Y axes medium performance mode   */
  LIS3MDL_OMXY_HIGH = 0x40,         /**< X-Y axes high performance mode     */
  LIS3MDL_OMXY_ULTRA = 0x60         /**< X-Y axes ultra performance mode    */
}lis3mdl_omxy_t;

/**
 * @brief  LIS3MDL operation mode for Z axis
 */
typedef enum {
  LIS3MDL_OMZ_LOW_POWER  = 0x00,    /**< Z axis low power mode              */
  LIS3MDL_OMZ_MEDIUM = 0x04,        /**< Z axis medium performance mode     */
  LIS3MDL_OMZ_HIGH = 0x08,          /**< Z axis high performance mode       */
  LIS3MDL_OMZ_ULTRA = 0x0C          /**< Z axis ultra performance mode      */
}lis3mdl_omz_t;

/**
 * @brief   LIS3MDL temperature sensor enabling
 */
typedef enum {
  LIS3MDL_TEMP_DISABLED = 0x00,     /**< Temperature sensor disabled.       */
  LIS3MDL_TEMP_ENABLED = 0x80       /**< Temperature sensor enabled.        */
}lis3mdl_temp_t;

/**
 * @brief  LIS3MDL block data update
 */
typedef enum {
  LIS3MDL_BDU_CONTINUOUS = 0x00,    /**< Continuous Update                  */
  LIS3MDL_BDU_BLOCKED = 0x40        /**< Block data updated after reading.  */
}lis3mdl_bdu_t;

/**
 * @brief   LIS3MDL endianness
 */
typedef enum {
  LIS3MDL_END_LITTLE = 0x00,        /**< Little endian.                     */ 
  LIS3MDL_END_BIG = 0x02            /**< Big endian.                        */
}lis3mdl_end_t;

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  LIS3MDL_UNINIT = 0,               /**< Not initialized.                   */
  LIS3MDL_STOP = 1,                 /**< Stopped.                           */
  LIS3MDL_READY = 2,                /**< Ready.                             */
} lis3mdl_state_t;

/**
 * @brief   LIS3MDL configuration structure.
 */
typedef struct {

#if (LIS3MDL_USE_SPI) || defined(__DOXYGEN__)
  /**
   * @brief SPI driver associated to this LIS3MDL.
   */
  SPIDriver                 *spip;
  /**
   * @brief SPI configuration associated to this LIS3MDL.
   */
  const SPIConfig           *spicfg;
#endif /* LIS3MDL_USE_SPI */
#if (LIS3MDL_USE_I2C) || defined(__DOXYGEN__)
  /**
   * @brief I2C driver associated to this LIS3MDL.
   */
  I2CDriver                 *i2cp;
  /**
   * @brief I2C configuration associated to this LIS3MDL.
   */
  const I2CConfig           *i2ccfg;
#endif /* LIS3MDL_USE_I2C */
  /**
   * @brief  LIS3MDL slave address
   */
  lis3mdl_sad_t              slaveaddress;
  /**
   * @brief   LIS3MDL full scale
   */
  lis3mdl_fs_t               fullscale;
  /**
   * @brief   LIS3MDL output data rate
   */
  lis3mdl_odr_t              outputdatarate;
  /**
   * @brief  LIS3MDL low power mode configuration
   */
  lis3mdl_lp_t               lowpowermode;
  /**
   * @brief  LIS3MDL conversion mode
   */
  lis3mdl_md_t               conversionmode;
  /**
   * @brief  LIS3MDL operation mode for X and Y axes
   */
  lis3mdl_omxy_t             operationmodexy;
  /**
   * @brief  LIS3MDL operation mode for Z axis
   */
  lis3mdl_omz_t              operationmodez;
  /**
   * @brief   LIS3MDL temperature sensor enabling
   */
  lis3mdl_temp_t             temperature;
  /**
   * @brief  LIS3MDL block data update
   */
  lis3mdl_bdu_t              blockdataupdate;
  /**
   * @brief   LIS3MDL endianness
   */
  lis3mdl_end_t              endianness;
} LIS3MDLConfig;

/**
 * @brief   Structure representing a LIS3MDL driver.
 */
typedef struct LIS3MDLDriver LIS3MDLDriver;

/**
 * @brief   @p LIS3MDL specific methods.
 */
#define _lis3mdl_methods                                                    \
  _base_compass_methods                                                     \
  /* Retrieve the temperature of LIS3MDL chip.*/                            \
  msg_t (*get_temperature)(void *instance, float* temperature);


/**
 * @extends BaseCompassVMT
 *
 * @brief   @p LIS3MDL virtual methods table.
 */
struct LIS3MDLVMT {
  _lis3mdl_methods
};

/**
 * @brief   @p LIS3MDLDriver specific data.
 */
#define _lis3mdl_data                                                       \
  _base_compass_data                                                        \
  /* Driver state.*/                                                        \
  lis3mdl_state_t           state;                                          \
  /* Current configuration data.*/                                          \
  const LIS3MDLConfig       *config;                                        \
  /* Current sensitivity.*/                                                 \
  float                     sensitivity[LIS3MDL_NUMBER_OF_AXES];            \
  /* Bias data.*/                                                           \
  int32_t                   bias[LIS3MDL_NUMBER_OF_AXES];

/**
 * @extends BaseCompass
 *
 * @brief   LIS3MDL 3-axis compass class.
 * @details This class extends @p BaseCompass by adding physical
 *          driver implementation.
 */
struct LIS3MDLDriver {
  /** @brief BaseSensor Virtual Methods Table. */
  const struct BaseSensorVMT *vmt_basesensor;
  /** @brief BaseCompass Virtual Methods Table. */
  const struct BaseCompassVMT *vmt_basecompass;
  /** @brief LIS3MDL Virtual Methods Table. */
  const struct LIS3MDLVMT *vmt_lis3mdl;
  _lis3mdl_data
};
/** @} */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Get current MEMS temperature.
 * @detail  This information is very useful especially for high accuracy IMU.
 * @note    Temperature sensor must be enabled using a proper configuration.
 *
 * @param[in] ip        pointer to a @p BaseCompass class.
 * @param[out] temp     the MEMS temperature as single precision floating.
 *
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 * @api
 */
#define compassGetTemp(ip, tpp)                                             \
        (ip)->vmt_lis3mdl->get_temperature(ip, tpp)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void lis3mdlObjectInit(LIS3MDLDriver *devp);
  void lis3mdlStart(LIS3MDLDriver *devp, const LIS3MDLConfig *config);
  void lis3mdlStop(LIS3MDLDriver *devp);
#ifdef __cplusplus
}
#endif

#endif /* _LIS3MDL_H_ */

/** @} */

