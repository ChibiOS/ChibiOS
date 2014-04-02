/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    RX62N/pal_lld.c
 * @brief   RX62N GPIO low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

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

static void odrset(ioportid_t port, ioportmask_t mask) {

#if defined(RX62N_HAS_PORT0_OD)
    if ( port == (ioportid_t)&PORT0 )
    {
	  port->ODR |= mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORT1_OD)
    if ( port == (ioportid_t)&PORT1 )
    {
	  port->ODR |= mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORT2_OD)
    if ( port == (ioportid_t)&PORT2 )
    {
	  port->ODR |= mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORT3_OD)
    if ( port == (ioportid_t)&PORT3 )
    {
	  port->ODR |= mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTC_OD)
    if ( port == (ioportid_t)&PORTC )
    {
	  port->ODR |= mask;
	  return;
    }
#endif
}

static void odrclear(ioportid_t port, ioportmask_t mask) {

#if defined(RX62N_HAS_PORT0_OD)
    if ( port == (ioportid_t)&PORT0 )
    {
	  port->ODR &= ~mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORT1_OD)
    if ( port == (ioportid_t)&PORT1 )
    {
	  port->ODR &= ~mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORT2_OD)
    if ( port == (ioportid_t)&PORT2 )
    {
	  port->ODR &= ~mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORT3_OD)
    if ( port == (ioportid_t)&PORT3 )
    {
	  port->ODR &= ~mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTC_OD)
    if ( port == (ioportid_t)&PORTC )
    {
	  port->ODR &= ~mask;
	  return;
    }
#endif
}

static void pcrset(ioportid_t port, ioportmask_t mask) {
#if defined(RX62N_HAS_PORTA_PU)
    if ( port == (ioportid_t)&PORTA )
    {
	  port->PCR |= mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTB_PU)
    if ( port == (ioportid_t)&PORTB )
    {
	  port->PCR |= mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTC_PU)
    if ( port == (ioportid_t)&PORTC )
    {
	  port->PCR |= mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTD_PU)
    if ( port == (ioportid_t)&PORTD )
    {
	  port->PCR |= mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTE_PU)
    if ( port == (ioportid_t)&PORTE )
    {
	  port->PCR |= mask;
	  return;
    }
#endif
}

static void pcrclear(ioportid_t port, ioportmask_t mask) {
#if defined(RX62N_HAS_PORTA_PU)
    if ( port == (ioportid_t)&PORTA )
    {
	  port->PCR &= ~mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTB_PU)
    if ( port == (ioportid_t)&PORTB )
    {
	  port->PCR &= ~mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTC_PU)
    if ( port == (ioportid_t)&PORTC )
    {
	  port->PCR &= ~mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTD_PU)
    if ( port == (ioportid_t)&PORTD )
    {
	  port->PCR &= ~mask;
	  return;
    }
#endif
#if defined(RX62N_HAS_PORTE_PU)
    if ( port == (ioportid_t)&PORTE )
    {
	  port->PCR &= ~mask;
	  return;
    }
#endif
}

static void initgpio(GPIO_TypeDef *gpiop, const rx62n_gpio_setup_t *config) {

  gpiop->DDR     = config->ddr;
  gpiop->DR      = config->dr;
  gpiop->ICR     = config->icr;
/*
  gpiop->ODR     = config->odr;
  gpiop->PCR     = config->pcr;
*/
  odrset(gpiop, config->odr);
  pcrset(gpiop, config->pcr);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   RX62N I/O ports configuration.
 * @param[in] config    the RX62N ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {

  /*
   * Enables the GPIO related clocks (always enabled).
   */

  /*
   * Initial GPIO setup.
   */
  initgpio((GPIO_TypeDef *)&PORT0, &config->P0Data);
  initgpio((GPIO_TypeDef *)&PORT1, &config->P1Data);
  initgpio((GPIO_TypeDef *)&PORT2, &config->P2Data);
  initgpio((GPIO_TypeDef *)&PORT3, &config->P3Data);
  initgpio((GPIO_TypeDef *)&PORT4, &config->P4Data);
  initgpio((GPIO_TypeDef *)&PORT5, &config->P5Data);
#if RX62N_HAS_PORT6
  initgpio((GPIO_TypeDef *)&PORT6, &config->P6Data);
#endif
#if RX62N_HAS_PORT7
  initgpio((GPIO_TypeDef *)&PORT7, &config->P7Data);
#endif
#if RX62N_HAS_PORT8
  initgpio((GPIO_TypeDef *)&PORT8, &config->P8Data);
#endif
#if RX62N_HAS_PORT9
  initgpio((GPIO_TypeDef *)&PORT9, &config->P9Data);
#endif
  initgpio((GPIO_TypeDef *)&PORTA, &config->PAData);
  initgpio((GPIO_TypeDef *)&PORTB, &config->PBData);
  initgpio((GPIO_TypeDef *)&PORTC, &config->PCData);
  initgpio((GPIO_TypeDef *)&PORTD, &config->PDData);
#if RX62N_HAS_PORTE
  initgpio((GPIO_TypeDef *)&PORTE, &config->PEData);
#endif
#if RX62N_HAS_PORTF
  initgpio((GPIO_TypeDef *)&PORTF, &config->PFData);
#endif
#if RX62N_HAS_PORTG
  initgpio((GPIO_TypeDef *)&PORTG, &config->PGData);
#endif
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as input (needs external
               pull-up or pull-down resistor).
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
  case PAL_MODE_UNCONNECTED:
    port->DDR &= ~mask;
    pcrclear(port, mask);
	break;
  case PAL_MODE_INPUT_PULLUP:
    port->DDR &= ~mask;
    pcrset(port, mask);
    break;
  case PAL_MODE_OUTPUT_PUSHPULL:
    port->DDR |= mask;
    odrclear(port, mask);
    break;
  case PAL_MODE_OUTPUT_OPENDRAIN:
    port->DDR |= mask;
    odrset(port, mask);
    break;
  }
}

#endif /* HAL_USE_PAL */

/** @} */
