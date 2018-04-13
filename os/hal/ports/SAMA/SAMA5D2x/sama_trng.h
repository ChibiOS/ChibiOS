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
 * @file    SAMA5D2x/sama_trng.h
 * @brief   SAMA TRNG support macros and structures.
 *
 * @addtogroup SAMA5D2x_TRNG
 * @{
 */

#ifndef SAMA_TRNG_LLD_H
#define SAMA_TRNG_LLD_H

#if (HAL_USE_TRNG) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  TRNG_UNINIT = 0,                  /**< Not initialized.                   */
  TRNG_STOP = 1,                    /**< Stopped.                           */
  TRNG_READY = 2                    /**< Ready.                             */
} trngstate_t;

/**
 * @brief   Type of a structure representing an CLASSD driver.
 */
typedef struct TRNGDriver TRNGDriver;

/**
 * @brief   Structure representing an TRNG driver.
 */
struct TRNGDriver {
  /**
   * @brief   Driver state.
   */
  trngstate_t               state;
  /**
   * @brief   Pointer to the WDT registers block.
   */
  Trng                      *trng;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern TRNGDriver TRNGD0;

#ifdef __cplusplus
extern "C" {
#endif
  void trngInit(void);
  void trngStart(TRNGDriver *trngp);
  void trngStop(TRNGDriver *trngp);
  uint32_t trngGetRandomNumber(TRNGDriver *trngp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_TRNG */

#endif /* SAMA_TRNG_LLD_H */

/** @} */
