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
 * @file        hal_can.c
 * @brief       Generated CAN Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_CAN
 * @{
 */

#include "hal.h"

#if (HAL_USE_CAN == TRUE) || defined(__DOXYGEN__)

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
 * @brief       CAN Driver initialization.
 *
 * @init
 */
void canInit(void) {
  can_lld_init();
}

/*===========================================================================*/
/* Module class "hal_can_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_can_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_can_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__can_objinit_impl(void *ip, const void *vmt) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->events          = 0U;
  self->rx_mailbox_mask = 0U;
  self->tx_mailbox_mask = 0U;
  self->tx_error_mask   = 0U;
  self->errors          = 0U;
#if CAN_USE_SYNCHRONIZATION == TRUE
  osalThreadQueueObjectInit(&self->txqueue);
  osalThreadQueueObjectInit(&self->rxqueue);
#endif

#if defined(CAN_DRIVER_EXT_INIT_HOOK)
  CAN_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance to be
 *                              disposed.
 */
void __can_dispose_impl(void *ip) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __can_start_impl(void *ip, const void *config) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  msg_t msg;

  if (config != NULL) {
    self->config = __can_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  msg = can_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
    self->events = 0U;
    self->rx_mailbox_mask = 0U;
    self->tx_mailbox_mask = 0U;
    self->tx_error_mask = 0U;
    self->errors = 0U;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 */
void __can_stop_impl(void *ip) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  can_lld_stop(self);
  can_lld_reset(self);
#if CAN_USE_SYNCHRONIZATION == TRUE
  osalThreadDequeueAllI(&self->rxqueue, MSG_RESET);
  osalThreadDequeueAllI(&self->txqueue, MSG_RESET);
  osalOsRescheduleS();
#endif
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__can_setcfg_impl(void *ip, const void *config) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  return (const void *)can_lld_setcfg(self,
                                      (const hal_can_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__can_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  return (const void *)can_lld_selcfg(self, cfgnum);
}

/**
 * @brief       Override of method @p __cbdrv_on_cb_set().
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __can_oncbset_impl(void *ip, drv_cb_t cb) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  can_lld_set_callback(self, cb);
}
/** @} */

/**
 * @brief       VMT structure of CAN driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_can_driver_vmt __hal_can_driver_vmt = {
  .dispose                  = __can_dispose_impl,
  .start                    = __can_start_impl,
  .stop                     = __can_stop_impl,
  .setcfg                   = __can_setcfg_impl,
  .selcfg                   = __can_selcfg_impl,
  .oncbset                  = __can_oncbset_impl
};

/**
 * @name        Regular methods of hal_can_driver_c
 * @{
 */
/**
 * @brief       Attempts to queue a CAN frame for transmission.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param[in]     mailbox       Mailbox number or @p CAN_ANY_MAILBOX.
 * @param[in]     ctfp          Frame to transmit.
 * @return                      The operation result.
 * @retval false                If the frame has been queued.
 * @retval true                 If the mailbox is full.
 *
 * @iclass
 */
bool canTryTransmitI(void *ip, canmbx_t mailbox, const CANTxFrame *ctfp) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (ctfp != NULL) &&
               (mailbox <= (canmbx_t)CAN_TX_MAILBOXES));
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == CAN_SLEEP),
                "invalid state");

  if (!can_lld_is_tx_empty(self, mailbox)) {
    return true;
  }

  can_lld_transmit(self, mailbox, ctfp);

  return false;
}

/**
 * @brief       Attempts to fetch a CAN frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param[in]     mailbox       Mailbox number or @p CAN_ANY_MAILBOX.
 * @param[out]    crfp          Receive buffer.
 * @return                      The operation result.
 * @retval false                If a frame has been fetched.
 * @retval true                 If the mailbox is empty.
 *
 * @iclass
 */
bool canTryReceiveI(void *ip, canmbx_t mailbox, CANRxFrame *crfp) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (crfp != NULL) &&
               (mailbox <= (canmbx_t)CAN_RX_MAILBOXES));
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == CAN_SLEEP),
                "invalid state");

  if (!can_lld_is_rx_nonempty(self, mailbox)) {
    return true;
  }

  can_lld_receive(self, mailbox, crfp);

  return false;
}

/**
 * @brief       Attempts to abort an ongoing transmission.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param[in]     mailbox       Mailbox number.
 *
 * @xclass
 */
void canTryAbortX(void *ip, canmbx_t mailbox) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  osalDbgCheck((self != NULL) &&
               (mailbox != CAN_ANY_MAILBOX) &&
               (mailbox <= (canmbx_t)CAN_TX_MAILBOXES));

  can_lld_abort(self, mailbox);
}

#if (CAN_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Queues a CAN frame for transmission.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param[in]     mailbox       Mailbox number or @p CAN_ANY_MAILBOX.
 * @param[in]     ctfp          Frame to transmit.
 * @param[in]     timeout       Timeout interval.
 * @return                      The operation status.
 *
 * @api
 */
msg_t canTransmitTimeout(void *ip, canmbx_t mailbox, const CANTxFrame *ctfp,
                         sysinterval_t timeout) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  osalDbgCheck((self != NULL) && (ctfp != NULL) &&
               (mailbox <= (canmbx_t)CAN_TX_MAILBOXES));

  osalSysLock();
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == CAN_SLEEP),
                "invalid state");

  while ((self->state == CAN_SLEEP) || !can_lld_is_tx_empty(self, mailbox)) {
    msg_t msg = osalThreadEnqueueTimeoutS(&self->txqueue, timeout);
    if (msg != MSG_OK) {
      osalSysUnlock();
      return msg;
    }
  }

  can_lld_transmit(self, mailbox, ctfp);
  osalSysUnlock();

  return MSG_OK;
}

/**
 * @brief       Receives a CAN frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param[in]     mailbox       Mailbox number or @p CAN_ANY_MAILBOX.
 * @param[out]    crfp          Receive buffer.
 * @param[in]     timeout       Timeout interval.
 * @return                      The operation status.
 *
 * @api
 */
msg_t canReceiveTimeout(void *ip, canmbx_t mailbox, CANRxFrame *crfp,
                        sysinterval_t timeout) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  osalDbgCheck((self != NULL) && (crfp != NULL) &&
               (mailbox <= (canmbx_t)CAN_RX_MAILBOXES));

  osalSysLock();
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == CAN_SLEEP),
                "invalid state");

  while ((self->state == CAN_SLEEP) || !can_lld_is_rx_nonempty(self, mailbox)) {
    msg_t msg = osalThreadEnqueueTimeoutS(&self->rxqueue, timeout);
    if (msg != MSG_OK) {
      osalSysUnlock();
      return msg;
    }
  }

  can_lld_receive(self, mailbox, crfp);
  osalSysUnlock();

  return MSG_OK;
}
#endif /* CAN_USE_SYNCHRONIZATION == TRUE */

#if (CAN_USE_SLEEP_MODE == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Enters CAN sleep mode.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 *
 * @api
 */
void canSleep(void *ip) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == CAN_SLEEP),
                "invalid state");
  if (self->state == HAL_DRV_STATE_READY) {
    can_lld_sleep(self);
    self->state = CAN_SLEEP;
    self->events |= CAN_EVENT_SLEEP;
  }
  osalSysUnlock();
}

/**
 * @brief       Forces CAN wakeup from sleep mode.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 *
 * @api
 */
void canWakeup(void *ip) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == CAN_SLEEP),
                "invalid state");
  if (self->state == CAN_SLEEP) {
    can_lld_wakeup(self);
    self->state = HAL_DRV_STATE_READY;
    self->events |= CAN_EVENT_WAKEUP;
  }
  osalSysUnlock();
}
#endif /* CAN_USE_SLEEP_MODE == TRUE */
/** @} */

#endif /* HAL_USE_CAN == TRUE */

/** @} */
