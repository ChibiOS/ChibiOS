/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file    hal_safety.h
 * @brief   Safety manager macros and structures.
 *
 * @addtogroup HAL_SAFETY
 * @{
 */

#ifndef HAL_SAFETY_H
#define HAL_SAFETY_H

#if (HAL_USE_SAFETY == TRUE) || defined(__DOXYGEN__)

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

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Function variants
 * @{
 */
/**
 * @brief   Sets bits into an 8 bits register.
 *
 * @param[in] p         pointer to the register
 * @param[in] s         mask of bits to be set
 * @param[in] v         verification flag, register is read back if @p true
 *
 * @xclass
 */
#define halRegSet8X(p, s, v)        halRegModify8X(p, s, (uint8_t)0, v)

/**
 * @brief   Sets bits into a 16 bits register.
 *
 * @param[in] p         pointer to the register
 * @param[in] s         mask of bits to be set
 * @param[in] v         verification flag, register is read back if @p true
 *
 * @xclass
 */
#define halRegSet16X(p, s, v)       halRegModify16X(p, s, (uint16_t)0, v)

/**
 * @brief   Sets bits into a 32 bits register.
 *
 * @param[in] p         pointer to the register
 * @param[in] s         mask of bits to be set
 * @param[in] v         verification flag, register is read back if @p true
 *
 * @xclass
 */
#define halRegSet32X(p, s, v)       halRegModify32X(p, s, (uint32_t)0, v)

/**
 * @brief   Clears bits into an 8 bits register.
 *
 * @param[in] p         pointer to the register
 * @param[in] c         mask of bits to be cleared
 * @param[in] v         verification flag, register is read back if @p true
 *
 * @xclass
 */
#define halRegClear8X(p, c, v)      halRegModify8X(p, (uint8_t)0, c, v)

/**
 * @brief   Clears bits into a 16 bits register.
 *
 * @param[in] p         pointer to the register
 * @param[in] c         mask of bits to be cleared
 * @param[in] v         verification flag, register is read back if @p true
 *
 * @xclass
 */
#define halRegClear16X(p, c, v)     halRegModify16X(p, (uint16_t)0, c, v)

/**
 * @brief   Clears bits into a 32 bits register.
 *
 * @param[in] p         pointer to the register
 * @param[in] c         mask of bits to be cleared
 * @param[in] v         verification flag, register is read back if @p true
 *
 * @xclass
 */
#define halRegClear32X(p, c, v)     halRegModify32X(p, (uint32_t)0, c, v)
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
/* Inline functions.                                                         */
/*===========================================================================*/

/**
 * @brief   Modifies a 8 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 *
 * @param[in] p         pointer to the register
 * @param[in] setmask   mask of bits to be set
 * @param[in] clrmask   mask of bits to be cleared
 * @param[in] verify    verification flag, register is read back if @p true
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
    uint8_t check = *p;

    if ((check & setmask) != setmask) {
      return true;
    }
    if ((check & clrmask) != (uint8_t)0) {
      return true;
    }
    return false;
  }
  return false;
}

/**
 * @brief   Modifies a 16 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 *
 * @param[in] p         pointer to the register
 * @param[in] setmask   mask of bits to be set
 * @param[in] clrmask   mask of bits to be cleared
 * @param[in] verify    verification flag, register is read back if @p true
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
    uint16_t check = *p;

    if ((check & setmask) != setmask) {
      return true;
    }
    if ((check & clrmask) != (uint16_t)0) {
      return true;
    }
    return false;
  }
  return false;
}

/**
 * @brief   Modifies a 32 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 *
 * @param[in] p         pointer to the register
 * @param[in] setmask   mask of bits to be set
 * @param[in] clrmask   mask of bits to be cleared
 * @param[in] verify    verification flag, register is read back if @p true
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
    uint32_t check = *p;

    if ((check & setmask) != setmask) {
      return true;
    }
    if ((check & clrmask) != (uint32_t)0) {
      return true;
    }
    return false;
  }

  return false;
}

#endif /* HAL_USE_SAFETY == TRUE */

#endif /* HAL_SAFETY_H */

/** @} */
