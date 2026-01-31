/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    templates/hal_eth_lld.h
 * @brief   XXXX low level ETH driver header.
 *
 * @addtogroup HAL_ETH
 * @{
 */

#ifndef HAL_ETH_LLD_H
#define HAL_ETH_LLD_H

#if HAL_USE_ETH || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/* Supports zero-copy API.*/
#define ETH_SUPPORTS_ZERO_COPY              TRUE

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the MAC driver structure.
 */
#define eth_lld_driver_fields

/**
 * @brief   Low level fields of the MAC configuration structure.
 */
#define eth_lld_config_fields

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern hal_eth_driver_c ETHD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void eth_lld_init(void);
  msg_t eth_lld_start(hal_eth_driver_c *ethp);
  void eth_lld_stop(hal_eth_driver_c *ethp);
  const hal_eth_config_t *eth_lld_setcfg(hal_eth_driver_c *ethp,
                                         const hal_eth_config_t *config);
  const hal_eth_config_t *eth_lld_selcfg(hal_eth_driver_c *ethp,
                                         unsigned cfgnum);
  etc_receive_handle_t eth_lld_get_receive_handle(hal_eth_driver_c *ethp);
  etc_transmit_handle_t eth_lld_get_transmit_handle(hal_eth_driver_c *ethp);
  void eth_lld_release_receive_handle(hal_eth_driver_c *ethp,
                                      etc_receive_handle_t rxh);
  void eth_lld_release_transmit_handle(hal_eth_driver_c *ethp,
                                       etc_transmit_handle_t txh);
  size_t eth_lld_read_receive_handle(hal_eth_driver_c *ethp,
                                     etc_receive_handle_t rxh,
                                     uint8_t *bp, size_t n);
  size_t eth_lld_write_transmit_handle(hal_eth_driver_c *ethp,
                                       etc_transmit_handle_t txh,
                                       const uint8_t *bp, size_t n);
  bool eth_lld_poll_link_status(hal_eth_driver_c *ethp);
#if (ETH_SUPPORTS_ZERO_COPY == TRUE) || defined(__DOXYGEN__)
  const uint8_t *eth_lld_get_receive_buffer(hal_eth_driver_c *ethp,
                                            etc_receive_handle_t rxh,
                                            size_t *np);
  uint8_t *eth_lld_get_transmit_buffer(hal_eth_driver_c *ethp,
                                       etc_transmit_handle_t txh,
                                       size_t *sizep);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ETH */

#endif /* HAL_ETH_LLD_H */

/** @} */
