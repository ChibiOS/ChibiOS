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
 * @{
 */

#ifndef _L3GD20_H_
#define _L3GD20_H_

#include "hal_gyroscope.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   L3GD20 number of axes.
 */
#define  L3GD20_NUMBER_OF_AXES                   ((size_t)   3U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   L3GD20 SPI interface selector.
 * @details If set to @p TRUE the support for SPI is included.
 * @note    The default is @p TRUE.
 */
#if !defined(L3GD20_USE_SPI) || defined(__DOXYGEN__)
#define L3GD20_USE_SPI              TRUE
#endif

/**
 * @brief   L3GD20 I2C interface selector.
 * @details If set to @p TRUE the support for I2C is included.
 * @note    The default is @p FALSE.
 */
#if !defined(L3GD20_USE_I2C) || defined(__DOXYGEN__)
#define L3GD20_USE_I2C              FALSE
#endif

/**
 * @brief   Number of acquisitions for bias removal
 * @details This is the number of acquisitions performed to compute the
 *          bias. A repetition is required in order to remove noise.
 */
#if !defined(L3GD20_BIAS_ACQ_TIMES) || defined(__DOXYGEN__)
#define L3GD20_BIAS_ACQ_TIMES       50
#endif

/**
 * @brief   Settling time for bias removal
 * @details This is the time between each bias acquisition.
 */
#if !defined(L3GD20_BIAS_SETTLING_uS) || defined(__DOXYGEN__)
#define L3GD20_BIAS_SETTLING_uS     5000
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if L3GD20_USE_SPI && L3GD20_USE_I2C
#error "L3GD20_USE_SPI and L3GD20_USE_I2C cannot be both true"
#endif

#if L3GD20_USE_SPI && !HAL_USE_SPI
#error "L3GD20_USE_SPI requires HAL_USE_SPI"
#endif

#if L3GD20_USE_I2C && !HAL_USE_I2C
#error "L3GD20_USE_SPI requires HAL_USE_I2C"
#endif
/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @name    L3GD20 data structures and types
 * @{
 */

/**
 * @brief  L3GD20 Output Data Rate
 */
typedef enum {
  L3GD20_ODR_95HZ_FC_12_5  = 0x00,          
  L3GD20_ODR_95HZ_FC_25    = 0x10,          
  L3GD20_ODR_190HZ_FC_12_5 = 0x40,          
  L3GD20_ODR_190HZ_FC_25   = 0x50,          
  L3GD20_ODR_190HZ_FC_50   = 0x60,          
  L3GD20_ODR_190HZ_FC_70   = 0x70,          
  L3GD20_ODR_380HZ_FC_20   = 0x80,          
  L3GD20_ODR_380HZ_FC_25   = 0x90,          
  L3GD20_ODR_380HZ_FC_50   = 0xA0,          
  L3GD20_ODR_380HZ_FC_100  = 0xB0,          
  L3GD20_ODR_760HZ_FC_30   = 0xC0,          
  L3GD20_ODR_760HZ_FC_35   = 0xD0,          
  L3GD20_ODR_760HZ_FC_50   = 0xE0,          
  L3GD20_ODR_760HZ_FC_100  = 0xF0           
}l3gd20_odr_t;

/**
 * @brief  L3GD20 Full Scale
 */
typedef enum {
  L3GD20_FS_250DPS  = 0x00,                 
  L3GD20_FS_500DPS  = 0x10,                 
  L3GD20_FS_2000DPS = 0x20                  
}l3gd20_fs_t;

/**
 * @brief   L3GD20 Axes Enabling
 */
typedef enum {
  L3GD20_AE_DISABLED = 0x00,                
  L3GD20_AE_X        = 0x01,                
  L3GD20_AE_Y        = 0x02,                
  L3GD20_AE_XY       = 0x03,                
  L3GD20_AE_Z        = 0x04,                
  L3GD20_AE_XZ       = 0x05,                
  L3GD20_AE_YZ       = 0x06,                
  L3GD20_AE_XYZ      = 0x07                 
}l3gd20_ae_t;

/**
 * @brief  L3GD20 Block Data Update
 */
typedef enum {
  L3GD20_BDU_CONTINOUS = 0x00,              
  L3GD20_BDU_BLOCKED   = 0x80               
}l3gd20_bdu_t;

/**
 * @brief  L3GD20 Endianness
 */
typedef enum {
  L3GD20_END_LITTLE = 0x00,
  L3GD20_END_BIG    = 0x40
}l3gd20_end_t;

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  L3GD20_UNINIT = 0,                 /**< Not initialized. */
  L3GD20_STOP = 1,                   /**< Stopped.         */
  L3GD20_READY = 2,                  /**< Ready.           */
} l3gd20_state_t;

/**
 * @brief   L3GD20 configuration structure.
 */
typedef struct {

#if (L3GD20_USE_SPI) || defined(__DOXYGEN__)
  /**
   * @brief SPI driver associated to this L3GD20.
   */
  SPIDriver      *spip;
  /**
   * @brief SPI configuration associated to this L3GD20.
   */
  const SPIConfig      *spicfg;
#endif /* L3GD20_USE_SPI */
#if (L3GD20_USE_I2C) || defined(__DOXYGEN__)
  /**
   * @brief I2C driver associated to this L3GD20.
   */
  I2CDriver      *i2cp;
  /**
   * @brief I2C configuration associated to this L3GD20.
   */
  const I2CConfig      *i2ccfg;
#endif /* L3GD20_USE_I2C */
  /**
   * @brief L3GD20 full scale value.
   */
  l3gd20_fs_t    fullscale;
  /**
   * @brief L3GD20 output data rate selection.
   */
  l3gd20_odr_t   outputdatarate;
  /**
   * @brief L3GD20 axes enabling.
   */
  l3gd20_ae_t    axesenabling;
  /**
   * @brief L3GD20 endianness.
   */
  l3gd20_end_t   endianness;
  /**
   * @brief L3GD20 block data update.
   */
  l3gd20_bdu_t   blockdataupdate;
} L3GD20Config;

/**
 * @brief   Structure representing a L3GD20 driver.
 */
typedef struct L3GD20Driver L3GD20Driver;

/**
 * @brief   @p L3GD20 specific methods.
 */
#define _l3gd20_methods                                                     \
  _base_gyroscope_methods

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
    _base_gyroscope_data                                                    \
    /* Driver state.*/                                                      \
    l3gd20_state_t             state;                                       \
    /* Current configuration data.*/                                        \
    const L3GD20Config         *config;                                     \
    /* Current sensitivity.*/                                               \
    float                      sensitivity;                                 \
	/* Bias data.*/                                                       \
	int32_t                    bias[L3GD20_NUMBER_OF_AXES];

/**
 * @extends BaseGyroscope
 *
 * @brief   L3GD20 3-axis gyroscope class.
 * @details This class extends @p BaseGyroscope by adding physical
 *          driver implementation.
 */
struct L3GD20Driver {
  /** @brief Virtual Methods Table.*/
  const struct L3GD20VMT *vmt;
  _l3gd20_data
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
  void l3gd20ObjectInit(L3GD20Driver *devp);
  void l3gd20Start(L3GD20Driver *devp, const L3GD20Config *config);
  void l3gd20Stop(L3GD20Driver *devp);
#ifdef __cplusplus
}
#endif

#endif /* _L3GD20_H_ */

/** @} */

