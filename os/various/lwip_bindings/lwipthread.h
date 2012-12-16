/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

/**
 * @file lwipthread.h
 * @brief LWIP wrapper thread macros and structures.
 * @addtogroup LWIP_THREAD
 * @{
 */

#ifndef _LWIPTHREAD_H_
#define _LWIPTHREAD_H_

/** @brief MAC thread priority.*/
#ifndef LWIP_THREAD_PRIORITY
#define LWIP_THREAD_PRIORITY    LOWPRIO
#endif

/** @brief IP Address. */
#if !defined(LWIP_IPADDR) || defined(__DOXYGEN__)
#define LWIP_IPADDR(p)          IP4_ADDR(p, 192, 168, 1, 20)
#endif

/** @brief IP Gateway. */
#if !defined(LWIP_GATEWAY) || defined(__DOXYGEN__)
#define LWIP_GATEWAY(p)         IP4_ADDR(p, 192, 168, 1, 1)
#endif

/** @brief IP netmask. */
#if !defined(LWIP_NETMASK) || defined(__DOXYGEN__)
#define LWIP_NETMASK(p)         IP4_ADDR(p, 255, 255, 255, 0)
#endif

/** @brief MAC thread stack size. */
#if !defined(LWIP_THREAD_STACK_SIZE) || defined(__DOXYGEN__)
#define LWIP_THREAD_STACK_SIZE  512
#endif

/** @brief Transmission timeout. */
#if !defined(LWIP_SEND_TIMEOUT) || defined(__DOXYGEN__)
#define LWIP_SEND_TIMEOUT       50
#endif

/** @brief Link speed. */
#if !defined(LWIP_LINK_SPEED) || defined(__DOXYGEN__)
#define LWIP_LINK_SPEED         100000000
#endif

/** @brief MAC Address byte 0. */
#if !defined(LWIP_ETHADDR_0) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_0          0xC2
#endif

/** @brief MAC Address byte 1. */
#if !defined(LWIP_ETHADDR_1) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_1          0xAF
#endif

/** @brief MAC Address byte 2. */
#if !defined(LWIP_ETHADDR_2) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_2          0x51
#endif

/** @brief MAC Address byte 3. */
#if !defined(LWIP_ETHADDR_3) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_3          0x03
#endif

/** @brief MAC Address byte 4. */
#if !defined(LWIP_ETHADDR_4) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_4          0xCF
#endif

/** @brief MAC Address byte 5. */
#if !defined(LWIP_ETHADDR_5) || defined(__DOXYGEN__)
#define LWIP_ETHADDR_5          0x46
#endif

/** @brief Interface name byte 0. */
#if !defined(LWIP_IFNAME0) || defined(__DOXYGEN__)
#define LWIP_IFNAME0            'm'
#endif

/** @brief Interface name byte 1. */
#if !defined(LWIP_IFNAME1) || defined(__DOXYGEN__)
#define LWIP_IFNAME1            's'
#endif

/**
 * @brief Runtime TCP/IP settings.
 */
struct lwipthread_opts {
  uint8_t       *macaddress;
  uint32_t      address;
  uint32_t      netmask;
  uint32_t      gateway;
};

extern WORKING_AREA(wa_lwip_thread, LWIP_THREAD_STACK_SIZE);

#ifdef __cplusplus
extern "C" {
#endif
  msg_t lwip_thread(void *p);
#ifdef __cplusplus
}
#endif

#endif /* _LWIPTHREAD_H_ */

/** @} */
