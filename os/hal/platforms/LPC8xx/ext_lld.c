/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * @file    LPC8xx/ext_lld.c
 * @brief   LPC8xx EXT subsystem low level driver source.
 *
 * @addtogroup EXT
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_EXT || defined(__DOXYGEN__)

#include "ext_lld_isr.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EXTD1 driver identifier.
 */
EXTDriver EXTD1;

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
 * @brief   Low level EXT driver initialization.
 *
 * @notapi
 */
void ext_lld_init(void) {

  /* Driver initialization.*/
  extObjectInit(&EXTD1);
}

/**
 * @brief   Configures and activates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_start(EXTDriver *extp) {
  int i;

  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
  /* Configuration of automatic channels.*/
  for (i = 0; i < EXT_MAX_CHANNELS; i++)
    if (extp->config->channels[i].mode & EXT_CH_MODE_AUTOSTART)
      ext_lld_channel_enable(extp, i);
    else
      ext_lld_channel_disable(extp, i);
}

/**
 * @brief   Deactivates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_stop(EXTDriver *extp) {
  int i;

  if (extp->state == EXT_ACTIVE)
    for (i = 0; i < EXT_MAX_CHANNELS; i++)
      ext_lld_exti_irq_disable(i);

  LPC_PIN_INT->ISEL  = 0;
  LPC_PIN_INT->CIENR = EXT_CHANNELS_MASK;
  LPC_PIN_INT->RISE  = EXT_CHANNELS_MASK;
  LPC_PIN_INT->FALL  = EXT_CHANNELS_MASK;
  LPC_PIN_INT->IST   = EXT_CHANNELS_MASK;

  /* Leave clock enabled, its shared with GPIO */
  /*LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<6);*/
}

/**
 * @brief   Enables an EXT channel.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be enabled
 *
 * @notapi
 */
void ext_lld_channel_enable(EXTDriver *extp, expchannel_t channel) {

  /* program the IOpin for this channel */
  LPC_SYSCON->PINTSEL[channel] = extp->config->channels[channel].iopin;
  
  /* Programming edge irq enables */
  if (extp->config->channels[channel].mode & EXT_CH_MODE_RISING_EDGE)
    LPC_PIN_INT->SIENR =  (1 << channel);
  else
    LPC_PIN_INT->CIENR =  (1 << channel);
    
  if (extp->config->channels[channel].mode & EXT_CH_MODE_FALLING_EDGE)
    LPC_PIN_INT->SIENF =  (1 << channel);
  else
    LPC_PIN_INT->CIENF =  (1 << channel);

  LPC_PIN_INT->RISE = (1<<channel);
  LPC_PIN_INT->FALL = (1<<channel);
  LPC_PIN_INT->IST  = (1<<channel);
  
  ext_lld_exti_irq_enable( channel );
}

/**
 * @brief   Disables an EXT channel.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be disabled
 *
 * @notapi
 */
void ext_lld_channel_disable(EXTDriver *extp, expchannel_t channel) {
  (void)extp;

  ext_lld_exti_irq_disable(channel);

  LPC_PIN_INT->ISEL &= ~(1 << channel);
  LPC_PIN_INT->CIENR =  (1 << channel);
  LPC_PIN_INT->RISE  =  (1 << channel);
  LPC_PIN_INT->FALL  =  (1 << channel);
  LPC_PIN_INT->IST   =  (1 << channel);
}

#endif /* HAL_USE_EXT */

/** @} */
