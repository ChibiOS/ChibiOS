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

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config = {
 {VAL_GPIO0DATA, VAL_GPIO0DIR},
 {VAL_GPIO1DATA, VAL_GPIO1DIR},
 {VAL_GPIO2DATA, VAL_GPIO2DIR},
 {VAL_GPIO3DATA, VAL_GPIO3DIR},
 {VAL_GPIO4DATA, VAL_GPIO4DIR}
};
#endif

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

  LPC17xx_clock_init();
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  /*
   * Extra, board-specific, initializations.
   */
  LPC_PINCON->PINSEL0  |= (1UL << 4) | (1UL << 6); 	    /* Set UART0 TXD0 P0.2 and RXD0 P0.3 pins.*/
  LPC_PINCON->PINMODE0 |= (2UL << 4) | (2UL << 6); 	    /* Disable pull-up on UART0 TXD0 and RXD0 pins.*/

  LPC_PINCON->PINSEL3  |= (1UL << 22); 	                /* Set CLKOUT P1.27 pin.*/
  LPC_PINCON->PINMODE3 |= (2UL << 22); 	                /* Disable pull-up on CLKOUT pin. */

  /* I2C1 config */
  LPC_PINCON->PINSEL1 |= (3UL << 8) | (3UL << 6);       /* Set I2C1 SCL1 P0.20, SDA1 P0.19 pins. */
  LPC_PINCON->PINMODE1 |= (2UL << 8) | (2UL << 6);      /* Disable pull-up on I2C1 SCL1 P0.20, SDA1 P0.19 pins. */
  LPC_PINCON->PINMODE_OD0 |= (1UL << 20) | (1UL << 19); /* Set I2C1 SCL1 P0.20, SDA1 P0.19 as open drain pins. */

  /* ADC config */
  LPC_PINCON->PINMODE1 |= (2UL << 16) | (2UL << 14);    /* Disable pull-up on AD0.1 P0.24 and AD0.0 P0.23 pins.*/
  LPC_PINCON->PINSEL1 |= (1UL << 16) | (1UL << 14);     /* Set AD0.1 P0.24 and AD0.0 P0.23 pins.*/

}
