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

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Enables the safety-related features in HAL.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(HAL_USE_SAFETY) || defined(__DOXYGEN__)
#define HAL_USE_SAFETY                      FALSE
#endif

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
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] s         mask of bits to be set
 * @param[in] v         verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 *
 * @xclass
 */
#define halRegSet8X(p, s, v)        halRegModify8X(p, s, (uint8_t)0, v)

/**
 * @brief   Sets bits into a 16 bits register.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] s         mask of bits to be set
 * @param[in] v         verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 *
 * @xclass
 */
#define halRegSet16X(p, s, v)       halRegModify16X(p, s, (uint16_t)0, v)

/**
 * @brief   Sets bits into a 32 bits register.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] s         mask of bits to be set
 * @param[in] v         verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 *
 * @xclass
 */
#define halRegSet32X(p, s, v)       halRegModify32X(p, s, (uint32_t)0, v)

/**
 * @brief   Clears bits into an 8 bits register.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] c         mask of bits to be cleared
 * @param[in] v         verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 *
 * @xclass
 */
#define halRegClear8X(p, c, v)      halRegModify8X(p, (uint8_t)0, c, v)

/**
 * @brief   Clears bits into a 16 bits register.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] c         mask of bits to be cleared
 * @param[in] v         verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 *
 * @xclass
 */
#define halRegClear16X(p, c, v)     halRegModify16X(p, (uint16_t)0, c, v)

/**
 * @brief   Clears bits into a 32 bits register.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] c         mask of bits to be cleared
 * @param[in] v         verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
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
#if (HAL_USE_SAFETY == TRUE) || defined(__DOXYGEN__)
  bool halRegWaitAllSet8X(volatile uint8_t *p, uint8_t mask,
                          rtcnt_t tmo, uint8_t *valp);
  bool halRegWaitAllSet16X(volatile uint16_t *p, uint16_t mask,
                           rtcnt_t tmo, uint16_t *valp);
  bool halRegWaitAllSet32X(volatile uint32_t *p, uint32_t mask,
                           rtcnt_t tmo, uint32_t *valp);
  bool halRegWaitAnySet8X(volatile uint8_t *p, uint8_t mask,
                          rtcnt_t tmo, uint8_t *valp);
  bool halRegWaitAnySet16X(volatile uint16_t *p, uint16_t mask,
                           rtcnt_t tmo, uint16_t *valp);
  bool halRegWaitAnySet32X(volatile uint32_t *p, uint32_t mask,
                           rtcnt_t tmo, uint32_t *valp);
  bool halRegWaitAllClear8X(volatile uint8_t *p, uint8_t mask,
                            rtcnt_t tmo, uint8_t *valp);
  bool halRegWaitAllClear16X(volatile uint16_t *p, uint16_t mask,
                             rtcnt_t tmo, uint16_t *valp);
  bool halRegWaitAllClear32X(volatile uint32_t *p, uint32_t mask,
                             rtcnt_t tmo, uint32_t *valp);
  bool halRegWaitAnyClear8X(volatile uint8_t *p, uint8_t mask,
                            rtcnt_t tmo, uint8_t *valp);
  bool halRegWaitAnyClear16X(volatile uint16_t *p, uint16_t mask,
                             rtcnt_t tmo, uint16_t *valp);
  bool halRegWaitAnyClear32X(volatile uint32_t *p, uint32_t mask,
                             rtcnt_t tmo, uint32_t *valp);
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
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] setmask   mask of bits to be set
 * @param[in] clrmask   mask of bits to be cleared
 * @param[in] verify    verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 * @return              The check result.
 * @retval false        if the comparison succeeded.
 * @retval true         if the comparison failed.
 *
 * @xclass
 */
static inline bool halRegModify8X(volatile uint8_t *p,
                                  uint8_t setmask,
                                  uint8_t clrmask,
                                  bool verify) {
  uint8_t v = *p;
  v |= setmask;
  v &= ~clrmask;
  *p = v;

#if HAL_USE_SAFETY == FALSE
  (void)verify;
#else
  if (verify) {
    uint8_t check = *p;

    if ((check & setmask) != setmask) {
      return true;
    }
    if ((check & clrmask) != (uint8_t)0) {
      return true;
    }
  }
#endif

  return false;
}

/**
 * @brief   Modifies a 16 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] setmask   mask of bits to be set
 * @param[in] clrmask   mask of bits to be cleared
 * @param[in] verify    verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 * @return              The check result.
 * @retval false        if the comparison succeeded.
 * @retval true         if the comparison failed.
 *
 * @xclass
 */
static inline bool halRegModify16X(volatile uint16_t *p,
                                   uint16_t setmask,
                                   uint16_t clrmask,
                                   bool verify) {
  uint16_t v = *p;
  v |= setmask;
  v &= ~clrmask;
  *p = v;

#if HAL_USE_SAFETY == FALSE
  (void)verify;
#else
  if (verify) {
    uint16_t check = *p;

    if ((check & setmask) != setmask) {
      return true;
    }
    if ((check & clrmask) != (uint16_t)0) {
      return true;
    }
  }
#endif

 return false;
}

/**
 * @brief   Modifies a 32 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] setmask   mask of bits to be set
 * @param[in] clrmask   mask of bits to be cleared
 * @param[in] verify    verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 * @return              The check result.
 * @retval false        if the comparison succeeded.
 * @retval true         if the comparison failed.
 *
 * @xclass
 */
static inline bool halRegModify32X(volatile uint32_t *p,
                                   uint32_t setmask,
                                   uint32_t clrmask,
                                   bool verify) {
  uint32_t v = *p;
  v |= setmask;
  v &= ~clrmask;
  *p = v;

#if HAL_USE_SAFETY == FALSE
  (void)verify;
#else
  if (verify) {
    uint32_t check = *p;

    if ((check & setmask) != setmask) {
      return true;
    }
    if ((check & clrmask) != (uint32_t)0) {
      return true;
    }
  }
#endif

  return false;
}

/**
 * @brief   Writes a field into an 8 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] mask      mask of bits field to be written
 * @param[in] value     value to be written, must be pre-shifted to match the
 *                      mask
 * @param[in] verify    verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 * @return              The check result.
 * @retval false        if the comparison succeeded.
 * @retval true         if the comparison failed.
 *
 * @xclass
 */
static inline bool halRegMaskedWrite8X(volatile uint8_t *p,
                                       uint8_t mask,
                                       uint8_t value,
                                       bool verify) {
  uint8_t v;

  osalDbgCheck((value & ~mask) == 0U);

  v = *p;
  v &= ~mask;
  v |= value;
  *p = v;

#if HAL_USE_SAFETY == FALSE
  (void)verify;
#else
  if (verify) {
    uint8_t check = *p;

    if ((check & mask) != value) {
      return true;
    }
  }
#endif

  return false;
}

/**
 * @brief   Writes a field into a 16 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] mask      mask of bits field to be written
 * @param[in] value     value to be written, must be pre-shifted to match the
 *                      mask
 * @param[in] verify    verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 * @return              The check result.
 * @retval false        if the comparison succeeded.
 * @retval true         if the comparison failed.
 *
 * @xclass
 */
static inline bool halRegMaskedWrite16X(volatile uint16_t *p,
                                        uint16_t mask,
                                        uint16_t value,
                                        bool verify) {
  uint16_t v;

  osalDbgCheck((value & ~mask) == 0U);

  v= *p;
  v &= ~mask;
  v |= value;
  *p = v;

#if HAL_USE_SAFETY == FALSE
  (void)verify;
#else
  if (verify) {
    uint16_t check = *p;

    if ((check & mask) != value) {
      return true;
    }
  }
#endif

  return false;
}

/**
 * @brief   Writes a field into a 32 bits register.
 * @note    This code relies on compiler optimizations, the constant
 *          operations that cause no change to the final value are
 *          optimized out.
 * @note    There is a strong assumption for memory-mapped I/O, this is not
 *          necessarily true on all architectures.
 *
 * @param[in] p         pointer to the register
 * @param[in] mask      mask of bits field to be written
 * @param[in] value     value to be written, must be pre-shifted to match the
 *                      mask
 * @param[in] verify    verification flag, register is read back if @p true,
 *                      this flag only has effect if @p HAL_USE_SAFETY is
 *                      set to @p TRUE
 * @return              The check result.
 * @retval false        if the comparison succeeded.
 * @retval true         if the comparison failed.
 *
 * @xclass
 */
static inline bool halRegMaskedWrite32X(volatile uint32_t *p,
                                        uint32_t mask,
                                        uint32_t value,
                                        bool verify) {
  uint32_t v;

  osalDbgCheck((value & ~mask) == 0U);

  v = *p;
  v &= ~mask;
  v |= value;
  *p = v;

#if HAL_USE_SAFETY == FALSE
  (void)verify;
#else
  if (verify) {
    uint32_t check = *p;

    if ((check & mask) != value) {
      return true;
    }
  }
#endif

  return false;
}

#endif /* HAL_SAFETY_H */

/** @} */
