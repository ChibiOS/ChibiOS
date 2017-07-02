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
 * @file    l3gd20.h
 * @brief   L3GD20 MEMS interface module header.
 *
 * @addtogroup L3GD20
 * @ingroup EX_ST
 * @{
 */
#ifndef _L3GD20_H_
#define _L3GD20_H_

#include "hal_gyroscope.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Version identification
 * @{
 */
/**
 * @brief   L3GD20 driver version string.
 */
#define EX_L3GD20_VERSION           "1.0.4"

/**
 * @brief   L3GD20 driver version major number.
 */
#define EX_L3GD20_MAJOR             1

/**
 * @brief   L3GD20 driver version minor number.
 */
#define EX_L3GD20_MINOR             0

/**
 * @brief   L3GD20 driver version patch number.
 */
#define EX_L3GD20_PATCH             4
/** @} */

/**
 * @brief   L3GD20 characteristics.
 *
 * @{
 */
#define L3GD20_NUMBER_OF_AXES       3U

#define L3GD20_250DPS               250.0f
#define L3GD20_500DPS               500.0f
#define L3GD20_2000DPS              2000.0f

#define L3GD20_SENS_250DPS          0.00875f
#define L3GD20_SENS_500DPS          0.01750f
#define L3GD20_SENS_2000DPS         0.07000f
/** @} */

/**
 * @name    L3GD20 communication interfaces related bit masks
 * @{
 */
#define L3GD20_DI_MASK              0xFF        /**< Data In mask           */
#define L3GD20_DI(n)                (1 << n)    /**< Data In bit n          */
#define L3GD20_AD_MASK              0x3F        /**< Address Data mask      */
#define L3GD20_AD(n)                (1 << n)    /**< Address Data bit n     */
#define L3GD20_MS                   (1 << 6)    /**< Multiple read write    */
#define L3GD20_RW                   (1 << 7)    /**< Read Write selector    */
/** @} */

/**
 * @name    L3GD20 register addresses
 * @{
 */
#define L3GD20_AD_WHO_AM_I          0x0F
#define L3GD20_AD_CTRL_REG1         0x20
#define L3GD20_AD_CTRL_REG2         0x21
#define L3GD20_AD_CTRL_REG3         0x22
#define L3GD20_AD_CTRL_REG4         0x23
#define L3GD20_AD_CTRL_REG5         0x24
#define L3GD20_AD_REFERENCE         0x25
#define L3GD20_AD_OUT_TEMP          0x26
#define L3GD20_AD_STATUS_REG        0x27
#define L3GD20_AD_OUT_X_L           0x28
#define L3GD20_AD_OUT_X_H           0x29
#define L3GD20_AD_OUT_Y_L           0x2A
#define L3GD20_AD_OUT_Y_H           0x2B
#define L3GD20_AD_OUT_Z_L           0x2C
#define L3GD20_AD_OUT_Z_H           0x2D
#define L3GD20_AD_FIFO_CTRL_REG     0x2E
#define L3GD20_AD_FIFO_SRC_REG      0x2F
#define L3GD20_AD_INT1_CFG          0x30
#define L3GD20_AD_INT1_SRC          0x31
#define L3GD20_AD_INT1_THS_XH       0x32
#define L3GD20_AD_INT1_THS_XL       0x33
#define L3GD20_AD_INT1_THS_YH       0x34
#define L3GD20_AD_INT1_THS_YL       0x35
#define L3GD20_AD_INT1_THS_ZH       0x36
#define L3GD20_AD_INT1_THS_ZL       0x37
#define L3GD20_AD_INT1_DURATION     0x38
/** @} */

/**
 * @name    L3GD20_CTRL_REG1 register bits definitions
 * @{
 */
#define L3GD20_CTRL_REG1_MASK       0xFF        /**< L3GD20_CTRL_REG1 mask  */
#define L3GD20_CTRL_REG1_XEN        (1 << 0)    /**< X axis enable          */
#define L3GD20_CTRL_REG1_YEN        (1 << 1)    /**< Y axis enable          */
#define L3GD20_CTRL_REG1_ZEN        (1 << 2)    /**< Z axis enable          */
#define L3GD20_CTRL_REG1_PD         (1 << 3)    /**< Power-down mode enable */
#define L3GD20_CTRL_REG1_BW0        (1 << 4)    /**< Bandwidth bit 0        */
#define L3GD20_CTRL_REG1_BW1        (1 << 5)    /**< Bandwidth bit 1        */
#define L3GD20_CTRL_REG1_DR0        (1 << 6)    /**< Output data rate bit 0 */
#define L3GD20_CTRL_REG1_DR1        (1 << 7)    /**< Output data rate bit 1 */
/** @} */

/**
 * @name    L3GD20_CTRL_REG2 register bits definitions
 * @{
 */                                              
#define L3GD20_CTRL_REG2_MASK       0x3F        /**< L3GD20_CTRL_REG2 mask  */
#define L3GD20_CTRL_REG2_HPCF0      (1 << 0)    /**< HP filter cutoff bit 0 */
#define L3GD20_CTRL_REG2_HPCF1      (1 << 1)    /**< HP filter cutoff bit 1 */
#define L3GD20_CTRL_REG2_HPCF2      (1 << 2)    /**< HP filter cutoff bit 2 */
#define L3GD20_CTRL_REG2_HPCF3      (1 << 3)    /**< HP filter cutoff bit 3 */
#define L3GD20_CTRL_REG2_HPM0       (1 << 4)    /**< HP filter mode bit 0   */
#define L3GD20_CTRL_REG2_HPM1       (1 << 5)    /**< HP filter mode bit 1   */
/** @} */

/**
 * @name    L3GD20_CTRL_REG3 register bits definitions
 * @{
 */                                               
#define L3GD20_CTRL_REG3_MASK       0xFF        /**< L3GD20_CTRL_REG3 mask  */
#define L3GD20_CTRL_REG3_I2_EMPTY   (1 << 0)    /**< FIFO empty IRQ         */
#define L3GD20_CTRL_REG3_I2_ORUN    (1 << 1)    /**< FIFO overrun IRQ       */
#define L3GD20_CTRL_REG3_I2_WTM     (1 << 2)    /**< FIFO watermark IRQ     */
#define L3GD20_CTRL_REG3_I2_DRDY    (1 << 3)    /**< Data ready             */
#define L3GD20_CTRL_REG3_PP_OD      (1 << 4)    /**< Push-pull / Open Drain */
#define L3GD20_CTRL_REG3_H_LACTIVE  (1 << 5)    /**< IRQ active             */
#define L3GD20_CTRL_REG3_I1_BOOT    (1 << 6)    /**< Boot status available  */
#define L3GD20_CTRL_REG3_I1_INT1    (1 << 7)    /**< IRQ enable             */
/** @} */

/**
 * @name    L3GD20_CTRL_REG4 register bits definitions
 * @{
 */
#define L3GD20_CTRL_REG4_MASK       0xF1        /**< L3GD20_CTRL_REG4 mask  */
#define L3GD20_CTRL_REG4_SIM        (1 << 0)    /**< SPI mode               */
#define L3GD20_CTRL_REG4_FS_MASK    0x30        /**< Full scale field mask  */
#define L3GD20_CTRL_REG4_FS0        (1 << 4)    /**< Full scale bit 0       */
#define L3GD20_CTRL_REG4_FS1        (1 << 5)    /**< Full scale bit 1       */
#define L3GD20_CTRL_REG4_BLE        (1 << 6)    /**< Big/little endian data */
#define L3GD20_CTRL_REG4_BDU        (1 << 7)    /**< Block data update      */
/** @} */

/**
 * @name    L3GD20_CTRL_REG5 register bits definitions
 * @{
 */                                               
#define L3GD20_CTRL_REG5_MASK       0xDF        /**< L3GD20_CTRL_REG5 mask  */
#define L3GD20_CTRL_REG5_OUT_SEL0   (1 << 0)    /**< Out selection  bit 0   */
#define L3GD20_CTRL_REG5_OUT_SEL1   (1 << 1)    /**< Out selection  bit 1   */
#define L3GD20_CTRL_REG5_INT1_SEL0  (1 << 2)    /**< INT1 selection  bit 0  */
#define L3GD20_CTRL_REG5_INT1_SEL1  (1 << 3)    /**< INT1 selection  bit 1  */
#define L3GD20_CTRL_REG5_HPEN       (1 << 4)    /**< HP filter enable       */
#define L3GD20_CTRL_REG5_FIFO_EN    (1 << 6)    /**< FIFO enable            */
#define L3GD20_CTRL_REG5_BOOT       (1 << 7)    /**< Reboot memory content  */
/** @} */

/**
 * @name    L3GD20_INT1_CFG register bits definitions
 * @{
 */
#define L3GD20_INT1_CFG_MASK       0xFF        /**< L3GD20_INT1_CFG mask   */
#define L3GD20_INT1_CFG_XLIE       (1 << 0)    /**< Enable INT1 on X low   */
#define L3GD20_INT1_CFG_XHIE       (1 << 1)    /**< Enable INT1 on X high  */
#define L3GD20_INT1_CFG_YLIE       (1 << 2)    /**< Enable INT1 on Y low   */
#define L3GD20_INT1_CFG_YHIE       (1 << 3)    /**< Enable INT1 on Y high  */
#define L3GD20_INT1_CFG_ZLIE       (1 << 4)    /**< Enable INT1 on Z low   */
#define L3GD20_INT1_CFG_ZHIE       (1 << 5)    /**< Enable INT1 on Z high  */
#define L3GD20_INT1_CFG_LIR        (1 << 6)    /**< Latch INT1             */
#define L3GD20_INT1_CFG_AND_OR     (1 << 7)    /**< AND OR combination     */
/** @} */

/**
 * @name    L3GD20_INT1_SRC register bits definitions
 * @{
 */
#define L3GD20_INT1_SRC_MASK       0x7F        /**< L3GD20_INT1_SRC mask   */
#define L3GD20_INT1_SRC_XL         (1 << 0)    /**< X low event            */
#define L3GD20_INT1_SRC_XH         (1 << 1)    /**< X high event           */
#define L3GD20_INT1_SRC_YL         (1 << 2)    /**< Y low event            */
#define L3GD20_INT1_SRC_YH         (1 << 3)    /**< Y high event           */
#define L3GD20_INT1_SRC_ZL         (1 << 4)    /**< Z low event            */
#define L3GD20_INT1_SRC_ZH         (1 << 5)    /**< Z high event           */
#define L3GD20_INT1_SRC_IA         (1 << 6)    /**< Interrupt active       */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   L3GD20 SPI interface switch.
 * @details If set to @p TRUE the support for SPI is included.
 * @note    The default is @p TRUE.
 */
#if !defined(L3GD20_USE_SPI) || defined(__DOXYGEN__)
#define L3GD20_USE_SPI                      TRUE
#endif

/**
 * @brief   L3GD20 I2C interface switch.
 * @details If set to @p TRUE the support for I2C is included.
 * @note    The default is @p FALSE.
 */
#if !defined(L3GD20_USE_I2C) || defined(__DOXYGEN__)
#define L3GD20_USE_I2C                      FALSE
#endif

/**
 * @brief   L3GD20 advanced configurations switch.
 * @details If set to @p TRUE more configurations are available.
 * @note    The default is @p FALSE.
 */
#if !defined(L3GD20_USE_ADVANCED) || defined(__DOXYGEN__)
#define L3GD20_USE_ADVANCED                 FALSE
#endif

/**
 * @brief   L3GD20 shared SPI switch.
 * @details If set to @p TRUE the device acquires SPI bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires SPI_USE_MUTUAL_EXCLUSION
 */
#if !defined(L3GD20_SHARED_SPI) || defined(__DOXYGEN__)
#define L3GD20_SHARED_SPI                   FALSE
#endif

/**
 * @brief   Number of acquisitions for bias removal
 * @details This is the number of acquisitions performed to compute the
 *          bias. A repetition is required in order to remove noise.
 */
#if !defined(L3GD20_BIAS_ACQ_TIMES) || defined(__DOXYGEN__)
#define L3GD20_BIAS_ACQ_TIMES               50
#endif

/**
 * @brief   Settling time for bias removal
 * @details This is the time between each bias acquisition.
 */
#if !defined(L3GD20_BIAS_SETTLING_uS) || defined(__DOXYGEN__)
#define L3GD20_BIAS_SETTLING_uS             5000
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !(L3GD20_USE_SPI ^ L3GD20_USE_I2C)
#error "L3GD20_USE_SPI and L3GD20_USE_I2C cannot be both true or both false"
#endif

#if L3GD20_USE_SPI && !HAL_USE_SPI
#error "L3GD20_USE_SPI requires HAL_USE_SPI"
#endif

//TODO: add I2C support.
#if L3GD20_USE_I2C
#error "L3GD20 over I2C still not supported"
#endif

#if L3GD20_USE_I2C && !HAL_USE_I2C
#error "L3GD20_USE_I2C requires HAL_USE_I2C"
#endif

#if L3GD20_SHARED_SPI && !SPI_USE_MUTUAL_EXCLUSION
#error "L3GD20_SHARED_SPI requires SPI_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @name    L3GD20 data structures and types.
 * @{
 */
/**
 * @brief   L3GD20 full scale.
 */
typedef enum {
  L3GD20_FS_250DPS = 0x00,          /**< Full scale 250 degree per second.  */
  L3GD20_FS_500DPS = 0x10,          /**< Full scale 500 degree per second.  */
  L3GD20_FS_2000DPS = 0x20          /**< Full scale 2000 degree per second. */
}l3gd20_fs_t;

/**
 * @brief   L3GD20 output data rate and bandwidth.
 */
typedef enum {
  L3GD20_ODR_95HZ = 0x00,           /**< Output data rate 95 Hz.            */
  L3GD20_ODR_190HZ = 0x40,          /**< Output data rate 190 Hz.           */
  L3GD20_ODR_380HZ = 0x80,          /**< Output data rate 380 Hz.           */
  L3GD20_ODR_760HZ = 0xC0           /**< Output data rate 760 Hz.           */
}l3gd20_odr_t;

/**
 * @brief   L3GD20 low pass filter 1 bandwidth.
 */
typedef enum {
  L3GD20_BW0 = 0x00,                /**< LPF1 bandwidth. Depends on ODR.    */
  L3GD20_BW1 = 0x40,                /**< LPF1 bandwidth. Depends on ODR.    */
  L3GD20_BW2 = 0x80,                /**< LPF1 bandwidth. Depends on ODR.    */
  L3GD20_BW3 = 0xC0                 /**< LPF1 bandwidth. Depends on ODR.    */
}l3gd20_bw_t;
/**
 * @brief   L3GD20 block data update.
 */
typedef enum {
  L3GD20_BDU_CONTINUOUS = 0x00,     /**< Block data continuously updated.   */
  L3GD20_BDU_BLOCKED = 0x80         /**< Block data updated after reading.  */
}l3gd20_bdu_t;

/**
 * @brief   L3GD20 HP filter mode.
 */
typedef enum {
  L3GD20_HPM_NORMAL = 0x00,         /**< Normal mode.                       */
  L3GD20_HPM_REFERENCE = 0x10,      /**< Reference signal for filtering.    */
  L3GD20_HPM_AUTORESET = 0x30,      /**< Autoreset on interrupt event.      */  
  L3GD20_HPM_BYPASSED = 0xFF        /**< HP filter bypassed                 */
}l3gd20_hpm_t;

/**
 * @brief   L3GD20 HP configuration.
 */
typedef enum {
  L3GD20_HPCF_0 = 0x00,             /**< Depends on ODR (Table 26 for more).*/
  L3GD20_HPCF_1 = 0x01,             /**< Depends on ODR (Table 26 for more).*/
  L3GD20_HPCF_2 = 0x02,             /**< Depends on ODR (Table 26 for more).*/
  L3GD20_HPCF_3 = 0x03,             /**< Depends on ODR (Table 26 for more).*/
  L3GD20_HPCF_4 = 0x04,             /**< Depends on ODR (Table 26 for more).*/
  L3GD20_HPCF_5 = 0x05,             /**< Depends on ODR (Table 26 for more).*/
  L3GD20_HPCF_6 = 0x06,             /**< Depends on ODR (Table 26 for more).*/
  L3GD20_HPCF_7 = 0x07,             /**< Depends on ODR (Table 26 for more).*/
  L3GD20_HPCF_8 = 0x08,             /**< Depends on ODR (Table 26 for more).*/
  L3GD20_HPCF_9 = 0x09              /**< Depends on ODR (Table 26 for more).*/
}l3gd20_hpcf_t;

/**
 * @brief   L3GD20 LP2 filter mode.
 * @details To activate LP2 HP should be active
 */
typedef enum {
  L3GD20_LP2M_ON = 0x00,            /**< LP2 filter activated.              */
  L3GD20_LP2M_BYPASSED = 0xFF,      /**< LP2 filter bypassed.               */ 
}l3gd20_lp2m_t;

/**
 * @brief   L3GD20 endianness.
 */
typedef enum {
  L3GD20_END_LITTLE = 0x00,         /**< Little endian.                     */
  L3GD20_END_BIG = 0x40             /**< Big endian.                        */
}l3gd20_end_t;

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  L3GD20_UNINIT = 0,                /**< Not initialized.                   */
  L3GD20_STOP = 1,                  /**< Stopped.                           */
  L3GD20_READY = 2,                 /**< Ready.                             */
} l3gd20_state_t;

/**
 * @brief   L3GD20 configuration structure.
 */
typedef struct {

#if L3GD20_USE_SPI || defined(__DOXYGEN__)
  /**
   * @brief SPI driver associated to this L3GD20.
   */
  SPIDriver                 *spip;
  /**
   * @brief SPI configuration associated to this L3GD20.
   */
  const SPIConfig           *spicfg;
#endif /* L3GD20_USE_SPI */
#if L3GD20_USE_I2C || defined(__DOXYGEN__)
  /**
   * @brief I2C driver associated to this L3GD20.
   */
  I2CDriver                 *i2cp;
  /**
   * @brief I2C configuration associated to this L3GD20.
   */
  const I2CConfig           *i2ccfg;
#endif /* L3GD20_USE_I2C */
  /**
   * @brief L3GD20 initial sensitivity.
   */
  float                     *sensitivity;
  /**
   * @brief L3GD20 initial bias.
   */
  float                     *bias;
  /** 
   * @brief L3GD20 initial full scale value.
   */
  l3gd20_fs_t               fullscale;
  /**
   * @brief L3GD20 output data rate selection.
   */
  l3gd20_odr_t              outputdatarate;
#if L3GD20_USE_ADVANCED || defined(__DOXYGEN__)
  /**
   * @brief L3GD20 block data update.
   */
  l3gd20_bdu_t              blockdataupdate;
  /**
   * @brief L3GD20 endianness.
   */
  l3gd20_end_t              endianness;
  /**
   * @brief   L3GD20 LP1 filter bandwidth.
   */
  l3gd20_bw_t               bandwidth;
  /**
   * @brief   L3GD20 HP filter mode.
   */
  l3gd20_hpm_t              hpmode;
  /**
   * @brief   L3GD20 HP configuration.
   */
  l3gd20_hpcf_t             hpconfiguration;
  /**
   * @brief   L3GD20 LP2 filter mode.
   * @details To activate LP2 HP should be active
   */
  l3gd20_lp2m_t             lp2mode;
#endif
} L3GD20Config;

/**
 * @brief   Structure representing a L3GD20 driver.
 */
typedef struct L3GD20Driver L3GD20Driver;

/**
 * @brief   @p L3GD20 specific methods.
 */
#define _l3gd20_methods                                                     \
  _base_gyroscope_methods                                                   \
  /* Change full scale value of L3GD20.*/                                   \
  msg_t (*set_full_scale)(void *instance, l3gd20_fs_t fs);

/**
 * @extends BaseGyroscopeVMT
 *
 * @brief   @p L3GD20 virtual methods table.
 */
struct L3GD20VMT {
  _l3gd20_methods
};

/**
 * @brief   @p L3GD20Driver specific data.
 */
#define _l3gd20_data                                                        \
  _base_gyroscope_data                                                      \
  /* Driver state.*/                                                        \
  l3gd20_state_t            state;                                          \
  /* Current configuration data.*/                                          \
  const L3GD20Config        *config;                                        \
  /* Current sensitivity data.*/                                            \
  float                     sensitivity[L3GD20_NUMBER_OF_AXES];             \
  /* Current Bias data.*/                                                   \
  float                     bias[L3GD20_NUMBER_OF_AXES];                    \
  /* Current full scale value.*/                                            \
  float                     fullscale;

/**
 * @extends BaseGyroscope
 *
 * @brief   L3GD20 3-axis gyroscope class.
 * @details This class extends @p BaseGyroscope by adding physical
 *          driver implementation.
 */
struct L3GD20Driver {
  /** @brief BaseSensor Virtual Methods Table. */
  const struct BaseSensorVMT *vmt_basesensor;
  /** @brief BaseGyroscope Virtual Methods Table. */
  const struct BaseGyroscopeVMT *vmt_basegyroscope;
  /** @brief L3GD20 Virtual Methods Table. */
  const struct L3GD20VMT *vmt_l3gd20;
  _l3gd20_data
};
/** @} */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Change gyroscope full scale value.
 *
 * @param[in] ip        pointer to a @p BaseGyroscope class.
 * @param[in] fs        the new full scale value.
 *
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more errors occurred.
 * @api
 */
#define gyroscopeSetFullScale(ip, fs)                                       \
        (ip)->vmt_l3gd20->set_full_scale(ip, fs)
        
/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void l3gd20ObjectInit(L3GD20Driver *devp);
  void l3gd20Start(L3GD20Driver *devp, const L3GD20Config *config);
  void l3gd20Stop(L3GD20Driver *devp);
#ifdef __cplusplus
}
#endif

#endif /* _L3GD20_H_ */

/** @} */

