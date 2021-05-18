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
 * @file    vt_storm.h
 * @brief   VT Storm stress test header.
 *
 * @addtogroup VT_STORM
 * @{
 */

#ifndef VT_STORM_H
#define VT_STORM_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Timings randomization.
 */
#if !defined(VT_STORM_CFG_RANDOMIZE) || defined(__DOXYGEN__)
#define VT_STORM_CFG_RANDOMIZE              FALSE
#endif

/**
 * @brief   Number of test iterations.
 */
#if !defined(VT_STORM_CFG_ITERATIONS) || defined(__DOXYGEN__)
#define VT_STORM_CFG_ITERATIONS             100
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

typedef struct {
  /**
   * @brief   Stream for output.
   */
  BaseSequentialStream  *out;
  /**
   * @brief   LED line.
   */
  ioline_t              line;
  /**
   * @brief   System clock.
   */
  uint32_t              sysclk;
} vt_storm_config_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void vt_storm_execute(const vt_storm_config_t *cfg);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VT_STORM_H */

/** @} */
