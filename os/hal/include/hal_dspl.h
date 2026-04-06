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
 * @file    hal_dspl.h
 * @brief   Display Driver macros and structures.
 *
 * @addtogroup DISPLAY
 * @{
 */

#ifndef HAL_DSPL_H
#define HAL_DSPL_H

#if (HAL_USE_DISPLAY == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Display capability flags
 * @{
 */
#define DSPL_CAP_PARTIAL_WRITE          (1U << 0)
#define DSPL_CAP_FRAMEBUFFER            (1U << 1)
#define DSPL_CAP_DOUBLE_BUFFER          (1U << 2)
#define DSPL_CAP_ASYNC_WRITE            (1U << 3)
#define DSPL_CAP_ASYNC_PRESENT          (1U << 4)
#define DSPL_CAP_VSYNC                  (1U << 5)
#define DSPL_CAP_TE                     (1U << 6)
#define DSPL_CAP_HW_ROTATION            (1U << 7)
#define DSPL_CAP_READBACK               (1U << 8)
/** @} */

/**
 * @name    Display event flags
 * @{
 */
#define DSPL_EVT_NONE                       0U
#define DSPL_EVT_WRITE_COMPLETE             (1U << 0)
#define DSPL_EVT_PRESENT_COMPLETE           (1U << 1)
#define DSPL_EVT_VSYNC                      (1U << 2)
#define DSPL_EVT_TE                         (1U << 3)
#define DSPL_EVT_UNDERRUN                   (1U << 4)
#define DSPL_EVT_ERROR                      (1U << 5)
#define DSPL_EVT_ALL_COMPLETIONS            (DSPL_EVT_WRITE_COMPLETE |      \
                                             DSPL_EVT_PRESENT_COMPLETE)
#define DSPL_EVT_ALL_TIMING                 (DSPL_EVT_VSYNC |               \
                                             DSPL_EVT_TE)
#define DSPL_EVT_ALL_EVENTS                 (DSPL_EVT_ALL_COMPLETIONS |     \
                                             DSPL_EVT_ALL_TIMING      |     \
                                             DSPL_EVT_UNDERRUN        |     \
                                             DSPL_EVT_ERROR)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Display configuration options
 * @{
 */
/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(DSPL_USE_WAIT) || defined(__DOXYGEN__)
#define DSPL_USE_WAIT                       TRUE
#endif

/**
 * @brief   Enables the event source notification API.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(DSPL_USE_EVENTS) || defined(__DOXYGEN__)
#define DSPL_USE_EVENTS                     TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (DSPL_USE_WAIT != FALSE) && (DSPL_USE_WAIT != TRUE)
#error "invalid DSPL_USE_WAIT value"
#endif

#if (DSPL_USE_EVENTS != FALSE) && (DSPL_USE_EVENTS != TRUE)
#error "invalid DSPL_USE_EVENTS value"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of display event flags.
 */
typedef eventflags_t dsplevents_t;

/**
 * @brief   Type of display capability flags.
 */
typedef uint32_t dsplcaps_t;

/**
 * @brief   Type of structure representing a display driver.
 */
typedef struct hal_dspl_driver DSPLDriver;

/**
 * @brief   Type of structure representing a display configuration.
 */
typedef struct hal_dspl_config DSPLConfig;

/**
 * @brief   Type of a display notification callback.
 * @details The callback is invoked from ISR context.
 *
 * @param[in] dsplp     pointer to the @p DSPLDriver object triggering the
 *                      callback
 */
typedef void (*dsplcallback_t)(DSPLDriver *dsplp);

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  DSPL_UNINIT = 0,                 /**< Not initialized.                   */
  DSPL_STOP = 1,                   /**< Stopped.                           */
  DSPL_READY = 2,                  /**< Ready.                             */
  DSPL_WRITE = 3,                  /**< Writing a display area.            */
  DSPL_PRESENT = 4,                /**< Presenting a framebuffer.          */
  DSPL_COMPLETE = 5,               /**< Asynchronous operation complete.   */
  DSPL_ERROR = 6                   /**< Driver error condition.            */
} dsplstate_t;

/**
 * @brief   Display rotation.
 */
typedef enum {
  DSPL_ROTATION_0 = 0,             /**< No rotation.                       */
  DSPL_ROTATION_90 = 1,            /**< Clockwise 90 degrees.              */
  DSPL_ROTATION_180 = 2,           /**< Clockwise 180 degrees.             */
  DSPL_ROTATION_270 = 3            /**< Clockwise 270 degrees.             */
} dsplrotation_t;

/**
 * @brief   Display pixel formats.
 */
typedef enum {
  DSPL_FMT_RGB565 = 0,             /**< 16 bits, RGB 565.                  */
  DSPL_FMT_RGB888 = 1,             /**< 24 bits, RGB 888.                  */
  DSPL_FMT_XRGB8888 = 2,           /**< 32 bits, XRGB 8888.                */
  DSPL_FMT_ARGB8888 = 3,           /**< 32 bits, ARGB 8888.                */
  DSPL_FMT_L8 = 4,                 /**< 8 bits luminance.                  */
  DSPL_FMT_I1 = 5                  /**< 1 bit monochrome.                  */
} dsplformat_t;

/**
 * @brief   Display area descriptor.
 */
typedef struct {
  /**
   * @brief   Left coordinate of the area.
   */
  uint16_t                  x;
  /**
   * @brief   Top coordinate of the area.
   */
  uint16_t                  y;
  /**
   * @brief   Area width in pixels.
   */
  uint16_t                  width;
  /**
   * @brief   Area height in pixels.
   */
  uint16_t                  height;
} DSPLArea;

/**
 * @brief   Display descriptor.
 */
typedef struct {
  /**
   * @brief   Logical width in pixels.
   */
  uint16_t                  width;
  /**
   * @brief   Logical height in pixels.
   */
  uint16_t                  height;
  /**
   * @brief   Physical width in pixels.
   * @note    It can differ from the logical size when only a sub-area of the
   *          physical panel is exposed.
   */
  uint16_t                  physical_width;
  /**
   * @brief   Physical height in pixels.
   */
  uint16_t                  physical_height;
  /**
   * @brief   Horizontal offset of the logical area within the physical panel.
   */
  uint16_t                  offset_x;
  /**
   * @brief   Vertical offset of the logical area within the physical panel.
   */
  uint16_t                  offset_y;
  /**
   * @brief   Horizontal alignment requirement, in pixels, for area origins.
   */
  uint16_t                  x_alignment;
  /**
   * @brief   Vertical alignment requirement, in pixels, for area origins.
   */
  uint16_t                  y_alignment;
  /**
   * @brief   Horizontal alignment requirement, in pixels, for area sizes.
   */
  uint16_t                  width_alignment;
  /**
   * @brief   Vertical alignment requirement, in pixels, for area sizes.
   */
  uint16_t                  height_alignment;
  /**
   * @brief   Alignment requirement, in bytes, for line stride values.
   */
  uint32_t                  stride_alignment;
  /**
   * @brief   Native pixel format.
   */
  dsplformat_t              format;
  /**
   * @brief   Supported capabilities.
   */
  dsplcaps_t                capabilities;
  /**
   * @brief   Number of framebuffers made available by the driver.
   */
  uint8_t                   framebuffers_num;
} DSPLDescriptor;

/* Including the low level driver header, it exports information required
   for completing types.*/
#include "hal_dspl_lld.h"

/**
 * @brief   Driver configuration structure.
 */
struct hal_dspl_config {
  /**
   * @brief   Initial rotation.
   */
  dsplrotation_t            rotation;
  /* End of the mandatory fields.*/
  dspl_lld_config_fields
};

/**
 * @brief   Structure representing a display driver.
 */
struct hal_dspl_driver {
  /**
   * @brief   Driver state.
   */
  dsplstate_t               state;
  /**
   * @brief   Current configuration data.
   */
  const DSPLConfig          *config;
  /**
   * @brief   Current display rotation.
   */
  dsplrotation_t            rotation;
  /**
   * @brief   Latched events not yet cleared by the client.
   */
  dsplevents_t              lastevents;
  /**
   * @brief   Completion status of the last asynchronous operation.
   */
  msg_t                     lastmsg;
  /**
   * @brief   Notification callback.
   * @note    Can be @p NULL.
   */
  dsplcallback_t            cb;
  /**
   * @brief   User argument.
   */
  void                      *arg;
#if (DSPL_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread for generic operations.
   */
  thread_reference_t        sync_op;
  /**
   * @brief   Waiting thread for vertical synchronization.
   */
  thread_reference_t        sync_vsync;
#endif /* DSPL_USE_WAIT == TRUE */
#if (DSPL_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Event source for display notifications.
   */
  event_source_t            event;
#endif /* DSPL_USE_EVENTS == TRUE */
#if defined(DISPLAY_DRIVER_EXT_FIELDS)
  DISPLAY_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  dspl_lld_driver_fields
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Associates a callback to the display instance.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] f          callback to be associated
 */
#define dsplSetCallbackX(dsplp, f) ((dsplp)->cb = (f))

/**
 * @brief   Associates a user argument to the display instance.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] p          argument to be associated
 */
#define dsplSetArgumentX(dsplp, p) ((dsplp)->arg = (p))

/**
 * @brief   Returns the associated user argument.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @return               The associated user argument.
 */
#define dsplGetArgumentX(dsplp) ((dsplp)->arg)

/**
 * @brief   Returns the current driver state.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @return               The current driver state.
 */
#define dsplGetDriverStateX(dsplp) ((dsplp)->state)

#if (DSPL_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Returns the event source of the display driver.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @return               Pointer to the associated event source.
 */
#define dsplGetEventSourceX(dsplp) (&(dsplp)->event)
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void dsplInit(void);
  void dsplObjectInit(DSPLDriver *dsplp);
  msg_t dsplStart(DSPLDriver *dsplp, const DSPLConfig *config);
  void dsplStop(DSPLDriver *dsplp);
  const DSPLDescriptor *dsplGetDescriptorX(DSPLDriver *dsplp);
  msg_t dsplSetRotation(DSPLDriver *dsplp,
                        dsplrotation_t rotation);
  msg_t dsplConstrainArea(DSPLDriver *dsplp, DSPLArea *areap);
  dsplevents_t dsplGetAndClearEvents(DSPLDriver *dsplp);
  msg_t dsplStartWrite(DSPLDriver *dsplp,
                       const DSPLArea *areap,
                       const void *buffer,
                       size_t stride);
  msg_t dsplStartPresent(DSPLDriver *dsplp, void *buffer);
  void *dsplGetFramebuffer(DSPLDriver *dsplp, unsigned index);
  msg_t dsplControl(DSPLDriver *dsplp, unsigned int operation, void *arg);
#if DSPL_USE_WAIT == TRUE
  msg_t dsplSynchronizeS(DSPLDriver *dsplp, sysinterval_t timeout);
  msg_t dsplSynchronize(DSPLDriver *dsplp, sysinterval_t timeout);
  msg_t dsplWaitVSyncS(DSPLDriver *dsplp, sysinterval_t timeout);
  msg_t dsplWaitVSync(DSPLDriver *dsplp, sysinterval_t timeout);
  msg_t dsplWrite(DSPLDriver *dsplp,
                  const DSPLArea *areap,
                  const void *buffer,
                  size_t stride);
  msg_t dsplPresent(DSPLDriver *dsplp, void *buffer);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name    Low level driver helper functions
 * @{
 */
/**
 * @brief   Completion/error notification from ISR context.
 * @details This function handles the portable ISR-side processing for
 *          operation completion notifications:
 *          - latched events update
 *          - operation result update
 *          - waiting thread wakeup
 *          - event broadcast
 *          - callback invocation
 *          - state transition back to @p DSPL_READY
 *          .
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] events     event flags to be latched
 * @param[in] msg        completion status
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __dspl_complete_isr(DSPLDriver *dsplp,
                                       dsplevents_t events,
                                       msg_t msg) {
  osalDbgCheckClassI();
  osalDbgCheck(dsplp != NULL);

  osalSysLockFromISR();
  dsplp->lastevents |= events;
  dsplp->lastmsg    = msg;
#if DSPL_USE_WAIT == TRUE
  osalThreadResumeI(&dsplp->sync_op, msg);
#endif
#if DSPL_USE_EVENTS == TRUE
  osalEventBroadcastFlagsI(&dsplp->event, events);
#endif
  osalSysUnlockFromISR();

  if (dsplp->cb != NULL) {
    if ((events & DSPL_EVT_ERROR) != 0U) {
      dsplp->state = DSPL_ERROR;
      dsplp->cb(dsplp);
      if (dsplp->state == DSPL_ERROR) {
        dsplp->state = DSPL_READY;
      }
    }
    else {
      dsplp->state = DSPL_COMPLETE;
      dsplp->cb(dsplp);
      if (dsplp->state == DSPL_COMPLETE) {
        dsplp->state = DSPL_READY;
      }
    }
  }
  else {
    dsplp->state = DSPL_READY;
  }
}

/**
 * @brief   Timing notification from ISR context.
 * @details This function handles the portable ISR-side processing for VSYNC
 *          and TE notifications.
 *
 * @param[in] dsplp      pointer to the @p DSPLDriver object
 * @param[in] events     event flags to be latched
 *
 * @notapi
 */
CC_FORCE_INLINE
static inline void __dspl_timing_isr(DSPLDriver *dsplp, dsplevents_t events) {
  osalDbgCheckClassI();
  osalDbgCheck(dsplp != NULL);

  osalSysLockFromISR();
  dsplp->lastevents |= events;
#if DSPL_USE_WAIT == TRUE
  osalThreadResumeI(&dsplp->sync_vsync, MSG_OK);
#endif
#if DSPL_USE_EVENTS == TRUE
  osalEventBroadcastFlagsI(&dsplp->event, events);
#endif
  osalSysUnlockFromISR();

  if (dsplp->cb != NULL) {
    dsplp->cb(dsplp);
  }
}
/** @} */

#endif /* HAL_USE_DISPLAY == TRUE */

#endif /* HAL_DSPL_H */

/** @} */
