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
 * @addtogroup LIS302DL
 * @ingroup EX_ST
 * @{
 */

#ifndef _LIS302DL_H_
#define _LIS302DL_H_

#include "hal_accelerometer.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Version identification
 * @{
 */
/**
 * @brief   LIS302DL driver version string.
 */
#define EX_LIS302DL_VERSION         "1.0.3"

/**
 * @brief   LIS302DL driver version major number.
 */
#define EX_LIS302DL_MAJOR           1

/**
 * @brief   LIS302DL driver version minor number.
 */
#define EX_LIS302DL_MINOR           0

/**
 * @brief   LIS302DL driver version patch number.
 */
#define EX_LIS302DL_PATCH           3
/** @} */

/**
 * @brief   LIS302DL characteristics.
 *
 * @{
 */
#define LIS302DL_NUMBER_OF_AXES     3U

#define LIS302DL_2G                 2.0f
#define LIS302DL_8G                 8.0f

#define LIS302DL_SENS_2G            18.0f
#define LIS302DL_SENS_8G            72.0f
/** @} */

/**
 * @name    LIS302DL communication interfaces related bit masks
 * @{
 */
#define LIS302DL_DI_MASK            0xFF        /**< Data In mask           */
#define LIS302DL_DI(n)              (1 << n)    /**< Data In bit n          */
#define LIS302DL_AD_MASK            0x3F        /**< Address Data mask      */
#define LIS302DL_AD(n)              (1 << n)    /**< Address Data bit n     */
#define LIS302DL_MS                 (1 << 6)    /**< Multiple read write    */
#define LIS302DL_RW                 (1 << 7)    /**< Read Write selector    */
/** @} */

/**
 * @name    LIS302DL register addresses
 * @{
 */
#define  LIS302DL_AD_WHO_AM_I               0x0F
#define  LIS302DL_AD_CTRL_REG1              0x20
#define  LIS302DL_AD_CTRL_REG2              0x21
#define  LIS302DL_AD_CTRL_REG3              0x22
#define  LIS302DL_AD_HP_FILER_RESET         0x23
#define  LIS302DL_AD_STATUS_REG             0x27
#define  LIS302DL_AD_OUT_X                  0x29
#define  LIS302DL_AD_OUT_Y                  0x2B
#define  LIS302DL_AD_OUT_Z                  0x2D
#define  LIS302DL_AD_FF_WU_CFG_1            0x30
#define  LIS302DL_AD_FF_WU_SRC_1            0x31
#define  LIS302DL_AD_FF_WU_THS_1            0x32
#define  LIS302DL_AD_FF_WU_DURATION_1       0x33
#define  LIS302DL_AD_FF_WU_CFG_2            0x34
#define  LIS302DL_AD_FF_WU_SRC_2            0x35
#define  LIS302DL_AD_FF_WU_THS_2            0x36
#define  LIS302DL_AD_FF_WU_DURATION_2       0x37
#define  LIS302DL_AD_CLICK_CFG              0x38
#define  LIS302DL_AD_CLICK_SRC              0x39
#define  LIS302DL_AD_CLICK_THSY_X           0x3B
#define  LIS302DL_AD_CLICK_THSZ             0x3C
#define  LIS302DL_AD_CLICK_TIME_LIMIT       0x3D
#define  LIS302DL_AD_CLICK_LATENCY          0x3E
#define  LIS302DL_AD_CLICK_WINDOW           0x3F
/** @} */

/**
 * @name    LIS302DL_CTRL_REG1 register bits definitions
 * @{
 */
#define LIS302DL_CTRL_REG1_MASK     0xFF        /**< LIS302DL_CTRL_REG1 mask  */
#define LIS302DL_CTRL_REG1_XEN      (1 << 0)    /**< X axis enable            */
#define LIS302DL_CTRL_REG1_YEN      (1 << 1)    /**< Y axis enable            */
#define LIS302DL_CTRL_REG1_ZEN      (1 << 2)    /**< Z axis enable            */
#define LIS302DL_CTRL_REG1_STM      (1 << 3)    /**< Self test P-M            */
#define LIS302DL_CTRL_REG1_STP      (1 << 4)    /**< Self test P-M            */
#define LIS302DL_CTRL_REG1_FS_MASK  0x20        /**< Full scale field mask    */
#define LIS302DL_CTRL_REG1_FS       (1 << 5)    /**< Full scale               */
#define LIS302DL_CTRL_REG1_PD       (1 << 6)    /**< Power-down mode enable   */
#define LIS302DL_CTRL_REG1_DR       (1 << 7)    /**< Output data rate         */
/** @} */

/**
 * @name    LIS302DL_CTRL_REG2 register bits definitions
 * @{
 */                                              
#define LIS302DL_CTRL_REG2_MASK     0xDF        /**< LIS302DL_CTRL_REG2 mask  */
#define LIS302DL_CTRL_REG2_HPCF1    (1 << 0)    /**< HP filter cutoff bit 0   */
#define LIS302DL_CTRL_REG2_HPCF2    (1 << 1)    /**< HP filter cutoff bit 1   */
#define LIS302DL_CTRL_REG2_HPFFWU1  (1 << 2)    /**< HP filter cutoff bit 2   */
#define LIS302DL_CTRL_REG2_HPFFWU2  (1 << 3)    /**< HP filter cutoff bit 3   */
#define LIS302DL_CTRL_REG2_FDS      (1 << 4)    /**< HP filter mode bit 0     */
#define LIS302DL_CTRL_REG2_BOOT     (1 << 6)    /**< HP filter mode bit 1     */
#define LIS302DL_CTRL_REG2_SIM      (1 << 7)    /**< HP filter mode bit 1     */
/** @} */

/**
 * @name    LIS302DL_CTRL_REG3 register bits definitions
 * @{
 */                                               
#define LIS302DL_CTRL_REG3_MASK     0xFF        /**< LIS302DL_CTRL_REG3 mask  */
#define LIS302DL_CTRL_REG3_I1CFG0   (1 << 0)    /**< Interrupt 1 config bit 0 */
#define LIS302DL_CTRL_REG3_I1CFG1   (1 << 1)    /**< Interrupt 1 config bit 1 */
#define LIS302DL_CTRL_REG3_I1CFG2   (1 << 2)    /**< Interrupt 1 config bit 2 */
#define LIS302DL_CTRL_REG3_I2CFG0   (1 << 3)    /**< Interrupt 2 config bit 0 */
#define LIS302DL_CTRL_REG3_I2CFG1   (1 << 4)    /**< Interrupt 2 config bit 1 */
#define LIS302DL_CTRL_REG3_I2CFG2   (1 << 5)    /**< Interrupt 2 config bit 2 */
#define LIS302DL_CTRL_REG3_PP_OD    (1 << 6)    /**< Push-pull open-drain     */
#define LIS302DL_CTRL_REG3_IHL      (1 << 7)    /**< Int active high low      */
/** @} */

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
 * @brief   LIS302DL advanced configurations switch.
 * @details If set to @p TRUE more configurations are available.
 * @note    The default is @p FALSE.
 */
#if !defined(LIS302DL_USE_ADVANCED) || defined(__DOXYGEN__)
#define LIS302DL_USE_ADVANCED               FALSE
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
   * @brief LIS302DL initial sensitivity.
   */
  float                     *sensitivity;
  /**
   * @brief LIS302DL initial bias.
   */
  float                     *bias;
  /**
   * @brief LIS302DL full scale value.
   */
  lis302dl_fs_t             fullscale;
  /**
   * @brief LIS302DL output data rate selection.
   */
  lis302dl_odr_t            outputdatarate;
#if LIS302DL_USE_ADVANCED || defined(__DOXYGEN__)
  /**
   * @brief LIS302DL high pass filtering.
   */
  lis302dl_hp_t             highpass;
#endif
} LIS302DLConfig;

/**
 * @brief   Structure representing a LIS302DL driver.
 */
typedef struct LIS302DLDriver LIS302DLDriver;

/**
 * @brief   @p LIS302DL specific methods.
 */
#define _lis302dl_methods                                                   \
  _base_accelerometer_methods                                               \
  /* Change full scale value of LIS302DL .*/                                \
  msg_t (*set_full_scale)(void *instance, lis302dl_fs_t fs);


/**
 * @extends BaseAccelerometerVMT
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
  lis302dl_state_t          state;                                          \
  /* Current configuration data.*/                                          \
  const LIS302DLConfig      *config;                                        \
  /* Current sensitivity.*/                                                 \
  float                     sensitivity[LIS302DL_NUMBER_OF_AXES];           \
  /* Bias data.*/                                                           \
  int32_t                   bias[LIS302DL_NUMBER_OF_AXES];                  \
  /* Current full scale value.*/                                            \
  float                     fullscale;

/**
 * @extends BaseAccelerometer
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
        (ip)->vmt_lis302dl->set_full_scale(ip, fs)
        

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

