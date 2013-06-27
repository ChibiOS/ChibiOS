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
*/

/**
 * @file    templates/adc_lld.c
 * @brief   ADC Driver subsystem low level driver source template.
 *
 * @addtogroup ADC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
/** @brief ADC1 driver identifier.*/
#if AVR_ADC_USE_ADC1 || defined(__DOXYGEN__)
ADCDriver ADCD1;
#endif
/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static size_t getAdcChannelNumberFromMask(uint8_t mask, uint8_t currentChannel)
{
     for(uint8_t i = 0; mask>0; i++)
     {
	if(mask & 0x01)
	  {
	      if(!currentChannel)
		return i;
	      currentChannel--;
	  }
        mask >>= 1;
    }
     
     /* error, should never reach this line */
}

static void setAdcChannel(uint8_t channelNum)
{
	ADMUX = (ADMUX & 0xf8) | (channelNum & 0x07);
}
/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#include <util/delay.h>

CH_IRQ_HANDLER(ADC_vect) {
 
  CH_IRQ_PROLOGUE();
	uint8_t low = ADCL;
	uint8_t high = ADCH;
	uint16_t result = (high << 8) | low;
	
	ADCD1.samples[ADCD1.currentBufferPosition] = result;
	ADCD1.currentBufferPosition++;
	
	size_t bufferSize = ADCD1.depth * ADCD1.grpp->num_channels;
	size_t currentChannel = ADCD1.currentBufferPosition % ADCD1.grpp->num_channels;
	size_t currentIteration = ADCD1.currentBufferPosition / ADCD1.grpp->num_channels;
	if(ADCD1.grpp-> circular && currentChannel == 0 && currentIteration == ADCD1.depth/2)
	{
	  _adc_isr_half_code(&ADCD1);
	  
	}
	
	if(ADCD1.currentBufferPosition == bufferSize)
	{
	  _adc_isr_full_code(&ADCD1);
	}
	else
	{
	  
	  setAdcChannel(getAdcChannelNumberFromMask(ADCD1.grpp->channelsMask,currentChannel));
	  ADCSRA |= 1<<ADSC;
	}
 
  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {
    adcObjectInit(&ADCD1);
    ADCSRA =(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) |   //prescaler 128, unico valore possibile a 20Mhz
	     (1<<ADIE) ; 	//interrupt

    ADCSRB=0; //single shot
    ADMUX=(0<<REFS1)| (0<<REFS0); //uso aref, vale solo per arduino. arduino ha aref collegato
}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start(ADCDriver *adcp) {

  if (adcp->state == ADC_STOP) {
    /* Clock activation.*/
    ADCSRA |= (1<<ADEN);
  }
  
  if (adcp->config != NULL)    
  {
      ADMUX = (adcp->config->analog_reference << REFS0);
  }
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop(ADCDriver *adcp) {

  if (adcp->state == ADC_READY) {
    /* Clock de-activation.*/
    ADCSRA &= ~(1<<ADEN);  
  }
}

/**
 * @brief   Starts an ADC conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start_conversion(ADCDriver *adcp) {
	adcp->currentBufferPosition=0;
	
	setAdcChannel(getAdcChannelNumberFromMask(adcp->grpp->channelsMask,0));
	ADCSRA |= 1<<ADSC;
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {
    ADCSRA &= ~(1<<ADSC);
}

#endif /* HAL_USE_ADC */

/** @} */
