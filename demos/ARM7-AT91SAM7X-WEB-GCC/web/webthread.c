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

#include <string.h>

#include <ch.h>
#include <evtimer.h>
#include <sam7x_emac.h>

#include <uip.h>
#include <uip_arp.h>
#include <httpd.h>
#include <timer.h>
#include <clock-arch.h>

#define IPADDR0  10
#define IPADDR1  151
#define IPADDR2  218
#define IPADDR3  245

#define SEND_RETRY_MAX 10
#define SEND_RETRY_INTERVAL 2

static const struct uip_eth_addr macaddr = {
  {0xC2, 0xAF, 0x51, 0x03, 0xCF, 0x46}
};

static EvTimer evt;
struct EventListener el0, el1;

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

/*
 * uIP send function wrapping the EMAC functions.
 */
static void network_device_send(void) {
  int i;
  BufDescriptorEntry *bdep;

  for (i = 0; i < SEND_RETRY_MAX; i++) {
    if ((bdep = EMACGetTransmitBuffer()) != NULL) {
      uint8_t *bp = (uint8_t *)bdep->w1;
      
      if(uip_len <= UIP_LLH_LEN + UIP_TCPIP_HLEN)
        memcpy(bp, &uip_buf[0], uip_len);
      else {
        memcpy(bp, &uip_buf[0], UIP_LLH_LEN + UIP_TCPIP_HLEN);
        memcpy(bp + UIP_LLH_LEN + UIP_TCPIP_HLEN,
               uip_appdata,
               uip_len - (UIP_LLH_LEN + UIP_TCPIP_HLEN));
      }
      EMACTransmit(bdep, uip_len);
      return;
    }
    chThdSleep(SEND_RETRY_INTERVAL);
  }
  /* Dropped... */
}

/*
 * uIP receive function wrapping the EMAC function.
 */
static size_t network_device_read(void) {
  size_t size = UIP_CONF_BUFFER_SIZE;
  if (EMACReceive(uip_buf, &size))
    return size;
  return 0;
}

void clock_init(void) {}

clock_time_t clock_time( void )
{
  return chSysGetTime();
}

/*
 * Ethernet link status monitor.
 */
static void TimerHandler(eventid_t id) {

  (void)EMACGetLinkStatus();
}

msg_t WebThread(void *p) {
  static const evhandler_t evhndl[] = {
    TimerHandler,
    NULL
  };
  uip_ipaddr_t ipaddr;
  struct timer periodic_timer, arp_timer;
  
  EMACSetAddress(&macaddr.addr[0]);

  /*
   * Event sources setup.
   */
  evtInit(&evt, 1000);                  /* Initializes an event timer object.   */
  evtStart(&evt);                       /* Starts the event timer.              */
  chEvtRegister(&evt.et_es, &el0, 0);   /* Registers on the timer event source. */
  chEvtRegister(&EMACFrameReceived, &el1, 1);

  /*
   * uIP initialization.
   */
  timer_set(&periodic_timer, CLOCK_SECOND / 2);
  timer_set(&arp_timer, CLOCK_SECOND * 10);
  uip_init();
  uip_setethaddr(macaddr);
  uip_ipaddr(ipaddr, IPADDR0, IPADDR1, IPADDR2, IPADDR3);
  uip_sethostaddr(ipaddr);
  httpd_init();

  while (TRUE) {
    uip_len = network_device_read();
    if (uip_len > 0) {    
      if (BUF->type == htons(UIP_ETHTYPE_IP)) {
        uip_arp_ipin();
        uip_input();
        if (uip_len > 0) {
          uip_arp_out();
          network_device_send();
        }
      }
      else if (BUF->type == htons(UIP_ETHTYPE_ARP)) {
        uip_arp_arpin();
        if (uip_len > 0)
          network_device_send();
      }
    }
    else {
      if (timer_expired(&periodic_timer)) {
        int i;
        timer_reset(&periodic_timer);
        for (i = 0; i < UIP_CONNS; i++) {
          uip_periodic(i);
          if (uip_len > 0) {
            uip_arp_out();
            network_device_send();
          }
        }       
        if (timer_expired(&arp_timer)) {
          timer_reset(&arp_timer);
          uip_arp_timer();
        }
      }
      else {                       
        chEvtWait(ALL_EVENTS, evhndl);
      }
    }
  }
  return 0;
}
