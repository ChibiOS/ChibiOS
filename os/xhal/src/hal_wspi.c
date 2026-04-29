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
 * @file        hal_wspi.c
 * @brief       Generated WSPI Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_WSPI
 * @{
 */

#include "hal.h"

#if (HAL_USE_WSPI == TRUE) || defined(__DOXYGEN__)

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
 * @brief       WSPI Driver initialization.
 *
 * @init
 */
void wspiInit(void) {
  wspi_lld_init();
}

/*===========================================================================*/
/* Module class "hal_wspi_driver_c" methods.                                 */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_wspi_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_wspi_driver_c instance to
 *                              be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__wspi_objinit_impl(void *ip, const void *vmt) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
#if WSPI_USE_SYNCHRONIZATION == TRUE
  self->sync_transfer = NULL;
#endif

  /* Optional, user-defined initializer.*/
#if defined(WSPI_DRIVER_EXT_INIT_HOOK)
  WSPI_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance to
 *                              be disposed.
 */
void __wspi_dispose_impl(void *ip) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __wspi_start_impl(void *ip, const void *config) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  msg_t msg;

  if (config != NULL) {
    self->config = __wspi_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  msg = wspi_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 */
void __wspi_stop_impl(void *ip) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  wspi_lld_stop(self);
#if WSPI_USE_SYNCHRONIZATION == TRUE
  osalThreadResumeI(&self->sync_transfer, MSG_RESET);
#endif
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__wspi_setcfg_impl(void *ip, const void *config) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  return (const void *)wspi_lld_setcfg(self,
                                       (const hal_wspi_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__wspi_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  return (const void *)wspi_lld_selcfg(self, cfgnum);
}
/** @} */

/**
 * @brief       VMT structure of WSPI driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_wspi_driver_vmt __hal_wspi_driver_vmt = {
  .dispose                  = __wspi_dispose_impl,
  .start                    = __wspi_start_impl,
  .stop                     = __wspi_stop_impl,
  .setcfg                   = __wspi_setcfg_impl,
  .selcfg                   = __wspi_selcfg_impl,
  .oncbset                  = __cbdrv_oncbset_impl
};

/**
 * @name        Regular methods of hal_wspi_driver_c
 * @{
 */
/**
 * @brief       Sends a command without data phase.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 *
 * @iclass
 */
void wspiStartCommandI(void *ip, const wspi_command_t *cmdp) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (cmdp != NULL));
  osalDbgCheck((cmdp->cfg & WSPI_CFG_DATA_MODE_MASK) == WSPI_CFG_DATA_MODE_NONE);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = WSPI_STATE_COMMAND;
  wspi_lld_command(self, cmdp);
}

/**
 * @brief       Sends a command without data phase.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 *
 * @api
 */
void wspiStartCommand(void *ip, const wspi_command_t *cmdp) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheck((self != NULL) && (cmdp != NULL));

  osalSysLock();
  wspiStartCommandI(self, cmdp);
  osalSysUnlock();
}

/**
 * @brief       Sends a command with data over the WSPI bus.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 * @param[in]     n             Number of bytes to send.
 * @param[in]     txbuf         Pointer to the transmit buffer.
 *
 * @iclass
 */
void wspiStartSendI(void *ip, const wspi_command_t *cmdp, size_t n,
                    const uint8_t *txbuf) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (cmdp != NULL) && (n > 0U) && (txbuf != NULL));
  osalDbgCheck((cmdp->cfg & WSPI_CFG_DATA_MODE_MASK) != WSPI_CFG_DATA_MODE_NONE);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = WSPI_STATE_SEND;
  wspi_lld_send(self, cmdp, n, txbuf);
}

/**
 * @brief       Sends a command with data over the WSPI bus.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 * @param[in]     n             Number of bytes to send.
 * @param[in]     txbuf         Pointer to the transmit buffer.
 *
 * @api
 */
void wspiStartSend(void *ip, const wspi_command_t *cmdp, size_t n,
                   const uint8_t *txbuf) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheck((self != NULL) && (cmdp != NULL) && (n > 0U) && (txbuf != NULL));

  osalSysLock();
  wspiStartSendI(self, cmdp, n, txbuf);
  osalSysUnlock();
}

/**
 * @brief       Receives data from the WSPI bus.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 * @param[in]     n             Number of bytes to receive.
 * @param[out]    rxbuf         Pointer to the receive buffer.
 *
 * @iclass
 */
void wspiStartReceiveI(void *ip, const wspi_command_t *cmdp, size_t n,
                       uint8_t *rxbuf) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (cmdp != NULL) && (n > 0U) && (rxbuf != NULL));
  osalDbgCheck((cmdp->cfg & WSPI_CFG_DATA_MODE_MASK) != WSPI_CFG_DATA_MODE_NONE);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = WSPI_STATE_RECEIVE;
  wspi_lld_receive(self, cmdp, n, rxbuf);
}

/**
 * @brief       Receives data from the WSPI bus.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 * @param[in]     n             Number of bytes to receive.
 * @param[out]    rxbuf         Pointer to the receive buffer.
 *
 * @api
 */
void wspiStartReceive(void *ip, const wspi_command_t *cmdp, size_t n,
                      uint8_t *rxbuf) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheck((self != NULL) && (cmdp != NULL) && (n > 0U) && (rxbuf != NULL));

  osalSysLock();
  wspiStartReceiveI(self, cmdp, n, rxbuf);
  osalSysUnlock();
}

#if (WSPI_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Sends a command without data phase and waits for completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 * @return                      The operation status.
 * @retval false                If the operation succeeded.
 * @retval true                 If the operation failed because HW issues.
 *
 * @api
 */
bool wspiCommand(void *ip, const wspi_command_t *cmdp) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  msg_t msg;

  osalDbgCheck((self != NULL) && (cmdp != NULL));
  osalDbgCheck((cmdp->cfg & WSPI_CFG_DATA_MODE_MASK) == WSPI_CFG_DATA_MODE_NONE);

  osalSysLock();
  osalDbgAssert(drvGetCallbackX(self) == NULL, "has callback");
  wspiStartCommandI(self, cmdp);
  msg = osalThreadSuspendS(&self->sync_transfer);
  osalSysUnlock();

  return (bool)(msg != MSG_OK);
}

/**
 * @brief       Sends a command with data over the WSPI bus and waits for
 *              completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 * @param[in]     n             Number of bytes to send.
 * @param[in]     txbuf         Pointer to the transmit buffer.
 * @return                      The operation status.
 * @retval false                If the operation succeeded.
 * @retval true                 If the operation failed because HW issues.
 *
 * @api
 */
bool wspiSend(void *ip, const wspi_command_t *cmdp, size_t n,
              const uint8_t *txbuf) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  msg_t msg;

  osalDbgCheck((self != NULL) && (cmdp != NULL) && (n > 0U) && (txbuf != NULL));
  osalDbgCheck((cmdp->cfg & WSPI_CFG_DATA_MODE_MASK) != WSPI_CFG_DATA_MODE_NONE);

  osalSysLock();
  osalDbgAssert(drvGetCallbackX(self) == NULL, "has callback");
  wspiStartSendI(self, cmdp, n, txbuf);
  msg = osalThreadSuspendS(&self->sync_transfer);
  osalSysUnlock();

  return (bool)(msg != MSG_OK);
}

/**
 * @brief       Receives data from the WSPI bus and waits for completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 * @param[in]     n             Number of bytes to receive.
 * @param[out]    rxbuf         Pointer to the receive buffer.
 * @return                      The operation status.
 * @retval false                If the operation succeeded.
 * @retval true                 If the operation failed because HW issues.
 *
 * @api
 */
bool wspiReceive(void *ip, const wspi_command_t *cmdp, size_t n,
                 uint8_t *rxbuf) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  msg_t msg;

  osalDbgCheck((self != NULL) && (cmdp != NULL) && (n > 0U) && (rxbuf != NULL));
  osalDbgCheck((cmdp->cfg & WSPI_CFG_DATA_MODE_MASK) != WSPI_CFG_DATA_MODE_NONE);

  osalSysLock();
  osalDbgAssert(drvGetCallbackX(self) == NULL, "has callback");
  wspiStartReceiveI(self, cmdp, n, rxbuf);
  msg = osalThreadSuspendS(&self->sync_transfer);
  osalSysUnlock();

  return (bool)(msg != MSG_OK);
}
#endif /* WSPI_USE_SYNCHRONIZATION == TRUE */

#if (WSPI_SUPPORTS_MEMMAP == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Maps in memory space a WSPI flash device.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 * @param[out]    addrp         Pointer to the mapped memory base address or @p
 *                              NULL.
 *
 * @iclass
 */
void wspiMapFlashI(void *ip, const wspi_command_t *cmdp, uint8_t **addrp) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (cmdp != NULL));
  osalDbgCheck((cmdp->cfg & WSPI_CFG_DATA_MODE_MASK) != WSPI_CFG_DATA_MODE_NONE);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = WSPI_STATE_MEMMAP;
  wspi_lld_map_flash(self, cmdp, addrp);
}

/**
 * @brief       Maps in memory space a WSPI flash device.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 * @param[in]     cmdp          Pointer to the WSPI command descriptor.
 * @param[out]    addrp         Pointer to the mapped memory base address or @p
 *                              NULL.
 *
 * @api
 */
void wspiMapFlash(void *ip, const wspi_command_t *cmdp, uint8_t **addrp) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheck((self != NULL) && (cmdp != NULL));

  osalSysLock();
  wspiMapFlashI(self, cmdp, addrp);
  osalSysUnlock();
}

/**
 * @brief       Unmaps a WSPI flash device from memory space.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 *
 * @iclass
 */
void wspiUnmapFlashI(void *ip) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == WSPI_STATE_MEMMAP, "not mapped");

  wspi_lld_unmap_flash(self);
  self->state = HAL_DRV_STATE_READY;
}

/**
 * @brief       Unmaps a WSPI flash device from memory space.
 *
 * @param[in,out] ip            Pointer to a @p hal_wspi_driver_c instance.
 *
 * @api
 */
void wspiUnmapFlash(void *ip) {
  hal_wspi_driver_c *self = (hal_wspi_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  wspiUnmapFlashI(self);
  osalSysUnlock();
}
#endif /* WSPI_SUPPORTS_MEMMAP == TRUE */
/** @} */

#endif /* HAL_USE_WSPI == TRUE */

/** @} */
