/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    sb/host/sbregions.h
 * @brief   ARM SandBox host macros and structures.
 *
 * @addtogroup ARM_SANDBOX_REGIONS
 * @{
 */

#ifndef SBREGIONS_H
#define SBREGIONS_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Memory regions types
 * @{
 */
#define SB_REG_TYPE_MASK        3U
#define SB_REG_TYPE_UNUSED      0U
#define SB_REG_TYPE_MEMORY      1U
#define SB_REG_TYPE_DEVICE      2U

/* Extra attributes regions.*/
#define SB_REG_ATTR_WRITABLE    (1U << 8)
#define SB_REG_ATTR_EXECUTABLE  (1U << 9)   /* Only for SB_REG_TYPE_MEMORY.*/
#define SB_REG_ATTR_CACHEABLE   (1U << 10)  /* Only for SB_REG_TYPE_MEMORY.*/

/* Derived region types.*/
#define SB_REG_IS_UNUSED        (SB_REG_TYPE_UNUSED)
#define SB_REG_IS_CODE          (SB_REG_TYPE_MEMORY |                       \
                                 SB_REG_ATTR_CACHEABLE |                    \
                                 SB_REG_ATTR_EXECUTABLE)
#define SB_REG_IS_DATA          (SB_REG_TYPE_MEMORY |                       \
                                 SB_REG_ATTR_CACHEABLE |                    \
                                 SB_REG_ATTR_WRITABLE)
#define SB_REG_IS_NOCACHE_DATA  (SB_REG_TYPE_MEMORY |                       \
                                 SB_REG_ATTR_WRITABLE)
#define SB_REG_IS_CODE_AND_DATA (SB_REG_TYPE_MEMORY |                       \
                                 SB_REG_ATTR_CACHEABLE |                    \
                                 SB_REG_ATTR_EXECUTABLE |                   \
                                 SB_REG_ATTR_WRITABLE)
#define SB_REG_IS_DEVICE        (SB_REG_TYPE_DEVICE |                       \
                                 SB_REG_ATTR_WRITABLE)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a sandbox memory region.
 */
typedef struct {
  /**
   * @brief   Associated memory area.
   */
  memory_area_t                 area;
  /**
   * @brief   Region attributes.
   */
  uint32_t                      attributes;
} sb_memory_region_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Memory regions attributes checks
 * @{
 */
#define sb_reg_get_type(r)      ((r)->attributes & SB_REG_TYPE_MASK)
#define sb_reg_is_unused(r)     (bool)(sb_reg_get_type(r) == SB_REG_TYPE_UNUSED)
#define sb_reg_is_device(r)     (bool)(sb_reg_get_type(r) == SB_REG_TYPE_DEVICE)
#define sb_reg_is_memory(r)     (bool)(sb_reg_get_type(r) == SB_REG_TYPE_MEMORY)
#define sb_reg_is_writable(r)   (bool)(((r)->attributes & SB_REG_ATTR_WRITABLE) != 0U)
#define sb_reg_is_executable(r) (bool)(((r)->attributes & SB_REG_ATTR_EXECUTABLE) != 0U)
#define sb_reg_is_cacheable(r)  (bool)(((r)->attributes & SB_REG_ATTR_CACHEABLE) != 0U)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  bool sb_is_valid_read_range(sb_class_t *sbp, const void *start, size_t size);
  bool sb_is_valid_write_range(sb_class_t *sbp, void *start, size_t size);
  size_t sb_check_string(sb_class_t *sbp, const char *s, size_t max);
  size_t sb_check_pointers_array(sb_class_t *sbp, const void *pp[], size_t max);
  size_t sb_check_strings_array(sb_class_t *sbp, const char *pp[], size_t max);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SBREGIONS_H */

/** @} */
