/*
    N25Q128 Flash Driver - Copyright (C) 2016 Giovanni Di Sirio

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
 * @file    n25q128.c
 * @brief   N25Q128 flash interface module code.
 *
 * @addtogroup n25q128
 * @{
 */

#include "hal.h"

#include "n25q128.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] devp     pointer to the @p N25Q128Driver object
 *
 * @init
 */
void n15q128ObjectInit(N25Q128Driver *devp) {

}

/**
 * @brief   Configures and activates N25Q128 driver.
 *
 * @param[in] devp      pointer to the @p N25Q128Driver object
 * @param[in] config    pointer to the configuration
 *
 * @api
 */
void n15q128Start(N25Q128Driver *devp, const N25Q128Config *config) {

} 

/**
 * @brief   Deactivates the N25Q128 driver.
 *
 * @param[in] devp       pointer to the @p N25Q128Driver object
 *
 * @api
 */
void n15q128Stop(N25Q128Driver *devp) {

}

/** @} */
