/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include <string.h>

#include <ch.h>
#include <evtimer.h>
#include <sam7x_emac.h>

#include <uip.h>
#include <uip_arp.h>
#include <httpd.h>
#include <clock-arch.h>

#define IPADDR0  192
#define IPADDR1  168
#define IPADDR2  1
#define IPADDR3  20

#define SEND_RETRY_MAX 10
#define SEND_RETRY_INTERVAL 2

static const struct uip_eth_addr macaddr = {
  {0xC2, 0xAF, 0x51, 0x03, 0xCF, 0x46}
};

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
  return chTimeNow();
}

/*
 * TCP/IP periodic timer.
 */
static void PeriodicTimerHandler(eventid_t id) {
  int i;

  for (i = 0; i < UIP_CONNS; i++) {
    uip_periodic(i);
    if (uip_len > 0) {
      uip_arp_out();
      network_device_send();
    }
  }
}

/*
 * ARP periodic timer.
 */
static void ARPTimerHandler(eventid_t id) {

  uip_arp_timer();
  (void)EMACGetLinkStatus();
}

/*
 * Ethernet frame received.
 */
static void FrameReceivedHandler(eventid_t id) {

  while ((uip_len = network_device_read()) > 0) {
    if (BUF->type == HTONS(UIP_ETHTYPE_IP)) {
      uip_arp_ipin();
      uip_input();
      if (uip_len > 0) {
        uip_arp_out();
        network_device_send();
      }
    }
    else if (BUF->type == HTONS(UIP_ETHTYPE_ARP)) {
      uip_arp_arpin();
      if (uip_len > 0)
        network_device_send();
    }
  }
}

#define FRAME_RECEIVED_ID       0
#define PERIODIC_TIMER_ID       1
#define ARP_TIMER_ID            2

static const evhandler_t evhndl[] = {
  FrameReceivedHandler,
  PeriodicTimerHandler,
  ARPTimerHandler
};

msg_t WebThread(void *p) {
  EvTimer evt1, evt2;
  EventListener el0, el1, el2;
  uip_ipaddr_t ipaddr;

  /*
   * Event sources setup.
   */
  chEvtRegister(&EMACFrameReceived, &el0, FRAME_RECEIVED_ID);
  chEvtPend(EVENT_MASK(FRAME_RECEIVED_ID)); /* In case some frames are already buffered */

  evtInit(&evt1, MS2ST(500));
  evtStart(&evt1);
  chEvtRegister(&evt1.et_es, &el1, PERIODIC_TIMER_ID);

  evtInit(&evt2, S2ST(10));
  evtStart(&evt2);
  chEvtRegister(&evt2.et_es, &el2, ARP_TIMER_ID);

  /*
   * EMAC settings.
   */
  EMACSetAddress(&macaddr.addr[0]);
  (void)EMACGetLinkStatus();

  /*
   * uIP initialization.
   */
  uip_init();
  uip_setethaddr(macaddr);
  uip_ipaddr(ipaddr, IPADDR0, IPADDR1, IPADDR2, IPADDR3);
  uip_sethostaddr(ipaddr);
  httpd_init();

  while (TRUE) {
    chEvtDispatch(evhndl, chEvtWaitOne(ALL_EVENTS));
  }
  return 0;
}
