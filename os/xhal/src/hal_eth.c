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
 * @file        hal_eth.c
 * @brief       Generated ETH Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_ETH
 * @{
 */

#include "hal.h"

#if (HAL_USE_ETH == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local macros.                                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       ETH Driver initialization.
 * @note        This function is implicitly invoked by @p halInit(), there is
 *              no need to explicitly initialize the driver.
 *
 * @init
 */
void ethInit(void) {

  eth_lld_init();
}

/*===========================================================================*/
/* Module class "hal_eth_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_eth_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_eth_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__eth_objinit_impl(void *ip, const void *vmt) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
#if ETH_USE_SYNCHRONIZATION == TRUE
  osalThreadQueueObjectInit(&self->txqueue);
  osalThreadQueueObjectInit(&self->txqueue);
#endif

#if ETH_USE_EVENTS == TRUE
  osalEventObjectInit(&self->es);
#endif

  /* Optional, user-defined initializer.*/
#if defined(ETH_DRIVER_EXT_INIT_HOOK)
  ETH_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance to be
 *                              disposed.
 */
void __eth_dispose_impl(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The operation status.
 */
msg_t __eth_start_impl(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_start(self);
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 */
void __eth_stop_impl(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  eth_lld_stop(self);
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__eth_setcfg_impl(void *ip, const void *config) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return (const void *)eth_lld_setcfg(self, (const hal_eth_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__eth_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return (const void *)eth_lld_selcfg(self, cfgnum);
}
/** @} */

/**
 * @brief       VMT structure of ETH driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_eth_driver_vmt __hal_eth_driver_vmt = {
  .dispose                  = __eth_dispose_impl,
  .start                    = __eth_start_impl,
  .stop                     = __eth_stop_impl,
  .setcfg                   = __eth_setcfg_impl,
  .selcfg                   = __eth_selcfg_impl,
  .setcb                    = __cbdrv_setcb_impl
};

/**
 * @name        Regular methods of hal_eth_driver_c
 * @{
 */
/**
 * @brief       Returns the link status.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The link status,
 * @retval false                If the link is active.
 * @retval true                 If the link is down or the driver is not in
 *                              active state.
 *
 * @api
 */
bool ethPollLinkStatus(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  if (drvGetStateX(self) == HAL_DRV_STATE_ACTIVE) {
    return eth_lld_poll_link_status(self);
  }

  return true;
}

#if (ETH_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Waits for a received frame availability.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     timeout       Receive timeout.
 * @return                      A receive handle.
 * @retval NULL                 If a received frame is not available within the
 *                              specified timeout.
 */
eth_receive_handle_t ethWaitReceiveHandle(void *ip, sysinterval_t timeout) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;
  eth_receive_handle_t rxh;

  osalDbgCheck(self != NULL);
  osalDbgAssert(drvGetStateX(self) == HAL_DRV_STATE_ACTIVE, "not active");

  osalSysLock();

  while (((rxh = ethGetReceiveHandleI(self)) == NULL)) {
    msg_t msg = osalThreadEnqueueTimeoutS(&self->rxqueue, timeout);
    if (msg == MSG_TIMEOUT) {
      rxh = NULL;
      break;
    }
  }

  osalSysUnlock();

  return rxh;
}

/**
 * @brief       Waits for a transmit frame availability.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     timeout       Transmit timeout.
 * @return                      A transmit handle.
 * @retval NULL                 If a transmit frame is not available within the
 *                              specified timeout.
 */
eth_transmit_handle_t ethWaitTransmitHandle(void *ip, sysinterval_t timeout) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;
  eth_transmit_handle_t txh;

  osalDbgCheck(self != NULL);
  osalDbgAssert(drvGetStateX(self) == HAL_DRV_STATE_ACTIVE, "not active");

  osalSysLock();

  while (((txh = ethGetTransmitHandleI(self)) == NULL)) {
    msg_t msg = osalThreadEnqueueTimeoutS(&self->txqueue, timeout);
    if (msg == MSG_TIMEOUT) {
      txh = NULL;
      break;
    }
  }

  osalSysUnlock();

  return txh;
}
#endif /* ETH_USE_SYNCHRONIZATION == TRUE */
/** @} */

#endif /* HAL_USE_ETH == TRUE */

/** @} */
