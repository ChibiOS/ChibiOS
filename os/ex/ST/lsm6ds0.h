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
 * @file    lsm6ds0.h
 * @brief   LSM6DS0 MEMS interface module header.
 *
 * @{
 */
#ifndef _LSM6DS0_H_
#define _LSM6DS0_H_

#include "hal_accelerometer.h"
#include "hal_gyroscope.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   LSM6DS0 accelerometer subsystem number of axes.
 */
#define  LSM6DS0_ACC_NUMBER_OF_AXES         ((size_t)   3U)

/**
 * @brief   LSM6DS0 gyroscope subsystem number of axes.
 */
#define  LSM6DS0_GYRO_NUMBER_OF_AXES        ((size_t)   3U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   LSM6DS0 SPI interface selector.
 * @details If set to @p TRUE the support for SPI is included.
 * @note    The default is @p FALSE.
 */
#if !defined(LSM6DS0_USE_SPI) || defined(__DOXYGEN__)
#define LSM6DS0_USE_SPI                     FALSE
#endif

/**
 * @brief   LSM6DS0 I2C interface selector.
 * @details If set to @p TRUE the support for I2C is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LSM6DS0_USE_I2C) || defined(__DOXYGEN__)
#define LSM6DS0_USE_I2C                     TRUE
#endif

/**
 * @brief   LSM6DS0 shared I2C switch.
 * @details If set to @p TRUE the device acquires I2C bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires I2C_USE_MUTUAL_EXCLUSION
 */
#if !defined(LSM6DS0_SHARED_I2C) || defined(__DOXYGEN__)
#define LSM6DS0_SHARED_I2C                  FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if LSM6DS0_USE_SPI && LSM6DS0_USE_I2C
#error "LSM6DS0_USE_SPI and LSM6DS0_USE_I2C cannot be both true"
#endif

#if LSM6DS0_USE_SPI && !HAL_USE_SPI
#error "LSM6DS0_USE_SPI requires HAL_USE_SPI"
#endif

#if LSM6DS0_USE_I2C && !HAL_USE_I2C
#error "LSM6DS0_USE_I2C requires HAL_USE_I2C"
#endif

#if LSM6DS0_SHARED_I2C && !I2C_USE_MUTUAL_EXCLUSION
#error "LSM6DS0_SHARED_SPI requires I2C_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @name    LSM6DS0 accelerometer subsystem data structures and types
 * @{
 */
 
/**
 * @brief   LSM6DS0 accelerometer subsystem full scale
 */
typedef enum {
  LSM6DS0_ACC_FS_2G = 0x00,         /**< Full scale ±2g.                    */
  LSM6DS0_ACC_FS_4G = 0x10,         /**< Full scale ±4g.                    */
  LSM6DS0_ACC_FS_8G = 0x18,         /**< Full scale ±8g.                    */
  LSM6DS0_ACC_FS_16G = 0x08         /**< Full scale ±16g.                   */
} lsm6ds0_acc_fs_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem output data rate
 */
typedef enum {
  LSM6DS0_ACC_ODR_PD = 0x00,        /**< Power down                         */
  LSM6DS0_ACC_ODR_10Hz = 0x20,      /**< ODR 10 Hz                          */
  LSM6DS0_ACC_ODR_50Hz = 0x40,      /**< ODR 50 Hz                          */
  LSM6DS0_ACC_ODR_119Hz = 0x60,     /**< ODR 119 Hz                         */
  LSM6DS0_ACC_ODR_238Hz = 0x80,     /**< ODR 238 Hz                         */
  LSM6DS0_ACC_ODR_476Hz = 0xA0,     /**< ODR 476 Hz                         */
  LSM6DS0_ACC_ODR_952Hz = 0xC0      /**< ODR 952 Hz                         */
} lsm6ds0_acc_odr_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem axes enabling
 */
typedef enum {
  LSM6DS0_ACC_AE_DISABLED = 0x00,   /**< All axes disabled.                 */
  LSM6DS0_ACC_AE_X = 0x08,          /**< Only X-axis enabled.               */
  LSM6DS0_ACC_AE_Y = 0x10,          /**< Only Y-axis enabled.               */
  LSM6DS0_ACC_AE_XY = 0x18,         /**< X and Y axes enabled.              */
  LSM6DS0_ACC_AE_Z = 0x20,          /**< Only Z-axis enabled.               */
  LSM6DS0_ACC_AE_XZ = 0x28,         /**< X and Z axes enabled.              */
  LSM6DS0_ACC_AE_YZ = 0x30,         /**< Y and Z axes enabled.              */
  LSM6DS0_ACC_AE_XYZ = 0x38         /**< All axes enabled.                  */
} lsm6ds0_acc_ae_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem anti aliasing filter bandwidth
 */
typedef enum {
  LSM6DS0_ACC_OBW_ODR = 0x00,      /**< Depending on ODR.                   */
  LSM6DS0_ACC_OBW_AA = 0x04        /**< Same of the Anti aliasing.          */
} lsm6ds0_acc_obw_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem high resolution mode
 */
typedef enum {
  LSM6DS0_ACC_HR_DISABLED = 0x00,   /**< High resolution mode disabled.     */
  LSM6DS0_ACC_HR_ENABLED = 0x80     /**< High resolution mode enabled.      */
} lsm6ds0_acc_hr_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem filtered data selection
 */
typedef enum {
  LSM6DS0_ACC_FDS_ENABLED = 0x00,   /**< Internal filter bypassed.          */
  LSM6DS0_ACC_FDS_DISABLED = 0x04   /**< Internal filter not bypassed.      */
} lsm6ds0_acc_fds_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem digital filter
 */
typedef enum {
  LSM6DS0_ACC_DCF_400 = 0x00,       /**< Low pass cutoff freq. ODR/400 Hz.  */
  LSM6DS0_ACC_DCF_100 = 0x20,       /**< Low pass cutoff freq. ODR/100 Hz.  */
  LSM6DS0_ACC_DCF_50 = 0x60,        /**< Low pass cutoff freq. ODR/50 Hz.   */
  LSM6DS0_ACC_DCF_9 = 0x40          /**< Low pass cutoff freq. ODR/9 Hz.    */
} lsm6ds0_acc_dcf_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem anti aliasing filter bandwidth
 */
typedef enum {
  LSM6DS0_ACC_AABW_408Hz = 0x00,    /**< Anti Aliasing filter BW 408Hz.     */
  LSM6DS0_ACC_AABW_211Hz = 0x01,    /**< Anti Aliasing filter BW 211Hz.     */
  LSM6DS0_ACC_AABW_105Hz = 0x02,    /**< Anti Aliasing filter BW 105Hz.     */
  LSM6DS0_ACC_AABW_50Hz = 0x03,     /**< Anti Aliasing filter BW 50Hz.      */
  LSM6DS0_ACC_AABW_AUTO = 0x04      /**< Anti Aliasing filter BW auto.      */
} lsm6ds0_acc_aabw_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem decimation mode
 */
typedef enum {
  LSM6DS0_ACC_DEC_DISABLED = 0x00,  /**< Decimation disabled.               */
  LSM6DS0_ACC_DEC_X2 = 0x40,        /**< Output updated every 2 samples.    */
  LSM6DS0_ACC_DEC_X4 = 0x80,        /**< Output updated every 4 samples.    */
  LSM6DS0_ACC_DEC_X8 = 0xC0         /**< Output updated every 8 samples.    */
} lsm6ds0_acc_dec_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem unit
 */
typedef enum {
  LSM6DS0_ACC_UNIT_G = 0x00,        /**< Cooked data in g.                  */
  LSM6DS0_ACC_UNIT_MG = 0x01,       /**< Cooked data in mg.                 */
  LSM6DS0_ACC_UNIT_SI = 0x02,       /**< Cooked data in m/s^2.              */
} lsm6ds0_acc_unit_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem configuration structure.
 */
typedef struct {
  /**
   * @brief   LSM6DS0 accelerometer subsystem full scale
   */
  lsm6ds0_acc_fs_t          fullscale;
  /**
   * @brief   LSM6DS0 accelerometer subsystem output data rate
   */
  lsm6ds0_acc_odr_t         outdatarate;
  /**
   * @brief   LSM6DS0 accelerometer subsystem axes enabling
   */
  lsm6ds0_acc_ae_t          axesenabling;
  /**
   * @brief   LSM6DS0 accelerometer subsystem output bandwidth
   */
  lsm6ds0_acc_obw_t         outbandwidth;
  /**
   * @brief   LSM6DS0 accelerometer subsystem high resolution mode
   */
  lsm6ds0_acc_hr_t          highresmode;
  /**
   * @brief   LSM6DS0 accelerometer subsystem filtered data selection
   */
  lsm6ds0_acc_fds_t         filtdatasel;
  /**
   * @brief   LSM6DS0 accelerometer subsystem digital filter
   */
  lsm6ds0_acc_dcf_t         digifilter;
  /**
   * @brief   LSM6DS0 accelerometer subsystem anti aliasing filter bandwidth
   */
  lsm6ds0_acc_aabw_t        aabandwidth;
  /**
   * @brief   LSM6DS0 accelerometer subsystem decimation mode
   */
  lsm6ds0_acc_dec_t         decmode;
  /**
   * @brief   LSM6DS0 accelerometer subsystem unit
   */
  lsm6ds0_acc_unit_t        unit;
} LSM6DS0AccConfig;
/** @} */

/**
 * @name    LSM6DS0 gyroscope subsystem data structures and types
 * @{
 */
/**
 * @brief   LSM6DS0 gyroscope subsystem full scale
 */
typedef enum {
  LSM6DS0_GYRO_FS_245DSP  = 0x00,   /**< Full scale Â±245 degree per second  */
  LSM6DS0_GYRO_FS_500DSP  = 0x08,   /**< Full scale Â±500 degree per second  */
  LSM6DS0_GYRO_FS_2000DSP = 0x18    /**< Full scale Â±2000 degree per second */
} lsm6ds0_gyro_fs_t;

/**
 * @brief   LSM6DS0 gyroscope subsystem output data rate
 */
typedef enum {
  LSM6DS0_GYRO_ODR_PD = 0x00,
  LSM6DS0_GYRO_ODR_95HZ_FC_25 = 0x10,
  LSM6DS0_GYRO_ODR_14_9HZ_FC_5 = 0X20,
  LSM6DS0_GYRO_ODR_59_5HZ_FC_16 = 0X40,
  LSM6DS0_GYRO_ODR_119HZ_FC_14 = 0X60,
  LSM6DS0_GYRO_ODR_119HZ_FC_31 = 0X61,
  LSM6DS0_GYRO_ODR_238HZ_FC_14 = 0X80,
  LSM6DS0_GYRO_ODR_238HZ_FC_29 = 0X81,
  LSM6DS0_GYRO_ODR_238HZ_FC_63 = 0X82,
  LSM6DS0_GYRO_ODR_238HZ_FC_78 = 0X83,
  LSM6DS0_GYRO_ODR_476HZ_FC_21 = 0XA0,
  LSM6DS0_GYRO_ODR_476HZ_FC_28 = 0XA1,
  LSM6DS0_GYRO_ODR_476HZ_FC_57 = 0XA2,
  LSM6DS0_GYRO_ODR_476HZ_FC_100 = 0XA3,
  LSM6DS0_GYRO_ODR_952HZ_FC_33 = 0XC0,
  LSM6DS0_GYRO_ODR_952HZ_FC_40 = 0XC1,
  LSM6DS0_GYRO_ODR_952HZ_FC_58 = 0XC2,
  LSM6DS0_GYRO_ODR_952HZ_FC_100 = 0XC3
} lsm6ds0_gyro_odr_t;

/**
 * @brief   LSM6DS0 gyroscope subsystem axes enabling
 */
typedef enum {
  LSM6DS0_GYRO_AE_DISABLED = 0x00,  /**< All axes disabled.                 */
  LSM6DS0_GYRO_AE_X = 0x08,         /**< Only X-axis enabled.               */
  LSM6DS0_GYRO_AE_Y = 0x10,         /**< Only Y-axis enabled.               */
  LSM6DS0_GYRO_AE_XY = 0x18,        /**< X and Y axes enabled.              */
  LSM6DS0_GYRO_AE_Z = 0x20,         /**< Only Z-axis enabled.               */
  LSM6DS0_GYRO_AE_XZ = 0x28,        /**< X and Z axes enabled.              */
  LSM6DS0_GYRO_AE_YZ = 0x30,        /**< Y and Z axes enabled.              */
  LSM6DS0_GYRO_AE_XYZ = 0x38        /**< All axes enabled.                  */
} lsm6ds0_gyro_ae_t;

/**
 * @brief   LSM6DS0 gyroscope subsystem low mode configuration
 */
typedef enum {
  LSM6DS0_GYRO_LP_DISABLED = 0x00,  /**< Low power mode disabled.           */
  LSM6DS0_GYRO_LP_ENABLED = 0x80    /**< Low power mode enabled.            */
} lsm6ds0_gyro_lp_t;

/**
 * @brief  LSM6DS0 gyroscope subsystem output selection
 */
typedef enum {
  LSM6DS0_GYRO_OUT_SEL_0 = 0x00,    /**< Low pass filter 1.                 */
  LSM6DS0_GYRO_OUT_SEL_1 = 0x01,    /**< High pass filter 1 if enabled.     */
  LSM6DS0_GYRO_OUT_SEL_2 = 0x02     /**< Low pass filter 2.                 */
} lsm6ds0_gyro_out_sel_t;

/**
 * @brief  LSM6DS0 gyroscope subsystem high pass filter
 */
typedef enum {
  LSM6DS0_GYRO_HP_DISABLED = 0x00,  /**< High pass filter disabled.         */
  LSM6DS0_GYRO_HP_ENABLED = 0x40    /**< High pass filter enabled.          */
} lsm6ds0_gyro_hp_t;

/**
 * @brief  LSM6DS0 gyroscope subsystem high pass filter configuration
 */
typedef enum {
  LSM6DS0_GYRO_HPCF_0 = 0x00,       /**< Refer to table 48 of RM            */
  LSM6DS0_GYRO_HPCF_1 = 0x01, 
  LSM6DS0_GYRO_HPCF_2 = 0x02, 
  LSM6DS0_GYRO_HPCF_3 = 0x03, 
  LSM6DS0_GYRO_HPCF_4 = 0x04, 
  LSM6DS0_GYRO_HPCF_5 = 0x05, 
  LSM6DS0_GYRO_HPCF_6 = 0x06, 
  LSM6DS0_GYRO_HPCF_7 = 0x07, 
  LSM6DS0_GYRO_HPCF_8 = 0x08, 
  LSM6DS0_GYRO_HPCF_9 = 0x09 
} lsm6ds0_gyro_hpcf_t;

/**
 * @brief   LSM6DS0 gyroscope subsystem configuration structure.
 */
typedef struct {
  /**
   * @brief   LSM6DS0 gyroscope subsystem full scale
   */
  lsm6ds0_gyro_fs_t         fullscale;
  /**
   * @brief   LSM6DS0 gyroscope subsystem output data rate
   */
  lsm6ds0_gyro_odr_t        outdatarate;
  /**
   * @brief   LSM6DS0 gyroscope subsystem axes enabling
   */
  lsm6ds0_gyro_ae_t         axesenabling;
  /**
   * @brief   LSM6DS0 gyroscope subsystem low mode configuration
   */
  lsm6ds0_gyro_lp_t         lowmodecfg;
  /**
   * @brief   LSM6DS0 gyroscope subsystem output selection
   */
  lsm6ds0_gyro_out_sel_t    outsel;
  /**
   * @brief   LSM6DS0 gyroscope subsystem high pass filter
   */
  lsm6ds0_gyro_hp_t         hpfenable;
  /**
   * @brief   LSM6DS0 gyroscope subsystem high pass filter configuration
   */
  lsm6ds0_gyro_hpcf_t       hpcfg;
} LSM6DS0GyroConfig;
/** @} */

/**
 * @name    LSM6DS0 main system data structures and types
 * @{
 */
/**
 * @brief  Accelerometer and Gyroscope Slave Address
 */
typedef enum {
  LSM6DS0_SAD_GND = 0x6A,           /**< SAD pin connected to GND.          */
  LSM6DS0_SAD_VCC = 0x6B            /**< SAD pin connected to VCC.          */
} lsm6ds0_sad_t;

/**
 * @brief   LSM6DS0 block data update
 */
typedef enum {
  LSM6DS0_BDU_CONTINOUS = 0x00,     /**< Block data continuously updated.   */
  LSM6DS0_BDU_BLOCKED = 0x40        /**< Block data updated after reading.  */
} lsm6ds0_bdu_t;

/**
 * @brief   LSM6DS0 endianness
 */
typedef enum {
  LSM6DS0_END_LITTLE = 0x00,        /**< Little endian.                     */
  LSM6DS0_END_BIG = 0x20            /**< Big endian.                        */
} lsm6ds0_end_t;

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  LSM6DS0_UNINIT = 0,               /**< Not initialized.                   */
  LSM6DS0_STOP = 1,                 /**< Stopped.                           */
  LSM6DS0_READY = 2,                /**< Ready.                             */
} lsm6ds0_state_t;

/**
 * @brief   LSM6DS0 configuration structure.
 */
typedef struct {
#if (LSM6DS0_USE_SPI) || defined(__DOXYGEN__)
  /**
   * @brief SPI driver associated to this LSM6DS0.
   */
  SPIDriver                 *spip;
  /**
   * @brief SPI configuration associated to this LSM6DS0 accelerometer
   *        subsystem.
   */
  const SPIConfig           *accspicfg;
  /**
   * @brief SPI configuration associated to this LSM6DS0 compass
   *        subsystem.
   */
  const SPIConfig           *gyrospicfg;
#endif /* LSM6DS0_USE_SPI */
#if (LSM6DS0_USE_I2C) || defined(__DOXYGEN__)
  /**
   * @brief I2C driver associated to this LSM6DS0.
   */
  I2CDriver                 *i2cp;
  /**
   * @brief I2C configuration associated to this LSM6DS0 accelerometer
   *        subsystem.
   */
  const I2CConfig           *i2ccfg;
#endif /* LSM6DS0_USE_I2C */
  /**
   * @brief   LSM6DS0 accelerometer subsystem configuration structure
   */
  const LSM6DS0AccConfig    *acccfg;
  /**
   * @brief   LSM6DS0 gyroscope subsystem configuration structure
   */
  const LSM6DS0GyroConfig   *gyrocfg;
  /**
   * @brief  Accelerometer and Gyroscope Slave Address
   */
  lsm6ds0_sad_t             slaveaddress;
  /**
   * @brief   LSM6DS0 block data update
   */
  lsm6ds0_bdu_t             blockdataupdate;
  /**
   * @brief   LSM6DS0  endianness
   */
  lsm6ds0_end_t             endianness;
} LSM6DS0Config;

/**
 * @brief   Structure representing a LSM6DS0 driver.
 */
typedef struct LSM6DS0Driver LSM6DS0Driver;

/**
 * @brief   @p LSM6DS0 accelerometer subsystem specific methods.
 */
#define _lsm6ds0_acc_methods                                                \
  _base_accelerometer_methods
  
/**
 * @brief   @p LSM6DS0 gyroscope subsystem specific methods.
 */
#define _lsm6ds0_gyro_methods                                               \
  _base_gyroscope_methods
  
/**
 * @extends BaseAccelerometerVMT
 *
 * @brief   @p LSM6DS0 accelerometer virtual methods table.
 */
struct LSM6DS0ACCVMT {
  _lsm6ds0_acc_methods
};

/**
 * @extends BaseCompassVMT
 *
 * @brief   @p LSM6DS0 gyroscope virtual methods table.
 */
struct LSM6DS0GYROVMT {
  _lsm6ds0_gyro_methods
};

/**
 * @brief   @p LSM6DS0Driver specific data.
 */
#define _lsm6ds0_data                                                       \
  _base_accelerometer_data                                                  \
  _base_gyroscope_data                                                      \
  /* Driver state.*/                                                        \
  lsm6ds0_state_t           state;                                          \
  /* Current configuration data.*/                                          \
  const LSM6DS0Config       *config;                                        \
  /* Current accelerometer sensitivity.*/                                   \
  float                     accsens;                                        \
  /* Current gyroscope sensitivity.*/                                       \
  float                     gyrosens;
  
/**
 * @brief   LSM6DS0 6-axis accelerometer/gyroscope class.
 */
struct LSM6DS0Driver {
  /** @brief Accelerometer Virtual Methods Table.*/
  const struct LSM6DS0ACCVMT  *vmt_baseaccelerometer;
  /** @brief Gyroscope Virtual Methods Table.*/
  const struct LSM6DS0GYROVMT *vmt_basegyroscope;
  _lsm6ds0_data
};
/** @} */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void lsm6ds0ObjectInit(LSM6DS0Driver *devp);
  void lsm6ds0Start(LSM6DS0Driver *devp, const LSM6DS0Config *config);
  void lsm6ds0Stop(LSM6DS0Driver *devp);
#ifdef __cplusplus
}
#endif

#endif /* _LSM6DS0_H_ */

/** @} */
