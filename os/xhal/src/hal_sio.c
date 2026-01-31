/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file        hal_sio.c
 * @brief       Generated SIO Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SIO
 * @{
 */

#include "hal.h"

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

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

#if (SIO_USE_STREAMS_INTERFACE == TRUE) || defined (__DOXYGEN__)
static size_t sio_sync_write(hal_sio_driver_c *siop, const uint8_t *bp,
                             size_t n, sysinterval_t timeout) {
  size_t i;

  i = 0U;
  while (i < n) {
    size_t written;
    msg_t msg;

    msg = sioSynchronizeTX(siop, timeout);
    if (msg != MSG_OK) {
      break;
    }

    written = sioAsyncWriteX(siop, bp, n - i);
    i += written;
    bp += written;
  }
  return i;
}

static size_t sio_sync_read(hal_sio_driver_c *siop, uint8_t *bp, size_t n,
                            sysinterval_t timeout) {
  size_t i;

  i = 0U;
  while (i < n) {
    size_t read;
    msg_t msg;

    msg = sioSynchronizeRX(siop, timeout);
    if (msg == SIO_MSG_ERRORS) {
      (void)sioGetAndClearErrors(siop);
    }
    else if (msg != MSG_OK) {
      break;
    }

    read = sioAsyncReadX(siop, bp, n - i);
    i += read;
    bp += read;
  }
  return i;
}
#endif /* SIO_USE_STREAMS_INTERFACE == TRUE */

#if (SIO_USE_BUFFERING == TRUE) || defined (__DOXYGEN__)
static void __bsio_push_data(hal_buffered_sio_c *bsiop) {

  while (!sioIsTXFullX(bsiop->siop)) {
    msg_t msg;

    msg = oqGetI(&bsiop->oqueue);
    if (msg < MSG_OK) {
      bsAddFlagsI(bsiop, CHN_FL_TX_NOTFULL);
      return;
    }
    sioPutX(bsiop->siop, (uint_fast16_t)msg);
  }
}

static void __bsio_pop_data(hal_buffered_sio_c *bsiop) {

  /* RX FIFO needs to be fully emptied or SIO will not generate more RX FIFO
     events.*/
  while (!sioIsRXEmptyX(bsiop->siop)) {
    bsIncomingDataI(bsiop, sioGetX(bsiop->siop));
  }
}

static void __bsio_default_cb(void *ip) {
  hal_sio_driver_c *siop = (hal_sio_driver_c *)ip;
  hal_buffered_sio_c *bsiop = (hal_buffered_sio_c *)siop->arg;
  sioevents_t events;

  osalSysLockFromISR();

  /* Posting the non-data SIO events as channel event flags, the masks are
     made to match.*/
  events = sioGetAndClearEventsX(siop, SIO_EV_ALL_EVENTS);
  bsAddFlagsI(bsiop, (eventflags_t)(events & ~SIO_EV_ALL_DATA));

  /* RX FIFO event.*/
  if ((events & SIO_EV_RX_NOTEMPTY) != (sioevents_t)0) {

    __bsio_pop_data(bsiop);
  }

  /* TX FIFO event.*/
  if ((events & SIO_EV_TX_NOTFULL) != (sioevents_t)0) {
     __bsio_push_data(bsiop);
  }

  osalSysUnlockFromISR();
}

static void __bsio_onotify(io_queue_t *qp) {

  __bsio_push_data((hal_buffered_sio_c *)qp->q_link);
}
#endif /* SIO_USE_BUFFERING == TRUE */

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       SIO Driver initialization.
 * @note        This function is implicitly invoked by @p halInit(), there is
 *              no need to explicitly initialize the driver.
 *
 * @init
 */
void sioInit(void) {

  sio_lld_init();
}

/*===========================================================================*/
/* Module class "hal_sio_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Interfaces implementation of hal_sio_driver_c
 * @{
 */
#if (SIO_USE_STREAMS_INTERFACE == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Implementation of interface method @p stmWrite().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
static size_t __sio_chn_write_impl(void *ip, const uint8_t *bp, size_t n) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);

  return sio_sync_write(self, bp, n, TIME_INFINITE);
}

/**
 * @brief       Implementation of interface method @p stmRead().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[out]    bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
static size_t __sio_chn_read_impl(void *ip, uint8_t *bp, size_t n) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);

  return sio_sync_read(self, bp, n, TIME_INFINITE);
}

/**
 * @brief       Implementation of interface method @p stmPut().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     b             The byte value to be written to the stream.
 * @return                      The operation status.
 */
static int __sio_chn_put_impl(void *ip, uint8_t b) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);
  msg_t msg;

  msg = sioSynchronizeTX(self, TIME_INFINITE);
  if (msg != MSG_OK) {
    return msg;
  }

  sioPutX(self, b);
  return MSG_OK;
}

/**
 * @brief       Implementation of interface method @p stmGet().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @return                      A byte value from the stream.
 */
static int __sio_chn_get_impl(void *ip) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);
  msg_t msg;

  while (true) {
    msg = sioSynchronizeRX(self, TIME_INFINITE);
    if (msg == SIO_MSG_ERRORS) {
      (void)sioGetAndClearErrors(self);
      continue;
    }
    if (msg != MSG_OK) {
      return msg;
    }

    return sioGetX(self);
  }
}

/**
 * @brief       Implementation of interface method @p stmUnget().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     b             The byte value to be pushed back to the stream.
 * @return                      The operation status.
 */
static int __sio_chn_unget_impl(void *ip, int b) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);

  (void)self;
  (void)b;

  return STM_RESET;
}

/**
 * @brief       Implementation of interface method @p chnWriteTimeout().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      The number of bytes transferred.
 */
static size_t __sio_chn_writet_impl(void *ip, const uint8_t *bp, size_t n,
                                    sysinterval_t timeout) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);

  return sio_sync_write(self, bp, n, timeout);
}

/**
 * @brief       Implementation of interface method @p chnReadTimeout().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      The number of bytes transferred.
 */
static size_t __sio_chn_readt_impl(void *ip, uint8_t *bp, size_t n,
                                   sysinterval_t timeout) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);

  return sio_sync_read(self, bp, n, timeout);
}

/**
 * @brief       Implementation of interface method @p chnPutTimeout().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     b             The byte value to be written to the channel.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      The operation status.
 */
static msg_t __sio_chn_putt_impl(void *ip, uint8_t b, sysinterval_t timeout) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);
  msg_t msg;

  msg = sioSynchronizeTX(self, timeout);
  if (msg != MSG_OK) {
    return msg;
  }

  sioPutX(self, b);
  return MSG_OK;
}

/**
 * @brief       Implementation of interface method @p chnGetTimeout().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      A byte value from the channel.
 */
static msg_t __sio_chn_gett_impl(void *ip, sysinterval_t timeout) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);
  msg_t msg;

  while (true) {
    msg = sioSynchronizeRX(self, timeout);
    if (msg == SIO_MSG_ERRORS) {
      (void)sioGetAndClearErrors(self);
      continue;
    }
    if (msg != MSG_OK) {
      return msg;
    }

    return sioGetX(self);
  }
}

/**
 * @brief       Implementation of interface method @p chnGetAndClearFlags().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     mask          Mask of flags to be returned and cleared.
 * @return                      The cleared event flags.
 */
static chnflags_t __sio_chn_getclr_impl(void *ip, chnflags_t mask) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);

  return (chnflags_t)sioGetAndClearEventsX(self, (sioevents_t)mask);
}

/**
 * @brief       Implementation of interface method @p chnControl().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     operation     Control operation code
 * @param[in,out] arg           Operation argument.
 * @return                      The operation status.
 */
static msg_t __sio_chn_ctl_impl(void *ip, unsigned int operation, void *arg) {
  hal_sio_driver_c *self = oopIfGetOwner(hal_sio_driver_c, ip);

  switch (operation) {
  case CHN_CTL_NOP:
    osalDbgCheck(arg == NULL);
    break;
  case CHN_CTL_INVALID:
    return HAL_RET_UNKNOWN_CTL;
  default:
    /* Delegating to the LLD if supported.*/
    return sio_lld_control(self, operation, arg);
  }
  return HAL_RET_SUCCESS;
}
#endif /* SIO_USE_STREAMS_INTERFACE == TRUE */
/** @} */

/**
 * @name        Methods implementations of hal_sio_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_sio_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__sio_objinit_impl(void *ip, const void *vmt) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

#if (SIO_USE_STREAMS_INTERFACE == TRUE) || defined (__DOXYGEN__)
  /* Initialization of interface asynchronous_channel_i.*/
  {
    static const struct asynchronous_channel_vmt sio_chn_vmt = {
      .instance_offset      = offsetof(hal_sio_driver_c, chn),
      .write                = __sio_chn_write_impl,
      .read                 = __sio_chn_read_impl,
      .put                  = __sio_chn_put_impl,
      .get                  = __sio_chn_get_impl,
      .unget                = __sio_chn_unget_impl,
      .writet               = __sio_chn_writet_impl,
      .readt                = __sio_chn_readt_impl,
      .putt                 = __sio_chn_putt_impl,
      .gett                 = __sio_chn_gett_impl,
      .getclr               = __sio_chn_getclr_impl,
      .ctl                  = __sio_chn_ctl_impl
    };
    oopIfObjectInit(&self->chn, &sio_chn_vmt);
  }
#endif /* SIO_USE_STREAMS_INTERFACE == TRUE */

  /* Initialization code.*/
  self->enabled     = (sioevents_t)0;
  self->cb          = NULL;
#if SIO_USE_SYNCHRONIZATION == TRUE
  self->sync_rx     = NULL;
  self->sync_rxidle = NULL;
  self->sync_tx     = NULL;
  self->sync_txend  = NULL;
#endif

  /* Optional, user-defined initializer.*/
#if defined(SIO_DRIVER_EXT_INIT_HOOK)
  SIO_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance to be
 *                              disposed.
 */
void __sio_dispose_impl(void *ip) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @return                      The operation status.
 */
msg_t __sio_start_impl(void *ip) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;
  msg_t msg;

  msg = sio_lld_start(self);
  if (msg == HAL_RET_SUCCESS) {
#if SIO_USE_SYNCHRONIZATION == TRUE
    /* If synchronization is enabled then all events by default.*/
    sioWriteEnableFlagsX(self, SIO_EV_ALL_EVENTS);
#else
    /* If synchronization is disabled then no events by default.*/
    sioWriteEnableFlagsX(self, SIO_EV_NONE);
#endif
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 */
void __sio_stop_impl(void *ip) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;

  sio_lld_stop(self);
  self->cb      = NULL;
  self->enabled = (sioevents_t)0;

#if SIO_USE_SYNCHRONIZATION == TRUE
  /* Informing waiting threads, if any.*/
  osalThreadResumeI(&self->sync_rx, MSG_RESET);
  osalThreadResumeI(&self->sync_rxidle, MSG_RESET);
  osalThreadResumeI(&self->sync_tx, MSG_RESET);
  osalThreadResumeI(&self->sync_txend, MSG_RESET);
  osalOsRescheduleS();
#endif
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__sio_setcfg_impl(void *ip, const void *config) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;

  return (const void *)sio_lld_setcfg(self, (const hal_sio_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__sio_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;

  return (const void *)sio_lld_selcfg(self, cfgnum);
}
/** @} */

/**
 * @brief       VMT structure of SIO driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_sio_driver_vmt __hal_sio_driver_vmt = {
  .dispose                  = __sio_dispose_impl,
  .start                    = __sio_start_impl,
  .stop                     = __sio_stop_impl,
  .setcfg                   = __sio_setcfg_impl,
  .selcfg                   = __sio_selcfg_impl,
  .setcb                    = __cbdrv_setcb_impl
};

/**
 * @name        Regular methods of hal_sio_driver_c
 * @{
 */
/**
 * @brief       Writes the enabled events flags mask.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param         mask          Enabled events mask to be written
 *
 * @api
 */
void sioWriteEnableFlags(void *ip, sioevents_t mask) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");
  sioWriteEnableFlagsX(self, mask);
  osalSysUnlock();
}

/**
 * @brief       Sets flags into the enabled events flags mask.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param         mask          Enabled events mask to be set
 *
 * @api
 */
void sioSetEnableFlags(void *ip, sioevents_t mask) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");
  sioSetEnableFlagsX(self, mask);
  osalSysUnlock();
}

/**
 * @brief       Clears flags from the enabled events flags mask.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param         mask          Enabled events mask to be cleared
 *
 * @api
 */
void sioClearEnableFlags(void *ip, sioevents_t mask) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");
  sioClearEnableFlagsX(self, mask);
  osalSysUnlock();
}

/**
 * @brief       Get and clears SIO error event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @return                      The pending error event flags.
 *
 * @api
 */
sioevents_t sioGetAndClearErrors(void *ip) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;
  sioevents_t errors;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");
  errors = sioGetAndClearErrorsX(self);
  osalSysUnlock();

  return errors;
}

/**
 * @brief       Get and clears SIO event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param[in]     mask          Mask of events to be returned and cleared.
 * @return                      The pending event flags.
 *
 * @api
 */
sioevents_t sioGetAndClearEvents(void *ip, sioevents_t mask) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;
  sioevents_t events;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");
  events = sioGetAndClearEventsX(self, mask);
  osalSysUnlock();

  return events;
}

/**
 * @brief       Returns the pending SIO event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @return                      The pending event flags.
 *
 * @api
 */
sioevents_t sioGetEvents(void *ip) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;
  sioevents_t events;

  osalDbgCheck(self != NULL);

  osalSysLock();
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");
  events = sioGetEventsX(self);
  osalSysUnlock();

  return events;
}

#if (SIO_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Synchronizes with RX FIFO data availability.
 * @note        The exact behavior depends on low level FIFO settings such as
 *              thresholds, etc.
 * @note        This function can only be called by a single thread at time.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param[in]     timeout       Synchronization timeout
 * @return                      The synchronization result.
 * @retval MSG_OK               If there is data in the RX FIFO.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            It the driver has been stopped while waiting.
 * @retval SIO_MSG_ERRORS       It RX errors occurred before or during wait.
 *
 * @api
 */
msg_t sioSynchronizeRX(void *ip, sysinterval_t timeout) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();

  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");

  /* Checking for errors before going to sleep.*/
  if (sioHasRXErrorsX(self)) {
    osalSysUnlock();
    return SIO_MSG_ERRORS;
  }

  msg = MSG_OK;
  /*lint -save -e506 -e681 [2.1] Silencing this error because it is
    tested with a template implementation of sio_lld_is_rx_empty() which
    is constant.*/
  while (sioIsRXEmptyX(self)) {
  /*lint -restore*/
    msg = osalThreadSuspendTimeoutS(&self->sync_rx, timeout);
    if (msg != MSG_OK) {
      break;
    }
  }

  osalSysUnlock();

  return msg;
}

/**
 * @brief       Synchronizes with RX going idle.
 * @note        This function can only be called by a single thread at time.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param[in]     timeout       Synchronization timeout
 * @return                      The synchronization result.
 * @retval MSG_OK               If there is data in the RX FIFO.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            It the driver has been stopped while waiting.
 * @retval SIO_MSG_ERRORS       It RX errors occurred before or during wait.
 *
 * @api
 */
msg_t sioSynchronizeRXIdle(void *ip, sysinterval_t timeout) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();

  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");

  /* Checking for errors before going to sleep.*/
  if (sioHasRXErrorsX(self)) {
    osalSysUnlock();
    return SIO_MSG_ERRORS;
  }

  msg = MSG_OK;
  /*lint -save -e506 -e681 [2.1] Silencing this error because it is
    tested with a template implementation of sio_lld_is_rx_empty() which
    is constant.*/
  while (!sioIsRXIdleX(self)) {
  /*lint -restore*/
    msg = osalThreadSuspendTimeoutS(&self->sync_rxidle, timeout);
    if (msg != MSG_OK) {
      break;
    }
  }

  osalSysUnlock();

  return msg;
}

/**
 * @brief       Synchronizes with TX FIFO space availability.
 * @note        The exact behavior depends on low level FIFO settings such as
 *              thresholds, etc.
 * @note        This function can only be called by a single thread at time.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param[in]     timeout       Synchronization timeout
 * @return                      The synchronization result.
 * @retval MSG_OK               If there is space in the TX FIFO.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            It the driver has been stopped while waiting.
 *
 * @api
 */
msg_t sioSynchronizeTX(void *ip, sysinterval_t timeout) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();

  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");

  msg = MSG_OK;
  /*lint -save -e506 -e681 [2.1] Silencing this error because it is
    tested with a template implementation of sio_lld_is_tx_full() which
    is constant.*/
  while (sioIsTXFullX(self)) {
  /*lint -restore*/
    msg = osalThreadSuspendTimeoutS(&self->sync_tx, timeout);
    if (msg != MSG_OK) {
      break;
    }
  }

  osalSysUnlock();

  return msg;
}

/**
 * @brief       Synchronizes with TX completion.
 * @note        This function can only be called by a single thread at time.
 *
 * @param[in,out] ip            Pointer to a @p hal_sio_driver_c instance.
 * @param[in]     timeout       Synchronization timeout
 * @return                      The synchronization result.
 * @retval MSG_OK               If there is space in the TX FIFO.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            It the driver has been stopped while waiting.
 *
 * @api
 */
msg_t sioSynchronizeTXEnd(void *ip, sysinterval_t timeout) {
  hal_sio_driver_c *self = (hal_sio_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();

  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");

  /*lint -save -e506 -e774 [2.1, 14.3] Silencing this error because
    it is tested with a template implementation of sio_lld_is_tx_ongoing()
    which is constant.*/
  if (sioIsTXOngoingX(self)) {
  /*lint -restore*/
    msg = osalThreadSuspendTimeoutS(&self->sync_txend, timeout);
  }
  else {
    msg = MSG_OK;
  }

  osalSysUnlock();

  return msg;
}
#endif /* SIO_USE_SYNCHRONIZATION == TRUE */
/** @} */

/*===========================================================================*/
/* Module class "hal_buffered_sio_c" methods.                                */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_buffered_sio_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_buffered_sio_c instance to
 *                              be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     siop          Pointer to the @p hal_sio_driver_c object.
 * @param[in]     ib            Pointer to the input buffer.
 * @param[in]     ibsize        Size of the input buffer.
 * @param[in]     ob            Pointer to the output buffer.
 * @param[in]     obsize        Size of the output buffer.
 * @return                      A new reference to the object.
 */
void *__bsio_objinit_impl(void *ip, const void *vmt, hal_sio_driver_c *siop,
                          uint8_t *ib, size_t ibsize, uint8_t *ob,
                          size_t obsize) {
  hal_buffered_sio_c *self = (hal_buffered_sio_c *)ip;

  /* Initialization code.*/
  __bs_objinit_impl((void *)self, vmt,
                    ib, ibsize, NULL, NULL,
                    ob, obsize, __bsio_onotify, (void *)self);
  drvSetArgumentX(siop, self);
  self->siop = siop;

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_buffered_sio_c instance to
 *                              be disposed.
 */
void __bsio_dispose_impl(void *ip) {
  hal_buffered_sio_c *self = (hal_buffered_sio_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __bs_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_buffered_sio_c instance.
 * @return                      The operation status.
 */
msg_t __bsio_start_impl(void *ip) {
  hal_buffered_sio_c *self = (hal_buffered_sio_c *)ip;
  msg_t msg;

  /* Starting the undelying SIO driver.*/
  msg = drvStartS(self->siop);
  if (msg == HAL_RET_SUCCESS) {
    drvSetArgumentX(self->siop, self);
    drvSetCallbackX(self->siop, &__bsio_default_cb);
    sioWriteEnableFlagsX(self->siop, SIO_EV_ALL_EVENTS);
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_buffered_sio_c instance.
 */
void __bsio_stop_impl(void *ip) {
  hal_buffered_sio_c *self = (hal_buffered_sio_c *)ip;

  drvStopS(self->siop);
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_buffered_sio_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__bsio_setcfg_impl(void *ip, const void *config) {
  hal_buffered_sio_c *self = (hal_buffered_sio_c *)ip;

  /* Configuring the underlying SIO driver.*/
  return __sio_setcfg_impl(self->siop, config);
}
/** @} */

/**
 * @brief       VMT structure of buffered SIO wrapper class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_buffered_sio_vmt __hal_buffered_sio_vmt = {
  .dispose                  = __bsio_dispose_impl,
  .start                    = __bsio_start_impl,
  .stop                     = __bsio_stop_impl,
  .setcfg                   = __bsio_setcfg_impl,
  .selcfg                   = NULL /* Method not found.*/
};

#endif /* HAL_USE_SIO == TRUE */

/** @} */
