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
 * @file    lsm303dlhc.h
 * @brief   LSM303DLHC MEMS interface module header.
 *
 * @{
 */
#ifndef _LSM303DLHC_H_
#define _LSM303DLHC_H_

#include "hal_accelerometer.h"
#include "hal_compass.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   LSM303DLHC accelerometer subsystem number of axes.
 */
#define  LSM303DLHC_ACC_NUMBER_OF_AXES         ((size_t)   3U)

/**
 * @brief   LSM303DLHC compass subsystem number of axes.
 */
#define  LSM303DLHC_COMP_NUMBER_OF_AXES        ((size_t)   3U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   LSM303DLHC SPI interface selector.
 * @details If set to @p TRUE the support for SPI is included.
 * @note    The default is @p FALSE.
 */
#if !defined(LSM303DLHC_USE_SPI) || defined(__DOXYGEN__)
#define LSM303DLHC_USE_SPI                     FALSE
#endif

/**
 * @brief   LSM303DLHC I2C interface selector.
 * @details If set to @p TRUE the support for I2C is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LSM303DLHC_USE_I2C) || defined(__DOXYGEN__)
#define LSM303DLHC_USE_I2C                     TRUE
#endif

/**
 * @brief   LSM303DLHC shared I2C switch.
 * @details If set to @p TRUE the device acquires I2C bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires I2C_USE_MUTUAL_EXCLUSION.
 */
#if !defined(LSM303DLHC_SHARED_I2C) || defined(__DOXYGEN__)
#define LSM303DLHC_SHARED_I2C                  FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if LSM303DLHC_USE_SPI && LSM303DLHC_USE_I2C
#error "LSM303DLHC_USE_SPI and LSM303DLHC_USE_I2C cannot be both true"
#endif

#if LSM303DLHC_USE_SPI && !HAL_USE_SPI
#error "LSM303DLHC_USE_SPI requires HAL_USE_SPI"
#endif

#if LSM303DLHC_USE_I2C && !HAL_USE_I2C
#error "LSM303DLHC_USE_I2C requires HAL_USE_I2C"
#endif

#if LSM303DLHC_SHARED_I2C && !I2C_USE_MUTUAL_EXCLUSION
#error "LSM303DLHC_SHARED_SPI requires I2C_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @name    LSM303DLHC accelerometer subsystem data structures and types.
 * @{
 */

/**
 * @brief   LSM303DLHC accelerometer subsystem full scale.
 */
typedef enum {
  LSM303DLHC_ACC_FS_2G   = 0x00,    /**< Full scale ±2g.                    */
  LSM303DLHC_ACC_FS_4G   = 0x10,    /**< Full scale ±4g.                    */
  LSM303DLHC_ACC_FS_8G   = 0x20,    /**< Full scale ±8g.                    */
  LSM303DLHC_ACC_FS_16G  = 0x30     /**< Full scale ±16g.                   */
} lsm303dlhc_acc_fs_t;

/**
 * @brief   LSM303DLHC accelerometer subsystem output data rate.
 */
typedef enum {
  LSM303DLHC_ACC_ODR_PD = 0x00,     /**< Power down                         */
  LSM303DLHC_ACC_ODR_1Hz = 0x10,    /**< ODR 1 Hz                           */
  LSM303DLHC_ACC_ODR_10Hz = 0x20,   /**< ODR 10 Hz                          */
  LSM303DLHC_ACC_ODR_25Hz = 0x30,   /**< ODR 25 Hz                          */
  LSM303DLHC_ACC_ODR_50Hz = 0x40,   /**< ODR 50 Hz                          */
  LSM303DLHC_ACC_ODR_100Hz = 0x50,  /**< ODR 100 Hz                         */
  LSM303DLHC_ACC_ODR_200Hz = 0x60,  /**< ODR 200 Hz                         */
  LSM303DLHC_ACC_ODR_400Hz = 0x70,  /**< ODR 400 Hz                         */
  LSM303DLHC_ACC_ODR_1620Hz = 0x80, /**< ODR 1620 Hz (LP only)              */
  LSM303DLHC_ACC_ODR_1344Hz = 0x90  /**< ODR 1344 Hz or 5376 Hz in LP       */
} lsm303dlhc_acc_odr_t;


/**
 * @brief   LSM303DLHC accelerometer subsystem axes enabling.
 */
typedef enum {
  LSM303DLHC_ACC_AE_DISABLED = 0x00,/**< All axes disabled.                 */
  LSM303DLHC_ACC_AE_X = 0x01,       /**< Only X-axis enabled.               */
  LSM303DLHC_ACC_AE_Y = 0x02,       /**< Only Y-axis enabled.               */
  LSM303DLHC_ACC_AE_XY = 0x03,      /**< X and Y axes enabled.              */
  LSM303DLHC_ACC_AE_Z = 0x04,       /**< Only Z-axis enabled.               */
  LSM303DLHC_ACC_AE_XZ = 0x05,      /**< X and Z axes enabled.              */
  LSM303DLHC_ACC_AE_YZ = 0x06,      /**< Y and Z axes enabled.              */
  LSM303DLHC_ACC_AE_XYZ = 0x07      /**< All axes enabled.                  */
} lsm303dlhc_acc_ae_t;

/**
 * @brief   LSM303DLHC accelerometer subsystem low power mode.
 */
typedef enum {
  LSM303DLHC_ACC_LP_DISABLED = 0x00,/**< Low power mode disabled.           */
  LSM303DLHC_ACC_LP_ENABLED = 0x40  /**< Low power mode enabled.            */
} lsm303dlhc_acc_lp_t;

/**
 * @brief   LSM303DLHC accelerometer subsystem high resolution mode.
 */
typedef enum {
  LSM303DLHC_ACC_HR_DISABLED = 0x00,/**< High resolution mode disabled.     */
  LSM303DLHC_ACC_HR_ENABLED = 0x08  /**< High resolution mode enabled.      */
} lsm303dlhc_acc_hr_t;

/**
 * @brief   LSM303DLHC accelerometer subsystem block data update.
 */
typedef enum {
  LSM303DLHC_ACC_BDU_CONT = 0x00,   /**< Continuous update                  */
  LSM303DLHC_ACC_BDU_BLOCK = 0x80   /**< Update blocked                     */
} lsm303dlhc_acc_bdu_t;

/**
 * @brief  LSM303DLHC accelerometer endianness.
 */
typedef enum {
  LSM303DLHC_ACC_END_LITTLE = 0x00, /**< Little Endian                      */
  LSM303DLHC_ACC_END_BIG = 0x40     /**< Big Endian                         */
} lsm303dlhc_acc_end_t;


/**
 * @brief   LSM303DLHC accelerometer subsystem unit.
 */
typedef enum {
  LSM303DLHC_ACC_UNIT_G = 0x00,     /**< Cooked data in g.                  */
  LSM303DLHC_ACC_UNIT_MG = 0x01,    /**< Cooked data in mg.                 */
  LSM303DLHC_ACC_UNIT_SI = 0x02,    /**< Cooked data in m/s^2.              */
} lsm303dlhc_acc_unit_t;

/**
 * @brief   LSM303DLHC accelerometer subsystem configuration structure.
 */
typedef struct {
  /**
   * @brief   LSM303DLHC accelerometer subsystem full scale.
   */
  lsm303dlhc_acc_fs_t          fullscale;
  /**
   * @brief   LSM303DLHC accelerometer subsystem output data rate.
   */
  lsm303dlhc_acc_odr_t         outdatarate;
  /**
   * @brief   LSM303DLHC accelerometer subsystem axes enabling.
   */
  lsm303dlhc_acc_ae_t          axesenabling;
  /**
   * @brief   LSM303DLHC accelerometer subsystem low power mode.
   */
  lsm303dlhc_acc_lp_t          lowpower;
  /**
   * @brief   LSM303DLHC accelerometer subsystem high resolution mode.
   */
  lsm303dlhc_acc_hr_t          highresmode;
  /**
   * @brief   LSM303DLHC accelerometer subsystem block data update.
   */
  lsm303dlhc_acc_bdu_t         blockdataupdate;
  /**
   * @brief  LSM303DLHC accelerometer endianness.
   */
  lsm303dlhc_acc_end_t         endianess;
  /**
   * @brief   LSM303DLHC accelerometer subsystem unit.
   */
  lsm303dlhc_acc_unit_t        unit;
} LSM303DLHCAccConfig;
/** @} */

/**
 * @name    LSM303DLHC compass subsystem data structures and types.
 * @{
 */
/**
 * @brief   LSM303DLHC compass subsystem full scale.
 */
typedef enum {
  LSM303DLHC_COMP_FS_1_3_GA = 0x20, /**< Full scale ±1.3 Gauss              */
  LSM303DLHC_COMP_FS_1_9_GA = 0x40, /**< Full scale ±1.9 Gauss              */
  LSM303DLHC_COMP_FS_2_5_GA = 0x60, /**< Full scale ±2.5 Gauss              */
  LSM303DLHC_COMP_FS_4_0_GA = 0x80, /**< Full scale ±4.0 Gauss              */
  LSM303DLHC_COMP_FS_4_7_GA = 0xA0, /**< Full scale ±4.7 Gauss              */
  LSM303DLHC_COMP_FS_5_6_GA = 0xC0, /**< Full scale ±5.6 Gauss              */
  LSM303DLHC_COMP_FS_8_1_GA = 0xE0  /**< Full scale ±8.1 Gauss              */
} lsm303dlhc_comp_fs_t;

/**
 * @brief   LSM303DLHC compass subsystem output data rate.
 */
typedef enum {
  LSM303DLHC_COMP_ODR_0_75HZ = 0x00,/**< ODR 0.75 Hz                        */
  LSM303DLHC_COMP_ODR_1_5HZ = 0x04, /**< ODR 1.5 Hz                         */
  LSM303DLHC_COMP_ODR_3_0HZ = 0x08, /**< ODR 3 Hz                           */
  LSM303DLHC_COMP_ODR_7_5HZ = 0x0C, /**< ODR 7.5 Hz                         */
  LSM303DLHC_COMP_ODR_15HZ = 0x10,  /**< ODR 15 Hz                          */
  LSM303DLHC_COMP_ODR_30HZ = 0x14,  /**< ODR 30 Hz                          */
  LSM303DLHC_COMP_ODR_75HZ = 0x18,  /**< ODR 75 Hz                          */
  LSM303DLHC_COMP_ODR_220HZ = 0x1C  /**< ODR 220 Hz                         */
} lsm303dlhc_comp_odr_t;

/**
 * @brief   LSM303DLHC compass subsystem working mode.
 */
typedef enum {
  LSM303DLHC_COMP_MD_CONT = 0x00,   /**< Continuous-Conversion Mode         */
  LSM303DLHC_COMP_MD_BLOCK = 0x01,  /**< Single-Conversion Mode             */
  LSM303DLHC_COMP_MD_SLEEP = 0x02   /**< Sleep Mode                         */
} lsm303dlhc_comp_md_t;

/**
 * @brief   LSM303DLHC compass subsystem configuration structure.
 */
typedef struct {
  /**
   * @brief   LSM303DLHC compass subsystem full scale.
   */
  lsm303dlhc_comp_fs_t         fullscale;
  /**
   * @brief   LSM303DLHC compass subsystem output data rate.
   */
  lsm303dlhc_comp_odr_t        outputdatarate;
  /**
   * @brief   LSM303DLHC compass subsystem working mode.
   */
  lsm303dlhc_comp_md_t         mode;
} LSM303DLHCCompConfig;
/** @} */

/**
 * @name    LSM303DLHC main system data structures and types.
 * @{
 */

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  LSM303DLHC_UNINIT = 0,            /**< Not initialized.                   */
  LSM303DLHC_STOP = 1,              /**< Stopped.                           */
  LSM303DLHC_READY = 2,             /**< Ready.                             */
} lsm303dlhc_state_t;

/**
 * @brief   LSM303DLHC configuration structure.
 */
typedef struct {
#if (LSM303DLHC_USE_SPI) || defined(__DOXYGEN__)
  /**
   * @brief SPI driver associated to this LSM303DLHC.
   */
  SPIDriver                 *spip;
  /**
   * @brief SPI configuration associated to this LSM303DLHC accelerometer
   *        subsystem.
   */
  const SPIConfig           *accspicfg;
  /**
   * @brief SPI configuration associated to this LSM303DLHC compass
   *        subsystem.
   */
  const SPIConfig           *compspicfg;
#endif /* LSM303DLHC_USE_SPI */
#if (LSM303DLHC_USE_I2C) || defined(__DOXYGEN__)
  /**
   * @brief I2C driver associated to this LSM303DLHC.
   */
  I2CDriver                 *i2cp;
  /**
   * @brief I2C configuration associated to this LSM303DLHC accelerometer
   *        subsystem.
   */
  const I2CConfig           *i2ccfg;
#endif /* LSM303DLHC_USE_I2C */
  /**
   * @brief   LSM303DLHC accelerometer subsystem configuration structure
   */
  const LSM303DLHCAccConfig    *acccfg;
  /**
   * @brief   LSM303DLHC compass subsystem configuration structure
   */
  const LSM303DLHCCompConfig   *compcfg;
} LSM303DLHCConfig;

/**
 * @brief   Structure representing a LSM303DLHC driver.
 */
typedef struct LSM303DLHCDriver LSM303DLHCDriver;

/**
 * @brief   @p LSM303DLHC accelerometer subsystem specific methods.
 */
#define _lsm303dlhc_acc_methods                                             \
  _base_accelerometer_methods

/**
 * @brief   @p LSM303DLHC compass subsystem specific methods.
 */
#define _lsm303dlhc_comp_methods                                            \
  _base_compass_methods

/**
 * @extends BaseAccelerometerVMT
 *
 * @brief   @p LSM303DLHC accelerometer virtual methods table.
 */
struct LSM303DLHCACCVMT {
  _lsm303dlhc_acc_methods
};

/**
 * @extends BaseCompassVMT
 *
 * @brief   @p LSM303DLHC compass virtual methods table.
 */
struct LSM303DLHCCOMPVMT {
  _lsm303dlhc_comp_methods
};

/**
 * @brief   @p LSM303DLHCDriver specific data.
 */
#define _lsm303dlhc_data                                                    \
  _base_accelerometer_data                                                  \
  _base_compass_data                                                        \
  /* Driver state.*/                                                        \
  lsm303dlhc_state_t           state;                                       \
  /* Current configuration data.*/                                          \
  const LSM303DLHCConfig       *config;                                     \
  /* Current accelerometer sensitivity.*/                                   \
  float                     accsensitivity[LSM303DLHC_ACC_NUMBER_OF_AXES];  \
  /* Accelerometer bias data.*/                                             \
  int32_t                   accbias[LSM303DLHC_ACC_NUMBER_OF_AXES];         \
  /* Current compass sensitivity.*/                                         \
  float                     compsensitivity[LSM303DLHC_COMP_NUMBER_OF_AXES];\
  /* Bias data.*/                                                           \
  int32_t                   compbias[LSM303DLHC_COMP_NUMBER_OF_AXES];

/**
 * @brief   LSM303DLHC 6-axis accelerometer/compass class.
 */
struct LSM303DLHCDriver {
  /** @brief BaseSensor Virtual Methods Table. */
  const struct BaseSensorVMT *vmt_basesensor;
  /** @brief BaseAccelerometer Virtual Methods Table. */
  const struct BaseAccelerometerVMT *vmt_baseaccelerometer;
  /** @brief BaseCompass Virtual Methods Table. */
  const struct BaseCompassVMT *vmt_basecompass;
  /** @brief LSM303DLHC Accelerometer Virtual Methods Table. */
  const struct LSM303DLHCACCVMT *vmt_lsm303dlhcacc;
  /** @brief LSM303DLHC Compass Virtual Methods Table. */
  const struct LSM303DLHCCOMPVMT *vmt_lsm303dlhccomp;
  _lsm303dlhc_data
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
  void lsm303dlhcObjectInit(LSM303DLHCDriver *devp);
  void lsm303dlhcStart(LSM303DLHCDriver *devp, const LSM303DLHCConfig *config);
  void lsm303dlhcStop(LSM303DLHCDriver *devp);
#ifdef __cplusplus
}
#endif

#endif /* _LSM303DLHC_H_ */

/** @} */
