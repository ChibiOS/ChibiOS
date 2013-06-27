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

#include "ch.h"
#include "hal.h"
#include "chprintf.h"



/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  
  palSetPadMode(IOPORT2, 0, PAL_MODE_OUTPUT_PUSHPULL);
  
  sdStart(&SD1, NULL);

  #define ADC_GRP1_NUM_CHANNELS   3
  #define ADC_GRP1_BUF_DEPTH      10
    DDRD |= _BV(DDD5);  
    
  static adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
  static const ADCConversionGroup adcgrpcfg = {
    FALSE,
    ADC_GRP1_NUM_CHANNELS,
    NULL,
    0b00000111                        /*   enabled channels                */
  };

  ADCConfig adccfg = {
    ANALOG_REFERENCE_AVCC /* use the AVCC pin as the ADC reference voltage */
  };

  adcStart(&ADCD1, &adccfg);
  
  palSetGroupMode(IOPORTADC, 0b00000111, 0, PAL_MODE_INPUT_PULLUP);
  

  while(TRUE)
  {
	int i;
    adcConvert(&ADCD1, &adcgrpcfg, samples, ADC_GRP1_BUF_DEPTH);
      
    chprintf((BaseSequentialStream *) &SD1,"Result:\r\n");
    for(i=0; i < (ADC_GRP1_BUF_DEPTH * ADC_GRP1_NUM_CHANNELS); i++) {
      if((i % ADC_GRP1_NUM_CHANNELS) == 0) chprintf((BaseSequentialStream *) &SD1,"\r\n");
      chprintf((BaseSequentialStream *) &SD1," %d", samples[i]);
    }
     
    chprintf((BaseSequentialStream *) &SD1,"\r\n");
    chThdSleepMilliseconds(500);
	palTogglePad(IOPORT2, PORTB_LED1);
  }
}
