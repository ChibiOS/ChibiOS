/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    sbvio.h
 * @brief   ARM SandBox host Virtual I/O macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO
 * @{
 */

#ifndef SBVIO_H
#define SBVIO_H

#if (SB_CFG_ENABLE_VIO == TRUE) || defined(__DOXYGEN__)

#include "vioconf.h"
#include "sbvio_adc.h"
#include "sbvio_eth.h"
#include "sbvio_gpt.h"
#include "sbvio_gpio.h"
#include "sbvio_i2c.h"
#include "sbvio_spi.h"
#include "sbvio_uart.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on configuration options.*/
#if !defined(VIO_CFG_ENABLE_ADC) || defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_ADC not defined in vioconf.h"
#endif

#if !defined(VIO_CFG_ENABLE_GPIO) || defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_GPIO not defined in vioconf.h"
#endif

#if !defined(VIO_CFG_ENABLE_ETH) || defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_ETH not defined in vioconf.h"
#endif

#if !defined(VIO_CFG_ENABLE_GPT) || defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_GPT not defined in vioconf.h"
#endif

#if !defined(VIO_CFG_ENABLE_I2C) || defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_I2C not defined in vioconf.h"
#endif

#if !defined(VIO_CFG_ENABLE_SPI) || defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_SPI not defined in vioconf.h"
#endif

#if !defined(VIO_CFG_ENABLE_UART) || defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_UART not defined in vioconf.h"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a VIO instance configuration structure.
 */
typedef struct vio_conf {
#if (VIO_CFG_ENABLE_ADC == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Virtual ADC units.
   */
  const vio_adc_units_t         *adcs;
  /**
   * @brief   Virtual ADC configurations.
   */
  const adc_configurations_t    *adcconfs;
#endif
#if (VIO_CFG_ENABLE_ETH == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Virtual ETH units.
   */
  const vio_eth_units_t         *eths;
  /**
   * @brief   Virtual ETH configurations.
   */
  const eth_configurations_t    *ethconfs;
#endif
#if (VIO_CFG_ENABLE_GPT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Virtual GPT units.
   */
  const vio_gpt_units_t         *gpts;
  /**
   * @brief   Virtual GPT configurations.
   */
  const gpt_configurations_t    *gptconfs;
#endif
#if (VIO_CFG_ENABLE_GPIO == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Virtual GPIO units.
   */
  const vio_gpio_units_t        *gpios;
#endif
#if (VIO_CFG_ENABLE_I2C == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Virtual I2C units.
   */
  const vio_i2c_units_t         *i2cs;
  /**
   * @brief   Virtual I2C configurations.
   */
  const i2c_configurations_t    *i2cconfs;
#endif
#if (VIO_CFG_ENABLE_UART == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Virtual UART units.
   */
  const vio_uart_units_t        *uarts;
  /**
   * @brief   Virtual UART configurations.
   */
  const sio_configurations_t    *uartconfs;
#endif
#if (VIO_CFG_ENABLE_SPI == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Virtual SPI units.
   */
  const vio_spi_units_t         *spis;
  /**
   * @brief   Virtual SPI configurations.
   */
  const spi_configurations_t    *spiconfs;
#endif
} vio_conf_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Sub-code part of VIO calls 1st parameter.
 *
 * @param[n] n          the VIO 1st parameter
 * @return              The VIO call sub-code.
 */
#define VIO_CALL_SUBCODE(n)     ((uint32_t)(n) & 0xFFU)

/**
 * @brief   Unit identifier part of VIO calls 1st parameter.
 *
 * @param[n] n          the VIO 1st parameter
 * @return              The VIO call unit identifier.
 */
#define VIO_CALL_UNIT(n)        ((uint32_t)(n) >> 24)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void __sb_vio_cleanup(sb_class_t *sbp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SB_CFG_ENABLE_VIO == TRUE */

#endif /* SBVIO_H */

/** @} */
