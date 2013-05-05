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
 * @file    STM8S/pal_lld.c
 * @brief   STM8S GPIO low level driver code.
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
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as push pull output at 2MHz.
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
  case PAL_MODE_INPUT_PULLUP:
    port->DDR &= ~mask;
    port->CR1 |=  mask;
    port->CR2 &= ~mask;
    break;
  case PAL_MODE_INPUT:
  case PAL_MODE_INPUT_ANALOG:
    port->DDR &= ~mask;
    port->CR1 &= ~mask;
    port->CR2 &= ~mask;
    break;
  case PAL_MODE_UNCONNECTED:
  case PAL_MODE_OUTPUT_PUSHPULL_SLOW:
    port->DDR |=  mask;
    port->CR1 |=  mask;
    port->CR2 &= ~mask;
    break;
  case PAL_MODE_OUTPUT_PUSHPULL:
    port->DDR |=  mask;
    port->CR1 |=  mask;
    port->CR2 |=  mask;
    break;
  case PAL_MODE_OUTPUT_OPENDRAIN_SLOW:
    port->DDR |=  mask;
    port->CR1 &= ~mask;
    port->CR2 &= ~mask;
    break;
  case PAL_MODE_OUTPUT_OPENDRAIN:
    port->DDR |=  mask;
    port->CR1 &= ~mask;
    port->CR2 |=  mask;
    break;
  }
}

#endif /* HAL_USE_PAL */

/** @} */
