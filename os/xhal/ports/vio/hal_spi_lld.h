/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    sandbox/hal_spi_lld.h
 * @brief   Sandbox SPI subsystem low level driver header.
 *
 * @addtogroup SPI
 * @{
 */

#ifndef HAL_SPI_LLD_H
#define HAL_SPI_LLD_H

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Circular mode support flag.
 */
#define SPI_SUPPORTS_CIRCULAR               TRUE

/**
 * @brief   Slave mode support flag.
 */
#define SPI_SUPPORTS_SLAVE_MODE             TRUE

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SPI1 driver enable switch.
 * @details If set to @p TRUE the support for SPI1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(VIO_SPI_USE_VSPI1) || defined(__DOXYGEN__)
#define VIO_SPI_USE_VSPI1                   FALSE
#endif

/**
 * @brief   SPI2 driver enable switch.
 * @details If set to @p TRUE the support for SPI2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(VIO_SPI_USE_VSPI2) || defined(__DOXYGEN__)
#define VIO_SPI_USE_VSPI2                   FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !VIO_SPI_USE_VSPI1 && !VIO_SPI_USE_VSPI2
#error "SIO driver activated but no VSPI peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the SPI driver structure.
 */
#define spi_lld_driver_fields                                               \
  /* Number of the associated VSPI.*/                                       \
  uint32_t                  nvspi;                                          \
  /* Buffer for the local copy of the driver configuration.*/               \
  hal_spi_config_t          cfgbuf

/**
 * @brief   Low level fields of the SPI configuration structure.
 */
#define spi_lld_config_fields

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if VIO_SPI_USE_VSPI1 && !defined(__DOXYGEN__)
extern hal_spi_driver_c SPID1;
#endif

#if VIO_SPI_USE_VSPI2 && !defined(__DOXYGEN__)
extern hal_spi_driver_c SPID2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void spi_lld_init(void);
  msg_t spi_lld_start(hal_spi_driver_c *spip);
  void spi_lld_stop(hal_spi_driver_c *spip);
  const hal_spi_config_t *spi_lld_setcfg(hal_spi_driver_c *spip,
                                         const hal_spi_config_t *config);
  const hal_spi_config_t *spi_lld_selcfg(hal_spi_driver_c *spip,
                                         unsigned cfgnum);
  void spi_lld_select(hal_spi_driver_c *spip);
  void spi_lld_unselect(hal_spi_driver_c *spip);
  msg_t spi_lld_ignore(hal_spi_driver_c *spip, size_t n);
  msg_t spi_lld_exchange(hal_spi_driver_c *spip, size_t n,
                         const void *txbuf, void *rxbuf);
  msg_t spi_lld_send(hal_spi_driver_c *spip, size_t n, const void *txbuf);
  msg_t spi_lld_receive(hal_spi_driver_c *spip, size_t n, void *rxbuf);
  msg_t spi_lld_stop_transfer(hal_spi_driver_c *spip, size_t *sizep);
  uint16_t spi_lld_polled_exchange(hal_spi_driver_c *spip, uint16_t frame);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SPI */

#endif /* HAL_SPI_LLD_H */

/** @} */
