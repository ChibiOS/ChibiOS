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
 * @file    MSP430/pal_lld.c
 * @brief   MSP430 Digital I/O low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   MSP430 I/O ports configuration.
 * @note    The @p PxIFG, @p PxIE and @p PxSEL registers are cleared. @p PxOUT
 *          and @p PxDIR are configured as specified.
 *
 * @param[in] config the MSP430 ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {

#if defined(__MSP430_HAS_PORT1__) || defined(__MSP430_HAS_PORT1_R__)
  IOPORT1->iop_full.ie.reg_p = 0;
  IOPORT1->iop_full.ifg.reg_p = 0;
  IOPORT1->iop_full.sel.reg_p = 0;
  IOPORT1->iop_common.out = config->P1Data.out;
  IOPORT1->iop_common.dir = config->P1Data.dir;
#endif

#if defined(__MSP430_HAS_PORT2__) || defined(__MSP430_HAS_PORT2_R__)
  IOPORT2->iop_full.ie.reg_p = 0;
  IOPORT2->iop_full.ifg.reg_p = 0;
  IOPORT2->iop_full.sel.reg_p = 0;
  IOPORT2->iop_common.out = config->P2Data.out;
  IOPORT2->iop_common.dir = config->P2Data.dir;
#endif

#if defined(__MSP430_HAS_PORT3__) || defined(__MSP430_HAS_PORT3_R__)
  IOPORT3->iop_simple.sel.reg_p = 0;
  IOPORT3->iop_common.out = config->P3Data.out;
  IOPORT3->iop_common.dir = config->P3Data.dir;
#endif

#if defined(__MSP430_HAS_PORT4__) || defined(__MSP430_HAS_PORT4_R__)
  IOPORT4->iop_simple.sel.reg_p = 0;
  IOPORT4->iop_common.out = config->P4Data.out;
  IOPORT4->iop_common.dir = config->P4Data.dir;
#endif

#if defined(__MSP430_HAS_PORT5__) || defined(__MSP430_HAS_PORT5_R__)
  IOPORT5->iop_simple.sel.reg_p = 0;
  IOPORT5->iop_common.out = config->P5Data.out;
  IOPORT5->iop_common.dir = config->P5Data.dir;
#endif

#if defined(__MSP430_HAS_PORT6__) || defined(__MSP430_HAS_PORT6_R__)
  IOPORT6->iop_simple.sel.reg_p = 0;
  IOPORT6->iop_common.out = config->P6Data.out;
  IOPORT6->iop_common.dir = config->P6Data.dir;
#endif
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as output as recommended by
 *          the MSP430x1xx Family User's Guide. Unconnected pads are set to
 *          high logic state by default.
 * @note    This function does not alter the @p PxSEL registers. Alternate
 *          functions setup must be handled by device-specific code.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {

  switch (mode) {
  case PAL_MODE_RESET:
  case PAL_MODE_INPUT:
    port->iop_common.dir.reg_p &= ~mask;
    break;
  case PAL_MODE_UNCONNECTED:
    port->iop_common.out.reg_p |= mask;
  case PAL_MODE_OUTPUT_PUSHPULL:
    port->iop_common.dir.reg_p |= mask;
    break;
  }
}

#endif /* HAL_USE_PAL */

/** @} */
