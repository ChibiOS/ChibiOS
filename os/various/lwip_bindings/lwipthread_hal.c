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

#include <netif/etharp.h>

#if !defined(__CHIBIOS_XHAL_CONF__)

static const uint8_t lwip_default_mac[] = {
  LWIP_ETHADDR_0,
  LWIP_ETHADDR_1,
  LWIP_ETHADDR_2,
  LWIP_ETHADDR_3,
  LWIP_ETHADDR_4,
  LWIP_ETHADDR_5
};

static const MACConfig lwip_default_mac_config = {
  .mac_address = lwip_default_mac
};

msg_t lwip_lld_start(const lwipthread_opts_t *opts, struct netif *netif) {
  const MACConfig *cfgp = &lwip_default_mac_config;

  if ((opts != NULL) && (opts->eth_config != NULL)) {
    cfgp = opts->eth_config;
  }

  if (cfgp->mac_address != NULL) {
    memcpy(netif->hwaddr, cfgp->mac_address, ETHARP_HWADDR_LEN);
  }

  macStart(&ETHD1, cfgp);

  return HAL_RET_SUCCESS;
}

msg_t lwip_wait_transmit_handle(lwip_transmit_handle_t *txhp) {

  return macWaitTransmitDescriptor(&ETHD1, txhp, TIME_MS2I(LWIP_SEND_TIMEOUT));
}

size_t lwip_write_transmit_handle(lwip_transmit_handle_t *txhp,
                                  const uint8_t *bp,
                                  size_t size) {

  macWriteTransmitDescriptor(txhp, (uint8_t *)bp, size);

  return size;
}

void lwip_release_transmit_handle(lwip_transmit_handle_t *txhp) {

  macReleaseTransmitDescriptorX(txhp);
}

msg_t lwip_wait_receive_handle(lwip_receive_handle_t *rxhp) {

  return macWaitReceiveDescriptor(&ETHD1, rxhp, TIME_IMMEDIATE);
}

size_t lwip_read_receive_handle(lwip_receive_handle_t *rxhp,
                                uint8_t *bp,
                                size_t size) {

  macReadReceiveDescriptor(rxhp, bp, size);

  return size;
}

size_t lwip_receive_size(lwip_receive_handle_t *rxhp) {

  return rxhp->size;
}

void lwip_release_receive_handle(lwip_receive_handle_t *rxhp) {

  macReleaseReceiveDescriptorX(rxhp);
}

event_source_t *lwip_get_event_source(void) {

  return macGetEventSource(&ETHD1);
}

eventflags_t lwip_get_receive_event_flag(void) {

  return MAC_FLAGS_RX;
}

bool lwip_poll_link_status(void) {

  return macPollLinkStatus(&ETHD1);
}

#endif /* !defined(__CHIBIOS_XHAL_CONF__) */
