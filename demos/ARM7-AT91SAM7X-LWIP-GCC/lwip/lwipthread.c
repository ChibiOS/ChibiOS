/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

#include <ch.h>
#include <evtimer.h>
#include <mac.h>

#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include <lwip/tcpip.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

#include "lwipthread.h"

#define PERIODIC_TIMER_ID       1
#define FRAME_RECEIVED_ID       2

/**
 * Stack area for the LWIP-MAC thread.
 */
WORKING_AREA(wa_lwip_thread, LWIP_THREAD_STACK_SIZE);

/*
 * Initialization.
 */
static void low_level_init(struct netif *netif) {
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] = 0xC2;
  netif->hwaddr[1] = 0xAF;
  netif->hwaddr[2] = 0x51;
  netif->hwaddr[3] = 0x03;
  netif->hwaddr[4] = 0xCF;
  netif->hwaddr[5] = 0x46;

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

  /* Do whatever else is needed to initialize interface. */
  macSetAddress(&ETH1, netif->hwaddr);
}

/*
 * Transmits a frame.
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p) {
  struct pbuf *q;
  MACTransmitDescriptor td;

  if (macWaitTransmitDescriptor(&ETH1, &td, MS2ST(LWIP_SEND_TIMEOUT)) != RDY_OK)
    return ERR_TIMEOUT;

#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE);        /* drop the padding word */
#endif

  /* Iterates through the pbuf chain. */
  for(q = p; q != NULL; q = q->next) {
    macWriteTransmitDescriptor(&td, (uint8_t *)q->payload, (size_t)q->len);
  }
  macReleaseTransmitDescriptor(&td);

#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE);         /* reclaim the padding word */
#endif

  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

/*
 * Receives a frame.
 */
static struct pbuf *low_level_input(struct netif *netif) {
  MACReceiveDescriptor rd;
  struct pbuf *p, *q;
  u16_t len;

  if (macWaitReceiveDescriptor(&ETH1, &rd, TIME_IMMEDIATE) == RDY_OK) {
    len = (u16_t)rd.rd_size;

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE;        /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (p != NULL) {

#if ETH_PAD_SIZE
      pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

      /* Iterates through the pbuf chain. */
      for(q = p; q != NULL; q = q->next) {
        macReadReceiveDescriptor(&rd, (uint8_t *)q->payload, (size_t)q->len);
      }
      macReleaseReceiveDescriptor(&rd);

#if ETH_PAD_SIZE
      pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

      LINK_STATS_INC(link.recv);
    }
    else {
      macReleaseReceiveDescriptor(&rd);
      LINK_STATS_INC(link.memerr);
      LINK_STATS_INC(link.drop);
    }
    return p;
  }
  return NULL;
}

/*
 * Initialization.
 */
static err_t ethernetif_init(struct netif *netif) {
#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LWIP_LINK_SPEED);

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

/**
 * @brief LWIP handling thread.
 *
 * @param[in] p not used
 * @return The function does not return.
 */
msg_t lwip_thread(void *p) {
  EvTimer evt;
  EventListener el0, el1;
  struct ip_addr ip, gateway, netmask;
  static struct netif thisif;

  /* Initializes the thing.*/
  sys_init();
  mem_init();
  memp_init();
  pbuf_init();
  netif_init();
  ip_init();
  tcpip_init(NULL, NULL);

  /* TCP/IP parameters.*/
  LWIP_IPADDR(&ip);
  LWIP_GATEWAY(&gateway);
  LWIP_NETMASK(&netmask);
  netif_add(&thisif, &ip, &netmask, &gateway, NULL, ethernetif_init, tcpip_input);

  netif_set_default(&thisif);
  netif_set_up(&thisif);

  /* Setup event sources.*/
  evtInit(&evt, S2ST(5));
  evtStart(&evt);
  chEvtRegisterMask(&evt.et_es, &el0, PERIODIC_TIMER_ID);
  chEvtRegisterMask(macGetReceiveEventSource(&ETH1), &el1, FRAME_RECEIVED_ID);
  chEvtPend(PERIODIC_TIMER_ID | FRAME_RECEIVED_ID);

  while (TRUE) {
    eventmask_t mask = chEvtWaitAny(ALL_EVENTS);
    if (mask & PERIODIC_TIMER_ID)
      (void)macPollLinkStatus(&ETH1);
    if (mask & FRAME_RECEIVED_ID) {
      struct pbuf *p;
      while ((p = low_level_input(&thisif)) != NULL) {
        struct eth_hdr *ethhdr = p->payload;
        switch (htons(ethhdr->type)) {
        /* IP or ARP packet? */
        case ETHTYPE_IP:
        case ETHTYPE_ARP:
#if PPPOE_SUPPORT
        /* PPPoE packet? */
        case ETHTYPE_PPPOEDISC:
        case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
          /* full packet send to tcpip_thread to process */
          if (thisif.input(p, &thisif) == ERR_OK)
            break;
          LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
        default:
          pbuf_free(p);
        }
      }
    }
  }
  return 0;
}

/** @} */
