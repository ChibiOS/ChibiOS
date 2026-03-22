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
/*
 * **** This file incorporates work covered by the following copyright and ****
 * **** permission notice:                                                 ****
 *
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/**
 * @file lwipthread.c
 * @brief LWIP wrapper thread code.
 * @addtogroup LWIP_THREAD
 * @{
 */

#include "hal.h"
#include "evtimer.h"

#include "lwipthread.h"

#include <lwip/opt.h>
#include <lwip/def.h>
#include <lwip/mem.h>
#include <lwip/pbuf.h>
#include <lwip/sys.h>
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include <lwip/tcpip.h>
#include <netif/etharp.h>
#include <lwip/netifapi.h>

#if LWIP_DHCP
#include <lwip/dhcp.h>
#endif

#if LWIP_AUTOIP
#include <lwip/autoip.h>
#endif

#define PERIODIC_TIMER_ID       1
#define FRAME_RECEIVED_ID       2

static net_addr_mode_t addressMode;
static ip4_addr_t ip, gateway, netmask;
static struct netif thisif;

/*
 * Suspension point for initialization procedure.
 */
thread_reference_t lwip_trp = NULL;

/*
 * Stack area for the LWIP-MAC thread.
 */
static THD_WORKING_AREA(wa_lwip_thread, LWIP_THREAD_STACK_SIZE);

/*
 * Initialization.
 */
static void low_level_init(struct netif *netif) {
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* maximum transfer unit */
  netif->mtu = LWIP_NETIF_MTU;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an Ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

  /* Do whatever else is needed to initialize interface. */
}

/*
 * This function does the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p) {
  struct pbuf *q;
  lwip_transmit_handle_t txh;

  (void)netif;
  if (lwip_wait_transmit_handle(&txh) != MSG_OK)
    return ERR_TIMEOUT;

#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE);        /* drop the padding word */
#endif

  /* Iterates through the pbuf chain. */
  for (q = p; q != NULL; q = q->next) {
    size_t n = lwip_write_transmit_handle(&txh, (const uint8_t *)q->payload,
                                          (size_t)q->len);
    if (n != (size_t)q->len) {
      lwip_release_transmit_handle(&txh);
#if ETH_PAD_SIZE
      pbuf_header(p, ETH_PAD_SIZE);      /* reclaim the padding word */
#endif
      LINK_STATS_INC(link.drop);
      MIB2_STATS_NETIF_INC(netif, ifoutdiscards);
      return ERR_BUF;
    }
  }
  lwip_release_transmit_handle(&txh);

  MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
  if (((u8_t*)p->payload)[0] & 1) {
    /* broadcast or multicast packet*/
    MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
  } 
  else {
    /* unicast packet */
    MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
  }
  /* increase ifoutdiscards or ifouterrors on error */

#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE);         /* reclaim the padding word */
#endif

  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

/*
 * Receives a frame.
 * Allocates a pbuf and transfers the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static bool low_level_input(struct netif *netif, struct pbuf **pbuf) {
  lwip_receive_handle_t rxh;
  struct pbuf *q;
  size_t total;
  u16_t len;

  (void)netif;

  osalDbgAssert(pbuf != NULL, "invalid null pointer");

  if (lwip_wait_receive_handle(&rxh) != MSG_OK)
    return false;

  len = (u16_t)lwip_receive_size(&rxh);

#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE;        /* allow room for Ethernet padding */
#endif

  /* We allocate a pbuf chain of pbufs from the pool. */
  *pbuf = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

  if (*pbuf != NULL) {
#if ETH_PAD_SIZE
    pbuf_header(*pbuf, -ETH_PAD_SIZE); /* drop the padding word */
#endif
    total = 0U;

    /* Iterates through the pbuf chain. */
    for (q = *pbuf; q != NULL; q = q->next) {
      size_t n;

      n = lwip_read_receive_handle(&rxh, (uint8_t *)q->payload,
                                   (size_t)q->len);
      total += n;
      if (n != (size_t)q->len) {
        break;
      }
    }
    lwip_release_receive_handle(&rxh);

#if defined(__CHIBIOS_XHAL_CONF__) && (ETH_SUPPORTS_ZERO_COPY != TRUE)
    pbuf_realloc(*pbuf, (u16_t)total);
#endif

    MIB2_STATS_NETIF_ADD(netif, ifinoctets, (*pbuf)->tot_len);

    if (*(uint8_t *)((*pbuf)->payload) & 1) {
      /* broadcast or multicast packet*/
      MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
    } 
    else {
      /* unicast packet*/
      MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
    }

#if ETH_PAD_SIZE
    pbuf_header(*pbuf, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.recv);
  }
  else {
    lwip_release_receive_handle(&rxh);     /* Drop packet. */
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
    MIB2_STATS_NETIF_INC(netif, ifindiscards);
  }
  
  return true;
}

/*
 * Called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netifapi_netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialised
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
static err_t ethernetif_init(struct netif *netif) {
  osalDbgAssert((netif != NULL), "netif != NULL");

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, LWIP_LINK_SPEED);

  netif->state = NULL;
  netif->name[0] = LWIP_IFNAME0;
  netif->name[1] = LWIP_IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

void lwipDefaultLinkUpCB(void *p)
{
  struct netif *ifc = (struct netif*) p;
  (void) ifc;
#if LWIP_AUTOIP
  if (addressMode == NET_ADDRESS_AUTO)
    autoip_start(ifc);
#endif
#if LWIP_DHCP
  if (addressMode == NET_ADDRESS_DHCP)
    dhcp_start(ifc);
#endif
}

void lwipDefaultLinkDownCB(void *p)
{
  struct netif *ifc = (struct netif*) p;
  (void) ifc;
#if LWIP_AUTOIP
  if (addressMode == NET_ADDRESS_AUTO)
    autoip_stop(ifc);
#endif
#if LWIP_DHCP
  if (addressMode == NET_ADDRESS_DHCP)
    dhcp_stop(ifc);
#endif
}

/**
 * @brief LWIP handling thread.
 *
 * @param[in] p pointer to a @p lwipthread_opts structure or @p NULL
 * @return The function does not return.
 */
static THD_FUNCTION(lwip_thread, p) {
  event_timer_t evt;
  event_listener_t el0, el1;
  err_t result;
  tcpip_callback_fn link_up_cb = NULL;
  tcpip_callback_fn link_down_cb = NULL;

  chRegSetThreadName(LWIP_THREAD_NAME);

  /* Initializes the thing.*/
  tcpip_init(NULL, NULL);

  /* TCP/IP parameters, runtime or compile time.*/
  if (p) {
    lwipthread_opts_t *opts = p;

    ip.addr = opts->address;
    gateway.addr = opts->gateway;
    netmask.addr = opts->netmask;
    addressMode = opts->addrMode;
#if LWIP_NETIF_HOSTNAME
    thisif.hostname = opts->ourHostName;
#endif
    link_up_cb = opts->link_up_cb;
    link_down_cb = opts->link_down_cb;
  }
  else {
    LWIP_IPADDR(&ip);
    LWIP_GATEWAY(&gateway);
    LWIP_NETMASK(&netmask);
#if LWIP_DHCP
    addressMode = NET_ADDRESS_DHCP;
#elif LWIP_AUTOIP
    addressMode = NET_ADDRESS_AUTO;
#else
    addressMode = NET_ADDRESS_STATIC;
#endif
#if LWIP_NETIF_HOSTNAME
    thisif.hostname = NULL;
#endif
  }

  if (!link_up_cb)
    link_up_cb = lwipDefaultLinkUpCB;
  if (!link_down_cb)
    link_down_cb = lwipDefaultLinkDownCB;

#if LWIP_NETIF_HOSTNAME
  if (thisif.hostname == NULL)
    thisif.hostname = LWIP_NETIF_HOSTNAME_STRING;
#endif

  if (lwip_lld_start((const lwipthread_opts_t *)p, &thisif) !=
      HAL_RET_SUCCESS) {
    chThdSleepMilliseconds(1000);
    osalSysHalt("ethernet start error");
  }

  MIB2_INIT_NETIF(&thisif, snmp_ifType_ethernet_csmacd, 0);

  /* Add interface. */
  result = netifapi_netif_add(&thisif, &ip, &netmask, &gateway, NULL, ethernetif_init, tcpip_input);
  if (result != ERR_OK)
  {
    chThdSleepMilliseconds(1000);     // Give some time to print any other diagnostics.
    osalSysHalt("netif_add error");   // Not sure what else we can do if an error occurs here.
  };

  netifapi_netif_set_default(&thisif);
  netifapi_netif_set_up(&thisif);

  /* Setup event sources.*/
  evtObjectInit(&evt, LWIP_LINK_POLL_INTERVAL);
  evtStart(&evt);
  chEvtRegisterMask(&evt.et_es, &el0, PERIODIC_TIMER_ID);
  chEvtRegisterMaskWithFlags(lwip_get_event_source(), &el1,
                             FRAME_RECEIVED_ID, lwip_get_receive_event_flag());
  chEvtAddEvents(PERIODIC_TIMER_ID | FRAME_RECEIVED_ID);

  /* Resumes the caller and goes to the final priority.*/
  chThdResume(&lwip_trp, MSG_OK);
  chThdSetPriority(LWIP_THREAD_PRIORITY);

  while (true) {
    eventmask_t mask = chEvtWaitAny(ALL_EVENTS);
    if (mask & PERIODIC_TIMER_ID) {
      bool current_link_status = lwip_poll_link_status();
      if (current_link_status != netif_is_link_up(&thisif)) {
        if (current_link_status) {
          tcpip_callback_with_block((tcpip_callback_fn) netif_set_link_up,
                                     &thisif, 1);
          tcpip_callback_with_block(link_up_cb, &thisif, 1);
        }
        else {
          tcpip_callback_with_block((tcpip_callback_fn) netif_set_link_down,
                                     &thisif, 1);
          tcpip_callback_with_block(link_down_cb, &thisif, 1);
        }
      }
    }

    if (mask & FRAME_RECEIVED_ID) {
      struct pbuf *p;
      while (low_level_input(&thisif, &p)) {
        if (p != NULL) {
          struct eth_hdr *ethhdr = p->payload;
          switch (htons(ethhdr->type)) {
            /* IP or ARP packet? */
            case ETHTYPE_IP:
            case ETHTYPE_ARP:
#if LWIP_IPV6
            case ETHTYPE_IPV6:
#endif
              /* full packet send to tcpip_thread to process */
              if (thisif.input(p, &thisif) == ERR_OK)
                break;
              LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
          /* Falls through */
            default:
              pbuf_free(p);
          }
        }
      }
    }
  }
}

/**
 * @brief   Initializes the lwIP subsystem.
 * @note    The function exits after the initialization is finished.
 *
 * @param[in] opts      pointer to the configuration structure, if @p NULL
 *                      then the static configuration is used.
 */
void lwipInit(const lwipthread_opts_t *opts) {

  /* Creating the lwIP thread (it changes priority internally).*/
  chThdCreateStatic(wa_lwip_thread, sizeof (wa_lwip_thread),
                    chThdGetPriorityX() - 1, lwip_thread, (void *)opts);

  /* Waiting for the lwIP thread complete initialization. Note,
     this thread reaches the thread reference object first because
     the relative priorities.*/
  chSysLock();
  chThdSuspendS(&lwip_trp);
  chSysUnlock();
}

typedef struct lwip_reconf_params {
  const lwipreconf_opts_t *opts;
  semaphore_t completion;
} lwip_reconf_params_t;

static void do_reconfigure(void *p)
{
  lwip_reconf_params_t *reconf = (lwip_reconf_params_t*) p;

  switch (addressMode) {
#if LWIP_DHCP
  case NET_ADDRESS_DHCP: {
    if (netif_is_up(&thisif))
      dhcp_stop(&thisif);
    break;
  }
#endif
  case NET_ADDRESS_STATIC: {
    ip4_addr_t zero = { 0 };
    netif_set_ipaddr(&thisif, &zero);
    netif_set_netmask(&thisif, &zero);
    netif_set_gw(&thisif, &zero);
    break;
  }
#if LWIP_AUTOIP
  case NET_ADDRESS_AUTO: {
    if (netif_is_up(&thisif))
      autoip_stop(&thisif);
    break;
  }
#endif
  }

  ip.addr = reconf->opts->address;
  gateway.addr = reconf->opts->gateway;
  netmask.addr = reconf->opts->netmask;
  addressMode = reconf->opts->addrMode;

  switch (addressMode) {
#if LWIP_DHCP
  case NET_ADDRESS_DHCP: {
    if (netif_is_up(&thisif))
      dhcp_start(&thisif);
    break;
  }
#endif
  case NET_ADDRESS_STATIC: {
    netif_set_ipaddr(&thisif, &ip);
    netif_set_netmask(&thisif, &netmask);
    netif_set_gw(&thisif, &gateway);
    break;
  }
#if LWIP_AUTOIP
  case NET_ADDRESS_AUTO: {
    if (netif_is_up(&thisif))
      autoip_start(&thisif);
    break;
  }
#endif
  }

  chSemSignal(&reconf->completion);
}

void lwipReconfigure(const lwipreconf_opts_t *opts)
{
  lwip_reconf_params_t params;
  err_t err;

  params.opts = opts;
  chSemObjectInit(&params.completion, 0);
  err = tcpip_callback_with_block(do_reconfigure, &params, 1);
  if (err == ERR_OK) {
    chSemWait(&params.completion);
  }
}

/** @} */
