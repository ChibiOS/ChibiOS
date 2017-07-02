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
 * @addtogroup LIS3MDL
 * @ingroup EX_ST
 * @{
 */
#ifndef _LIS3MDL_H_
#define _LIS3MDL_H_

#include "hal_compass.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Version identification
 * @{
 */
/**
 * @brief   LIS3MDL driver version string.
 */
#define EX_LIS3MDL_VERSION          "1.0.4"

/**
 * @brief   LIS3MDL driver version major number.
 */
#define EX_LIS3MDL_MAJOR            1

/**
 * @brief   LIS3MDL driver version minor number.
 */
#define EX_LIS3MDL_MINOR            0

/**
 * @brief   LIS3MDL driver version patch number.
 */
#define EX_LIS3MDL_PATCH            4
/** @} */

/**
 * @brief   LIS3MDL characteristics.
 *
 * @{
 */
#define LIS3MDL_NUMBER_OF_AXES      3U

#define LIS3MDL_4GA                 4.0f
#define LIS3MDL_8GA                 8.0f
#define LIS3MDL_12GA                12.0f
#define LIS3MDL_16GA                16.0f

#define LIS3MDL_SENS_4GA            0.00014615f
#define LIS3MDL_SENS_8GA            0.00029231f
#define LIS3MDL_SENS_12GA           0.0004384f
#define LIS3MDL_SENS_16GA           0.00058445f
/** @} */

/**
 * @name    LIS3MDL communication interfaces related bit masks
 * @{
 */
#define LIS3MDL_DI_MASK             0xFF        /**< Data In mask           */
#define LIS3MDL_DI(n)               (1 << n)    /**< Data In bit n          */
#define LIS3MDL_AD_MASK             0x3F        /**< Address Data mask      */
#define LIS3MDL_AD(n)               (1 << n)    /**< Address Data bit n     */
#define LIS3MDL_MS                  (1 << 6)    /**< Multiple selection     */
#define LIS3MDL_RW                  (1 << 7)    /**< Read Write selector    */

#define LIS3MDL_SUB_MS              (1 << 7)    /**< Multiple RW in I2C mode*/
/** @} */

/**
 * @name    LIS3MDL register addresses
 * @{
 */
#define  LIS3MDL_AD_WHO_AM_I        0x0F
#define  LIS3MDL_AD_CTRL_REG1       0x20
#define  LIS3MDL_AD_CTRL_REG2       0x21
#define  LIS3MDL_AD_CTRL_REG3       0x22
#define  LIS3MDL_AD_CTRL_REG4       0x23
#define  LIS3MDL_AD_CTRL_REG5       0x24
#define  LIS3MDL_AD_STATUS_REG      0x27
#define  LIS3MDL_AD_OUT_X_L         0x28
#define  LIS3MDL_AD_OUT_X_H         0x29
#define  LIS3MDL_AD_OUT_Y_L         0x2A
#define  LIS3MDL_AD_OUT_Y_H         0x2B
#define  LIS3MDL_AD_OUT_Z_L         0x2C
#define  LIS3MDL_AD_OUT_Z_H         0x2D
#define  LIS3MDL_AD_TEMP_OUT_L      0x2E
#define  LIS3MDL_AD_TEMP_OUT_H      0x2F
#define  LIS3MDL_AD_INT_CFG         0x30
#define  LIS3MDL_AD_INT_SOURCE      0x31
#define  LIS3MDL_AD_INT_THS_L       0x32
#define  LIS3MDL_AD_INT_THS_H       0x33
/** @} */

/**
 * @name    LIS3MDL_CTRL_REG1 register bits definitions
 * @{
 */
#define LIS3MDL_CTRL_REG1_MASK      0xFF        /**< LIS3MDL_CTRL_REG1 mask  */
#define LIS3MDL_CTRL_REG1_ST        (1 << 0)    /**< Self test enable        */
#define LIS3MDL_CTRL_REG1_FAST_ODR  (1 << 1)    /**< Fast data rate          */
#define LIS3MDL_CTRL_REG1_DO0       (1 << 2)    /**< Output data rate bit 0  */
#define LIS3MDL_CTRL_REG1_DO1       (1 << 3)    /**< Output data rate bit 1  */
#define LIS3MDL_CTRL_REG1_DO2       (1 << 4)    /**< Output data rate bit 2  */
#define LIS3MDL_CTRL_REG1_OM0       (1 << 5)    /**< X-Y mode bit 0          */
#define LIS3MDL_CTRL_REG1_OM1       (1 << 6)    /**< X-Y mode bit 1          */
#define LIS3MDL_CTRL_REG1_TEMP_EN   (1 << 7)    /**< Temperature sensor      */
/** @} */

/**
 * @name    LIS3MDL_CTRL_REG2 register bits definitions
 * @{
 */
#define LIS3MDL_CTRL_REG2_MASK      0x6C        /**< LIS3MDL_CTRL_REG2 mask  */
#define LIS3MDL_CTRL_REG2_SOFT_RST  (1 << 2)    /**< Soft reset              */
#define LIS3MDL_CTRL_REG2_REBOOT    (1 << 3)    /**< Reboot memory           */
#define LIS3MDL_CTRL_REG2_FS_MASK   0x60        /**< Full scale mask         */
#define LIS3MDL_CTRL_REG2_FS0       (1 << 5)    /**< Full scale bit 0        */
#define LIS3MDL_CTRL_REG2_FS1       (1 << 6)    /**< Full scale bit 1        */
/** @} */

/**
 * @name    LIS3MDL_CTRL_REG3 register bits definitions
 * @{
 */
#define LIS3MDL_CTRL_REG3_MASK      0x27        /**< LIS3MDL_CTRL_REG3 mask  */
#define LIS3MDL_CTRL_REG3_MD0       (1 << 0)    /**< Operating mode bit 0    */
#define LIS3MDL_CTRL_REG3_MD1       (1 << 1)    /**< Operating mode bit 1    */
#define LIS3MDL_CTRL_REG3_SIM       (1 << 2)    /**< SPI interface mode      */
#define LIS3MDL_CTRL_REG3_LP        (1 << 5)    /**< Low power               */
/** @} */

/**
 * @name    LIS3MDL_CTRL_REG4 register bits definitions
 * @{
 */
#define LIS3MDL_CTRL_REG4_MASK      0x0E        /**< LIS3MDL_CTRL_REG4 mask  */
#define LIS3MDL_CTRL_REG4_BLE       (1 << 1)    /**< Endianess               */
#define LIS3MDL_CTRL_REG4_OMZ0      (1 << 2)    /**< Z mode bit 0            */
#define LIS3MDL_CTRL_REG4_OMZ1      (1 << 3)    /**< Z mode bit 1            */
/** @} */

/**
 * @name    LIS3MDL_CTRL_REG5 register bits definitions
 * @{
 */
#define LIS3MDL_CTRL_REG5_MASK      0xC0        /**< LIS3MDL_CTRL_REG5 mask  */
#define LIS3MDL_CTRL_REG5_BDU       (1 << 6)    /**< Block data update       */
#define LIS3MDL_CTRL_REG5_FAST_READ (1 << 7)    /**< Fast read mode          */
/** @} */

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
 * @brief   LIS3MDL advanced configurations switch.
 * @details If set to @p TRUE more configurations are available.
 * @note    The default is @p FALSE.
 */
#if !defined(LIS3MDL_USE_ADVANCED) || defined(__DOXYGEN__)
#define LIS3MDL_USE_ADVANCED                FALSE
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
  LIS3MDL_FS_8GA = 0x20,            /**< ±8 Gauss                           */
  LIS3MDL_FS_12GA = 0x40,           /**< ±12 Gauss                          */
  LIS3MDL_FS_16GA = 0x60            /**< ±16 Gauss                          */
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
   * @brief LIS3MDL initial sensitivity.
   */
  float                     *sensitivity;
  /**
   * @brief LIS3MDL initial bias.
   */
  float                     *bias;
  /**
   * @brief  LIS3MDL slave address
   */
  lis3mdl_sad_t             slaveaddress;
  /**
   * @brief   LIS3MDL full scale
   */
  lis3mdl_fs_t              fullscale;
  /**
   * @brief   LIS3MDL output data rate
   */
  lis3mdl_odr_t             outputdatarate;
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
  /**
   * @brief  LIS3MDL low power mode configuration
   */
  lis3mdl_lp_t              lowpowermode;
  /**
   * @brief  LIS3MDL conversion mode
   */
  lis3mdl_md_t              conversionmode;
  /**
   * @brief  LIS3MDL operation mode for X and Y axes
   */
  lis3mdl_omxy_t            operationmodexy;
  /**
   * @brief  LIS3MDL operation mode for Z axis
   */
  lis3mdl_omz_t             operationmodez;
  /**
   * @brief  LIS3MDL block data update
   */
  lis3mdl_bdu_t             blockdataupdate;
  /**
   * @brief   LIS3MDL endianness
   */
  lis3mdl_end_t             endianness;
#endif
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
  /* Change full scale value of LIS3MDL.*/                                  \
  msg_t (*set_full_scale)(void *instance, lis3mdl_fs_t fs);

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
  float                     bias[LIS3MDL_NUMBER_OF_AXES];                   \
  /* Current full scale value.*/                                            \
  float                     fullscale;

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
 * @brief   Change compass full scale value.
 *
 * @param[in] ip        pointer to a @p BaseCompass class.
 * @param[in] fs        the new full scale value.
 *
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 * @api
 */
#define compassSetFullScale(ip, fs)                                       \
        (ip)->vmt_lis3mdl->set_full_scale(ip, fs)

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

