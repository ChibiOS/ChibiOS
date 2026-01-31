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
 * @file    templates/hal_eth_lld.c
 * @brief   XXXX low level ETH driver code.
 *
 * @addtogroup HAL_ETH
 * @{
 */

#include <string.h>

#include "hal.h"

#if HAL_USE_ETH || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Ethernet driver 1.
 */
hal_eth_driver_c ETHD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       ETH Low Level Driver initialization.
 *
 * @init
 */
void eth_lld_init(void) {

}

/**
 * @brief   Activates the ETH peripheral using the default configuration.
 *
 * @param[in,out] ethp          Pointer to a @p hal_eth_driver_c instance.
 *
 * @notapi
 */
msg_t eth_lld_start(hal_eth_driver_c *ethp) {

  (void)ethp;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Stops the ETH peripheral.
 *
 * @param[in,out] ethp          Pointer to a @p hal_eth_driver_c instance.
 *
 * @notapi
 */
void eth_lld_stop(hal_eth_driver_c *ethp) {

  (void)ethp;
}

/**
 * @brief   ETH Low Level Driver direct configuration.
 *
 * @param[in,out] ethp          Pointer to a @p hal_eth_driver_c instance.
 * @param[in] config            Pointer to the @p hal_eth_config_t structure.
 * @return                      A pointer to the current configuration
 *                              structure.
 * @retval NULL                 If the configuration failed.
 *
 * @notapi
 */
const hal_eth_config_t *eth_lld_setcfg(hal_eth_driver_c *ethp,
                                       const hal_eth_config_t *config) {

  (void)ethp;
  (void)config;

  return NULL;
}

/**
 * @brief   ETH Low Level Driver configuration selection.
 *
 * @param[in,out] ethp          Pointer to a @p hal_eth_driver_c instance.
 * @param[in] config            Pointer to the @p hal_eth_config_t structure.
 * @return                      A pointer to the current configuration
 *                              structure.
 * @retval NULL                 If the configuration failed.
 *
 * @notapi
 */
const hal_eth_config_t *eth_lld_selcfg(hal_eth_driver_c *ethp,
                                       unsigned cfgnum) {

  (void)ethp;
  (void)cfgnum;

  return NULL;
}

/**
 * @brief       Queries for a received frame handle.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The receive handle.
 * @retval NULL                 If a received frame is not available.
 *
 * @notapi
 */
etc_receive_handle_t eth_lld_get_receive_handle(hal_eth_driver_c *ethp) {

  (void)ethp;

  return NULL;
}

/**
 * @brief       Queries for a transmit frame handle.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The transmit handle.
 * @retval NULL                 If an empty transmit frame is not available.
 *
 * @notapi
 */
etc_transmit_handle_t eth_lld_get_transmit_handle(hal_eth_driver_c *ethp) {

  (void)ethp;

  return NULL;
}

/**
 * @brief       Releases a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 *
 * @notapi
 */
void eth_lld_release_receive_handle(hal_eth_driver_c *ethp,
                                    etc_receive_handle_t rxh) {

  (void)ethp;
  (void)rxh;
}

/**
 * @brief       Releases and transmits a frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmi] handle.
 *
 * @notapi
 */
void eth_lld_release_transmit_handle(hal_eth_driver_c *ethp,
                                     etc_transmit_handle_t txh) {

  (void)ethp;
  (void)txh;
}

/**
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    bp            Received data buffer pointer.
 * @param[in]     n             Number of bytes to read.
 * @return                      The number of bytes read from the handle
 *                              buffer, this value can be less than the amount
 *                              specified in the parameter @p size if there are
 *                              no more bytes to read.
 *
 * @notapi
 */
size_t eth_lld_read_receive_handle(hal_eth_driver_c *ethp,
                                   etc_receive_handle_t rxh,
                                   uint8_t *bp, size_t n) {

  (void)ethp;
  (void)rxh;
  (void)bp;
  (void)n;

  return 0;
}

/**
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmit handle.
 * @param[in]     bp            Transmit data buffer pointer.
 * @param[in]     n             Number of bytes to write.
 * @return                      The number of bytes written into the handle
 *                              buffer this value can be less than the amount
 *                              specified in the parameter @p size if the
 *                              maximum frame size is reached.
 *
 * @notapi
 */
size_t eth_lld_write_transmit_handle(hal_eth_driver_c *ethp,
                                     etc_transmit_handle_t txh,
                                     const uint8_t *bp, size_t n) {

  (void)ethp;
  (void)txh;
  (void)bp;
  (void)n;

  return 0;
}

/**
 * @brief       Direct access to the receive handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    sizep         Size of the received frame.
 * @return                      Pointer to the received frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 *
 * @notapi
 */
const uint8_t *eth_lld_get_receive_buffer(hal_eth_driver_c *ethp,
                                          etc_receive_handle_t rxh,
                                          size_t *sizep) {

  (void)ethp;
  (void)rxh;
  (void)sizep;

  return 0;
}

/**
 * @brief       Direct access to the transmit handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmit handle.
 * @param[out]    sizep         Maximum size of the transmit buffer.
 * @return                      Pointer to the transmit frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 *
 * @notapi
 */
uint8_t *eth_lld_get_transmit_buffer(hal_eth_driver_c *ethp,
                                     etc_transmit_handle_t txh,
                                     size_t *sizep) {

  (void)ethp;
  (void)txh;
  (void)sizep;

  return NULL;
}

/**
 * @brief       Returns the link status.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The link status,
 * @retval false                If the link is active.
 * @retval true                 If the link is down.
 *
 * @notapi
 */
bool eth_lld_poll_link_status(hal_eth_driver_c *ethp) {

  (void)ethp;

  return true;
}

#endif /* HAL_USE_ETH */

/** @} */
