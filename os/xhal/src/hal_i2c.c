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
 * @file        hal_i2c.c
 * @brief       Generated I2C Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_I2C
 * @{
 */

#include "hal.h"

#if (HAL_USE_I2C == TRUE) || defined(__DOXYGEN__)

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
 * @brief       I2C Driver initialization.
 *
 * @init
 */
void i2cInit(void) {
  i2c_lld_init();
}

/*===========================================================================*/
/* Module class "hal_i2c_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_i2c_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_i2c_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__i2c_objinit_impl(void *ip, const void *vmt) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->errors = I2C_NO_ERROR;
#if I2C_USE_SYNCHRONIZATION == TRUE
  self->sync_transfer = NULL;
#endif
#if defined(I2C_DRIVER_EXT_INIT_HOOK)
  I2C_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance to be
 *                              disposed.
 */
void __i2c_dispose_impl(void *ip) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __i2c_start_impl(void *ip, const void *config) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  msg_t msg;

  if (config != NULL) {
    self->config = __i2c_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  msg = i2c_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
    self->errors = I2C_NO_ERROR;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 */
void __i2c_stop_impl(void *ip) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  i2c_lld_stop(self);
  self->errors = I2C_NO_ERROR;
#if I2C_USE_SYNCHRONIZATION == TRUE
  osalThreadResumeI(&self->sync_transfer, MSG_RESET);
#endif
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__i2c_setcfg_impl(void *ip, const void *config) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  return (const void *)i2c_lld_setcfg(self,
                                      (const hal_i2c_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__i2c_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  return (const void *)i2c_lld_selcfg(self, cfgnum);
}

/**
 * @brief       Override of method @p __cbdrv_on_cb_set().
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __i2c_oncbset_impl(void *ip, drv_cb_t cb) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  i2c_lld_set_callback(self, cb);
}
/** @} */

/**
 * @brief       VMT structure of I2C driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_i2c_driver_vmt __hal_i2c_driver_vmt = {
  .dispose                  = __i2c_dispose_impl,
  .start                    = __i2c_start_impl,
  .stop                     = __i2c_stop_impl,
  .setcfg                   = __i2c_setcfg_impl,
  .selcfg                   = __i2c_selcfg_impl,
  .oncbset                  = __i2c_oncbset_impl
};

/**
 * @name        Regular methods of hal_i2c_driver_c
 * @{
 */
/**
 * @brief       Starts an I2C master transmit transaction.
 * @details     When @p rxbytes is greater than zero, a receive phase follows
 *              the transmit phase using a repeated start.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     addr          Slave address.
 * @param[in]     txbuf         Transmit buffer.
 * @param[in]     txbytes       Number of bytes to transmit.
 * @param[out]    rxbuf         Receive buffer.
 * @param[in]     rxbytes       Number of bytes to receive.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t i2cStartMasterTransmitI(void *ip, i2caddr_t addr, const uint8_t *txbuf,
                              size_t txbytes, uint8_t *rxbuf, size_t rxbytes) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) &&
               (txbytes > 0U) && (txbuf != NULL) &&
               ((rxbytes == 0U) || ((rxbytes > 0U) && (rxbuf != NULL))));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->errors = I2C_NO_ERROR;
  self->state  = HAL_DRV_STATE_ACTIVE;
  msg = i2c_lld_start_master_transmit(self, addr, txbuf, txbytes,
                                      rxbuf, rxbytes);
  if (msg != HAL_RET_SUCCESS) {
    self->state = HAL_DRV_STATE_READY;
  }

  return msg;
}

/**
 * @brief       Starts an I2C master receive transaction.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     addr          Slave address.
 * @param[out]    rxbuf         Receive buffer.
 * @param[in]     rxbytes       Number of bytes to receive.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t i2cStartMasterReceiveI(void *ip, i2caddr_t addr, uint8_t *rxbuf,
                             size_t rxbytes) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (addr != 0U) &&
               (rxbytes > 0U) && (rxbuf != NULL));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->errors = I2C_NO_ERROR;
  self->state  = I2C_ACTIVE_RX;
  msg = i2c_lld_start_master_receive(self, addr, rxbuf, rxbytes);
  if (msg != HAL_RET_SUCCESS) {
    self->state = HAL_DRV_STATE_READY;
  }

  return msg;
}

/**
 * @brief       Stops an ongoing I2C master transaction.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t i2cStopTransferI(void *ip) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  msg_t msg = HAL_RET_SUCCESS;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == I2C_ACTIVE_RX),
                "invalid state");

  if ((self->state == HAL_DRV_STATE_ACTIVE) ||
      (self->state == I2C_ACTIVE_RX)) {
    msg = i2c_lld_stop_transfer(self);
    self->state = HAL_DRV_STATE_READY;
#if I2C_USE_SYNCHRONIZATION == TRUE
    osalThreadResumeI(&self->sync_transfer, MSG_RESET);
#endif
  }

  return msg;
}

/**
 * @brief       Stops an ongoing I2C master transaction.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @return                      The operation status.
 *
 * @api
 */
msg_t i2cStopTransfer(void *ip) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = i2cStopTransferI(self);
  osalOsRescheduleS();
  osalSysUnlock();

  return msg;
}

#if (I2C_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Waits for I2C transfer completion.
 * @note        This function can only be called by a single thread at time.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 *
 * @sclass
 */
msg_t i2cSynchronizeS(void *ip, sysinterval_t timeout) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == I2C_ACTIVE_RX) ||
                (self->state == HAL_DRV_STATE_READY),
                "invalid state");

  if ((self->state == HAL_DRV_STATE_ACTIVE) ||
      (self->state == I2C_ACTIVE_RX)) {
    msg = osalThreadSuspendTimeoutS(&self->sync_transfer, timeout);
    if (msg == MSG_TIMEOUT) {
      (void)i2c_lld_stop_transfer(self);
      self->state = I2C_LOCKED;
    }
  }
  else {
    msg = MSG_OK;
  }

  return msg;
}

/**
 * @brief       Waits for I2C transfer completion.
 * @note        This function can only be called by a single thread at time.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 *
 * @api
 */
msg_t i2cSynchronize(void *ip, sysinterval_t timeout) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = i2cSynchronizeS(self, timeout);
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Performs an I2C master transmit transaction.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     addr          Slave address.
 * @param[in]     txbuf         Transmit buffer.
 * @param[in]     txbytes       Number of bytes to transmit.
 * @param[out]    rxbuf         Receive buffer.
 * @param[in]     rxbytes       Number of bytes to receive.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The operation status.
 *
 * @api
 */
msg_t i2cMasterTransmitTimeout(void *ip, i2caddr_t addr, const uint8_t *txbuf,
                               size_t txbytes, uint8_t *rxbuf, size_t rxbytes,
                               sysinterval_t timeout) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = i2cStartMasterTransmitI(self, addr, txbuf, txbytes, rxbuf, rxbytes);
  if (msg == HAL_RET_SUCCESS) {
    msg = i2cSynchronizeS(self, timeout);
  }
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Performs an I2C master receive transaction.
 *
 * @param[in,out] ip            Pointer to a @p hal_i2c_driver_c instance.
 * @param[in]     addr          Slave address.
 * @param[out]    rxbuf         Receive buffer.
 * @param[in]     rxbytes       Number of bytes to receive.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The operation status.
 *
 * @api
 */
msg_t i2cMasterReceiveTimeout(void *ip, i2caddr_t addr, uint8_t *rxbuf,
                              size_t rxbytes, sysinterval_t timeout) {
  hal_i2c_driver_c *self = (hal_i2c_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = i2cStartMasterReceiveI(self, addr, rxbuf, rxbytes);
  if (msg == HAL_RET_SUCCESS) {
    msg = i2cSynchronizeS(self, timeout);
  }
  osalSysUnlock();

  return msg;
}
#endif /* I2C_USE_SYNCHRONIZATION == TRUE */
/** @} */

#endif /* HAL_USE_I2C == TRUE */

/** @} */
