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
 * @brief LSM303DLHC MEMS interface module header.
 *
 * @addtogroup LSM303DLHC
 * @ingroup EX_ST
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
 * @name    Version identification
 * @{
 */
/**
 * @brief   LSM303DLHC driver version string.
 */
#define EX_LSM303DLHC_VERSION       "1.0.3"

/**
 * @brief   LSM303DLHC driver version major number.
 */
#define EX_LSM303DLHC_MAJOR         1

/**
 * @brief   LSM303DLHC driver version minor number.
 */
#define EX_LSM303DLHC_MINOR         0

/**
 * @brief   LSM303DLHC driver version patch number.
 */
#define EX_LSM303DLHC_PATCH         3
/** @} */

/**
 * @brief   LSM303DLHC accelerometer subsystem characteristics.
 *
 * @{
 */
#define LSM303DLHC_ACC_NUMBER_OF_AXES       3U

#define LSM303DLHC_ACC_2G                   2.0f
#define LSM303DLHC_ACC_4G                   4.0f
#define LSM303DLHC_ACC_8G                   8.0f
#define LSM303DLHC_ACC_16G                  16.0f

#define LSM303DLHC_ACC_SENS_2G              0.0610f
#define LSM303DLHC_ACC_SENS_4G              0.1221f
#define LSM303DLHC_ACC_SENS_8G              0.2442f
#define LSM303DLHC_ACC_SENS_16G             0.4884f
/** @} */

/**
 * @brief   LSM303DLHC compass subsystem characteristics.
 *
 * @{
 */
#define LSM303DLHC_COMP_NUMBER_OF_AXES      3U

#define LSM303DLHC_COMP_1P3GA               1.3f
#define LSM303DLHC_COMP_1P9GA               1.9f
#define LSM303DLHC_COMP_2P5GA               2.5f
#define LSM303DLHC_COMP_4P0GA               4.0f
#define LSM303DLHC_COMP_4P7GA               4.7f
#define LSM303DLHC_COMP_5P6GA               5.6f
#define LSM303DLHC_COMP_8P1GA               8.1f

#define LSM303DLHC_COMP_SENS_XY_1P3GA       0.000909f
#define LSM303DLHC_COMP_SENS_XY_1P9GA       0.001169f
#define LSM303DLHC_COMP_SENS_XY_2P5GA       0.0014925f
#define LSM303DLHC_COMP_SENS_XY_4P0GA       0.0022222f
#define LSM303DLHC_COMP_SENS_XY_4P7GA       0.0025000f
#define LSM303DLHC_COMP_SENS_XY_5P6GA       0.0030303f
#define LSM303DLHC_COMP_SENS_XY_8P1GA       0.0043478f

#define LSM303DLHC_COMP_SENS_Z_1P3GA        0.0010204f
#define LSM303DLHC_COMP_SENS_Z_1P9GA        0.0013071f
#define LSM303DLHC_COMP_SENS_Z_2P5GA        0.0016666f
#define LSM303DLHC_COMP_SENS_Z_4P0GA        0.0025000f
#define LSM303DLHC_COMP_SENS_Z_4P7GA        0.0028169f
#define LSM303DLHC_COMP_SENS_Z_5P6GA        0.0033898f
#define LSM303DLHC_COMP_SENS_Z_8P1GA        0.0048780f
/** @} */

/**
 * @name   LSM303DLHC communication interfaces related bit masks
 * @{
 */
#define LSM303DLHC_DI_MASK          0xFF        /**< Data In mask           */
#define LSM303DLHC_DI(n)            (1 << n)    /**< Data In bit n          */
#define LSM303DLHC_AD_MASK          0x7F        /**< Address Data mask      */
#define LSM303DLHC_AD(n)            (1 << n)    /**< Address Data bit n     */
#define LSM303DLHC_MS               (1 << 7)    /**< Multiple read write    */
/** @} */

/**
 * @name    LSM303DLHC register addresses
 * @{
 */
#define LSM303DLHC_AD_ACC_CTRL_REG1         0x20
#define LSM303DLHC_AD_ACC_CTRL_REG2         0x21
#define LSM303DLHC_AD_ACC_CTRL_REG3         0x22
#define LSM303DLHC_AD_ACC_CTRL_REG4         0x23
#define LSM303DLHC_AD_ACC_CTRL_REG5         0x24
#define LSM303DLHC_AD_ACC_CTRL_REG6         0x25
#define LSM303DLHC_AD_ACC_REFERENCE         0x26
#define LSM303DLHC_AD_ACC_STATUS_REG        0x27
#define LSM303DLHC_AD_ACC_OUT_X_L           0x28
#define LSM303DLHC_AD_ACC_OUT_X_H           0x29
#define LSM303DLHC_AD_ACC_OUT_Y_L           0x2A
#define LSM303DLHC_AD_ACC_OUT_Y_H           0x2B
#define LSM303DLHC_AD_ACC_OUT_Z_L           0x2C
#define LSM303DLHC_AD_ACC_OUT_Z_H           0x2D
#define LSM303DLHC_AD_ACC_FIFO_CTRL_REG     0x2E
#define LSM303DLHC_AD_ACC_FIFO_SRC_REG      0x2F
#define LSM303DLHC_AD_ACC_INT1_CFG          0x30
#define LSM303DLHC_AD_ACC_INT1_SRC          0x31
#define LSM303DLHC_AD_ACC_INT1_THS          0x32
#define LSM303DLHC_AD_ACC_INT1_DURATION     0x33
#define LSM303DLHC_AD_ACC_INT2_CFG          0x34
#define LSM303DLHC_AD_ACC_INT2_SRC          0x35
#define LSM303DLHC_AD_ACC_INT2_THS          0x36
#define LSM303DLHC_AD_ACC_INT2_DURATION     0x37
#define LSM303DLHC_AD_ACC_CLICK_CFG         0x38
#define LSM303DLHC_AD_ACC_CLICK_SRC         0x39
#define LSM303DLHC_AD_ACC_CLICK_THS         0x3A
#define LSM303DLHC_AD_ACC_TIME_LIMIT        0x3B
#define LSM303DLHC_AD_ACC_TIME_LATENCY      0x3C
#define LSM303DLHC_AD_ACC_TIME_WINDOW       0x3D
#define LSM303DLHC_AD_COMP_CRA_REG          0x00
#define LSM303DLHC_AD_COMP_CRB_REG          0x01
#define LSM303DLHC_AD_COMP_MR_REG           0x02
#define LSM303DLHC_AD_COMP_OUT_X_H          0x03
#define LSM303DLHC_AD_COMP_OUT_X_L          0x04
#define LSM303DLHC_AD_COMP_OUT_Z_H          0x05
#define LSM303DLHC_AD_COMP_OUT_Z_L          0x06
#define LSM303DLHC_AD_COMP_OUT_Y_H          0x07
#define LSM303DLHC_AD_COMP_OUT_Y_L          0x08
#define LSM303DLHC_AD_COMP_SR_REG           0x09
#define LSM303DLHC_AD_COMP_IRA_REG          0x0A
#define LSM303DLHC_AD_COMP_IRB_REG          0x0B
#define LSM303DLHC_AD_COMP_IRC_REG          0x0C
#define LSM303DLHC_AD_COMP_TEMP_OUT_H       0x31
#define LSM303DLHC_AD_COMP_TEMP_OUT_L       0x32
/** @} */

/**
 * @name    LSM303DLHC_CTRL_REG1_A register bits definitions
 * @{
 */
#define LSM303DLHC_CTRL_REG1_A_MASK         0xFF
#define LSM303DLHC_CTRL_REG1_A_XEN          (1 << 0)
#define LSM303DLHC_CTRL_REG1_A_YEN          (1 << 1)
#define LSM303DLHC_CTRL_REG1_A_ZEN          (1 << 2)
#define LSM303DLHC_CTRL_REG1_A_LPEN         (1 << 3)
#define LSM303DLHC_CTRL_REG1_A_ODR0         (1 << 4)
#define LSM303DLHC_CTRL_REG1_A_ODR1         (1 << 5)
#define LSM303DLHC_CTRL_REG1_A_ODR2         (1 << 6)
#define LSM303DLHC_CTRL_REG1_A_ODR3         (1 << 7)
/** @} */

/**
 * @name    LSM303DLHC_CTRL_REG2_A register bits definitions
 * @{
 */
#define LSM303DLHC_CTRL_REG2_A_MASK         0xFF
#define LSM303DLHC_CTRL_REG2_A_HPIS1        (1 << 0)
#define LSM303DLHC_CTRL_REG2_A_HPIS2        (1 << 1)
#define LSM303DLHC_CTRL_REG2_A_HPCLICK      (1 << 2)
#define LSM303DLHC_CTRL_REG2_A_FDS          (1 << 3)
#define LSM303DLHC_CTRL_REG2_A_HPCF1        (1 << 4)
#define LSM303DLHC_CTRL_REG2_A_HPCF2        (1 << 5)
#define LSM303DLHC_CTRL_REG2_A_HPM0         (1 << 6)
#define LSM303DLHC_CTRL_REG2_A_HPM1         (1 << 7)
/** @} */

/**
 * @name    LSM303DLHC_CTRL_REG3_A register bits definitions
 * @{
 */
#define LSM303DLHC_CTRL_REG3_A_MASK         0xFD
#define LSM303DLHC_CTRL_REG3_A_I1_OVERRUN   (1 << 1)
#define LSM303DLHC_CTRL_REG3_A_I1_WTM       (1 << 2)
#define LSM303DLHC_CTRL_REG3_A_I1_DRDY2     (1 << 3)
#define LSM303DLHC_CTRL_REG3_A_I1_DRDY1     (1 << 4)
#define LSM303DLHC_CTRL_REG3_A_I1_AOI2      (1 << 5)
#define LSM303DLHC_CTRL_REG3_A_I1_AOI1      (1 << 6)
#define LSM303DLHC_CTRL_REG3_A_I1_CLICK     (1 << 7)
/** @} */

/**
 * @name    LSM303DLHC_CTRL_REG4_A register bits definitions
 * @{
 */
#define LSM303DLHC_CTRL_REG4_A_MASK         0xF9
#define LSM303DLHC_CTRL_REG4_A_SIM          (1 << 0)
#define LSM303DLHC_CTRL_REG4_A_HR           (1 << 3)
#define LSM303DLHC_CTRL_REG4_A_FS_MASK      0x30
#define LSM303DLHC_CTRL_REG4_A_FS0          (1 << 4)
#define LSM303DLHC_CTRL_REG4_A_FS1          (1 << 5)
#define LSM303DLHC_CTRL_REG4_A_BLE          (1 << 6)
#define LSM303DLHC_CTRL_REG4_A_BDU          (1 << 7)
/** @} */

/**
 * @name    LSM303DLHC_CTRL_REG5_A register bits definitions
 * @{
 */
#define LSM303DLHC_CTRL_REG5_A_MASK         0xCF
#define LSM303DLHC_CTRL_REG5_A_D4D_INT2     (1 << 0)
#define LSM303DLHC_CTRL_REG5_A_LIR_INT2     (1 << 1)
#define LSM303DLHC_CTRL_REG5_A_D4D_INT1     (1 << 2)
#define LSM303DLHC_CTRL_REG5_A_LIR_INT1     (1 << 3)
#define LSM303DLHC_CTRL_REG5_A_FIFO_EN      (1 << 6)
#define LSM303DLHC_CTRL_REG5_A_BOOT         (1 << 7)
/** @} */

/**
 * @name    LSM303DLHC_CTRL_REG6_A register bits definitions
 * @{
 */
#define LSM303DLHC_CTRL_REG6_A_MASK         0xFA
#define LSM303DLHC_CTRL_REG6_A_H_LACTIVE    (1 << 1)
#define LSM303DLHC_CTRL_REG6_A_P2_ACT       (1 << 3)
#define LSM303DLHC_CTRL_REG6_A_BOOT_I1      (1 << 4)
#define LSM303DLHC_CTRL_REG6_A_I2_INT2      (1 << 5)
#define LSM303DLHC_CTRL_REG6_A_I2_INT1      (1 << 6)
#define LSM303DLHC_CTRL_REG6_A_I2_CLICKEN   (1 << 7)
/** @} */

/**
 * @name    LSM303DLHC_CRA_REG_M register bits definitions
 * @{
 */
#define LSM303DLHC_CRA_REG_M_MASK           0x9C
#define LSM303DLHC_CRA_REG_M_DO0            (1 << 2)
#define LSM303DLHC_CRA_REG_M_DO1            (1 << 3)
#define LSM303DLHC_CRA_REG_M_DO2            (1 << 4)
#define LSM303DLHC_CRA_REG_M_TEMP_EN        (1 << 7)
/** @} */

/**
 * @name    LSM303DLHC_CRB_REG_M register bits definitions
 * @{
 */
#define LSM303DLHC_CRB_REG_M_MASK           0xE0
#define LSM303DLHC_CRB_REG_M_GN_MASK        0xE0
#define LSM303DLHC_CRB_REG_M_GN0            (1 << 5)
#define LSM303DLHC_CRB_REG_M_GN1            (1 << 6)
#define LSM303DLHC_CRB_REG_M_GN2            (1 << 7)

/**
 * @name    LSM303DLHC_CRB_REG_M register bits definitions
 * @{
 */
#define LSM303DLHC_MR_REG_M_MASK            0x03
#define LSM303DLHC_MR_REG_M_MD0             (1 << 0)
#define LSM303DLHC_MR_REG_M_MD1             (1 << 1)

/** @} */

//TODO: ADD more LSM303DLHC register bits definitions

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   LSM303DLHC accelerometer subsystem advanced configurations 
 *          switch.
 * @details If set to @p TRUE more configurations are available.
 * @note    The default is @p FALSE.
 */
#if !defined(LSM303DLHC_ACC_USE_ADVANCED) || defined(__DOXYGEN__)
#define LSM303DLHC_ACC_USE_ADVANCED            FALSE
#endif

/**
 * @brief   LSM303DLHC compass subsystem advanced configurations 
 *          switch.
 * @details If set to @p TRUE more configurations are available.
 * @note    The default is @p FALSE.
 */
#if !defined(LSM303DLHC_COMP_USE_ADVANCED) || defined(__DOXYGEN__)
#define LSM303DLHC_COMP_USE_ADVANCED           FALSE
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

#if !HAL_USE_I2C
#error "LSM303DLHC requires HAL_USE_I2C"
#endif

#if LSM303DLHC_SHARED_I2C && !I2C_USE_MUTUAL_EXCLUSION
#error "LSM303DLHC_SHARED_I2C requires I2C_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @name    LSM303DLHC accelerometer subsystem data structures and types.
 * @{
 */

/**
 * @brief LSM303DLHC accelerometer subsystem full scale.
 */
typedef enum {
  LSM303DLHC_ACC_FS_2G   = 0x00,    /**< Full scale ±2g.                    */
  LSM303DLHC_ACC_FS_4G   = 0x10,    /**< Full scale ±4g.                    */
  LSM303DLHC_ACC_FS_8G   = 0x20,    /**< Full scale ±8g.                    */
  LSM303DLHC_ACC_FS_16G  = 0x30     /**< Full scale ±16g.                   */
} lsm303dlhc_acc_fs_t;

/**
 * @brief LSM303DLHC accelerometer subsystem output data rate.
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
 * @brief LSM303DLHC accelerometer subsystem axes enabling.
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
 * @brief LSM303DLHC accelerometer subsystem low power mode.
 */
typedef enum {
  LSM303DLHC_ACC_LP_DISABLED = 0x00,/**< Low power mode disabled.           */
  LSM303DLHC_ACC_LP_ENABLED = 0x40  /**< Low power mode enabled.            */
} lsm303dlhc_acc_lp_t;

/**
 * @brief LSM303DLHC accelerometer subsystem high resolution mode.
 */
typedef enum {
  LSM303DLHC_ACC_HR_DISABLED = 0x00,/**< High resolution mode disabled.     */
  LSM303DLHC_ACC_HR_ENABLED = 0x08  /**< High resolution mode enabled.      */
} lsm303dlhc_acc_hr_t;

/**
 * @brief LSM303DLHC accelerometer subsystem block data update.
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
 * @brief LSM303DLHC accelerometer subsystem configuration structure.
 */
typedef struct {
  /**
   * @brief LSM303DLHC initial sensitivity.
   */
  float                        *sensitivity;
  /**
   * @brief LSM303DLHC initial bias.
   */
  float                        *bias;
  /**
   * @brief LSM303DLHC accelerometer subsystem initial full scale.
   */
  lsm303dlhc_acc_fs_t          fullscale;
  /**
   * @brief LSM303DLHC accelerometer subsystem output data rate.
   */
  lsm303dlhc_acc_odr_t         outdatarate;
#if LSM303DLHC_ACC_USE_ADVANCED || defined(__DOXYGEN__)
  /**
   * @brief LSM303DLHC accelerometer subsystem low power mode.
   */
  lsm303dlhc_acc_lp_t          lowpower;
  /**
   * @brief LSM303DLHC accelerometer subsystem high resolution mode.
   */
  lsm303dlhc_acc_hr_t          highresmode;
  /**
   * @brief LSM303DLHC accelerometer subsystem block data update.
   */
  lsm303dlhc_acc_bdu_t         blockdataupdate;
  /**
   * @brief  LSM303DLHC accelerometer endianness.
   */
  lsm303dlhc_acc_end_t         endianess;
#endif
} LSM303DLHCAccConfig;
/** @} */

/**
 * @name    LSM303DLHC compass subsystem data structures and types.
 * @{
 */
/**
 * @brief LSM303DLHC compass subsystem full scale.
 */
typedef enum {
  LSM303DLHC_COMP_FS_1P3GA = 0x20, /**< Full scale ±1.3 Gauss              */
  LSM303DLHC_COMP_FS_1P9GA = 0x40, /**< Full scale ±1.9 Gauss              */
  LSM303DLHC_COMP_FS_2P5GA = 0x60, /**< Full scale ±2.5 Gauss              */
  LSM303DLHC_COMP_FS_4P0GA = 0x80, /**< Full scale ±4.0 Gauss              */
  LSM303DLHC_COMP_FS_4P7GA = 0xA0, /**< Full scale ±4.7 Gauss              */
  LSM303DLHC_COMP_FS_5P6GA = 0xC0, /**< Full scale ±5.6 Gauss              */
  LSM303DLHC_COMP_FS_8P1GA = 0xE0  /**< Full scale ±8.1 Gauss              */
} lsm303dlhc_comp_fs_t;

/**
 * @brief LSM303DLHC compass subsystem output data rate.
 */
typedef enum {
  LSM303DLHC_COMP_ODR_0P75HZ = 0x00,/**< ODR 0.75 Hz                        */
  LSM303DLHC_COMP_ODR_1P5HZ = 0x04, /**< ODR 1.5 Hz                         */
  LSM303DLHC_COMP_ODR_3P0HZ = 0x08, /**< ODR 3 Hz                           */
  LSM303DLHC_COMP_ODR_7P5HZ = 0x0C, /**< ODR 7.5 Hz                         */
  LSM303DLHC_COMP_ODR_15HZ = 0x10,  /**< ODR 15 Hz                          */
  LSM303DLHC_COMP_ODR_30HZ = 0x14,  /**< ODR 30 Hz                          */
  LSM303DLHC_COMP_ODR_75HZ = 0x18,  /**< ODR 75 Hz                          */
  LSM303DLHC_COMP_ODR_220HZ = 0x1C  /**< ODR 220 Hz                         */
} lsm303dlhc_comp_odr_t;

/**
 * @brief LSM303DLHC compass subsystem working mode.
 */
typedef enum {
  LSM303DLHC_COMP_MD_CONT = 0x00,   /**< Continuous-Conversion Mode         */
  LSM303DLHC_COMP_MD_BLOCK = 0x01,  /**< Single-Conversion Mode             */
  LSM303DLHC_COMP_MD_SLEEP = 0x02   /**< Sleep Mode                         */
} lsm303dlhc_comp_md_t;

/**
 * @brief LSM303DLHC compass subsystem configuration structure.
 */
typedef struct {
  /**
   * @brief LSM303DLHC compass initial sensitivity.
   */
  float                        *sensitivity;
  /**
   * @brief LSM303DLHC compass initial bias.
   */
  float                        *bias;
  /**
   * @brief LSM303DLHC compass subsystem initial full scale.
   */
  lsm303dlhc_comp_fs_t         fullscale;
  /**
   * @brief LSM303DLHC compass subsystem output data rate.
   */
  lsm303dlhc_comp_odr_t        outputdatarate;
#if LSM303DLHC_COMP_USE_ADVANCED || defined(__DOXYGEN__)
  /**
   * @brief LSM303DLHC compass subsystem working mode.
   */
  lsm303dlhc_comp_md_t         mode;
#endif
} LSM303DLHCCompConfig;
/** @} */

/**
 * @name    LSM303DLHC main system data structures and types.
 * @{
 */

/**
 * @brief Driver state machine possible states.
 */
typedef enum {
  LSM303DLHC_UNINIT = 0,            /**< Not initialized.                   */
  LSM303DLHC_STOP = 1,              /**< Stopped.                           */
  LSM303DLHC_READY = 2,             /**< Ready.                             */
} lsm303dlhc_state_t;

/**
 * @brief LSM303DLHC configuration structure.
 */
typedef struct {
  /**
   * @brief I2C driver associated to this LSM303DLHC.
   */
  I2CDriver                 *i2cp;
  /**
   * @brief I2C configuration associated to this LSM303DLHC accelerometer
   *        subsystem.
   */
  const I2CConfig           *i2ccfg;
  /**
   * @brief LSM303DLHC accelerometer subsystem configuration structure
   */
  const LSM303DLHCAccConfig *acccfg;
  /**
   * @brief LSM303DLHC compass subsystem configuration structure
   */
  const LSM303DLHCCompConfig *compcfg;
} LSM303DLHCConfig;

/**
 * @brief Structure representing a LSM303DLHC driver.
 */
typedef struct LSM303DLHCDriver LSM303DLHCDriver;

/**
 * @brief @p LSM303DLHC accelerometer subsystem specific methods.
 */
#define _lsm303dlhc_acc_methods                                             \
  _base_accelerometer_methods

/**
 * @brief @p LSM303DLHC compass subsystem specific methods.
 */
#define _lsm303dlhc_comp_methods                                            \
  _base_compass_methods

/**
 * @extends BaseAccelerometerVMT
 *
 * @brief @p LSM303DLHC accelerometer virtual methods table.
 */
struct LSM303DLHCACCVMT {
  _lsm303dlhc_acc_methods                                                   \
  /* Change full scale value of LSM303DLHC accelerometer subsystem .*/      \
  msg_t (*set_full_scale)(void *instance, lsm303dlhc_acc_fs_t fs);          \
};

/**
 * @extends BaseCompassVMT
 *
 * @brief @p LSM303DLHC compass virtual methods table.
 */
struct LSM303DLHCCOMPVMT {
  _lsm303dlhc_comp_methods                                                  \
  /* Change full scale value of LSM303DLHC compass subsystem .*/            \
  msg_t (*set_full_scale)(void *instance, lsm303dlhc_comp_fs_t fs);         \
};

/**
 * @brief @p LSM303DLHCDriver specific data.
 */
#define _lsm303dlhc_data                                                    \
  _base_accelerometer_data                                                  \
  _base_compass_data                                                        \
  /* Driver state.*/                                                        \
  lsm303dlhc_state_t        state;                                          \
  /* Current configuration data.*/                                          \
  const LSM303DLHCConfig    *config;                                        \
  /* Current accelerometer sensitivity.*/                                   \
  float                     accsensitivity[LSM303DLHC_ACC_NUMBER_OF_AXES];  \
  /* Accelerometer bias data.*/                                             \
  float                     accbias[LSM303DLHC_ACC_NUMBER_OF_AXES];         \
  /* Current accelerometer full scale value.*/                              \
  float                     accfullscale;                                   \
  /* Current compass sensitivity.*/                                         \
  float                     compsensitivity[LSM303DLHC_COMP_NUMBER_OF_AXES];\
  /* Compass bias data.*/                                                   \
  float                     compbias[LSM303DLHC_COMP_NUMBER_OF_AXES];       \
  /* Current compass full scale value.*/                                    \
  float                     compfullscale;
  
/**
 * @brief LSM303DLHC 6-axis accelerometer/compass class.
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
        (ip)->vmt_lsm303dlhcacc->set_full_scale(ip, fs)

/**
 * @brief   Change compass fullscale value.
 *
 * @param[in] ip        pointer to a @p BaseCompass class.
 * @param[in] fs        the new full scale value.
 *
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 * @api
 */
#define compassSetFullScale(ip, fs)                                         \
        (ip)->vmt_lsm303dlhccomp->set_full_scale(ip, fs)
        
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
