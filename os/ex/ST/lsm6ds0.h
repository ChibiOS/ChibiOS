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
 * @addtogroup LSM6DS0
 * @ingroup EX_ST
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
 * @name    Version identification
 * @{
 */
/**
 * @brief   LSM6DS0 driver version string.
 */
#define EX_LSM6DS0_VERSION          "1.0.3"

/**
 * @brief   LSM6DS0 driver version major number.
 */
#define EX_LSM6DS0_MAJOR            1

/**
 * @brief   LSM6DS0 driver version minor number.
 */
#define EX_LSM6DS0_MINOR            0

/**
 * @brief   LSM6DS0 driver version patch number.
 */
#define EX_LSM6DS0_PATCH            3
/** @} */

/**
 * @brief   LSM6DS0 accelerometer subsystem characteristics.
 *
 * @{
 */
#define LSM6DS0_ACC_NUMBER_OF_AXES          3U

#define LSM6DS0_ACC_2G                      2.0f
#define LSM6DS0_ACC_4G                      4.0f
#define LSM6DS0_ACC_8G                      8.0f
#define LSM6DS0_ACC_16G                     16.0f

#define LSM6DS0_ACC_SENS_2G                 0.061f
#define LSM6DS0_ACC_SENS_4G                 0.122f
#define LSM6DS0_ACC_SENS_8G                 0.244f
#define LSM6DS0_ACC_SENS_16G                0.732f
/** @} */

/**
 * @brief   LSM6DS0 gyroscope subsystem characteristics.
 *
 * @{
 */
#define LSM6DS0_GYRO_NUMBER_OF_AXES         3U

#define LSM6DS0_GYRO_245DPS                 245.0f
#define LSM6DS0_GYRO_500DPS                 500.0f
#define LSM6DS0_GYRO_2000DPS                2000.0f

#define LSM6DS0_GYRO_SENS_245DPS            0.00875f
#define LSM6DS0_GYRO_SENS_500DPS            0.01750f
#define LSM6DS0_GYRO_SENS_2000DPS           0.07000f
/** @} */

/**
 * @name   LSM6DS0 communication interfaces related bit masks
 * @{
 */
#define LSM6DS0_DI_MASK             0xFF        /**< Data In mask           */
#define LSM6DS0_DI(n)               (1 << n)    /**< Data In bit n          */
#define LSM6DS0_AD_MASK             0x7F        /**< Address Data mask      */
#define LSM6DS0_AD(n)               (1 << n)    /**< Address Data bit n     */
#define LSM6DS0_MS                  (1 << 7)    /**< Multiple read write    */
/** @} */

/**
 * @name   LSM6DS0 register addresses
 * @{
 */
#define LSM6DS0_AD_ACT_THS                  0x04
#define LSM6DS0_AD_ACT_DUR                  0x05
#define LSM6DS0_AD_INT_GEN_CFG_XL           0x06
#define LSM6DS0_AD_INT_GEN_THS_X_XL         0x07
#define LSM6DS0_AD_INT_GEN_THS_Y_XL         0x08
#define LSM6DS0_AD_INT_GEN_THS_Z_XL         0x09
#define LSM6DS0_AD_INT_GEN_DUR_XL           0x0A
#define LSM6DS0_AD_REFERENCE_G              0x0B
#define LSM6DS0_AD_INT_CTRL                 0x0C
#define LSM6DS0_AD_WHO_AM_I                 0x0F
#define LSM6DS0_AD_CTRL_REG1_G              0x10
#define LSM6DS0_AD_CTRL_REG2_G              0x11
#define LSM6DS0_AD_CTRL_REG3_G              0x12
#define LSM6DS0_AD_ORIENT_CFG_G             0x13
#define LSM6DS0_AD_INT_GEN_SRC_G            0x14
#define LSM6DS0_AD_OUT_TEMP_L               0x15
#define LSM6DS0_AD_OUT_TEMP_H               0x16
#define LSM6DS0_AD_STATUS_REG1              0x17
#define LSM6DS0_AD_OUT_X_L_G                0x18
#define LSM6DS0_AD_OUT_X_H_G                0x19
#define LSM6DS0_AD_OUT_Y_L_G                0x1A
#define LSM6DS0_AD_OUT_Y_H_G                0x1B
#define LSM6DS0_AD_OUT_Z_L_G                0x1C
#define LSM6DS0_AD_OUT_Z_H_G                0x1D
#define LSM6DS0_AD_CTRL_REG4                0x1E
#define LSM6DS0_AD_CTRL_REG5_XL             0x1F
#define LSM6DS0_AD_CTRL_REG6_XL             0x20
#define LSM6DS0_AD_CTRL_REG7_XL             0x21
#define LSM6DS0_AD_CTRL_REG8                0x22
#define LSM6DS0_AD_CTRL_REG9                0x23
#define LSM6DS0_AD_CTRL_REG10               0x24
#define LSM6DS0_AD_INT_GEN_SRC_XL           0x26
#define LSM6DS0_AD_STATUS_REG2              0x27
#define LSM6DS0_AD_OUT_X_L_XL               0x28
#define LSM6DS0_AD_OUT_X_H_XL               0x29
#define LSM6DS0_AD_OUT_Y_L_XL               0x2A
#define LSM6DS0_AD_OUT_Y_H_XL               0x2B
#define LSM6DS0_AD_OUT_Z_L_XL               0x2C
#define LSM6DS0_AD_OUT_Z_H_XL               0x2D
#define LSM6DS0_AD_FIFO_CTRL                0x2E
#define LSM6DS0_AD_FIFO_SRC                 0x2F
#define LSM6DS0_AD_INT_GEN_CFG_G            0x30
#define LSM6DS0_AD_INT_GEN_THS_XH_G         0x31
#define LSM6DS0_AD_INT_GEN_THS_XL_G         0x32
#define LSM6DS0_AD_INT_GEN_THS_YH_G         0x33
#define LSM6DS0_AD_INT_GEN_THS_YL_G         0x34
#define LSM6DS0_AD_INT_GEN_THS_ZH_G         0x35
#define LSM6DS0_AD_INT_GEN_THS_ZL_G         0x36
#define LSM6DS0_AD_INT_GEN_DUR_G            0x37
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG1_G register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG1_G                 0xFA
#define LSM6DS0_CTRL_REG1_G_BW_G0           (1 << 0)
#define LSM6DS0_CTRL_REG1_G_BW_G1           (1 << 1)
#define LSM6DS0_CTRL_REG1_G_FS_MASK         0x1F
#define LSM6DS0_CTRL_REG1_G_FS_G0           (1 << 3)
#define LSM6DS0_CTRL_REG1_G_FS_G1           (1 << 4)
#define LSM6DS0_CTRL_REG1_G_ODR_G0          (1 << 5)
#define LSM6DS0_CTRL_REG1_G_ODR_G1          (1 << 6)
#define LSM6DS0_CTRL_REG1_G_ODR_G2          (1 << 7)
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG2_G register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG2_G                 0x0F
#define LSM6DS0_CTRL_REG2_G_OUT_SEL0        (1 << 0)
#define LSM6DS0_CTRL_REG2_G_OUT_SEL1        (1 << 1)
#define LSM6DS0_CTRL_REG2_G_INT_SEL0        (1 << 2)
#define LSM6DS0_CTRL_REG2_G_INT_SEL1        (1 << 3)
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG3_G register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG3_G                 0x64
#define LSM6DS0_CTRL_REG3_G_HP_CF0_G        (1 << 0)
#define LSM6DS0_CTRL_REG3_G_HP_CF1_G        (1 << 1)
#define LSM6DS0_CTRL_REG3_G_HP_CF2_G        (1 << 2)
#define LSM6DS0_CTRL_REG3_G_HP_CF3_G        (1 << 3)
#define LSM6DS0_CTRL_REG3_G_HP_EN           (1 << 6)
#define LSM6DS0_CTRL_REG3_G_LP_MODE         (1 << 7)
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG4 register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG4                   0x3A
#define LSM6DS0_CTRL_REG4_4D_XL1            (1 << 0)
#define LSM6DS0_CTRL_REG4_LIR_XL1           (1 << 1)
#define LSM6DS0_CTRL_REG4_XEN_G             (1 << 3)
#define LSM6DS0_CTRL_REG4_YEN_G             (1 << 4)
#define LSM6DS0_CTRL_REG4_ZEN_G             (1 << 5)
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG5_XL register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG5_XL                0xF8
#define LSM6DS0_CTRL_REG5_XL_XEN_XL         (1 << 3)
#define LSM6DS0_CTRL_REG5_XL_YEN_XL         (1 << 4)
#define LSM6DS0_CTRL_REG5_XL_ZEN_XL         (1 << 5)
#define LSM6DS0_CTRL_REG5_XL_DEC0           (1 << 6)
#define LSM6DS0_CTRL_REG5_XL_DEC1           (1 << 7)
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG6_XL register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG6_XL                0xFF
#define LSM6DS0_CTRL_REG6_XL_BW_XL0         (1 << 0)
#define LSM6DS0_CTRL_REG6_XL_BW_XL1         (1 << 1)
#define LSM6DS0_CTRL_REG6_XL_BW_SCAL_ODR    (1 << 2)
#define LSM6DS0_CTRL_REG6_XL_FS_MASK        0x1F
#define LSM6DS0_CTRL_REG6_XL_FS0_XL         (1 << 3)
#define LSM6DS0_CTRL_REG6_XL_FS1_XL         (1 << 4)
#define LSM6DS0_CTRL_REG6_XL_ODR_XL0        (1 << 5)
#define LSM6DS0_CTRL_REG6_XL_ODR_XL1        (1 << 6)
#define LSM6DS0_CTRL_REG6_XL_ODR_XL2        (1 << 7)
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG7_XL register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG7_XL                0xE5
#define LSM6DS0_CTRL_REG7_XL_HPIS1          (1 << 0)
#define LSM6DS0_CTRL_REG7_XL_FDS            (1 << 2)
#define LSM6DS0_CTRL_REG7_XL_DCF0           (1 << 5)
#define LSM6DS0_CTRL_REG7_XL_DCF1           (1 << 6)
#define LSM6DS0_CTRL_REG7_XL_HR             (1 << 7)
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG8 register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG8                   0xFF
#define LSM6DS0_CTRL_REG8_SW_RESET          (1 << 0)
#define LSM6DS0_CTRL_REG8_BLE               (1 << 1)
#define LSM6DS0_CTRL_REG8_IF_ADD_INC        (1 << 2)
#define LSM6DS0_CTRL_REG8_SIM               (1 << 3)
#define LSM6DS0_CTRL_REG8_PP_OD             (1 << 4)
#define LSM6DS0_CTRL_REG8_H_LACTIVE         (1 << 5)
#define LSM6DS0_CTRL_REG8_BDU               (1 << 6)
#define LSM6DS0_CTRL_REG8_BOOT              (1 << 7)
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG9 register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG9                   0x5F
#define LSM6DS0_CTRL_REG9_STOP_ON_FTH       (1 << 0)
#define LSM6DS0_CTRL_REG9_FIFO_EN           (1 << 1)
#define LSM6DS0_CTRL_REG9_I2C_DISABLE       (1 << 2)
#define LSM6DS0_CTRL_REG9_DRDY_MASK_BIT     (1 << 3)
#define LSM6DS0_CTRL_REG9_FIFO_TEMP_EN      (1 << 4)
#define LSM6DS0_CTRL_REG9_SLEEP_G           (1 << 6)
/** @} */

/**
 * @name    LSM6DS0_AD_CTRL_REG10 register bits definitions
 * @{
 */
#define LSM6DS0_CTRL_REG10                  0x05
#define LSM6DS0_CTRL_REG10_ST_XL            (1 << 0)
#define LSM6DS0_CTRL_REG10_ST_G             (1 << 2)

/** @} */

//TODO: ADD more LSM6DS0 register bits definitions

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
 * @note    The default is @p FALSE. Requires I2C_USE_MUTUAL_EXCLUSION.
 */
#if !defined(LSM6DS0_SHARED_I2C) || defined(__DOXYGEN__)
#define LSM6DS0_SHARED_I2C                  FALSE
#endif

/**
 * @brief   LSM6DS0 subsystem advanced configurations switch.
 * @details If set to @p TRUE more configurations are available.
 * @note    The default is @p FALSE.
 */
#if !defined(LSM6DS0_USE_ADVANCED) || defined(__DOXYGEN__)
#define LSM6DS0_USE_ADVANCED                FALSE
#endif

/**
 * @brief   LSM6DS0 accelerometer subsystem advanced configurations 
 *          switch.
 * @details If set to @p TRUE more configurations are available.
 * @note    The default is @p FALSE.
 */
#if !defined(LSM6DS0_ACC_USE_ADVANCED) || defined(__DOXYGEN__)
#define LSM6DS0_ACC_USE_ADVANCED            FALSE
#endif

/**
 * @brief   LSM6DS0 gyroscope subsystem advanced configurations 
 *          switch.
 * @details If set to @p TRUE more configurations are available.
 * @note    The default is @p FALSE.
 */
#if !defined(LSM6DS0_GYRO_USE_ADVANCED) || defined(__DOXYGEN__)
#define LSM6DS0_GYRO_USE_ADVANCED           FALSE
#endif

/**
 * @brief   Number of acquisitions for gyroscope bias removal.
 * @details This is the number of acquisitions performed to compute the
 *          bias. A repetition is required in order to remove noise.
 */
#if !defined(LSM6DS0_GYRO_BIAS_ACQ_TIMES) || defined(__DOXYGEN__)
#define LSM6DS0_GYRO_BIAS_ACQ_TIMES         50
#endif

/**
 * @brief   Settling time for gyroscope bias removal.
 * @details This is the time between each bias acquisition.
 */
#if !defined(LSM6DS0_GYRO_BIAS_SETTLING_uS) || defined(__DOXYGEN__)
#define LSM6DS0_GYRO_BIAS_SETTLING_uS       5000
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
 * @name    LSM6DS0 accelerometer subsystem data structures and types.
 * @{
 */
/**
 * @brief   LSM6DS0 accelerometer subsystem full scale.
 */
typedef enum {
  LSM6DS0_ACC_FS_2G = 0x00,         /**< Full scale ±2g.                    */
  LSM6DS0_ACC_FS_4G = 0x10,         /**< Full scale ±4g.                    */
  LSM6DS0_ACC_FS_8G = 0x18,         /**< Full scale ±8g.                    */
  LSM6DS0_ACC_FS_16G = 0x08         /**< Full scale ±16g.                   */
} lsm6ds0_acc_fs_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem output data rate.
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
 * @brief   LSM6DS0 accelerometer subsystem decimation mode.
 */
typedef enum {
  LSM6DS0_ACC_DEC_DISABLED = 0x00,  /**< Decimation disabled.               */
  LSM6DS0_ACC_DEC_X2 = 0x40,        /**< Output updated every 2 samples.    */
  LSM6DS0_ACC_DEC_X4 = 0x80,        /**< Output updated every 4 samples.    */
  LSM6DS0_ACC_DEC_X8 = 0xC0         /**< Output updated every 8 samples.    */
} lsm6ds0_acc_dec_t;

/**
 * @brief   LSM6DS0 accelerometer subsystem configuration structure.
 */
typedef struct {
  /**
   * @brief LSM6DS0 accelerometer initial sensitivity.
   */
  float*                       sensitivity;
  /**
   * @brief LSM6DS0 accelerometer initial bias.
   */
  float*                       bias;
  /**
   * @brief LSM6DS0 accelerometer subsystem full scale.
   */
  lsm6ds0_acc_fs_t             fullscale;
  /**
   * @brief LSM6DS0 accelerometer subsystem output data rate.
   */
  lsm6ds0_acc_odr_t            outdatarate;
#if LSM6DS0_ACC_USE_ADVANCED || defined(__DOXYGEN__)
  /**
   * @brief LSM6DS0 accelerometer subsystem decimation mode.
   */
  lsm6ds0_acc_dec_t            decmode;
#endif /* LSM6DS0_ACC_USE_ADVANCED */
} LSM6DS0AccConfig;
/** @} */

/**
 * @name  LSM6DS0 gyroscope subsystem data structures and types.
 * @{
 */
/**
 * @brief LSM6DS0 gyroscope subsystem full scale.
 */
typedef enum {
  LSM6DS0_GYRO_FS_245DPS  = 0x00,   /**< Full scale ±245 degree per second  */
  LSM6DS0_GYRO_FS_500DPS  = 0x08,   /**< Full scale ±500 degree per second  */
  LSM6DS0_GYRO_FS_2000DPS = 0x18    /**< Full scale ±2000 degree per second */
} lsm6ds0_gyro_fs_t;

/**
 * @brief LSM6DS0 gyroscope subsystem output data rate.
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
 * @brief LSM6DS0 gyroscope subsystem low mode configuration.
 */
typedef enum {
  LSM6DS0_GYRO_LP_DISABLED = 0x00,  /**< Low power mode disabled.           */
  LSM6DS0_GYRO_LP_ENABLED = 0x80    /**< Low power mode enabled.            */
} lsm6ds0_gyro_lp_t;

/**
 * @brief  LSM6DS0 gyroscope subsystem output selection.
 */
typedef enum {
  LSM6DS0_GYRO_OUT_SEL_0 = 0x00,    /**< Low pass filter 1.                 */
  LSM6DS0_GYRO_OUT_SEL_1 = 0x01,    /**< High pass filter 1 if enabled.     */
  LSM6DS0_GYRO_OUT_SEL_2 = 0x02     /**< Low pass filter 2.                 */
} lsm6ds0_gyro_out_sel_t;

/**
 * @brief  LSM6DS0 gyroscope subsystem high pass filter.
 */
typedef enum {
  LSM6DS0_GYRO_HP_DISABLED = 0x00,  /**< High pass filter disabled.         */
  LSM6DS0_GYRO_HP_ENABLED = 0x40    /**< High pass filter enabled.          */
} lsm6ds0_gyro_hp_t;

/**
 * @brief  LSM6DS0 gyroscope subsystem high pass filter configuration.
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
 * @brief LSM6DS0 gyroscope subsystem configuration structure.
 */
typedef struct {
  /**
   * @brief LSM6DS0 gyroscope initial sensitivity.
   */
  float*                       sensitivity;
  /**
   * @brief LSM6DS0 gyroscope initial bias.
   */
  float*                       bias;
  /**
   * @brief LSM6DS0 gyroscope subsystem full scale.
   */
  lsm6ds0_gyro_fs_t            fullscale;
  /**
   * @brief LSM6DS0 gyroscope subsystem output data rate.
   */
  lsm6ds0_gyro_odr_t           outdatarate;
#if LSM6DS0_GYRO_USE_ADVANCED || defined(__DOXYGEN__)
  /**
   * @brief LSM6DS0 gyroscope subsystem low mode configuration.
   */
  lsm6ds0_gyro_lp_t            lowmodecfg;
  /**
   * @brief LSM6DS0 gyroscope subsystem output selection.
   */
  lsm6ds0_gyro_out_sel_t       outsel;
  /**
   * @brief LSM6DS0 gyroscope subsystem high pass filter.
   */
  lsm6ds0_gyro_hp_t            hpfenable;
  /**
   * @brief LSM6DS0 gyroscope subsystem high pass filter configuration.
   */
  lsm6ds0_gyro_hpcf_t          hpcfg;
  #endif /* LSM6DS0_GYRO_USE_ADVANCED */
} LSM6DS0GyroConfig;
/** @} */

/**
 * @name  LSM6DS0 main system data structures and types.
 * @{
 */
/**
 * @brief  Accelerometer and Gyroscope Slave Address.
 */
typedef enum {
  LSM6DS0_SAD_GND = 0x6A,           /**< SAD pin connected to GND.          */
  LSM6DS0_SAD_VCC = 0x6B            /**< SAD pin connected to VCC.          */
} lsm6ds0_sad_t;

/**
 * @brief LSM6DS0 block data update.
 */
typedef enum {
  LSM6DS0_BDU_CONTINUOUS = 0x00,    /**< Block data continuously updated.   */
  LSM6DS0_BDU_BLOCKED = 0x40        /**< Block data updated after reading.  */
} lsm6ds0_bdu_t;

/**
 * @brief LSM6DS0 endianness.
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
 * @brief LSM6DS0 configuration structure.
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
   * @brief LSM6DS0 accelerometer subsystem configuration structure
   */
  const LSM6DS0AccConfig    *acccfg;
  /**
   * @brief LSM6DS0 gyroscope subsystem configuration structure
   */
  const LSM6DS0GyroConfig   *gyrocfg;
  /**
   * @brief  Accelerometer and Gyroscope Slave Address
   */
  lsm6ds0_sad_t             slaveaddress;
#if (LSM6DS0_USE_ADVANCED) || defined(__DOXYGEN__)
  /**
   * @brief LSM6DS0 block data update
   */
  lsm6ds0_bdu_t             blockdataupdate;
  /**
   * @brief LSM6DS0  endianness
   */
  lsm6ds0_end_t             endianness;
#endif /* LSM6DS0_USE_ADVANCED */
} LSM6DS0Config;

/**
 * @brief   Structure representing a LSM6DS0 driver.
 */
typedef struct LSM6DS0Driver LSM6DS0Driver;

/**
 * @brief   @p LSM6DS0 accelerometer subsystem specific methods.
 */
#define _lsm6ds0_acc_methods                                                \
  _base_accelerometer_methods                                               \
  /* Change full scale value of LSM6DS0 accelerometer subsystem .*/         \
  msg_t (*set_full_scale)(void *instance, lsm6ds0_acc_fs_t fs);
  
/**
 * @brief   @p LSM6DS0 gyroscope subsystem specific methods.
 */
#define _lsm6ds0_gyro_methods                                               \
  _base_gyroscope_methods                                                   \
  /* Change full scale value of LSM6DS0 gyroscope subsystem .*/             \
  msg_t (*set_full_scale)(void *instance, lsm6ds0_gyro_fs_t fs);
  
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
  float                     accsensitivity[LSM6DS0_ACC_NUMBER_OF_AXES];     \
  /* Accelerometer bias data.*/                                             \
  float                     accbias[LSM6DS0_ACC_NUMBER_OF_AXES];            \
  /* Current accelerometer full scale value.*/                              \
  float                     accfullscale;                                   \
  /* Current gyroscope sensitivity.*/                                       \
  float                     gyrosensitivity[LSM6DS0_GYRO_NUMBER_OF_AXES];   \
  /* Bias data.*/                                                           \
  float                     gyrobias[LSM6DS0_GYRO_NUMBER_OF_AXES];          \
  /* Current gyroscope full scale value.*/                                  \
  float                     gyrofullscale;
  
/**
 * @brief LSM6DS0 6-axis accelerometer/gyroscope class.
 */
struct LSM6DS0Driver {
  /** @brief BaseSensor Virtual Methods Table. */
  const struct BaseSensorVMT *vmt_basesensor;
  /** @brief BaseAccelerometer Virtual Methods Table. */
  const struct BaseAccelerometerVMT *vmt_baseaccelerometer;
  /** @brief BaseGyroscope Virtual Methods Table. */
  const struct BaseGyroscopeVMT *vmt_basegyroscope;
  /** @brief LSM6DS0 Accelerometer Virtual Methods Table. */
  const struct LSM6DS0ACCVMT *vmt_lsm6ds0acc;
  /** @brief LSM6DS0 Gyroscope Virtual Methods Table. */
  const struct LSM6DS0GYROVMT *vmt_lsm6ds0gyro;
  _lsm6ds0_data
};
/** @} */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Change accelerometer fullscale value.
 *
 * @param[in] ip        pointer to a @p BaseAccelerometer class.
 * @param[in] fs        the new full scale value.
 *
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 * @api
 */
#define accelerometerSetFullScale(ip, fs)                                   \
        (ip)->vmt_lsm6ds0acc->set_full_scale(ip, fs)

/**
 * @brief   Change compass fullscale value.
 *
 * @param[in] ip        pointer to a @p BaseGyroscope class.
 * @param[in] fs        the new full scale value.
 *
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 * @api
 */
#define gyroscopeSetFullScale(ip, fs)                                         \
        (ip)->vmt_lsm6ds0gyro->set_full_scale(ip, fs)
  
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
