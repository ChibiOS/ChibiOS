/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio.

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
 * @file    sb/common/sberr.h
 * @brief   ARMv7-M sandbox common macros and structures.
 *
 * @addtogroup ARM_SANDBOX_ERRORS
 * @{
 */

#ifndef SBERR_H
#define SBERR_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Sandbox API error codes
 * @{
 */
#define SB_ERR_NOERROR          0U
#define SB_ERR_NOT_IMPLEMENTED  ((uint32_t)(-88))   /* ENOSYS */
#define SB_ERR_MEMORY_FAULT     ((uint32_t)(-14))   /* EFAULT */
#define SB_ERR_API_USAGE        ((uint32_t)(-16))   /* EBUSY */
#define SB_ERR_INV_ARGUMENT     ((uint32_t)(-22))   /* EINVAL */

#define SB_ERR_ERRORMASK        0xFFFFFF00U
#define SB_ERR_ISERROR(x)       (((x) & SB_ERR_ERRORMASK) == SB_ERR_ERRORMASK)
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

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

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

#endif /* SBERR_H */

/** @} */
