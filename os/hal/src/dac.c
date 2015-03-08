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
 * @file    dac.c
 * @brief   DAC Driver code.
 *
 * @addtogroup DAC
 * @{
 */

#include "hal.h"

#if (HAL_USE_DAC == TRUE) || defined(__DOXYGEN__)

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
 * @brief   DAC Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void dacInit(void) {

  dac_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p DACDriver structure.
 *
 * @param[out] dacp     pointer to the @p DACDriver object
 *
 * @init
 */
void dacObjectInit(DACDriver *dacp) {

  dacp->state = DAC_STOP;
  dacp->config = NULL;
#if DAC_USE_WAIT
  dacp->thread = NULL;
#endif
#if DAC_USE_MUTUAL_EXCLUSION
  osalMutexObjectInit(&dacp->mutex);
#endif
#if defined(DAC_DRIVER_EXT_INIT_HOOK)
  DAC_DRIVER_EXT_INIT_HOOK(dacp);
#endif
}

/**
 * @brief   Configures and activates the DAC peripheral.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] config    pointer to the @p DACConfig object
 *
 * @api
 */
void dacStart(DACDriver *dacp, const DACConfig *config) {

  osalDbgCheck((dacp != NULL) && (config != NULL));

  osalSysLock();

  osalDbgAssert((dacp->state == DAC_STOP) || (dacp->state == DAC_READY),
                "invalid state");

  dacp->config = config;
  dac_lld_start(dacp);
  dacp->state = DAC_READY;

  osalSysUnlock();
}

/**
 * @brief Deactivates the DAC peripheral.
 * @note  Deactivating the peripheral also enforces a release of the slave
 *        select line.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @api
 */
void dacStop(DACDriver *dacp) {

  osalDbgCheck(dacp != NULL);

  osalSysLock();

  osalDbgAssert((dacp->state == DAC_STOP) || (dacp->state == DAC_READY),
                "invalid state");

  dac_lld_stop(dacp);
  dacp->state = DAC_STOP;

  osalSysUnlock();
}

/**
 * @brief   Starts a DAC conversion.
 * @details Starts an asynchronous conversion operation.
 * @note    The buffer is organized as a matrix of M*N elements where M is the
 *          channels number configured into the conversion group and N is the
 *          buffer depth. The samples are sequentially written into the buffer
 *          with no gaps.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] grpp      pointer to a @p DACConversionGroup object
 * @param[in] samples   pointer to the samples buffer
 * @param[in] depth     buffer depth (matrix rows number). The buffer depth
 *                      must be one or an even number.
 *
 * @api
 */
void dacStartConversion(DACDriver *dacp,
                        const DACConversionGroup *grpp,
                        const dacsample_t *samples,
                        size_t depth) {

  osalSysLock();
  dacStartConversionI(dacp, grpp, samples, depth);
  osalSysUnlock();
}

/**
 * @brief   Starts a DAC conversion.
 * @details Starts an asynchronous conversion operation.
 * @post    The callbacks associated to the conversion group will be invoked
 *          on buffer fill and error events.
 * @note    The buffer is organized as a matrix of M*N elements where M is the
 *          channels number configured into the conversion group and N is the
 *          buffer depth. The samples are sequentially written into the buffer
 *          with no gaps.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] grpp      pointer to a @p DACConversionGroup object
 * @param[in] samples   pointer to the samples buffer
 * @param[in] depth     buffer depth (matrix rows number). The buffer depth
 *                      must be one or an even number.
 *
 * @iclass
 */
void dacStartConversionI(DACDriver *dacp,
                         const DACConversionGroup *grpp,
                         const dacsample_t *samples,
                         size_t depth) {

  osalDbgCheckClassI();
  osalDbgCheck((dacp != NULL) && (grpp != NULL) && (samples != NULL) &&
               ((depth == 1) || ((depth & 1) == 0)));
  osalDbgAssert((dacp->state == DAC_READY) ||
                (dacp->state == DAC_COMPLETE) ||
                (dacp->state == DAC_ERROR),
                "not ready");

  dacp->samples  = samples;
  dacp->depth    = depth;
  dacp->grpp     = grpp;
  dacp->state    = DAC_ACTIVE;
  dac_lld_start_conversion(dacp);
}

/**
 * @brief   Stops an ongoing conversion.
 * @details This function stops the currently ongoing conversion and returns
 *          the driver in the @p DAC_READY state. If there was no conversion
 *          being processed then the function does nothing.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @api
 */
void dacStopConversion(DACDriver *dacp) {

  osalDbgCheck(dacp != NULL);

  osalSysLock();

  osalDbgAssert((dacp->state == DAC_READY) ||
                (dacp->state == DAC_ACTIVE),
                "invalid state");

  if (dacp->state != DAC_READY) {
    dac_lld_stop_conversion(dacp);
    dacp->grpp  = NULL;
    dacp->state = DAC_READY;
    _dac_reset_s(dacp);
  }

  osalSysUnlock();
}

/**
 * @brief   Stops an ongoing conversion.
 * @details This function stops the currently ongoing conversion and returns
 *          the driver in the @p DAC_READY state. If there was no conversion
 *          being processed then the function does nothing.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @iclass
 */
void dacStopConversionI(DACDriver *dacp) {

  osalDbgCheckClassI();
  osalDbgCheck(dacp != NULL);
  osalDbgAssert((dacp->state == DAC_READY) ||
                (dacp->state == DAC_ACTIVE) ||
                (dacp->state == DAC_COMPLETE),
                "invalid state");

  if (dacp->state != DAC_READY) {
    dac_lld_stop_conversion(dacp);
    dacp->grpp  = NULL;
    dacp->state = DAC_READY;
    _dac_reset_i(dacp);
  }
}

#if (DAC_USE_WAIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Performs a DAC conversion.
 * @details Performs a synchronous conversion operation.
 * @note    The buffer is organized as a matrix of M*N elements where M is the
 *          channels number configured into the conversion group and N is the
 *          buffer depth. The samples are sequentially written into the buffer
 *          with no gaps.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] grpp      pointer to a @p DACConversionGroup object
 * @param[out] samples  pointer to the samples buffer
 * @param[in] depth     buffer depth (matrix rows number). The buffer depth
 *                      must be one or an even number.
 * @return              The operation result.
 * @retval MSG_OK       Conversion finished.
 * @retval MSG_RESET    The conversion has been stopped using
 *                      @p acdStopConversion() or @p acdStopConversionI(),
 *                      the result buffer may contain incorrect data.
 * @retval MSG_TIMEOUT  The conversion has been stopped because an hardware
 *                      error.
 *
 * @api
 */
msg_t dacConvert(DACDriver *dacp,
                 const DACConversionGroup *grpp,
                 const dacsample_t *samples,
                 size_t depth) {
  msg_t msg;

  osalSysLock();

  dacStartConversionI(dacp, grpp, samples, depth);
  msg = osalThreadSuspendS(&dacp->thread);

  osalSysUnlock();
  return msg;
}
#endif /* DAC_USE_WAIT == TRUE */

#if (DAC_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the DAC bus.
 * @details This function tries to gain ownership to the DAC bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option @p DAC_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @api
 */
void dacAcquireBus(DACDriver *dacp) {

  osalDbgCheck(dacp != NULL);
	
  osalMutexLock(&dacp->mutex);
}

/**
 * @brief   Releases exclusive access to the DAC bus.
 * @pre     In order to use this function the option @p DAC_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @api
 */
void dacReleaseBus(DACDriver *dacp) {

  osalDbgCheck(dacp != NULL);
	
  osalMutexUnlock(&dacp->mutex);
}
#endif /* DAC_USE_MUTUAL_EXCLUSION == TRUE */

#endif /* HAL_USE_DAC == TRUE */

/** @} */
