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
 * @file    gpt.c
 * @brief   GPT Driver code.
 *
 * @addtogroup GPT
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
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

  chDbgCheck((gptp != NULL) && (config != NULL), "ptStart");

  chSysLock();
  chDbgAssert((gptp->state == GPT_STOP) || (gptp->state == GPT_READY),
              "gptStart(), #1", "invalid state");
  gptp->config = config;
  gpt_lld_start(gptp);
  gptp->state = GPT_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @api
 */
void gptStop(GPTDriver *gptp) {

  chDbgCheck(gptp != NULL, "gptStop");

  chSysLock();
  chDbgAssert((gptp->state == GPT_STOP) || (gptp->state == GPT_READY),
              "gptStop(), #1", "invalid state");
  gpt_lld_stop(gptp);
  gptp->state = GPT_STOP;
  chSysUnlock();
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

  chSysLock();
  gptStartContinuousI(gptp, interval);
  chSysUnlock();
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

  chDbgCheckClassI();
  chDbgCheck(gptp != NULL, "gptStartContinuousI");
  chDbgAssert(gptp->state == GPT_READY,
              "gptStartContinuousI(), #1", "invalid state");

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

  chSysLock();
  gptStartOneShotI(gptp, interval);
  chSysUnlock();
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

  chDbgCheckClassI();
  chDbgCheck(gptp != NULL, "gptStartOneShotI");
  chDbgAssert(gptp->state == GPT_READY,
              "gptStartOneShotI(), #1", "invalid state");

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

  chSysLock();
  gptStopTimerI(gptp);
  chSysUnlock();
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @api
 */
void gptStopTimerI(GPTDriver *gptp) {

  chDbgCheckClassI();
  chDbgCheck(gptp != NULL, "gptStopTimerI");
  chDbgAssert((gptp->state == GPT_READY) || (gptp->state == GPT_CONTINUOUS) ||
              (gptp->state == GPT_ONESHOT),
              "gptStopTimerI(), #1", "invalid state");

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

  chDbgAssert(gptp->state == GPT_READY,
              "gptPolledDelay(), #1", "invalid state");

  gptp->state = GPT_ONESHOT;
  gpt_lld_polled_delay(gptp, interval);
}

#endif /* HAL_USE_GPT */

/** @} */
