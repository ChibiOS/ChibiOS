/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
#include "hal.h"
#include "sama_trng_lld.h"


void trng_lld_init(void) {
   	  pmcEnableTRNG();
}

void trng_lld_start(void) {

  TRNG->TRNG_CR = TRNG_CR_ENABLE | TRNG_CR_KEY_PASSWD;
  //enable interrupt
 // TRNG->TRNG_IER = TRNG_IER_DATRDY;


}

void trng_lld_getrandom(uint32_t * random) {

	//generate a random every 84 clock cycles.
  while (!(TRNG->TRNG_ISR & TRNG_ISR_DATRDY));

  *random = TRNG->TRNG_ODATA;

}

void trng_lld_stop(void) {
	//disable interrupt
	//TRNG->TRNG_IDR = TRNG_IDR_DATRDY;
	TRNG->TRNG_CR = TRNG_CR_KEY_PASSWD;
	pmcDisableTRNG();
}






