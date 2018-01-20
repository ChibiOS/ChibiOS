/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio.

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
 * @file    smcclient.S
 * @brief   SMC client module code.
 *
 * @addtogroup SMC
 * @{
 */
                .section .text
                .code   32
                .balign 4
/*
 * r0 smc_service_t     handle
 * r1 smc_params_area_t data
 * r2 size_t            size
 *
 * r0 msg_t             result
 */
                .global smcInvoke
smcInvoke:
                smc     #0
                bx      lr
/** @} */

