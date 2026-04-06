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
 * @file    hal_dspl_lld.h
 * @brief   PLATFORM display subsystem low level driver header.
 *
 * @addtogroup DISPLAY
 * @{
 */

#ifndef HAL_DSPL_LLD_H
#define HAL_DSPL_LLD_H

#if (HAL_USE_DISPLAY == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    PLATFORM configuration options
 * @{
 */
/**
 * @brief   Display driver enable switch.
 * @details If set to @p TRUE then the support for @p DSPLD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(PLATFORM_DISPLAY_USE_DSP1) || defined(__DOXYGEN__)
#define PLATFORM_DISPLAY_USE_DSP1           FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the display driver structure.
 */
#define dspl_lld_driver_fields                                              \
  uint32_t                  dummy

/**
 * @brief   Low level fields of the display configuration structure.
 */
#define dspl_lld_config_fields                                              \
  uint32_t                  dummy

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (PLATFORM_DISPLAY_USE_DSP1 == TRUE) && !defined(__DOXYGEN__)
extern DSPLDriver DSPLD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dspl_lld_init(void);
  msg_t dspl_lld_start(DSPLDriver *dsplp);
  void dspl_lld_stop(DSPLDriver *dsplp);
  const DSPLDescriptor *dspl_lld_get_descriptor(DSPLDriver *dsplp);
  msg_t dspl_lld_set_rotation(DSPLDriver *dsplp, dsplrotation_t rotation);
  msg_t dspl_lld_constrain_area(DSPLDriver *dsplp, DSPLArea *areap);
  dsplevents_t dspl_lld_get_and_clear_events(DSPLDriver *dsplp);
  msg_t dspl_lld_start_write(DSPLDriver *dsplp,
                             const DSPLArea *areap,
                             const void *buffer,
                             size_t stride);
  msg_t dspl_lld_start_present(DSPLDriver *dsplp, void *buffer);
  void *dspl_lld_get_framebuffer(DSPLDriver *dsplp, unsigned index);
  msg_t dspl_lld_control(DSPLDriver *dsplp,
                         unsigned int operation,
                         void *arg);
  void dspl_lld_serve_interrupt(DSPLDriver *dsplp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_DISPLAY == TRUE */

#endif /* HAL_DSPL_LLD_H */

/** @} */
