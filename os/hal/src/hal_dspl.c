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
 * @file    hal_dspl.c
 * @brief   Display Driver code.
 *
 * @addtogroup DISPLAY
 * @{
 */

#include "hal.h"
#include "hal_dspl.h"

#if (HAL_USE_DISPLAY == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static msg_t dspl_start_write_s(DSPLDriver *dsplp,
                                const DSPLArea *areap,
                                const void *buffer,
                                size_t stride) {
  msg_t msg;

  osalDbgAssert(dsplp->state == DSPL_READY, "not ready");

  dsplp->lastmsg = HAL_RET_SUCCESS;
  dsplp->state   = DSPL_WRITE;
  msg = dspl_lld_start_write(dsplp, areap, buffer, stride);
  if (msg != HAL_RET_SUCCESS) {
    dsplp->lastmsg = msg;
    dsplp->state   = DSPL_READY;
  }

  return msg;
}

static msg_t dspl_start_present_s(DSPLDriver *dsplp, void *buffer) {
  msg_t msg;

  osalDbgAssert(dsplp->state == DSPL_READY, "not ready");

  dsplp->lastmsg = HAL_RET_SUCCESS;
  dsplp->state   = DSPL_PRESENT;
  msg = dspl_lld_start_present(dsplp, buffer);
  if (msg != HAL_RET_SUCCESS) {
    dsplp->lastmsg = msg;
    dsplp->state   = DSPL_READY;
  }

  return msg;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Display Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void dsplInit(void) {

  dspl_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p DSPLDriver structure.
 *
 * @param[out] dsplp    pointer to the @p DSPLDriver object
 *
 * @init
 */
void dsplObjectInit(DSPLDriver *dsplp) {

  dsplp->state     = DSPL_STOP;
  dsplp->config    = NULL;
  dsplp->rotation  = DSPL_ROTATION_0;
  dsplp->lastevents = DSPL_EVT_NONE;
  dsplp->lastmsg   = HAL_RET_SUCCESS;
  dsplp->cb        = NULL;
  dsplp->arg       = NULL;
#if DSPL_USE_WAIT == TRUE
  dsplp->sync_op   = NULL;
  dsplp->sync_vsync = NULL;
#endif
#if DSPL_USE_EVENTS == TRUE
  osalEventObjectInit(&dsplp->event);
#endif
#if defined(DISPLAY_DRIVER_EXT_INIT_HOOK)
  DISPLAY_DRIVER_EXT_INIT_HOOK(dsplp);
#endif
}

/**
 * @brief   Configures and activates the display peripheral.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @param[in] config    pointer to the @p DSPLConfig object
 * @return              The operation status.
 *
 * @api
 */
msg_t dsplStart(DSPLDriver *dsplp, const DSPLConfig *config) {
  msg_t msg;

  osalDbgCheck((dsplp != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((dsplp->state == DSPL_STOP) || (dsplp->state == DSPL_READY),
                "invalid state");

  dsplp->config     = config;
  dsplp->rotation   = config->rotation;
  dsplp->lastevents = DSPL_EVT_NONE;
  dsplp->lastmsg    = HAL_RET_SUCCESS;

  msg = dspl_lld_start(dsplp);
  if (msg == HAL_RET_SUCCESS) {
    dsplp->state = DSPL_READY;
  }
  else {
    dsplp->config = NULL;
    dsplp->state  = DSPL_STOP;
  }

  osalSysUnlock();

  return msg;
}

/**
 * @brief   Deactivates the display peripheral.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 *
 * @api
 */
void dsplStop(DSPLDriver *dsplp) {

  osalDbgCheck(dsplp != NULL);

  osalSysLock();
  osalDbgAssert((dsplp->state == DSPL_STOP)    ||
                (dsplp->state == DSPL_READY)   ||
                (dsplp->state == DSPL_COMPLETE) ||
                (dsplp->state == DSPL_ERROR),
                "invalid state");

  dspl_lld_stop(dsplp);
  dsplp->config     = NULL;
  dsplp->rotation   = DSPL_ROTATION_0;
  dsplp->lastevents = DSPL_EVT_NONE;
  dsplp->lastmsg    = MSG_RESET;
  dsplp->state      = DSPL_STOP;
#if DSPL_USE_WAIT == TRUE
  osalThreadResumeI(&dsplp->sync_op, MSG_RESET);
  osalThreadResumeI(&dsplp->sync_vsync, MSG_RESET);
  osalOsRescheduleS();
#endif
  osalSysUnlock();
}

/**
 * @brief   Returns the descriptor of a display device.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @return              Pointer to the display descriptor.
 *
 * @api
 */
const DSPLDescriptor *dsplGetDescriptorX(DSPLDriver *dsplp) {

  osalDbgCheck(dsplp != NULL);
  osalDbgAssert(dsplp->state != DSPL_UNINIT, "not initialized");

  return dspl_lld_get_descriptor(dsplp);
}

/**
 * @brief   Applies a rotation to the display device.
 *
 * @param[in] dsplp       pointer to the @p DSPLDriver object
 * @param[in] rotation    selected rotation
 * @return                The operation status.
 *
 * @api
 */
msg_t dsplSetRotation(DSPLDriver *dsplp, dsplrotation_t rotation) {
  msg_t msg;

  osalDbgCheck(dsplp != NULL);

  osalSysLock();
  osalDbgAssert(dsplp->state == DSPL_READY, "not ready");

  msg = dspl_lld_set_rotation(dsplp, rotation);
  if (msg == HAL_RET_SUCCESS) {
    dsplp->rotation = rotation;
  }

  osalSysUnlock();

  return msg;
}

/**
 * @brief   Adjusts an area according to implementation constraints.
 *
 * @param[in] dsplp       pointer to the @p DSPLDriver object
 * @param[in,out] areap   pointer to the area descriptor
 * @return                The operation status.
 *
 * @api
 */
msg_t dsplConstrainArea(DSPLDriver *dsplp, DSPLArea *areap) {

  osalDbgCheck((dsplp != NULL) && (areap != NULL));
  osalDbgAssert((dsplp->state == DSPL_READY)    ||
                (dsplp->state == DSPL_WRITE)    ||
                (dsplp->state == DSPL_PRESENT)  ||
                (dsplp->state == DSPL_COMPLETE) ||
                (dsplp->state == DSPL_ERROR),
                "invalid state");

  return dspl_lld_constrain_area(dsplp, areap);
}

/**
 * @brief   Returns and clears latched display events.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @return              The latched events set.
 *
 * @api
 */
dsplevents_t dsplGetAndClearEvents(DSPLDriver *dsplp) {
  dsplevents_t events;

  osalDbgCheck(dsplp != NULL);

  osalSysLock();
  events = dsplp->lastevents;
  dsplp->lastevents = DSPL_EVT_NONE;
  osalSysUnlock();

  events |= dspl_lld_get_and_clear_events(dsplp);

  return events;
}

/**
 * @brief   Starts an asynchronous area write operation.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @param[in] areap     pointer to the area descriptor
 * @param[in] buffer    pointer to the source pixel buffer
 * @param[in] stride    source buffer line stride, in bytes
 * @return              The operation status.
 *
 * @api
 */
msg_t dsplStartWrite(DSPLDriver *dsplp,
                     const DSPLArea *areap,
                     const void *buffer,
                     size_t stride) {
  msg_t msg;

  osalDbgCheck((dsplp != NULL) && (areap != NULL) && (buffer != NULL));
  osalDbgCheck((areap->width > 0U) && (areap->height > 0U));

  osalSysLock();
  msg = dspl_start_write_s(dsplp, areap, buffer, stride);

  osalSysUnlock();

  return msg;
}

/**
 * @brief   Starts an asynchronous framebuffer present operation.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @param[in] buffer    pointer to the framebuffer to be presented
 * @return              The operation status.
 *
 * @api
 */
msg_t dsplStartPresent(DSPLDriver *dsplp, void *buffer) {
  msg_t msg;

  osalDbgCheck((dsplp != NULL) && (buffer != NULL));

  osalSysLock();
  msg = dspl_start_present_s(dsplp, buffer);

  osalSysUnlock();

  return msg;
}

/**
 * @brief   Returns one framebuffer pointer by index.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @param[in] index     framebuffer index
 * @return              Pointer to the framebuffer or @p NULL.
 *
 * @api
 */
void *dsplGetFramebuffer(DSPLDriver *dsplp, unsigned index) {

  osalDbgCheck(dsplp != NULL);
  osalDbgAssert(dsplp->state != DSPL_UNINIT, "not initialized");

  return dspl_lld_get_framebuffer(dsplp, index);
}

/**
 * @brief   Performs a driver-specific control operation.
 *
 * @param[in] dsplp       pointer to the @p DSPLDriver object
 * @param[in] operation   control code
 * @param[in,out] arg     argument pointer
 * @return                The operation status.
 *
 * @api
 */
msg_t dsplControl(DSPLDriver *dsplp, unsigned int operation, void *arg) {

  osalDbgCheck(dsplp != NULL);

  return dspl_lld_control(dsplp, operation, arg);
}

#if (DSPL_USE_WAIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Waits for completion of the current asynchronous operation.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] timeout    operation timeout
 * @return               The completion status.
 *
 * @sclass
 */
msg_t dsplSynchronizeS(DSPLDriver *dsplp, sysinterval_t timeout) {
  msg_t msg;

  osalDbgCheck(dsplp != NULL);
  osalDbgAssert((dsplp->state == DSPL_READY)   ||
                (dsplp->state == DSPL_WRITE)   ||
                (dsplp->state == DSPL_PRESENT) ||
                (dsplp->state == DSPL_COMPLETE) ||
                (dsplp->state == DSPL_ERROR),
                "invalid state");

  if ((dsplp->state == DSPL_WRITE) || (dsplp->state == DSPL_PRESENT)) {
    msg = osalThreadSuspendTimeoutS(&dsplp->sync_op, timeout);
  }
  else {
    msg = dsplp->lastmsg;
  }

  return msg;
}

/**
 * @brief   Waits for completion of the current asynchronous operation.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] timeout    operation timeout
 * @return               The completion status.
 *
 * @api
 */
msg_t dsplSynchronize(DSPLDriver *dsplp, sysinterval_t timeout) {
  msg_t msg;

  osalSysLock();
  msg = dsplSynchronizeS(dsplp, timeout);
  osalSysUnlock();

  return msg;
}

/**
 * @brief   Waits for a VSYNC/TE notification.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] timeout    operation timeout
 * @return               The operation status.
 *
 * @sclass
 */
msg_t dsplWaitVSyncS(DSPLDriver *dsplp, sysinterval_t timeout) {
  msg_t msg;

  osalDbgCheck(dsplp != NULL);
  osalDbgAssert((dsplp->state == DSPL_READY)   ||
                (dsplp->state == DSPL_WRITE)   ||
                (dsplp->state == DSPL_PRESENT) ||
                (dsplp->state == DSPL_COMPLETE) ||
                (dsplp->state == DSPL_ERROR),
                "invalid state");

  if ((dsplp->lastevents & DSPL_EVT_ALL_TIMING) != 0U) {
    dsplp->lastevents &= (dsplevents_t)~DSPL_EVT_ALL_TIMING;
    msg = MSG_OK;
  }
  else {
    msg = osalThreadSuspendTimeoutS(&dsplp->sync_vsync, timeout);
  }

  return msg;
}

/**
 * @brief   Waits for a VSYNC/TE notification.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] timeout    operation timeout
 * @return               The operation status.
 *
 * @api
 */
msg_t dsplWaitVSync(DSPLDriver *dsplp, sysinterval_t timeout) {
  msg_t msg;

  osalSysLock();
  msg = dsplWaitVSyncS(dsplp, timeout);
  osalSysUnlock();

  return msg;
}

/**
 * @brief   Performs a synchronous area write operation.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] areap      pointer to the area descriptor
 * @param[in] buffer     pointer to the source pixel buffer
 * @param[in] stride     source buffer line stride, in bytes
 * @return               The operation status.
 *
 * @api
 */
msg_t dsplWrite(DSPLDriver *dsplp,
                const DSPLArea *areap,
                const void *buffer,
                size_t stride) {
  msg_t msg;

  osalDbgCheck((dsplp != NULL) && (areap != NULL) && (buffer != NULL));

  osalSysLock();
  msg = dspl_start_write_s(dsplp, areap, buffer, stride);
  if (msg == HAL_RET_SUCCESS) {
    msg = dsplSynchronizeS(dsplp, TIME_INFINITE);
  }
  osalSysUnlock();

  return msg;
}

/**
 * @brief   Performs a synchronous framebuffer present operation.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] buffer     pointer to the framebuffer
 * @return               The operation status.
 *
 * @api
 */
msg_t dsplPresent(DSPLDriver *dsplp, void *buffer) {
  msg_t msg;

  osalDbgCheck((dsplp != NULL) && (buffer != NULL));

  osalSysLock();
  msg = dspl_start_present_s(dsplp, buffer);
  if (msg == HAL_RET_SUCCESS) {
    msg = dsplSynchronizeS(dsplp, TIME_INFINITE);
  }
  osalSysUnlock();

  return msg;
}
#endif /* DSPL_USE_WAIT == TRUE */

#endif /* HAL_USE_DISPLAY == TRUE */

/** @} */
