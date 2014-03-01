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

#if HAL_USE_MAC
/*
 * Board Ethernet pins configuration.
 * ENET_REF_CLK pin must be set before macInit().
 */
static void board_eth_pin_config(void) {

  /* Ethernet pin config  */
  LPC_PINCON->PINSEL2 |= (1UL << 30) | (1UL << 28) | (1UL << 20) | (1UL << 18) \
    | (1UL << 16) | (1UL << 8) | (1UL << 2) | (1UL << 0);    /* Set Ethernet ENET_REF_CLK P1.15, ENET_RX_ER P1.14,
                                                             ENET_RXD1 P1.10, ENET_RXD0 P1.9, ENET_CRS P1.8, ENET_TX_EN P1.4,
                                                             ENET_TXD1 P1.1, ENET_TXD0 P1.0 pins. */
  LPC_PINCON->PINMODE2 |= (2UL << 30) | (2UL << 28) | (2UL << 20) | (2UL << 18) \
    | (2UL << 16) | (2UL << 8) | (2UL << 2) | (2UL << 0);    /* Disable pull-up on ENET_REF_CLK P1.15, ENET_RX_ER P1.14,
                                                             ENET_RXD1 P1.10, ENET_RXD0 P1.9, ENET_CRS P1.8, ENET_TX_EN P1.4,
                                                             ENET_TXD1 P1.1, ENET_TXD0 P1.0 pins. */
  LPC_PINCON->PINSEL3 |= (1UL << 2) | (1UL << 0);           /* Set ENET_MDIO P1.17, ENET_MDC P1.16 */
  LPC_PINCON->PINMODE3 |= (2UL << 2) | (2UL << 0);          /* Disable pull-up on ENET_MDIO P1.17, ENET_MDC P1.16 */

}
#endif

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

  LPC17xx_clock_init();
#if HAL_USE_MAC
  board_eth_pin_config();
#endif
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  /*
   * Extra, board-specific, initializations.
   */
  /* UART0 pin config */
  LPC_PINCON->PINSEL0  |= (1UL << 4) | (1UL << 6);      /* Set UART0 TXD0 P0.2 and RXD0 P0.3 pins.*/
  LPC_PINCON->PINMODE0 |= (2UL << 4) | (2UL << 6);      /* Disable pull-up on UART0 TXD0 and RXD0 pins.*/

  /* CLKOUT pin config */
  /* LPC_PINCON->PINSEL3  |= (1UL << 22);  */           /* Set CLKOUT P1.27 pin.*/
  /* LPC_PINCON->PINMODE3 |= (2UL << 22);  */           /* Disable pull-up on CLKOUT pin. */

  /* I2C1 pin config */
  LPC_PINCON->PINSEL1 |= (3UL << 8) | (3UL << 6);       /* Set I2C1 SCL1 P0.20, SDA1 P0.19 pins. */
  LPC_PINCON->PINMODE1 |= (2UL << 8) | (2UL << 6);      /* Disable pull-up on I2C1 SCL1 P0.20, SDA1 P0.19 pins. */
  LPC_PINCON->PINMODE_OD0 |= (1UL << 20) | (1UL << 19); /* Set I2C1 SCL1 P0.20, SDA1 P0.19 as open drain pins. */

  /* DAC pin config */
  /* DAC pin set in dac_lld_start() */

#if HAL_USE_CAN
  /* CAN config i/o */
  LPC_PINCON->PINSEL0  |= (1UL << 2) | (1UL << 0);      /* Set CAN1 TD1 P0.1 and RD1 P0.0 pins. */
  LPC_PINCON->PINMODE0 |= (2UL << 2) | (2UL << 0);      /* Disable pull-up on TD1 and RD1 pins.*/

  LPC_PINCON->PINSEL0  |= (2UL << 10) | (2UL << 8);     /* Set CAN2 TD2 P0.5 and RD2 P0.4 pins. */
  LPC_PINCON->PINMODE0 |= (2UL << 10) | (2UL << 8);     /* Disable pull-up on TD1 and RD1 pins.*/
#endif

}
