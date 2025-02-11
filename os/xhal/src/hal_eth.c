/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @memberof    hal_eth_driver_c
 * @protected
 *
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
 * @memberof    hal_eth_driver_c
 * @protected
 *
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
 * @memberof    hal_eth_driver_c
 * @protected
 *
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
 * @memberof    hal_eth_driver_c
 * @protected
 *
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 */
void __eth_stop_impl(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  eth_lld_stop(self);
}

/**
 * @memberof    hal_eth_driver_c
 * @protected
 *
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
 * @memberof    hal_eth_driver_c
 * @protected
 *
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
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Queries for a received frame handle.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The receive handle.
 * @retval NULL                 If a received frame is not available.
 *
 * @iclass
 */
etc_receive_handle_t ethGetReceiveHandleI(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  osalDbgCheckClassI();

  return eth_lld_get_receive_handle(self);
}

/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Queries for a transmit frame handle.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The transmit handle.
 * @retval NULL                 If an empty transmit frame is not available.
 *
 * @iclass
 */
etc_transmit_handle_t ethGetTransmitHandleI(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  osalDbgCheckClassI();

  return eth_lld_get_transmit_handle(self);
}

/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Releases a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 *
 * @api
 */
void ethReleaseReceiveHandle(void *ip, etc_receive_handle_t rxh) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  eth_lld_release_receive_handle(self, rxh);
}

/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Releases and transmits a frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmi] handle.
 *
 * @api
 */
void ethReleaseTransmitHandle(void *ip, etc_transmit_handle_t txh) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  eth_lld_release_transmit_handle(self, txh);
}

/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    bp            Received data buffer pointer.
 * @param[in]     n             Number of bytes to read.
 * @return                      The number of bytes read from the handle
 *                              buffer, this value can be less than the amount
 *                              specified in the parameter @p size if there are
 *                              no more bytes to read.
 *
 * @api
 */
size_t ethReadReceiveHandle(void *ip, etc_receive_handle_t rxh, uint8_t *bp,
                            size_t n) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_read_receive_handle(self, rxh, bp, n);
}

/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmit handle.
 * @param[in]     bp            Transmit data buffer pointer.
 * @param[in]     n             Number of bytes to write.
 * @return                      The number of bytes written into the handle
 *                              buffer this value can be less than the amount
 *                              specified in the parameter @p size if the
 *                              maximum frame size is reached.
 *
 * @api
 */
size_t ethWriteTransmitHandle(void *ip, etc_transmit_handle_t txh,
                              const uint8_t *bp, size_t n) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_write_transmit_handle(self, txh, bp, n);
}

/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Direct access to the receive handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    sizep         Size of the received frame.
 * @return                      Pointer to the received frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 *
 * @xclass
 */
const uint8_t *ethGetReceiveBufferX(void *ip, etc_receive_handle_t rxh,
                                    size_t *sizep) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_get_receive_buffer(self, rxh, sizep);
}

/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Direct access to the transmit handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmit handle.
 * @param[out]    sizep         Maximum size of the transmit buffer.
 * @return                      Pointer to the transmit frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 *
 * @xclass
 */
uint8_t *ethGetTransmitBufferX(void *ip, etc_transmit_handle_t txh,
                               size_t *sizep) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  return eth_lld_get_transmit_buffer(self, txh, sizep);
}

/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Returns the link status.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The link status,
 * @retval false                If the link is active.
 * @retval true                 If the link is down.
 *
 * @api
 */
bool ethPollLinkStatus(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;

  osalDbgCheck(self != NULL);
  osalDbgAssert(drvGetStateX(self) == HAL_DRV_STATE_ACTIVE, "not active");

  return eth_lld_poll_link_status(self);
}

#if (ETH_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Synchronizes with frame reception.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 */
msg_t ethSynchronizeReceive(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;
}

/**
 * @memberof    hal_eth_driver_c
 * @public
 *
 * @brief       Synchronizes with frame transmission.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 */
msg_t ethSynchronizeTransmit(void *ip) {
  hal_eth_driver_c *self = (hal_eth_driver_c *)ip;
}
#endif /* ETH_USE_SYNCHRONIZATION == TRUE */
/** @} */

#endif /* HAL_USE_ETH == TRUE */

/** @} */
