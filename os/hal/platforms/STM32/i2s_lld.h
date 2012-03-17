/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    templates/i2s_lld.h
 * @brief   I2S Driver subsystem low level driver header template.
 *
 * @addtogroup I2S
 * @{
 */

#ifndef _I2S_LLD_H_
#define _I2S_LLD_H_

#if HAL_USE_I2S || defined(__DOXYGEN__)

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
 * @brief   Type of a structure representing an I2S driver.
 */
typedef struct I2SDriver I2SDriver;

/**
 * @brief   I2S notification callback type.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 * @param[in] buffer    pointer to the buffer
 * @param[in] n         number of sample positions starting from @p buffer
 */
typedef void (*i2scallback_t)(I2SDriver *i2sp, void *buffer, size_t n);

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Callback function associated to the reception or @p NULL.
   */
  i2scallback_t             rx_cb;;
  /**
   * @brief   Callback function associated to the transmission or @p NULL.
   */
  i2scallback_t             tx_cb;
  /* End of the mandatory fields.*/
} I2SConfig;

/**
 * @brief   Structure representing an I2S driver.
 */
struct I2SDriver {
  /**
   * @brief Driver state.
   */
  i2sstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const I2SConfig           *config;
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void i2s_lld_init(void);
  void i2s_lld_start(I2SDriver *i2sp);
  void i2s_lld_stop(I2SDriver *i2sp);
  void i2s_lld_start_exchange(I2SDriver *i2sp);
  void i2s_lld_stop_exchange(I2SDriver *i2sp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_I2S */

#endif /* _I2S_LLD_H_ */

/** @} */
