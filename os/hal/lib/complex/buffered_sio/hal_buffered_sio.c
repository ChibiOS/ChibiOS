/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    hal_buffered_sio.c
 * @brief   Buffered SIO Driver code.
 *
 * @addtogroup HAL_BUFFERED_SIO
 * @{
 */

#include "hal.h"

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

#include "hal_buffered_sio.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static void bs_default_cb(SIODriver *siop);
static const SIOOperation bs_default_operation = {
  .cb = bs_default_cb
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void bs_default_cb(SIODriver *siop) {
  BufferedSIODriver *bsdp = (BufferedSIODriver *)siop->arg;
  sioevents_t events;

  osalSysLockFromISR();

  /* Posting the non-data SIO events as channel event flags, the masks are
     made to match.*/
  events = sioGetAndClearEventsI(siop);
  chnAddFlagsI(bsdp, (eventflags_t)(events & ~SIO_EV_ALL_DATA));

  /* RX FIFO event.*/
  if ((events & SIO_EV_RXNOTEMPY) != (sioevents_t)0) {
    /* RX FIFO needs to be emptied or SIO will not generate more RX FIFO
       events.*/
    while (!sioIsRXEmptyX(siop)) {
      bsIncomingDataI((BufferedSerial *)bsdp, sioGetX(siop));
    }
  }

  /* TX FIFO event.*/
  if ((events & SIO_EV_TXNOTFULL) != (sioevents_t)0) {
    while (!sioIsTXFullX(siop)) {
      msg_t msg;

      msg = oqGetI(&bsdp->oqueue);
      if (msg < MSG_OK) {
        chnAddFlagsI((BufferedSerial *)bsdp, CHN_OUTPUT_EMPTY);
        break;
      }
      sioPutX(siop, (uint_fast16_t)msg);
    }
  }

  osalSysUnlockFromISR();
}

/*
 * Interface implementation.
 */
static size_t _write(void *ip, const uint8_t *bp, size_t n) {

  return __buffered_serial_write_impl(ip, bp, n);
}

static size_t _read(void *ip, uint8_t *bp, size_t n) {

  return __buffered_serial_read_impl(ip, bp, n);
}

static msg_t _put(void *ip, uint8_t b) {

  return __buffered_serial_put_impl(ip, b);
}

static msg_t _get(void *ip) {

  return __buffered_serial_get_impl(ip);
}

static msg_t _putt(void *ip, uint8_t b, sysinterval_t timeout) {

  return __buffered_serial_put_timeout_impl(ip, b, timeout);
}

static msg_t _gett(void *ip, sysinterval_t timeout) {

  return __buffered_serial_get_timeout_impl(ip, timeout);
}

static size_t _writet(void *ip, const uint8_t *bp, size_t n,
                      sysinterval_t timeout) {

  return __buffered_serial_write_timeout_impl(ip, bp, n, timeout);
}

static size_t _readt(void *ip, uint8_t *bp, size_t n,
                     sysinterval_t timeout) {

  return __buffered_serial_read_timeout_impl(ip, bp, n, timeout);
}

static msg_t _ctl(void *ip, unsigned int operation, void *arg) {

  /* TODO call SIO control first.*/
  return __buffered_serial_ctl_impl(ip, operation, arg);
}

static const struct BufferedSIODriverVMT vmt = {
  (size_t)0,
  _write, _read, _put, _get,
  _putt, _gett, _writet, _readt,
  _ctl
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes a generic serial driver object.
 * @details The HW dependent part of the initialization has to be performed
 *          outside, usually in the hardware initialization code.
 *
 * @param[out] bsdp     pointer to a @p BufferedSIODriver structure
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] ib        pointer to the input buffer
 * @param[in] ibsize    size of the input buffer
 * @param[in] ob        pointer to the output buffer
 * @param[in] obsize    size of the output buffer
 *
 * @init
 */
void bsdObjectInit(BufferedSIODriver *bsdp, SIODriver *siop,
                   uint8_t *ib, size_t ibsize,
                   uint8_t *ob, size_t obsize) {

  __buffered_serial_objinit_impl((void *)bsdp, (const void *)&vmt,
                                 ib, ibsize, NULL, NULL,
                                 ob, obsize, NULL, NULL);
  bsdp->siop = siop;
  siop->arg  = (void *)bsdp;
}

/**
 * @brief   Configures and starts the driver.
 *
 * @param[out] bsdp     pointer to a @p BufferedSIODriver structure
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 * @return              The operation status.
 *
 * @api
 */
msg_t bsdStart(BufferedSIODriver *bsdp, const BufferedSIOConfig *config) {
  msg_t msg;

  osalDbgCheck(bsdp != NULL);

  osalDbgAssert((bsdp->state == BS_STOP) || (bsdp->state == BS_READY),
                "invalid state");

  /* Stopping current operation, if any.*/
  if (bsdp->siop->state == SIO_ACTIVE) {
    sioStopOperation(bsdp->siop);
  }

  msg = sioStart(bsdp->siop, config);
  if (msg == HAL_RET_SUCCESS) {
    osalSysLock();
    sioStartOperationI(bsdp->siop, &bs_default_operation);
    sioWriteEnableFlagsI(bsdp->siop, SIO_FL_ALL);
    bsdp->state = BS_READY;
    osalSysUnlock();
  }
  else {
    bsdp->state = BS_STOP;
  }

  return msg;
}

/**
 * @brief   Stops the driver.
 * @details Any thread waiting on the driver's queues will be awakened with
 *          the message @p MSG_RESET.
 *
 * @param[out] bsdp     pointer to a @p BufferedSIODriver structure
 *
 * @api
 */
void bsdStop(BufferedSIODriver *bsdp) {

  osalDbgCheck(bsdp != NULL);

  osalDbgAssert((bsdp->state == BS_STOP) || (bsdp->state == BS_READY),
                "invalid state");

  /* Stopping current operation, if any.*/
  sioStopOperation(bsdp->siop);

  /* Stopping undelying SIO driver.*/
  sioStop(bsdp->siop);

  bsdp->state = BS_STOP;

  osalSysLock();
  oqResetI(&bsdp->oqueue); /* TODO should go in the upper class.*/
  iqResetI(&bsdp->iqueue);
  osalOsRescheduleS();
  osalSysUnlock();
}

#endif /* HAL_USE_SIO == TRUE */

/** @} */
