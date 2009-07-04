/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
*/

/**
 * @file ports/ARMCM3-STM32F103/pal_lld.c
 * @brief STM32 GPIO low level driver code
 * @addtogroup STM32F103_PAL
 * @{
 */

#include <ch.h>
#include <pal.h>

/**
 * @brief STM32 I/O ports configuration.
 * @details Ports A-D clocks enabled, AFIO clock enabled.
 *
 * @param[in] config the STM32 ports configuration
 */
void _pal_lld_init(const STM32GPIOConfig *config) {
  uint32_t clocks = RCC_APB2ENR_AFIOEN;

#if defined(_GPIOA)
  clocks |= RCC_APB2ENR_IOPAEN;
#endif
#if defined(_GPIOB)
  clocks |= RCC_APB2ENR_IOPBEN;
#endif
#if defined(_GPIOC)
  clocks |= RCC_APB2ENR_IOPCEN;
#endif
#if defined(_GPIOD)
  clocks |= RCC_APB2ENR_IOPDEN;
#endif
#if defined(_GPIOE)
  clocks |= RCC_APB2ENR_IOPEEN;
#endif
  RCC->APB2ENR |= clocks;

  /*
   * Resets the GPIO ports and AFIO.
   */
  RCC->APB2RSTR = RCC_APB2RSTR_IOPARST | RCC_APB2RSTR_IOPBRST |
                  RCC_APB2RSTR_IOPCRST | RCC_APB2RSTR_IOPDRST |
                  RCC_APB2RSTR_IOPERST | RCC_APB2RSTR_IOPFRST |
                  RCC_APB2RSTR_IOPGRST | RCC_APB2RSTR_AFIORST;
  RCC->APB2RSTR = 0;

#if defined(_GPIOA)
  IOPORT_A->ODR = config->PAData.odr;
  IOPORT_A->CRH = config->PAData.crh;
  IOPORT_A->CRL = config->PAData.crl;
#endif

#if defined(_GPIOB)
  IOPORT_B->ODR = config->PBData.odr;
  IOPORT_B->CRH = config->PBData.crh;
  IOPORT_B->CRL = config->PBData.crl;
#endif

#if defined(_GPIOC)
  IOPORT_C->ODR = config->PCData.odr;
  IOPORT_C->CRH = config->PCData.crh;
  IOPORT_C->CRL = config->PCData.crl;
#endif

#if defined(_GPIOD)
  IOPORT_D->ODR = config->PDData.odr;
  IOPORT_D->CRH = config->PDData.crh;
  IOPORT_D->CRL = config->PDData.crl;
#endif

#if defined(_GPIOE)
  IOPORT_E->ODR = config->PEData.odr;
  IOPORT_E->CRH = config->PEData.crh;
  IOPORT_E->CRL = config->PEData.crl;
#endif

#if defined(_GPIOF)
  IOPORT_F->ODR = config->PFData.odr;
  IOPORT_F->CRH = config->PFData.crh;
  IOPORT_F->CRL = config->PFData.crl;
#endif

#if defined(_GPIOG)
  IOPORT_G->ODR = config->PGData.odr;
  IOPORT_G->CRH = config->PGData.crh;
  IOPORT_G->CRL = config->PGData.crl;
#endif
}

/**
 * @brief Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port the port identifier
 * @param[in] mask the group mask
 * @param[in] mode the mode
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note @p PAL_MODE_UNCONNECTED is implemented as push pull output at 2MHz.
 * @note Writing on pads programmed as pull-up or pull-down has the side
 *       effect to modify the resistor setting because the output latched data
 *       is used for the resistor selection.
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           uint_fast8_t mode) {
  static const uint8_t cfgtab[] = {
    4,          /* PAL_MODE_RESET, implemented as input.*/
    2,          /* PAL_MODE_UNCONNECTED, implemented as push pull output 2MHz.*/
    4,          /* PAL_MODE_INPUT */
    8,          /* PAL_MODE_INPUT_PULLUP */
    8,          /* PAL_MODE_INPUT_PULLDOWN */
    3,          /* PAL_MODE_OUTPUT_PUSHPULL, 50MHz.*/
    7,          /* PAL_MODE_OUTPUT_OPENDRAIN, 50MHz.*/
  };
  uint32_t mh, ml, crh, crl, cfg;
  unsigned i;

  if (mode == PAL_MODE_INPUT_PULLUP)
    port->BSRR = mask;
  else if (mode == PAL_MODE_INPUT_PULLDOWN)
    port->BRR = mask;
  cfg = cfgtab[mode];
  mh = ml = crh = crl = 0;
  for (i = 0; i < 8; i++) {
    ml <<= 4;
    mh <<= 4;
    crl <<= 4;
    crh <<= 4;
    if ((mask & 1) == 0)
      ml |= 0xf;
    else
      crl |= cfg;
    if ((mask & 0x10000) == 0)
      mh |= 0xf;
    else
      crh |= cfg;
    mask >>= 1;
  }
  port->CRH = (port->CRH & mh) | crh;
  port->CRL = (port->CRL & ml) | crl;
}

/** @} */
