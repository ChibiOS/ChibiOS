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
 * @file    sandbox/hal_eth_lld.c
 * @brief   Sandbox ETH subsystem low level driver source.
 *
 * @addtogroup ETH
 * @{
 */

#include <string.h>

#include "hal.h"

#if HAL_USE_ETH || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief ETH1 driver identifier.*/
#if VIO_ETH_USE_VETH1 || defined(__DOXYGEN__)
hal_eth_driver_c ETHD1;
#endif

/** @brief ETH2 driver identifier.*/
#if VIO_ETH_USE_VETH2 || defined(__DOXYGEN__)
hal_eth_driver_c ETHD2;
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

CC_FORCE_INLINE
static inline uint32_t __eth_veth_init(uint32_t nveth, size_t n, void *p) {

  __syscall3r(227, VIO_CALL(SB_VETH_INIT, nveth), n, p);
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __eth_veth_deinit(uint32_t nveth) {

  __syscall1r(227, VIO_CALL(SB_VETH_DEINIT, nveth));
  return (uint32_t)r0;
}

CC_FORCE_INLINE
static inline uint32_t __eth_veth_selcfg(uint32_t nveth, uint32_t ncfg,
                                         size_t n, void *p) {

  __syscall4r(227, VIO_CALL(SB_VETH_SELCFG, nveth), ncfg, n, p);
  return (uint32_t)r0;
}

static void eth_lld_serve_interrupt(hal_eth_driver_c *ethp, uint32_t nvrq) {
  eventflags_t flags;

  flags = (eventflags_t)__sb_vrq_gcsts(nvrq);
  ethp->lastflags = flags;

  if (flags == (eventflags_t)0U) {
    return;
  }

  osalSysLockFromISR();
#if ETH_USE_SYNCHRONIZATION == TRUE
  if ((flags & ETH_FLAGS_TX) != 0U) {
    osalThreadDequeueAllI(&ethp->txqueue, MSG_OK);
  }
  if ((flags & ETH_FLAGS_RX) != 0U) {
    osalThreadDequeueAllI(&ethp->rxqueue, MSG_OK);
  }
#endif
#if ETH_USE_EVENTS == TRUE
  osalEventBroadcastFlagsI(&ethp->es, flags);
#endif
  osalSysUnlockFromISR();

  __cbdrv_invoke_cb(ethp);
}

static void eth_lld_apply_cfgbuf(hal_eth_driver_c *ethp,
                                 const vio_eth_config_t *vcfgp) {

  ethp->cfgbuf.mac_address = NULL;
  if ((vcfgp->flags & VIO_ETH_CFGF_MAC_ADDRESS) != 0U) {
    memcpy(ethp->cfgmac, vcfgp->mac_address, sizeof ethp->cfgmac);
    ethp->cfgbuf.mac_address = ethp->cfgmac;
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if VIO_ETH_USE_VETH1 || defined(__DOXYGEN__)
#if !defined(VIO_VETH1_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(VIO_VETH1_IRQ)) {

  OSAL_IRQ_PROLOGUE();

  eth_lld_serve_interrupt(&ETHD1, VIO_VETH1_IRQ);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if VIO_ETH_USE_VETH2 || defined(__DOXYGEN__)
#if !defined(VIO_VETH2_SUPPRESS_ISR)
OSAL_IRQ_HANDLER(MK_VECTOR(VIO_VETH2_IRQ)) {

  OSAL_IRQ_PROLOGUE();

  eth_lld_serve_interrupt(&ETHD2, VIO_VETH2_IRQ);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void eth_lld_init(void) {

#if VIO_ETH_USE_VETH1 == TRUE
  ethObjectInit(&ETHD1);
  ETHD1.nveth = 0U;
  __sb_vrq_seten(1U << VIO_VETH1_IRQ);
#endif
#if VIO_ETH_USE_VETH2 == TRUE
  ethObjectInit(&ETHD2);
  ETHD2.nveth = 1U;
  __sb_vrq_seten(1U << VIO_VETH2_IRQ);
#endif
}

msg_t eth_lld_start(hal_eth_driver_c *ethp) {
  vio_eth_config_t vcfg;
  msg_t msg = HAL_RET_SUCCESS;

  if (false) {
  }
#if VIO_ETH_USE_VETH1 == TRUE
  else if (&ETHD1 == ethp) {
    msg = (msg_t)__eth_veth_init(ethp->nveth, sizeof vcfg, &vcfg);
  }
#endif
#if VIO_ETH_USE_VETH2 == TRUE
  else if (&ETHD2 == ethp) {
    msg = (msg_t)__eth_veth_init(ethp->nveth, sizeof vcfg, &vcfg);
  }
#endif
  else {
    osalDbgAssert(false, "invalid ETH instance");
  }

  if (msg == HAL_RET_SUCCESS) {
    eth_lld_apply_cfgbuf(ethp, &vcfg);
    ethp->config = &ethp->cfgbuf;
  }

  return msg;
}

void eth_lld_stop(hal_eth_driver_c *ethp) {
  msg_t msg = HAL_RET_SUCCESS;

  if (false) {
  }
#if VIO_ETH_USE_VETH1 == TRUE
  else if (&ETHD1 == ethp) {
    msg = (msg_t)__eth_veth_deinit(ethp->nveth);
  }
#endif
#if VIO_ETH_USE_VETH2 == TRUE
  else if (&ETHD2 == ethp) {
    msg = (msg_t)__eth_veth_deinit(ethp->nveth);
  }
#endif
  else {
    osalDbgAssert(false, "invalid ETH instance");
  }

  osalDbgAssert(msg == HAL_RET_SUCCESS, "unexpected failure");
}

const hal_eth_config_t *eth_lld_setcfg(hal_eth_driver_c *ethp,
                                       const hal_eth_config_t *config) {

  (void)ethp;
  (void)config;

  return NULL;
}

const hal_eth_config_t *eth_lld_selcfg(hal_eth_driver_c *ethp,
                                       unsigned cfgnum) {
  msg_t msg;
  vio_eth_config_t vcfg;

  msg = (msg_t)__eth_veth_selcfg(ethp->nveth, cfgnum, sizeof vcfg, &vcfg);
  if (msg == HAL_RET_SUCCESS) {
    eth_lld_apply_cfgbuf(ethp, &vcfg);
    return &ethp->cfgbuf;
  }

  return NULL;
}

eth_receive_handle_t eth_lld_get_receive_handle(hal_eth_driver_c *ethp) {

  __syscall1r(99, VIO_CALL(SB_VETH_RXGET, ethp->nveth));
  return (eth_receive_handle_t)r0;
}

eth_transmit_handle_t eth_lld_get_transmit_handle(hal_eth_driver_c *ethp) {

  __syscall1r(99, VIO_CALL(SB_VETH_TXGET, ethp->nveth));
  return (eth_transmit_handle_t)r0;
}

bool eth_lld_is_receive_handle_valid(hal_eth_driver_c *ethp,
                                     eth_receive_handle_t rxh) {

  (void)ethp;

  return rxh != (eth_receive_handle_t)0U;
}

bool eth_lld_is_transmit_handle_valid(hal_eth_driver_c *ethp,
                                      eth_transmit_handle_t txh) {

  (void)ethp;

  return txh != (eth_transmit_handle_t)0U;
}

void eth_lld_release_receive_handle(hal_eth_driver_c *ethp,
                                    eth_receive_handle_t rxh) {

  __syscall2r(227, VIO_CALL(SB_VETH_RXREL, ethp->nveth), rxh);
  osalDbgAssert(r0 == HAL_RET_SUCCESS, "unexpected failure");
}

void eth_lld_release_transmit_handle(hal_eth_driver_c *ethp,
                                     eth_transmit_handle_t txh) {

  __syscall2r(227, VIO_CALL(SB_VETH_TXREL, ethp->nveth), txh);
  osalDbgAssert(r0 == HAL_RET_SUCCESS, "unexpected failure");
}

size_t eth_lld_read_receive_handle(hal_eth_driver_c *ethp,
                                   eth_receive_handle_t rxh,
                                   uint8_t *bp, size_t n) {

  __syscall4r(227, VIO_CALL(SB_VETH_RXREAD, ethp->nveth), rxh, bp, n);
  osalDbgAssert(((int32_t)r0) >= 0, "host RXREAD failed");
  return (size_t)r0;
}

size_t eth_lld_write_transmit_handle(hal_eth_driver_c *ethp,
                                     eth_transmit_handle_t txh,
                                     const uint8_t *bp, size_t n) {

  __syscall4r(227, VIO_CALL(SB_VETH_TXWRITE, ethp->nveth), txh, bp, n);
  osalDbgAssert(((int32_t)r0) >= 0, "host TXWRITE failed");
  return (size_t)r0;
}

bool eth_lld_poll_link_status(hal_eth_driver_c *ethp) {

  __syscall1r(227, VIO_CALL(SB_VETH_LINK, ethp->nveth));
  return (bool)r0;
}

#endif /* HAL_USE_ETH */

/** @} */
