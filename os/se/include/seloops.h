/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    seloops.h
 * @brief   Safety Extensions Loops module macros and structures.
 *
 * @addtogroup SELOOPS
 * @{
 */

#ifndef SELOOPS_H
#define SELOOPS_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  bool seRegWaitAllSet8X(volatile uint8_t *p, uint8_t mask,
                         rtcnt_t tmo, uint8_t *valp);
  bool seRegWaitAllSet16X(volatile uint16_t *p, uint16_t mask,
                          rtcnt_t tmo, uint16_t *valp);
  bool seRegWaitAllSet32X(volatile uint32_t *p, uint32_t mask,
                          rtcnt_t tmo, uint32_t *valp);
  bool seRegWaitAnySet8X(volatile uint8_t *p, uint8_t mask,
                         rtcnt_t tmo, uint8_t *valp);
  bool seRegWaitAnySet16X(volatile uint16_t *p, uint16_t mask,
                          rtcnt_t tmo, uint16_t *valp);
  bool seRegWaitAnySet32X(volatile uint32_t *p, uint32_t mask,
                          rtcnt_t tmo, uint32_t *valp);
  bool seRegWaitAllClear8X(volatile uint8_t *p, uint8_t mask,
                           rtcnt_t tmo, uint8_t *valp);
  bool seRegWaitAllClear16X(volatile uint16_t *p, uint32_t mask,
                            rtcnt_t tmo, uint16_t *valp);
  bool seRegWaitAllClear32X(volatile uint32_t *p, uint32_t mask,
                            rtcnt_t tmo, uint32_t *valp);
  bool seRegWaitAnyClear8X(volatile uint8_t *p, uint8_t mask,
                           rtcnt_t tmo, uint8_t *valp);
  bool seRegWaitAnyClear16X(volatile uint16_t *p, uint16_t mask,
                            rtcnt_t tmo, uint16_t *valp);
  bool seRegWaitAnyClear32X(volatile uint32_t *p, uint32_t mask,
                            rtcnt_t tmo, uint32_t *valp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SELOOPS_H */

/** @} */
