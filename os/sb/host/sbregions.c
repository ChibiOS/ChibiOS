/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    sb/host/sbregions.c
 * @brief   ARM SandBox memory regions code.
 *
 * @addtogroup ARM_SANDBOX_REGIONS
 * @{
 */

#include <string.h>

#include "sb.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

bool sb_is_valid_read_range(sb_class_t *sbp, const void *start, size_t size) {
  const sb_memory_region_t *rp = &sbp->regions[0];

  do {
    if (sb_reg_is_memory(rp) && chMemIsSpaceWithinX(&rp->area, start, size)) {
      return true;
    }
    rp++;
  } while (rp < &sbp->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

bool sb_is_valid_write_range(sb_class_t *sbp, void *start, size_t size) {
  const sb_memory_region_t *rp = &sbp->regions[0];

  do {
    if (sb_reg_is_memory(rp) && chMemIsSpaceWithinX(&rp->area, start, size)) {
      return sb_reg_is_writable(rp);
    }
    rp++;
  } while (rp < &sbp->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

size_t sb_check_string(sb_class_t *sbp, const char *s, size_t max) {
  const sb_memory_region_t *rp = &sbp->regions[0];

  do {
    if (sb_reg_is_memory(rp)) {
      size_t n = chMemIsStringWithinX(&rp->area, s, max);
      if (n > (size_t)0) {
        return n;
      }
    }
    rp++;
  } while (rp < &sbp->regions[SB_CFG_NUM_REGIONS]);

  return (size_t)0;
}

size_t sb_check_pointers_array(sb_class_t *sbp, const void *pp[], size_t max) {
  const sb_memory_region_t *rp = &sbp->regions[0];

  do {
    if (sb_reg_is_memory(rp)) {
      size_t an = chMemIsPointersArrayWithinX(&rp->area, pp, max);
      if (an > (size_t)0) {
        return an;
      }
    }
    rp++;
  } while (rp < &sbp->regions[SB_CFG_NUM_REGIONS]);

  return (size_t)0;
}

size_t sb_check_strings_array(sb_class_t *sbp, const char *pp[], size_t max) {
  const char *s;
  size_t n;

  n = sb_check_pointers_array(sbp, (const void **)pp, max);
  if (n > (size_t)0) {
    while ((s = *pp++) != NULL) {
      size_t sn;

      sn = sb_check_string(sbp, s, max - n);
      if (sn == (size_t)0) {
        return (size_t)0;
      }

      n += sn;
    }
  }

  return n;
}

/** @} */
