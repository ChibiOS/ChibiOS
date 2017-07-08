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
 * @file    lps25h.h
 * @brief   LPS25H MEMS interface module header.
 *
 * @addtogroup LPS25H
 * @ingroup EX_ST
 * @{
 */
#ifndef _LPS25H_H_
#define _LPS25H_H_

#include "hal_barometer.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Version identification
 * @{
 */
/**
 * @brief   LPS25H driver version string.
 */
#define EX_LPS25H_VERSION           "1.0.3"

/**
 * @brief   LPS25H driver version major number.
 */
#define EX_LPS25H_MAJOR             1

/**
 * @brief   LPS25H driver version minor number.
 */
#define EX_LPS25H_MINOR             0

/**
 * @brief   LPS25H driver version patch number.
 */
#define EX_LPS25H_PATCH             3
/** @} */

/**
 * @brief   LPS25H characteristics.
 *
 * @{
 */
#define LPS25H_NUMBER_OF_AXES       1U

#define LPS25H_SENS                 0.00024414f /**< hPa/LSB                */
/** @} */

/**
 * @name    LPS25H communication interfaces related bit masks
 * @{
 */
#define LPS25H_DI_MASK              0xFF        /**< Data In mask           */
#define LPS25H_DI(n)                (1 << n)    /**< Data In bit n          */
#define LPS25H_AD_MASK              0x3F        /**< Address Data mask      */
#define LPS25H_AD(n)                (1 << n)    /**< Address Data bit n     */
#define LPS25H_MS                   (1 << 6)    /**< Multiple read write    */
#define LPS25H_RW                   (1 << 7)    /**< Read Write selector    */

#define LPS25H_SUB_MS               (1 << 7)    /**< Multiple read write in I2C mode */
/** @} */

/**
 * @name    LPS25H register addresses
 * @{
 */
#define LPS25H_AD_REF_P_XL          0x08
#define LPS25H_AD_REF_P_L           0x09
#define LPS25H_AD_REF_P_H           0x0A
#define LPS25H_AD_WHO_AM_I          0x0F
#define LPS25H_AD_RES_CONF          0x10
#define LPS25H_AD_CTRL_REG1         0x20
#define LPS25H_AD_CTRL_REG2         0x21
#define LPS25H_AD_CTRL_REG3         0x22
#define LPS25H_AD_CTRL_REG4         0x23
#define LPS25H_AD_INT_CFG           0x24
#define LPS25H_AD_INT_SRC           0x25
#define LPS25H_AD_STATUS_REG        0x27
#define LPS25H_AD_PRESS_OUT_XL      0x28
#define LPS25H_AD_PRESS_OUT_L       0x29
#define LPS25H_AD_PRESS_OUT_H       0x2A
#define LPS25H_AD_TEMP_OUT_L        0x2B
#define LPS25H_AD_TEMP_OUT_H        0x2C
#define LPS25H_AD_FIFO_CTRL         0x2E
#define LPS25H_AD_FIFO_SRC          0x2F
#define LPS25H_AD_THS_P_L           0x30
#define LPS25H_AD_THS_P_H           0x31
#define LPS25H_AD_RPDS_L            0x39
#define LPS25H_AD_RPDS_H            0x3A
/** @} */

/**
 * @name    LPS25H_CTRL_REG1 register bits definitions
 * @{
 */
#define LPS25H_CTRL_REG1_MASK               0xFF
#define LPS25H_CTRL_REG1_SIM                (1 << 0)
#define LPS25H_CTRL_REG1_RESET_AZ           (1 << 1)
#define LPS25H_CTRL_REG1_BDU                (1 << 2)
#define LPS25H_CTRL_REG1_DIFF_EN            (1 << 3)
#define LPS25H_CTRL_REG1_ODR0               (1 << 4)
#define LPS25H_CTRL_REG1_ODR1               (1 << 5)
#define LPS25H_CTRL_REG1_ODR2               (1 << 6)
#define LPS25H_CTRL_REG1_PD                 (1 << 7)
/** @} */

/**
 * @name    LPS25H_CTRL_REG2 register bits definitions
 * @{
 */
#define LPS25H_CTRL_REG2_MASK               0xF3
#define LPS25H_CTRL_REG2_ONE_SHOT           (1 << 0)
#define LPS25H_CTRL_REG2_AUTO_ZERO          (1 << 1)
#define LPS25H_CTRL_REG2_SWRESET            (1 << 2)
#define LPS25H_CTRL_REG2_FIFO_MEAN_DEC      (1 << 4)
#define LPS25H_CTRL_REG2_WTM_EN             (1 << 5)
#define LPS25H_CTRL_REG2_FIFO_EN            (1 << 6)
#define LPS25H_CTRL_REG2_BOOT               (1 << 7)
/** @} */

/**
 * @name    LPS25H_CTRL_REG3 register bits definitions
 * @{
 */
#define LPS25H_CTRL_REG3_MASK               0xC3
#define LPS25H_CTRL_REG3_INT_S1             (1 << 0)
#define LPS25H_CTRL_REG3_INT_S2             (1 << 1)
#define LPS25H_CTRL_REG3_PP_OD              (1 << 6)
#define LPS25H_CTRL_REG3_INT_H_L            (1 << 7)
/** @} */

/**
 * @name    LPS25H_CTRL_REG4 register bits definitions
 * @{
 */
#define LPS25H_CTRL_REG4_MASK               0x0F
#define LPS25H_CTRL_REG4_P1_DRDY            (1 << 0)
#define LPS25H_CTRL_REG4_P1_OVERRUN         (1 << 1)
#define LPS25H_CTRL_REG4_P1_WTM             (1 << 2)
#define LPS25H_CTRL_REG4_P1_EMPTY           (1 << 3)
/** @} */

/**
 * @name    LPS25H_INT1_CFG register bits definitions
 * @{
 */
#define LPS25H_INT1_CFG_MASK                0x07
#define LPS25H_INT1_CFG_PH_E                (1 << 0)
#define LPS25H_INT1_CFG_PL_E                (1 << 1)
#define LPS25H_INT1_CFG_LIR                 (1 << 2)
/** @} */

/**
 * @name    LPS25H_INT1_SRC register bits definitions
 * @{
 */
#define LPS25H_INT1_SRC_MASK                0x07
#define LPS25H_INT1_SRC_PH                  (1 << 0)
#define LPS25H_INT1_SRC_PL                  (1 << 1)
#define LPS25H_INT1_SRC_IA                  (1 << 2)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   LPS25H SPI interface switch.
 * @details If set to @p TRUE the support for SPI is included.
 * @note    The default is @p FALSE.
 */
#if !defined(LPS25H_USE_SPI) || defined(__DOXYGEN__)
#define LPS25H_USE_SPI                      FALSE
#endif

/**
 * @brief   LPS25H I2C interface switch.
 * @details If set to @p TRUE the support for I2C is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LPS25H_USE_I2C) || defined(__DOXYGEN__)
#define LPS25H_USE_I2C                      TRUE
#endif

/**
 * @brief   LPS25H advanced configurations switch.
 * @details If set to @p TRUE more configurations are available.
 * @note    The default is @p FALSE.
 */
#if !defined(LPS25H_USE_ADVANCED) || defined(__DOXYGEN__)
#define LPS25H_USE_ADVANCED                 FALSE
#endif

/**
 * @brief   LPS25H shared I2C switch.
 * @details If set to @p TRUE the device acquires I2C bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires I2C_USE_MUTUAL_EXCLUSION
 */
#if !defined(LPS25H_SHARED_I2C) || defined(__DOXYGEN__)
#define LPS25H_SHARED_I2C                   FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !(LPS25H_USE_SPI ^ LPS25H_USE_I2C)
#error "LPS25H_USE_SPI and LPS25H_USE_I2C cannot be both true or both false"
#endif

#if LPS25H_USE_SPI && !HAL_USE_SPI
#error "LPS25H_USE_SPI requires HAL_USE_SPI"
#endif

#if LPS25H_USE_SPI
#error "LPS25H over SPI still not supported"
#endif

#if LPS25H_USE_I2C && !HAL_USE_I2C
#error "LPS25H_USE_I2C requires HAL_USE_I2C"
#endif

#if LPS25H_SHARED_I2C && !I2C_USE_MUTUAL_EXCLUSION
#error "LPS25H_SHARED_I2C requires I2C_USE_MUTUAL_EXCLUSION"
#endif

/*
 * TODO: Add SPI support.
 */

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @name    LPS25H data structures and types.
 * @{
 */
/**
 * @brief  LPS25H slave address
 */
typedef enum {
  LPS25H_SAD_GND = 0x5C,           /**< Slave Address when SA0 is to GND   */
  LPS25H_SAD_VCC = 0x5D            /**< Slave Address when SA0 is to VCC   */
}lps25h_sad_t; 
 
/**
 * @brief   LPS25H output data rate and bandwidth.
 */
typedef enum {
  LPS25H_ODR_ONE_SHOT = 0x00,       /**< One shot.                          */
  LPS25H_ODR_1HZ = 0x10,            /**< Output data rate 1 Hz.             */
  LPS25H_ODR_7HZ = 0x20,            /**< Output data rate 7 Hz.             */
  LPS25H_ODR_12P5HZ = 0x30,         /**< Output data rate 12.5 Hz.          */
  LPS25H_ODR_25HZ = 0x40            /**< Output data rate 25 Hz.            */
}lps25h_odr_t;

/**
 * @brief   LPS25H pressure resolution.
 */
typedef enum {
  LPS25H_AVGP_8 = 0x00,             /**< Number of internal average is 8.   */
  LPS25H_AVGP_32 = 0x01,            /**< Number of internal average is 32.  */
  LPS25H_AVGP_128 = 0x02,           /**< Number of internal average is 128. */
  LPS25H_AVGP_512 = 0x03,           /**< Number of internal average is 512. */
}lps25h_avgp_t;

/**
 * @brief   LPS25H temperature resolution.
 */
typedef enum {
  LPS25H_AVGT_8 = 0x00,             /**< Number of internal average is 8.   */
  LPS25H_AVGT_32 = 0x04,            /**< Number of internal average is 32.  */
  LPS25H_AVGT_128 = 0x08,           /**< Number of internal average is 128. */
  LPS25H_AVGT_512 = 0x0C,           /**< Number of internal average is 512. */
}lps25h_avgt_t;

/**
 * @brief   LPS25H block data update.
 */
typedef enum {
  LPS25H_BDU_CONTINUOUS = 0x00,     /**< Block data continuously updated.   */
  LPS25H_BDU_BLOCKED = 0x40         /**< Block data updated after reading.  */
}lps25h_bdu_t;

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  LPS25H_UNINIT = 0,                /**< Not initialized.                   */
  LPS25H_STOP = 1,                  /**< Stopped.                           */
  LPS25H_READY = 2,                 /**< Ready.                             */
} lps25h_state_t;

/**
 * @brief   LPS25H configuration structure.
 */
typedef struct {

#if LPS25H_USE_SPI || defined(__DOXYGEN__)
  /**
   * @brief SPI driver associated to this LPS25H.
   */
  SPIDriver                 *spip;
  /**
   * @brief SPI configuration associated to this LPS25H.
   */
  const SPIConfig           *spicfg;
#endif /* LPS25H_USE_SPI */
#if LPS25H_USE_I2C || defined(__DOXYGEN__)
  /**
   * @brief I2C driver associated to this LPS25H.
   */
  I2CDriver                 *i2cp;
  /**
   * @brief I2C configuration associated to this LPS25H.
   */
  const I2CConfig           *i2ccfg;
#endif /* LPS25H_USE_I2C */
  /**
   * @brief LPS25H initial sensitivity.
   */
  float*                    sensitivity;
  /**
   * @brief LPS25H initial bias.
   */
  float*                    bias;
  /**
   * @brief LPS25H slave address
   */
  lps25h_sad_t              slaveaddress;
  /**
   * @brief LPS25H output data rate selection.
   */
  lps25h_odr_t              outputdatarate;
#if LPS25H_USE_ADVANCED || defined(__DOXYGEN__)
  /**
   * @brief LPS25H block data update.
   */
  lps25h_bdu_t              blockdataupdate;
  /**
   * @brief   LPS25H pressure resolution.
   */
  lps25h_avgp_t             respressure;
  /**
   * @brief   LPS25H temperature resolution.
   */
  lps25h_avgt_t             restemperature;
#endif
} LPS25HConfig;

/**
 * @brief   Structure representing a LPS25H driver.
 */
typedef struct LPS25HDriver LPS25HDriver;

/**
 * @brief   @p LPS25HDriver specific data.
 */
#define _lps25h_data                                                        \
  _base_barometer_data                                                      \
  /* Driver state.*/                                                        \
  lps25h_state_t            state;                                          \
  /* Current configuration data.*/                                          \
  const LPS25HConfig        *config;                                        \
  /* Current sensitivity data.*/                                            \
  float                     sensitivity;                                    \
  /* Current Bias data.*/                                                   \
  float                     bias;

/**
 * @extends BaseGyroscope
 *
 * @brief   LPS25H 3-axis barometer class.
 * @details This class extends @p BaseGyroscope by adding physical
 *          driver implementation.
 */
struct LPS25HDriver {
  /** @brief BaseSensor Virtual Methods Table. */
  const struct BaseSensorVMT *vmt_basesensor;
  /** @brief BaseBarometer Virtual Methods Table. */
  const struct BaseBarometerVMT *vmt_basebarometer;
  /** @brief LPS25H Virtual Methods Table. */
  const struct LPS25HVMT *vmt_lps25h;
  _lps25h_data
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
  void lps25hObjectInit(LPS25HDriver *devp);
  void lps25hStart(LPS25HDriver *devp, const LPS25HConfig *config);
  void lps25hStop(LPS25HDriver *devp);
#ifdef __cplusplus
}
#endif

#endif /* _LPS25H_H_ */

/** @} */

