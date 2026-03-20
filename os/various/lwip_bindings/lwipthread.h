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
 * @file lwipthread.h
 * @brief LWIP wrapper thread macros and structures.
 * @addtogroup LWIP_THREAD
 * @{
 */

#ifndef LWIPTHREAD_H
#define LWIPTHREAD_H

#include "hal.h"
#include <lwip/init.h>

/**
 * @brief   lwIP default network interface maximum transmission unit (MTU).
 */
#if !defined(LWIP_NETIF_MTU) || defined(__DOXYGEN__)
#define LWIP_NETIF_MTU                      1500
#endif

/**
 * @brief   Default network interface hostname.
 */
#if !defined(LWIP_NETIF_HOSTNAME_STRING) || defined(__DOXYGEN__)
#define LWIP_NETIF_HOSTNAME_STRING          "lwip"
#endif

/**
 * @brief   Default network interface hostname.
 */
#if !defined(LWIP_THREAD_NAME) || defined(__DOXYGEN__)
#define LWIP_THREAD_NAME                    "lwipthread"
#endif

/**
 * @brief   lwIP thread priority.
 */
#ifndef LWIP_THREAD_PRIORITY
#define LWIP_THREAD_PRIORITY                LOWPRIO
#endif

/**
 * @brief  lwIP thread stack size.
 */
#if !defined(LWIP_THREAD_STACK_SIZE) || defined(__DOXYGEN__)
#define LWIP_THREAD_STACK_SIZE              672
#endif

/**
 * @brief   Link poll interval.
 */
#if !defined(LWIP_LINK_POLL_INTERVAL) || defined(__DOXYGEN__)
#define LWIP_LINK_POLL_INTERVAL             TIME_S2I(5)
#endif

/**
 *  @brief  IP Address.
 */
#if !defined(LWIP_IPADDR) || defined(__DOXYGEN__)
#define LWIP_IPADDR(p)                      IP4_ADDR(p, 192, 168, 1, 10)
#endif

/**
 * @brief   IP Gateway.
 */
#if !defined(LWIP_GATEWAY) || defined(__DOXYGEN__)
#define LWIP_GATEWAY(p)                     IP4_ADDR(p, 192, 168, 1, 1)
#endif

/**
 * @brief   IP netmask.
 */
#if !defined(LWIP_NETMASK) || defined(__DOXYGEN__)
#define LWIP_NETMASK(p)                     IP4_ADDR(p, 255, 255, 255, 0)
#endif

/**
 * @brief   Transmission timeout.
 */
#if !defined(LWIP_SEND_TIMEOUT) || defined(__DOXYGEN__)
#define LWIP_SEND_TIMEOUT                   50
#endif

/**
 * @brief   Link speed.
 */
#if !defined(LWIP_LINK_SPEED) || defined(__DOXYGEN__)
#define LWIP_LINK_SPEED                     100000000
#endif

/**
 * @brief   MAC Address byte 0.
 */
#if !defined(LWIP_ETHADDR_0) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_0                      0xC2
#endif

/**
 * @brief   MAC Address byte 1.
 */
#if !defined(LWIP_ETHADDR_1) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_1                      0xAF
#endif

/**
 * @brief   MAC Address byte 2.
 */
#if !defined(LWIP_ETHADDR_2) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_2                      0x51
#endif

/**
 * @brief   MAC Address byte 3.
 */
#if !defined(LWIP_ETHADDR_3) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_3                      0x03
#endif

/**
 * @brief   MAC Address byte 4.
 */
#if !defined(LWIP_ETHADDR_4) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_4                      0xCF
#endif

/**
 * @brief   MAC Address byte 5.
 */
#if !defined(LWIP_ETHADDR_5) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_5                      0x46
#endif

/**
 * @brief   Interface name byte 0.
 */
#if !defined(LWIP_IFNAME0) || defined(__DOXYGEN__)
#define LWIP_IFNAME0                        'm'
#endif

/**
 * @brief  Interface name byte 1.
 */
#if !defined(LWIP_IFNAME1) || defined(__DOXYGEN__)
#define LWIP_IFNAME1                        's'
#endif

/**
 *  @brief   Utility macro to define an IPv4 address.
 *
 *  @note    Within the networking subsystem, IPv4 network addresses are
 *           stored with LS byte of network address in MS byte of unsigned int.
 */
#if BYTE_ORDER == LITTLE_ENDIAN
#define IP4_ADDR_VALUE(a,b,c,d)        \
        (((u32_t)((d) & 0xff) << 24) | \
         ((u32_t)((c) & 0xff) << 16) | \
         ((u32_t)((b) & 0xff) << 8)  | \
          (u32_t)((a) & 0xff))
#else
#define IP4_ADDR_VALUE(a,b,c,d)        \
        (((u32_t)((a) & 0xff) << 24) | \
         ((u32_t)((b) & 0xff) << 16) | \
         ((u32_t)((c) & 0xff) << 8)  | \
          (u32_t)((d) & 0xff))
#endif

/**
 *  @brief   Startup network assigning modes.
 */
typedef enum {
#if LWIP_DHCP || defined(__DOXYGEN__)
    /** 
     * @brief   Assign a DHCP given address.
     */
    NET_ADDRESS_DHCP = 1,
#endif
    /**
     * @brief   Assign a statically IPv4 address. 
     */
    NET_ADDRESS_STATIC = 2,
#if LWIP_AUTOIP  || defined(__DOXYGEN__)
    /**
     * @brief   Assign an IPv4 link-Local address.
     */
    NET_ADDRESS_AUTO = 3
#endif
} net_addr_mode_t;

/**
 * @brief   Type of an application-owned Ethernet driver configuration.
 */
#if defined(__CHIBIOS_XHAL_CONF__)

#if (HAL_USE_ETH != TRUE)
#error "lwipthread requires HAL_USE_ETH in XHAL mode"
#endif

#if (ETH_USE_SYNCHRONIZATION != TRUE)
#error "lwipthread requires ETH_USE_SYNCHRONIZATION in XHAL mode"
#endif

#if (ETH_USE_EVENTS != TRUE)
#error "lwipthread requires ETH_USE_EVENTS in XHAL mode"
#endif

typedef hal_eth_config_t lwipthread_ethcfg_t;
typedef eth_receive_handle_t lwip_receive_handle_t;
typedef eth_transmit_handle_t lwip_transmit_handle_t;
#elif defined(__DOXYGEN__)
typedef hal_eth_config_t lwipthread_ethcfg_t;
typedef eth_receive_handle_t lwip_receive_handle_t;
typedef eth_transmit_handle_t lwip_transmit_handle_t;
#else

#if (HAL_USE_MAC != TRUE)
#error "lwipthread requires HAL_USE_MAC in HAL mode"
#endif

typedef MACConfig lwipthread_ethcfg_t;
typedef MACReceiveDescriptor lwip_receive_handle_t;
typedef MACTransmitDescriptor lwip_transmit_handle_t;
#endif

/**
 * @brief   Runtime TCP/IP settings.
 */
typedef struct lwipthread_opts {
  /**
   * @brief   Pointer to an application-owned Ethernet driver configuration.
   * @note    If not @p NULL then it is applied before the driver is started.
   * @note    The effective MAC address is taken from the selected/applied
   *          driver configuration.
   */
  const lwipthread_ethcfg_t *eth_config;
#if defined(__CHIBIOS_XHAL_CONF__) || defined(__DOXYGEN__)
  /**
   * @brief   ETH configuration index to be selected when @p eth_config is
   *          @p NULL.
   * @note    The default configuration is index zero.
   */
  unsigned        eth_cfgnum;
#endif
  /**
   * @brief   Network address as 32-bit unsigned integer.
   */
  uint32_t        address;
  /**
   * @brief   Network subnet mask as 32-bit unsigned integer.
   */
  uint32_t        netmask;
  /**
   * @brief   Network gateway as 32-bit unsigned integer.
   */
  uint32_t        gateway;
  /**
   * @brief   Startup network addressing mode - static, DHCP, auto.
   */
  net_addr_mode_t addrMode;
  /**
   * @brief   Host name. If NULL, a default string is used.
   * @note    Not checked for validity. In particular, spaces not allowed.
   */
#if LWIP_NETIF_HOSTNAME || defined(__DOXYGEN__)
  const char              *ourHostName;
#endif
  /**
   * @brief   Link up callback.
   *
   * @note    Called from the tcpip thread when the link goes up.
   *          Can be NULL to default to lwipDefaultLinkUpCB.
   */
  void (*link_up_cb)(void*);
  /**
   * @brief   Link down callback.
   *
   * @note    Called from the tcpip thread when the link goes down.
   *          Can be NULL to default to lwipDefaultLinkDownCB.
   */
  void (*link_down_cb)(void*);
} lwipthread_opts_t;

/**
 * @brief   Parameters for lwipReconfigure.
 * @note    Same meaning as in lwipthread_opts_t.
 */
typedef struct lwipreconf_opts {
  uint32_t        address;
  uint32_t        netmask;
  uint32_t        gateway;
  net_addr_mode_t addrMode;
} lwipreconf_opts_t;

#ifdef __cplusplus
extern "C" {
#endif
  struct netif;

  /*
   * Backend interface used by the common lwIP thread implementation.
   */
  msg_t lwip_lld_start(const lwipthread_opts_t *opts, struct netif *netif);
  msg_t lwip_wait_transmit_handle(lwip_transmit_handle_t *txhp);
  size_t lwip_write_transmit_handle(lwip_transmit_handle_t *txhp,
                                    const uint8_t *bp,
                                    size_t size);
  void lwip_release_transmit_handle(lwip_transmit_handle_t *txhp);
  msg_t lwip_wait_receive_handle(lwip_receive_handle_t *rxhp);
  size_t lwip_read_receive_handle(lwip_receive_handle_t *rxhp,
                                  uint8_t *bp,
                                  size_t size);
  size_t lwip_receive_size(lwip_receive_handle_t *rxhp);
  void lwip_release_receive_handle(lwip_receive_handle_t *rxhp);
  event_source_t *lwip_get_event_source(void);
  eventflags_t lwip_get_receive_event_flag(void);
  bool lwip_poll_link_status(void);

  void lwipDefaultLinkUpCB(void *p);
  void lwipDefaultLinkDownCB(void *p);
  void lwipInit(const lwipthread_opts_t *opts);
  void lwipReconfigure(const lwipreconf_opts_t *opts);
#ifdef __cplusplus
}
#endif

#endif /* LWIPTHREAD_H */

/** @} */
