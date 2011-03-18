/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    serial_usb.c
 * @brief   Serial over USB Driver code.
 *
 * @addtogroup SERIAL_USB
 * @{
 */

#include "ch.h"
#include "hal.h"

#include "usb_cdc.h"

#if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*
 * Current Line Coding.
 */
static cdc_linecoding_t linecoding = {
  {0x00, 0x96, 0x00, 0x00},             /* 38400.                           */
  LC_STOP_1, LC_PARITY_NONE, 8
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*
 * Interface implementation.
 */

static size_t writes(void *ip, const uint8_t *bp, size_t n) {

  return chOQWriteTimeout(&((SerialDriver *)ip)->oqueue, bp,
                          n, TIME_INFINITE);
}

static size_t reads(void *ip, uint8_t *bp, size_t n) {

  return chIQReadTimeout(&((SerialDriver *)ip)->iqueue, bp,
                         n, TIME_INFINITE);
}

static bool_t putwouldblock(void *ip) {

  return chOQIsFullI(&((SerialDriver *)ip)->oqueue);
}

static bool_t getwouldblock(void *ip) {

  return chIQIsEmptyI(&((SerialDriver *)ip)->iqueue);
}

static msg_t putt(void *ip, uint8_t b, systime_t timeout) {

  return chOQPutTimeout(&((SerialDriver *)ip)->oqueue, b, timeout);
}

static msg_t gett(void *ip, systime_t timeout) {

  return chIQGetTimeout(&((SerialDriver *)ip)->iqueue, timeout);
}

static size_t writet(void *ip, const uint8_t *bp, size_t n, systime_t time) {

  return chOQWriteTimeout(&((SerialDriver *)ip)->oqueue, bp, n, time);
}

static size_t readt(void *ip, uint8_t *bp, size_t n, systime_t time) {

  return chIQReadTimeout(&((SerialDriver *)ip)->iqueue, bp, n, time);
}

static ioflags_t getflags(void *ip) {
  _ch_get_and_clear_flags_impl(ip);
}

static const struct SerialUSBDriverVMT vmt = {
  writes, reads, putwouldblock, getwouldblock,
  putt, gett, writet, readt,
  getflags
};

/**
 * @brief   Notification of data removed from the input queue.
 */
static void inotify(GenericQueue *qp) {
  SerialUSBDriver *sdup = (SerialUSBDriver *)qp->q_wrptr;
  size_t n;

  /* Writes to the input queue can only happen when the queue has been
     emptied, then a whole packet is loaded in the queue.*/
  if (chIQIsEmptyI(&sdup->iqueue)) {

    n = usbReadPacketI(sdup->config->usbp, DATA_AVAILABLE_EP,
                       sdup->iqueue.q_buffer, SERIAL_USB_BUFFERS_SIZE);
    if (n != USB_ENDPOINT_BUSY) {
      sdup->iqueue.q_rdptr = sdup->iqueue.q_buffer;
      chSemSetCounterI(&sdup->iqueue.q_sem, n);
      chIOAddFlagsI(sdup, IO_INPUT_AVAILABLE);
    }
  }
}

/**
 * @brief   Notification of data inserted into the output queue.
 */
static void onotify(GenericQueue *qp) {
  SerialUSBDriver *sdup = (SerialUSBDriver *)qp->q_rdptr;
  size_t n;

  /* If there is any data in the output queue then it is sent within a
     single packet and the queue is emptied.*/
  n = usbWritePacketI(sdup->config->usbp, DATA_REQUEST_EP,
                      sdup->oqueue.q_buffer, chOQGetFullI(&sdup->oqueue));
  if (n != USB_ENDPOINT_BUSY) {
    sdup->oqueue.q_wrptr = sdup->oqueue.q_buffer;
    chSemSetCounterI(&sdup->oqueue.q_sem, SERIAL_USB_BUFFERS_SIZE);
    chIOAddFlagsI(sdup, IO_OUTPUT_EMPTY);
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Serial Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void sduInit(void) {
}

/**
 * @brief   Initializes a generic full duplex driver object.
 * @details The HW dependent part of the initialization has to be performed
 *          outside, usually in the hardware initialization code.
 *
 * @param[out] sdup     pointer to a @p SerialUSBDriver structure
 *
 * @init
 */
void sduObjectInit(SerialUSBDriver *sdup) {

  sdup->vmt = &vmt;
  chEvtInit(&sdup->event);
  sdup->flags = IO_NO_ERROR;
  sdup->state = SDU_STOP;
  chIQInit(&sdup->iqueue, sdup->ib, SERIAL_USB_BUFFERS_SIZE, inotify);
  chOQInit(&sdup->oqueue, sdup->ob, SERIAL_USB_BUFFERS_SIZE, onotify);
  /* This is a dirty trick but those pointers are never used because queues
     are accessed in block mode from the low level.*/
  sdup->iqueue.q_wrptr = (uint8_t *)sdup;
  sdup->oqueue.q_rdptr = (uint8_t *)sdup;
}

/**
 * @brief   Configures and starts the driver.
 *
 * @param[in] sdup      pointer to a @p SerialUSBDriver object
 * @param[in] config    the serial over USB driver configuration
 *
 * @api
 */
void sduStart(SerialUSBDriver *sdup, const SerialUSBConfig *config) {

  chDbgCheck(sdup != NULL, "sduStart");

  chSysLock();
  chDbgAssert((sdup->state == SDU_STOP) || (sdup->state == SDU_READY),
              "sduStart(), #1",
              "invalid state");
  sdup->config = config;
  usbStart(config->usbp, &config->usb_config);
  config->usbp->param = sdup;
  sdup->state = SDU_READY;
  chSysUnlock();
}

/**
 * @brief   Stops the driver.
 * @details Any thread waiting on the driver's queues will be awakened with
 *          the message @p Q_RESET.
 *
 * @param[in] sdup      pointer to a @p SerialUSBDriver object
 *
 * @api
 */
void sduStop(SerialUSBDriver *sdup) {

  chDbgCheck(sdup != NULL, "sdStop");

  chSysLock();
  chDbgAssert((sdup->state == SDU_STOP) || (sdup->state == SDU_READY),
              "sduStop(), #1",
              "invalid state");
  usbStop(sdup->config->usbp);
  sdup->state = SDU_STOP;
  chSysUnlock();
}

/**
 * @brief   Default requests hook.
 * @details Applications wanting to use the Serial over USB driver can use
 *          this function as requests hook in the USB configuration.
 *          The following requests are emulated:
 *          - CDC_GET_LINE_CODING.
 *          - CDC_SET_LINE_CODING.
 *          - CDC_SET_CONTROL_LINE_STATE.
 *          .
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @return              The hook status.
 * @retval TRUE         Message handled internally.
 * @retval FALSE        Message not handled.
 */
bool_t sduRequestsHook(USBDriver *usbp) {

  if ((usbp->setup[0] & USB_RTYPE_TYPE_MASK) == USB_RTYPE_TYPE_CLASS) {
    switch (usbp->setup[1]) {
    case CDC_GET_LINE_CODING:
      usbSetupTransfer(usbp, (uint8_t *)&linecoding, sizeof(linecoding), NULL);
      return TRUE;
    case CDC_SET_LINE_CODING:
      usbSetupTransfer(usbp, (uint8_t *)&linecoding, sizeof(linecoding), NULL);
      return TRUE;
    case CDC_SET_CONTROL_LINE_STATE:
      /* Nothing to do, there are no control lines.*/
      usbSetupTransfer(usbp, NULL, 0, NULL);
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}

/**
 * @brief   Default data transmitted callback.
 * @details The application must use this function as callback for the IN
 *          data endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 */
void sduDataTransmitted(USBDriver *usbp, usbep_t ep) {
  SerialUSBDriver *sdup = usbp->param;
  size_t n;

  chSysLockFromIsr();
  /* If there is any data in the output queue then it is sent within a
     single packet and the queue is emptied.*/
  n = chOQGetFullI(&sdup->oqueue);
  if (n > 0) {
    n = usbWritePacketI(usbp, ep, sdup->oqueue.q_buffer, n);
    if (n != USB_ENDPOINT_BUSY) {
      sdup->oqueue.q_wrptr = sdup->oqueue.q_buffer;
      chSemSetCounterI(&sdup->oqueue.q_sem, SERIAL_USB_BUFFERS_SIZE);
      chIOAddFlagsI(sdup, IO_OUTPUT_EMPTY);
    }
  }
  chSysUnlockFromIsr();
}

/**
 * @brief   Default data received callback.
 * @details The application must use this function as callback for the OUT
 *          data endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 */
void sduDataReceived(USBDriver *usbp, usbep_t ep) {
  SerialUSBDriver *sdup = usbp->param;

  chSysLockFromIsr();
  /* Writes to the input queue can only happen when the queue has been
     emptied, then a whole packet is loaded in the queue.*/
  if (chIQIsEmptyI(&sdup->iqueue)) {
    size_t n;

    n = usbReadPacketI(usbp, ep, sdup->iqueue.q_buffer,
                       SERIAL_USB_BUFFERS_SIZE);
    if (n != USB_ENDPOINT_BUSY) {
      sdup->iqueue.q_rdptr = sdup->iqueue.q_buffer;
      chSemSetCounterI(&sdup->iqueue.q_sem, n);
      chIOAddFlagsI(sdup, IO_INPUT_AVAILABLE);
    }
  }
  chSysUnlockFromIsr();
}

/**
 * @brief   Default data received callback.
 * @details The application must use this function as callback for the IN
 *          interrupt endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 */
void sduInterruptTransmitted(USBDriver *usbp, usbep_t ep) {

  (void)usbp;
  (void)ep;
}

#endif /* HAL_USE_SERIAL */

/** @} */
