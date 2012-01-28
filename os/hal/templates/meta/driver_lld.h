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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    templates/xxx_lld.h
 * @brief   XXX Driver subsystem low level driver header template.
 *
 * @addtogroup XXX
 * @{
 */

#ifndef _XXX_LLD_H_
#define _XXX_LLD_H_

#if HAL_USE_XXX || defined(__DOXYGEN__)

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
 * @brief   Type of a structure representing an XXX driver.
 */
typedef struct XXXDriver XXXDriver;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {

} XXXConfig;

/**
 * @brief   Structure representing an XXX driver.
 */
struct XXXDriver {
  /**
   * @brief Driver state.
   */
  xxxstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const XXXConfig           *config;
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
  void xxx_lld_init(void);
  void xxx_lld_start(XXXDriver *xxxp);
  void xxx_lld_stop(XXXDriver *xxxp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_XXX */

#endif /* _XXX_LLD_H_ */

/** @} */
