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
 * @file    hal_dspl_lld.c
 * @brief   PLATFORM display subsystem low level driver source.
 *
 * @addtogroup DISPLAY
 * @{
 */

#include "hal.h"

#if (HAL_USE_DISPLAY == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   DSPLD1 driver identifier.
 */
#if (PLATFORM_DISPLAY_USE_DSP1 == TRUE) || defined(__DOXYGEN__)
DSPLDriver DSPLD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level display driver initialization.
 *
 * @notapi
 */
void dspl_lld_init(void) {

#if PLATFORM_DISPLAY_USE_DSP1 == TRUE
  dsplObjectInit(&DSPLD1);
#endif
}

/**
 * @brief   Configures and activates the display peripheral.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t dspl_lld_start(DSPLDriver *dsplp) {

  if (dsplp->state == DSPL_STOP) {
#if PLATFORM_DISPLAY_USE_DSP1 == TRUE
    if (&DSPLD1 == dsplp) {

    }
    else
#endif
    {
      osalDbgAssert(false, "invalid DSPL instance");
      return HAL_RET_IS_INVALID;
    }
  }

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the display peripheral.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 *
 * @notapi
 */
void dspl_lld_stop(DSPLDriver *dsplp) {

  if (dsplp->state == DSPL_READY) {
#if PLATFORM_DISPLAY_USE_DSP1 == TRUE
    if (&DSPLD1 == dsplp) {

    }
#endif
  }
}

/**
 * @brief   Returns the display descriptor.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @return              Pointer to the display descriptor.
 *
 * @notapi
 */
const DSPLDescriptor *dspl_lld_get_descriptor(DSPLDriver *dsplp) {

  (void)dsplp;

  return NULL;
}

/**
 * @brief   Applies a rotation to the display device.
 *
 * @param[in] dsplp       pointer to the @p DSPLDriver object
 * @param[in] rotation    selected rotation
 * @return                The operation status.
 *
 * @notapi
 */
msg_t dspl_lld_set_rotation(DSPLDriver *dsplp, dsplrotation_t rotation) {

  (void)dsplp;
  (void)rotation;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Adjusts an area according to implementation constraints.
 *
 * @param[in] dsplp       pointer to the @p DSPLDriver object
 * @param[in,out] areap   pointer to the area descriptor
 * @return                The operation status.
 *
 * @notapi
 */
msg_t dspl_lld_constrain_area(DSPLDriver *dsplp, DSPLArea *areap) {

  (void)dsplp;
  (void)areap;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Returns and clears pending low level events.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @return              The pending event flags.
 *
 * @notapi
 */
dsplevents_t dspl_lld_get_and_clear_events(DSPLDriver *dsplp) {

  (void)dsplp;

  return DSPL_EVT_NONE;
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
 * @notapi
 */
msg_t dspl_lld_start_write(DSPLDriver *dsplp,
                           const DSPLArea *areap,
                           const void *buffer,
                           size_t stride) {

  (void)dsplp;
  (void)areap;
  (void)buffer;
  (void)stride;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Starts an asynchronous framebuffer present operation.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @param[in] buffer    pointer to the framebuffer
 * @return              The operation status.
 *
 * @notapi
 */
msg_t dspl_lld_start_present(DSPLDriver *dsplp, void *buffer) {

  (void)dsplp;
  (void)buffer;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Returns one framebuffer pointer by index.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 * @param[in] index     framebuffer index
 * @return              Pointer to the framebuffer or @p NULL.
 *
 * @notapi
 */
void *dspl_lld_get_framebuffer(DSPLDriver *dsplp, unsigned index) {

  (void)dsplp;
  (void)index;

  return NULL;
}

/**
 * @brief   Performs a driver-specific control operation.
 *
 * @param[in] dsplp       pointer to the @p DSPLDriver object
 * @param[in] operation   control code
 * @param[in,out] arg     argument pointer
 * @return                The operation status.
 *
 * @notapi
 */
msg_t dspl_lld_control(DSPLDriver *dsplp,
                       unsigned int operation,
                       void *arg) {

  (void)dsplp;
  (void)operation;
  (void)arg;

  return HAL_RET_UNKNOWN_CTL;
}

/**
 * @brief   Display interrupt service routine code.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object
 *
 * @notapi
 */
void dspl_lld_serve_interrupt(DSPLDriver *dsplp) {

  (void)dsplp;
}

#endif /* HAL_USE_DISPLAY == TRUE */

/** @} */
