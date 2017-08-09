/*
    ChibiOS - Copyright (C) 2017 Theodore Ateba

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
 * @file    GPIOv1/hal_pal_lld.c
 * @brief   AVR XMEGA GPIO low level driver source file.
 *
 * @addtogroup PAL
 * @{
 */

#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

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
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   AVR GPIO ports configuration.
 * @details GPIO registers initialization.
 *
 * @param[in] config    the XMEGA AVR ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {

#if defined(PORTA) || defined(__DOXYGEN__)
  PORTA.OUT = config->porta.out;
  PORTA.DIR = config->porta.dir;
#endif

#if defined(PORTB) || defined(__DOXYGEN__)
  PORTB.OUT = config->portb.out;
  PORTB.DIR = config->portb.dir;
#endif

#if defined(PORTC) || defined(__DOXYGEN__)
  PORTC.OUT = config->portc.out;
  PORTC.DIR = config->portc.dir;
#endif

#if defined(PORTD) || defined(__DOXYGEN__)
  PORTD.OUT = config->portd.out;
  PORTD.DIR = config->portd.dir;
#endif

#if defined(PORTE) || defined(__DOXYGEN__)
  PORTE.OUT = config->porte.out;
  PORTE.DIR = config->porte.dir;
#endif

#if defined(PORTF) || defined(__DOXYGEN__)
  PORTF.OUT = config->portf.out;
  PORTF.DIR = config->portf.dir;
#endif

#if defined(PORTG) || defined(__DOXYGEN__)
  PORTG.OUT = config->portg.out;
  PORTG.DIR = config->portg.dir;
#endif

#if defined(PORTH) || defined(__DOXYGEN__)
  PORTH.OUT = config->porth.out;
  PORTH.DIR = config->porth.dir;
#endif

#if defined(PORTJ) || defined(__DOXYGEN__)
  PORTJ.OUT = config->portj.out;
  PORTJ.DIR = config->portj.dir;
#endif

#if defined(PORTK) || defined(__DOXYGEN__)
  PORTK.OUT = config->portk.out;
  PORTK.DIR = config->portk.dir;
#endif

#if defined(PORTL) || defined(__DOXYGEN__)
  PORTL.OUT = config->portl.out;
  PORTL.DIR = config->portl.dir;
#endif
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port  the port identifier
 * @param[in] mask  the group mask
 * @param[in] mode  the mode
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note @p PAL_MODE_UNCONNECTED is implemented as output as recommended by
 *       the AVR Family User's Guide. Unconnected pads are set to input
 *       with pull-up by default.
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t     port,
                           ioportmask_t   mask,
                           iomode_t       mode) {

  switch (mode) {
  case PAL_MODE_RESET:
  case PAL_MODE_INPUT:
  case PAL_MODE_INPUT_ANALOG:
    port->DIRCLR  = mask;
    port->OUT     = mask;
    break;
  case PAL_MODE_UNCONNECTED:
  case PAL_MODE_INPUT_PULLUP:
    port->DIRCLR  = mask;
    port->OUT     = mask;
    break;
  case PAL_MODE_OUTPUT_PUSHPULL:
    port->DIRSET  = mask;
    break;
  }
}

#endif /* HAL_USE_PAL */

/** @} */

