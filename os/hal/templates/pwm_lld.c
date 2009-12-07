/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file templates/pwm_lld.c
 * @brief PWM Driver subsystem low level driver source template
 * @addtogroup PWM_LLD
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Low Level Driver exported variables.                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver local variables.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver interrupt handlers.                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver exported functions.                                      */
/*===========================================================================*/

/**
 * @brief Low level PWM driver initialization.
 */
void pwm_lld_init(void) {

}

/**
 * @brief Configures and activates the PWM peripheral.
 *
 * @param[in] pwmp pointer to the @p PWMDriver object
 */
void pwm_lld_start(PWMDriver *pwmp) {

  if (pwmp->pwm_state == PWM_STOP) {
    /* Clock activation.*/
  }
  /* Configuration.*/
}

/**
 * @brief Deactivates the PWM peripheral.
 *
 * @param[in] pwmp pointer to the @p PWMDriver object
 */
void pwm_lld_stop(PWMDriver *pwmp) {

}

/** @} */
