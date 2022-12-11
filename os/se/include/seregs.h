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
 * @file    seregs.h
 * @brief   Safety Extensions Registers module macros and structures.
 *
 * @addtogroup SEREGS
 * @{
 */

#ifndef SEREGS_H
#define SEREGS_H

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

/**
 * @name    Function variants
 * @{
 */
#define seRegSet8X(p, s, v) seRegModify8X(p, s, (uint8_t)0, v)
#define seRegSet16X(p, s, v) seRegModify16X(p, s, (uint16_t)0, v)
#define seRegSet32X(p, s, v) seRegModify32X(p, s, (uint32_t)0, v)
#define seRegClear8X(p, c, v) seRegModify8X(p, (uint8_t)0, c, v)
#define seRegClear16X(p, c, v) seRegModify16X(p, (uint16_t)0, c, v)
#define seRegClear32X(p, c, v) seRegModify32X(p, (uint32_t)0, c, v)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Modifies a 8 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 *
 * @param[in] p         address of the register
 * @param[in] setmask   mask of bits to be set
 * @param[in] clrmask   mask of bits to be cleared
 * @param[in] verify    comparison flag
 * @return              The check result.
 * @retval false        if the comparison succeeded.
 * @retval true         if the comparison failed.
 *
 * @xclass
 */
static inline bool seRegModify8X(volatile uint8_t *p,
                                 uint8_t setmask,
                                 uint8_t clrmask,
                                 bool verify) {
  uint8_t v = *p;
  v |= setmask;
  v &= ~clrmask;
  *p = v;

  if (verify) {
    return (*p & (uint8_t)(setmask | clrmask)) != setmask;
  }
  return false;
}

/**
 * @brief   Modifies a 16 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 *
 * @param[in] p         address of the register
 * @param[in] setmask   mask of bits to be set
 * @param[in] clrmask   mask of bits to be cleared
 * @param[in] verify    comparison flag
 * @return              The check result.
 * @retval false        if the comparison succeeded.
 * @retval true         if the comparison failed.
 *
 * @xclass
 */
static inline bool seRegModify16X(volatile uint16_t *p,
                                  uint16_t setmask,
                                  uint16_t clrmask,
                                  bool verify) {
  uint16_t v = *p;
  v |= setmask;
  v &= ~clrmask;
  *p = v;

  if (verify) {
    return (*p & (uint16_t)(setmask | clrmask)) != setmask;
  }
  return false;
}

/**
 * @brief   Modifies a 32 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 *
 * @param[in] p         address of the register
 * @param[in] setmask   mask of bits to be set
 * @param[in] clrmask   mask of bits to be cleared
 * @param[in] verify    comparison flag
 * @return              The check result.
 * @retval false        if the comparison succeeded.
 * @retval true         if the comparison failed.
 *
 * @xclass
 */
static inline bool seRegModify32X(volatile uint32_t *p,
                                  uint32_t setmask,
                                  uint32_t clrmask,
                                  bool verify) {
  uint32_t v = *p;
  v |= setmask;
  v &= ~clrmask;
  *p = v;

  if (verify) {
    return (*p & (uint32_t)(setmask | clrmask)) != setmask;
  }

  return false;
}

#endif /* SEREGS_H */

/** @} */
