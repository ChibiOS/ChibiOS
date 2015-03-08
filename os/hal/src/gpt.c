/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    gpt.c
 * @brief   GPT Driver code.
 *
 * @addtogroup GPT
 * @{
 */

#include "hal.h"

#if (HAL_USE_GPT == TRUE) || defined(__DOXYGEN__)

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

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   GPT Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void gptInit(void) {

  gpt_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p GPTDriver structure.
 *
 * @param[out] gptp     pointer to the @p GPTDriver object
 *
 * @init
 */
void gptObjectInit(GPTDriver *gptp) {

  gptp->state  = GPT_STOP;
  gptp->config = NULL;
}

/**
 * @brief   Configures and activates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] config    pointer to the @p GPTConfig object
 *
 * @api
 */
void gptStart(GPTDriver *gptp, const GPTConfig *config) {

  osalDbgCheck((gptp != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((gptp->state == GPT_STOP) || (gptp->state == GPT_READY),
              "invalid state");
  gptp->config = config;
  gpt_lld_start(gptp);
  gptp->state = GPT_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @api
 */
void gptStop(GPTDriver *gptp) {

  osalDbgCheck(gptp != NULL);

  osalSysLock();
  osalDbgAssert((gptp->state == GPT_STOP) || (gptp->state == GPT_READY),
                "invalid state");
  gpt_lld_stop(gptp);
  gptp->state = GPT_STOP;
  osalSysUnlock();
}

/**
 * @brief   Changes the interval of GPT peripheral.
 * @details This function changes the interval of a running GPT unit.
 * @pre     The GPT unit must be running in continuous mode.
 * @post    The GPT unit interval is changed to the new value.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 * @param[in] interval  new cycle time in timer ticks
 *
 * @api
 */
void gptChangeInterval(GPTDriver *gptp, gptcnt_t interval) {

  osalDbgCheck(gptp != NULL);

  osalSysLock();
  osalDbgAssert(gptp->state == GPT_CONTINUOUS,
                "invalid state");
  gptChangeIntervalI(gptp, interval);
  osalSysUnlock();
}

/**
 * @brief   Starts the timer in continuous mode.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  period in ticks
 *
 * @api
 */
void gptStartContinuous(GPTDriver *gptp, gptcnt_t interval) {

  osalSysLock();
  gptStartContinuousI(gptp, interval);
  osalSysUnlock();
}

/**
 * @brief   Starts the timer in continuous mode.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  period in ticks
 *
 * @iclass
 */
void gptStartContinuousI(GPTDriver *gptp, gptcnt_t interval) {

  osalDbgCheckClassI();
  osalDbgCheck(gptp != NULL);
  osalDbgAssert(gptp->state == GPT_READY,
                "invalid state");

  gptp->state = GPT_CONTINUOUS;
  gpt_lld_start_timer(gptp, interval);
}

/**
 * @brief   Starts the timer in one shot mode.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  time interval in ticks
 *
 * @api
 */
void gptStartOneShot(GPTDriver *gptp, gptcnt_t interval) {

  osalSysLock();
  gptStartOneShotI(gptp, interval);
  osalSysUnlock();
}

/**
 * @brief   Starts the timer in one shot mode.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  time interval in ticks
 *
 * @api
 */
void gptStartOneShotI(GPTDriver *gptp, gptcnt_t interval) {

  osalDbgCheckClassI();
  osalDbgCheck(gptp != NULL);
  osalDbgCheck(gptp->config->callback != NULL);
  osalDbgAssert(gptp->state == GPT_READY,
                "invalid state");

  gptp->state = GPT_ONESHOT;
  gpt_lld_start_timer(gptp, interval);
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @api
 */
void gptStopTimer(GPTDriver *gptp) {

  osalSysLock();
  gptStopTimerI(gptp);
  osalSysUnlock();
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @api
 */
void gptStopTimerI(GPTDriver *gptp) {

  osalDbgCheckClassI();
  osalDbgCheck(gptp != NULL);
  osalDbgAssert((gptp->state == GPT_READY) || (gptp->state == GPT_CONTINUOUS) ||
                (gptp->state == GPT_ONESHOT),
                "invalid state");

  gptp->state = GPT_READY;
  gpt_lld_stop_timer(gptp);
}

/**
 * @brief   Starts the timer in one shot mode and waits for completion.
 * @details This function specifically polls the timer waiting for completion
 *          in order to not have extra delays caused by interrupt servicing,
 *          this function is only recommended for short delays.
 * @note    The configured callback is not invoked when using this function.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  time interval in ticks
 *
 * @api
 */
void gptPolledDelay(GPTDriver *gptp, gptcnt_t interval) {

  osalDbgAssert(gptp->state == GPT_READY,
                "invalid state");

  gptp->state = GPT_ONESHOT;
  gpt_lld_polled_delay(gptp, interval);
  gptp->state = GPT_READY;
}

#endif /* HAL_USE_GPT == TRUE */

/** @} */
