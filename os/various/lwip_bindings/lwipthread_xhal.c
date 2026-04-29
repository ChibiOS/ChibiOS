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

#include <string.h>

#include "hal.h"
#include "lwipthread.h"

#include <lwip/prot/ethernet.h>
#include <netif/etharp.h>

#if defined(__CHIBIOS_XHAL_CONF__)

msg_t lwip_lld_start(const lwipthread_opts_t *opts, struct netif *netif) {
  const hal_eth_config_t *cfgp;
  const hal_eth_config_t *config = NULL;
  msg_t msg;
  unsigned cfgnum = 0U;

  if (opts != NULL) {
    cfgnum = opts->eth_cfgnum;
  }

  if ((opts != NULL) && (opts->eth_config != NULL)) {
    config = opts->eth_config;
  }
  else if (cfgnum != 0U) {
#if ETH_USE_CONFIGURATIONS == TRUE
    extern const eth_configurations_t eth_configurations;

    if (cfgnum >= eth_configurations.cfgsnum) {
      return HAL_RET_CONFIG_ERROR;
    }
    config = &eth_configurations.cfgs[cfgnum];
#else
    return HAL_RET_CONFIG_ERROR;
#endif
  }

  msg = drvStart(&ETHD1, config);
  if (msg != HAL_RET_SUCCESS) {
    return msg;
  }

  cfgp = (const hal_eth_config_t *)ETHD1.config;
  if (cfgp->mac_address != NULL) {
    memcpy(netif->hwaddr, cfgp->mac_address, ETHARP_HWADDR_LEN);
  }

  return HAL_RET_SUCCESS;
}

msg_t lwip_wait_transmit_handle(lwip_transmit_handle_t *txhp) {
  *txhp = ethWaitTransmitHandle(&ETHD1, TIME_MS2I(LWIP_SEND_TIMEOUT));

  return (*txhp != (lwip_transmit_handle_t)0U) ? MSG_OK : MSG_TIMEOUT;
}

size_t lwip_write_transmit_handle(lwip_transmit_handle_t *txhp,
                                  const uint8_t *bp,
                                  size_t size) {

  return ethWriteTransmitHandle(&ETHD1, *txhp, bp, size);
}

void lwip_release_transmit_handle(lwip_transmit_handle_t *txhp) {

  ethReleaseTransmitHandle(&ETHD1, *txhp);
}

msg_t lwip_wait_receive_handle(lwip_receive_handle_t *rxhp) {
  *rxhp = ethWaitReceiveHandle(&ETHD1, TIME_IMMEDIATE);

  return (*rxhp != (lwip_receive_handle_t)0U) ? MSG_OK : MSG_TIMEOUT;
}

size_t lwip_read_receive_handle(lwip_receive_handle_t *rxhp,
                                uint8_t *bp,
                                size_t size) {

  return ethReadReceiveHandle(&ETHD1, *rxhp, bp, size);
}

size_t lwip_receive_size(lwip_receive_handle_t *rxhp) {
#if ETH_SUPPORTS_ZERO_COPY == TRUE
  size_t size;

  (void)ethGetReceiveBufferX(&ETHD1, *rxhp, &size);

  return size;
#else
  (void)rxhp;

  return (size_t)(LWIP_NETIF_MTU + SIZEOF_ETH_HDR);
#endif
}

void lwip_release_receive_handle(lwip_receive_handle_t *rxhp) {

  ethReleaseReceiveHandle(&ETHD1, *rxhp);
}

event_source_t *lwip_get_event_source(void) {

  return &ETHD1.es;
}

eventflags_t lwip_get_receive_event_flag(void) {

  return ETH_FLAGS_RX;
}

bool lwip_poll_link_status(void) {

  return ethPollLinkStatus(&ETHD1);
}

#endif /* defined(__CHIBIOS_XHAL_CONF__) */
